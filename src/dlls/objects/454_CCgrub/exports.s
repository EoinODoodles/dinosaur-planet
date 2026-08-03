.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCgrub_ctor
.dword CCgrub_dtor

# export table
/*0*/ .dword CCgrub_obj_Setup
/*1*/ .dword CCgrub_obj_Control
/*2*/ .dword CCgrub_obj_Update
/*3*/ .dword CCgrub_obj_Print
/*4*/ .dword CCgrub_obj_Free
/*5*/ .dword CCgrub_obj_GetModelFlags
/*6*/ .dword CCgrub_obj_GetDataSize
