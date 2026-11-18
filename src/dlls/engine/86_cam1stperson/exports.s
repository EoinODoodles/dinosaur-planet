.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword cam1stperson_ctor
.dword cam1stperson_dtor

# export table
/*0*/ .dword cam1stperson_Setup
/*1*/ .dword cam1stperson_Control
/*2*/ .dword cam1stperson_Free
/*3*/ .dword cam1stperson_Func_A64
