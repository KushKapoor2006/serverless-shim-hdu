// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef _VHDU_TOP__SYMS_H_
#define _VHDU_TOP__SYMS_H_  // guard

#include "verilated_heavy.h"

// INCLUDE MODULE CLASSES
#include "Vhdu_top.h"
#include "Vhdu_top___024unit.h"

// SYMS CLASS
class Vhdu_top__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool __Vm_didInit;
    
    // SUBCELL STATE
    Vhdu_top*                      TOPp;
    
    // SCOPE NAMES
    VerilatedScope __Vscope_hdu_top__u_alloc;
    VerilatedScope __Vscope_hdu_top__u_alloc__unnamedblk2;
    VerilatedScope __Vscope_hdu_top__u_cam;
    VerilatedScope __Vscope_hdu_top__u_fallback;
    
    // CREATORS
    Vhdu_top__Syms(Vhdu_top* topp, const char* namep);
    ~Vhdu_top__Syms() {}
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
