.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIM2Conveyor_ctor
.dword DIM2Conveyor_dtor

# export table
/*0*/ .dword DIM2Conveyor_obj_Setup
/*1*/ .dword DIM2Conveyor_obj_Control
/*2*/ .dword DIM2Conveyor_obj_Update
/*3*/ .dword DIM2Conveyor_obj_Print
/*4*/ .dword DIM2Conveyor_obj_Free
/*5*/ .dword DIM2Conveyor_obj_GetModelFlags
/*6*/ .dword DIM2Conveyor_obj_GetDataSize
/*7*/ .dword DIM2Conveyor_Move
