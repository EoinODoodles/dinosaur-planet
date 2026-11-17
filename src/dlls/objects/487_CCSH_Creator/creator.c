#include "dll.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "game/objects/setups/29_Baddie.h"
#include "game/objects/setups/57_ObjCreator.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objprint.h"

typedef struct {
    s16 timer;
    s16 countdownSpeed;
} CCSH_Creator_Data;

// offset: 0x0 | ctor
void CCSH_Creator_ctor(void* dll) { }

// offset: 0xC | dtor
void CCSH_Creator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCSH_Creator_obj_Setup(Object* self, ObjCreator_Setup* objSetup, s32 reset) {
    CCSH_Creator_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw8 << 8;
    self->unkE0 = 0;
    
    objData->timer = 100;
    objData->countdownSpeed = 0;
    
    self->opacityWithFade = OBJECT_OPACITY_MAX;
    self->opacity = OBJECT_OPACITY_MAX;
}

// offset: 0x50 | func: 1 | export: 1
void CCSH_Creator_obj_Control(Object* self) {
    Baddie_Setup* sabreSetup;
    Object* sabreBaddie;
    ObjCreator_Setup* objSetup;
    CCSH_Creator_Data* objData;
    Baddie* baddie;
    DLL_IModgfx* modGfxDLL;

    objSetup = (ObjCreator_Setup*)self->setup;
    objData = self->data;
    
    if (mainGetBits(BIT_5B9)) {
        self->unkE0 = FALSE;
        objData->timer = 100;
        objData->countdownSpeed = 0;
        self->opacityWithFade = OBJECT_OPACITY_MAX;
        self->opacity = OBJECT_OPACITY_MAX;
        return;
    }
    
    if ((self->unkE0 == FALSE) && (mainGetBits(BIT_1CD + objSetup->creatorIndex))) {
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
        sabreSetup = objAllocSetup(sizeof(Baddie_Setup), OBJ_SabreBaddie);
        sabreSetup->base.x = objSetup->base.x;
        sabreSetup->base.y = objSetup->base.y;
        sabreSetup->base.z = objSetup->base.z;
        sabreSetup->base.loadFlags = objSetup->base.loadFlags;
        sabreSetup->base.fadeFlags = objSetup->base.fadeFlags;
        sabreSetup->base.loadDistance = objSetup->base.loadDistance;
        sabreSetup->base.fadeDistance = objSetup->base.fadeDistance;
        sabreSetup->initialWeaponID = BADDIE_WEAPON_SWORD;
        sabreSetup->unk18 = BIT_DF_Shrine_SharpClaw_Defeated;
        sabreSetup->unk30 = -1;
        sabreSetup->unk2A = self->srt.yaw >> 8;
        sabreSetup->unk2B = 2;
        sabreSetup->unk22 = mainGetBits(BIT_1CE) ? OBJ_BoneDust : -1; //@bug: this seems to be using an objectID, but it should be using an index from `BaddieDrop_IDs`
        sabreSetup->unk29 = 0xFF;
        sabreSetup->unk2E = -1;
        sabreSetup->quarterHitpoints = objSetup->creatorIndex; //TODO: not using the quarterHP field, so maybe requires a unique ObjSetup struct?
        sabreBaddie = objSetupObject(&sabreSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
        
        if (sabreBaddie != NULL) {
            baddie = sabreBaddie->data;
            if (baddie != NULL) {
                baddie->unk3B0 = 0x20;
            }
        }
        
        objData->timer = 100;
        objData->countdownSpeed = 0;
    }
}

// offset: 0x308 | func: 2 | export: 2
void CCSH_Creator_obj_Update(Object* self) { }

// offset: 0x314 | func: 3 | export: 3
void CCSH_Creator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x368 | func: 4 | export: 4
void CCSH_Creator_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x378 | func: 5 | export: 5
u32 CCSH_Creator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x388 | func: 6 | export: 6
u32 CCSH_Creator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CCSH_Creator_Data);
}
