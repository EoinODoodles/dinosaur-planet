.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KrazoaText_ctor
.dword KrazoaText_dtor

# export table
.dword KrazoaText_obj_Setup
.dword KrazoaText_obj_Control
.dword KrazoaText_obj_Update
.dword KrazoaText_obj_Print
.dword KrazoaText_obj_Free
.dword KrazoaText_obj_GetModelFlags
.dword KrazoaText_obj_GetDataSize
.dword KrazoaText_PrintText
