.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MagicPlant_ctor
.dword MagicPlant_dtor

# export table
/*0*/ .dword MagicPlant_obj_Setup
/*1*/ .dword MagicPlant_obj_Control
/*2*/ .dword MagicPlant_obj_Update
/*3*/ .dword MagicPlant_obj_Print
/*4*/ .dword MagicPlant_obj_Free
/*5*/ .dword MagicPlant_obj_GetModelFlags
/*6*/ .dword MagicPlant_obj_GetDataSize
