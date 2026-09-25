#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/420_DFropenode.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objhits.h"
#include "sys/objtype.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 unkC[0x1C - 0xC];
    Vec3f unk1C;
    u8 unk28[0x34 - 0x28];
    Object* unk34;
    Object* unk38;
    f32 unk3C;
    f32 unk40;
    s8 unk44;
    s8 unk45;
    u8 unk46;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s16 unk58;
} RopeBaddie_DataActual;

/*0x0*/ static s32 data_0[] = {
    0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 
    0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 
    0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 
    0x00000007, 0x00000007, 0x00000007, 0x00000007
};
/*0x70*/ static s8 data_70[] = {
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff
};
/*0x8C*/ static u32 data_8C[] = {
    0x00000483, 0x00000484, 0x00000486
};
/*0x98*/ static u32 data_98[] = {
    0x00000485, 0x00000000
};

/*0x0*/ static ObjFSA_StateCallback bss_0[10];
/*0x28*/ static ObjFSA_StateCallback bss_28[6];

static void RopeBaddie_func_694(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void RopeBaddie_func_794(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa);
static void RopeBaddie_func_830(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void RopeBaddie_func_8CC(Object* self);

static s32 RopeBaddie_func_AE4(Object* self, RopeBaddie_DataActual* objData);
static int RopeBaddie_func_E84(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

static s32 RopeBaddie_func_EA0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_10C4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1218(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1514(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_15D0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_168C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1768(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1974(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1AA4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1BAC(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 RopeBaddie_func_1CBC(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1E10(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_1E84(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_2174(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_219C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_func_2264(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void RopeBaddie_func_0(void) {
    bss_0[0] = RopeBaddie_func_EA0;
    bss_0[1] = RopeBaddie_func_10C4;
    bss_0[2] = RopeBaddie_func_1218;
    bss_0[3] = RopeBaddie_func_1514;
    bss_0[4] = RopeBaddie_func_15D0;
    bss_0[5] = RopeBaddie_func_168C;
    bss_0[6] = RopeBaddie_func_1768;
    bss_0[7] = RopeBaddie_func_1974;
    bss_0[8] = RopeBaddie_func_1AA4;
    bss_0[9] = RopeBaddie_func_1BAC;
    
    bss_28[0] = RopeBaddie_func_1CBC;
    bss_28[1] = RopeBaddie_func_1E10;
    bss_28[2] = RopeBaddie_func_1E84;
    bss_28[3] = RopeBaddie_func_2174;
    bss_28[4] = RopeBaddie_func_219C;
    bss_28[5] = RopeBaddie_func_2264;
}

// offset: 0xE4 | ctor
void RopeBaddie_ctor(void* dll) {
    RopeBaddie_func_0();
}

// offset: 0x124 | dtor
void RopeBaddie_dtor(void* dll) { }

// offset: 0x130 | func: 1 | export: 0
void RopeBaddie_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    u8 flags;

    baddie = self->data;

    flags = 2;
    if (reset) {
        flags = 3;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0, 0, 0, flags, 20.0f);

    self->animCallback = RopeBaddie_func_E84;

    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    baddie->fsa.logicState = 0;
    baddie->fsa.unk278 = 0.0f;

    objData = baddie->objdata;
    objData->unk34 = NULL;
}

// offset: 0x1F8 | func: 2 | export: 1
void RopeBaddie_obj_Control(Object* self) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    objData = baddie->objdata;
    
    if (self->unkDC) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0xA, 6, 0x10E, 0x36, 20.0f);
            baddie->fsa.logicState = 1;
            baddie->fsa.enteredLogicState = TRUE;
            self->opacity = 0;
        }
        return;
    } 
    
    if (objData->unk34) {
        gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, bss_0, bss_28);
        
        dll_DFropenode(objData->unk38)->func8(objData->unk38, objData->unk48, &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z);
        
        RopeBaddie_func_694(self, baddie, &baddie->fsa);
        if ((baddie->fsa.target != NULL) || (baddie->fsa.hitpoints == 0)) {
            RopeBaddie_func_794(self, 0, baddie, &baddie->fsa);
        } else {
            RopeBaddie_func_830(self, baddie, &baddie->fsa);
        }
    } else {
        RopeBaddie_func_8CC(self);
    }
}

// offset: 0x3F0 | func: 3 | export: 2
void RopeBaddie_obj_Update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, bss_0);
}

// offset: 0x440 | func: 4 | export: 3
void RopeBaddie_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (visibility && (self->unkDC == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (objData->unk50 > 0.0f) {
            gDLL_32_modelfx->vtbl->func2(self, 0x52A, NULL);
        }

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

// offset: 0x600 | func: 5 | export: 4
void RopeBaddie_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    objFreeObjectType(self, OBJTYPE_Baddie);
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0);
}


// offset: 0x670 | func: 6 | export: 5
u32 RopeBaddie_obj_GetModelFlags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_10 | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x680 | func: 7 | export: 6
u32 RopeBaddie_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(RopeBaddie_DataActual);
}

// offset: 0x694 | func: 8
void RopeBaddie_func_694(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    s32 damageType;

    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);

    damageType = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, data_0, data_70, 3, &baddie->unk3A8, NULL);
    if (damageType == Damage_Type_Projectile) {
        baddie->unk3B4 = 2;
        fsa->target = objGetPlayer();
    }
}

// offset: 0x794 | func: 9
void RopeBaddie_func_794(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa) {
    self->objhitInfo->unk58 |= 1;

    if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1) != 0) {
        fsa->target = NULL;
    }
}

// offset: 0x830 | func: 10
void RopeBaddie_func_830(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;

    self->objhitInfo->unk58 &= ~1;

    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
    if (target != NULL) {
        fsa->target = target;
        fsa->unk33D = 0;
    }
}

// offset: 0x8CC | func: 11
void RopeBaddie_func_8CC(Object* self) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    Object** ropes;
    s32 i;
    s32 count;
    f32 sp58;
    f32 sp54;
    s8 sp53;
    s32 yawOffset;

    baddie = self->data;
    ropes = objGetAllOfType(OBJTYPE_RopeNode, &count);
    if (count == 0) {
        return;
    }
    
    objData = baddie->objdata;
    objData->unk34 = NULL;
    objData->unk3C = 200.0f;

    for (i = 0; i < count; i++) {
        if (dll_DFropenode(ropes[i])->func11(ropes[i], self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp58, &sp54, &sp53) && (sp58 < objData->unk3C)) {
            objData->unk34 = ropes[i];
            objData->unk3C = sp58;
            objData->unk40 = sp54;
        }
    }
    
    if (objData->unk34 != NULL) {
        objData->unk48 = objData->unk40;
        objData->unk38 = objData->unk34;
        RopeBaddie_func_AE4(self, objData);

        yawOffset = objData->unk45 == 0;
        self->srt.yaw = objData->unk58 + (yawOffset << 0xF);
        objData->unk54 = objData->unk48 - ((mathRnd(0xA, 0x3C) / 10.0f) * ((objData->unk45 * 2) - 1));
        
        if (objData->unk54 > 1.0f) {
            objData->unk54 = objData->unk54;
        } else {
            objData->unk54 = 1.0f;
        }
        
        if (objData->unk54 < 6.0f) {
            objData->unk54 = objData->unk54;
        } else {
            objData->unk54 = 6.0f;
        }
    }
}

// offset: 0xAE4 | func: 12
s32 RopeBaddie_func_AE4(Object* self, RopeBaddie_DataActual* objData) {
    s32 yawDiff;
    
    dll_DFropenode(objData->unk38)->func7(objData->unk38, (f32*)&objData->unkC);
    dll_DFropenode(objData->unk38)->func8(objData->unk38, objData->unk48, &objData->unk1C.x, &objData->unk1C.y, &objData->unk1C.z);
    
    objData->unk58 = dll_DFropenode(objData->unk38)->func12(objData->unk38);
    objData->unk46 = 0;
    objData->unk4C = objData->unk48;
    objData->unk4 = objData->unk1C.y;
    objData->unk8 = self->srt.transl.y;
    objData->unk0 = objData->unk1C.y - objData->unk8;
    
    yawDiff = self->srt.yaw - (objData->unk58 & 0xFFFF);
    CIRCLE_WRAP(yawDiff);

    objData->unk45 = !(yawDiff > (M_90_DEGREES - 4)) && !(yawDiff < -(M_90_DEGREES - 4));
    
    return 0;
}

// offset: 0xC10 | func: 13
static s16 RopeBaddie_func_C10(RopeBaddie_DataActual* objData) {
    s32 angle;
    f32 Ax;
    f32 Ay;
    f32 Az;
    f32 Bx;
    f32 By;
    f32 Bz;

    dll_DFropenode(objData->unk38)->func8(objData->unk38, objData->unk48 - 0.1f, &Ax, &Ay, &Az);
    dll_DFropenode(objData->unk38)->func8(objData->unk38, objData->unk48 + 0.1f, &Bx, &By, &Bz);
    
    Ax -= Bx;
    Ay -= By;
    Az -= Bz;
    
    Ax = sqrtf(SQ(Ax) + SQ(Az));
    angle = mathAtan2f(Ay, Ax);    
    return (s16)angle * ((objData->unk45 * 2) - 1);
    
}

// offset: 0xD48 | func: 14
static s32 RopeBaddie_func_D48(RopeBaddie_DataActual* objData) {
    if (objData->unk48 < 0.3f) {
        objData->unk48 = 0.3f;
        return 1;
    }
    
    if (objData->unk48 > 6.7f) {
        objData->unk48 = 6.7f;
        return 1;
    }
    
    return 0;
}

// offset: 0xDAC | func: 15
static void RopeBaddie_func_DAC(Object* self) {
    gDLL_6_AMSFX->vtbl->Play(self, 0x482, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0xE08 | func: 16
static void RopeBaddie_func_E08(Object* self, ObjFSA_Data* fsa) {
    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        gDLL_6_AMSFX->vtbl->Play(self, 0x768, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0xE84 | func: 17
int RopeBaddie_func_E84(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0;
}

// offset: 0xEA0 | func: 18
s32 RopeBaddie_func_EA0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    RopeBaddie_DataActual* objData;
    Baddie* baddie;
    u16 sp3E;
    s16 sp3C;
    u16 sp3A;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.03f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    dll_DFropenode(objData->unk38)->func9(objData->unk38, &objData->unk48, (1 - (objData->unk45 * 2)) * fsa->unk278);
    
    RopeBaddie_func_D48(objData);
    
    gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 0x10U, &sp3E, &sp3C, &sp3A);
    
    if ((sp3E >= 4) && (sp3E < 0xC) && (sp3A > 0x190)) {
        if ((objData->unk48 > 2.0f) && (objData->unk48 < 5.0f)) {
            return 3;
        }
    }

    if (((objData->unk45 != (0, objData->unk54 <= objData->unk48)) != 0) && fsa->unk33A) { //fake
        return 3;
    }
    
    RopeBaddie_func_E08(self, fsa);
    self->srt.pitch = RopeBaddie_func_C10(objData);
    
    return 0;
}

// offset: 0x10C4 | func: 19
s32 RopeBaddie_func_10C4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    RopeBaddie_DataActual* objData;
    Baddie* baddie;
    
    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    RopeBaddie_func_E08(self, fsa);
    
    dll_DFropenode(objData->unk38)->func9(objData->unk38, &objData->unk48, (1 - (objData->unk45 * 2)) * fsa->animTickDelta * 50.4f);
    
    if (RopeBaddie_func_D48(objData)) {
        return 7;
    }
    
    self->srt.pitch = RopeBaddie_func_C10(objData);
    
    return 0;
}

// offset: 0x1218 | func: 20
s32 RopeBaddie_func_1218(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u16 sp4E;
    s16 sp4C;
    u16 sp4A;
    f32 var_fv1;
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    s32 temp;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 3, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.03f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 9);

    dll_DFropenode(objData->unk38)->func9(objData->unk38, &objData->unk48, (1 - (objData->unk45 * 2)) * fsa->unk278);
    
    RopeBaddie_func_D48(objData);
    self->srt.pitch = RopeBaddie_func_C10(objData) * (1.0f - (2.0f * self->animProgress));
    
    if (fsa->unk33A) {
        gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 0x10, &sp4E, &sp4C, &sp4A);
        objData->unk45 = 1 - objData->unk45;

        temp = objData->unk45 == 0;
        self->srt.yaw = objData->unk58 + (temp << 0xF);
        
        var_fv1 = (mathRnd(50, 100) / 100.0f) * ((objData->unk45 * 2) - 1);
        if ((sp4E < 4) || (sp4E >= 0xC)) {
            if (sp4A >= 0x1F5) {
                var_fv1 *= 1.0f + (sp4A / 100.0f);
            } else {
                var_fv1 *= 1.0f + (sp4A / 300.0f);
            }
        }
        
        objData->unk54 = objData->unk48 - var_fv1;
        
        if (objData->unk54 > 1.0f) {
            objData->unk54 = objData->unk54;
        } else {
            objData->unk54 = 1.0f;
        }

        if (objData->unk54 < 6.0f) {
            objData->unk54 = objData->unk54;
        } else {
            objData->unk54 = 6.0f;
        }

        return 4;
    }
    
    return 0;
}

// offset: 0x1514 | func: 21
s32 RopeBaddie_func_1514(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 2, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->animTickDelta = 0.025f;
    self->srt.pitch = RopeBaddie_func_C10(objData);

    if (fsa->unk33A) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x15D0 | func: 22
s32 RopeBaddie_func_15D0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.03f;
    
    self->srt.pitch = RopeBaddie_func_C10(objData);

    if (fsa->unk33A) {
        return 6;
    } else {
        return 0;
    }
}

// offset: 0x168C | func: 23
s32 RopeBaddie_func_168C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 6, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.03f;
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 0, data_98);
    self->srt.pitch = RopeBaddie_func_C10(objData);
    
    return 0;
}

// offset: 0x1768 | func: 24
s32 RopeBaddie_func_1768(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    RopeBaddie_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;

    objData = baddie->objdata;
    self->objhitInfo->unk5F = 9;
    self->objhitInfo->unk60 = 1;
    func_80028D2C(self);
    
    if (mathRnd(0, 100) < 50) {
        if (fsa->enteredAnimState) {
            objAnimSet(self, 1, 0.0f, 0);
            fsa->unk33A = 0;
        }
    } else {
        if (fsa->enteredAnimState) {
            objAnimSet(self, 4, 0.0f, 0);
            fsa->unk33A = 0;
        }
    }
    
    fsa->animTickDelta = 0.03f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), data_8C);

    dll_DFropenode(objData->unk38)->func9(objData->unk38, &objData->unk48, (1 - (objData->unk45 * 2)) * fsa->unk278);
    
    RopeBaddie_func_D48(objData);
    self->srt.pitch = RopeBaddie_func_C10(objData);
    
    if (fsa->unk33A) {
        return 5;
    } else {
        return 0;
    }
}

// offset: 0x1974 | func: 25
s32 RopeBaddie_func_1974(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 yawDiff;
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 7, 0.0f, 0);
        fsa->unk33A = 0;
    }
    if (fsa->enteredAnimState) {
        RopeBaddie_func_DAC(self);
    }
    fsa->animTickDelta = 0.018f;
    
    yawDiff = self->srt.yaw - (objData->unk58 & 0xFFFF);
    CIRCLE_WRAP(yawDiff);
    self->srt.yaw = objData->unk58;
    
    if ((yawDiff > (M_90_DEGREES - 4)) || (yawDiff < -(M_90_DEGREES - 4))) {
        self->srt.yaw += M_180_DEGREES;
    }
    
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->unk33A) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x1AA4 | func: 26
s32 RopeBaddie_func_1AA4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 8, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.048f;
    
    if (fsa->unk308 & 0x200) {
        gDLL_6_AMSFX->vtbl->Play(self, 0xB1F, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk308 &= ~0x200;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 1);
    }
    
    return 0;
}

// offset: 0x1BAC | func: 27
s32 RopeBaddie_func_1BAC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 0;
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        gDLL_6_AMSFX->vtbl->Play(self, 0xB20, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (fsa->enteredAnimState) {
            objAnimSet(self, 2, 0.0f, 0);
            fsa->unk33A = 0;
        }
        fsa->animTickDelta = 0.025f;
        fsa->unk33A = 0;
        self->opacity = OBJECT_OPACITY_MAX;
        baddie->unk3B2 |= 0x100;
    }
    
    if (fsa->unk33A) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x1CBC | func: 28
s32 RopeBaddie_func_1CBC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* target;
    RopeBaddie_DataActual* objData;
    u16 sp36;
    s16 sp34;
    u16 sp32;
    Baddie* baddie;

    target = fsa->target;
    baddie = self->data;
    objData = baddie->objdata;
    
    if (target != NULL) {
        if ((dll_player(target)->func45(target) == objData->unk38) && (fsa->animState != 2) && ((4.0f * gUpdateRateF) < fsa->logicStateTime)) {
            gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 0x10, &sp36, &sp34, &sp32);
            if (( sp36 < 4) || (sp36 >= 0xC)) {
                return 3;
            }
            
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
            fsa->animTickDelta = 0.028f;
            fsa->unk33A = 0;
        }
    }
    
    return 0;
}

// offset: 0x1E10 | func: 29
s32 RopeBaddie_func_1E10(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 9);
    }
    
    if (fsa->unk33A) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x1E84 | func: 30
s32 RopeBaddie_func_1E84(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    f32 distance;
    Object* target;
    f32 dx;
    f32 dz;
    s32 yawDiff;

    target = fsa->target;
    if ((target == NULL) || dll_player(target)->func45(target) == 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
        return 1;
    }
    
    if (fsa->animState != 6) {
        dx = self->srt.transl.x - target->srt.transl.x;
        dz = self->srt.transl.z - target->srt.transl.z;
        yawDiff = (mathAtan2f(dx, dz) - self->srt.yaw) & 0xFFFF;
        if ((yawDiff > M_90_DEGREES) && (yawDiff < M_90_DEGREES * 3)) {
            dx = -100.0f;
        } else {
            dx = sqrtf(SQ(dx) + SQ(dz)) - 45.0f;
        }
        
        if (dx < 0.0f) {
            distance = -dx;
        } else {
            distance = dx;
        }
        
        if (distance < 1.0f && (fsa->animState == 1 || (fsa->animState == 5 && fsa->unk33A))) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        } else {
            if (fsa->animState != 1) {
                if ((dx > 2.5f) && (fsa->animState != 4) && (fsa->animState != 5 || fsa->unk33A)) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
                }
                if (dx < -2.5f) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
                }
            }
        }
        
        if (fsa->animState == 1) {
            if (dx > 0.0f) {
                fsa->animTickDelta = 0.04f;
            } else {
                fsa->animTickDelta = -0.07f;
            }
        }
    }
    
    return 0;
}

// offset: 0x2174 | func: 31
s32 RopeBaddie_func_2174(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return 5;
    } else {
        return 1;
    }
}

// offset: 0x219C | func: 32
s32 RopeBaddie_func_219C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 8);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= 8;
    }
    
    if (self->opacity == 0) {
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        return 6;
    } else {
        return 0;
    }
}

// offset: 0x2264 | func: 33
s32 RopeBaddie_func_2264(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39E, FALSE);
        mainSetBits(baddie->unk39C, TRUE);
    }
    
    return 0;
}
