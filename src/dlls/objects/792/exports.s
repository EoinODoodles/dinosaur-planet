.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DLL792_ctor
.dword DLL792_dtor

# export table
/*0*/ .dword DLL792_obj_Setup
/*1*/ .dword DLL792_obj_Control
/*2*/ .dword DLL792_obj_Update
/*3*/ .dword DLL792_obj_Print
/*4*/ .dword DLL792_obj_Free
/*5*/ .dword DLL792_obj_GetModelFlags
/*6*/ .dword DLL792_obj_GetDataSize
