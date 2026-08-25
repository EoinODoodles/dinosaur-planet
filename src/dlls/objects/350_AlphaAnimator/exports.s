.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword AlphaAnimator_ctor
.dword AlphaAnimator_dtor

# export table
/*0*/ .dword AlphaAnimator_obj_Setup
/*1*/ .dword AlphaAnimator_obj_Control
/*2*/ .dword AlphaAnimator_obj_Update
/*3*/ .dword AlphaAnimator_obj_Print
/*4*/ .dword AlphaAnimator_obj_Free
/*5*/ .dword AlphaAnimator_obj_GetModelFlags
/*6*/ .dword AlphaAnimator_obj_GetDataSize
