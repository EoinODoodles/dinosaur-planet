.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword HitAnimator_ctor
.dword HitAnimator_dtor

# export table
/*0*/ .dword HitAnimator_obj_Setup
/*1*/ .dword HitAnimator_obj_Control
/*2*/ .dword HitAnimator_obj_Update
/*3*/ .dword HitAnimator_print
/*4*/ .dword HitAnimator_obj_Free
/*5*/ .dword HitAnimator_obj_GetModelFlags
/*6*/ .dword HitAnimator_obj_GetDataSize
