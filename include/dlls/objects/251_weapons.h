#ifndef _DLLS_251_H
#define _DLLS_251_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    s8 unk0[0x8 - 0];
    void* unk8;
    s8 unkC[0x14 - 0xC];
    f32 unk14;
    f32 unk18;
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
    s8 unk44[0x58 - 0x44];
    f32 unk58; //Vec3f?
    f32 unk5C;
    f32 unk60;
    s8 unk64[0x6C - 0x64];
    u32 unk6C; //soundHandle
    s16 unk70;
    s16 unk72;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    s16 unk7A;
    s16 unk7C;
    s16 unk7E;
    s16 unk80;
    s16 unk82;
    u8 unk84;
    s8 unk85[0x8A - 0x85];
    s16 unk8A;
    s16 unk8C;
    s8 unk8E;
    s8 unk8F;
    s8 unk90[0x94 - 0x90];
} DLL251_Data; //0x94

DLL_INTERFACE(DLL_251_Weapons) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ void (*func7)(Object* self, f32 arg1);
    /*08*/ void (*func8)(Object* self);
    /*09*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ void (*func11)(Object* self);
    /*12*/ void (*func12)(Object* self, s32 arg1);
    /*13*/ void (*func13)(Object* self, s32 arg1);
    /*14*/ void (*func14)(Object* self, s32 arg1);
    /*15*/ void (*func15)(Object* self, s32 arg1);
    /*16*/ s16 (*func16)(Object* self);
    /*17*/ void (*func17)(Object* self, Vec3f* arg1, Vec3f* arg2);
    /*18*/ void (*func18)(Object* self, f32 arg1, f32 arg2);
    /*19*/ s8 (*func19)(Object* self);
};

#endif // _DLLS_251_H
