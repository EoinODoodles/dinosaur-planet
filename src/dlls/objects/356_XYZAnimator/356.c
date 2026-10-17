#include "common.h"
#include "sys/map.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s8 unk28;
    s8 unk29;
    s8 unk2A;
    s8 unk2B;
    u8 unk2C;
    u16 unk2E;
} XYZAnimator_Setup;

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32* unk14;
    s16* unk18; //new coords for each animated vertex?
    s16* unk1C; //bounds
    s16* unk20;
    s16* unk24;
    s16* unk28;
    s16* unk2C;
    s16* unk30;
    Vec3f unk34;
    s8 unk40; //animatorID
    s8 unk41;
    s8 unk42;
    s8 unk43;
} XYZAnimator_Data; //0x44

static void XYZAnimator_func_1174(XYZAnimator_Data* objData, XYZAnimator_Setup* objSetup, Block* block);
static void XYZAnimator_func_17EC(Object* self, XYZAnimator_Data* objData);
static void XYZAnimator_func_1A04(Object* self, XYZAnimator_Data* objData);

// offset: 0x0 | ctor
void XYZAnimator_ctor(void* dll) { }

// offset: 0xC | dtor
void XYZAnimator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void XYZAnimator_obj_Setup(Object* self, XYZAnimator_Setup* objSetup, s32 reset) {
    XYZAnimator_Data* objData = self->data;
    
    objData->unk43 = -1;
}

// offset: 0x34 | func: 1 | export: 1
void XYZAnimator_obj_Control(Object* self) {
    XYZAnimator_Setup* objSetup;
    XYZAnimator_Data* objData;
    Block* block;
    s32 var_v0;
    s32 count;
    
    objSetup = (XYZAnimator_Setup*)self->setup;
    objData = self->data;
    
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objData->unk42 = 0;
        return;
    }
    
    if ((block->vtxFlags & 8) == FALSE) {
        return;
    }
    
    if (objData->unk4 == 0) {
        objData->unk40 = objSetup->unk28;
        objData->unk4 = blockGetAnimatorVertexCount(self, objData->unk40);
        objData->unk0 = blockGetAnimatorShapeCount(self, objData->unk40);
        if (objData->unk4 == 0) {
            objData->unk40 = 0;
        }
        
        if (objData->unk40 == 0) {
            return;
        }
        
        objData->unk8 = 0.0f;
        objData->unkC = 0.0f;
        objData->unk10 = objSetup->unk2E;
        
        if (objSetup->unk18 == NO_GAMEBIT) {
            objData->unk41 = 1;
        } else {
            objData->unk41 = mainGetBits(objSetup->unk18);
        }
        
        objData->unk34.x = objSetup->unk1C;
        objData->unk34.y = objSetup->unk1E;
        objData->unk34.z = objSetup->unk20;
        
        if (objSetup->unk1A != NO_GAMEBIT && mainGetBits(objSetup->unk1A)) {
            objData->unk34.x = objSetup->unk22;
            objData->unk34.y = objSetup->unk24;
            objData->unk34.z = objSetup->unk26;
            objData->unk41 = 1;
            objData->unk8 = objData->unk10 + 1.0f;
        }
        
        if (objSetup->unk2C == 3) {
            objData->unk14 = mmAlloc(objData->unk4 * 4, 5, NULL);
            XYZAnimator_func_17EC(self, objData);
        }
        
        objData->unk18 = mmAlloc((objData->unk4 * (sizeof(s16))) * 3, ALLOC_TAG_TRACK_COL, NULL);
        objData->unk1C = mmAlloc(objData->unk0 * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
        objData->unk20 = mmAlloc(objData->unk0 * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
        objData->unk24 = mmAlloc(objData->unk0 * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
        objData->unk28 = mmAlloc(objData->unk0 * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
        objData->unk2C = mmAlloc(objData->unk0 * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
        objData->unk30 = mmAlloc(objData->unk0 * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
        XYZAnimator_func_1A04(self, objData);
        
        if (objSetup->unk2C != 4) {
            XYZAnimator_func_1174(objData, objSetup, block);
            block->vtxFlags ^= 1;
            XYZAnimator_func_1174(objData, objSetup, block);
            block->vtxFlags ^= 1;
        }
    }
    
    if (objData->unk40 == 0) {
        return;
    }
    
    if (objSetup->unk2C == 2) {
        objData->unk41 = mainGetBits(objSetup->unk18);
        if (objData->unk42 >= 3) {
            if (objData->unk41 != objData->unk43) {
                objData->unk42 = 0;
                objData->unk43 = objData->unk41;
            }
        }
        
        if (objData->unk42 >= 3) {
            return;
        }
    } else if ((objData->unk42 >= 3)) {
        return;
    } else if (objData->unk41 == 0) {
        objData->unk41 = mainGetBits(objSetup->unk18);
        if (objData->unk41 == 0) {
            return;
        }
    }
    
    switch (objSetup->unk2C) {
    case 0:
    case 4:
        count = 0;
        if (objSetup->unk22 < objSetup->unk1C) {
            objData->unk34.x -= (objSetup->unk29 * gUpdateRate) * 0.1f;
            if (objData->unk34.x <= objSetup->unk22) {
                objData->unk34.x = objSetup->unk22;
                count = 1;
            }
        } else {
            objData->unk34.x += (objSetup->unk29 * gUpdateRate) * 0.1f;
            if (objData->unk34.x >= objSetup->unk22) {
                objData->unk34.x = objSetup->unk22;
                count = 1;
            }
        }
        
        if (objSetup->unk24 < objSetup->unk1E) {
            objData->unk34.y -= (objSetup->unk2A * gUpdateRate) * 0.1f;
            if (objData->unk34.y <= objSetup->unk24) {
                objData->unk34.y = objSetup->unk24;
                count++;
            }
        } else {
            objData->unk34.y += (objSetup->unk2A * gUpdateRate) * 0.1f;
            if (objData->unk34.y >= objSetup->unk24) {
                objData->unk34.y = objSetup->unk24;
                count++;
            }
        }
        
        if (objSetup->unk26 < objSetup->unk20) {
            objData->unk34.z -= (objSetup->unk2B * gUpdateRate) * 0.1f;
            if (objData->unk34.z <= objSetup->unk26) {
                objData->unk34.z = objSetup->unk26;
                count++;
            }
        } else {
            objData->unk34.z += (objSetup->unk2B * gUpdateRate) * 0.1f;
            if (objData->unk34.z >= objSetup->unk26) {
                objData->unk34.z = objSetup->unk26;
                count++;
            }
        }
        
        if (count == 3) {
            if (objSetup->unk1A != NO_GAMEBIT) {
                mainSetBits(objSetup->unk1A, 1);
            }
            objData->unk42++;
        }
        break;
    case 1:
        if (objSetup->unk22 < objSetup->unk1C) {
            objData->unk34.x -= (objSetup->unk29 * gUpdateRate) * 0.1f;
            if (objData->unk34.x < objSetup->unk22) {
                var_v0 = objSetup->unk22 - objData->unk34.x;
                objData->unk34.x = objSetup->unk1C - var_v0;
            }
        } else {
            objData->unk34.x += (objSetup->unk29 * gUpdateRate) * 0.1f;
            if (objSetup->unk1C < objData->unk34.x) {
                var_v0 = objData->unk34.x - objSetup->unk22;
                objData->unk34.x = objSetup->unk22 + var_v0;
            }
        }
        
        if (objSetup->unk24 < objSetup->unk1E) {
            objData->unk34.y -= (objSetup->unk2A * gUpdateRate) * 0.1f;
            if (objData->unk34.y < objSetup->unk24) {
                var_v0 = objSetup->unk24 - objData->unk34.y;
                objData->unk34.y = objSetup->unk1E - (var_v0 * 0.1f);
            }
        } else {
            objData->unk34.y += (objSetup->unk2A * gUpdateRate) * 0.1f;
            if (objSetup->unk1E < objData->unk34.y) {
                var_v0 = objData->unk34.y - objSetup->unk24;
                objData->unk34.y = objSetup->unk24 + var_v0;
            }
        }
        
        if (objSetup->unk26 < objSetup->unk20) {
            objData->unk34.z -= (objSetup->unk2B * gUpdateRate) * 0.1f;
            if (objData->unk34.z < objSetup->unk26) {
                var_v0 = objSetup->unk26 - objData->unk34.z;
                objData->unk34.z = objSetup->unk20 - var_v0;
            }
        } else {
            objData->unk34.z += (objSetup->unk2B * gUpdateRate) * 0.1f;
            if (objSetup->unk20 < objData->unk34.z) {
                var_v0 = objData->unk34.z - objSetup->unk26;
                if (((!objSetup->unk26) && (!objSetup->unk26)) && (!objSetup->unk26)) { } //fake
                objData->unk34.z = objSetup->unk26 + var_v0;
            }
        }
        break;
    case 2:
        count = 0;
        if (objData->unk41) {
            if (objSetup->unk22 < objSetup->unk1C) {
                objData->unk34.x -= (objSetup->unk29 * gUpdateRate) * 0.1f;
                if (objData->unk34.x <= objSetup->unk22) {
                    objData->unk34.x = objSetup->unk22;
                    count = 1;
                }
            } else {
                objData->unk34.x += (objSetup->unk29 * gUpdateRate) * 0.1f;
                if (objData->unk34.x >= objSetup->unk22) {
                    objData->unk34.x = objSetup->unk22;
                    count = 1;
                }
            }
            
            if (objSetup->unk24 < objSetup->unk1E) {
                objData->unk34.y -= (objSetup->unk2A * gUpdateRate) * 0.1f;
                if (objData->unk34.y <= objSetup->unk24) {
                    objData->unk34.y = objSetup->unk24;
                    count++;
                }
            } else {
                objData->unk34.y += (objSetup->unk2A * gUpdateRate) * 0.1f;
                if (objData->unk34.y >= objSetup->unk24) {
                    objData->unk34.y = objSetup->unk24;
                    count++;
                }
            }
            
            if (objSetup->unk26 < objSetup->unk20) {
                objData->unk34.z -= (objSetup->unk2B * gUpdateRate) * 0.1f;
                if (objData->unk34.z <= objSetup->unk26) {
                objData->unk34.z = objSetup->unk26;
                count++;
                }
            } else {
                objData->unk34.z += (objSetup->unk2B * gUpdateRate) * 0.1f;
                if (objData->unk34.z >= objSetup->unk26) {
                    objData->unk34.z = objSetup->unk26;
                    count++;
                }
            }
            
            if (count == 3) {
                if (objSetup->unk1A != NO_GAMEBIT) {
                    mainSetBits(objSetup->unk1A, 1);
                }
                objData->unk42++; 
            } 
        } else {
            if (objSetup->unk22 < objSetup->unk1C) { 
                objData->unk34.x += (objSetup->unk29 * gUpdateRate) * 0.1f; 
                if (objData->unk34.x >= objSetup->unk1C) { 
                    objData->unk34.x = objSetup->unk1C; 
                    count = 1; 
                } 
            } else {
                objData->unk34.x -= (objSetup->unk29 * gUpdateRate) * 0.1f;
                if (objData->unk34.x <= objSetup->unk1C) {
                    objData->unk34.x = objSetup->unk1C;
                    count = 1;
                }
            }
            
            if (objSetup->unk24 < objSetup->unk1E) {
                objData->unk34.y += (objSetup->unk2A * gUpdateRate) * 0.1f;
                if (objData->unk34.y >= objSetup->unk1E) {
                    objData->unk34.y = objSetup->unk1E;
                    count++;
                }
            } else {
                objData->unk34.y -= (objSetup->unk2A * gUpdateRate) * 0.1f;
                if (objData->unk34.y <= objSetup->unk1E) {
                    objData->unk34.y = objSetup->unk1E;
                    count++;
                }
            }
            
            if (objSetup->unk26 < objSetup->unk20) {
                objData->unk34.z += (objSetup->unk2B * gUpdateRate) * 0.1f;
                if (objData->unk34.z >= objSetup->unk20) {
                    objData->unk34.z = objSetup->unk20;
                    count++;
                }
            } else {
                objData->unk34.z -= (objSetup->unk2B * gUpdateRate) * 0.1f;
                if (objData->unk34.z <= objSetup->unk20) {
                    objData->unk34.z = objSetup->unk20;
                    count++;
                }
            }
            
            if (count == 3) {
                if (objSetup->unk1A != NO_GAMEBIT) {
                    mainSetBits(objSetup->unk1A, 1);
                }
                objData->unk42++;
            }
        }
        break;
    case 3:
        objData->unk8 += (objSetup->unk29 * gUpdateRateF) * 0.1f;
        if (objData->unk8 > objData->unk10) {
            objData->unk8 = objData->unk10;
            mainSetBits(objSetup->unk1A, 1);
            objData->unk42++;
        }
        objData->unkC = objData->unk8 - 50.0f;
        break;
    }
    
    XYZAnimator_func_1174(objData, objSetup, block);
}

// offset: 0x1174 | func: 2
static void XYZAnimator_func_1174(XYZAnimator_Data* objData, XYZAnimator_Setup* objSetup, Block* block) {
    f32 tValue;
    s32 vtxIdx;
    s16 bounds;
    s32 animVtxIdx;
    s32 animShapeIdx;
    s32 shapeIdx;
    BlockShape* shapes;
    BlockVertex* vertices;

    vertices = (BlockVertex*)block->vertices2[(block->vtxFlags & 1)];
    
    shapeIdx = 0;
    animVtxIdx = 0;
    animShapeIdx = 0;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->unk40 == shapes[shapeIdx].animatorID) {
            //Update bounds
            {
                shapes[shapeIdx].Ymin = objData->unk1C[animShapeIdx] + objData->unk34.y;
                
                bounds = objData->unk24[animShapeIdx] + objData->unk34.x;
                shapes[shapeIdx].Xmin = bounds >> 2;
                shapes[shapeIdx].boundsRemainder = bounds & 3;
                
                bounds = objData->unk2C[animShapeIdx] + objData->unk34.z;
                shapes[shapeIdx].Zmin = bounds >> 2;
                shapes[shapeIdx].boundsRemainder |= (bounds & 3) << 2;
                
                shapes[shapeIdx].Ymax = objData->unk20[animShapeIdx] + objData->unk34.y;
                
                bounds = objData->unk28[animShapeIdx] + objData->unk34.x;
                shapes[shapeIdx].Xmax = bounds >> 2;
                shapes[shapeIdx].boundsRemainder |= (bounds & 3) << 4;
                
                bounds = objData->unk30[animShapeIdx] + objData->unk34.z;
                shapes[shapeIdx].Zmax = bounds >> 2;
                shapes[shapeIdx].boundsRemainder |= (bounds & 3) << 6;
            }
                
            animShapeIdx++;

            //Update vertices
            for (vtxIdx = shapes[shapeIdx].vtxBase; vtxIdx < shapes[shapeIdx + 1].vtxBase; vtxIdx++, animVtxIdx++) {
                if (objSetup->unk2C != 3) {
                    vertices[vtxIdx].ob[0] = objData->unk18[(animVtxIdx * 3) + 0] + objData->unk34.x;
                    vertices[vtxIdx].ob[1] = objData->unk18[(animVtxIdx * 3) + 1] + objData->unk34.y;
                    vertices[vtxIdx].ob[2] = objData->unk18[(animVtxIdx * 3) + 2] + objData->unk34.z;
                    block->vertices[vtxIdx].ob[0] = vertices[vtxIdx].ob[0];
                    block->vertices[vtxIdx].ob[1] = vertices[vtxIdx].ob[1];
                    block->vertices[vtxIdx].ob[2] = vertices[vtxIdx].ob[2];
                } else {
                    tValue = (objData->unk14[animVtxIdx] - objData->unkC) * 0.02f;
                    if (tValue > 1.0f) {
                        tValue = 1.0f;
                    } else if (tValue < 0.0f) {
                        tValue = 0.0f;
                    }
                    vertices[vtxIdx].ob[0] = objData->unk18[(animVtxIdx * 3) + 0] + objData->unk34.x + (tValue * (objSetup->unk22 - objSetup->unk1C));
                    vertices[vtxIdx].ob[1] = objData->unk18[(animVtxIdx * 3) + 1] + objData->unk34.y + (tValue * (objSetup->unk24 - objSetup->unk1E));
                    vertices[vtxIdx].ob[2] = objData->unk18[(animVtxIdx * 3) + 2] + objData->unk34.z + (tValue * (objSetup->unk26 - objSetup->unk20));
                    block->vertices[vtxIdx].ob[0] = vertices[vtxIdx].ob[0];
                    block->vertices[vtxIdx].ob[1] = vertices[vtxIdx].ob[1];
                    block->vertices[vtxIdx].ob[2] = vertices[vtxIdx].ob[2];
                }
            }
        }
    }
}

// offset: 0x15DC | func: 3 | export: 2
void XYZAnimator_obj_Update(Object* self) { }

// offset: 0x15E8 | func: 4 | export: 3
void XYZAnimator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x163C | func: 5 | export: 4
void XYZAnimator_obj_Free(Object* self, s32 onlySelf) {
    s32 pad[2];
    XYZAnimator_Setup* objSetup;
    XYZAnimator_Data* objData;
    Block* block;

    objData = self->data;
    objSetup = (XYZAnimator_Setup*)self->setup;
    
    objData->unk34.x = 0;
    objData->unk34.y = 0;
    objData->unk34.z = 0;
    
    if (onlySelf == FALSE) {
        block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
        if ((block != NULL) && (objData->unk4 != 0)) {
            XYZAnimator_func_1174(objData, objSetup, block);
        }
    }
    
    if (objData->unk14 != NULL) {
        mmFree(objData->unk14);
    }
    if (objData->unk18 != NULL) {
        mmFree(objData->unk18);
    }
    if (objData->unk1C != NULL) {
        mmFree(objData->unk1C);
    }
    if (objData->unk20 != NULL) {
        mmFree(objData->unk20);
    }
    if (objData->unk24 != NULL) {
        mmFree(objData->unk24);
    }
    if (objData->unk28 != NULL) {
        mmFree(objData->unk28);
    }
    if (objData->unk2C != NULL) {
        mmFree(objData->unk2C);
    }
    if (objData->unk30 != NULL) {
        mmFree(objData->unk30);
    }
}

// offset: 0x17C8 | func: 6 | export: 5
u32 XYZAnimator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x17D8 | func: 7 | export: 6
u32 XYZAnimator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(XYZAnimator_Data);
}

// offset: 0x17EC | func: 8
void XYZAnimator_func_17EC(Object* self, XYZAnimator_Data* objData) {
    Block* block;
    BlockShape* shapes;
    f32 localX;
    f32 localZ;
    f32 dx;
    f32 dz;
    s32 vtxIdx;
    s32 blockWorldGridX;
    s32 blockWorldGridZ;
    s32 animVtxIdx;
    s32 shapeIdx;

    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if ((block == NULL) || (block->vtxFlags & 8) == FALSE) {
        return;
    }
    
    blockWorldGridX = floorf((self->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    blockWorldGridZ = floorf((self->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);

    localX = self->srt.transl.x - ((blockWorldGridX * BLOCKS_GRID_UNIT_F) + gWorldX);
    localZ = self->srt.transl.z - ((blockWorldGridZ * BLOCKS_GRID_UNIT_F) + gWorldZ);
    
    shapes = block->shapes;

    for (shapeIdx = 0, animVtxIdx = 0; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->unk40 == shapes[shapeIdx].animatorID) {
            for (vtxIdx = shapes[shapeIdx].vtxBase; vtxIdx < shapes[shapeIdx + 1].vtxBase; vtxIdx++) {
                dx = block->vertices[vtxIdx].ob[0];
                dx -= localX;
                dz = block->vertices[vtxIdx].ob[2];
                dz -= localZ;
                objData->unk14[animVtxIdx] = sqrtf(SQ(dx) + SQ(dz));
                
                animVtxIdx++;
            }
        }
    }
}

// offset: 0x1A04 | func: 9
void XYZAnimator_func_1A04(Object* self, XYZAnimator_Data* objData) {
    Block* block;
    BlockShape* shapes;
    s32 shapeIdx;
    s32 vtxIdx;
    s32 animVtxIdx;
    s32 animShapeIdx;

    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        return;
    }
    
    if ((block->vtxFlags & 8) == FALSE) {
        return;
    }
    
    shapeIdx = 0;
    animVtxIdx = 0;
    animShapeIdx = 0;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->unk40 == shapes[shapeIdx].animatorID) {
            objData->unk1C[animShapeIdx] = shapes[shapeIdx].Ymin;
            objData->unk20[animShapeIdx] = shapes[shapeIdx].Ymax;
            objData->unk24[animShapeIdx] = (shapes[shapeIdx].Xmin * 4) | (shapes[shapeIdx].boundsRemainder & 3);
            objData->unk28[animShapeIdx] = (shapes[shapeIdx].Xmax * 4) | ((shapes[shapeIdx].boundsRemainder >> 4) & 3);
            objData->unk2C[animShapeIdx] = (shapes[shapeIdx].Zmin * 4) | ((shapes[shapeIdx].boundsRemainder >> 2) & 3);
            objData->unk30[animShapeIdx] = (shapes[shapeIdx].Zmax * 4) | ((shapes[shapeIdx].boundsRemainder >> 6) & 3);
            
            for (vtxIdx = shapes[shapeIdx].vtxBase; (vtxIdx < shapes[shapeIdx + 1].vtxBase); vtxIdx++) {
                objData->unk18[(animVtxIdx * 3) + 0] = block->vertices[vtxIdx].ob[0];
                objData->unk18[(animVtxIdx * 3) + 1] = block->vertices[vtxIdx].ob[1];
                objData->unk18[(animVtxIdx * 3) + 2] = block->vertices[vtxIdx].ob[2];
                animVtxIdx++;
            }
            animShapeIdx++;
        }
    }
}
