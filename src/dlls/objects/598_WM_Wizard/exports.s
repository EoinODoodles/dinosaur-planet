.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMWizard_ctor
.dword WMWizard_dtor

# export table
/*0*/ .dword WMWizard_obj_Setup
/*1*/ .dword WMWizard_obj_Control
/*2*/ .dword WMWizard_obj_Update
/*3*/ .dword WMWizard_obj_Print
/*4*/ .dword WMWizard_obj_Free
/*5*/ .dword WMWizard_obj_GetModelFlags
/*6*/ .dword WMWizard_obj_GetDataSize
