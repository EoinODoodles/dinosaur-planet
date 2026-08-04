.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_565_ctor
.dword dll_565_dtor

# export table
/*0*/ .dword dll_565_obj_Setup
/*1*/ .dword dll_565_obj_Control
/*2*/ .dword dll_565_obj_Update
/*3*/ .dword dll_565_obj_Print
/*4*/ .dword dll_565_obj_Free
/*5*/ .dword dll_565_obj_GetModelFlags
/*6*/ .dword dll_565_obj_GetDataSize
/*7*/ .dword dll_565_Func_9C4
/*8*/ .dword dll_565_Func_9D4
