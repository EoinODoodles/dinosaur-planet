.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VisAnimator_ctor
.dword VisAnimator_dtor

# export table
/*0*/ .dword VisAnimator_obj_Setup
/*1*/ .dword VisAnimator_obj_Control
/*2*/ .dword VisAnimator_obj_Update
/*3*/ .dword VisAnimator_obj_Print
/*4*/ .dword VisAnimator_obj_Free
/*5*/ .dword VisAnimator_obj_GetModelFlags
/*6*/ .dword VisAnimator_obj_GetDataSize
