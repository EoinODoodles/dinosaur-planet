.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCPressureSwitch_ctor
.dword WCPressureSwitch_dtor

# export table
.dword WCPressureSwitch_obj_Setup
.dword WCPressureSwitch_obj_Control
.dword WCPressureSwitch_obj_Update
.dword WCPressureSwitch_obj_Print
.dword WCPressureSwitch_obj_Free
.dword WCPressureSwitch_obj_GetModelFlags
.dword WCPressureSwitch_obj_GetDataSize
