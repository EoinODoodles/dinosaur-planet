.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SPShop_ctor
.dword SPShop_dtor

# export table
/*0*/ .dword SPShop_obj_Setup
/*1*/ .dword SPShop_obj_Control
/*2*/ .dword SPShop_obj_Update
/*3*/ .dword SPShop_obj_Print
/*4*/ .dword SPShop_obj_Free
/*5*/ .dword SPShop_obj_GetModelFlags
/*6*/ .dword SPShop_obj_GetDataSize
/*7*/ .dword SPShop_GetUnk0
/*8*/ .dword SPShop_PlaySequence
/*9*/ .dword SPShop_IsItemShown
/*10*/ .dword SPShop_IsItemHidden
/*11*/ .dword SPShop_GetMinimumPrice
/*12*/ .dword SPShop_GetSpecialPrice
/*13*/ .dword SPShop_GetInitialPrice
/*14*/ .dword SPShop_GetItemGametextIndex
/*15*/ .dword SPShop_SetCurrentItemIndex
/*16*/ .dword SPShop_GetCurrentItemIndex
/*17*/ .dword SPShop_BuyItem
/*18*/ .dword SPShop_InitMinigameStats
/*19*/ .dword SPShop_UpdateMinigameStats
/*20*/ .dword SPShop_GetMinigameStats
