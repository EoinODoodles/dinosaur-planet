.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFCheapGalleon_ctor
.dword CFCheapGalleon_dtor

# export table
/*0*/ .dword CFCheapGalleon_obj_Setup
/*1*/ .dword CFCheapGalleon_obj_Control
/*2*/ .dword CFCheapGalleon_obj_Update
/*3*/ .dword CFCheapGalleon_obj_Print
/*4*/ .dword CFCheapGalleon_obj_Free
/*5*/ .dword CFCheapGalleon_obj_GetModelFlags
/*6*/ .dword CFCheapGalleon_obj_GetDataSize
