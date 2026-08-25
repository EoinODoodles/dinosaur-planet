.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GroundAnimator_ctor
.dword GroundAnimator_dtor

# export table
/*0*/ .dword GroundAnimator_obj_Setup
/*1*/ .dword GroundAnimator_obj_Control
/*2*/ .dword GroundAnimator_obj_Update
/*3*/ .dword GroundAnimator_obj_Print
/*4*/ .dword GroundAnimator_obj_Free
/*5*/ .dword GroundAnimator_obj_GetModelFlags
/*6*/ .dword GroundAnimator_obj_GetDataSize
/*7*/ .dword GroundAnimator_TickDig
/*8*/ .dword GroundAnimator_IsDigFinished
/*9*/ .dword GroundAnimator_GetMagicCaveIndex
