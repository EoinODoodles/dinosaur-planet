#include "common.h"
#include "sys/gfx/modgfx.h"
#include "sys/objmsg.h"
#include "dlls/objects/210_player.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} WCSunTempleLaser_Setup;

typedef struct {
    Texture* unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u32 unk10;
    u8 _unk14[0x2C - 0x14];
    f32 unk2C;
    f32 unk30;
    u8 unk34;
    u8 unk35;
    s8 unk36;
    s8 unk37;
    s16 unk38;
    s16 unk3A;
    Object* unk3C;
    u8 _unk40[0x48 - 0x40];
    Vec3f unk48;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
} WCSunTempleLaser_Data; //58

/*0x0*/ static DLL_IModgfx* data_0 = NULL;

// offset: 0x0 | ctor
void WCSunTempleLaser_ctor(void* dll) { }

// offset: 0xC | dtor
void WCSunTempleLaser_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCSunTempleLaser_obj_Setup(Object* self, WCSunTempleLaser_Setup* arg1, s32 reset) {
    WCSunTempleLaser_Data* objData = self->data;
    
    objInitMesgQueue(self, 2);
    
    self->srt.yaw = arg1->unk18 << 8;
    
    objData->unk4 = mathRnd(-80, 80) + 400;
    
    data_0 = dllLoad(DLL_ID_145, 1);
    
    objData->unk3A = 280;
    
    if (objData->unk0 == NULL) {
        objData->unk0 = texLoadTexture(TEXTABLE_2E);
    }
    
    objData->unk8 = mathRnd(0, arg1->unk1C);
    objData->unkC = mathRnd(0, arg1->unk1A);
}

// offset: 0x140 | func: 1 | export: 1
void WCSunTempleLaser_obj_Control(Object* self) {
    WCSunTempleLaser_Setup* objSetup; //8C
    WCSunTempleLaser_Data* objData;
    s32 pad1[3];
    f32 sin; //78
    f32 cos; //74
    f32 sp70; //70
    f32 sp6C; //6C
    f32 var_fv0;
    f32 sp64; //64
    s32 pad2[2];
    Object* player; //58
    s32 i; //54
    f32 range;
    s32 pad3[4];
    f32 sp34; //34

    objSetup = (WCSunTempleLaser_Setup*)self->setup;
    objData = self->data;
    objData->unk4 -= gUpdateRateF;
    
    if ((objSetup->unk19) && (mainGetBits(0x338))) {
        mainSetBits(objSetup->unk1E, 0);
    }
    
    if (mainGetBits(objSetup->unk1E)) {
        if (objData->unk57 & 1) {
            if (objSetup->unk19 != 0) {
                objData->unk8 -= gUpdateRateF;
            }
            
            if (objData->unk8 < 0.0f) {
                objData->unkC = objSetup->unk1A;
                objData->unk57 &= ~1;
                
                if (objData->unk10 != 0) {
                    gDLL_6_AMSFX->vtbl->Stop(objData->unk10);
                    objData->unk10 = 0;
                }
                
                gDLL_14_Modgfx->vtbl->func5(self);
                gDLL_6_AMSFX->vtbl->Play(self, 0x2BB, MAX_VOLUME, NULL, NULL, 0, NULL);
            } else {
                if (objData->unk4 < 0.0f) {
                    objData->unk4 = 275.0f;
                    objData->unk54 = 0;
                } else if (objData->unk4 < objData->unk3A) {
                    if (objData->unk54 == 0) {
                        if (objData->unk10 == 0) {
                            gDLL_6_AMSFX->vtbl->Play(self, 0x9FA, MAX_VOLUME, &objData->unk10, NULL, 0, NULL);
                        }
                        
                        if (objData->unk56 == 0) {
                            objData->unk56 = 1;
                            gDLL_6_AMSFX->vtbl->Play(self, 0x9F9, MAX_VOLUME, NULL, NULL, 0, NULL);
                        }
                        
                        objData->unk54 = 1;
                        
                        if (data_0 != NULL) {
                            data_0->vtbl->func0(self, (objSetup->unk19 == 1) ? 0xD : 0xF, NULL, 0x10004, -1, NULL);
                        }
                    }
                    
                    if (objData->unk4 < 140.0f) {
                        if (objData->unk54 == 1) {
                            objData->unk54 = 2;
                            if (data_0 != NULL) {
                                data_0->vtbl->func0(self, (objSetup->unk19 == 1) ? 0xD : 0x10, NULL, 0x10004, -1, NULL);
                            }
                        }
                    } else {
                        if (objData->unk2C <= 1.0f) {
                            objData->unk2C += 0.052f * gUpdateRateF;
                        }
                    }
                }
            }
        } else if (objSetup->unk19 != 0) {
            objData->unkC -= gUpdateRateF;
            if (objData->unkC <= 0.0f) {
                objData->unk8 = objSetup->unk1C;
                objData->unk57 |= 1;
                objData->unk54 = 0;
                objData->unk56 = 0;
                objData->unk4 = 0.0f;
                objData->unk2C = 0.0f;
            }
        } else {
            objData->unk57 |= 1;
        }
    } else {
        objData->unk54 = 0;
        objData->unk56 = 0;
        objData->unkC = 0.0f;
        objData->unk4 = 0.0f;
        objData->unk2C = 0.0f;
    }
    
    if (objData->unk34 != 0) {
        if (objData->unk10 != 0) {
            gDLL_6_AMSFX->vtbl->SetVol(objData->unk10, (s8) (objData->unk2C * 127.0f));
        }
    }
    
    sin = mathSinfInterp(self->srt.yaw);
    cos = mathCosfInterp(self->srt.yaw);
    sp70 = -((self->srt.transl.x * sin) + (self->srt.transl.z * cos));
    
    player = objGetPlayer();
    
    objData->unk37 -= gUpdateRate;
    if (objData->unk37 < 0) {
        objData->unk37 = 0;
    }
    
    if (objData->unk34 != 0) {
        objData->unk38 += gUpdateRate;
        if (objData->unk38 > 0x3C) {
            objData->unk38 = 0x3C;
            objData->unk35 = 1;
        }
    }
    
    if (objData->unk35 == 0) {
        objData->unk34 = objData->unk54 & 3;
    } else {
        objData->unk34 = 1;
    }
    
    if ((mainGetBits(objSetup->unk1E) == 0) || !(objData->unk57 & 1)) {
        objData->unk34 = 0;
    }

    if (player == NULL) {
        return;
    }
    if (objData->unk37 != 0) {
        return;
    }
    if (objData->unk34 == 0) {
        return;
    }

    range = objData->unk36 + 5.0f;
    var_fv0 = player->srt.transl.y - self->srt.transl.y;
    if ((var_fv0 < range) && (-(range + 25.0f) < var_fv0)) {

        sp6C = -cos; 
        sp64 = -((self->srt.transl.x * sp6C) + (self->srt.transl.z * sin)); 
        var_fv0 = (player->srt.transl.x * sp6C) + (sin * player->srt.transl.z) + sp64;

        if ((var_fv0 < range) && (-range < var_fv0)) {

            var_fv0 = (player->srt.transl.x * sin) + (cos * player->srt.transl.z) + sp70;
            
            if ((0.0f < var_fv0) && (var_fv0 < 170.0f)) {
                if (((DLL_210_Player*)player->dll)->vtbl->func50(player) != 0x1D7) {
                    gDLL_6_AMSFX->vtbl->Play(self, 0x228, MAX_VOLUME, NULL, NULL, 0, NULL);
                    
                    for (i = 0; i < 4; i++) {
                        gDLL_17_partfx->vtbl->spawn(objGetPlayer(), 0x28B, NULL, 4, -1, NULL);
                    }
                    
                    if ((var_fv0 = ((player->prevLocalPosition.x * sp6C) + (sin * player->prevLocalPosition.z)) + sp64) < 0.0f) {
                        var_fv0 = -20.0f;
                    } else {
                        var_fv0 = 20.0f;
                    }

                    objData->unk48.f[0] = player->srt.transl.x + (sp6C * var_fv0);
                    objData->unk48.f[2] = player->srt.transl.z + (sin * var_fv0);
                    
                    if ((objData->unk55 == 0) || (objData->unk55 == 1)) {
                        objSendMesg(player, 0x60003, &objData->unk3C, NULL);
                    }
                    
                    objData->unk37 = 0x14;
                } else {
                    mainSetBits(0x468, 1);
                }     
            }
        }
    }
}

// offset: 0x9D4 | func: 2 | export: 2
void WCSunTempleLaser_obj_Update(Object* self) { }

// offset: 0x9E0 | func: 3 | export: 3
void WCSunTempleLaser_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x9F8 | func: 4 | export: 4
void WCSunTempleLaser_obj_Free(Object* self, s32 onlySelf) {
    WCSunTempleLaser_Data* objData = self->data;
    
    if (objData->unk10 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objData->unk10);
    }
    
    gDLL_14_Modgfx->vtbl->func5(self);
    
    dllFree(data_0);

    if (objData->unk0 != NULL) {
        texFreeTexture(objData->unk0);
    }
    objData->unk0 = NULL;
}

// offset: 0xABC | func: 5 | export: 5
u32 WCSunTempleLaser_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xACC | func: 6 | export: 6
u32 WCSunTempleLaser_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCSunTempleLaser_Data);
}
