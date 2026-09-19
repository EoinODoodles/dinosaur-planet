#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s32 unk34;
    u16 unk38;
    u16 unk3A;
    u16 unk3C;
    s16 unk3E; //gamebit
    s16 unk40; //gamebit
} CFExplodeWall_Setup;

typedef struct {
    s32 unk0;
    Vec3f unk4;
    Vec3f unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    Vec3f unk4C;
    f32 unk58;
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    s16 unk68;
    s8 unk6A;
    u8 unk6B;
    u8 unk6C;
    u8 unk6D;
} ExplodeWallFuncC60Arg2; //Part of objData, seems to have one of these for all 15 pieces. Ton of data!

typedef struct {
    ExplodeWallFuncC60Arg2 unk0[15];
    Object* unk690[15];
    s32 unk6CC;
    s32 unk6D0;
    u8 unk6D4;
    u8 unk6D5[15];
    u8 unk6E4;
} CFExplodeWall_Data; //0x6E8;

typedef struct {
    ObjSetup base;
    u8 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
} ExplodePiece_Setup; //TODO: May be the same as CFExplodeWall_Setup?

/*0x0*/ static Vec3f data_0[4][15] = {
    {
        VEC3F(-14657.105, 1870.912, 22338.988), 
        VEC3F(-14653.412, 1868.940, 22350.191), 
        VEC3F(-14664.002, 1872.833, 22332.936), 
        VEC3F(-14666.821, 1872.891, 22322.693), 
        VEC3F(-14650.792, 1873.479, 22332.201), 
        VEC3F(-14664.084, 1869.271, 22325.994), 
        VEC3F(-14647.538, 1870.727, 22332.354), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
    },
    {
        VEC3F(-14629.397, 1899.117, 22331.582), 
        VEC3F(-14628.425, 1896.516, 22370.727), 
        VEC3F(-14603.391, 1894.752, 22351.193), 
        VEC3F(-14605.199, 1899.383, 22373.939), 
        VEC3F(-14610.144, 1894.525, 22322.656), 
        VEC3F(-14650.631, 1896.519, 22319.727), 
        VEC3F(-14656.056, 1896.761, 22350.285), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
    },
    {
        VEC3F(3194.0510, 1941.219, 40863.609), 
        VEC3F(3195.0510, 1935.219, 40911.609), 
        VEC3F(3222.0510, 1939.219, 40886.609), 
        VEC3F(3225.0510, 1944.219, 40917.609), 
        VEC3F(3214.0510, 1912.219, 40855.609), 
        VEC3F(3168.0510, 1935.219, 40849.609), 
        VEC3F(3164.0510, 1941.219, 40885.609), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0)
    },
    {
        VEC3F(3197.656, 1935.569, 40856.269), 
        VEC3F(3198.656, 1929.569, 40904.269), 
        VEC3F(3225.656, 1933.569, 40879.269), 
        VEC3F(3228.656, 1938.569, 40910.269), 
        VEC3F(3217.656, 1906.569, 40848.269), 
        VEC3F(3171.656, 1929.569, 40842.269), 
        VEC3F(3167.656, 1935.569, 40878.269), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
    }
};

/*0x2D0*/ static f32 data_2D0 = 203631;
/*0x2D4*/ static f32 data_2D4 = 203633;
/*0x2D8*/ static f32 data_2D8 = 203638;
/*0x2DC*/ static f32 data_2DC = 203641;
/*0x2E0*/ static u8 data_2E0 = 0x02;

typedef struct {
    s32 objIDWhole;
    s32 objIDPieces;
    s32 unk8;
    u8 unkC;
} ExplodeObjDef;

/*0x2E4*/ static ExplodeObjDef data_2E4[11] = {
    {OBJ_CFExplodeFloor, OBJ_CFExplodePieces, 0x00000858, 0x01 },
    {OBJ_DFdebris1, OBJ_DFdebris1piece, 0x00000001, 0x32 },
    {OBJ_DFdebris2, OBJ_DFdebris2piece, 0x00000001, 0x64 },
    {OBJ_DFdebris3, OBJ_DFdebris3piece, 0x00000001, 0x64}, 
    {OBJ_CFExplodeTunnel, OBJ_CFExplodeTunnPi, 0x00000001, 0x14}, 
    {OBJ_DRSmallExplodeW, OBJ_DRSmallExplodeP, 0x00000858, 0x01}, 
    {OBJ_DRExplodeWall, OBJ_DRExplodePieces, 0x00000858, 0x01}, 
    {OBJ_CCrockDoor, OBJ_CCrockDoorPiece, 0x00000001, 0x01}, 
    {OBJ_CCgrassfloor, OBJ_CCgrassfloorPie, 0x00000001, 0x01}, 
    {OBJ_CFExplodeWall, OBJ_CFWallPieaces, 0x00000858, 0x32}, 
    {OBJ_CCexplodedoor, OBJ_CCexplodedoorPi, 0x00000001, 0x01}
};

static void CFExplodeWall_func_884(Object* self, CFExplodeWall_Setup* objSetup, s32 skipModelCentrepointCalc, CFExplodeWall_Data* objData);
static Object* CFExplodeWall_func_C60(Object* self, s32 objectID, ExplodeWallFuncC60Arg2* arg2, s32 index);

// offset: 0x0 | ctor
void CFExplodeWall_ctor(void* dll) { }

// offset: 0xC | dtor
void CFExplodeWall_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFExplodeWall_obj_Setup(Object* self, CFExplodeWall_Setup* objSetup, s32 reset) {
    CFExplodeWall_Data* objData;
    s32 i;

    objAddObjectType(self, OBJTYPE_ExplodeObj);
    
    objData = self->data;
    
    if (objSetup->unk18 == 0) {
        objData->unk6D4 = 1;
    } else {
        objData->unk6D4 = objSetup->unk18;
    }
    
    objData->unk6CC = 0;
    
    for (i = 0; i < 15; i++) {
        objData->unk690[i] = NULL;
    }
    
    self->srt.yaw = objSetup->unk1A;
    self->srt.pitch = objSetup->unk1C;
    self->srt.roll = objSetup->unk1E;
    
    if (mainGetBits(objSetup->unk3E)) {
        objData->unk6E4 = 2;
    }
}

// offset: 0x108 | func: 1 | export: 1
void CFExplodeWall_obj_Control(Object* self) {
    s32 i;
    CFExplodeWall_Setup* objSetup;
    CFExplodeWall_Data* objData;
    s32 objVal;
    Object* obj;

    objData = self->data;
    objSetup = (CFExplodeWall_Setup*)self->setup;

    //Finished
    if (objData->unk6E4 == 2) {
        return;
    }

    //Waiting to explode
    if (objData->unk6E4 == 0) {
        if (mainGetBits(objSetup->unk40)) {
            CFExplodeWall_func_884(self, objSetup, 0, objData);
            if (objData->unk6D0 != -1) {
                gDLL_6_AMSFX->vtbl->Play(self, objData->unk6D0, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objData->unk6E4 = 1;
            self->opacity = 0;
        }
        return;
    }

    //Exploding
    for (i = 0; i < 15; i++) {
        obj = objData->unk690[i];
        if (obj) {
            objVal = ((DLL_Unknown*)obj->dll)->vtbl->func[7].withOneArgS32(obj);
            
            switch (objVal) {
            case 2:
                mainSetBits(objSetup->unk3E, 1);
                objFreeObject(objData->unk690[i]);
                objData->unk690[i] = NULL;
                break;
            case 0:
                mainSetBits(objSetup->unk3E, 1);
                if (!(objData->unk6CC & (1 << i))) {
                    gDLL_6_AMSFX->vtbl->Play(self, 0x5B5, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objData->unk6CC |= 1 << i;
                }
                break;
            }
        }
    }
}

// offset: 0x310 | func: 2 | export: 2
void CFExplodeWall_obj_Update(Object* self) { }

// offset: 0x31C | func: 3 | export: 3
void CFExplodeWall_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x334 | func: 4 | export: 4
void CFExplodeWall_obj_Free(Object* self, s32 onlySelf) {
    s32 i = OBJTYPE_ExplodeObj;
    CFExplodeWall_Data* objData;

    objData = self->data;
    
    objFreeObjectType(self, i);
    
    if (onlySelf) {
        return;
    }
    
    for (i *= 0; i < 15; i++) {
        if (objData->unk690[i] != NULL) {
            objFreeObject(objData->unk690[i]); 
        } 
    }
}

// offset: 0x3C4 | func: 5 | export: 5
u32 CFExplodeWall_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x3D4 | func: 6 | export: 6
u32 CFExplodeWall_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFExplodeWall_Data);
}

// offset: 0x3E8 | func: 7
static void CFExplodeWall_func_3E8(Object* self, ExplodeWallFuncC60Arg2* piece, CFExplodeWall_Setup* objSetup) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sp38;
    f32 pad;
    f32 magnitude;
    s32 sp28;

    mathRotateRPY((SRT*)&objSetup->unk1A, piece->unk10.f);
    
    piece->unk4C.x = objSetup->base.x + (piece->unk10.x * self->srt.scale);
    piece->unk4C.y = objSetup->base.y + (piece->unk10.y * self->srt.scale);
    piece->unk4C.z = objSetup->base.z + (piece->unk10.z * self->srt.scale);
    
    piece->unk68 = objSetup->unk1A;
    piece->unk66 = objSetup->unk1C;
    piece->unk64 = objSetup->unk1E;
    
    dx = piece->unk10.f[0] - objSetup->unk20;
    dy = piece->unk10.f[1] - objSetup->unk22;
    dz = piece->unk10.f[2] - objSetup->unk24;
    magnitude = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
    
    if (magnitude == 0.0f) {
        return;
    }
    
    sp38 = objSetup->unk2C / (5.0f * magnitude);
    if ((dx != 0.0f) || (dy != 0.0f) || (dz != 0.0f)) {
        guNormalize(&dx, &dy, &dz);
    }
    
    piece->unk40 = dx * sp38;
    piece->unk44 = dy * sp38;
    piece->unk48 = dz * sp38;
    sp28 = (sp38 + 0.5f) * 200.0f;
    
    piece->unk1C = mathRnd(0, sp28) / 50.0f;
    piece->unk20 = mathRnd(0, sp28) / 50.0f;
    piece->unk24 = mathRnd(0, sp28) / 50.0f;
    
    sp38 = objSetup->unk30 / 1000.0f;
    sp28 = (sp38 + 0.5f) * 200.0f;
    
    if (self->velocity.x > 0) {
        piece->unk6C |= 1;
    }
    if (self->velocity.z > 0) {
        piece->unk6C |= 2;
    }
    if (piece->unk1C > 0) {
        piece->unk6C |= 4;
    }
    if (piece->unk20 > 0) {
        piece->unk6C |= 8;
    }
    if (piece->unk24 > 0) {
        piece->unk6C |= 0x10;
    }
    
    piece->unk28 = mathRnd(0, sp28) / 200.0f;
    piece->unk2C = mathRnd(0, sp28) / 200.0f;
    piece->unk30 = mathRnd(0, sp28) / 200.0f;
    piece->unk34 = dx * sp38;
    piece->unk38 = (dy * sp38) - 0.07f;
    piece->unk3C = dz * sp38;

    if (objSetup->unk2E != 0) {
        piece->unk58 = objSetup->unk2E;
    }
    
    piece->unk5C = objSetup->unk38;
    
    if (objSetup->unk38 != 0) {
        piece->unk60 = ((mathRnd(0, 100) + 100) * objSetup->unk38) / 200;
    } else {
        piece->unk60 = -1;
    }
}

// offset: 0x884 | func: 8
void CFExplodeWall_func_884(Object* self, CFExplodeWall_Setup* objSetup, s32 skipModelCentrepointCalc, CFExplodeWall_Data* objData) {
    s32 sumX;
    s32 sumY;
    s32 sumZ;
    Vtx* vertices;
    s32 i;
    s32 vtxIdx;
    s32 objectID;
    s16 count;
    u8 var_s7;
    ModelInstance* modelInstance;
    Model* model;
    ExplodeWallFuncC60Arg2* piece;

    objectID = -1;
    var_s7 = 1;

    if (1) { //fake?
        i = 0;
    }
    for (; i < ARRAYCOUNT(data_2E4); i++) {
        if (self->id == data_2E4[i].objIDWhole) {
            objectID = data_2E4[i].objIDPieces;
            objData->unk6D0 = data_2E4[i].unk8;
            var_s7 = data_2E4[i].unkC;
            //@bug? doesn't break out after finding the ID
        }
    }
    
    if (objectID == -1) {
        return;
    }

    for (i = 0; i < objData->unk6D4; i++) {
        objData->unk6D5[i] = 1;
        (&objData->unk0[i])->unk6D = var_s7;
        
        if (skipModelCentrepointCalc == 0) {
            (&objData->unk0[i])->unk4.x = 0.0f;
            (&objData->unk0[i])->unk4.y = 0.0f;
            (&objData->unk0[i])->unk4.z = 0.0f;
            
            sumX = 0;
            sumY = 0;
            sumZ = 0;
            modelInstance = self->modelInsts[i];
            model = modelInstance->model;
            for (vtxIdx = 0; vtxIdx < model->vertexCount; vtxIdx++) {
                sumX += model->vertices[vtxIdx].v.ob[0];
                sumY += model->vertices[vtxIdx].v.ob[1];
                sumZ += model->vertices[vtxIdx].v.ob[2];
            }
            sumX /= model->vertexCount;
            sumY /= model->vertexCount;
            sumZ /= model->vertexCount;
            (&objData->unk0[i])->unk4.x = sumX;
            (&objData->unk0[i])->unk4.y = sumY;
            (&objData->unk0[i])->unk4.z = sumZ;
        }
        
        (&objData->unk0[i])->unk10.x = (&objData->unk0[i])->unk4.x;
        (&objData->unk0[i])->unk10.y = (&objData->unk0[i])->unk4.y;
        (&objData->unk0[i])->unk10.z = (&objData->unk0[i])->unk4.z;
        CFExplodeWall_func_3E8(self, (&objData->unk0[i]), objSetup);
        (&objData->unk0[i])->unk6B = 0xFF;
        
        if (mainGetBits(objSetup->unk3E)) {
            (&objData->unk0[i])->unk6A = 2;
        } else {
            (&objData->unk0[i])->unk6A = 0;
        }
        
        objData->unk690[i] = CFExplodeWall_func_C60(self, objectID, (&objData->unk0[i]), i);

    }

    
    if (mainGetBits(objSetup->unk3E)) {
        objData->unk6E4 = 1;
        return;
    }
    
    objData->unk6E4 = 0;
}

// offset: 0xC60 | func: 9
Object* CFExplodeWall_func_C60(Object* self, s32 objectID, ExplodeWallFuncC60Arg2* arg2, s32 index) {
    ExplodePiece_Setup* setup;

    setup = objAllocSetup(sizeof(CFExplodeWall_Setup), objectID);
    setup->base.objId = objectID;
    setup->base.loadFlags = 2;
    setup->base.loadDistance = 0xFF;
    setup->base.fadeFlags = 1;
    setup->base.fadeDistance = 0xFF;
    setup->base.x = self->srt.transl.x;
    setup->base.y = self->srt.transl.y;
    setup->base.z = self->srt.transl.z;
    setup->unk20 = arg2->unk40 * 100.0f;
    setup->unk22 = arg2->unk44 * 100.0f;
    setup->unk24 = arg2->unk48 * 100.0f;
    setup->unk1A = arg2->unk68;
    setup->unk1C = arg2->unk66;
    setup->unk1E = arg2->unk64;
    setup->unk2C = arg2->unk1C * arg2->unk6D;
    setup->unk2E = arg2->unk20 * arg2->unk6D;
    setup->unk30 = arg2->unk24 * arg2->unk6D;
    setup->unk32 = arg2->unk28 * 10.0f;
    setup->unk36 = arg2->unk30 * 10.0f;
    setup->unk34 = arg2->unk2C * 10.0f;
    setup->unk26 = arg2->unk34 * 1000.0f;
    setup->unk28 = arg2->unk38 * 1000.0f;
    setup->unk2A = arg2->unk3C * 1000.0f;
    setup->unk18 = index;
    setup->unk38 = arg2->unk5C;
    setup->unk3A = arg2->unk58;
    return objSetupObject(&setup->base, 4 | 1, -1, -1, NULL);
}

/*0x0*/ static const char str_0[] = " Simple Exploder ";
/*0x14*/ static const char str_14[] = " Sorry You have no displacement for the force ";
/*0x44*/ static const char str_44[] = " Warning : DefNumber Lists in Exploder DLL is not set up correctly for this object";
/*0x98*/ static const char str_98[] = " Creating Object ";
