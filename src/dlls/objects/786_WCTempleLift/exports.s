.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTempleLift_ctor
.dword WCTempleLift_dtor

# export table
/*0*/ .dword WCTempleLift_obj_Setup
/*1*/ .dword WCTempleLift_obj_Control
/*2*/ .dword WCTempleLift_obj_Update
/*3*/ .dword WCTempleLift_obj_Print
/*4*/ .dword WCTempleLift_obj_Free
/*5*/ .dword WCTempleLift_obj_GetModelFlags
/*6*/ .dword WCTempleLift_obj_GetDataSize
