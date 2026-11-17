.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword crawler_ctor
.dword crawler_dtor

# export table
/*0*/ .dword crawler_obj_Setup
/*1*/ .dword crawler_obj_Control
/*2*/ .dword crawler_obj_Update
/*3*/ .dword crawler_obj_Print
/*4*/ .dword crawler_obj_Free
/*5*/ .dword crawler_obj_GetModelFlags
/*6*/ .dword crawler_obj_GetDataSize
