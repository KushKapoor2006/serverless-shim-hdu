// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vhdu_top__Syms.h"
#include "Vhdu_top.h"
#include "Vhdu_top___024unit.h"



// FUNCTIONS
Vhdu_top__Syms::Vhdu_top__Syms(Vhdu_top* topp, const char* namep)
    // Setup locals
    : __Vm_namep(namep)
    , __Vm_didInit(false)
    // Setup submodule names
{
    // Pointer to top level
    TOPp = topp;
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOPp->__Vconfigure(this, true);
    // Setup scopes
    __Vscope_hdu_top__u_alloc.configure(this, name(), "hdu_top.u_alloc", "u_alloc", -9, VerilatedScope::SCOPE_OTHER);
    __Vscope_hdu_top__u_alloc__unnamedblk2.configure(this, name(), "hdu_top.u_alloc.unnamedblk2", "unnamedblk2", -9, VerilatedScope::SCOPE_OTHER);
    __Vscope_hdu_top__u_cam.configure(this, name(), "hdu_top.u_cam", "u_cam", -9, VerilatedScope::SCOPE_OTHER);
    __Vscope_hdu_top__u_fallback.configure(this, name(), "hdu_top.u_fallback", "u_fallback", -9, VerilatedScope::SCOPE_OTHER);
}
