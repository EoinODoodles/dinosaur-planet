.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Fall_Ladders_ctor
.dword Fall_Ladders_dtor

# export table
/*0*/ .dword Fall_Ladders_obj_Setup
/*1*/ .dword Fall_Ladders_obj_Control
/*2*/ .dword Fall_Ladders_obj_Update
/*3*/ .dword Fall_Ladders_obj_Print
/*4*/ .dword Fall_Ladders_obj_Free
/*5*/ .dword Fall_Ladders_obj_GetModelFlags
/*6*/ .dword Fall_Ladders_obj_GetDataSize
