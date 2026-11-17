.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCSH_Creator_ctor
.dword CCSH_Creator_dtor

# export table
/*0*/ .dword CCSH_Creator_obj_Setup
/*1*/ .dword CCSH_Creator_obj_Control
/*2*/ .dword CCSH_Creator_obj_Update
/*3*/ .dword CCSH_Creator_obj_Print
/*4*/ .dword CCSH_Creator_obj_Free
/*5*/ .dword CCSH_Creator_obj_GetModelFlags
/*6*/ .dword CCSH_Creator_obj_GetDataSize
