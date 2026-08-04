.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FindKyteObject_ctor
.dword FindKyteObject_dtor

# export table
/*0*/ .dword FindKyteObject_obj_Setup
/*1*/ .dword FindKyteObject_obj_Control
/*2*/ .dword FindKyteObject_obj_Update
/*3*/ .dword FindKyteObject_obj_Print
/*4*/ .dword FindKyteObject_obj_Free
/*5*/ .dword FindKyteObject_obj_GetModelFlags
/*6*/ .dword FindKyteObject_obj_GetDataSize
/*7*/ .dword FindKyteObject_kyteTarget_Interact
/*8*/ .dword FindKyteObject_kyteTarget_Func_3E4
/*9*/ .dword FindKyteObject_kyteTarget_Approach
/*10*/ .dword FindKyteObject_kyteTarget_GetType
