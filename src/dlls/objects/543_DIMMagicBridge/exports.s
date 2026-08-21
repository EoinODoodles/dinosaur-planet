.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMMagicBridge_ctor
.dword DIMMagicBridge_dtor

# export table
/*0*/ .dword DIMMagicBridge_obj_Setup
/*1*/ .dword DIMMagicBridge_obj_Control
/*2*/ .dword DIMMagicBridge_obj_Update
/*3*/ .dword DIMMagicBridge_obj_Print
/*4*/ .dword DIMMagicBridge_obj_Free
/*5*/ .dword DIMMagicBridge_obj_GetModelFlags
/*6*/ .dword DIMMagicBridge_obj_GetDataSize
