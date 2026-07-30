.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHqueenearthwalker_ctor
.dword SHqueenearthwalker_dtor

# export table
/*0*/ .dword SHqueenearthwalker_obj_Setup
/*1*/ .dword SHqueenearthwalker_obj_Control
/*2*/ .dword SHqueenearthwalker_obj_Update
/*3*/ .dword SHqueenearthwalker_obj_Print
/*4*/ .dword SHqueenearthwalker_obj_Free
/*5*/ .dword SHqueenearthwalker_obj_GetModelFlags
/*6*/ .dword SHqueenearthwalker_obj_GetDataSize
