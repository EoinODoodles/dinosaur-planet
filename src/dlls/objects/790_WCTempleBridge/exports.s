.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTempleBridge_ctor
.dword WCTempleBridge_dtor

# export table
/*0*/ .dword WCTempleBridge_obj_Setup
/*1*/ .dword WCTempleBridge_obj_Control
/*2*/ .dword WCTempleBridge_obj_Update
/*3*/ .dword WCTempleBridge_obj_Print
/*4*/ .dword WCTempleBridge_obj_Free
/*5*/ .dword WCTempleBridge_obj_GetModelFlags
/*6*/ .dword WCTempleBridge_obj_GetDataSize
