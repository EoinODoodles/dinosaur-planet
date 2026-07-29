.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword IMIcePillar_ctor
.dword IMIcePillar_dtor

# export table
/*0*/ .dword IMIcePillar_obj_Setup
/*1*/ .dword IMIcePillar_obj_Control
/*2*/ .dword IMIcePillar_obj_Update
/*3*/ .dword IMIcePillar_obj_Print
/*4*/ .dword IMIcePillar_obj_Free
/*5*/ .dword IMIcePillar_obj_GetModelFlags
/*6*/ .dword IMIcePillar_obj_GetDataSize
