.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword LevelName_ctor
.dword LevelName_dtor

# export table
.dword LevelName_obj_Setup
.dword LevelName_obj_Control
.dword LevelName_obj_Update
.dword LevelName_obj_Print
.dword LevelName_obj_Free
.dword LevelName_obj_GetModelFlags
.dword LevelName_obj_GetDataSize
