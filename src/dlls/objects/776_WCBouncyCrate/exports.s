.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCBouncyCrate_ctor
.dword WCBouncyCrate_dtor

# export table
/*0*/ .dword WCBouncyCrate_obj_Setup
/*1*/ .dword WCBouncyCrate_obj_Control
/*2*/ .dword WCBouncyCrate_obj_Update
/*3*/ .dword WCBouncyCrate_obj_Print
/*4*/ .dword WCBouncyCrate_obj_Free
/*5*/ .dword WCBouncyCrate_obj_GetModelFlags
/*6*/ .dword WCBouncyCrate_obj_GetDataSize
