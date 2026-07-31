.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFBigDoorSwitch_ctor
.dword CFBigDoorSwitch_dtor

# export table
/*0*/ .dword CFBigDoorSwitch_obj_Setup
/*1*/ .dword CFBigDoorSwitch_obj_Control
/*2*/ .dword CFBigDoorSwitch_obj_Update
/*3*/ .dword CFBigDoorSwitch_obj_Print
/*4*/ .dword CFBigDoorSwitch_obj_Free
/*5*/ .dword CFBigDoorSwitch_obj_GetModelFlags
/*6*/ .dword CFBigDoorSwitch_obj_GetDataSize
