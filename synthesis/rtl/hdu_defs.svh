// ============================================================
//  hdu_defs.svh  (Yosys-friendly global definitions)
// ============================================================

`define HDU_DATA_WIDTH     512
`define HDU_FUNC_ID_WIDTH  16
`define HDU_TOKEN_WIDTH    64
`define HDU_ARG_LEN_WIDTH  16
`define HDU_MAX_SLOTS      32

// Derived widths
`define HDU_SLOT_ID_WIDTH ((`HDU_MAX_SLOTS<=1) ? 1 : $clog2(`HDU_MAX_SLOTS))

// Header field bit positions
`define HDU_TOKEN_TOP (`HDU_DATA_WIDTH-1)
`define HDU_TOKEN_BOT (`HDU_DATA_WIDTH - `HDU_TOKEN_WIDTH)

`define HDU_FUNC_TOP  (`HDU_TOKEN_BOT - 1)
`define HDU_FUNC_BOT  (`HDU_TOKEN_BOT - `HDU_FUNC_ID_WIDTH)
