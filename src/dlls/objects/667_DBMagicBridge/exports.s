.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBMagicBridge_ctor
.dword DBMagicBridge_dtor

# export table
/*0*/ .dword DBMagicBridge_obj_Setup
/*1*/ .dword DBMagicBridge_obj_Control
/*2*/ .dword DBMagicBridge_obj_Update
/*3*/ .dword DBMagicBridge_obj_Print
/*4*/ .dword DBMagicBridge_obj_Free
/*5*/ .dword DBMagicBridge_obj_GetModelFlags
/*6*/ .dword DBMagicBridge_obj_GetDataSize
