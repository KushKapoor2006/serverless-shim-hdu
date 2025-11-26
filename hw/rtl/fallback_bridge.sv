`timescale 1ns / 1ps
import hdu_pkg::*;

module fallback_bridge (
    input  logic clk,
    input  logic rst_n,

    // Fallback Trigger
    input  logic                     fail_valid,
    input  logic [FUNC_ID_WIDTH-1:0] fail_func_id,
    input  logic [TOKEN_WIDTH-1:0]   fail_token,

    // Host Interface
    input  logic        host_ready,
    output logic        irq_valid,
    output logic [63:0] irq_data,
    
    // Telemetry
    output logic [31:0] cnt_overflows
);

    localparam int FIFO_DEPTH = 4;
    localparam int PTR_W      = hdu_pkg::safe_clog2(FIFO_DEPTH);
    localparam int DEPTH_W    = PTR_W + 1;

    logic [63:0] mem [FIFO_DEPTH-1:0];
    logic [PTR_W-1:0] wr_ptr, rd_ptr;
    logic [DEPTH_W-1:0] count; // width for count

    wire full  = (count == DEPTH_W'(FIFO_DEPTH));
    wire empty = (count == 0);

    // Compose a 64-bit debug word: [47:0] token_hash | [15:0] func_id
    wire [47:0] token_low  = {32'b0, fail_token[15:0]};
    wire [47:0] token_high = fail_token[63:16];
    wire [47:0] token_hash = token_low ^ token_high;
    wire [63:0] write_data = {token_hash, fail_func_id};

    // next-state temp
    logic [PTR_W-1:0] wr_ptr_n, rd_ptr_n;
    logic [DEPTH_W-1:0] count_n;
    logic [63:0] mem_n [FIFO_DEPTH-1:0];

    always_comb begin
        wr_ptr_n = wr_ptr;
        rd_ptr_n = rd_ptr;
        count_n  = count;
        for (int i = 0; i < FIFO_DEPTH; i++) mem_n[i] = mem[i];

        // write
        if (fail_valid && (count != DEPTH_W'(FIFO_DEPTH))) begin
            mem_n[wr_ptr] = write_data;
            wr_ptr_n = wr_ptr + 1;
            count_n  = count + 1;
        end

        // read (host consumes)
        if (host_ready && (count != 0)) begin
            rd_ptr_n = rd_ptr + 1;
            count_n  = count - 1;
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr        <= '0;
            rd_ptr        <= '0;
            count         <= '0;
            cnt_overflows <= '0;
            irq_data      <= '0;
            for (int i = 0; i < FIFO_DEPTH; i++) mem[i] <= '0;
        end else begin
            // overflow counting
            if (fail_valid && (count == DEPTH_W'(FIFO_DEPTH))) begin
                if (cnt_overflows != 32'hFFFFFFFF) cnt_overflows <= cnt_overflows + 1;
            end

            // commit
            for (int i = 0; i < FIFO_DEPTH; i++) mem[i] <= mem_n[i];
            wr_ptr <= wr_ptr_n;
            rd_ptr <= rd_ptr_n;
            count  <= count_n;

            if (count_n != 0) irq_data <= mem_n[rd_ptr];
            else irq_data <= '0;
        end
    end

    // synthesis translate_off
    always_ff @(posedge clk) begin
        if (count > DEPTH_W'(FIFO_DEPTH)) $fatal("fallback_bridge: count exceeded depth!");
    end
    // synthesis translate_on

    assign irq_valid = (count != 0);

endmodule
