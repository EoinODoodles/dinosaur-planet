.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MagicDust_ctor
.dword MagicDust_dtor

# export table
/*0*/ .dword MagicDust_obj_Setup
/*1*/ .dword MagicDust_obj_Control
/*2*/ .dword MagicDust_obj_Update
/*3*/ .dword MagicDust_obj_Print
/*4*/ .dword MagicDust_obj_Free
/*5*/ .dword MagicDust_obj_GetModelFlags
/*6*/ .dword MagicDust_obj_GetDataSize
