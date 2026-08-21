.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCLevelControl_ctor
.dword WCLevelControl_dtor

# export table
/*0*/ .dword WCLevelControl_obj_Setup
/*1*/ .dword WCLevelControl_obj_Control
/*2*/ .dword WCLevelControl_obj_Update
/*3*/ .dword WCLevelControl_obj_Print
/*4*/ .dword WCLevelControl_obj_Free
/*5*/ .dword WCLevelControl_obj_GetModelFlags
/*6*/ .dword WCLevelControl_obj_GetDataSize
/*7*/ .dword WCLevelControl_SunPuzzleSetCoordsFromGridPosition
/*8*/ .dword WCLevelControl_SunPuzzleSetGridPositionFromCoords
/*9*/ .dword WCLevelControl_SunPuzzleSetCell
/*10*/ .dword WCLevelControl_SunPuzzleGetCell
/*11*/ .dword WCLevelControl_SunPuzzleSetupPositionInitial
/*12*/ .dword WCLevelControl_SunPuzzleSetupPositionFinished
/*13*/ .dword WCLevelControl_SunPuzzleMove
/*14*/ .dword WCLevelControl_MoonPuzzleSetCoordsFromGridPosition
/*15*/ .dword WCLevelControl_MoonPuzzleSetGridPositionFromCoords
/*16*/ .dword WCLevelControl_MoonPuzzleSetCell
/*17*/ .dword WCLevelControl_MoonPuzzleGetCell
/*18*/ .dword WCLevelControl_MoonPuzzleSetupPositionInitial
/*19*/ .dword WCLevelControl_MoonPuzzleSetupPositionFinished
/*20*/ .dword WCLevelControl_MoonPuzzleMove
