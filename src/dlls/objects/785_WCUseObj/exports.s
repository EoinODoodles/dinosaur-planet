.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCUseObj_ctor
.dword WCUseObj_dtor

# export table
/*0*/ .dword WCUseObj_obj_Setup
/*1*/ .dword WCUseObj_obj_Control
/*2*/ .dword WCUseObj_obj_Update
/*3*/ .dword WCUseObj_obj_Print
/*4*/ .dword WCUseObj_obj_Free
/*5*/ .dword WCUseObj_obj_GetModelFlags
/*6*/ .dword WCUseObj_obj_GetDataSize
