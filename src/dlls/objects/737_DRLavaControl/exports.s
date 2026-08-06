.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DRLavaControl_ctor
.dword DRLavaControl_dtor

# export table
/*0*/ .dword DRLavaControl_obj_Setup
/*1*/ .dword DRLavaControl_obj_Control
/*2*/ .dword DRLavaControl_obj_Update
/*3*/ .dword DRLavaControl_obj_Print
/*4*/ .dword DRLavaControl_obj_Free
/*5*/ .dword DRLavaControl_obj_GetModelFlags
/*6*/ .dword DRLavaControl_obj_GetDataSize
