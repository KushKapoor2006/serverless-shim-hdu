// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VHDU_TOP_H_
#define _VHDU_TOP_H_  // guard

#include "verilated_heavy.h"

//==========

class Vhdu_top__Syms;

//----------

VL_MODULE(Vhdu_top) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(s_axis_tlast,0,0);
    VL_IN8(s_axis_tvalid,0,0);
    VL_OUT8(s_axis_tready,0,0);
    VL_IN8(tb_inject_valid,0,0);
    VL_OUT8(dispatch_slot,4,0);
    VL_OUT8(dispatch_valid,0,0);
    VL_IN8(compute_done_slot,4,0);
    VL_IN8(compute_done_valid,0,0);
    VL_IN8(cfg_addr,3,0);
    VL_IN8(cfg_wr_en,0,0);
    VL_OUT8(host_irq,0,0);
    VL_IN8(host_ready,0,0);
    VL_IN16(tb_inject_func,15,0);
    VL_IN16(cfg_func_id,15,0);
    VL_INW(s_axis_tdata,511,0,16);
    VL_OUT(status_allocs,31,0);
    VL_OUT(status_fails,31,0);
    VL_OUT(status_overflows,31,0);
    VL_IN64(tb_inject_token,63,0);
    VL_IN64(cfg_token,63,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*0:0*/ hdu_top__DOT__prs_valid;
    CData/*0:0*/ hdu_top__DOT__auth_pass;
    CData/*0:0*/ hdu_top__DOT__auth_done;
    CData/*0:0*/ hdu_top__DOT__alloc_success;
    CData/*0:0*/ hdu_top__DOT__alloc_fail;
    CData/*0:0*/ hdu_top__DOT__alloc_valid_out;
    CData/*4:0*/ hdu_top__DOT__alloc_slot_id;
    CData/*0:0*/ hdu_top__DOT__trigger_fallback;
    CData/*0:0*/ hdu_top__DOT__u_parser__DOT__state;
    CData/*4:0*/ hdu_top__DOT__u_alloc__DOT__next_free_slot;
    CData/*0:0*/ hdu_top__DOT__u_alloc__DOT__found_free;
    CData/*2:0*/ hdu_top__DOT__u_fallback__DOT__count;
    CData/*2:0*/ hdu_top__DOT__u_fallback__DOT__count_n;
    SData/*15:0*/ hdu_top__DOT__prs_func_id;
    SData/*15:0*/ hdu_top__DOT__u_cam__DOT__match_vector;
    IData/*31:0*/ hdu_top__DOT__u_alloc__DOT__busy_bitmap;
    IData/*31:0*/ hdu_top__DOT__u_alloc__DOT__free_idx;
    IData/*31:0*/ hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next;
    QData/*63:0*/ hdu_top__DOT__prs_token;
    SData/*15:0*/ hdu_top__DOT__u_cam__DOT__cam_func_ids[16];
    QData/*63:0*/ hdu_top__DOT__u_cam__DOT__cam_tokens[16];
    CData/*0:0*/ hdu_top__DOT__u_cam__DOT__cam_valid[16];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vdly__hdu_top__DOT__prs_valid;
    CData/*0:0*/ __Vdly__hdu_top__DOT__u_parser__DOT__state;
    CData/*0:0*/ __Vclklast__TOP__clk;
    CData/*0:0*/ __Vclklast__TOP__rst_n;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vhdu_top__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vhdu_top);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vhdu_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vhdu_top();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vhdu_top__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vhdu_top__Syms* symsp, bool first);
  private:
    static QData _change_request(Vhdu_top__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vhdu_top__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__5(Vhdu_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vhdu_top__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vhdu_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vhdu_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vhdu_top__Syms* __restrict vlSymsp);
    static void _sequent__TOP__2(Vhdu_top__Syms* __restrict vlSymsp);
    static void _sequent__TOP__4(Vhdu_top__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vhdu_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
