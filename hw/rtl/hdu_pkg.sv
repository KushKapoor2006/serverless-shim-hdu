`timescale 1ns / 1ps
package hdu_pkg;
    // ---------------------------------------------------------
    // SYSTEM CONSTANTS
    // ---------------------------------------------------------
    localparam int DATA_WIDTH    = 512; // bits (64 bytes)
    localparam int FUNC_ID_WIDTH = 16;
    localparam int TOKEN_WIDTH   = 64;
    localparam int ARG_LEN_WIDTH = 16;
    localparam int MAX_SLOTS     = 32;

    // Safety for 1-slot case (log2(1) = 0)
    function automatic int safe_clog2(int n);
        return (n <= 1) ? 1 : $clog2(n);
    endfunction

    localparam int SLOT_ID_WIDTH = safe_clog2(MAX_SLOTS);

    // Header bits used
    localparam int HEADER_USED_BITS = TOKEN_WIDTH + FUNC_ID_WIDTH + ARG_LEN_WIDTH;
    localparam int PADDING_WIDTH    = DATA_WIDTH - HEADER_USED_BITS;

    // NOTE:
    // Removed the 'initial' simulation check from the package because Verilator
    // rejects package-scoped initial blocks. If you still want simulation-only
    // checks, create a small simulation module with synthesis translate_off.
    // e.g.:
    //
    //   // synthesis translate_off
    //   module hdu_pkg_sim_checks();
    //       initial begin
    //           if (PADDING_WIDTH < 0) $error("Header fields exceed DATA_WIDTH!");
    //       end
    //   endmodule
    //   // synthesis translate_on

    // Helper index getters (MSB-first layout)
    function automatic int get_token_top(); return DATA_WIDTH - 1; endfunction
    function automatic int get_token_bot(); return DATA_WIDTH - TOKEN_WIDTH; endfunction
    function automatic int get_func_top();  return get_token_bot() - 1; endfunction
    function automatic int get_func_bot();  return get_token_bot() - FUNC_ID_WIDTH; endfunction

    // Dispatch descriptor (used by allocator/dispatch)
    typedef struct packed {
        logic [FUNC_ID_WIDTH-1:0] func_id;
        logic [SLOT_ID_WIDTH-1:0] slot_id;
        logic                     valid;
    } dispatch_desc_t;

endpackage : hdu_pkg
