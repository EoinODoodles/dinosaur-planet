.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLSeqObject_ctor
.dword WLSeqObject_dtor

# export table
/*0*/ .dword WLSeqObject_obj_Setup
/*1*/ .dword WLSeqObject_obj_Control
/*2*/ .dword WLSeqObject_obj_Update
/*3*/ .dword WLSeqObject_obj_Print
/*4*/ .dword WLSeqObject_obj_Free
/*5*/ .dword WLSeqObject_obj_GetModelFlags
/*6*/ .dword WLSeqObject_obj_GetDataSize
