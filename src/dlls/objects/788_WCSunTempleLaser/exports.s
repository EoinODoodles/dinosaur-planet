.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCSunTempleLaser_ctor
.dword WCSunTempleLaser_dtor

# export table
/*0*/ .dword WCSunTempleLaser_obj_Setup
/*1*/ .dword WCSunTempleLaser_obj_Control
/*2*/ .dword WCSunTempleLaser_obj_Update
/*3*/ .dword WCSunTempleLaser_obj_Print
/*4*/ .dword WCSunTempleLaser_obj_Free
/*5*/ .dword WCSunTempleLaser_obj_GetModelFlags
/*6*/ .dword WCSunTempleLaser_obj_GetDataSize
