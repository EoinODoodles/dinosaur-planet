.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLPressureSwitch_ctor
.dword WLPressureSwitch_dtor

# export table
.dword WLPressureSwitch_obj_Setup
.dword WLPressureSwitch_obj_Control
.dword WLPressureSwitch_obj_Update
.dword WLPressureSwitch_obj_Print
.dword WLPressureSwitch_obj_Free
.dword WLPressureSwitch_obj_GetModelFlags
.dword WLPressureSwitch_obj_GetDataSize
