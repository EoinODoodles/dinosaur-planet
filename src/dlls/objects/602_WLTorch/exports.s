.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLTorch_ctor
.dword WLTorch_dtor

# export table
/*0*/ .dword WLTorch_obj_Setup
/*1*/ .dword WLTorch_obj_Control
/*2*/ .dword WLTorch_obj_Update
/*3*/ .dword WLTorch_obj_Print
/*4*/ .dword WLTorch_obj_Free
/*5*/ .dword WLTorch_obj_GetModelFlags
/*6*/ .dword WLTorch_obj_GetDataSize
