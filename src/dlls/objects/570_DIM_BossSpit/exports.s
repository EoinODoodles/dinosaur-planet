.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIM_BossSpit_ctor
.dword DIM_BossSpit_dtor

# export table
/*0*/ .dword DIM_BossSpit_obj_Setup
/*1*/ .dword DIM_BossSpit_obj_Control
/*2*/ .dword DIM_BossSpit_obj_Update
/*3*/ .dword DIM_BossSpit_obj_Print
/*4*/ .dword DIM_BossSpit_obj_Free
/*5*/ .dword DIM_BossSpit_obj_GetModelFlags
/*6*/ .dword DIM_BossSpit_obj_GetDataSize
