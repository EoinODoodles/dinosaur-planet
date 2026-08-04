.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFIceFire_ctor
.dword DFIceFire_dtor

# export table
/*0*/ .dword DFIceFire_obj_Setup
/*1*/ .dword DFIceFire_obj_Control
/*2*/ .dword DFIceFire_obj_Update
/*3*/ .dword DFIceFire_obj_Print
/*4*/ .dword DFIceFire_obj_Free
/*5*/ .dword DFIceFire_obj_GetModelFlags
/*6*/ .dword DFIceFire_obj_GetDataSize
