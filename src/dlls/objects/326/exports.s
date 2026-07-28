.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DLL326_ctor
.dword DLL326_dtor

# export table
/*0*/ .dword DLL326_obj_Setup
/*1*/ .dword DLL326_obj_Control
/*2*/ .dword DLL326_obj_Update
/*3*/ .dword DLL326_obj_Print
/*4*/ .dword DLL326_obj_Free
/*5*/ .dword DLL326_obj_GetModelFlags
/*6*/ .dword DLL326_obj_GetDataSize
