#ifndef _DLLS_30_H
#define _DLLS_30_H

#include "PR/ultratypes.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_30_task) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*LoadRecentlyCompleted)(void);
    /*1*/ void (*MarkTaskCompleted)(u8 task);
    /*2*/ u8 (*GetNumRecentlyCompleted)(void);
    /*3*/ char *(*GetRecentlyCompletedTaskText)(u8 idx);
    /*4*/ char *(*GetCompletionTaskText)(void);
    /*5*/ s16 (*GetCompletionPercentage)(void);
};

#define dll_task (gDLL_30_Task->vtbl)

#endif //_DLLS_30_H
