.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTempleDial_ctor
.dword WCTempleDial_dtor

# export table
/*0*/ .dword WCTempleDial_obj_Setup
/*1*/ .dword WCTempleDial_obj_Control
/*2*/ .dword WCTempleDial_obj_Update
/*3*/ .dword WCTempleDial_obj_Print
/*4*/ .dword WCTempleDial_obj_Free
/*5*/ .dword WCTempleDial_obj_GetModelFlags
/*6*/ .dword WCTempleDial_obj_GetDataSize
