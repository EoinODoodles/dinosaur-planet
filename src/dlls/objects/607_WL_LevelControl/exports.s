.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_LevelControl_ctor
.dword WL_LevelControl_dtor

# export table
.dword WL_LevelControl_obj_Setup
.dword WL_LevelControl_obj_Control
.dword WL_LevelControl_obj_Update
.dword WL_LevelControl_obj_Print
.dword WL_LevelControl_obj_Free
.dword WL_LevelControl_obj_GetModelFlags
.dword WL_LevelControl_obj_GetDataSize
