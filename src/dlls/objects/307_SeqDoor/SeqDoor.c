// NOTE: this DLL's official name is unknown. The DLL is shared by many sequence-driven door objects 
// (e.g. WCCageDoor, CF_MovDeckDoor, DIM2PuzzleDoor), so the name "SeqDoor" has been used here!

#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s16 gamebitA;
    s16 gamebitRestoreState;
    s16 preemptTime;
    s8 objSeqIdx;
    u8 yaw;
    u8 enabledActors;
    u8 scale;
    s16 gamebitB;
    s16 gamebitC;
    s16 gamebitD;
    u8 flipBitsGamebitC;
    u8 flipBitsGamebitD;
} SeqDoor_Setup;

typedef struct {
    f32 sinYaw;
    f32 cosYaw;
    f32 unk8;
    u8 state;
    u8 startSequence;
    u8 flags;
} SeqDoor_Data;

typedef enum {
    SeqDoor_STATE_0,
    SeqDoor_STATE_1,
    SeqDoor_STATE_2,
    SeqDoor_STATE_3,
    SeqDoor_STATE_4
} SeqDoor_States;

typedef enum {
    SeqDoor_FLAG_1 = 1,
    SeqDoor_FLAG_2 = 2
} SeqDoor_Flags;

static int SeqDoor_animCallback(Object *self, Object *animObj, AnimObj_Data *animData, s8 prevCallbackValue);
static void SeqDoor_func_6E4(SeqDoor_Data *objdata, SeqDoor_Setup *setup);

// offset: 0x0 | ctor
void SeqDoor_ctor(void* dll) { }

// offset: 0xC | dtor
void SeqDoor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqDoor_obj_Setup(Object* self, SeqDoor_Setup* setup, s32 reset) {
    SeqDoor_Data* objdata = self->data;
    
    objdata->startSequence = TRUE;
    self->srt.yaw = setup->yaw << 8;
    self->animCallback = SeqDoor_animCallback;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    //Set scale
    {
        self->srt.scale = setup->scale * (1.0f / 64.0f);
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }

    //Restore state via gamebit
    if (setup->gamebitRestoreState != NO_GAMEBIT) {
        objdata->state = mainGetBits(setup->gamebitRestoreState);
    } else {
        objdata->state = SeqDoor_STATE_0;
    }

    objdata->sinYaw = mathSinfInterp(self->srt.yaw);
    objdata->cosYaw = mathCosfInterp(self->srt.yaw);
    objdata->unk8 = -((objdata->sinYaw * self->srt.transl.x) + (objdata->cosYaw * self->srt.transl.z));

    objdata->flags = 0;
    if (mainGetBits(setup->gamebitA)) {
        objdata->flags |= SeqDoor_FLAG_1;
    }
    if (mainGetBits(setup->gamebitB)) {
        objdata->flags |= SeqDoor_FLAG_2;
    }
}

// offset: 0x1C8 | func: 1 | export: 1
void SeqDoor_obj_Control(Object* self) {
    SeqDoor_Data* objdata;
    SeqDoor_Setup* setup;
    s32 enabledActors;

    setup = (SeqDoor_Setup*)self->setup;
    objdata = (SeqDoor_Data*)self->data;

    if (objdata->startSequence) {
        if (setup->preemptTime && (objdata->state != SeqDoor_STATE_0)) {
            enabledActors = setup->enabledActors;
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, setup->preemptTime);
        } else {
            enabledActors = -1;
        }
        
        if (setup->objSeqIdx != -1) {
            gDLL_3_Animation->vtbl->start_obj_sequence(setup->objSeqIdx, self, enabledActors);
        }

        objdata->startSequence = FALSE;
    }
}

// offset: 0x2A8 | func: 2 | export: 2
void SeqDoor_obj_Update(Object *self) { }

// offset: 0x2B4 | func: 3 | export: 3
void SeqDoor_obj_Print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x308 | func: 4 | export: 4
void SeqDoor_obj_Free(Object *self, s32 a1) { }

// offset: 0x318 | func: 5 | export: 5
u32 SeqDoor_obj_GetModelFlags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x328 | func: 6 | export: 6
u32 SeqDoor_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(SeqDoor_Data);
}

// offset: 0x33C | func: 7
static int SeqDoor_animCallback(Object *self, Object *animObj, AnimObj_Data *animData, s8 prevCallbackValue) {
    SeqDoor_Data *objdata;
    SeqDoor_Setup *setup;
    TextureAnimator *animator;
    u32 valueGamebitA;
    s32 gamebitBSet;
    s32 i;

    objdata = (SeqDoor_Data*)self->data;
    setup = (SeqDoor_Setup*)self->setup;

    if (self->def->numAnimatedFrames != 0) {
        if (objdata->flags & SeqDoor_FLAG_1) {
            animator = objExprGetTexAnimator(self, 0, 0);
            if (animator != NULL) {
                animator->frame = 0x100;
            }
        }

        if (objdata->flags & SeqDoor_FLAG_2) {
            animator = objExprGetTexAnimator(self, 1, 0);
            if (animator != NULL) {
                animator->frame = 0x100;
            }
        }
    }

    if (objdata->state == SeqDoor_STATE_0) {
        valueGamebitA = mainGetBits(setup->gamebitA);
        gamebitBSet = FALSE;

        //Check if gamebitB is set (or it wasn't specified)
        if ((setup->gamebitB == NO_GAMEBIT) || mainGetBits(setup->gamebitB)) {
            gamebitBSet = TRUE;
        }

        if ((valueGamebitA != 0) && !(objdata->flags & SeqDoor_FLAG_1)) {
            if (self->def->numAnimatedFrames != 0) {
                dll_amSfx->Play(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->flags |= SeqDoor_FLAG_1;
        }

        if (gamebitBSet && !(objdata->flags & SeqDoor_FLAG_2)) {
            if (self->def->numAnimatedFrames != 0) {
                dll_amSfx->Play(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->flags |= SeqDoor_FLAG_2;
        }

        if (objdata->flags == (SeqDoor_FLAG_1 | SeqDoor_FLAG_2)) {
            SeqDoor_func_6E4(objdata, setup);
            objdata->state = SeqDoor_STATE_2;
        }
    } else if ((objdata->state == SeqDoor_STATE_1) && (mainGetBits(setup->gamebitA) == FALSE)) {
        objdata->state = SeqDoor_STATE_3;
    }

    if (objdata->state == SeqDoor_STATE_2) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 2) {
                objdata->state = SeqDoor_STATE_1;
                if (setup->gamebitRestoreState != NO_GAMEBIT) {
                    mainSetBits(setup->gamebitRestoreState, 1);
                }
            }
        }
    } else if (objdata->state == SeqDoor_STATE_3) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 1) {
                SeqDoor_func_6E4(objdata, setup);
                objdata->state = SeqDoor_STATE_0;
                objdata->flags = 0;
                if (setup->gamebitRestoreState != NO_GAMEBIT) {
                    mainSetBits(setup->gamebitRestoreState, 0);
                }
            }
        }
    }

    return !(objdata->state == SeqDoor_STATE_2) && !(objdata->state == SeqDoor_STATE_3);
}

// offset: 0x6E4 | func: 8
static void SeqDoor_func_6E4(SeqDoor_Data *objdata, SeqDoor_Setup *setup) {
    Camera *camera;
    f32 temp;
    u8 value;

    camera = camGetMain();
    temp = (objdata->sinYaw * camera->srt.transl.x) + (objdata->cosYaw * camera->srt.transl.z) + objdata->unk8;
    if (temp < 0.0f) {
        if (setup->gamebitC != NO_GAMEBIT) {
            value = mainGetBits(setup->gamebitC);
            value ^= setup->flipBitsGamebitC;
            mainSetBits(setup->gamebitC, value);
        }
    } else {
        if (setup->gamebitD != NO_GAMEBIT) {
            value = mainGetBits(setup->gamebitD);
            value ^= setup->flipBitsGamebitD;
            mainSetBits(setup->gamebitD, value);
        }
    }
}
