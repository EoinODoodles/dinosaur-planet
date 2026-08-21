.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCApertureSymbol_ctor
.dword WCApertureSymbol_dtor

# export table
/*0*/ .dword WCApertureSymbol_setup
/*1*/ .dword WCApertureSymbol_control
/*2*/ .dword WCApertureSymbol_update
/*3*/ .dword WCApertureSymbol_print
/*4*/ .dword WCApertureSymbol_free
/*5*/ .dword WCApertureSymbol_get_model_flags
/*6*/ .dword WCApertureSymbol_get_data_size
