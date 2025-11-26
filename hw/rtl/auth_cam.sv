`timescale 1ns / 1ps
import hdu_pkg::*;

module auth_cam #(
    parameter int TABLE_SIZE = 16
)(
    input  logic clk,
    input  logic rst_n,

    // Config Interface
    input  logic [$clog2(TABLE_SIZE)-1:0] cfg_addr,
    input  logic                          cfg_wr_en,
    input  logic [FUNC_ID_WIDTH-1:0]      cfg_func_id,
    input  logic [TOKEN_WIDTH-1:0]        cfg_token,

    // Lookup Interface
    input  logic [FUNC_ID_WIDTH-1:0] lookup_func_id,
    input  logic [TOKEN_WIDTH-1:0]   lookup_token,
    input  logic                     lookup_valid,
    output logic                     lookup_ready,

    // Result
    output logic                     auth_pass,
    output logic                     auth_done
);

    // Storage
    logic [FUNC_ID_WIDTH-1:0] cam_func_ids [TABLE_SIZE-1:0];
    logic [TOKEN_WIDTH-1:0]   cam_tokens   [TABLE_SIZE-1:0];
    logic                     cam_valid    [TABLE_SIZE-1:0];

    // Reset/write
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (int i = 0; i < TABLE_SIZE; i++) begin
                cam_valid[i]    <= 1'b0;
                cam_func_ids[i] <= '0;
                cam_tokens[i]   <= '0;
            end
        end else if (cfg_wr_en) begin
            cam_func_ids[cfg_addr] <= cfg_func_id;
            cam_tokens[cfg_addr]   <= cfg_token;
            cam_valid[cfg_addr]    <= 1'b1;
        end
    end

    // Parallel match
    logic [TABLE_SIZE-1:0] match_vector;
    always_comb begin
        match_vector = '0;
        for (int i = 0; i < TABLE_SIZE; i++) begin
            if (cam_valid[i] &&
                (cam_func_ids[i] == lookup_func_id) &&
                (cam_tokens[i] == lookup_token)) begin
                match_vector[i] = 1'b1;
            end
        end
    end

    // register outputs (1 cycle)
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            auth_pass <= 1'b0;
            auth_done <= 1'b0;
        end else begin
            auth_done <= lookup_valid;
            auth_pass <= (|match_vector) && lookup_valid;
        end
    end

    assign lookup_ready = 1'b1;

    // simulation-only warning for duplicates
    // synthesis translate_off
    always_ff @(posedge clk) begin
        if (lookup_valid && $countones(match_vector) > 1)
            $warning("auth_cam: multiple matches for FuncID %h", lookup_func_id);
    end
    // synthesis translate_on

endmodule
