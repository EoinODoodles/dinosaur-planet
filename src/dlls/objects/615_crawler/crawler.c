#include "common.h"
#include "sys/gfx/animseq.h"
#include "sys/math.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dlls/engine/11_newlfx.h"

//Used by Skeetlas/Scorpions

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} DLL615_Setup;

typedef struct {
    DLL27_Data unk0;
    u32 unk260;
    f32 unk264;
    f32 unk268;
    Vec3f unk26C;
    u8 _unk278[0x280 - 0x278];
    f32 unk280;
    s16 unk284;
    s16 unk286;
    s16 unk288;
    s16 unk28A;
    s16 unk28C;
    s16 unk28E;
    u16 unk290;
    s8 unk292;
    u8 unk293;
    u8 unk294;
    u8 unk295_0 : 1;
    u8 unk295_1 : 7;
    u8 _unk296[0x298 - 0x296];
} DLL615_Data;

/*0x0*/ static u16 _data_0[] = {
    0x0000, 0x0002, 0x0004, 0x0001, 0x000c, 0x03f7, 0x0167, 0x050c
};
/*0x10*/ static f32 _data_10 = 0.1;
/*0x14*/ static u8 _data_14 = 0;
/*0x18*/ static Vec3f _data_18 = VEC3F(0, 0, 0);
/*0x24*/ static f32 _data_24 = 10;

static void dll_615_func_19FC(Object* self, TrackHeightResult* arg1);
static void dll_615_func_1AE8(Object* self);
static int dll_615_func_1AC8(Object* self, Object* animObj, AnimObj_Data* animData);

// offset: 0x0 | ctor
void dll_615_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_615_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_615_setup(Object* self, DLL615_Setup* objSetup, s32 reset) {
    DLL615_Data* objData = self->data;
    
    objAddObjectType(self, OBJTYPE_Baddie);
    
    self->srt.yaw = objSetup->unk18 << 8;
    
    objInitMesgQueue(self, 2);
    
    objData->unk26C.x = objSetup->base.x;
    objData->unk26C.y = objSetup->base.y;
    objData->unk26C.z = objSetup->base.z;
    objData->unk264 = objSetup->unk1A;
    objData->unk294 = objSetup->unk19;
    objData->unk290 = _data_0[objData->unk294];
    objData->unk286 = 0;
    objData->unk288 = 0;
    
    if (objData->unk290 & 1) {
        objData->unk284 = 0;
        self->srt.roll = 0;
        objData->unk292 = 1;
    } else if (objData->unk290 & 8) {
        objData->unk284 = 0;
        objData->unk286 = 1200;
        objData->unk264 = 100.0f;
        self->srt.roll = 0;
        objData->unk292 = 1;
    } else {
        objData->unk284 = 400;
        self->srt.roll = -(M_180_DEGREES - 1);
        objData->unk292 = 0;
    }
    
    if (objData->unk290 & 0x40) {
        self->opacity = 0;
    }
    
    objData->unk280 = 0.0f;
    objData->unk28A = objSetup->unk1C;
    self->srt.transl.y = objSetup->base.y + objData->unk28A;
    objData->unk28C = 400 + mathRnd(0, 80);
    objData->unk268 = 80.0f;
    objData->unk28E = objSetup->unk1E;
    
    if (objData->unk290 & 2) {
        objData->unk0.mode = 1;
        gDLL_27->vtbl->init(&objData->unk0, 0, 0, 1);
        gDLL_27->vtbl->setup_hits_collider(&objData->unk0, 1, &_data_18, &_data_24, 4);
        objData->unk0.flags |= 0x40000 | 8;
    }
    
    self->animCallback = (void*)&dll_615_func_1AC8;
    
    func_8002674C(self);
    func_800267C4(self);
}

// offset: 0x288 | func: 1 | export: 1
void dll_615_control(Object* self) {
    DLL615_Data* objData;
    Object* sp88; //88
    Object* sidekick; //84
    f32 dy;
    s32 i; //7C
    s32 sp78; //78
    Vec3f sp6C; //6C
    f32 range; //68
    f32 sp64; //64
    s32 count;
    f32 speedSq;
    u8 sp5B; //5B
    TrackHeightResult** sp54; //54
    f32 distance;
    f32 sp4C; //4C

    objData = self->data;
    sp78 = 0;
    sp54 = NULL;
    range = 1000.0f;
    sp64 = 1.0f;
    sp4C = 10000.0f;
    sp5B = 0;
    
    if (!(objData->unk290 & 0x10)) {
        sp88 = objGetPlayer();
    } else {
        sp88 = objGetNearestTypeTo(0xB, self, &sp4C);
    }
    
    if (sp88 == NULL) {
        return;
    }
    
    _data_10 = (mainGetBits(0x789) * 0.1f) + 0.1f;
    
    if (objData->unk292 == 6) {
        self->unkAF |= 8;
        if (self->curModAnimId != 1) {
            objAnimSet(self, 1, 0.0f, 0);
            gDLL_6_AMSFX->vtbl->Play(self, 0x7F9, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        if (self->animProgress > 0.4f) {
            self->srt.scale *= 0.95f;
        }
        if (objAnimAdvance(self, 0.01f, gUpdateRate, NULL) != 0) {
            if ((objData->unk28E != 0) && (objData->unk28E != -1)) {
                mainSetBits(objData->unk28E, mainGetBits(objData->unk28E) + 1);
            }
            dll_615_func_1AE8(self);
        }
        return;
    } 
    
    if (objData->unk290 & 8) {
        if (objData->unk286 != 0) {
            objData->unk286 -= gUpdateRate;
            if (objData->unk286 <= 0) {
                objData->unk286 = 0;

                for (i = 0; i != 0x1E; i++) {
                    gDLL_17_partfx->vtbl->spawn(self, 0x1A3, NULL, 0U, -1, NULL);
                }
                
                objData->unk288 = 0x64;
                return;
            }
        } else if (objData->unk288 != 0) {
            self->unkAF |= 8;
            objData->unk288 -= gUpdateRate;
            if (objData->unk288 <= 0) {
                objData->unk288 = 0;
                dll_615_func_1AE8(self);
            }
            return;
        }
    } 
    
    if (func_80025F40(self, NULL, NULL, NULL) != 0) {
        if (objData->unk290 & 0x100) {
            objData->unk292 = 6;
        } else {
            dll_615_func_1AE8(self);
        }
        return;
    } 
        
    for (i = 0; i < 6; i++) {
        sp5B += mainGetBits(i + 0x2AA);
    }
    
    if (sp5B >= 6) {
        dll_615_func_1AE8(self);
        return;
    }
    
    func_80025F40(self, NULL, NULL, NULL);
    
    if (objData->unk284 != 0) {
        objData->unk284 -= gUpdateRate;
        if (objData->unk284 < 0) {
            objData->unk284 = 0;
        }
        return;
    } 
    
    if (((objData->unk292 == 3) || (objData->unk292 == 1) || (objData->unk292 == 5)) && !(objData->unk290 & 0x80)) {
        if (objData->unk292 == 5) {
            distance = gDLL_11_Newlfx->vtbl->FindClosestLight(&self->srt.transl, objData->unk268 + 50.0f);
            if ((distance < 0.0f) || ((objData->unk268 + 30.0f) < distance)) {
                objData->unk292 = 3;
                objData->unk284 = 0x14;
            }
        } else {
            //Ooh, so Skeetlas do react to light sources!!
            distance = gDLL_11_Newlfx->vtbl->FindClosestLight(&self->srt.transl, objData->unk268 + 20.0f);
            if ((distance >= 0.0f) && (distance < objData->unk268)) {
                if (mathRnd(0, 0x32) == 0) {
                    gDLL_6_AMSFX->vtbl->Play(self, 0x7FA, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                
                if (objData->unk28C <= 0) {
                    if (objData->unk290 & 0x100) {
                        objData->unk292 = 6;
                    } else {
                        dll_615_func_1AE8(self);
                    }
                    return;
                } else if (objData->unk292 != 5) {
                    if (objData->unk260 != 0) {
                        gDLL_6_AMSFX->vtbl->Stop(objData->unk260);
                        objData->unk260 = 0;
                    }
                    
                    objData->unk292 = 5;
                    
                    self->velocity.x = -self->velocity.x / 4.0f;
                    self->velocity.z = -self->velocity.z / 4.0f;
                }
            }
        }
    } 
    
    if ((objData->unk290 & 0x200) && (objData->unk292 != 5)) {
        sidekick = objGetSidekick();
        if (sidekick != NULL) {
            if ((vec3Distance(&self->globalPosition, &sidekick->globalPosition) < 30.0f) && (((DLL_Unknown*)sidekick->dll)->vtbl->func[25].withOneArgS32(sidekick))) {
                objData->unk292 = 5;
                gDLL_6_AMSFX->vtbl->Play(self, 0x7FA, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
    }
    
    if (objData->unk292 == 5) {
        if (objData->unk290 & 2) {
            gDLL_27->vtbl->func_1E8(self, &objData->unk0, gUpdateRateF);
            gDLL_27->vtbl->func_5A8(self, &objData->unk0);
            gDLL_27->vtbl->func_624(self, &objData->unk0, gUpdateRateF);
        }

        speedSq = SQ(self->velocity.x) + SQ(self->velocity.z);
        if (speedSq != 0.0f) {
            sp64 = sqrtf(speedSq);
        }
        
        objData->unk280 = -0.065f * sp64;
        objAnimAdvance(self, objData->unk280, gUpdateRate, NULL);
        self->srt.transl.x += self->velocity.x * gUpdateRateF;
        self->srt.transl.z += self->velocity.z * gUpdateRateF;
        objData->unk28C -= gUpdateRate;
        
        if (objData->unk290 & 4) {
            sp4C = 10000.0f;
            count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp54, 0, 0);
            for (i = 0; i < count; i++) {
                dy = sp54[i]->y - self->srt.transl.f[1];
                if (dy < 0) {
                    dy *= -1.0f;
                }
                if (dy < sp4C) {
                    sp78 = i;
                    sp4C = dy;
                }
            }
            
            if (sp54 != NULL) {
                self->srt.transl.y = sp54[sp78]->y;
                dll_615_func_19FC(self, sp54[sp78]);
            } else {
                self->srt.transl.y = objData->unk26C.y;
            }
        } else {
            self->srt.transl.y = objData->unk26C.y;
        }
        
        if (!(objData->unk290 & 0x80) && (objData->unk28C <= 0)) {
            if (objData->unk290 & 0x100) {
                objData->unk292 = 6;
                return;
            }
            objData->unk292 = 0;
            
            if (objData->unk260 != 0) {
                gDLL_6_AMSFX->vtbl->Stop(objData->unk260);
                objData->unk260 = 0;
            }
            
            self->srt.transl.x = objData->unk26C.x;
            self->srt.transl.y = objData->unk26C.y + objData->unk28A;
            self->srt.transl.z = objData->unk26C.z;
        } else if ((objData->unk290 & 0x200) && (mathRnd(0, 0x14) == 0)) {
            objData->unk292 = 3;
            objData->unk284 = mathRnd(0, 0x14) + 0x32;
        }
    } else {
        range = vec3DistanceXZ(&sp88->globalPosition, &self->globalPosition);
        if ((range < objData->unk264) || (mainGetBits(0x1D9) != 0)) {
            if (objData->unk292 == 0) {
                objData->unk292 = 1;
                objData->unk284 = 1;
                self->srt.roll = 0;
            } else if (objData->unk292 == 1) {
                if (self->velocity.f[1] > -10.0f) {
                    self->velocity.f[1] += (-0.1f * gUpdateRateF);
                }

                if (self->srt.transl.f[1] < objData->unk26C.y) {
                    self->srt.transl.f[1] = objData->unk26C.y;
                    self->velocity.f[1] = 0.0f;
                    objData->unk292 = 3;
                    objData->unk284 = mathRnd(0, 0x14) + 0x32;
                    objData->unk264 *= 2.0f;
                    objAnimSet(self, 0, 0.0f, 0);
                }
            } else if (objData->unk292 == 3) {
                if (objData->unk260 == 0) {
                    gDLL_6_AMSFX->vtbl->Play(self, 0x669, 0x39, &objData->unk260, NULL, 0, NULL);
                }
                
                if (objData->unk290 & 2) {
                    gDLL_27->vtbl->func_1E8(self, &objData->unk0, gUpdateRateF);
                    gDLL_27->vtbl->func_5A8(self, &objData->unk0);
                    gDLL_27->vtbl->func_624(self, &objData->unk0, gUpdateRateF);
                }
                
                if (objData->unk290 & 4) {
                    sp4C = 10000.0f;
                    count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], &sp54, 0, 0);
                    for (i = 0; i < count; i++) {
                        dy = sp54[i]->y - self->srt.transl.f[1];
                        if (dy < 0) {
                            dy *= -1.0f;
                        }
                        if (dy < sp4C) {
                            sp78 = i;
                            sp4C = dy;
                        }
                    }
                        
                    if (sp54 != NULL) {
                        self->srt.transl.f[1] = sp54[sp78]->y;
                        dll_615_func_19FC(self, sp54[sp78]);
                    } else {
                        self->srt.transl.f[1] = objData->unk26C.y;
                    }
                } else {
                    self->srt.transl.f[1] = objData->unk26C.y;
                }
                
                sp6C.f[0] = sp88->srt.transl.x - self->srt.transl.x;
                sp6C.f[1] = sp88->srt.transl.y - self->srt.transl.y;
                sp6C.f[2] = sp88->srt.transl.z - self->srt.transl.z;
                self->velocity.x = (sp6C.f[0] / 300.0f) * gUpdateRateF;
                self->velocity.y = (sp6C.f[1] / 300.0f) * gUpdateRateF;
                self->velocity.z = (sp6C.f[2] / 300.0f) * gUpdateRateF;
                
                if (objData->unk290 & 0x20) {
                    if (_data_10 < sqrtf(SQ(self->velocity.f[0]) + SQ(self->velocity.f[1]) + SQ(self->velocity.f[2]))) {
                        vec3Normalize(&self->velocity);
                        self->velocity.x *= gUpdateRateF * _data_10;
                        self->velocity.y *= gUpdateRateF * _data_10;
                        self->velocity.z *= gUpdateRateF * _data_10;
                    }
                }
                
                diPrintf(" Range %f  ", &range);
                
                if ((self->curModAnimId == 0) && (objData->unk290 & 0x400) && (range < 15.0f)) {
                    gDLL_6_AMSFX->vtbl->Play(self, 0xB5B, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objAnimSet(self, 2, 0.0f, 0);
                }
                
                if ((range < 13.0f) || ((objData->unk290 & 0x10) && (self->objhitInfo->unk58 & 8) && (range < 50.0f))) {
                    _data_14++;
                    if (self->curModAnimId == 2) {
                        if ((self->animProgress > 0.3f) && (self->animProgress < 0.7f)) {
                            objSendMesg(sp88, 0x60004, self, (void* )1);
                            _data_14 = 0;
                        }
                    }
                    
                    if (mainGetBits(0x1D9) != 0) {
                        _data_14 = 0;
                    } else if ((_data_14 >= 3) || ((objData->unk290 & 0x10) && (_data_14 >= 3))) {
                        gDLL_6_AMSFX->vtbl->Play(self, 0x7F8, MAX_VOLUME, NULL, NULL, 0, NULL);
                        if (!(objData->unk290 & 0x10)) {
                            objSendMesg(sp88, 0x60004, self, (void* )1);
                        } else {
                            objData->unk295_0 = 1;
                        }
                        _data_14 = 0;
                    }
                    
                    if ((objData->unk290 & 0x10) == 0) {
                        self->srt.transl.x += (-self->velocity.x * 26.0f);
                        self->srt.transl.z += (-self->velocity.z * 26.0f);
                    } else {
                        self->srt.transl.x += (-self->velocity.x * 8.0f);
                        self->srt.transl.z += (-self->velocity.z * 8.0f);
                    }
                    objData->unk284 = mathRnd(0, 0x14) + 100;
                }

                self->srt.yaw = mathAtan2f(sp88->srt.transl.x - self->srt.transl.x, sp88->srt.transl.z - self->srt.transl.z) + (M_180_DEGREES - 1);
                
                speedSq = SQ(self->velocity.x) + SQ(self->velocity.z);
                if (speedSq != 0.0f) {
                    sp64 = sqrtf(speedSq);
                }
                
                switch (self->curModAnimId) {
                case 0:
                    objData->unk280 = 0.065f * sp64;
                    break;
                case 2:
                    objData->unk280 = 0.03f;
                    break;
                case 1:
                    objData->unk280 = 0.01f;
                    break;
                }
                
                if ((objAnimAdvance(self, objData->unk280, gUpdateRate, NULL) != 0) && (self->curModAnimId != 0)) {
                    objAnimSet(self, 0, 0.0f, 0);
                }
                
                self->srt.transl.x += self->velocity.x * gUpdateRateF;
                self->srt.transl.z += self->velocity.z * gUpdateRateF;
            }
        } else if (objData->unk292 == 1) {
            if (self->velocity.y > -1.0f) {
                self->velocity.y += -0.01f * gUpdateRateF;
            }

            if (self->srt.transl.y < objData->unk26C.y) {
                self->srt.transl.y = objData->unk26C.y;
                self->velocity.y = 0.0f;
                objData->unk292 = 3;
                objData->unk284 = mathRnd(0, 0x14) + 0x32;
                objData->unk264 = 2.0f * objData->unk264;
                objAnimSet(self, 0, 0.0f, 0);
            }
            self->srt.transl.y += self->velocity.y * gUpdateRateF;
        }
        
        if (objData->unk292 == 1) {
            self->srt.transl.y += self->velocity.y * gUpdateRateF;
        }
        
        if (mathRnd(0, 0x32) == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, mathRnd(0, 2) + 0x7BB, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
}

static const char str1[] = " Set MOve ";
static const char str2[] = " BITE ";
static const char str3[] = " Free CRawler";
static const char str4[] = " Deactivateing Crawler ";

// offset: 0x17B0 | func: 2 | export: 2
void dll_615_update(Object* self) {
    DLL615_Data* objData;
    Object* target;
    f32 distance;

    objData = self->data;
    distance = 100000.0f;
    
    if (objData->unk295_0) {
        if (!(objData->unk290 & 0x10)) {
            target = objGetPlayer();
        } else {
            target = objGetNearestTypeTo(OBJTYPE_Vehicle, self, &distance);
        }
        func_8002635C(target, self, Damage_Type_Sword_Staff_Strike2, 1, 0);
        objData->unk292 = 6;
        objData->unk295_0 = FALSE;
    }
}

// offset: 0x1888 | func: 3 | export: 3
void dll_615_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL615_Data* objData = self->data;
    
    if (objData->unk290 & 0x40) {
        if (self->opacity < OBJECT_OPACITY_MAX) {
            if (self->opacity > (OBJECT_OPACITY_MAX - gUpdateRate)) {
                self->opacity = OBJECT_OPACITY_MAX;
                objData->unk290 &= ~0x40;
            } else {
                self->opacity += gUpdateRate;
            }
        }
    }
    
    if (visibility && (objData->unk288 == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x194C | func: 4 | export: 4
void dll_615_free(Object* self, s32 onlySelf) {
    DLL615_Data* objData = self->data;

    if (objData->unk260 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objData->unk260);
        objData->unk260 = 0;
    }
    
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0x19D8 | func: 5 | export: 5
u32 dll_615_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x19E8 | func: 6 | export: 6
u32 dll_615_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL615_Data);
}

// offset: 0x19FC | func: 7
void dll_615_func_19FC(Object* self, TrackHeightResult* trackResult) {
    SRT xfm;
    Vec3f surfNorm;
    s32 angle;
    s32 pad;

    surfNorm.x = trackResult->norm[0];
    surfNorm.y = trackResult->norm[1];
    surfNorm.z = trackResult->norm[2];
    xfm.transl.x = 0.0f;
    xfm.transl.y = 0.0f;
    xfm.transl.z = 0.0f;
    xfm.scale = 1.0f;
    xfm.roll = 0;
    xfm.pitch = 0;
    xfm.yaw = self->srt.yaw;
    
    mathRotateRPY(&xfm, surfNorm.f); 
    angle = mathAtan2f(surfNorm.x, surfNorm.y);
    self->srt.pitch = mathAtan2f(surfNorm.z, surfNorm.y);
    self->srt.roll = angle;
}

// offset: 0x1AC8 | func: 8
int dll_615_func_1AC8(Object* self, Object* animObj, AnimObj_Data* animData/*, s8 prevCallbackValue */) {
    DLL615_Data* objData = self->data;
    objData->unk292 = 1;
    return 0;
}

// offset: 0x1AE8 | func: 9
void dll_615_func_1AE8(Object* self) {
    ObjSetup *objSetup;
    
    objSetup = self->setup;
    
    if (objSetup->uID == 0){
        func_800267A4(self);
        objFreeObject(self);
        return;
    }
    
    objDisable(self);
    func_800267A4(self);
    objFreeObjectType(self, OBJTYPE_Baddie);    
    self->srt.flags |= OBJFLAG_INVISIBLE;
}
