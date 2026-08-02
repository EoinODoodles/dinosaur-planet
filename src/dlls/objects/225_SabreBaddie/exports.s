.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SabreBaddie_ctor
.dword SabreBaddie_dtor

# export table
/*0*/ .dword SabreBaddie_obj_Setup
/*1*/ .dword SabreBaddie_obj_Control
/*2*/ .dword SabreBaddie_obj_Update
/*3*/ .dword SabreBaddie_obj_Print
/*4*/ .dword SabreBaddie_obj_Free
/*5*/ .dword SabreBaddie_obj_GetModelFlags
/*6*/ .dword SabreBaddie_obj_GetDataSize
