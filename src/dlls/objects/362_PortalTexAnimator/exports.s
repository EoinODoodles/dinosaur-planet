.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword PortalTexAnimator_ctor
.dword PortalTexAnimator_dtor

# export table
.dword PortalTexAnimator_obj_Setup
.dword PortalTexAnimator_obj_Control
.dword PortalTexAnimator_obj_Update
.dword PortalTexAnimator_obj_Print
.dword PortalTexAnimator_obj_Free
.dword PortalTexAnimator_obj_GetModelFlags
.dword PortalTexAnimator_obj_GetDataSize
