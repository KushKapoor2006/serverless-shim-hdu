// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vhdu_top.h for the primary calling header

#include "Vhdu_top.h"
#include "Vhdu_top__Syms.h"

//==========

void Vhdu_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vhdu_top::eval\n"); );
    Vhdu_top__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("rtl/hdu_top.sv", 4, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vhdu_top::_eval_initial_loop(Vhdu_top__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("rtl/hdu_top.sv", 4, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vhdu_top::_sequent__TOP__1(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_sequent__TOP__1\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*3:0*/ __Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_valid__v0;
    CData/*0:0*/ __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v0;
    CData/*0:0*/ __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v1;
    CData/*3:0*/ __Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0;
    CData/*0:0*/ __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0;
    CData/*0:0*/ __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v1;
    CData/*3:0*/ __Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_tokens__v0;
    CData/*0:0*/ __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v0;
    CData/*0:0*/ __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v1;
    SData/*15:0*/ __Vdlyvval__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0;
    QData/*63:0*/ __Vdlyvval__hdu_top__DOT__u_cam__DOT__cam_tokens__v0;
    // Body
    vlTOPp->__Vdly__hdu_top__DOT__u_parser__DOT__state 
        = vlTOPp->hdu_top__DOT__u_parser__DOT__state;
    vlTOPp->__Vdly__hdu_top__DOT__prs_valid = vlTOPp->hdu_top__DOT__prs_valid;
    __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v0 = 0U;
    __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v1 = 0U;
    __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v0 = 0U;
    __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v1 = 0U;
    __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0 = 0U;
    __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v1 = 0U;
    if (vlTOPp->rst_n) {
        if (((IData)(vlTOPp->hdu_top__DOT__trigger_fallback) 
             & (4U == (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count)))) {
            if ((0xffffffffU != vlTOPp->status_overflows)) {
                vlTOPp->status_overflows = ((IData)(1U) 
                                            + vlTOPp->status_overflows);
            }
        }
    } else {
        vlTOPp->status_overflows = 0U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->cfg_wr_en) {
            __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v0 = 1U;
            __Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_valid__v0 
                = vlTOPp->cfg_addr;
        }
    } else {
        __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v1 = 1U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->cfg_wr_en) {
            __Vdlyvval__hdu_top__DOT__u_cam__DOT__cam_tokens__v0 
                = vlTOPp->cfg_token;
            __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v0 = 1U;
            __Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_tokens__v0 
                = vlTOPp->cfg_addr;
        }
    } else {
        __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v1 = 1U;
    }
    if (vlTOPp->rst_n) {
        if (vlTOPp->cfg_wr_en) {
            __Vdlyvval__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0 
                = vlTOPp->cfg_func_id;
            __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0 = 1U;
            __Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0 
                = vlTOPp->cfg_addr;
        }
    } else {
        __Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v1 = 1U;
    }
    if (__Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v0) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[__Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_valid__v0] = 1U;
    }
    if (__Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_valid__v1) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[1U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[2U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[3U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[4U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[5U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[6U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[7U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[8U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[9U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0xaU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0xbU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0xcU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0xdU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0xeU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid[0xfU] = 0U;
    }
    if (__Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v0) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[__Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_tokens__v0] 
            = __Vdlyvval__hdu_top__DOT__u_cam__DOT__cam_tokens__v0;
    }
    if (__Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_tokens__v1) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[1U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[2U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[3U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[4U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[5U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[6U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[7U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[8U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[9U] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0xaU] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0xbU] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0xcU] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0xdU] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0xeU] = 0ULL;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens[0xfU] = 0ULL;
    }
    if (__Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[__Vdlyvdim0__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0] 
            = __Vdlyvval__hdu_top__DOT__u_cam__DOT__cam_func_ids__v0;
    }
    if (__Vdlyvset__hdu_top__DOT__u_cam__DOT__cam_func_ids__v1) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[1U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[2U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[3U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[4U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[5U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[6U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[7U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[8U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[9U] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0xaU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0xbU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0xcU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0xdU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0xeU] = 0U;
        vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids[0xfU] = 0U;
    }
}

VL_INLINE_OPT void Vhdu_top::_sequent__TOP__2(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_sequent__TOP__2\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (VL_UNLIKELY((4U < (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count)))) {
        VL_WRITEF("[%0t] %%Error: fallback_bridge.sv:91: Assertion failed in %Nhdu_top.u_fallback\n",
                  64,VL_TIME_UNITED_Q(1000),vlSymsp->name());
        VL_STOP_MT("rtl/fallback_bridge.sv", 91, "");
    }
    if (VL_UNLIKELY(((IData)(vlTOPp->hdu_top__DOT__prs_valid) 
                     & (1U < (0x1fU & VL_COUNTONES_I((IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector))))))) {
        VL_WRITEF("[%0t] %%Warning: auth_cam.sv:77: Assertion failed in %Nhdu_top.u_cam: auth_cam: multiple matches for FuncID %x\n",
                  64,VL_TIME_UNITED_Q(1000),vlSymsp->name(),
                  16,(IData)(vlTOPp->hdu_top__DOT__prs_func_id));
    }
    if (VL_UNLIKELY(((IData)(vlTOPp->hdu_top__DOT__alloc_success) 
                     & (IData)(vlTOPp->hdu_top__DOT__alloc_fail)))) {
        VL_WRITEF("[%0t] %%Error: slot_allocator.sv:107: Assertion failed in %Nhdu_top.u_alloc\n",
                  64,VL_TIME_UNITED_Q(1000),vlSymsp->name());
        VL_STOP_MT("rtl/slot_allocator.sv", 107, "");
    }
}

VL_INLINE_OPT void Vhdu_top::_sequent__TOP__4(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_sequent__TOP__4\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->hdu_top__DOT__u_fallback__DOT__count = 
        ((IData)(vlTOPp->rst_n) ? (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count_n)
          : 0U);
    if (vlTOPp->rst_n) {
        if (vlTOPp->tb_inject_valid) {
            vlTOPp->hdu_top__DOT__prs_token = vlTOPp->tb_inject_token;
            vlTOPp->hdu_top__DOT__prs_func_id = vlTOPp->tb_inject_func;
            vlTOPp->__Vdly__hdu_top__DOT__prs_valid = 1U;
            vlTOPp->__Vdly__hdu_top__DOT__u_parser__DOT__state = 0U;
        } else {
            if (vlTOPp->hdu_top__DOT__u_parser__DOT__state) {
                if (vlTOPp->hdu_top__DOT__u_parser__DOT__state) {
                    if (((IData)(vlTOPp->s_axis_tvalid) 
                         & (IData)(vlTOPp->s_axis_tlast))) {
                        vlTOPp->__Vdly__hdu_top__DOT__u_parser__DOT__state = 0U;
                    }
                }
            } else {
                if (((IData)(vlTOPp->s_axis_tvalid) 
                     & (IData)(vlTOPp->s_axis_tready))) {
                    vlTOPp->hdu_top__DOT__prs_token 
                        = (((QData)((IData)(vlTOPp->s_axis_tdata[0xfU])) 
                            << 0x20U) | (QData)((IData)(
                                                        vlTOPp->s_axis_tdata[0xeU])));
                    vlTOPp->hdu_top__DOT__prs_func_id 
                        = (0xffffU & ((vlTOPp->s_axis_tdata[0xeU] 
                                       << 0x10U) | 
                                      (vlTOPp->s_axis_tdata[0xdU] 
                                       >> 0x10U)));
                    vlTOPp->__Vdly__hdu_top__DOT__prs_valid = 1U;
                    if ((1U & (~ (IData)(vlTOPp->s_axis_tlast)))) {
                        vlTOPp->__Vdly__hdu_top__DOT__u_parser__DOT__state = 1U;
                    }
                }
                if (vlTOPp->hdu_top__DOT__prs_valid) {
                    vlTOPp->__Vdly__hdu_top__DOT__prs_valid = 0U;
                }
            }
        }
    } else {
        vlTOPp->__Vdly__hdu_top__DOT__prs_valid = 0U;
        vlTOPp->__Vdly__hdu_top__DOT__u_parser__DOT__state = 0U;
        vlTOPp->hdu_top__DOT__prs_func_id = 0U;
        vlTOPp->hdu_top__DOT__prs_token = 0ULL;
    }
    if (vlTOPp->rst_n) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next 
            = vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__free_idx 
            = vlTOPp->compute_done_slot;
        if (vlTOPp->compute_done_valid) {
            if (VL_LIKELY((VL_LTES_III(1,32,32, 0U, vlTOPp->hdu_top__DOT__u_alloc__DOT__free_idx) 
                           & VL_GTS_III(1,32,32, 0x20U, vlTOPp->hdu_top__DOT__u_alloc__DOT__free_idx)))) {
                vlTOPp->hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next 
                    = ((~ ((IData)(1U) << (0x1fU & vlTOPp->hdu_top__DOT__u_alloc__DOT__free_idx))) 
                       & vlTOPp->hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next);
            } else {
                VL_WRITEF("[%0t] %%Warning: slot_allocator.sv:75: Assertion failed in %Nhdu_top.u_alloc.unnamedblk2: slot_allocator: free_slot_id out of bounds: %0d\n",
                          64,VL_TIME_UNITED_Q(1000),
                          vlSymsp->name(),32,vlTOPp->hdu_top__DOT__u_alloc__DOT__free_idx);
            }
        }
        vlTOPp->hdu_top__DOT__alloc_valid_out = 0U;
        vlTOPp->hdu_top__DOT__alloc_success = 0U;
        vlTOPp->hdu_top__DOT__alloc_fail = 0U;
        if (((IData)(vlTOPp->hdu_top__DOT__auth_done) 
             & (IData)(vlTOPp->hdu_top__DOT__auth_pass))) {
            vlTOPp->hdu_top__DOT__alloc_valid_out = 1U;
            if (vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free) {
                vlTOPp->hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next 
                    = (vlTOPp->hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next 
                       | ((IData)(1U) << (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot)));
                if ((0xffffffffU != vlTOPp->status_allocs)) {
                    vlTOPp->status_allocs = ((IData)(1U) 
                                             + vlTOPp->status_allocs);
                }
                vlTOPp->hdu_top__DOT__alloc_slot_id 
                    = vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot;
                vlTOPp->hdu_top__DOT__alloc_success = 1U;
            } else {
                if ((0xffffffffU != vlTOPp->status_fails)) {
                    vlTOPp->status_fails = ((IData)(1U) 
                                            + vlTOPp->status_fails);
                }
                vlTOPp->hdu_top__DOT__alloc_fail = 1U;
            }
        }
        vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
            = vlTOPp->hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next;
    } else {
        vlTOPp->status_allocs = 0U;
        vlTOPp->status_fails = 0U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap = 0U;
        vlTOPp->hdu_top__DOT__alloc_valid_out = 0U;
        vlTOPp->hdu_top__DOT__alloc_success = 0U;
        vlTOPp->hdu_top__DOT__alloc_fail = 0U;
        vlTOPp->hdu_top__DOT__alloc_slot_id = 0U;
    }
    vlTOPp->hdu_top__DOT__u_parser__DOT__state = vlTOPp->__Vdly__hdu_top__DOT__u_parser__DOT__state;
    vlTOPp->host_irq = (0U != (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count));
    vlTOPp->dispatch_slot = vlTOPp->hdu_top__DOT__alloc_slot_id;
    vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0U;
    vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 0U;
    if ((1U & (~ vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 1U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 1U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 2U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 2U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 3U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 3U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 4U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 4U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 5U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 5U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 6U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 6U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 7U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 7U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 8U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 8U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 9U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 9U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0xaU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0xaU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0xbU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0xbU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0xcU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0xcU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0xdU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0xdU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0xeU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0xeU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0xfU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0xfU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x10U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x10U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x11U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x11U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x12U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x12U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x13U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x13U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x14U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x14U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x15U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x15U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x16U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x16U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x17U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x17U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x18U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x18U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x19U)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x19U;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x1aU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x1aU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x1bU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x1bU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x1cU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x1cU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x1dU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x1dU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x1eU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x1eU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    if ((1U & ((~ (vlTOPp->hdu_top__DOT__u_alloc__DOT__busy_bitmap 
                   >> 0x1fU)) & (~ (IData)(vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free))))) {
        vlTOPp->hdu_top__DOT__u_alloc__DOT__next_free_slot = 0x1fU;
        vlTOPp->hdu_top__DOT__u_alloc__DOT__found_free = 1U;
    }
    vlTOPp->dispatch_valid = ((IData)(vlTOPp->hdu_top__DOT__alloc_valid_out) 
                              & (IData)(vlTOPp->hdu_top__DOT__alloc_success));
    vlTOPp->hdu_top__DOT__auth_done = ((IData)(vlTOPp->rst_n) 
                                       & (IData)(vlTOPp->hdu_top__DOT__prs_valid));
    vlTOPp->hdu_top__DOT__auth_pass = ((IData)(vlTOPp->rst_n) 
                                       & ((0U != (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector)) 
                                          & (IData)(vlTOPp->hdu_top__DOT__prs_valid)));
    vlTOPp->hdu_top__DOT__prs_valid = vlTOPp->__Vdly__hdu_top__DOT__prs_valid;
    vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector = 0U;
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [0U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (1U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [1U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [1U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [1U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (2U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [2U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [2U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [2U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (4U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [3U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [3U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [3U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (8U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [4U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [4U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [4U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x10U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [5U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [5U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [5U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x20U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [6U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [6U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [6U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x40U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [7U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [7U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [7U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x80U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [8U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [8U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [8U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x100U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [9U] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                  [9U] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [9U] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x200U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0xaU] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                    [0xaU] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0xaU] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x400U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0xbU] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                    [0xbU] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0xbU] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x800U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0xcU] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                    [0xcU] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0xcU] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x1000U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0xdU] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                    [0xdU] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0xdU] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x2000U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0xeU] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                    [0xeU] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0xeU] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x4000U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    if (((vlTOPp->hdu_top__DOT__u_cam__DOT__cam_valid
          [0xfU] & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_func_ids
                    [0xfU] == (IData)(vlTOPp->hdu_top__DOT__prs_func_id))) 
         & (vlTOPp->hdu_top__DOT__u_cam__DOT__cam_tokens
            [0xfU] == vlTOPp->hdu_top__DOT__prs_token))) {
        vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector 
            = (0x8000U | (IData)(vlTOPp->hdu_top__DOT__u_cam__DOT__match_vector));
    }
    vlTOPp->hdu_top__DOT__trigger_fallback = (((IData)(vlTOPp->hdu_top__DOT__auth_done) 
                                               & (~ (IData)(vlTOPp->hdu_top__DOT__auth_pass))) 
                                              | ((IData)(vlTOPp->hdu_top__DOT__alloc_valid_out) 
                                                 & (IData)(vlTOPp->hdu_top__DOT__alloc_fail)));
}

VL_INLINE_OPT void Vhdu_top::_combo__TOP__5(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_combo__TOP__5\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->hdu_top__DOT__u_fallback__DOT__count_n 
        = vlTOPp->hdu_top__DOT__u_fallback__DOT__count;
    if (((IData)(vlTOPp->hdu_top__DOT__trigger_fallback) 
         & (4U != (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count)))) {
        vlTOPp->hdu_top__DOT__u_fallback__DOT__count_n 
            = (7U & ((IData)(1U) + (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count)));
    }
    if (((IData)(vlTOPp->host_ready) & (0U != (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count)))) {
        vlTOPp->hdu_top__DOT__u_fallback__DOT__count_n 
            = (7U & ((IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count) 
                     - (IData)(1U)));
    }
}

void Vhdu_top::_eval(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_eval\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
         | ((~ (IData)(vlTOPp->rst_n)) & (IData)(vlTOPp->__Vclklast__TOP__rst_n)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    if (((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
         | ((~ (IData)(vlTOPp->rst_n)) & (IData)(vlTOPp->__Vclklast__TOP__rst_n)))) {
        vlTOPp->_sequent__TOP__4(vlSymsp);
    }
    vlTOPp->_combo__TOP__5(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

VL_INLINE_OPT QData Vhdu_top::_change_request(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_change_request\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vhdu_top::_change_request_1(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_change_request_1\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vhdu_top::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((s_axis_tlast & 0xfeU))) {
        Verilated::overWidthError("s_axis_tlast");}
    if (VL_UNLIKELY((s_axis_tvalid & 0xfeU))) {
        Verilated::overWidthError("s_axis_tvalid");}
    if (VL_UNLIKELY((tb_inject_valid & 0xfeU))) {
        Verilated::overWidthError("tb_inject_valid");}
    if (VL_UNLIKELY((compute_done_slot & 0xe0U))) {
        Verilated::overWidthError("compute_done_slot");}
    if (VL_UNLIKELY((compute_done_valid & 0xfeU))) {
        Verilated::overWidthError("compute_done_valid");}
    if (VL_UNLIKELY((cfg_addr & 0xf0U))) {
        Verilated::overWidthError("cfg_addr");}
    if (VL_UNLIKELY((cfg_wr_en & 0xfeU))) {
        Verilated::overWidthError("cfg_wr_en");}
    if (VL_UNLIKELY((host_ready & 0xfeU))) {
        Verilated::overWidthError("host_ready");}
}
#endif  // VL_DEBUG
