#include "common.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    u16 unk20;
    u16 unk22;
    u8 unk24;
    s32 unk28;
} WaterBaddie_DataActual;

/*0x0*/ static s32 data_0[] = {
    2, 3
};
/*0x8*/ static u16 data_8[] = { 0x053f, 0x0540 };
/*0xC*/ static u16 data_C[] = {
    0x0541, 0x0542, 0x0543, 0x0547, 0x053e
};
/*0x18*/ static s32 data_18[] = {
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002
};
/*0x88*/ static s8 data_88[] = {
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};

/*0x0*/ static ObjFSA_StateCallback bss_0[4];
/*0x10*/ static ObjFSA_StateCallback bss_10[6];

static int WaterBaddie_func_9D0(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void WaterBaddie_func_C6C(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void WaterBaddie_func_1088(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void WaterBaddie_func_1238(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void WaterBaddie_func_13DC(Object* self, Baddie* fsa, ObjFSA_Data* baddie);
static void WaterBaddie_func_16FC(Object* self, Baddie* fsa, ObjFSA_Data* baddie);

static s32 WaterBaddie_func_1C48(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_func_1CA4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_func_1D04(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 WaterBaddie_func_1D8C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_func_1E50(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_func_1F70(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_func_203C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_func_247C(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void WaterBaddie_func_0(void) {
    bss_0[0] = WaterBaddie_func_1C48;
    bss_0[1] = WaterBaddie_func_1CA4;
    bss_0[2] = WaterBaddie_func_1D04;
    
    bss_10[0] = WaterBaddie_func_1D8C;
    bss_10[1] = WaterBaddie_func_1E50;
    bss_10[2] = WaterBaddie_func_1F70;
    bss_10[3] = WaterBaddie_func_203C;
    bss_10[4] = WaterBaddie_func_247C;
}

// offset: 0x84 | ctor
void WaterBaddie_ctor(void* dll) {
    WaterBaddie_func_0();
}

// offset: 0xC4 | dtor
void WaterBaddie_dtor(void* dll) { }

// offset: 0xD0 | func: 1 | export: 0
void WaterBaddie_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    WaterBaddie_DataActual* objData;
    s32 count;
    f32 temp;
    s32 i;
    TrackHeightResult** trackResult;
    u8 flags;

    baddie = self->data;
    
    flags = 0x10;
    if (reset) {
        flags = 0x11;
    }
    if (!(objSetup->unk2B & 1)) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 3, 5, 0x100, flags, 20.0f);
    
    self->animCallback = WaterBaddie_func_9D0;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    baddie->fsa.logicState = 4;
    
    objData = baddie->objdata;
    objData->unk1E = self->srt.yaw;
    objData->unk14 = objSetup->unk2F / 100.0f;
    
    count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &trackResult, 0, 0);
    objData->unkC = 0.0f;
    
    if (count) {
        for (i = 0, objData->unkC = -9999.0f; i < count; i++){
            temp = trackResult[i]->y - self->srt.transl.y;
            if ((trackResult[i]->unk14 == 0xE) && (objData->unkC < temp)) {
                objData->unkC = temp;
            }
        }
    }    
    
    objData->unkC += self->srt.transl.y;
    objData->unk18 = 0.075f;
}

// offset: 0x3C4 | func: 2 | export: 1
void WaterBaddie_obj_Control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 3, 5, 0x100, 0x30U, 20.0f);
            gDLL_6_AMSFX->vtbl->Play(self, 0xB20U, 0x7FU, NULL, NULL, 0, NULL);
            baddie->fsa.unk33A = 0;
            self->opacity = OBJECT_OPACITY_MAX;
            self->unkAF |= 8;
            baddie->unk3B2 |= 0x100;
        }
    } else {
        if (self->unkE0 == 0) {
            self->srt.transl.x = objSetup->base.x;
            self->srt.transl.y = objSetup->base.y;
            self->srt.transl.z = objSetup->base.z;
            gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
            self->unkE0 = 1;
            return;
        }
        
        if (baddie->unk3B2 & 2) {
            gDLL_33_BaddieControl->vtbl->func9(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 4, 0, 0, 1);
            baddie->unk3B2 &= ~2;
        }
        
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1)) {
            WaterBaddie_func_C6C(self, baddie, &baddie->fsa);
            if ((baddie->fsa.target != NULL) && (baddie->fsa.hitpoints != 0)) {
                WaterBaddie_func_1088(self, 0, baddie, &baddie->fsa);
            } else {
                WaterBaddie_func_1238(self, baddie, &baddie->fsa);
            }
        }
    }
}

// offset: 0x648 | func: 3 | export: 2
void WaterBaddie_obj_Update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, bss_0);
}

// offset: 0x698 | func: 4 | export: 3
void WaterBaddie_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if (visibility && (self->unkDC == 0)) {
        if (baddie->unk3E8) {
            objprintSetBlendColor(0xC8, 0, 0, baddie->unk3E8);
        }
        
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        if (baddie->unk3B2 & 0x60) {
            if (baddie->unk3B2 & 0x20) {
                gDLL_32_modelfx->vtbl->func2(self, 0x330, &baddie->unk3E8);
                gDLL_32_modelfx->vtbl->func2(self, 0x330, &baddie->unk3E8);
            }
            gDLL_32_modelfx->vtbl->func2(self, 0x32F, &baddie->unk3E8);
        }
        
        if (baddie->unk3B2 & 0x100) {
            gDLL_32_modelfx->vtbl->func2(self, 0x333, &baddie->unk3E8);
            gDLL_32_modelfx->vtbl->func2(self, 0x334, &baddie->unk3E8);
            baddie->unk3B2 &= ~0x100;
        }
    }
}

// offset: 0x8E8 | func: 5 | export: 4
void WaterBaddie_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    objFreeObjectType(self, 4);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 1);
}

// offset: 0x98C | func: 6 | export: 5
u32 WaterBaddie_obj_GetModelFlags(Object* self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x99C | func: 7 | export: 6
u32 WaterBaddie_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(WaterBaddie_DataActual);
}

// offset: 0x9B0 | func: 8 | export: 7
s16 WaterBaddie_Func_9B0(Object* self) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x9C0 | func: 9 | export: 8
void WaterBaddie_Func_9C0(Object* self, u8 message) {

}

// offset: 0x9D0 | func: 10
int WaterBaddie_func_9D0(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    Baddie_Setup* objSetup;
    Baddie* baddie;

    objSetup = (Baddie_Setup*)self->setup;
    baddie = self->data;
    
    if (self->unkDC != 0) {
        return 0;
    }
    
    if (self->seqSlot != -1) {
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1) == 0) {
            return 1;
        }
        
        WaterBaddie_func_C6C(self, baddie, &baddie->fsa);
        
        if (baddie->unk3A0 != NO_GAMEBIT && mainGetBits(baddie->unk3A0)) {
            gDLL_3_Animation->vtbl->func21(animData, objSetup->unk2C);
            baddie->unk3A0 = -1;
        }

        switch (baddie->unk3B4) {
        case 2:
            animData->unk7A = 0;
            WaterBaddie_func_1088(self, animData, baddie, &baddie->fsa);
            if (baddie->unk3B4 == 1) {
                baddie->fsa.logicState = 4;
                gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, bss_0, bss_10);
                animData->unk62 = 0;
            }
            break;
        case 1:
            if (gDLL_33_BaddieControl->vtbl->func12(self, animData, baddie, bss_0, bss_10, 0)) {
                gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.17f, 1);
            }
            break;
        case 0:
        default:
            animData->unk7A = -1;
            animData->unk7A &= ~0x40;
            WaterBaddie_func_1238(self, baddie, &baddie->fsa);
            break;
        }
    }
    
    if (self->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        return 0;
    }
    
    return baddie->unk3B4 != 0;
}

// offset: 0xC6C | func: 11
void WaterBaddie_func_C6C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
/*0x28*/ static SRT bss_28;
    Object* player = objGetPlayer();
/*0xA4*/ s16 data_A4[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
/*0xAC*/ s16 data_AC[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    Vec3f d;
    Object* weapon;
    s32 i;
    s32 scaleIdx;
    
    
    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }
    
    if (fsa->target != NULL) {
        d.f[0] = fsa->target->globalPosition.x - self->globalPosition.x;
        d.f[1] = fsa->target->globalPosition.y - self->globalPosition.y;
        d.f[2] = fsa->target->globalPosition.z - self->globalPosition.z;
        fsa->targetDist = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    }
    
    objExprEyeIdle(self, &baddie->unk3BC);
    WaterBaddie_func_13DC(self, baddie, fsa);
    WaterBaddie_func_16FC(self, baddie, fsa);
    
    if (!(baddie->unk3B0 & 1)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 1, 2, baddie->unk3A6, baddie->unk3A4);
    }
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 4, 4, 0);
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, data_18, data_88, 0, &baddie->unk3A8, &bss_28)) {
        weapon = player->linkedObject;
        scaleIdx = ((DLL_Unknown*)weapon->dll)->vtbl->func[19].withOneArgS32(weapon);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }
        
        bss_28.scale = data_A4[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, 0x323, &bss_28, 0x200001, -1, NULL);
        bss_28.transl.x -= self->srt.transl.x;
        bss_28.transl.y -= self->srt.transl.y;
        bss_28.transl.z -= self->srt.transl.z;
        bss_28.scale = data_AC[scaleIdx];
        
        for (i = 0; i < 4; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x324, &bss_28, 2, -1, NULL);
        }
        
        gDLL_6_AMSFX->vtbl->Play(self, data_C[2], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->Play(self, data_C[4], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x1088 | func: 12
void WaterBaddie_func_1088(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa) {
    Baddie_Setup* objSetup = (Baddie_Setup*)self->setup;
    
    self->objhitInfo->unk58 |= 1;
    
    if (fsa->hitpoints != 0) {
        fsa->unk33A = 1;
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2 + 50.0f, 1)) {
            fsa->target = baddie->unk3AC;
            fsa->unk33D = 0;
            if (objSetup->unk24 >= 0) {
                if (animData != NULL) {
                    gDLL_3_Animation->vtbl->func21(animData, objSetup->unk24);
                }
                baddie->unk3B4 = 1;
            } else {
                fsa->target = NULL;
            }
        }
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, 0);
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_10);
    self->animObj = baddie->unk3AC;
}

// offset: 0x1238 | func: 13
void WaterBaddie_func_1238(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    WaterBaddie_DataActual* objData;
    Object* target;

    objData = baddie->objdata;
    
    self->objhitInfo->unk58 &= ~1;
    
    if (fsa->hitpoints != 0) {
        if (baddie->unk3B2 & 8) {
            fsa->logicState = 3;
        }

        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
        if (target != NULL) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 4, 0, 0, 1);
            fsa->unk33D = 0;
            fsa->target = target;
            if (1) {
                objData->unk28 = 0; 
                objData->unk8 = 0;
            }
        }
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_10);
    self->animObj = baddie->unk3AC;
}

// offset: 0x13DC | func: 14
void WaterBaddie_func_13DC(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    WaterBaddie_DataActual* objData; //24
    f32 temp;
    s16 yaw;
    s16 yawDiff;
    s16 angle;

    objData = baddie->objdata;
    self->velocity.y = 0.0f;
    yaw = self->srt.yaw;
    yawDiff = yaw - (objData->unk1E & 0xFFFF);
    CIRCLE_WRAP(yawDiff);
    
    if (yawDiff < -2500) {
        yawDiff = -2500;
    }
    if (yawDiff > 2500) {
        yawDiff = 2500;
    }
    objData->unk1E = yaw;
    
    objData->unk4 += yawDiff >> 1;
    
    objData->unk10 = ((yawDiff / 182 < 0) ? -(yawDiff / 182) : (yawDiff / 182)) * 0.25f;
    if (objData->unk10 > 1.0f) {
        if (objData->unk8 < 0.4f) {
            objData->unk8 += 0.01f;
        } else {
            objData->unk8 = 0.0f;
        }
        objData->unk24 = 1;
    }
    
    temp = objData->unkC - self->srt.transl.y;
    objData->unk1C += 0x400;
    temp += (mathSinInterp(objData->unk1C) / (M_360_DEGREES * 2.0f));
    
    objData->unk0 += ((temp / 50.0f) - objData->unk8) * gUpdateRateF;
    self->srt.transl.y += objData->unk0;
    
    if (objData->unk0 > 10) {
        objData->unk0 = 10;
    }
    if (objData->unk0 < -10) {
        objData->unk0 = -10;
    }
    self->srt.pitch = objData->unk0 * 2048.0f;

    angle = -(self->srt.roll & 0xFFFF);
    CIRCLE_WRAP(angle);
    objData->unk4 += (angle / 182) * gUpdateRateF;
    
    self->srt.roll += (s16)objData->unk4;
    if (self->srt.roll < -M_90_DEGREES) {
        self->srt.roll = -M_90_DEGREES;
    }
    if (self->srt.roll > M_90_DEGREES) {
        self->srt.roll = M_90_DEGREES;
    }
    
    objData->unk0 /= 1.07f;
    objData->unk4 /= 1.04f;
}

// offset: 0x16FC | func: 15
void WaterBaddie_func_16FC(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    WaterBaddie_DataActual* objData;
    SRT sp5C;
    f32 sp58;
    f32 sp54;
    s32 var_a3;
    s32 var_v1;
    f32 var_fv1;
    f32 sp40;

    var_v1 = 0;
    
    objData = baddie->objdata;
    
    var_a3 = objData->unk24;
    if ((objData->unk0 < -0.025f) && (fsa->unk278 < 0.25f)) {
        var_a3 = 1;
    }
    
    if (objData->unk0 < 0.0f) {
        var_fv1 = -objData->unk0;
    } else {
        var_fv1 = objData->unk0;
    }
    
    if (var_fv1 > 1.0f) {
        var_a3 = 2;
    }
    
    if ((objData->unk22 >= 7) && (fsa->unk278 > 0.1f)) {
        var_v1 = 1;
    }
    
    if (var_v1 || var_a3) {
        if (objData->unkC < self->srt.transl.y) {
            sp54 = -(objData->unkC - self->srt.transl.y);
        } else {
            sp54 = objData->unkC - self->srt.transl.y;
        }
        if (sp54 < 14.0f) {
            sp5C.transl.y = objData->unkC;
            if (var_v1 != 0) {
                sp58 = ((1.0f - (sp54 / 14.0f)) * 12.0f) + (fsa->unk278 * 4.0f);
                sp5C.transl.x = self->srt.transl.x - mathSinfInterp(self->srt.yaw) * sp58;
                sp5C.transl.z = self->srt.transl.z - mathCosfInterp(self->srt.yaw) * sp58;
                sp5C.scale = fsa->unk278;
                sp5C.yaw = self->srt.yaw;
                gDLL_17_partfx->vtbl->spawn(self, 0x32A, &sp5C, 0x200001, -1, NULL);
                objData->unk22 = 0;
            }
            
            if (var_a3) {
                if (mathRnd(0x1E, 0x3C) < objData->unk20) {
                    sp58 = fsa->unk278 * 20.0f;
                    sp5C.transl.x = self->srt.transl.x - mathSinfInterp(self->srt.yaw) * sp58;
                    sp5C.transl.z = self->srt.transl.z - mathCosfInterp(self->srt.yaw) * sp58;
                    sp5C.scale = 1.0f - (sp54 / 14.0f);
                    var_v1 = mathRnd(0, 1);
                    gDLL_6_AMSFX->vtbl->Play(self, data_8[var_v1], (s32) (sp5C.scale * 60.0f) + 0x3F, NULL, NULL, 0, NULL);
                    if (var_a3 == 2) {
                        gDLL_17_partfx->vtbl->spawn(self, 0x32C, &sp5C, 0x200001, -1, NULL);
                    } else {
                        gDLL_17_partfx->vtbl->spawn(self, 0x32B, &sp5C, 0x200001, -1, NULL);
                    }
                    objData->unk20 = 0;
                }
            }
        }
        objData->unk24 = 0;
    }

    objData->unk20 += gUpdateRateF;
    objData->unk22 += gUpdateRateF;
}

// offset: 0x1C48 | func: 16
s32 WaterBaddie_func_1C48(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 5);
    
    return 0;
}

// offset: 0x1CA4 | func: 17
s32 WaterBaddie_func_1CA4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    gDLL_33_BaddieControl->vtbl->func3(self, fsa, self->data, 1.0f, 12.0f);
    return 0;
}

// offset: 0x1D04 | func: 18
s32 WaterBaddie_func_1D04(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    WaterBaddie_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objData = baddie->objdata;
        fsa->unk27C = 0.0f;
        if (self->animProgress > 0.35f) {
            objData->unk0 += 4.0f;
        } else {
            objData->unk0 -= 4.0f;
        }
        self->animProgress = 0.0f;
    }
    
    return 0;
}

// offset: 0x1D8C | func: 19
s32 WaterBaddie_func_1D8C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    static f32 bss_40;
    
    if (fsa->enteredLogicState) {
        bss_40 = 0.0f;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
    }
    
    if (fsa->hitpoints <= 0) {
        return 2;
    }
    
    if (bss_40 > 200.0f) {
        return 5;
    } else {
        bss_40 += gUpdateRateF;
    }
    
    return 0;
}

// offset: 0x1E50 | func: 20
s32 WaterBaddie_func_1E50(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        objSendMesg(objGetPlayer(), 0xE0000, self, NULL);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= 8;
        gDLL_6_AMSFX->vtbl->Play(self, 0xB21, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->Play(self, 0xB1F, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return 3;
}

// offset: 0x1F70 | func: 21
s32 WaterBaddie_func_1F70(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    if (fsa->enteredLogicState) {
        baddie = self->data;
        if (baddie->unk39E != NO_GAMEBIT) {
            mainSetBits(baddie->unk39E, FALSE);
        }
        if (baddie->unk39C != NO_GAMEBIT) {
            mainSetBits(baddie->unk39C, TRUE);
        }
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0U);
    }
    
    return 0;
}

// offset: 0x203C | func: 22
s32 WaterBaddie_func_203C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
/*0x44*/ static s32 bss_44;
    
    Baddie* baddie;
    CurvesStruct* sp3C;
    UnkCurvesStruct* curves;
    f32 temp_fa0;
    f32 dx;
    f32 dz;
    f32 var_fa0;
    f32 new_var;
    f32 var_fv1;
    f32 temp;
    WaterBaddie_DataActual* objData;

    baddie = self->data;
    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
    curves = baddie->unk3F8;
    objData = baddie->objdata;
    
    dx = curves->unk0.unk68.x - self->srt.transl.x;
    dz = curves->unk0.unk68.z - self->srt.transl.z;
    temp_fa0 = 10.0f / sqrtf(SQ(dx) + SQ(dz));
    
    if (self->animProgress > 0.01f) {
        self->animProgress -= 0.01f;
    } else {
        self->animProgress = 0.0f;
    }
    
    if ((curves_func_800053B0(&curves->unk0, temp_fa0) || bss_44 != curves->unk0.unk10) && 
        gDLL_26_Curves->vtbl->func_4704(curves) && 
        gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 400.0f, data_0, -1)
    ) {
        baddie->unk3B2 &= ~8;
    }
    
    bss_44 = curves->unk0.unk10;
    if ((fsa->unk278 > 0.15f) && (objData->unk28 == 0)) {
        if (objData->unk0 < 0.0f) {
            var_fv1 = -objData->unk0;
        } else {
            var_fv1 = objData->unk0;
        }
        if (var_fv1 < 0.015f) {
            if (objData->unkC < self->srt.transl.y) {
                var_fv1 = -(objData->unkC - self->srt.transl.y);
            } else {
                var_fv1 = objData->unkC - self->srt.transl.y;
            }
            if ((var_fv1 < 1.0f) && mathRnd(0, 100)) {
                objData->unk28 = mathRnd(25, 75);
            }
        }
    }

    if (objData->unk28 != 0) {
        if (objData->unk8 > 0.4f) {
            if (mathRnd(0, 1) != 0) {
                objData->unk8 = 0.0f;
            }
            objData->unk28 = 0;
            gDLL_6_AMSFX->vtbl->Play(self, data_C[mathRnd(0, 1)], 0x7F, NULL, NULL, 0, NULL);
        } else {
            objData->unk8 += 0.0001f * objData->unk28;
        }
    } else {
        if (objData->unk8 > 0.0f) {
            objData->unk8 /= 1.04f;
        }
    }
    
    gDLL_18_objfsa->vtbl->func6(self, fsa, curves->unk0.unk68.x, curves->unk0.unk68.z, 0, 0, 60.0f);

    new_var = objData->unk10;
    if (objData->unk10) {
        temp = objData->unk14 / new_var;
        var_fa0 = temp;
    } else {
        var_fa0 = objData->unk14;
    }
    
    fsa->xAnalogInput *= var_fa0;
    fsa->yAnalogInput *= var_fa0;

    if (self->animProgress > 0.01f) {
        self->animProgress -= 0.01f;
    }
    
    return 0;
}

// offset: 0x247C | func: 23
s32 WaterBaddie_func_247C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 4;
}
