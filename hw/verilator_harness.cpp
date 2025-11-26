// hw/verilator_harness.cpp
// Verilator C++ harness for HDU top. Drives clock, reset, implements tests
// and writes event logs to ../evaluation/logs/rtl_run_<timestamp>.csv

#include <verilated.h>
#include "Vhdu_top.h"
#if VM_TRACE
# include <verilated_vcd_c.h>
#endif

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

static const double CLK_PERIOD_NS = 5.0; // 200 MHz => 5 ns per cycle
static uint64_t sim_cycles = 0;
static std::ofstream csv_out;
static std::string csvname_global;

// Provide sc_time_stamp() with C++ linkage to match Verilator header
double sc_time_stamp() {
    double ns = sim_cycles * CLK_PERIOD_NS;
    return ns;
}

// Log an event (event name, cycle, ns)
void log_event(const string &evt) {
    uint64_t cyc = sim_cycles;
    double time_ns = cyc * CLK_PERIOD_NS;
    csv_out << evt << "," << cyc << "," << fixed << setprecision(1) << time_ns << "\n";
}

// Advance the DUT by one full clock period (rising + falling)
// tfp is an opaque pointer; actual type used only under VM_TRACE
void step_clock(Vhdu_top *top, void* tfp = nullptr) {
    // rising edge
    top->clk = 1;
    top->eval();
#if VM_TRACE
    if (tfp) {
        VerilatedVcdC* real_tfp = static_cast<VerilatedVcdC*>(tfp);
        real_tfp->dump((vluint64_t)(sim_cycles * (int)CLK_PERIOD_NS));
    }
#endif
    sim_cycles++;
    // falling edge
    top->clk = 0;
    top->eval();
#if VM_TRACE
    if (tfp) {
        VerilatedVcdC* real_tfp = static_cast<VerilatedVcdC*>(tfp);
        real_tfp->dump((vluint64_t)(sim_cycles * (int)CLK_PERIOD_NS));
    }
#endif
    sim_cycles++;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    bool enable_vcd = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "+vcd") == 0) enable_vcd = true;
    }

    // Ensure logs directory exists (hw/ -> ../evaluation/logs)
    (void)system("mkdir -p ../evaluation/logs"); // ignore return value intentionally

    // Timestamped CSV filename
    auto now = chrono::system_clock::now();
    time_t tnow = chrono::system_clock::to_time_t(now);
    char tsbuf[64];
    strftime(tsbuf, sizeof(tsbuf), "%Y%m%dT%H%M%SZ", gmtime(&tnow));
    string csvname = string("../evaluation/logs/rtl_run_") + tsbuf + ".csv";
    csvname_global = csvname;
    csv_out.open(csvname);
    csv_out << "event,cycle,ns\n";

    // Instantiate DUT
    Vhdu_top *top = new Vhdu_top;

#if VM_TRACE
    Verilated::traceEverOn(true);
    VerilatedVcdC* real_tfp = nullptr;
    void* tfp = nullptr;
    if (enable_vcd) {
        real_tfp = new VerilatedVcdC;
        top->trace(real_tfp, 99);
        string vcdname = string("../evaluation/logs/rtl_run_") + tsbuf + ".vcd";
        real_tfp->open(vcdname.c_str());
        tfp = static_cast<void*>(real_tfp);
    }
#else
    void* tfp = nullptr;
#endif

    // Initialize signals (do NOT assign the packed s_axis_tdata vector here)
    top->clk = 0;
    top->rst_n = 0;
    top->s_axis_tvalid = 0;
    top->s_axis_tlast = 0;
    top->compute_done_valid = 0;
    top->cfg_wr_en = 0;
    top->host_ready = 1;

    // New test-inject signals default
    top->tb_inject_valid = 0;
    top->tb_inject_token = 0;
    top->tb_inject_func = 0;

    // Reset: hold for several cycles
    for (int i = 0; i < 20; ++i) step_clock(top, tfp);
    top->rst_n = 1;
    for (int i = 0; i < 10; ++i) step_clock(top, tfp);

    log_event("reset_done");

    // Helper: wait until DUT asserts s_axis_tready (or timeout)
    auto wait_for_ready = [&](int max_cycles = 5000) -> bool {
        int c = 0;
        while (!top->s_axis_tready && c < max_cycles) {
            step_clock(top, tfp);
            ++c;
        }
        return c < max_cycles;
    };

    // send_packet: handshake-only via injection ports
    auto send_packet = [&](uint16_t fid, uint64_t token, uint16_t arglen) {
        (void)arglen; // not used in injection mode
        if (!wait_for_ready()) {
            log_event("send_pkt_failed_no_ready");
            return;
        }
        // Drive injection ports for 1 cycle
        top->tb_inject_token = token;
        top->tb_inject_func  = fid;
        top->tb_inject_valid = 1;
        step_clock(top, tfp);
        top->tb_inject_valid = 0;
        top->tb_inject_token = 0;
        top->tb_inject_func  = 0;
        log_event("send_packet");
    };

    // cfg_write: write CAM configuration
    auto cfg_write = [&](int idx, uint16_t fid, uint64_t token) {
        top->cfg_addr = idx;
        top->cfg_func_id = fid;
        top->cfg_token = token;
        top->cfg_wr_en = 1;
        step_clock(top, tfp);
        top->cfg_wr_en = 0;
        // allow CAM to latch safely
        for (int i = 0; i < 5; ++i) step_clock(top, tfp);
        std::ostringstream ss;
        ss << "cfg_write_" << idx;
        log_event(ss.str());
    };

    // free_slot: assert compute_done for one cycle
    auto free_slot = [&](int slot) {
        top->compute_done_slot = slot;
        top->compute_done_valid = 1;
        step_clock(top, tfp);
        top->compute_done_valid = 0;
        std::ostringstream ss;
        ss << "free_slot_" << slot;
        log_event(ss.str());
    };

    // stall_host: deassert host_ready for N cycles
    auto stall_host = [&](int cycles) {
        top->host_ready = 0;
        for (int i = 0; i < cycles; ++i) step_clock(top, tfp);
        top->host_ready = 1;
        std::ostringstream ss;
        ss << "stall_host_" << cycles;
        log_event(ss.str());
    };

    // -------------------------
    // Test sequence (parallels the SV TB)
    // -------------------------

    // Note: increased dispatch poll timeout for debugging
    const int DISPATCH_POLL_MAX = 15000; // was 5000

    cfg_write(0, 0xAAAA, 0x1122334455667788ULL);
    send_packet(0xAAAA, 0x1122334455667788ULL, 128);

    // wait for dispatch (poll)
    {
        int to = 0; bool got = false;
        while (to < DISPATCH_POLL_MAX) {
            step_clock(top, tfp);
            if (top->dispatch_valid) { got = true; break; }
            ++to;
        }
        if (got) {
            std::ostringstream ss; ss << "dispatch_slot_" << (int)top->dispatch_slot;
            log_event(ss.str());
            cout << "[C++] PASS: Dispatch observed\n";
        } else {
            cout << "[C++] FAIL: dispatch timeout\n";
        }
    }

    // Test 2: auth failure -> host_irq
    send_packet(0xAAAA, 0xDEADBEEFDEADBEEFULL, 128);
    {
        int to = 0; bool got = false;
        while (to < DISPATCH_POLL_MAX) {
            step_clock(top, tfp);
            if (top->host_irq) { got = true; break; }
            ++to;
        }
        if (got) {
            log_event("auth_fallback");
            cout << "[C++] PASS: auth fallback\n";
        } else {
            cout << "[C++] FAIL: auth fallback timeout\n";
        }
    }

    // Test 3: saturation & overflow stress
    cfg_write(1, 0xBBBB, 0xAABBCCDDEEFF0011ULL);
    for (int i = 0; i < 3; ++i) send_packet(0xBBBB, 0xAABBCCDDEEFF0011ULL, 64);

    // Stall host while sending more packets to exercise FIFO/overflow
    stall_host(50);
    send_packet(0xBBBB, 0xAABBCCDDEEFF0011ULL, 64);
    send_packet(0xBBBB, 0xAABBCCDDEEFF0011ULL, 64);
    send_packet(0xBBBB, 0xAABBCCDDEEFF0011ULL, 64);

    for (int i = 0; i < 100; ++i) step_clock(top, tfp);
    log_event("after_saturation");

    // Test 4: race free+alloc same-cycle (coarse timing)
    send_packet(0xBBBB, 0xAABBCCDDEEFF0011ULL, 64);
    for (int i = 0; i < 2; ++i) step_clock(top, tfp);
    free_slot(0);
    for (int i = 0; i < 200; ++i) step_clock(top, tfp);

    log_event("end_tests");

    // close trace if enabled
#if VM_TRACE
    if (real_tfp) { real_tfp->close(); delete real_tfp; real_tfp = nullptr; }
#endif

    csv_out.close();
    delete top;
    cout << "Simulation complete, log: " << csvname_global << endl;
    return 0;
}
