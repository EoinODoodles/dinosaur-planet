#ifndef _DLLS_283_H
#define _DLLS_283_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
 /*00*/ ObjSetup base;
 /*18*/ s16 unused18;
 /*1A*/ s16 stringID;
} KrazoaText_Setup;

DLL_INTERFACE(DLL_283_KrazoaText) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*PrintText)(Object* self, Gfx** gfx);
};

#define dll_krazoaText(obj) (((DLL_283_KrazoaText*)obj->dll)->vtbl)

#endif // _DLLS_283_H

