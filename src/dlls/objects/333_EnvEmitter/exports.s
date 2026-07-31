.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword EnvEmitter_ctor
.dword EnvEmitter_dtor

# export table
/*0*/ .dword EnvEmitter_obj_Setup
/*1*/ .dword EnvEmitter_obj_Control
/*2*/ .dword EnvEmitter_obj_Update
/*3*/ .dword EnvEmitter_obj_Print
/*4*/ .dword EnvEmitter_obj_Free
/*5*/ .dword EnvEmitter_obj_GetModelFlags
/*6*/ .dword EnvEmitter_obj_GetDataSize
