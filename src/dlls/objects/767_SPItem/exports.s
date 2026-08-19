.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SPItem_ctor
.dword SPItem_dtor

# export table
/*0*/ .dword SPItem_obj_Setup
/*1*/ .dword SPItem_obj_Control
/*2*/ .dword SPItem_update
/*3*/ .dword SPItem_obj_Print
/*4*/ .dword SPItem_obj_Free
/*5*/ .dword SPItem_obj_GetModelFlags
/*6*/ .dword SPItem_obj_GetDataSize
