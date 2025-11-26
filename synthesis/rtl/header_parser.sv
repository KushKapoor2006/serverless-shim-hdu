`timescale 1ns/1ps
`include "hdu_defs.svh"

module header_parser #(
    parameter DATA_W  = `HDU_DATA_WIDTH,
    parameter FUNC_W  = `HDU_FUNC_ID_WIDTH,
    parameter TOKEN_W = `HDU_TOKEN_WIDTH
)(
    input  logic                  clk,
    input  logic                  rst_n,
    input  logic                  in_valid,
    input  logic [DATA_W-1:0]     in_data,

    output logic                  out_valid,
    output logic [FUNC_W-1:0]     out_func_id,
    output logic [TOKEN_W-1:0]    out_token
);

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            out_valid   <= 0;
            out_func_id <= 0;
            out_token   <= 0;
        end else begin
            out_valid   <= in_valid;
            out_func_id <= in_data[`HDU_FUNC_TOP  : `HDU_FUNC_BOT];
            out_token   <= in_data[`HDU_TOKEN_TOP : `HDU_TOKEN_BOT];
        end
    end
endmodule
