#include "dll.h"
#include "game/objects/object.h"
#include "sys/camera.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/vi.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
} DF_Lantern_Setup;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u32 unk10;
} DF_Lantern_Data;

// offset: 0x0 | ctor
void DF_Lantern_ctor(void* dll) { }

// offset: 0xC | dtor
void DF_Lantern_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DF_Lantern_obj_Setup(Object* self, DF_Lantern_Setup* objSetup, s32 reset) {
    DF_Lantern_Data* objData = self->data;
    
    objData->unkA = objSetup->unk1B;
    objData->unkB = 0;
    
    if (!(objData->unkA)) {
        objData->unk0 = 490;
        objData->unk2 = 491;
        objData->unk6 = 100;
        objData->unk8 = 250;
    } else {
        objData->unk0 = 492;
        objData->unk2 = 493;
        objData->unk6 = 100;
        objData->unk8 = 250;
    }
    
    if (objSetup->unk23 & 0x20) {
        objData->unk8 = self->fadeDistance;
    }
    
    self->srt.scale = objSetup->unk1C;
    self->srt.roll = (objSetup->unk18 - 0x7F) << 8;
    self->srt.pitch = (objSetup->unk19 - 0x7F) << 8;
    self->srt.yaw = objSetup->unk1A << 8;
    
    if (objSetup->unk23 & 1) {
        objData->unkB |= 1;
    }
    
    if (objData->unkB & 1) {
        func_800267A4(self);
    } else {
        func_8002674C(self);
    }
}

// offset: 0x1C0 | func: 1 | export: 1
void DF_Lantern_obj_Control(Object* self) {
    DF_Lantern_Data* objData;
    f32 projectedX;
    f32 projectedY;
    f32 projectedZ;
    f32 pointX;
    f32 pointY;
    f32 pointZ;
    f32 distance;
    s32 screenX;
    s32 screenY;
    s32 zDepth;
    s32 zDepthOcclude;
    Camera* cam;
    Object* player;
    DF_Lantern_Setup* objSetup;

    objData = self->data;
    objSetup = (DF_Lantern_Setup*)self->setup;
    
    objData->unkB &= ~(2 | 4 | 8);
    
    if ((gDLL_7_Newday->vtbl->func8(&projectedX)) || (objSetup->unk23 & 0x10)) {
        cam = camGetMain();
        projectedX = self->srt.transl.x - cam->srt.transl.x;
        projectedY = self->srt.transl.y - cam->srt.transl.y;
        projectedZ = self->srt.transl.z - cam->srt.transl.z;
        objData->unk4 = sqrtf(SQ(projectedX) + SQ(projectedY) + SQ(projectedZ));
        
        player = objGetPlayer();
        projectedX = self->srt.transl.x - player->srt.transl.x;
        projectedZ = self->srt.transl.z - player->srt.transl.z;
        distance = (u16)sqrtf(SQ(projectedX) + SQ(projectedZ));

        if (distance < objData->unk6) {
            objData->unkB |= 2 | 8;
        }
        
        if (objData->unk4 < objData->unk8) {
            objData->unkB |= 8;
            pointX = self->srt.transl.x - gWorldX;
            pointY = self->srt.transl.y;
            pointZ = self->srt.transl.z - gWorldZ;
            camProjectPoint(pointX, pointY, pointZ, &projectedX, &projectedY, &projectedZ);
            camClipToScreen(projectedX, projectedY, projectedZ, &screenX, &screenY, NULL);
            zDepthOcclude = viObjDepth(screenX, screenY, self);
            camGetVec3ToCameraNormalized(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], &projectedX, &projectedY, &projectedZ);
            camProjectPoint(pointX += (projectedX * 20.0f), pointY += (projectedY * 20.0f), pointZ += (projectedZ * 20.0f), &projectedX, &projectedY, &projectedZ);
            camClipToScreen(projectedX, projectedY, projectedZ, NULL, NULL, &zDepth);
            if ((viContainsPoint(screenX, screenY) != 0) && (zDepth > 0) && (zDepth < zDepthOcclude)) {
                objData->unkB |= 4;
            }
        }
    }

    if (objData->unkB != objData->unkC) {
        if ((objData->unkB & 2) && !(objData->unkC & 2)) {
            lfxAction(self, self, objData->unk0, 0, 0, 0);
        } else if (((objData->unkB & 2) == 0) && (objData->unkC & 2)) {
            lfxAction(self, self, objData->unk2, 0, 0, 0);
        }

        if ((objData->unkB & 8) && !(objData->unkC & 8)) {
            objData->unk10 = gDLL_6_AMSFX->vtbl->Play(self, 0x612, MAX_VOLUME, NULL, NULL, 0, NULL);
        } else if (((objData->unkB & 8) == 0) && (objData->unkC & 8)) {
            gDLL_6_AMSFX->vtbl->Stop(objData->unk10);
        }
    }
    
    objData->unkC = objData->unkB;
}

// offset: 0x738 | func: 2 | export: 2
void DF_Lantern_obj_Update(Object* self) { }

// offset: 0x744 | func: 3 | export: 3
void DF_Lantern_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DF_Lantern_Data* objData;
    u16 opacity;

    objData = self->data;
    
    if (visibility && (objData->unkB & 4)) {
        opacity = (1.0f - ((f32)objData->unk4 / objData->unk8)) * 512.0f;
        if (opacity > 0xFF) {
            opacity = 0xFF;
        }
        gDLL_17_partfx->vtbl->spawn(self, 0x42B, NULL, 0x12, -1, &opacity);
    }
}

// offset: 0x8C8 | func: 4 | export: 4
void DF_Lantern_obj_Free(Object* self, s32 onlySelf) {
    DF_Lantern_Data* objData = self->data;
    
    if (objData->unkB & 2) {
        lfxAction(self, self, objData->unk2, 0, 0, 0);
    }
    
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x94C | func: 5 | export: 5
u32 DF_Lantern_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x95C | func: 6 | export: 6
u32 DF_Lantern_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DF_Lantern_Data);
}
