.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword EffectBox_ctor
.dword EffectBox_dtor

# export table
/*0*/ .dword EffectBox_obj_Setup
/*1*/ .dword EffectBox_obj_Control
/*2*/ .dword EffectBox_obj_Update
/*3*/ .dword EffectBox_obj_Print
/*4*/ .dword EffectBox_obj_Free
/*5*/ .dword EffectBox_obj_GetModelFlags
/*6*/ .dword EffectBox_obj_GetDataSize
