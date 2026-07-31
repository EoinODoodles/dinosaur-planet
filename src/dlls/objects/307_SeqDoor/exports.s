.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SeqDoor_ctor
.dword SeqDoor_dtor

# export table
/*0*/ .dword SeqDoor_obj_Setup
/*1*/ .dword SeqDoor_obj_Control
/*2*/ .dword SeqDoor_obj_Update
/*3*/ .dword SeqDoor_obj_Print
/*4*/ .dword SeqDoor_obj_Free
/*5*/ .dword SeqDoor_obj_GetModelFlags
/*6*/ .dword SeqDoor_obj_GetDataSize
