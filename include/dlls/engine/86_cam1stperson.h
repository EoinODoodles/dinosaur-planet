#ifndef _DLLS_86_H
#define _DLLS_86_H

#include "PR/ultratypes.h"

typedef struct {
    f32 distance3D; //The goal distance for the camera (relative to the player) when easing out of 1st person
    f32 offsetY;    //The goal height for the camera (relative to the player) when easing out of 1st person
    u16 verticalAdjust; //A camera pitch adjustment (relative to player pitch) when in locked view mode, or a secondary goal Y adjustment for the exit ease
} Cam1stPerson_Params;

typedef enum {
    Cam1stPerson_MODE_Locked_View = 1
} Cam1stPerson_Modes;

#endif
