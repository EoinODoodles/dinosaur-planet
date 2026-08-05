.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCPushBlock_ctor
.dword WCPushBlock_dtor

# export table
.dword WCPushBlock_obj_Setup
.dword WCPushBlock_obj_Control
.dword WCPushBlock_obj_Update
.dword WCPushBlock_obj_Print
.dword WCPushBlock_obj_Free
.dword WCPushBlock_obj_GetModelFlags
.dword WCPushBlock_obj_GetDataSize
