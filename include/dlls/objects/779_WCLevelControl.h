#ifndef _DLLS_779_H
#define _DLLS_779_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    WC_ObjGroup0_Sun_Beacon_Tunnel,
    WC_ObjGroup1_Moon_Beacon_Tunnel,
    WC_ObjGroup2_Sun_Pushblock_Puzzle,
    WC_ObjGroup3_Moon_Pushblock_Puzzle,
    WC_ObjGroup4_Boss_Lobby,
    WC_ObjGroup5_Central_Temple,
    WC_ObjGroup6_Sun_Temple_Exterior,
    WC_ObjGroup7_Moon_Temple_Exterior,
    WC_ObjGroup8_Sun_Temple_Interior,
    WC_ObjGroup9_Moon_Temple_Interior
} WC_ObjectGroups;

typedef enum {
    WCBlockPuzzle_HIT_Pushblock = 1,
    WCBlockPuzzle_HIT_Symbol = 2,
    WCBlockPuzzle_HIT_Bounds = 4
} WCBlockPuzzle_Collision_Flags;

DLL_INTERFACE(DLL_779_WCLevelControl) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*7*/ void (*SunPuzzleSetCoordsFromGridPosition)(Object* obj, s16 puzzleGridX, s16 puzzleGridZ, f32* x, f32* z);
/*8*/ void (*SunPuzzleSetGridPositionFromCoords)(Object* obj, f32 x, f32 z, s16* puzzleGridX, s16* puzzleGridZ);
/*9*/ void (*SunPuzzleSetCell)(s16 puzzleBlockID, s16 x, s16 y);
/*10*/ u8 (*SunPuzzleGetCell)(s16 x, s16 y);
/*11*/ void (*SunPuzzleSetupPositionHard)(s16 puzzleBlockID, s16* outX, s16* outZ);
/*12*/ void (*SunPuzzleSetupPositionEasy)(s16 puzzleBlockID, s16* outX, s16* outZ);
/*13*/ s32 (*SunPuzzleMove)(Object* obj, s16 puzzleGridX, s16 puzzleGridZ, f32* limitX, f32* limitZ, s32 stepX, s32 stepZ);
/*14*/ void (*MoonPuzzleSetCoordsFromGridPosition)(Object* obj, s16 puzzleGridX, s16 puzzleGridZ, f32* x, f32* z);
/*15*/ void (*MoonPuzzleSetGridPositionFromCoords)(Object* obj, f32 x, f32 z, s16* puzzleGridX, s16* puzzleGridZ);
/*16*/ void (*MoonPuzzleSetCell)(s16 puzzleBlockID, s16 x, s16 z);
/*17*/ u8 (*MoonPuzzleGetCell)(s16 x, s16 y);
/*18*/ void (*MoonPuzzleSetupPositionHard)(s16 puzzleBlockID, s16* outX, s16* outZ);
/*19*/ void (*MoonPuzzleSetupPositionEasy)(s16 puzzleBlockID, s16* outX, s16* outZ);
/*20*/ s32 (*MoonPuzzleMove)(Object* obj, s16 puzzleGridX, s16 puzzleGridZ, f32* limitX, f32* limitZ, s32 stepX, s32 stepZ);
};

#define dll_wcLevelControl(obj) (((DLL_779_WCLevelControl*)obj->dll)->vtbl)

#endif // _DLLS_779_H
