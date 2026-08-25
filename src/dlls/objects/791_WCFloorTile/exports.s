.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCFloorTile_ctor
.dword WCFloorTile_dtor

# export table
/*0*/ .dword WCFloorTile_obj_Setup
/*1*/ .dword WCFloorTile_obj_Control
/*2*/ .dword WCFloorTile_obj_Update
/*3*/ .dword WCFloorTile_obj_Print
/*4*/ .dword WCFloorTile_obj_Free
/*5*/ .dword WCFloorTile_obj_GetModelFlags
/*6*/ .dword WCFloorTile_obj_GetDataSize
