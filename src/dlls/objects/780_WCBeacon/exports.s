.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCBeacon_ctor
.dword WCBeacon_dtor

# export table
/*0*/ .dword WCBeacon_obj_Setup
/*1*/ .dword WCBeacon_obj_Control
/*2*/ .dword WCBeacon_obj_Update
/*3*/ .dword WCBeacon_obj_Print
/*4*/ .dword WCBeacon_obj_Free
/*5*/ .dword WCBeacon_obj_GetModelFlags
/*6*/ .dword WCBeacon_obj_GetDataSize
