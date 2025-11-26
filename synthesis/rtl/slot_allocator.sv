`timescale 1ns/1ps
`include "hdu_defs.svh"

module slot_allocator #(
    parameter MAX_SLOTS_P = `HDU_MAX_SLOTS
)(
    input  logic clk,
    input  logic rst_n,

    input  logic req_valid,
    output logic req_ready,

    input  logic [`HDU_SLOT_ID_WIDTH-1:0] free_slot_id,
    input  logic                          free_en,

    output logic [`HDU_SLOT_ID_WIDTH-1:0] alloc_slot_id,
    output logic                          alloc_success,
    output logic                          alloc_fail,
    output logic                          alloc_valid
);

    localparam SLOT_W = `HDU_SLOT_ID_WIDTH;

    logic [MAX_SLOTS_P-1:0] busy;
    logic found;  // ⭐ moved here (module scope, not inside always)

    assign req_ready = 1'b1;

    integer i;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            busy          <= '0;
            alloc_slot_id <= '0;
            alloc_success <= 1'b0;
            alloc_fail    <= 1'b0;
            alloc_valid   <= 1'b0;
        end else begin

            // Default outputs each cycle
            alloc_success <= 1'b0;
            alloc_fail    <= 1'b0;
            alloc_valid   <= 1'b0;

            // Free a slot if instructed
            if (free_en)
                busy[free_slot_id] <= 1'b0;

            // Allocation request
            if (req_valid) begin
                found = 1'b0;  // reset flag

                for (i = 0; i < MAX_SLOTS_P; i++) begin
                    if (!busy[i] && !found) begin
                        busy[i]        <= 1'b1;
                        alloc_slot_id  <= i[SLOT_W-1:0];
                        alloc_success  <= 1'b1;
                        alloc_valid    <= 1'b1;
                        found          <= 1'b1;
                    end
                end

                if (!found) begin
                    alloc_fail  <= 1'b1;
                    alloc_valid <= 1'b1;
                end
            end
        end
    end

endmodule
