#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/84_camnormal.h"
#include "dlls/engine/86_cam1stperson.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/intersect.h"
#include "dll.h"

// official filename: firstperson.c

// size: 0x124
typedef struct {
    f32 distance3D; //The goal distance for the camera (relative to the player) when easing out of 1st person
    f32 offsetY;    //The goal height for the camera (relative to the player) when easing out of 1st person
    f32 offsetX;    //The camera's position offset (relative to the player) when the ease into 1st person has finished
    f32 offsetZ;    //The camera's position offset (relative to the player) when the ease into 1st person has finished
    f32 splineX[4];         //Ease interpolation points
    f32 splineY[4];         //Ease interpolation points
    f32 splineZ[4];         //Ease interpolation points
    f32 splineYaw[4];       //Ease interpolation points
    f32 splinePitch[4];     //Ease interpolation points
    u8 _unk60[0x70 - 0x60];
    f32 nearestFloorY;      //Track height value, stored during the ease out
    f32 nearestCeilingY;    //Track height value, stored during the ease out
    CurvesStruct curves;    //For ease interpolation
    f32 verticalAdjust;     //A camera pitch adjustment (relative to player pitch) when in locked view mode, or a secondary goal Y adjustment for the exit ease
    f32 easeDistance2D;     //The lateral distance to travel during the exit ease
    f32 yawSpeed;           //Left/right turn speed while looking around, controlled with the joystick
    u8 state;               //`Cam1stPerson_States`
} Cam1stPerson;

typedef enum {
    Cam1stPerson_STATE_0_Ease_In,
    Cam1stPerson_STATE_1_Ease_In_Finished,
    Cam1stPerson_STATE_2_Player_Has_Control,
    Cam1stPerson_STATE_3_Ease_Out_Start,
    Cam1stPerson_STATE_4_Ease_Out,
    Cam1stPerson_STATE_5_Locked_View
} Cam1stPerson_States;

#define PLAYER_HEAD_HEIGHT 27.0f

/*0x0*/ static Cam1stPerson* sState;

static s32 cam1stperson_easeIn(Cam* cam, Object* player);
static void cam1stperson_lookAround(Cam* cam);
static void cam1stperson_easeOutCalculate(Cam* cam);
static void cam1stperson_easeOutBegin(Cam* cam, Object* player);

// offset: 0x0 | ctor
void cam1stperson_ctor(void* dll) { }

// offset: 0xC | dtor
void cam1stperson_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void cam1stperson_Setup(Cam* cam, s32 mode, Cam1stPerson_Params* data) {
    f32 dx;
    f32 dz;
    f32 distance;
    f32 mSin;
    f32 cos;
    f32 tangentStrength1;
    Object* player;
    s16 yawDiff;
    s16 absYawDiff;
    s16 yaw;
    f32 tangentStrength2;
    f32 yawRange;

    player = cam->player;

    sState = mmAlloc(sizeof(Cam1stPerson), ALLOC_TAG_CAM_COL, ALLOC_NAME("cam1stperson"));
    sState->distance3D = data->distance3D;
    sState->verticalAdjust = data->verticalAdjust;
    sState->offsetY = data->offsetY;
    sState->yawSpeed = 0;

    yawDiff = (-cam->srt.yaw - player->srt.yaw) + M_180_DEGREES;
    if (yawDiff < 0) {
        absYawDiff = -yawDiff;
    } else {
        absYawDiff = yawDiff;
    }
    tangentStrength1 = yawDiff / 50.0f;

    sState->curves.unk84 = sState->splineX;
    sState->curves.unk88 = sState->splineY;
    sState->curves.unk8C = sState->splineZ;
    sState->curves.numControlPoints = 4;
    sState->curves.unk80 = 0;
    sState->curves.splineFunc = curvesHermite;
    sState->curves.splineConverterFunc = curvesHermiteConverter;
    dx = cam->srt.transl.x - player->srt.transl.x;
    dz = cam->srt.transl.z - player->srt.transl.z;
    distance = sqrtf(SQ(dx) + SQ(dz));
    if (distance != 0.0f) {
        dx /= distance;
        dz /= distance;
    }
    mSin = -mathSinfInterp(player->srt.yaw);
    cos = mathCosfInterp(player->srt.yaw);
    tangentStrength2 = absYawDiff / 90.0f;

    sState->splineX[0] = cam->srt.transl.x;
    sState->splineX[1] = player->srt.transl.x;
    sState->splineX[2] = -dz * tangentStrength1;
    sState->splineX[3] = mSin * tangentStrength2;

    sState->splineY[0] = cam->srt.transl.y;
    sState->splineY[1] = player->srt.transl.y + PLAYER_HEAD_HEIGHT;
    sState->splineY[2] = 0.0f;
    sState->splineY[3] = 0.0f;

    sState->splineZ[0] = cam->srt.transl.z;
    sState->splineZ[1] = player->srt.transl.z;
    sState->splineZ[2] = dx * tangentStrength1;
    sState->splineZ[3] = -cos * tangentStrength2;

    sState->splineX[2] = 0.0f;
    sState->splineX[3] = 0.0f;
    sState->splineY[2] = 0.0f;
    sState->splineY[3] = 0.0f;
    sState->splineZ[2] = 0.0f;
    sState->splineZ[3] = 0.0f;
    sState->splineX[1] = sState->splineX[1];
    sState->splineZ[1] = sState->splineZ[1];
    curvesMove(&sState->curves);
    
    //Setup yaw interpolation
    {
        yaw = M_180_DEGREES - mathAtan2f(cam->srt.transl.x - sState->splineX[1], cam->srt.transl.z - sState->splineZ[1]);\
        yaw = cam->srt.yaw - (u16)yaw;\
        CIRCLE_WRAP(yaw);
        sState->splineYaw[0] = yaw;
        sState->splineYaw[1] = 0.0f;
        sState->splineYaw[2] = 0.0f;
        sState->splineYaw[3] = 0.0f;
        
        //Take the shortest angular path when interpolating yaw
        yawRange = sState->splineYaw[0] - sState->splineYaw[1];
        if ((yawRange > M_180_DEGREES) || (yawRange < -M_180_DEGREES)) {
            if (sState->splineYaw[0] < 0.0f) {
                sState->splineYaw[0] += (M_360_DEGREES - 1);
            } else if (sState->splineYaw[1] < 0.0f) {
                sState->splineYaw[1] += (M_360_DEGREES - 1);
            }
        }
    }

    sState->splinePitch[0] = cam->srt.pitch;
    sState->splinePitch[1] = 0.0f;
    sState->splinePitch[2] = 0.0f;
    sState->splinePitch[3] = 0.0f;

    if (mode == Cam1stPerson_MODE_Locked_View) {
        sState->state = Cam1stPerson_STATE_5_Locked_View;
    } else {
        sState->state = Cam1stPerson_STATE_0_Ease_In;
        dll_amSfx->Play(player, SOUND_1F, MAX_VOLUME, NULL, NULL, 0, NULL);
    }

    cam->easeActive1stPerson = TRUE;
}

// offset: 0x4F8 | func: 1 | export: 1
void cam1stperson_Control(Cam* cam) {
    u16 buttons;
    s32 prevEaseFinished;
    f32 distanceSq;
    f32 dx;
    f32 dz;
    f32 tValueOpacity;
    CamControl_Module* camNormal;
    Object* player;

    player = cam->player;
    cam->highlightFlags |= 2;
    buttons = joyGetPressed(0);

    switch (sState->state) {
    case Cam1stPerson_STATE_0_Ease_In:
        sState->state = cam1stperson_easeIn(cam, cam->player);
        cam->easeActive1stPerson = TRUE;
        break;
    case Cam1stPerson_STATE_1_Ease_In_Finished:
        if (curves_func_800053B0(&sState->curves, 1000.0f)) {
            sState->offsetX = cam->srt.transl.x - cam->player->srt.transl.x;
            sState->offsetZ = cam->srt.transl.z - cam->player->srt.transl.z;
            sState->state = Cam1stPerson_STATE_2_Player_Has_Control;
        }
        cam->srt.yaw = sState->curves.unk68.x;
        cam->easeActive1stPerson = TRUE;
        break;
    case Cam1stPerson_STATE_2_Player_Has_Control:
        //Look around with the joystick
        cam1stperson_lookAround(cam);

        //Exit 1st person if any button is pressed
        if (buttons != 0) {
            cam1stperson_easeOutCalculate(cam);
            sState->state = Cam1stPerson_STATE_3_Ease_Out_Start;
        }

        cam->easeActive1stPerson = FALSE;
        break;
    case Cam1stPerson_STATE_5_Locked_View:
        cam->srt.yaw = M_180_DEGREES - player->srt.yaw;
        cam->srt.pitch = player->srt.pitch + (sState->verticalAdjust * 182.0f);
        cam->srt.roll = player->srt.roll;
        cam->srt.transl.x = player->globalPosition.x;
        cam->srt.transl.y = player->globalPosition.y + sState->offsetY;
        cam->srt.transl.z = player->globalPosition.z;
        cam->fov = 80.0f;
        break;
    case Cam1stPerson_STATE_3_Ease_Out_Start:
        prevEaseFinished = curves_func_800053B0(&sState->curves, 1000.0f);
        cam->srt.yaw = sState->curves.unk68.x;
        cam->srt.pitch = sState->curves.unk68.y;
        if (prevEaseFinished) {
            cam1stperson_easeOutBegin(cam, cam->player);
            sState->state = Cam1stPerson_STATE_4_Ease_Out;
        }
        cam->easeActive1stPerson = TRUE;
        break;
    case Cam1stPerson_STATE_4_Ease_Out:
        //Fade player back in
        {
            tValueOpacity = sState->curves.unk0 - 0.2f;
            if (tValueOpacity < 0.0f) {
                tValueOpacity = 0.0f;
            }
            tValueOpacity *= 1.25f;
            cam->player->opacity = (s32) (tValueOpacity * OBJECT_OPACITY_MAX);
        }

        //Switch back to camnormal when the exit ease is finished
        if (curves_func_800053B0(&sState->curves, 5.0f)) {
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
            cam->player->opacity = OBJECT_OPACITY_MAX;
        }

        //Move the camera along the curves
        cam->srt.transl.x = sState->curves.unk68.x;
        cam->srt.transl.z = sState->curves.unk68.z;

        //Check track floor/ceiling heights, and limit the camera's Y accordingly
        camNormal = gDLL_2_Camera->vtbl->get_camnormal_module();
        ((DLL_84_camnormal*)camNormal->dll)->vtbl->func8(cam, CamNormal_TRACKFLAG_Get_Floor_Ceiling_Heights, 3, &sState->nearestFloorY, &sState->nearestCeilingY);
        ((DLL_84_camnormal*)camNormal->dll)->vtbl->func6(cam, cam->player, sState->nearestFloorY, sState->nearestCeilingY);

        dx = cam->srt.transl.x - cam->player->srt.transl.x;
        dz = cam->srt.transl.z - cam->player->srt.transl.z;
        distanceSq = SQ(dx) + SQ(dz);
        if (distanceSq < SQ(10)) {
            cam->srt.pitch = 0;
        } else {
            ((DLL_84_camnormal*)camNormal->dll)->vtbl->func5(cam, cam->player->srt.transl.y, sqrtf(distanceSq));
        }

        cam->easeActive1stPerson = TRUE;

        break;
    }

    //Exit first-person when the player is hit (@bug: doesn't seem to work)
    if (func_80025F40(cam->player, NULL, NULL, NULL) != 0) {
        cam1stperson_easeOutCalculate(cam);
        cam->srt.transl.x = sState->splineX[1];
        cam->srt.transl.y = sState->splineY[1];
        cam->srt.transl.z = sState->splineZ[1];
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_None);
    }
}

// offset: 0xA04 | func: 2 | export: 2
void cam1stperson_Free(Cam* cam) {
    cam->player->srt.flags &= ~OBJFLAG_SKIP_MODEL_DL;
    cam->player->opacity = OBJECT_OPACITY_MAX;
    mmFree(sState);
}

// offset: 0xA64 | func: 3 | export: 3
void cam1stperson_Func_A64(void* arg0, s32 arg1) {

}

// offset: 0xA74 | func: 4
/** Handles the initial ease into 1st-person mode */
s32 cam1stperson_easeIn(Cam* cam, Object* player) {
    s32 finished;
    s16 yaw;
    f32 yawRange;
    f32 tValueOpacity;

    finished = curves_func_800053B0(&sState->curves, 5.0f);
    cam->srt.transl.x = sState->curves.unk68.x;
    cam->srt.transl.y = sState->curves.unk68.y;
    cam->srt.transl.z = sState->curves.unk68.z;
    cam->srt.pitch = curvesHermite(sState->splinePitch, sState->curves.unk0, NULL);
    yaw = curvesHermite(sState->splineYaw, sState->curves.unk0, NULL);

    //Fade out player
    {
        tValueOpacity = sState->curves.unk0 - 0.2f;
        if (tValueOpacity < 0.0f) {
            tValueOpacity = 0.0f;
        }
        tValueOpacity *= 1.25f;
        cam->player->opacity = OBJECT_OPACITY_MAX - (s32) (tValueOpacity * OBJECT_OPACITY_MAX);
    }

    if (finished) {
        sState->curves.unk84 = sState->splineYaw;
        sState->curves.unk88 = NULL;
        sState->curves.unk8C = NULL;
        sState->curves.numControlPoints = 4;
        sState->curves.splineFunc = curvesHermite;
        sState->curves.splineConverterFunc = curvesHermiteConverter;
        sState->curves.unk80 = 0;
        sState->splineYaw[0] = cam->srt.yaw;
        sState->splineYaw[1] = (s16) (M_180_DEGREES - player->srt.yaw);

        yawRange = sState->splineYaw[0] - sState->splineYaw[1];
        if ((yawRange < 1820.0f) && (yawRange > -1820.0f)) {
            sState->splineYaw[1] = sState->splineYaw[0];
        } else {
            if (((sState->splineYaw[0] - sState->splineYaw[1]) > M_180_DEGREES) || 
                ((sState->splineYaw[0] - sState->splineYaw[1]) < -M_180_DEGREES)
            ) {
                if (sState->splineYaw[0] < 0.0f) {
                    sState->splineYaw[0] += M_360_DEGREES - 1;
                } else if (sState->splineYaw[1] < 0.0f) {
                    sState->splineYaw[1] += M_360_DEGREES - 1;
                }
            }
        }

        sState->splineYaw[2] = 0.0f;
        sState->splineYaw[3] = 0.0f;
        curvesMove(&sState->curves);
        cam->player->srt.flags |= OBJFLAG_SKIP_MODEL_DL;
        return Cam1stPerson_STATE_1_Ease_In_Finished;
    } else {
        cam->srt.yaw = (yaw - mathAtan2f(cam->srt.transl.x - sState->splineX[1], cam->srt.transl.z - sState->splineZ[1])) + M_180_DEGREES;
        
        return Cam1stPerson_STATE_0_Ease_In;
    }
}

// offset: 0xD90 | func: 5
/** Handles 1st person mode's joystick controls. */
void cam1stperson_lookAround(Cam* cam) {
    #define MAX_PITCH 0x3C00 //~85 degrees
    s8 stickX;
    s8 stickY;
    s16 dPitch;
    Object* player;

    player = cam->player;

    stickX = joyGetStickX(0);
    stickY = joyGetStickY(0);

    //Handle deadzone
    if ((stickX < 8) && (stickX > -8)) {
        stickX = 0;
    }
    if ((stickY < 8) && (stickY > -8)) {
        stickY = 0;
    }

    //Look left/right
    sState->yawSpeed += ((stickX * 6) - sState->yawSpeed) * 0.12f * gUpdateRateF;
    if ((sState->yawSpeed > -5.0f) && (sState->yawSpeed < 5.0f)) {
        sState->yawSpeed = 0.0f;
    }
    cam->srt.yaw += sState->yawSpeed * gUpdateRateF;

    //Look up/down
    dPitch = (s16)((stickY / 120.0f) * MAX_PITCH) - (u16)(cam->srt.pitch);
    CIRCLE_WRAP(dPitch);
    cam->srt.pitch += (dPitch * gUpdateRate) >> 4;

    //Limit pitch
    if (cam->srt.pitch > MAX_PITCH) {
        cam->srt.pitch = MAX_PITCH;
    }
    if (cam->srt.pitch < -MAX_PITCH) {
        cam->srt.pitch = -MAX_PITCH;
    }

    //Rotate the player
    player->srt.yaw = M_180_DEGREES - cam->srt.yaw;

    //Reposition the camera
    cam->srt.transl.x = player->srt.transl.x + sState->offsetX;
    cam->srt.transl.y = player->srt.transl.y + PLAYER_HEAD_HEIGHT;
    cam->srt.transl.z = player->srt.transl.z + sState->offsetZ;
}

// offset: 0x1008 | func: 6
void cam1stperson_easeOutCalculate(Cam* cam) {
    f32 distance;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sin;
    f32 cos;
    Object* player;
    Vec3f initial;
    Vec3f goal;
    AABBs32 aabb;
    TrackIntersectResult result;

    player = cam->player;

    //Set up XYZ spline points
    sState->splineX[0] = cam->srt.transl.x;
    sState->splineX[2] = 0.0f;
    sState->splineX[3] = 0.0f;

    sState->splineY[0] = cam->srt.transl.y;
    sState->splineY[2] = 0.0f;
    sState->splineY[3] = 0.0f;

    sState->splineZ[0] = cam->srt.transl.z;
    sState->splineZ[2] = 0.0f;
    sState->splineZ[3] = 0.0f;

    //Calculate goal XYZ coords
    sin = mathSinfInterp(player->srt.yaw);
    cos = mathCosfInterp(player->srt.yaw);
    distance = sqrtf(SQ(sState->distance3D) - SQ(sState->offsetY)); //Get lateral distance
    sState->splineX[1] = player->globalPosition.x + (sin * distance);
    sState->splineY[1] = player->globalPosition.y + sState->verticalAdjust + sState->offsetY;
    sState->splineZ[1] = player->globalPosition.z + (cos * distance);

    //Calculate delta from start to goal 
    dx = sState->splineX[1] - sState->splineX[0];
    dy = sState->splineY[1] - sState->splineY[0];
    dz = sState->splineZ[1] - sState->splineZ[0];

    distance = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
    if (distance != 0.0f) {
        dx *= (1.0f / distance);
        dz *= (1.0f / distance);
    }

    goal.x = sState->splineX[1];
    goal.y = sState->splineY[1];
    goal.z = sState->splineZ[1];
    initial.x = sState->splineX[0] - (dx * 20.0f);
    initial.y = sState->splineY[1];
    initial.z = sState->splineZ[0] - (dz * 20.0f);
    result.unk50[0] = -1;
    result.unk54[0] = 4;
    result.unk40[0] = 4.5f;
    trackIntersectBuildAABB(&aabb, &initial, &goal, &result.unk40[0], 1);
    trackIntersectBroadphase(player, &aabb, 1);
    trackGetIntersect(player, initial.f, goal.f, 1, &result, 0);
    sState->splineX[1] = goal.x;
    sState->splineY[1] = goal.y;
    sState->splineZ[1] = goal.z;

    dx = sState->splineX[1] - sState->splineX[0];
    dz = sState->splineZ[1] - sState->splineZ[0];
    sState->easeDistance2D = sqrtf(SQ(dx) + SQ(dz));

    sState->curves.unk84 = sState->splineYaw;
    sState->curves.unk88 = sState->splinePitch;
    sState->curves.unk8C = NULL;
    sState->curves.numControlPoints = 4;
    sState->curves.unk80 = 0;
    sState->curves.splineFunc = curvesHermite;
    sState->curves.splineConverterFunc = curvesHermiteConverter;

    //Setup yaw spline points
    {
        sState->splineYaw[0] = cam->srt.yaw;
        sState->splineYaw[1] = (s16) (M_180_DEGREES - mathAtan2f(
            sState->splineX[1] - player->globalPosition.x, 
            sState->splineZ[1] - player->globalPosition.z));
        sState->splineYaw[2] = 0.0f;
        sState->splineYaw[3] = 0.0f;

        //Take shortest angular path
        if (((sState->splineYaw[0] - sState->splineYaw[1]) > M_180_DEGREES) || 
            ((sState->splineYaw[0] - sState->splineYaw[1]) < -M_180_DEGREES)
        ) {
            if (sState->splineYaw[0] < 0.0f) {
                sState->splineYaw[0] += (M_360_DEGREES - 1);
            } else if (sState->splineYaw[1] < 0.0f) {
                sState->splineYaw[1] += (M_360_DEGREES - 1);
            }
        }
    }

    //Setup pitch spline points
    {
        sState->splinePitch[0] = cam->srt.pitch;
        sState->splinePitch[1] = 0.0f;
        sState->splinePitch[2] = 0.0f;
        sState->splinePitch[3] = 0.0f;

        //Take shortest angular path
        if (((sState->splinePitch[0] - sState->splinePitch[1]) > M_180_DEGREES) || 
            ((sState->splinePitch[0] - sState->splinePitch[1]) < -M_180_DEGREES)
        ) {
            if (sState->splinePitch[0] < 0.0f) {
                sState->splinePitch[0] += (M_360_DEGREES - 1);
            } else if (sState->splinePitch[1] < 0.0f) {
                sState->splinePitch[1] += (M_360_DEGREES - 1);
            }
        }
    }

    curvesMove(&sState->curves);
}

// offset: 0x1524 | func: 7
void cam1stperson_easeOutBegin(Cam* cam, Object* player) {
    sState->curves.unk84 = sState->splineX;
    sState->curves.unk88 = sState->splineY;
    sState->curves.unk8C = sState->splineZ;
    sState->curves.numControlPoints = 4;
    sState->curves.unk80 = 0;
    sState->curves.splineFunc = curvesHermite;
    sState->curves.splineConverterFunc = curvesHermiteConverter;
    curvesMove(&sState->curves);

    cam->player->srt.flags &= ~OBJFLAG_SKIP_MODEL_DL;
}
