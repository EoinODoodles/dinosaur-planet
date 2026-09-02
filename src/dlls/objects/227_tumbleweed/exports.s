.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Tumbleweed_ctor
.dword Tumbleweed_dtor

# export table
.dword Tumbleweed_obj_Setup
.dword Tumbleweed_obj_Control
.dword Tumbleweed_obj_Update
.dword Tumbleweed_obj_Print
.dword Tumbleweed_obj_Free
.dword Tumbleweed_obj_GetModelFlags
.dword Tumbleweed_obj_GetDataSize
.dword Tumbleweed_GetState
.dword Tumbleweed_SetHome
.dword Tumbleweed_Fall
.dword Tumbleweed_GravitateTowardsPoint
.dword Tumbleweed_IsGravitating
.dword Tumbleweed_StorePlayer
