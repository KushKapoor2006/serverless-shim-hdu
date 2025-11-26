`timescale 1ns/1ps
`include "hdu_defs.svh"

module fallback_bridge #(
    parameter FIFO_DEPTH = 4
)(
    input logic clk,
    input logic rst_n,

    input logic fail_valid,
    input logic [`HDU_FUNC_ID_WIDTH-1:0] fail_func_id,
    input logic [`HDU_TOKEN_WIDTH-1:0]   fail_token,

    input  logic host_ready,
    output logic irq_valid,
    output logic [63:0] irq_data
);

    localparam PTR_W = (FIFO_DEPTH <= 1) ? 1 : $clog2(FIFO_DEPTH);

    logic [63:0] mem [FIFO_DEPTH-1:0];
    logic [PTR_W-1:0] wr_ptr, rd_ptr;
    logic [PTR_W:0] count;

    assign irq_valid = (count != 0);

    function automatic [63:0] pack(input logic [`HDU_FUNC_ID_WIDTH-1:0] f,
                                   input logic [`HDU_TOKEN_WIDTH-1:0] t);
        pack = {f, t[47:0]};
    endfunction

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= 0;
            rd_ptr <= 0;
            count <= 0;
        end else begin
            if (fail_valid && count < FIFO_DEPTH) begin
                mem[wr_ptr] <= pack(fail_func_id, fail_token);
                wr_ptr <= wr_ptr + 1;
                count <= count + 1;
            end

            if (host_ready && count > 0) begin
                irq_data <= mem[rd_ptr];
                rd_ptr <= rd_ptr + 1;
                count <= count - 1;
            end
        end
    end
endmodule
