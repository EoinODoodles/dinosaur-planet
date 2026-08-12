.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WarpPoint_ctor
.dword WarpPoint_dtor

# export table
/*0*/ .dword WarpPoint_obj_Setup
/*1*/ .dword WarpPoint_obj_Control
/*2*/ .dword WarpPoint_obj_Update
/*3*/ .dword WarpPoint_obj_Print
/*4*/ .dword WarpPoint_obj_Free
/*5*/ .dword WarpPoint_obj_GetModelFlags
/*6*/ .dword WarpPoint_obj_GetDataSize
