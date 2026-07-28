.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KT_Fallingrocks_ctor
.dword KT_Fallingrocks_dtor

# export table
/*0*/ .dword KT_Fallingrocks_obj_Setup
/*1*/ .dword KT_Fallingrocks_obj_Control
/*2*/ .dword KT_Fallingrocks_obj_Update
/*3*/ .dword KT_Fallingrocks_obj_Print
/*4*/ .dword KT_Fallingrocks_obj_Free
/*5*/ .dword KT_Fallingrocks_obj_GetModelFlags
/*6*/ .dword KT_Fallingrocks_obj_GetDataSize
