.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCTrexStatue_ctor
.dword WCTrexStatue_dtor

# export table
/*0*/ .dword WCTrexStatue_obj_Setup
/*1*/ .dword WCTrexStatue_obj_Control
/*2*/ .dword WCTrexStatue_obj_Update
/*3*/ .dword WCTrexStatue_obj_Print
/*4*/ .dword WCTrexStatue_obj_Free
/*5*/ .dword WCTrexStatue_obj_GetModelFlags
/*6*/ .dword WCTrexStatue_obj_GetDataSize
