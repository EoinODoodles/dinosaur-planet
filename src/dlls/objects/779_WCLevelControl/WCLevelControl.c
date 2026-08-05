#include "macros.h"
#include "dlls/objects/779_WCLevelControl.h"
#include "game/gamebits.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/lfx.h"
#include "sys/envfx.h"
#include "dll.h"

#define PUZZLE_UNIT 48

#define SUN_PUZZLE_ORIGIN_X 32
#define SUN_PUZZLE_ORIGIN_Z 129

#define MOON_PUZZLE_ORIGIN_X 224
#define MOON_PUZZLE_ORIGIN_Z 128

typedef struct {
    f32 timer;
    u8 state;
    u8 flags;
    u8 previousState;
} WCLevelControl_Data;

typedef enum {
    FLAG_1_Entered_ObjSeq = 0x1,
    FLAG_2_Pressure_Switch_Challenge_Active = 0x2,
    FLAG_4_Sun_Beacon_Lit = 0x4,
    FLAG_8_Moon_Beacon_Lit = 0x8,
    FLAG_10_Sun_Aperture_Opened = 0x10,
    FLAG_20_Moon_Aperture_Opened = 0x20,
    FLAG_40 = 0x40, //Related to Sun Temple maze?
    FLAG_80 = 0x80  //Related to Moon Temple maze?
} WCLevelControl_Flags;

typedef enum {
    STATE_0_Idle,

    //Act 1 states
    STATE_1_Sun_Beacon_Timed_Challenge,
    STATE_2_Moon_Beacon_Timed_Challenge,
    STATE_3_WCSlabDoor_Opened,

    //Act 2 states
    STATE_4,
    STATE_5,
    STATE_6
} WCLevelControl_States;

/*0x0*/ static u8 dSunPuzzleHard[8][8] = {
    {0, 0, 0, 0, 0, 0, 8, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 4, 0, 0, 0}, 
    {0, 2, 0, 0, 0, 0, 3, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 7}, 
    {0, 0, 0, 0, 6, 0, 0, 0}, 
    {0, 5, 0, 0, 1, 0, 0, 0}
};
/*0x40*/ static u8 dSunPuzzleEasy[8][8] = {
    {0, 0, 0, 0, 0, 0, 4, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 0, 0, 0, 2, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 0, 0, 0}
};
/*0x80*/ static u8 dMoonPuzzleHard[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 5, 0, 0, 2, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 0, 0, 0, 0, 0, 6, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 7}, 
    {0, 8, 0, 0, 4, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};
/*0xC0*/ static u8 dMoonPuzzleEasy[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 1, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 2, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 4, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};

/*0x0*/ static u8 sSunPuzzleCells[8][8];
/*0x40*/ static u8 sMoonPuzzleCells[8][8];

static int WCLevelControl_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void WCLevelControl_handleAct1(Object* self, WCLevelControl_Data* objdata);
static void WCLevelControl_handleAct2(Object* self, WCLevelControl_Data* objdata);
static void WCLevelControl_sunPuzzleInitHard(void);
static void WCLevelControl_moonPuzzleInitHard(void);

// offset: 0x0 | ctor
void WCLevelControl_ctor(void* dll) { }

// offset: 0xC | dtor
void WCLevelControl_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCLevelControl_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    WCLevelControl_Data* objdata;

    objdata = self->data;
    self->animCallback = WCLevelControl_animCallback;

    WCLevelControl_sunPuzzleInitHard();
    WCLevelControl_moonPuzzleInitHard();

    if (mainGetBits(BIT_WC_Moon_Beacon_Lit)) {
        objdata->flags |= FLAG_8_Moon_Beacon_Lit;
    }
    if (mainGetBits(BIT_WC_Sun_Beacon_Lit)) {
        objdata->flags |= FLAG_4_Sun_Beacon_Lit;
    }
    if (mainGetBits(BIT_WC_Moon_Aperture_Opened)) {
        objdata->flags |= FLAG_20_Moon_Aperture_Opened;
    }
    if (mainGetBits(BIT_WC_Sun_Aperture_Opened)) {
        objdata->flags |= FLAG_10_Sun_Aperture_Opened;
    }
    if (mainGetBits(BIT_2A5)) {
        objdata->flags |= FLAG_40;
    }

    objAddObjectType(self, OBJTYPE_LevelControl);

    mainSetBits(BIT_226, 1);
    mainSetBits(BIT_2A6, 1);
    mainSetBits(BIT_206, 1);
    mainSetBits(BIT_25F, 1);
}

// offset: 0x1B4 | func: 1 | export: 1
void WCLevelControl_obj_Control(Object* self) {
    WCLevelControl_Data* objdata = self->data;
    f32 time;
    u8 act;

    //Set up environment/lighting effects
    if (self->unkDC == 0) {
        envfxAction(self, self, 0x1FB, 0);
        envfxAction(self, self, 0x1FC, 0);
        envfxAction(self, self, 0x149, 0);
        lfxAction(self, self, 0x97, 0, 0, 0);
        lfxAction(self, self, 0x24F, 0, 0, 0);
        self->unkDC = 1;
    }

    //Handle acts
    act = gDLL_29_Gplay->vtbl->get_act(self->mapID);
    if ((act == 1) || (act != 2)) {
        WCLevelControl_handleAct1(self, objdata);
    } else {
        WCLevelControl_handleAct2(self, objdata);
    }

    //Check if night-time
    if (gDLL_7_Newday->vtbl->func8(&time)) {
        mainSetBits(BIT_WC_Is_Nighttime, TRUE);
        mainSetBits(BIT_WC_Is_Daytime, FALSE);
    } else {
        mainSetBits(BIT_WC_Is_Nighttime, FALSE);
        mainSetBits(BIT_WC_Is_Daytime, TRUE);
    }
}

// offset: 0x3A0 | func: 2 | export: 2
void WCLevelControl_obj_Update(Object* self) { }

// offset: 0x3AC | func: 3 | export: 3
void WCLevelControl_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x400 | func: 4 | export: 4
void WCLevelControl_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_LevelControl);
}

// offset: 0x440 | func: 5 | export: 5
u32 WCLevelControl_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x450 | func: 6 | export: 6
u32 WCLevelControl_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCLevelControl_Data);
}

// offset: 0x464 | func: 7 | export: 7
void WCLevelControl_SunPuzzleSetCoordsFromGridPosition(Object* obj, s16 puzzleGridX, s16 puzzleGridZ, f32* x, f32* z) {
    f32 blockX;
    f32 blockZ;

    mapWorldToBlockWorldCoords(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, &blockX, &blockZ);

    *x = blockX + SUN_PUZZLE_ORIGIN_X + (puzzleGridX * PUZZLE_UNIT) + PUZZLE_UNIT/2;
    *z = blockZ + SUN_PUZZLE_ORIGIN_Z + (puzzleGridZ * PUZZLE_UNIT) + PUZZLE_UNIT/2;
}

// offset: 0x534 | func: 8 | export: 8
void WCLevelControl_SunPuzzleSetGridPositionFromCoords(Object* obj, f32 x, f32 z, s16* puzzleGridX, s16* puzzleGridZ) {
    f32 blockX;
    f32 blockZ;

    mapWorldToBlockWorldCoords(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, &blockX, &blockZ);
    *puzzleGridX = (s16) ((x - blockX) - SUN_PUZZLE_ORIGIN_X) / PUZZLE_UNIT;
    *puzzleGridZ = (s16) ((z - blockZ) - SUN_PUZZLE_ORIGIN_Z) / PUZZLE_UNIT;
}

// offset: 0x64C | func: 9 | export: 9
void WCLevelControl_SunPuzzleSetCell(s16 puzzleBlockID, s16 x, s16 z) {
    if ((x >= 0) && (x < 8) && (z >= 0) && (z < 8)) {
        sSunPuzzleCells[x][z] = puzzleBlockID;
    } else {
        STUBBED_PRINTF("SUNBLOCK: invalid grid position\n");
    }
}

// offset: 0x6C0 | func: 10 | export: 10
u8 WCLevelControl_SunPuzzleGetCell(s16 x, s16 z) {
    if ((x < 0) || (x >= 8) || (z < 0) || (z >= 8)) {
        STUBBED_PRINTF("SUNBLOCK: invalid grid position\n");
        return 0;
    }
    return sSunPuzzleCells[x][z];
}

// offset: 0x730 | func: 11 | export: 11
void WCLevelControl_SunPuzzleSetupPositionHard(s16 puzzleBlockID, s16* outX, s16* outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dSunPuzzleHard[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("SUN GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0x7FC | func: 12 | export: 12
void WCLevelControl_SunPuzzleSetupPositionEasy(s16 puzzleBlockID, s16* outX, s16* outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dSunPuzzleEasy[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("SUN GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0x8C8 | func: 13 | export: 13
s32 WCLevelControl_SunPuzzleMove(Object* obj, s16 gridX, s16 gridZ, f32* limitX, f32* limitZ, s32 stepX, s32 stepZ) {
    s32 step;
    s32 maxStepsX;
    s32 maxStepsZ;
    f32 temp;

    if (stepX != 0) {
        if (stepX == -1) {
            WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, 7, gridZ, limitX, limitZ);
            gridX++;
            maxStepsX = 8;
        } else {
            WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, 0, gridZ, limitX, limitZ);
            gridX--;
            maxStepsX = -1;
        }

        step = gridX;
        while (step != maxStepsX) {
            if (sSunPuzzleCells[step][gridZ] != 0) {
                if (sSunPuzzleCells[step][gridZ] < 5) {
                    step += stepX;
                    WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, step, gridZ, limitX, &temp);
                    return WCBlockPuzzle_HIT_Pushblock;
                } else {
                    WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, step, gridZ, limitX, &temp);
                    return WCBlockPuzzle_HIT_Symbol;
                }
            }
            step -= stepX;
        }
    } else {
        if (stepZ == -1) {
            WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, gridX, 7, limitX, limitZ);
            gridZ++;
            maxStepsZ = 8;
        } else {
            WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, gridX, 0, limitX, limitZ);
            gridZ--;
            maxStepsZ = -1;
        }

        step = gridZ;
        while (step != maxStepsZ) {
            if (sSunPuzzleCells[gridX][step] != 0) {
                if (sSunPuzzleCells[gridX][step] < 5) {
                    step += stepZ;
                    WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, gridX, step, &temp, limitZ);
                    return WCBlockPuzzle_HIT_Pushblock;
                } else {
                    WCLevelControl_SunPuzzleSetCoordsFromGridPosition(obj, gridX, step, &temp, limitZ);
                    return WCBlockPuzzle_HIT_Symbol;
                }
            }
            step -= stepZ;
        }
    }
    
    return WCBlockPuzzle_HIT_Bounds;
}

// offset: 0xBA8 | func: 14 | export: 14
void WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(Object* obj, s16 puzzleGridX, s16 puzzleGridZ, f32* x, f32* z) {
    f32 blockX;
    f32 blockZ;

    mapWorldToBlockWorldCoords(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, &blockX, &blockZ);
    *x = blockX + MOON_PUZZLE_ORIGIN_X + (puzzleGridX * PUZZLE_UNIT) + PUZZLE_UNIT/2;
    *z = blockZ + MOON_PUZZLE_ORIGIN_Z + (puzzleGridZ * PUZZLE_UNIT) + PUZZLE_UNIT/2;
}

// offset: 0xC78 | func: 15 | export: 15
void WCLevelControl_MoonPuzzleSetGridPositionFromCoords(Object* obj, f32 x, f32 z, s16* puzzleGridX, s16* puzzleGridZ) {
    f32 blockX;
    f32 blockZ;

    mapWorldToBlockWorldCoords(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, &blockX, &blockZ);
    *puzzleGridX = (s16)((x - blockX) - MOON_PUZZLE_ORIGIN_X) / PUZZLE_UNIT;
    *puzzleGridZ = (s16)((z - blockZ) - MOON_PUZZLE_ORIGIN_Z) / PUZZLE_UNIT;
}

// offset: 0xD90 | func: 16 | export: 16
void WCLevelControl_MoonPuzzleSetCell(s16 puzzleBlockID, s16 x, s16 z) {
    if ((x >= 0) && (x < 8) && (z >= 0) && (z < 8)) {
        sMoonPuzzleCells[x][z] = puzzleBlockID;
    } else {
        STUBBED_PRINTF("MOONBLOCK: invalid grid position\n");
    }
}

// offset: 0xE04 | func: 17 | export: 17
u8 WCLevelControl_MoonPuzzleGetCell(s16 x, s16 z) {
    if ((x < 0) || (x >= 8) || (z < 0) || (z >= 8)) {
        STUBBED_PRINTF("MOONBLOCK: invalid grid position\n");
        return 0;
    }
    return sMoonPuzzleCells[x][z];
}

// offset: 0xE74 | func: 18 | export: 18
void WCLevelControl_MoonPuzzleSetupPositionHard(s16 puzzleBlockID, s16* outX, s16* outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dMoonPuzzleHard[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("MOON GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0xF40 | func: 19 | export: 19
void WCLevelControl_MoonPuzzleSetupPositionEasy(s16 puzzleBlockID, s16* outX, s16* outZ) {
    s32 x;
    s32 z;

    for (x = 0; x < 8; x++) {
        for (z = 0; z < 8; z++) {
            if (puzzleBlockID == dMoonPuzzleEasy[x][z]) {
                *outX = x;
                *outZ = z;
                return;
            }
        }
    }

    STUBBED_PRINTF("MOON GRID PUZZLE: Error cannot find given id\n");
}

// offset: 0x100C | func: 20 | export: 20
s32 WCLevelControl_MoonPuzzleMove(Object* obj, s16 gridX, s16 gridZ, f32* limitX, f32* limitZ, s32 stepX, s32 stepZ) {
    s32 step;
    s32 maxStepsX;
    s32 maxStepsZ;
    f32 temp;

    if (stepX != 0) {
        if (stepX == -1) {
            WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, 7, gridZ, limitX, limitZ);
            gridX++;
            maxStepsX = 8;
        } else {
            WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, 0, gridZ, limitX, limitZ);
            gridX--;
            maxStepsX = -1;
        }

        step = gridX;
        while (step != maxStepsX) {
            if (sMoonPuzzleCells[step][gridZ] != 0) {
                if (sMoonPuzzleCells[step][gridZ] < 5) {
                    step += stepX;
                    WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, step, gridZ, limitX, &temp);
                    return WCBlockPuzzle_HIT_Pushblock;
                } else {
                    WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, step, gridZ, limitX, &temp);
                    return WCBlockPuzzle_HIT_Symbol;
                }
            }
            step -= stepX;
        }
    } else {
        if (stepZ == -1) {
            WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, gridX, 7, limitX, limitZ);
            gridZ++;
            maxStepsZ = 8;
        } else {
            WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, gridX, 0, limitX, limitZ);
            gridZ--;
            maxStepsZ = -1;
        }

        step = gridZ;
        while (step != maxStepsZ) {
            if (sMoonPuzzleCells[gridX][step] != 0) {
                if (sMoonPuzzleCells[gridX][step] < 5) {
                    step += stepZ;
                    WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, gridX, step, &temp, limitZ);
                    return WCBlockPuzzle_HIT_Pushblock;
                } else {
                    WCLevelControl_MoonPuzzleSetCoordsFromGridPosition(obj, gridX, step, &temp, limitZ);
                    return WCBlockPuzzle_HIT_Symbol;
                }
            }
            step -= stepZ;
        }
    }

    return 4;
}

// offset: 0x12EC | func: 21
static int WCLevelControl_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    WCLevelControl_Data* objdata = self->data;
    s32 i;
    
    objdata->flags |= FLAG_1_Entered_ObjSeq;
    objdata->flags &= ~FLAG_2_Pressure_Switch_Challenge_Active;

    if (objdata->previousState == STATE_1_Sun_Beacon_Timed_Challenge) {
        objdata->timer -= gUpdateRateF;
        if (objdata->timer <= 0.0f) {
            mainSetBits(BIT_WC_SlabDoor_Sun_Symbol_Lit, TRUE);
        }
    } else if (objdata->previousState == STATE_2_Moon_Beacon_Timed_Challenge) {
        objdata->timer -= gUpdateRateF;
        if (objdata->timer <= 0.0f) {
            mainSetBits(BIT_WC_SlabDoor_Moon_Symbol_Lit, TRUE);
        }
    }

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
            case 1:
                objdata->state = STATE_6;
                break;
            default:
                break;
        }
    }

    return 0;
}

// offset: 0x1444 | func: 22
/**
  * Runs if Walled City isn't currently in Act 2.
  */
static void WCLevelControl_handleAct1(Object* self, WCLevelControl_Data* objdata) {
    if (objdata->flags & FLAG_2_Pressure_Switch_Challenge_Active) {
        return;
    }

    objdata->previousState = objdata->state;

    switch (objdata->state) {
    case STATE_1_Sun_Beacon_Timed_Challenge:
        if (objdata->flags & FLAG_1_Entered_ObjSeq) {
            //Start a 60 second timed challenge
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            menu_func_8000F64C(0x11, 60);
            menu_func_8000F6CC();
        } else if (mainGetBits(BIT_WC_Sun_Beacon_Lit)) {
            //Success! Play a sequence when the player lights the beacon
            objdata->flags |= FLAG_4_Sun_Beacon_Lit;
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            menu_func_8000FAC8();

            //Play a different sequence (WCSlabDoor opening) if both beacons are now lit
            if (mainGetBits(BIT_WC_Moon_Beacon_Lit)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objdata->state = STATE_3_WCSlabDoor_Opened;
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->state = STATE_0_Idle;
            }
        } else if (menu_func_8000FB1C()) {
            //Failure... reset the gamebits and revert state
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            mainSetBits(BIT_WC_Sun_Beacon_Raised, FALSE);
            mainSetBits(BIT_WC_Sun_Pressure_Switch_Active, FALSE);
            objdata->state = STATE_0_Idle;
        }
        break;
    case STATE_2_Moon_Beacon_Timed_Challenge:
        if (objdata->flags & FLAG_1_Entered_ObjSeq) {
            //Start a 60 second timed challenge
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            menu_func_8000F64C(0x11, 60);
            menu_func_8000F6CC();
        } else if (mainGetBits(BIT_WC_Moon_Beacon_Lit)) {
            //Success! Play a sequence when the player lights the beacon
            objdata->flags |= FLAG_8_Moon_Beacon_Lit;
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            menu_func_8000FAC8();

            //Play a different sequence (WCSlabDoor opening) if both beacons are now lit
            if (mainGetBits(BIT_WC_Sun_Beacon_Lit)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objdata->state = STATE_3_WCSlabDoor_Opened;
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->state = STATE_0_Idle;
            }
        } else if (menu_func_8000FB1C()) {
            //Failure... reset the gamebits and revert state
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            mainSetBits(BIT_WC_Moon_Beacon_Raised, FALSE);
            mainSetBits(BIT_WC_Moon_Pressure_Switch_Active, FALSE);
            objdata->state = STATE_0_Idle;
        }
        break;
    case STATE_3_WCSlabDoor_Opened:
        break;
    default:
        //Start a timed challenge when one of the pressure switches is pressed
        if (!(objdata->flags & FLAG_4_Sun_Beacon_Lit) && (mainGetBits(BIT_WC_Sun_Pressure_Switch_Active))) {
            //Sun switch
            mainSetBits(BIT_WC_Sun_Beacon_Raised, TRUE);
            objdata->state = STATE_1_Sun_Beacon_Timed_Challenge;
            objdata->flags |= FLAG_2_Pressure_Switch_Challenge_Active;
            objdata->timer = 70.0f;
        } else if (!(objdata->flags & FLAG_8_Moon_Beacon_Lit) && (mainGetBits(BIT_WC_Moon_Pressure_Switch_Active))) {
            //Moon switch
            mainSetBits(BIT_WC_Moon_Beacon_Raised, TRUE);
            objdata->state = STATE_2_Moon_Beacon_Timed_Challenge;
            objdata->flags |= FLAG_2_Pressure_Switch_Challenge_Active;
            objdata->timer = 70.0f;
        }
        break;
    }

    objdata->flags &= ~FLAG_1_Entered_ObjSeq;
}

// offset: 0x1928 | func: 23
static void WCLevelControl_handleAct2(Object* self, WCLevelControl_Data* objdata) {
    u8 isNightTime;
    u8 temp;
    f32 time;

    isNightTime = gDLL_7_Newday->vtbl->func8(&time);

    switch (objdata->state) {
        case STATE_6:
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            menu_func_8000F64C(0x11, 60);
            menu_func_8000F6CC();
            objdata->state = STATE_4;
            break;
        case STATE_4:
            if (menu_func_8000FB1C()) {
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
                if (mainGetBits(BIT_2A5)) {
                    objdata->flags |= FLAG_40;
                } else {
                    mainSetBits(BIT_274, 0);
                    mainSetBits(BIT_2B1, 0);
                    mainSetBits(BIT_226, 1);
                    mainSetBits(BIT_2A6, 1);
                    mainSetBits(BIT_206, 1);
                    mainSetBits(BIT_25F, 1);
                }
                objdata->state = STATE_0_Idle;
            }
            break;
        default:
            if (!(objdata->flags & FLAG_40)) {
                if (mainGetBits(BIT_2B1) && mainGetBits(BIT_204)) {
                    mainSetBits(BIT_226, 0);
                    mainSetBits(BIT_2A6, 0);
                    mainSetBits(BIT_206, 0);
                    mainSetBits(BIT_25F, 0);
                    mainSetBits(BIT_274, 1);
                    objdata->state = STATE_6;
                }
            }
            break;
    }

    if (!(objdata->flags & FLAG_10_Sun_Aperture_Opened)) {
        temp = mainGetBits(BIT_810); // get sun block puzzle pieces in correct place
        if (temp == 4) {
            mainSetBits(BIT_WC_Sun_Aperture_Opened,  TRUE);
            objdata->flags |= FLAG_10_Sun_Aperture_Opened;
        } else if (isNightTime || mainGetBits(BIT_808)) {
            WCLevelControl_sunPuzzleInitHard();
        }
    }

    if (!(objdata->flags & FLAG_20_Moon_Aperture_Opened)) {
        temp = mainGetBits(BIT_811); // get moon block puzzle pieces in correct place
        if (temp == 4) {
            mainSetBits(BIT_WC_Moon_Aperture_Opened, TRUE);
            objdata->flags |= FLAG_20_Moon_Aperture_Opened;
        } else if (!isNightTime || mainGetBits(BIT_809)) {
            WCLevelControl_moonPuzzleInitHard();
        }
    }

    objdata->flags &= ~FLAG_1_Entered_ObjSeq;
}

// offset: 0x1CF4 | func: 24
static void WCLevelControl_sunPuzzleInitHard(void) {
    mainSetBits(BIT_810, FALSE);
    bcopy(dSunPuzzleHard, sSunPuzzleCells, sizeof(sSunPuzzleCells));
}

// offset: 0x1D54 | func: 25
static void WCLevelControl_moonPuzzleInitHard(void) {
    mainSetBits(BIT_811, FALSE);
    bcopy(dMoonPuzzleHard, sMoonPuzzleCells, sizeof(sMoonPuzzleCells));
}
