.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KT_RexLevel_ctor
.dword KT_RexLevel_dtor

# export table
.dword KT_RexLevel_obj_Setup
.dword KT_RexLevel_obj_Control
.dword KT_RexLevel_obj_Update
.dword KT_RexLevel_obj_Print
.dword KT_RexLevel_obj_Free
.dword KT_RexLevel_obj_GetModelFlags
.dword KT_RexLevel_obj_GetDataSize
