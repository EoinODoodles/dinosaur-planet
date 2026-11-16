.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ECSH_Creator_ctor
.dword ECSH_Creator_dtor

# export table
/*0*/ .dword ECSH_Creator_obj_Setup
/*1*/ .dword ECSH_Creator_obj_Control
/*2*/ .dword ECSH_Creator_obj_Update
/*3*/ .dword ECSH_Creator_obj_Print
/*4*/ .dword ECSH_Creator_obj_Free
/*5*/ .dword ECSH_Creator_obj_GetModelFlags
/*6*/ .dword ECSH_Creator_obj_GetDataSize
