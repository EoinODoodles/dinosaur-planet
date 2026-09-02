.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CloudShipControl_ctor
.dword CloudShipControl_dtor

# export table
/*0*/ .dword CloudShipControl_obj_Setup
/*1*/ .dword CloudShipControl_obj_Control
/*2*/ .dword CloudShipControl_obj_Update
/*3*/ .dword CloudShipControl_obj_Print
/*4*/ .dword CloudShipControl_obj_Free
/*5*/ .dword CloudShipControl_obj_GetModelFlags
/*6*/ .dword CloudShipControl_obj_GetDataSize
