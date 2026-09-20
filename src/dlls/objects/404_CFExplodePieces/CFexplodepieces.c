#include "common.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"

typedef struct {
    ObjSetup base;
    u8 modelIdx;
    s16 yaw;
    s16 pitch;
    s16 roll;
    Vec3s16 unk20;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u16 unk38;
    u16 unk3A;
    s16 unk3C;
    s16 gamebitFinished;
    s16 gamebitExplode;
} CFExplodePieces_Setup;

typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    Vec3f unk18;
    Vec3f unk24;
    Vec3f acceleration;
    u8 _unk3C[0x54 - 0x3C];
    f32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 _unk60[0x66 - 0x60];
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 unk69;
    u8 _unk6A[0x6C - 0x6A];
} CFExplodePieces_Data;

static s32 CFExplodePieces_func_16C(Object* self, CFExplodePieces_Data* objData);
static s32 CFExplodePieces_func_1E0(Object* self, CFExplodePieces_Data* objData);
static void CFExplodePieces_func_804(Object* self, CFExplodePieces_Setup* objSetup, s32 reset, CFExplodePieces_Data* objData);

// offset: 0x0 | ctor
void CFExplodePieces_ctor(void* dll) { }

// offset: 0xC | dtor
void CFExplodePieces_dtor(void* dll) { }

/*0x0*/ static const char str_0[] = "phys %i ";

// offset: 0x18 | func: 0 | export: 0
void CFExplodePieces_obj_Setup(Object* self, CFExplodePieces_Setup* objSetup, s32 reset) {
    CFExplodePieces_Data* objData;

    objData = self->data;
    self->modelInstIdx = objSetup->modelIdx;

    CFExplodePieces_func_804(self, objSetup, reset, objData);
    
    if (objSetup->unk20.x || objSetup->unk20.y || objSetup->unk20.z || objSetup->unk26 || objSetup->unk28 || objSetup->unk2A) {
        objData->unk69 = 1;
    } else {
        objData->unk69 = 0;
    }
}

// offset: 0xC4 | func: 1 | export: 1
void CFExplodePieces_obj_Control(Object* self) {
    CFExplodePieces_Data* objData = self->data;
    
    switch (objData->unk69) {
    case 0:
    case 2:
        break;
    case 1:
        if (CFExplodePieces_func_1E0(self, objData)) {
            objData->unk69 = 0;
        }
        break;
    }
    
    if (CFExplodePieces_func_16C(self, objData)) {
        objData->unk69 = 2;
    }
}

// offset: 0x16C | func: 2
s32 CFExplodePieces_func_16C(Object* self, CFExplodePieces_Data* objData) {
    s32 opacity;

    if (objData->unk5C != -1) {
        objData->unk58 += gUpdateRate;
        if (objData->unk58 >= objData->unk5C) {
            objData->unk5C = -1;
            self->opacity = 0;
            self->srt.flags |= OBJFLAG_INVISIBLE;
            return 1;
        } else {
            opacity = objData->unk5C - objData->unk58;
            if (opacity < OBJECT_OPACITY_MAX) {
                self->opacity = opacity;
            }
        }
    }
    
    return 0;
}

// offset: 0x1E0 | func: 3
s32 CFExplodePieces_func_1E0(Object* self, CFExplodePieces_Data* objData) {
    f32 lateralSpeed;

    self->velocity.x += gUpdateRateF * objData->acceleration.x;
    self->velocity.y += gUpdateRateF * objData->acceleration.y;
    self->velocity.z += gUpdateRateF * objData->acceleration.z;
    
    objData->unk18.x += gUpdateRateF * objData->unk24.x;
    objData->unk18.y += gUpdateRateF * objData->unk24.y;
    objData->unk18.z += gUpdateRateF * objData->unk24.z;

    self->srt.transl.x += self->velocity.x * gUpdateRateF;
    self->srt.transl.y += self->velocity.y * gUpdateRateF;
    self->srt.transl.z += self->velocity.z * gUpdateRateF;
    
    if (self->srt.transl.y < objData->unk54) {
        self->velocity.y = -self->velocity.y * 0.5f;
        objData->acceleration.y = -0.07f;
        objData->unk24.z = -objData->unk24.z;
        if (((self->velocity.y > 0) && (objData->unk66 & 4)) || (self->velocity.y == 0)) {
            objData->acceleration.y = 0;
            objData->unk24.z = 0;
            objData->unk18.z = 0;
            objData->unk24.y = 0;
            objData->unk18.y = 0;
            objData->unk24.x = 0;
            objData->unk18.x = 0;
            
            self->velocity.y = 0;
            
            objData->acceleration.x *= 0.6f;
            self->velocity.x *= 0.6f;
            objData->acceleration.z *= 0.6f;
            self->velocity.z *= 0.6f;
            
            if (self->velocity.x >= 0.0f) {
                lateralSpeed = self->velocity.x;
            } else {
                lateralSpeed = -self->velocity.x;
            }
            
            if (lateralSpeed < 0.15f) {
                if (self->velocity.z >= 0.0f) {
                    lateralSpeed = self->velocity.z;
                } else {
                    lateralSpeed = -self->velocity.z;
                }
                
                if (lateralSpeed < 0.15f) {
                    return 1;
                }
            }
        }

        objData->unk66 |= 4;
    } else {
        objData->unk66 &= ~4;
    }
    
    self->srt.yaw += objData->unk18.x * gUpdateRateF;
    self->srt.pitch += objData->unk18.y * gUpdateRateF;
    self->srt.roll += objData->unk18.z * gUpdateRateF;
    
    return 0;
}

// offset: 0x4B8 | func: 4 | export: 2
void CFExplodePieces_obj_Update(Object* self) { }

// offset: 0x4C4 | func: 5 | export: 3
void CFExplodePieces_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

/*0xC*/ static const char str_C[] = " Freeing Physic Obj ";

// offset: 0x518 | func: 6 | export: 4
void CFExplodePieces_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x528 | func: 7 | export: 5
u32 CFExplodePieces_obj_GetModelFlags(Object* self) {
    CFExplodePieces_Setup* setup = (CFExplodePieces_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(setup->modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x540 | func: 8 | export: 6
u32 CFExplodePieces_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFExplodePieces_Data);
}

// offset: 0x554 | func: 9 | export: 7
u8 CFExplodePieces_Func_554(Object* self) {
    CFExplodePieces_Data* objData = self->data;
    return objData->unk69;
}

/*0x24*/ static const char str_24[] = " Hieght From Ground %i ";

// offset: 0x564 | func: 10
static void CFExplodePieces_func_564(Object* self, CFExplodePieces_Data* objData, CFExplodePieces_Setup* objSetup) {
    f32 floorHeight = 0.0f;
    
    self->srt.transl.x = objSetup->base.x + (objData->unkC.x * self->srt.scale);
    self->srt.transl.y = objSetup->base.y + (objData->unkC.y * self->srt.scale);
    self->srt.transl.z = objSetup->base.z + (objData->unkC.z * self->srt.scale);
    self->srt.yaw = objSetup->yaw;
    self->srt.pitch = objSetup->pitch;
    self->srt.roll = objSetup->roll;
    self->velocity.x = objSetup->unk20.x / 100.0f;
    self->velocity.y = objSetup->unk20.y / 100.0f;
    self->velocity.z = objSetup->unk20.z / 100.0f;
    objData->unk18.x = objSetup->unk2C;
    objData->unk18.y = objSetup->unk2E;
    objData->unk18.z = objSetup->unk30;

    if (objSetup->unk3A == 0) {
        trackGetHeightFloor(self, self->srt.transl.x, self->srt.transl.y - 10.0f, self->srt.transl.z, &floorHeight, 0);
        objData->unk54 = self->srt.transl.y - floorHeight;
    } else {
        objData->unk54 = self->srt.transl.y + (s16) objSetup->unk3A;
    }
    
    objData->unk24.x = objSetup->unk32 / 10.0f;
    objData->unk24.y = objSetup->unk34 / 10.0f;
    objData->unk24.z = objSetup->unk36 / 10.0f;
    objData->acceleration.x = objSetup->unk26 / 1000.0f;
    objData->acceleration.y = objSetup->unk28 / 1000.0f;
    objData->acceleration.z = objSetup->unk2A / 1000.0f;
    objData->unk58 = 0;
    
    if (objSetup->unk38) {
        objData->unk5C = ((mathRnd(0, 100) + 100) * objSetup->unk38) / 200;
    } else {
        objData->unk5C = -1;
    }
}

// offset: 0x804 | func: 11
void CFExplodePieces_func_804(Object* self, CFExplodePieces_Setup* objSetup, s32 reset, CFExplodePieces_Data* objData) {
    Vtx* vtxs0;
    Vtx* vtxs1;
    s32 i;
    s32 averageX;
    s32 averageY;
    s32 averageZ;
    ModelInstance* modelInstance;
    Model* model;
    
    if (reset == FALSE) {
        objData->unk0.z = (objData->unk0.y = (objData->unk0.x = 0.0f));
        
        averageX = 0;
        averageY = 0;
        averageZ = 0;
        
        modelInstance = self->modelInsts[objSetup->modelIdx];
        model = modelInstance->model;
        
        for (i = 0; i < model->vertexCount; i++) {
            averageX += model->vertices[i].v.ob[0];
            averageY += model->vertices[i].v.ob[1];
            averageZ += model->vertices[i].v.ob[2];
        }
        averageX /= model->vertexCount;
        averageY /= model->vertexCount;
        averageZ /= model->vertexCount;
        
        vtxs0 = modelInstance->vertices[0];
        vtxs1 = modelInstance->vertices[1];
        
        for (i = 0; i < model->vertexCount; i++) {
            vtxs0[i].v.ob[0] -= averageX;
            vtxs0[i].v.ob[1] -= averageY;
            vtxs0[i].v.ob[2] -= averageZ;
            if (vtxs1 != vtxs0) {
                vtxs1[i].v.ob[0] -= averageX;
                vtxs1[i].v.ob[1] -= averageY; 
                vtxs1[i].v.ob[2] -= averageZ; 
            } 
        } 
        
        objData->unk0.x = averageX; 
        objData->unk0.y = averageY; 
        objData->unk0.z = averageZ; 
    }
    
    objData->unkC.x = objData->unk0.x;
    objData->unkC.y = objData->unk0.y;
    objData->unkC.z = objData->unk0.z;
    
    CFExplodePieces_func_564(self, objData, objSetup);
    
    objData->unk67 = 0xFF;
}
