#include "common.h"
#include "sys/objhits.h"
#include "sys/objtype.h"

//Used by Skeetlas/Scorpions

typedef struct {
    u8 _unk0[0x260];
    u32 unk260;
    u8 _unk264[0x288 - 0x264];
    s16 unk288;
    u8 _unk28A[0x290 - 0x28A];
    u16 unk290;
    s8 unk292;
    u8 unk293;
    u8 unk294;
    u8 unk295_0 : 1;
    u8 unk295_1 : 7;
    u8 _unk296[0x298 - 0x296];
} DLL615_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00040001, 0x000c03f7, 0x0167050c
};
/*0x10*/ static f32 _data_10 = 0.1;
/*0x14*/ static u32 _data_14 = 0x00000000;
/*0x18*/ static u32 _data_18[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x24*/ static f32 _data_24 = 10;

// offset: 0x0 | ctor
void dll_615_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_615_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_615_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_setup.s")

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
s32 dll_615_func_1AC8(Object* self, s32 arg1, s32 arg2) {
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
