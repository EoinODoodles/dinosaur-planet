.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword LaserBeam_ctor
.dword LaserBeam_dtor

# export table
/*0*/ .dword LaserBeam_obj_Setup
/*1*/ .dword LaserBeam_obj_Control
/*2*/ .dword LaserBeam_obj_Update
/*3*/ .dword LaserBeam_obj_Print
/*4*/ .dword LaserBeam_obj_Free
/*5*/ .dword LaserBeam_obj_GetModelFlags
/*6*/ .dword LaserBeam_obj_GetDataSize
