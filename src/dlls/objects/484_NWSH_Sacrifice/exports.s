.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWSH_Sacrifice_ctor
.dword NWSH_Sacrifice_dtor

# export table
/*0*/ .dword NWSH_Sacrifice_obj_Setup
/*1*/ .dword NWSH_Sacrifice_obj_Control
/*2*/ .dword NWSH_Sacrifice_obj_Update
/*3*/ .dword NWSH_Sacrifice_obj_Print
/*4*/ .dword NWSH_Sacrifice_obj_Free
/*5*/ .dword NWSH_Sacrifice_obj_GetModelFlags
/*6*/ .dword NWSH_Sacrifice_obj_GetDataSize
