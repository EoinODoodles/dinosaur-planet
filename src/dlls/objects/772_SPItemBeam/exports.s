.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SPItemBeam_ctor
.dword SPItemBeam_dtor

# export table
/*0*/ .dword SPItemBeam_obj_Setup
/*1*/ .dword SPItemBeam_obj_Control
/*2*/ .dword SPItemBeam_obj_Update
/*3*/ .dword SPItemBeam_obj_Print
/*4*/ .dword SPItemBeam_obj_Free
/*5*/ .dword SPItemBeam_obj_GetModelFlags
/*6*/ .dword SPItemBeam_obj_GetDataSize
