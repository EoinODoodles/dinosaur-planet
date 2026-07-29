.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword IMIcicle_ctor
.dword IMIcicle_dtor

# export table
/*0*/ .dword IMIcicle_obj_Setup
/*1*/ .dword IMIcicle_obj_Control
/*2*/ .dword IMIcicle_obj_Update
/*3*/ .dword IMIcicle_obj_Print
/*4*/ .dword IMIcicle_obj_Free
/*5*/ .dword IMIcicle_obj_GetModelFlags
/*6*/ .dword IMIcicle_obj_GetDataSize
