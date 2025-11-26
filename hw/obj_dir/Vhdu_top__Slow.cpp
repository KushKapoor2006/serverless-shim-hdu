// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vhdu_top.h for the primary calling header

#include "Vhdu_top.h"
#include "Vhdu_top__Syms.h"

//==========

VL_CTOR_IMP(Vhdu_top) {
    Vhdu_top__Syms* __restrict vlSymsp = __VlSymsp = new Vhdu_top__Syms(this, name());
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vhdu_top::__Vconfigure(Vhdu_top__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-9);
    Verilated::timeprecision(-12);
}

Vhdu_top::~Vhdu_top() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vhdu_top::_settle__TOP__3(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_settle__TOP__3\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->s_axis_tready = 1U;
    vlTOPp->dispatch_slot = vlTOPp->hdu_top__DOT__alloc_slot_id;
    vlTOPp->dispatch_valid = ((IData)(vlTOPp->hdu_top__DOT__alloc_valid_out) 
                              & (IData)(vlTOPp->hdu_top__DOT__alloc_success));
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
    vlTOPp->host_irq = (0U != (IData)(vlTOPp->hdu_top__DOT__u_fallback__DOT__count));
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

void Vhdu_top::_eval_initial(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_eval_initial\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst_n = vlTOPp->rst_n;
}

void Vhdu_top::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::final\n"); );
    // Variables
    Vhdu_top__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vhdu_top::_eval_settle(Vhdu_top__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_eval_settle\n"); );
    Vhdu_top* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
}

void Vhdu_top::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vhdu_top::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst_n = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(512, s_axis_tdata);
    s_axis_tlast = VL_RAND_RESET_I(1);
    s_axis_tvalid = VL_RAND_RESET_I(1);
    s_axis_tready = VL_RAND_RESET_I(1);
    tb_inject_valid = VL_RAND_RESET_I(1);
    tb_inject_token = VL_RAND_RESET_Q(64);
    tb_inject_func = VL_RAND_RESET_I(16);
    dispatch_slot = VL_RAND_RESET_I(5);
    dispatch_valid = VL_RAND_RESET_I(1);
    compute_done_slot = VL_RAND_RESET_I(5);
    compute_done_valid = VL_RAND_RESET_I(1);
    cfg_addr = VL_RAND_RESET_I(4);
    cfg_wr_en = VL_RAND_RESET_I(1);
    cfg_func_id = VL_RAND_RESET_I(16);
    cfg_token = VL_RAND_RESET_Q(64);
    host_irq = VL_RAND_RESET_I(1);
    host_ready = VL_RAND_RESET_I(1);
    status_allocs = VL_RAND_RESET_I(32);
    status_fails = VL_RAND_RESET_I(32);
    status_overflows = VL_RAND_RESET_I(32);
    hdu_top__DOT__prs_func_id = VL_RAND_RESET_I(16);
    hdu_top__DOT__prs_token = VL_RAND_RESET_Q(64);
    hdu_top__DOT__prs_valid = VL_RAND_RESET_I(1);
    hdu_top__DOT__auth_pass = VL_RAND_RESET_I(1);
    hdu_top__DOT__auth_done = VL_RAND_RESET_I(1);
    hdu_top__DOT__alloc_success = VL_RAND_RESET_I(1);
    hdu_top__DOT__alloc_fail = VL_RAND_RESET_I(1);
    hdu_top__DOT__alloc_valid_out = VL_RAND_RESET_I(1);
    hdu_top__DOT__alloc_slot_id = VL_RAND_RESET_I(5);
    hdu_top__DOT__trigger_fallback = VL_RAND_RESET_I(1);
    hdu_top__DOT__u_parser__DOT__state = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            hdu_top__DOT__u_cam__DOT__cam_func_ids[__Vi0] = VL_RAND_RESET_I(16);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            hdu_top__DOT__u_cam__DOT__cam_tokens[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
            hdu_top__DOT__u_cam__DOT__cam_valid[__Vi0] = VL_RAND_RESET_I(1);
    }}
    hdu_top__DOT__u_cam__DOT__match_vector = VL_RAND_RESET_I(16);
    hdu_top__DOT__u_alloc__DOT__busy_bitmap = VL_RAND_RESET_I(32);
    hdu_top__DOT__u_alloc__DOT__next_free_slot = VL_RAND_RESET_I(5);
    hdu_top__DOT__u_alloc__DOT__found_free = VL_RAND_RESET_I(1);
    hdu_top__DOT__u_alloc__DOT__free_idx = VL_RAND_RESET_I(32);
    hdu_top__DOT__u_alloc__DOT__unnamedblk2__DOT__busy_next = VL_RAND_RESET_I(32);
    hdu_top__DOT__u_fallback__DOT__count = VL_RAND_RESET_I(3);
    hdu_top__DOT__u_fallback__DOT__count_n = VL_RAND_RESET_I(3);
    __Vdly__hdu_top__DOT__prs_valid = VL_RAND_RESET_I(1);
    __Vdly__hdu_top__DOT__u_parser__DOT__state = VL_RAND_RESET_I(1);
}
