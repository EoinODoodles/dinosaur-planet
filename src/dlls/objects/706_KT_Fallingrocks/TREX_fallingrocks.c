#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    u16 particleID;
    s16 unk22;
    s16 gamebitFalling;
} KT_Fallingrocks_Setup;

// offset: 0x0 | ctor
void KT_Fallingrocks_ctor(void* dll) { }

// offset: 0xC | dtor
void KT_Fallingrocks_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void KT_Fallingrocks_obj_Setup(Object* self, KT_Fallingrocks_Setup* objSetup, s32 reset) {
    self->animCallback = NULL;
}

// offset: 0x2C | func: 1 | export: 1
void KT_Fallingrocks_obj_Control(Object* self) {
    KT_Fallingrocks_Setup* objSetup;
    Object* player;
    SRT fxTransform;
    s32 i;

    objSetup = (KT_Fallingrocks_Setup*)self->setup;
    
    if (mainGetBits(objSetup->gamebitFalling) == FALSE) {
        return;
    }
    
    player = objGetPlayer();
    if (player == NULL) {
        return;
    }
    
    self->srt.transl.x = player->srt.transl.x;
    self->srt.transl.z = player->srt.transl.z;
    
    for (i = 0; i < 10; i++) {
        fxTransform.transl.x = mathRnd(-200, 200) + self->srt.transl.x;
        fxTransform.transl.y = self->srt.transl.y;
        fxTransform.transl.z = mathRnd(-200, 200) + self->srt.transl.z;
        gDLL_17_partfx->vtbl->spawn(self, objSetup->particleID, &fxTransform, 0x200001, -1, NULL);
    }
    
    mainSetBits(objSetup->gamebitFalling, FALSE);
}

// offset: 0x190 | func: 2 | export: 2
void KT_Fallingrocks_obj_Update(Object* self) { }

// offset: 0x19C | func: 3 | export: 3
void KT_Fallingrocks_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x1B4 | func: 4 | export: 4
void KT_Fallingrocks_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1FC | func: 5 | export: 5
u32 KT_Fallingrocks_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x20C | func: 6 | export: 6
u32 KT_Fallingrocks_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
