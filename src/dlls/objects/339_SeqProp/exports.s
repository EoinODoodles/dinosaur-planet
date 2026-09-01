.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SeqProp_ctor
.dword SeqProp_dtor

# export table
/*0*/ .dword SeqProp_obj_Setup
/*1*/ .dword SeqProp_obj_Control
/*2*/ .dword SeqProp_obj_Update
/*3*/ .dword SeqProp_obj_Print
/*4*/ .dword SeqProp_obj_Free
/*5*/ .dword SeqProp_obj_GetModelFlags
/*6*/ .dword SeqProp_obj_GetDataSize
