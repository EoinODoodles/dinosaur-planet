#include "dll.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/objects/215_SharpClaw.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "game/objects/setups/29_Baddie.h"
#include "game/objects/setups/57_ObjCreator.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objprint.h"

typedef struct {
    s16 timer;          //Countdown to creating a SharpClaw
    s16 countdownSpeed; //Becomes nonzero when gamebitBase is set.

    /* The base gamebit that activates the ObjCreator. Also used as a base gamebitID for the gamebit
       to set when the SharpClaw is defeated (with objSetup->creatorIndex as an offset). */
    s16 gamebitBase;
    s16 unk6;
    s16 quarterHP;  //The SharpClaw's initial health
} ECSH_Creator_Data;

// offset: 0x0 | ctor
void ECSH_Creator_ctor(void* dll) { }

// offset: 0xC | dtor
void ECSH_Creator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void ECSH_Creator_obj_Setup(Object* self, ObjCreator_Setup* objSetup, s32 reset) {
    ECSH_Creator_Data* objData = self->data;

    self->srt.yaw = objSetup->yaw8 << 8;
    self->unkE0 = FALSE;

    objData->timer = 100;
    objData->countdownSpeed = 0;

    self->opacityWithFade = OBJECT_OPACITY_MAX;
    self->opacity = OBJECT_OPACITY_MAX;

    objData->gamebitBase = objSetup->gamebit;
    objData->quarterHP = 2;
    objData->quarterHP += objSetup->quarterHP;
}

// offset: 0x70 | func: 1 | export: 1
void ECSH_Creator_obj_Control(Object* self) {
    s32 pad[4];
    DLL_IModgfx* modGfxDLL;
    Baddie_Setup* baddieSetup;
    ECSH_Creator_Data* objData;
    ObjCreator_Setup* objSetup;
    Baddie* baddie;
    Object* sharpClaw;

    objSetup = (ObjCreator_Setup*)self->setup;
    objData = self->data;

    if ((self->unkE0 == FALSE) && mainGetBits(objData->gamebitBase)) {
        modGfxDLL = dllLoad(DLL_ID_146, 1);
        modGfxDLL->vtbl->Spawn(self, 0, 0, 1, -1, 0);
        modGfxDLL->vtbl->Spawn(self, 1, 0, 1, -1, 0);
        gDLL_6_AMSFX->vtbl->Play(NULL, SOUND_303, MAX_VOLUME, NULL, NULL, 0, NULL);
        dllFree(modGfxDLL);
        objData->countdownSpeed = 1;
        self->unkE0 = TRUE;
    }

    if (objData->countdownSpeed != 0) {
        objData->timer -= objData->countdownSpeed * gUpdateRate;
    }

    if (objData->timer <= 0) {
        u32 noUID = -1;
        baddieSetup = mmAlloc(sizeof(Baddie_Setup), ALLOC_TAG_OBJECTS_COL, NULL);
        baddieSetup->base.x = objSetup->base.x;
        baddieSetup->base.y = objSetup->base.y;
        baddieSetup->base.z = objSetup->base.z;
        baddieSetup->base.objId = OBJ_ClubSharpClaw;
        baddieSetup->base.uID = noUID;
        baddieSetup->base.loadFlags = objSetup->base.loadFlags;
        baddieSetup->base.fadeFlags = objSetup->base.fadeFlags;
        baddieSetup->base.loadDistance = objSetup->base.loadDistance;

        baddieSetup->base.fadeDistance = objSetup->base.fadeDistance;
        baddieSetup->initialWeaponID = BADDIE_WEAPON_CLUB;
        baddieSetup->unk28 = 0;
        baddieSetup->unk18 = objData->gamebitBase + objSetup->creatorIndex;
        baddieSetup->unk30 = -1;
        baddieSetup->unk2A = self->srt.yaw >> 8;
        baddieSetup->unk2B = 2;
        baddieSetup->unk20 = 0;
        baddieSetup->unk1E = 0;
        baddieSetup->unk22 = -1;
        baddieSetup->unk29 = -1;
        baddieSetup->unk2E = -1;
        baddieSetup->unk24 = 0;
        baddieSetup->unk2C = 0;
        baddieSetup->unk34 = -1;
        baddieSetup->unk1A = 0;
        baddieSetup->quarterHitpoints = objData->quarterHP;
        sharpClaw = objSetupObject(&baddieSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);

        if (sharpClaw != NULL) {
            baddie = sharpClaw->data;
            baddie->unk3B0 = SharpClaw_FLAG_20;
        }

        objData->timer = 100;
        objData->countdownSpeed = 0;
    }
}

// offset: 0x2E8 | func: 2 | export: 2
void ECSH_Creator_obj_Update(Object* self) { }

// offset: 0x2F4 | func: 3 | export: 3
void ECSH_Creator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x348 | func: 4 | export: 4
void ECSH_Creator_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x358 | func: 5 | export: 5
u32 ECSH_Creator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x368 | func: 6 | export: 6
u32 ECSH_Creator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(ECSH_Creator_Data);
}
