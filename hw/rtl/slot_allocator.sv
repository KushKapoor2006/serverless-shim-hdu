`timescale 1ns / 1ps
import hdu_pkg::*;

module slot_allocator #(
    parameter int MAX_SLOTS_P = hdu_pkg::MAX_SLOTS
)(
    input  logic clk,
    input  logic rst_n,

    // Request
    input  logic req_valid,
    output logic req_ready,

    // Free
    input  logic [hdu_pkg::safe_clog2(MAX_SLOTS_P)-1:0] free_slot_id,
    input  logic                                      free_en,

    // Allocation
    output logic [hdu_pkg::safe_clog2(MAX_SLOTS_P)-1:0] alloc_slot_id,
    output logic                     alloc_success,
    output logic                     alloc_fail,
    output logic                     alloc_valid,

    // Telemetry
    output logic [31:0] cnt_allocations,
    output logic [31:0] cnt_failures
);

    localparam int SLOT_W = hdu_pkg::safe_clog2(MAX_SLOTS_P);

    logic [MAX_SLOTS_P-1:0] busy_bitmap;
    logic [SLOT_W-1:0]    next_free_slot;
    logic                 found_free;

    // module-scope temp for compatibility
    integer free_idx;

    // Priority encoder
    always_comb begin
        next_free_slot = '0;
        found_free = 1'b0;
        for (int unsigned i = 0; i < MAX_SLOTS_P; i++) begin
            if (!busy_bitmap[i] && !found_free) begin
                next_free_slot = SLOT_W'(i);
                found_free = 1'b1;
            end
        end
    end

    assign req_ready = 1'b1;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            busy_bitmap     <= '0;
            alloc_valid     <= 1'b0;
            alloc_success   <= 1'b0;
            alloc_fail      <= 1'b0;
            alloc_slot_id   <= '0;
            cnt_allocations <= '0;
            cnt_failures    <= '0;
        end else begin
            logic [MAX_SLOTS_P-1:0] busy_next;
            busy_next = busy_bitmap;

            // snapshot free index safely
            free_idx = $unsigned(free_slot_id);

            // Apply free (priority)
            if (free_en) begin
                if ((free_idx >= 0) && (free_idx < MAX_SLOTS_P)) begin
                    busy_next[free_idx] = 1'b0;
                end else begin
                    // simulation-only
                    // synthesis translate_off
                    $warning("slot_allocator: free_slot_id out of bounds: %0d", free_idx);
                    // synthesis translate_on
                end
            end

            // default strobes
            alloc_valid   <= 1'b0;
            alloc_success <= 1'b0;
            alloc_fail    <= 1'b0;

            if (req_valid) begin
                alloc_valid <= 1'b1;
                if (found_free) begin
                    busy_next[next_free_slot] = 1'b1;
                    alloc_slot_id <= next_free_slot;
                    alloc_success <= 1'b1;
                    if (cnt_allocations != 32'hFFFFFFFF)
                        cnt_allocations <= cnt_allocations + 1;
                end else begin
                    alloc_fail <= 1'b1;
                    if (cnt_failures != 32'hFFFFFFFF)
                        cnt_failures <= cnt_failures + 1;
                end
            end

            busy_bitmap <= busy_next;
        end
    end

    // simulation-only assertion
    // synthesis translate_off
    always_ff @(posedge clk) begin
        if (alloc_success && alloc_fail) $fatal("Alloc success and fail simultaneously!");
    end
    // synthesis translate_on

endmodule
