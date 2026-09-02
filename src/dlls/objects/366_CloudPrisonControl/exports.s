.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CloudPrisonControl_ctor
.dword CloudPrisonControl_dtor

# export table
/*0*/ .dword CloudPrisonControl_obj_Setup
/*1*/ .dword CloudPrisonControl_obj_Control
/*2*/ .dword CloudPrisonControl_obj_Update
/*3*/ .dword CloudPrisonControl_obj_Print
/*4*/ .dword CloudPrisonControl_obj_Free
/*5*/ .dword CloudPrisonControl_obj_GetModelFlags
/*6*/ .dword CloudPrisonControl_obj_GetDataSize
