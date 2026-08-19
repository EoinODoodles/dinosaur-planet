.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword UseObj_ctor
.dword UseObj_dtor

# export table
/*0*/ .dword UseObj_obj_Setup
/*1*/ .dword UseObj_obj_Control
/*2*/ .dword UseObj_obj_Update
/*3*/ .dword UseObj_obj_Print
/*4*/ .dword UseObj_obj_Free
/*5*/ .dword UseObj_obj_GetModelFlags
/*6*/ .dword UseObj_obj_GetDataSize
