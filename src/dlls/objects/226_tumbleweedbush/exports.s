.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TumbleweedBush_ctor
.dword TumbleweedBush_dtor

# export table
/*0*/ .dword TumbleweedBush_obj_Setup
/*1*/ .dword TumbleweedBush_obj_Control
/*2*/ .dword TumbleweedBush_obj_Update
/*3*/ .dword TumbleweedBush_obj_Print
/*4*/ .dword TumbleweedBush_obj_Free
/*5*/ .dword TumbleweedBush_obj_GetModelFlags
/*6*/ .dword TumbleweedBush_obj_GetDataSize
/*7*/ .dword TumbleweedBush_RemoveTumbleweed
