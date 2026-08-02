.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VampireBat_ctor
.dword VampireBat_dtor

# export table
/*0*/ .dword VampireBat_obj_Setup
/*1*/ .dword VampireBat_obj_Control
/*2*/ .dword VampireBat_obj_Update
/*3*/ .dword VampireBat_obj_Print
/*4*/ .dword VampireBat_obj_Free
/*5*/ .dword VampireBat_obj_GetModelFlags
/*6*/ .dword VampireBat_obj_GetDataSize
