.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword PressureSwitch_ctor
.dword PressureSwitch_dtor

# export table
.dword PressureSwitch_obj_Setup
.dword PressureSwitch_obj_Control
.dword PressureSwitch_obj_Update
.dword PressureSwitch_obj_Print
.dword PressureSwitch_obj_Free
.dword PressureSwitch_obj_GetModelFlags
.dword PressureSwitch_obj_GetDataSize
