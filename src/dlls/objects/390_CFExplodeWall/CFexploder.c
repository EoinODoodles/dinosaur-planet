#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/objtype.h"

#define MAX_PIECES 15

typedef struct {
    ObjSetup base;
    u8 pieceCount;
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
    s16 unk3A;
    s16 unk3C;
    s16 gamebitFinished;
    s16 gamebitExplode;
} CFExplodeWall_Setup;

typedef struct {
    s32 unk0;
    Vec3f centrepoint;
    Vec3f unk10;
    Vec3f unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    Vec3f unk40;
    Vec3f unk4C;
    f32 unk58;
    s32 unk5C;
    s32 unk60;
    s16 roll;
    s16 pitch;
    s16 yaw;
    u8 unk6A;
    u8 unk6B;
    u8 flags;
    u8 unk6D;
} CFExplodeWall_PieceData;

typedef struct {
    CFExplodeWall_PieceData pieceData[MAX_PIECES];
    Object* pieceObjs[MAX_PIECES];
    s32 piecesSoundBitfield;
    s32 explodeSoundID;
    u8 pieceCount;
    u8 piecesConfigured[MAX_PIECES];
    u8 state;
} CFExplodeWall_Data;

typedef enum {
    CFExplodeWall_STATE_0_Waiting,
    CFExplodeWall_STATE_1_Exploding,
    CFExplodeWall_STATE_2_Finished
} CFExplodeWall_States;

typedef enum {
    CFExplodePiece_FLAG_1 = 1,
    CFExplodePiece_FLAG_2 = 2,
    CFExplodePiece_FLAG_4 = 4,
    CFExplodePiece_FLAG_8 = 8,
    CFExplodePiece_FLAG_10 = 0x10,
    CFExplodePiece_FLAG_20 = 0x20,
    CFExplodePiece_FLAG_40 = 0x40,
    CFExplodePiece_FLAG_80 = 0x80
} CFExplodePiece_Flags;

/* Unused, but seem to be worldSpace coords for the pieces! */
/*0x0*/ static Vec3f dPieceCoords[4][MAX_PIECES] = {
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
/*0x2E0*/ static u8 data_2E0 = 2;

typedef struct {
    s32 objIDWhole;
    s32 objIDPieces;
    s32 unk8;
    u8 unkC;
} ExplodeObjDef;

/*0x2E4*/ static ExplodeObjDef dExplodeDefs[11] = {
    { OBJ_CFExplodeFloor,  OBJ_CFExplodePieces, 0x858, 1 },
    { OBJ_DFdebris1,       OBJ_DFdebris1piece,  1,     50 },
    { OBJ_DFdebris2,       OBJ_DFdebris2piece,  1,     100 },
    { OBJ_DFdebris3,       OBJ_DFdebris3piece,  1,     100 }, 
    { OBJ_CFExplodeTunnel, OBJ_CFExplodeTunnPi, 1,     20 }, 
    { OBJ_DRSmallExplodeW, OBJ_DRSmallExplodeP, 0x858, 1 }, 
    { OBJ_DRExplodeWall,   OBJ_DRExplodePieces, 0x858, 1 }, 
    { OBJ_CCrockDoor,      OBJ_CCrockDoorPiece, 1,     1 }, 
    { OBJ_CCgrassfloor,    OBJ_CCgrassfloorPie, 1,     1 }, 
    { OBJ_CFExplodeWall,   OBJ_CFWallPieaces,   0x858, 50 }, 
    { OBJ_CCexplodedoor,  OBJ_CCexplodedoorPi, 1,     1 }
};

static void CFExplodeWall_explode(Object* self, CFExplodeWall_Setup* objSetup, s32 skipModelCentrepointCalc, CFExplodeWall_Data* objData);
static Object* CFExplodeWall_createPiece(Object* self, s32 objectID, CFExplodeWall_PieceData* arg2, s32 index);

// offset: 0x0 | ctor
void CFExplodeWall_ctor(void* dll) { }

// offset: 0xC | dtor
void CFExplodeWall_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFExplodeWall_obj_Setup(Object* self, CFExplodeWall_Setup* objSetup, s32 reset) {
    CFExplodeWall_Data* objData;
    u32 i;

    objAddObjectType(self, OBJTYPE_ExplodeObj);
    
    objData = self->data;
    
    if (objSetup->pieceCount == 0) {
        objData->pieceCount = 1;
    } else {
        objData->pieceCount = objSetup->pieceCount;
    }
    
    objData->piecesSoundBitfield = 0;
    
    for (i = 0; i < ARRAYCOUNT(objData->pieceObjs); i++) {
        objData->pieceObjs[i] = NULL;
    }
    
    self->srt.yaw = objSetup->yaw;
    self->srt.pitch = objSetup->pitch;
    self->srt.roll = objSetup->roll;
    
    if (mainGetBits(objSetup->gamebitFinished)) {
        objData->state = CFExplodeWall_STATE_2_Finished;
    }
}

// offset: 0x108 | func: 1 | export: 1
void CFExplodeWall_obj_Control(Object* self) {
    u32 i;
    CFExplodeWall_Setup* objSetup;
    CFExplodeWall_Data* objData;
    s32 pieceVal;
    Object* obj;

    objData = self->data;
    objSetup = (CFExplodeWall_Setup*)self->setup;

    //Finished
    if (objData->state == CFExplodeWall_STATE_2_Finished) {
        return;
    }

    //Waiting to explode
    if (objData->state == CFExplodeWall_STATE_0_Waiting) {
        if (mainGetBits(objSetup->gamebitExplode)) {
            CFExplodeWall_explode(self, objSetup, FALSE, objData);
            if (objData->explodeSoundID != NO_SOUND) {
                gDLL_6_AMSFX->vtbl->Play(self, objData->explodeSoundID, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objData->state = CFExplodeWall_STATE_1_Exploding;
            self->opacity = 0;
        }
        return;
    }

    //Exploding
    for (i = 0; i < ARRAYCOUNT(objData->pieceObjs); i++) {
        obj = objData->pieceObjs[i];
        if (obj) {
            pieceVal = ((DLL_Unknown*)obj->dll)->vtbl->func[7].withOneArgS32(obj); //TODO: interface
            
            switch (pieceVal) {
            case 2:
                //Free piece
                mainSetBits(objSetup->gamebitFinished, TRUE);
                objFreeObject(objData->pieceObjs[i]);
                objData->pieceObjs[i] = NULL;
                break;
            case 0:
                //Play piece impact sound
                mainSetBits(objSetup->gamebitFinished, TRUE);
                if ((objData->piecesSoundBitfield & (1 << i)) == FALSE) {
                    gDLL_6_AMSFX->vtbl->Play(self, SOUND_5B5_Explosion_Debris_Crash, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objData->piecesSoundBitfield |= 1 << i;
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
    
    for (i *= 0; i < ARRAYCOUNT_S(objData->pieceObjs); i++) {
        if (objData->pieceObjs[i] != NULL) {
            objFreeObject(objData->pieceObjs[i]); 
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

/*0x0*/ static const char str_0[] = " Simple Exploder ";

// offset: 0x3E8 | func: 7
static void CFExplodeWall_calculatePieceData(Object* self, CFExplodeWall_PieceData* piece, CFExplodeWall_Setup* objSetup) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sp38;
    f32 pad;
    f32 displacement;
    s32 sp28;

    mathRotateRPY((SRT*)&objSetup->yaw, piece->unk10.f);
    
    piece->unk4C.x = objSetup->base.x + (piece->unk10.x * self->srt.scale);
    piece->unk4C.y = objSetup->base.y + (piece->unk10.y * self->srt.scale);
    piece->unk4C.z = objSetup->base.z + (piece->unk10.z * self->srt.scale);
    
    piece->yaw = objSetup->yaw;
    piece->pitch = objSetup->pitch;
    piece->roll = objSetup->roll;
    
    dx = piece->unk10.x - objSetup->unk20.x;
    dy = piece->unk10.y - objSetup->unk20.y;
    dz = piece->unk10.z - objSetup->unk20.z;
    displacement = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
    
    if (displacement == 0.0f) {
        STUBBED_PRINTF(" Sorry You have no displacement for the force ");
        return;
    }
    
    sp38 = objSetup->unk2C / (5.0f * displacement);
    if ((dx != 0.0f) || (dy != 0.0f) || (dz != 0.0f)) {
        guNormalize(&dx, &dy, &dz);
    }
    
    piece->unk40.x = dx * sp38;
    piece->unk40.y = dy * sp38;
    piece->unk40.z = dz * sp38;
    sp28 = (sp38 + 0.5f) * 200.0f;
    
    piece->unk1C.x = mathRnd(0, sp28) / 50.0f;
    piece->unk1C.y = mathRnd(0, sp28) / 50.0f;
    piece->unk1C.z = mathRnd(0, sp28) / 50.0f;
    
    sp38 = objSetup->unk30 / 1000.0f;
    sp28 = (sp38 + 0.5f) * 200.0f;
    
    if (self->velocity.x > 0) {
        piece->flags |= CFExplodePiece_FLAG_1;
    }
    if (self->velocity.z > 0) {
        piece->flags |= CFExplodePiece_FLAG_2;
    }
    if (piece->unk1C.x > 0) {
        piece->flags |= CFExplodePiece_FLAG_4;
    }
    if (piece->unk1C.y > 0) {
        piece->flags |= CFExplodePiece_FLAG_8;
    }
    if (piece->unk1C.z > 0) {
        piece->flags |= CFExplodePiece_FLAG_10;
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
void CFExplodeWall_explode(Object* self, CFExplodeWall_Setup* objSetup, s32 skipModelCentrepointCalc, CFExplodeWall_Data* objData) {
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
    CFExplodeWall_PieceData* piece;

    objectID = -1;
    var_s7 = 1;

    if (1) { //fake?
        i = 0;
    }
    for (; i < ARRAYCOUNT_S(dExplodeDefs); i++) {
        if (self->id == dExplodeDefs[i].objIDWhole) {
            objectID = dExplodeDefs[i].objIDPieces;
            objData->explodeSoundID = dExplodeDefs[i].unk8;
            var_s7 = dExplodeDefs[i].unkC;
            //@bug? doesn't break out after finding the matching objIDWhole
        }
    }
    
    if (objectID == -1) {
        STUBBED_PRINTF(" Warning : DefNumber Lists in Exploder DLL is not set up correctly for this object");
        return;
    }

    for (i = 0; i < objData->pieceCount; i++) {
        objData->piecesConfigured[i] = TRUE;
        (&objData->pieceData[i])->unk6D = var_s7;
        
        if (skipModelCentrepointCalc == FALSE) {
            (&objData->pieceData[i])->centrepoint.x = 0.0f;
            (&objData->pieceData[i])->centrepoint.y = 0.0f;
            (&objData->pieceData[i])->centrepoint.z = 0.0f;
            
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
            (&objData->pieceData[i])->centrepoint.x = sumX;
            (&objData->pieceData[i])->centrepoint.y = sumY;
            (&objData->pieceData[i])->centrepoint.z = sumZ;
        }
        
        (&objData->pieceData[i])->unk10.x = (&objData->pieceData[i])->centrepoint.x;
        (&objData->pieceData[i])->unk10.y = (&objData->pieceData[i])->centrepoint.y;
        (&objData->pieceData[i])->unk10.z = (&objData->pieceData[i])->centrepoint.z;
        CFExplodeWall_calculatePieceData(self, (&objData->pieceData[i]), objSetup);
        (&objData->pieceData[i])->unk6B = 0xFF;
        
        if (mainGetBits(objSetup->gamebitFinished)) {
            (&objData->pieceData[i])->unk6A = 2;
        } else {
            (&objData->pieceData[i])->unk6A = 0;
        }
        
        objData->pieceObjs[i] = CFExplodeWall_createPiece(self, objectID, &objData->pieceData[i], i);
    }

    if (mainGetBits(objSetup->gamebitFinished)) {
        objData->state = CFExplodeWall_STATE_1_Exploding;
        return;
    }
    
    objData->state = CFExplodeWall_STATE_0_Waiting;
}

/*0x98*/ static const char str_98[] = " Creating Object ";

// offset: 0xC60 | func: 9
Object* CFExplodeWall_createPiece(Object* self, s32 objectID, CFExplodeWall_PieceData* pieceData, s32 index) {
    CFExplodeWall_Setup* setup;

    setup = objAllocSetup(sizeof(CFExplodeWall_Setup), objectID);
    setup->base.objId = objectID;
    setup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    setup->base.loadDistance = 0xFF;
    setup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    setup->base.fadeDistance = 0xFF;
    setup->base.x = self->srt.transl.x;
    setup->base.y = self->srt.transl.y;
    setup->base.z = self->srt.transl.z;
    setup->unk20.x = pieceData->unk40.x * 100.0f;
    setup->unk20.y = pieceData->unk40.y * 100.0f;
    setup->unk20.z = pieceData->unk40.z * 100.0f;
    setup->yaw = pieceData->yaw;
    setup->pitch = pieceData->pitch;
    setup->roll = pieceData->roll;
    setup->unk2C = pieceData->unk1C.x * pieceData->unk6D;
    setup->unk2E = pieceData->unk1C.y * pieceData->unk6D;
    setup->unk30 = pieceData->unk1C.z * pieceData->unk6D;
    setup->unk32 = pieceData->unk28 * 10.0f;
    setup->unk36 = pieceData->unk30 * 10.0f;
    setup->unk34 = pieceData->unk2C * 10.0f;
    setup->unk26 = pieceData->unk34 * 1000.0f;
    setup->unk28 = pieceData->unk38 * 1000.0f;
    setup->unk2A = pieceData->unk3C * 1000.0f;
    setup->pieceCount = index;
    setup->unk38 = pieceData->unk5C;
    setup->unk3A = pieceData->unk58;
    return objSetupObject(&setup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, NULL);
}
