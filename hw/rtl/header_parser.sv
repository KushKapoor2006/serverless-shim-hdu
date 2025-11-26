`timescale 1ns / 1ps
import hdu_pkg::*;

module header_parser #(
    parameter int DATA_WIDTH = hdu_pkg::DATA_WIDTH
)(
    input  logic                   clk,
    input  logic                   rst_n,

    // AXI-Stream Input
    input  logic [DATA_WIDTH-1:0]  s_axis_tdata,
    input  logic                   s_axis_tlast,
    input  logic                   s_axis_tvalid,
    output logic                   s_axis_tready,

    // Handshake with Downstream (Auth Stage)
    output logic [FUNC_ID_WIDTH-1:0] out_func_id,
    output logic [TOKEN_WIDTH-1:0]   out_token,
    output logic                     out_valid,
    input  logic                     out_ready,

    // Test-injection ports (for harness)
    input  logic                     tb_inject_valid,
    input  logic [TOKEN_WIDTH-1:0]   tb_inject_token,
    input  logic [FUNC_ID_WIDTH-1:0] tb_inject_func
);

    typedef enum logic {IDLE, DROP_PAYLOAD} state_t;
    state_t state;

    // Local token/func top indices (MSB-first)
    localparam int TOKEN_TOP = DATA_WIDTH - 1;
    localparam int FUNC_TOP  = TOKEN_TOP - TOKEN_WIDTH;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state       <= IDLE;
            out_valid   <= 1'b0;
            out_func_id <= '0;
            out_token   <= '0;
        end else begin
            // Test injection path has priority
            if (tb_inject_valid) begin
                out_token   <= tb_inject_token;
                out_func_id <= tb_inject_func;
                out_valid   <= 1'b1;
                // If injection used, we don't touch stream state
                state <= IDLE;
            end else begin
                case (state)
                    IDLE: begin
                        if (s_axis_tvalid && s_axis_tready) begin
                            // slice fields
                            out_token   <= s_axis_tdata[TOKEN_TOP -: TOKEN_WIDTH];
                            out_func_id <= s_axis_tdata[FUNC_TOP  -: FUNC_ID_WIDTH];
                            out_valid   <= 1'b1;

                            if (!s_axis_tlast)
                                state <= DROP_PAYLOAD;
                        end

                        // clear valid on downstream accept
                        if (out_valid && out_ready) begin
                            out_valid <= 1'b0;
                        end
                    end

                    DROP_PAYLOAD: begin
                        // consume payload beats until tlast
                        if (s_axis_tvalid && s_axis_tlast) begin
                            state <= IDLE;
                        end
                    end
                endcase
            end
        end
    end

    // Ready when we are dropping payload or we are not holding a stalled output
    assign s_axis_tready = (state == DROP_PAYLOAD) || (!out_valid || out_ready);

endmodule
