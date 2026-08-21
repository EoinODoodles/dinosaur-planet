.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ProjectileSwitch_ctor
.dword ProjectileSwitch_dtor

# export table
/*0*/ .dword ProjectileSwitch_obj_Setup
/*1*/ .dword ProjectileSwitch_obj_Control
/*2*/ .dword ProjectileSwitch_obj_Update
/*3*/ .dword ProjectileSwitch_obj_Print
/*4*/ .dword ProjectileSwitch_obj_Free
/*5*/ .dword ProjectileSwitch_obj_GetModelFlags
/*6*/ .dword ProjectileSwitch_obj_GetDataSize
