.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SeqObj_ctor
.dword SeqObj_dtor

# export table
/*0*/ .dword SeqObj_obj_Setup
/*1*/ .dword SeqObj_obj_Control
/*2*/ .dword SeqObj_obj_Update
/*3*/ .dword SeqObj_obj_Print
/*4*/ .dword SeqObj_obj_Free
/*5*/ .dword SeqObj_obj_GetModelFlags
/*6*/ .dword SeqObj_obj_GetDataSize
