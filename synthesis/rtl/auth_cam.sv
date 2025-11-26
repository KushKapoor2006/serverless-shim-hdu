`timescale 1ns/1ps
`include "hdu_defs.svh"

module auth_cam #(
    parameter TABLE_SIZE = 16
)(
    input  logic clk,
    input  logic rst_n,

    input  logic [$clog2(TABLE_SIZE)-1:0] cfg_addr,
    input  logic                          cfg_wr_en,
    input  logic [`HDU_FUNC_ID_WIDTH-1:0] cfg_func_id,
    input  logic [`HDU_TOKEN_WIDTH-1:0]   cfg_token,

    input  logic [`HDU_FUNC_ID_WIDTH-1:0] lookup_func_id,
    input  logic [`HDU_TOKEN_WIDTH-1:0]   lookup_token,
    input  logic                          lookup_valid,

    output logic                          lookup_ready,
    output logic                          auth_pass,
    output logic                          auth_done
);

    logic [`HDU_FUNC_ID_WIDTH-1:0] table_func [TABLE_SIZE-1:0];
    logic [`HDU_TOKEN_WIDTH-1:0]   table_tok  [TABLE_SIZE-1:0];

    assign lookup_ready = 1'b1;

    integer i;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            auth_pass <= 0;
            auth_done <= 0;
        end else begin
            auth_done <= 0;

            if (cfg_wr_en)
                {table_func[cfg_addr], table_tok[cfg_addr]} <= {cfg_func_id, cfg_token};

            if (lookup_valid) begin
                auth_pass <= 0;
                for (i = 0; i < TABLE_SIZE; i++)
                    if (table_func[i] == lookup_func_id &&
                        table_tok[i]  == lookup_token)
                        auth_pass <= 1;

                auth_done <= 1;
            end
        end
    end
endmodule
