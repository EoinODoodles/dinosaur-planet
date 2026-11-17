#include "common.h"
#include "sys/gfx/animseq.h"
#include "sys/math.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

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
/*0x14*/ static u32 _data_14 = 0x00000000;
/*0x18*/ static Vec3f _data_18 = VEC3F(0, 0, 0);
/*0x24*/ static f32 _data_24 = 10;

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
void dll_615_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_control.s")

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
void dll_615_func_19FC(Object* self, f32* arg1) {
    SRT sp38;
    Vec3f sp2C;
    s32 angle;
    s32 pad;

    sp2C.x = arg1[1];
    sp2C.y = arg1[2];
    sp2C.z = arg1[3];
    sp38.transl.x = 0.0f;
    sp38.transl.y = 0.0f;
    sp38.transl.z = 0.0f;
    sp38.scale = 1.0f;
    sp38.roll = 0;
    sp38.pitch = 0;
    sp38.yaw = self->srt.yaw;
    
    mathRotateRPY(&sp38, sp2C.f); 
    angle = mathAtan2f(sp2C.x, sp2C.y);
    self->srt.pitch = mathAtan2f(sp2C.z, sp2C.y);
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
