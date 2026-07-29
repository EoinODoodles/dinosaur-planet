.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MMP_GeyserVent_ctor
.dword MMP_GeyserVent_dtor

# export table
/*0*/ .dword MMP_GeyserVent_obj_Setup
/*1*/ .dword MMP_GeyserVent_obj_Control
/*2*/ .dword MMP_GeyserVent_obj_Update
/*3*/ .dword MMP_GeyserVent_obj_Print
/*4*/ .dword MMP_GeyserVent_obj_Free
/*5*/ .dword MMP_GeyserVent_obj_GetModelFlags
/*6*/ .dword MMP_GeyserVent_obj_GetDataSize
