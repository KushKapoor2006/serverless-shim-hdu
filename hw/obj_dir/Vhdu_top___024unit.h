// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vhdu_top.h for the primary calling header

#ifndef _VHDU_TOP___024UNIT_H_
#define _VHDU_TOP___024UNIT_H_  // guard

#include "verilated_heavy.h"

//==========

class Vhdu_top__Syms;

//----------

VL_MODULE(Vhdu_top___024unit) {
  public:
    
    // INTERNAL VARIABLES
  private:
    Vhdu_top__Syms* __VlSymsp;  // Symbol table
  public:
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vhdu_top___024unit);  ///< Copying not allowed
  public:
    Vhdu_top___024unit(const char* name = "TOP");
    ~Vhdu_top___024unit();
    
    // INTERNAL METHODS
    void __Vconfigure(Vhdu_top__Syms* symsp, bool first);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
