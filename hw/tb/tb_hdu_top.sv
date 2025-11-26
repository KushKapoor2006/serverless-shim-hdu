// tb_hdu_top.sv
// Unit test for the HDU — now adds extra event logging for header_parsed and hdu_dispatch.

`timescale 1ns / 1ps
import hdu_pkg::*;

module tb_hdu_top;

    // ---------------------------------------------------------
    // TEST CONFIG (small sizes to saturate quickly)
    // ---------------------------------------------------------
    localparam int TABLE_SIZE_P = 4;
    localparam int MAX_SLOTS_P  = 4;

    localparam int SLOT_W = hdu_pkg::safe_clog2(MAX_SLOTS_P);
    localparam int ADDR_W = hdu_pkg::safe_clog2(TABLE_SIZE_P);

    // ---------------------------------------------------------
    // DUT SIGNALS
    // ---------------------------------------------------------
    logic clk, rst_n;

    // AXI-Stream
    logic [DATA_WIDTH-1:0] s_axis_tdata;
    logic                  s_axis_tlast;
    logic                  s_axis_tvalid;
    logic                  s_axis_tready;

    // Dispatch
    logic [SLOT_W-1:0]     dispatch_slot;
    logic                  dispatch_valid;
    logic [SLOT_W-1:0]     compute_done_slot;
    logic                  compute_done_valid;

    // Config & Host
    logic [ADDR_W-1:0]     cfg_addr;
    logic                  cfg_wr_en;
    logic [FUNC_ID_WIDTH-1:0] cfg_func_id;
    logic [TOKEN_WIDTH-1:0]   cfg_token;

    logic                  host_ready;
    logic                  host_irq;

    // Telemetry
    logic [31:0] status_allocs, status_fails, status_overflows;

    // File handle for extra events
    integer log_fh;

    // ---------------------------------------------------------
    // INSTANTIATE DUT
    // ---------------------------------------------------------
    hdu_top #(
        .TABLE_SIZE(TABLE_SIZE_P)
    ) dut (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(s_axis_tdata), .s_axis_tlast(s_axis_tlast),
        .s_axis_tvalid(s_axis_tvalid), .s_axis_tready(s_axis_tready),
        .dispatch_slot(dispatch_slot), .dispatch_valid(dispatch_valid),
        .compute_done_slot(compute_done_slot), .compute_done_valid(compute_done_valid),
        .cfg_addr(cfg_addr), .cfg_wr_en(cfg_wr_en),
        .cfg_func_id(cfg_func_id), .cfg_token(cfg_token),
        .host_ready(host_ready), .host_irq(host_irq),
        .status_allocs(status_allocs), .status_fails(status_fails), .status_overflows(status_overflows)
    );

    // ---------------------------------------------------------
    // CLOCK (200 MHz -> 5 ns period)
    // ---------------------------------------------------------
    initial begin
        clk = 0;
        forever #2.5 clk = ~clk;
    end

    // ---------------------------------------------------------
    // OPEN the extra events log (appends) — path relative to hw/ (where `make run` runs)
    // This will create repo_root/evaluation/logs/rtl_events_extra.csv
    // ---------------------------------------------------------
    initial begin
        // wait a few cycles for simulator start
        repeat(5) @(posedge clk);

        // create directory (best effort)
        $system("mkdir -p ../evaluation/logs");
        log_fh = $fopen("../evaluation/logs/rtl_events_extra.csv", "a");
        if (log_fh == 0) begin
            $display("[TB] Failed to open extra events file");
        end else begin
            // add header comment (optional)
            $fwrite(log_fh, "# extra RTL events (header_parsed/hdu_dispatch)\n");
        end
    end

    // ---------------------------------------------------------
    // Helper tasks
    // ---------------------------------------------------------
    task sys_reset();
        begin
            rst_n = 0;
            s_axis_tdata = '0;
            s_axis_tlast = 0;
            s_axis_tvalid = 0;
            compute_done_valid = 0;
            cfg_wr_en = 0;
            host_ready = 1;
            repeat(10) @(posedge clk);
            rst_n = 1;
            repeat(5) @(posedge clk);
        end
    endtask

    task config_cam(input int idx, input int fid, input logic [63:0] token);
        begin
            @(posedge clk);
            cfg_addr    <= idx[ADDR_W-1:0];
            cfg_func_id <= fid[15:0];
            cfg_token   <= token;
            cfg_wr_en   <= 1;
            @(posedge clk);
            cfg_wr_en   <= 0;
            $display("[TB] Configured CAM[%0d]: FuncID=%0h, Token=%0h", idx, fid, token);
        end
    endtask

    // Send network packet (single-beat)
    // This task logs header_parsed when the AXI handshake occurs (s_axis_tvalid && s_axis_tready)
    task send_packet(input int fid, input logic [63:0] token, input int arg_len);
        logic [DATA_WIDTH-1:0] pkt;
        integer cycle_when_handshake;
        begin
            pkt = '0;
            // Pack token and func_id into the top bits according to hdu_pkg helpers
            pkt[hdu_pkg::get_token_top() -: TOKEN_WIDTH] = token;
            pkt[hdu_pkg::get_func_top()  -: FUNC_ID_WIDTH] = fid[15:0];

            // Wait until DUT ready, then assert valid for one cycle
            @(posedge clk);
            wait (s_axis_tready == 1);
            // Drive values and assert valid
            s_axis_tdata  <= pkt;
            s_axis_tvalid <= 1;
            s_axis_tlast  <= 1;

            // On the same posedge where handshake occurs (s_axis_tvalid && s_axis_tready),
            // the header has been captured by parser. Log header_parsed event.
            @(posedge clk);
            if (s_axis_tvalid && s_axis_tready) begin
                // compute cycle = $time / 5 ns
                cycle_when_handshake = $time / 5;
                if (log_fh != 0) $fwrite(log_fh, "header_parsed,%0d,%0t\n", cycle_when_handshake, $time);
                $display("[TB] header_parsed at cycle=%0d (ns=%0t)", cycle_when_handshake, $time);
            end

            // Deassert valid next cycle
            @(posedge clk);
            s_axis_tvalid <= 0;
            s_axis_tdata  <= '0;
            s_axis_tlast  <= 0;
        end
    endtask

    task free_slot(input int slot);
        begin
            @(posedge clk);
            compute_done_slot  <= slot[SLOT_W-1:0];
            compute_done_valid <= 1;
            @(posedge clk);
            compute_done_valid <= 0;
            $display("[TB] Freed Slot %0d", slot);
        end
    endtask

    // Log hdu_dispatch events when dispatch_valid asserted
    always @(posedge clk) begin
        if (dispatch_valid) begin
            integer cyc;
            cyc = $time / 5;
            if (log_fh != 0) $fwrite(log_fh, "hdu_dispatch,%0d,%0t,%0d\n", cyc, $time, dispatch_slot);
            $display("[TB] hdu_dispatch at cycle=%0d slot=%0d (ns=%0t)", cyc, dispatch_slot, $time);
        end
    end

    // Also log fallback events (host_irq) for easier pairing (some harnesses already record)
    always @(posedge clk) begin
        if (host_irq) begin
            integer cyc;
            cyc = $time / 5;
            if (log_fh != 0) $fwrite(log_fh, "auth_fallback,%0d,%0t\n", cyc, $time);
            $display("[TB] auth_fallback at cycle=%0d (ns=%0t)", cyc, $time);
        end
    end

    // ---------------------------------------------------------
    // TEST SCENARIOS
    // ---------------------------------------------------------
    initial begin
        $timeformat(-9, 2, " ns", 10);

        $display("===== START HDU TB (with extra logging) =====");

        sys_reset();

        // Test 1: configure and send 1 packet that should dispatch
        config_cam(0, 16'hAAAA, 64'h1122334455667788);
        send_packet(16'hAAAA, 64'h1122334455667788, 128);

        // Wait a little for dispatch
        repeat(20) @(posedge clk);

        // Test 2: auth failure -> fallback
        send_packet(16'hAAAA, 64'hDEADBEEFDEADBEEF, 128);
        repeat(20) @(posedge clk);

        // Test 3: fill slots and saturate
        config_cam(1, 16'hBBBB, 64'hAABBCCDDEEFF0011);
        repeat (3) begin
            send_packet(16'hBBBB, 64'hAABBCCDDEEFF0011, 64);
            @(posedge clk);
        end
        repeat(20) @(posedge clk);

        // Send another to overflow -> should fallback
        send_packet(16'hBBBB, 64'hAABBCCDDEEFF0011, 64);
        repeat(20) @(posedge clk);

        // Race condition: free slot and send same time
        fork
            begin
                send_packet(16'hBBBB, 64'hAABBCCDDEEFF0011, 64);
            end
            begin
                repeat(2) @(posedge clk);
                free_slot(0);
            end
        join

        repeat(20) @(posedge clk);

        $display("===== TB COMPLETE =====");
        if (log_fh != 0) $fclose(log_fh);
        $finish;
    end

endmodule
