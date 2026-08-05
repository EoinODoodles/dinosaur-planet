.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTile_ctor
.dword WCTile_dtor

# export table
/*0*/ .dword WCTile_obj_Setup
/*1*/ .dword WCTile_obj_Control
/*2*/ .dword WCTile_obj_Update
/*3*/ .dword WCTile_obj_Print
/*4*/ .dword WCTile_obj_Free
/*5*/ .dword WCTile_obj_GetModelFlags
/*6*/ .dword WCTile_obj_GetDataSize
