#include "dlls/objects/779_WCLevelControl.h"
#include "game/gamebits.h"
#include "sys/main.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    Object* levelCtrl;
    u8 _unk4[0x8 - 0x4];
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 state;
} WCTile_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 _unk18;
/*19*/ s8 modelIdx;
/*1A*/ s16 unk1A;
} WCTile_Setup;

typedef enum {
    WCTile_STATE_0,
    WCTile_STATE_1,
    WCTile_STATE_2,
    WCTile_STATE_3,
    WCTile_STATE_4_Fading_In,
    WCTile_STATE_5
} WCTile_States;

// offset: 0x0 | ctor
void WCTile_ctor(void* dll) { }

// offset: 0xC | dtor
void WCTile_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCTile_obj_Setup(Object* self, WCTile_Setup* setup, s32 reset) {
    WCTile_Data* objdata = self->data;

    self->srt.transl.y = setup->base.y + 25.0f;

    self->modelInstIdx = setup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }

    objdata->unkC = setup->unk1A;

    self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    
    self->opacity = 0;
}

// offset: 0x88 | func: 1 | export: 1
void WCTile_obj_Control(Object* self) {
    f32 distance;
    f32 time;
    WCTile_Data* objdata;
    u8 isNight;
    s32 opacity;

    distance = 100000.0f;
    objdata = self->data;

    if (objdata->levelCtrl == NULL) {
        objdata->levelCtrl = objGetNearestTypeTo(OBJTYPE_LevelControl, self, &distance);
        self->opacity = 0;
        return;
    }

    self->srt.yaw += (s16) (gUpdateRateF * 180.0f);

    if (objdata->state != WCTile_STATE_5) {
        isNight = gDLL_7_Newday->vtbl->func8(&time);
        if (self->modelInstIdx == 1) {
            if (mainGetBits(BIT_WC_Sun_Aperture_Opened)) {  //Completed Sun Block Puzzle
                objdata->state = WCTile_STATE_5;
            } else if (isNight) {
                objdata->state = WCTile_STATE_3;
            }
        } else {
            if (mainGetBits(BIT_WC_Moon_Aperture_Opened)) { //Completed Moon Block Puzzle
                objdata->state = WCTile_STATE_5;
            } else if (!isNight) {
                objdata->state = WCTile_STATE_3;
            }
        }
    }

    switch (objdata->state) {
    case WCTile_STATE_0:
        if (self->modelInstIdx == 1) {
            dll_wcLevelControl(objdata->levelCtrl)->func11(objdata->unkC, &objdata->unk8, &objdata->unkA);
            dll_wcLevelControl(objdata->levelCtrl)->func7(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
        } else {
            dll_wcLevelControl(objdata->levelCtrl)->func18(objdata->unkC, &objdata->unk8, &objdata->unkA);
            dll_wcLevelControl(objdata->levelCtrl)->func14(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
        }
        objdata->state = WCTile_STATE_1;
        break;
    case WCTile_STATE_2:
        self->opacity = 0;
        break;
    case WCTile_STATE_5:
        self->opacity = 0;
        break;
    case WCTile_STATE_3:
        opacity = self->opacity - (gUpdateRate * 8);
        if (opacity < 0) {
            opacity = 0;
        }
        self->opacity = opacity;

        if (self->opacity == 0) {
            if (self->modelInstIdx == 1) {
                if (!isNight) {
                    dll_wcLevelControl(objdata->levelCtrl)->func11(objdata->unkC, &objdata->unk8, &objdata->unkA);
                    dll_wcLevelControl(objdata->levelCtrl)->func7(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
                    objdata->state = WCTile_STATE_4_Fading_In;
                }
            } else {
                if (isNight) {
                    dll_wcLevelControl(objdata->levelCtrl)->func18(objdata->unkC, &objdata->unk8, &objdata->unkA);
                    dll_wcLevelControl(objdata->levelCtrl)->func14(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
                    objdata->state = WCTile_STATE_4_Fading_In;
                }
            }
        }
        break;
    case WCTile_STATE_4_Fading_In:
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > 200) {
            opacity = 200;
        }
        self->opacity = opacity;
        if (self->opacity >= 200) {
            objdata->state = WCTile_STATE_1;
        }
        break;
    case WCTile_STATE_1:
    default:
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > 200) {
            opacity = 200;
        }
        self->opacity = opacity;
        if (self->modelInstIdx == 1) {
            if (objdata->unkC != dll_wcLevelControl(objdata->levelCtrl)->func10(objdata->unk8, objdata->unkA)) {
                objdata->state = WCTile_STATE_2;
            }
        } else {
            if (objdata->unkC != dll_wcLevelControl(objdata->levelCtrl)->func17(objdata->unk8, objdata->unkA)) {
                objdata->state = WCTile_STATE_2;
            }
        }
        break;
    }
}

// offset: 0x510 | func: 2 | export: 2
void WCTile_obj_Update(Object* self) { }

// offset: 0x51C | func: 3 | export: 3
void WCTile_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x570 | func: 4 | export: 4
void WCTile_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x580 | func: 5 | export: 5
u32 WCTile_obj_GetModelFlags(Object* self) {
    WCTile_Setup* setup = (WCTile_Setup*)self->setup;
    s8 modelIdx = setup->modelIdx;
    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5B4 | func: 6 | export: 6
u32 WCTile_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCTile_Data);
}
