#include "dll.h"
#include "dlls/engine/11_newlfx.h"
#include "dlls/objects/common/sidekick.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "sys/rand.h"

//Used by Skeetlas/Scorpions

typedef struct {
    DLL27_Data collision;
    u32 soundHandle;            //Handles crawl sound loop
    f32 activateRange;          //Crawler falls/activates when their target is inside this range
    f32 lightHurtRadius;        //Range for being hurt by nearby light sources
    Vec3f home;                 //Base ObjSetup position, used as a reference for the ground Y when falling among other things.
    u8 _unk278[0x280 - 0x278];
    f32 animSpeed;              //Anim playback speed
    s16 timerPause;             //A delay timer, pausing movement briefly between actions
    s16 timerLifetime;          //Countdown until disappearing with a burst of particles (if enabled via `Crawler_FLAG_Expires`)
    s16 timerExpire;            //Secondary countdown after timerLifetime finishes; the crawler vanishes when this timer is active (showing only the particles) and finishes dying when the timer's up
    s16 yOffset;                //Initial vertical offset from the base ObjSetup position (used when Skeetlas hang from ceilings initially, etc.)
    s16 timerHurt;              //Countdown until dying from exposure to a light source, or from the Guard Command
    s16 gamebitDied;            //Gamebit to increment when the crawler's dying animation finishes (not set if the crawler vanishes on dying)
    u16 flags;                  //`Crawler_Flags`
    s8 state;                   //`Crawler_States`
    u8 _unk293;
    u8 configIdx;               //Which flag preset to use (from `dFlagConfigs`)
    u8 attackAndDie : 1;        //Trade damage with the EarthWalker when nearby
} Crawler_Data;

typedef enum {
    Crawler_STATE_0_Waiting_to_Fall,
    Crawler_STATE_1_Falling,
    Crawler_STATE_2,                //Unused?
    Crawler_STATE_3_Move,
    Crawler_STATE_4,                //Unused?
    Crawler_STATE_5_Hurt,
    Crawler_STATE_6_Dying
} Crawler_States;

typedef enum {
    Crawler_FLAG_Start_Upright_No_Expiry = 1,   //No lifetime timer, starts out with zero roll (instead of being flipped upsidedown, i.e. clinging from ceiling)
    Crawler_FLAG_Check_Collision = 2,           //Check HITS lines and other collision
    Crawler_FLAG_Snap_to_Track = 4,             //Set y coord using closest surface, and rotate with the surface normal
    Crawler_FLAG_Expires = 8,                   //Creates particles and vanishes after 20 seconds. Also falls when target approaches (100 units away). Starts out with 0 roll.
    Crawler_FLAG_Target_Vehicles = 0x10,        //Target any nearby vehicles (e.g. the imprisoned EarthWalker) instead of targeting the player
    Crawler_FLAG_Limit_Max_Speed = 0x20,        //Apply an upper speed limit, so the crawler doesn't zoom rapidly to their target when far away
    Crawler_FLAG_Fade_In = 0x40,                //Fade up to full opacity when in view
    Crawler_FLAG_Immune_to_Light = 0x80,        //Whether or not the crawler should be hurt by nearby light sources
    Crawler_FLAG_Play_Dying_Animation = 0x100,  //Play a dying animation instead of vanishing immediately
    Crawler_FLAG_Hurt_by_Guard_Command = 0x200, //Can be hurt by the sidekick when they use the Guard Command
    Crawler_FLAG_Bite_When_Nearby = 0x400       //Play an attack animation when near the target
} Crawler_Flags;

typedef enum {
    Crawler_MODANIM_0_Crawl_LOOP,
    Crawler_MODANIM_1_Dying,
    Crawler_MODANIM_2_Attack
} Crawler_ModAnim;

/*0x0*/ static u16 dFlagConfigs[] = {
    0, 
    Crawler_FLAG_Check_Collision, 
    Crawler_FLAG_Snap_to_Track, 
    Crawler_FLAG_Start_Upright_No_Expiry, 
    Crawler_FLAG_Snap_to_Track | Crawler_FLAG_Expires, 
    Crawler_FLAG_Start_Upright_No_Expiry | Crawler_FLAG_Check_Collision | Crawler_FLAG_Snap_to_Track | Crawler_FLAG_Target_Vehicles | Crawler_FLAG_Limit_Max_Speed | Crawler_FLAG_Fade_In | Crawler_FLAG_Immune_to_Light | Crawler_FLAG_Play_Dying_Animation | Crawler_FLAG_Hurt_by_Guard_Command,
    Crawler_FLAG_Start_Upright_No_Expiry | Crawler_FLAG_Check_Collision | Crawler_FLAG_Snap_to_Track | Crawler_FLAG_Limit_Max_Speed | Crawler_FLAG_Fade_In | Crawler_FLAG_Play_Dying_Animation,
    Crawler_FLAG_Snap_to_Track | Crawler_FLAG_Expires | Crawler_FLAG_Play_Dying_Animation | Crawler_FLAG_Bite_When_Nearby
};
/*0x10*/ static f32 dMaxSpeed = 0.1f;
/*0x14*/ static u8 dCountAtTarget = 0;
/*0x18*/ static Vec3f dCollisionPoint = VEC3F(0, 0, 0);
/*0x24*/ static f32 dCollisionRadius = 10;

static void crawler_setRotationFromTrackNormal(Object* self, TrackHeightResult* trackResult);
static void crawler_finishDying(Object* self);
static int crawler_animCallback(Object* self, Object* animObj, AnimObj_Data* animData);

// offset: 0x0 | ctor
void crawler_ctor(void* dll) { }

// offset: 0xC | dtor
void crawler_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void crawler_obj_Setup(Object* self, Crawler_Setup* objSetup, s32 reset) {
    Crawler_Data* objData = self->data;
    
    objAddObjectType(self, OBJTYPE_Baddie);
    
    self->srt.yaw = objSetup->yaw8 << 8;
    
    objInitMesgQueue(self, 2);
    
    objData->home.x = objSetup->base.x;
    objData->home.y = objSetup->base.y;
    objData->home.z = objSetup->base.z;
    objData->activateRange = objSetup->activateRange;
    objData->configIdx = objSetup->configIdx;
    objData->flags = dFlagConfigs[objData->configIdx];
    objData->timerLifetime = 0;
    objData->timerExpire = 0;
    
    if (objData->flags & Crawler_FLAG_Start_Upright_No_Expiry) {
        objData->timerPause = 0;
        self->srt.roll = 0;
        objData->state = Crawler_STATE_1_Falling;
    } else if (objData->flags & Crawler_FLAG_Expires) {
        objData->timerPause = 0;
        objData->timerLifetime = 1200;
        objData->activateRange = 100.0f;
        self->srt.roll = 0;
        objData->state = Crawler_STATE_1_Falling;
    } else {
        objData->timerPause = 400;
        self->srt.roll = -(M_180_DEGREES - 1); //Upside-down, clinging to the ceiling
        objData->state = Crawler_STATE_0_Waiting_to_Fall;
    }
    
    if (objData->flags & Crawler_FLAG_Fade_In) {
        self->opacity = 0;
    }
    
    objData->animSpeed = 0.0f;
    objData->yOffset = objSetup->yOffset;
    self->srt.transl.y = objSetup->base.y + objData->yOffset;
    objData->timerHurt = 400 + mathRnd(0, 80);
    objData->lightHurtRadius = 80.0f;
    objData->gamebitDied = objSetup->gamebitDied;
    
    if (objData->flags & Crawler_FLAG_Check_Collision) {
        objData->collision.mode = DLL27MODE_1;
        gDLL_27->vtbl->init(&objData->collision, 0, 0, 1);
        gDLL_27->vtbl->setup_hits_collider(&objData->collision, 1, &dCollisionPoint, &dCollisionRadius, 4);
        objData->collision.flags |= DLL27FLAG_40000 | DLL27FLAG_HAS_HITS_COLLIDER;
    }
    
    self->animCallback = (void*)&crawler_animCallback;
    
    func_8002674C(self);
    func_800267C4(self);
}

// offset: 0x288 | func: 1 | export: 1
void crawler_obj_Control(Object* self) {
    Crawler_Data* objData;
    Object* target;
    Object* sidekick;
    f32 dy;
    s32 i;
    s32 closestIdx;
    Vec3f delta;
    f32 range;
    f32 speed;
    s32 count;
    f32 speedSq;
    u8 setBitsCount;
    TrackHeightResult** trackResult;
    f32 distance;
    f32 closestDist;

    objData = self->data;
    closestIdx = 0;
    trackResult = NULL;
    range = 1000.0f;
    speed = 1.0f;
    closestDist = 10000.0f;
    setBitsCount = 0;
    
    //Find a target
    if (!(objData->flags & Crawler_FLAG_Target_Vehicles)) {
        target = objGetPlayer();
    } else {
        //Targeting a vehicle (e.g. the imprisoned EarthWalker)
        target = objGetNearestTypeTo(OBJTYPE_Vehicle, self, &closestDist);
    }
    if (target == NULL) {
        return;
    }
    
    //Dragon Rock: correlate max speed with the number of pools cooled
    //(TODO: does this end up affecting scorpions and non-Dragon Rock Skeetlas too, or do the flag configs prevent it?)
    dMaxSpeed = (mainGetBits(BIT_DR_Lava_Pools_Cooled_Count) * 0.1f) + 0.1f; 
    
    //Handle dying animation
    if (objData->state == Crawler_STATE_6_Dying) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;

        if (self->curModAnimId != Crawler_MODANIM_1_Dying) {
            objAnimSet(self, Crawler_MODANIM_1_Dying, 0.0f, 0);
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_7F9_Crawler_Dying, MAX_VOLUME, NULL, NULL, 0, NULL);
        }

        //Shrink
        if (self->animProgress > 0.4f) {
            self->srt.scale *= 0.95f;
        }

        //Increment gamebit when the dying animation is finished
        if (objAnimAdvance(self, 0.01f, gUpdateRate, NULL)) {
            if ((objData->gamebitDied != 0) && (objData->gamebitDied != NO_GAMEBIT)) {
                mainSetBits(objData->gamebitDied, mainGetBits(objData->gamebitDied) + 1);
            }
            crawler_finishDying(self);
        }
        return;
    } 
    
    //Handle lifetime timers
    if (objData->flags & Crawler_FLAG_Expires) {
        if (objData->timerLifetime != 0) {
            objData->timerLifetime -= gUpdateRate;

            //Create particles when expiring
            if (objData->timerLifetime <= 0) {
                objData->timerLifetime = 0;

                for (i = 0; i < 30; i++) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A3, NULL, 0, -1, NULL);
                }
                
                objData->timerExpire = 100;
                return;
            }
        } else if (objData->timerExpire != 0) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            objData->timerExpire -= gUpdateRate;
            if (objData->timerExpire <= 0) {
                objData->timerExpire = 0;
                crawler_finishDying(self);
            }
            return;
        }
    } 
    
    //React to damage
    if (func_80025F40(self, NULL, NULL, NULL)) {
        if (objData->flags & Crawler_FLAG_Play_Dying_Animation) {
            objData->state = Crawler_STATE_6_Dying;
        } else {
            crawler_finishDying(self);
        }
        return;
    } 
    
    //If 6 particular gamebits are set, don't move or do anything else
    //(@bug?: this doesn't check crawler config flags, so presumably it can affect all kinds of crawlers)
    {
        for (i = 0; i < 6; i++) {
            setBitsCount += mainGetBits(BIT_WM_Killed_SharpClaw_2573 + i);
        }
        
        if (setBitsCount >= 6) {
            crawler_finishDying(self);
            return;
        }
    }
    
    func_80025F40(self, NULL, NULL, NULL);
    
    //Don't move or do anything else until a delay timer finishes counting down
    if (objData->timerPause != 0) {
        objData->timerPause -= gUpdateRate;
        if (objData->timerPause < 0) {
            objData->timerPause = 0;
        }
        return;
    } 
    
    //React to nearby light sources ("Stay close to a lit torch. The light will kill them!")
    if (((objData->state == Crawler_STATE_3_Move) || (objData->state == Crawler_STATE_1_Falling) || (objData->state == Crawler_STATE_5_Hurt)) && 
        !(objData->flags & Crawler_FLAG_Immune_to_Light)
    ) {
        if (objData->state == Crawler_STATE_5_Hurt) {
            distance = gDLL_11_Newlfx->vtbl->FindClosestLight(&self->srt.transl, objData->lightHurtRadius + 50.0f);

            //Resume crawling if outside the light's hurt radius
            if ((distance < 0.0f) || (distance > (objData->lightHurtRadius + 30.0f))) {
                objData->state = Crawler_STATE_3_Move;
                objData->timerPause = 20;
            }
        } else {
            distance = gDLL_11_Newlfx->vtbl->FindClosestLight(&self->srt.transl, objData->lightHurtRadius + 20.0f);
            if ((distance >= 0.0f) && (distance < objData->lightHurtRadius)) {
                if (mathRnd(0, 50) == 0) {
                    gDLL_6_AMSFX->vtbl->Play(self, SOUND_7FA_Crawler_Hurt, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                
                //Die if the hurt timer runs out while in the light, otherwise back away slowly
                if (objData->timerHurt <= 0) {
                    if (objData->flags & Crawler_FLAG_Play_Dying_Animation) {
                        objData->state = Crawler_STATE_6_Dying;
                    } else {
                        crawler_finishDying(self);
                    }
                    return;
                } else if (objData->state != Crawler_STATE_5_Hurt) {
                    //Stop sound loop
                    if (objData->soundHandle != 0) {
                        gDLL_6_AMSFX->vtbl->Stop(objData->soundHandle);
                        objData->soundHandle = 0;
                    }
                    
                    objData->state = Crawler_STATE_5_Hurt;
                    
                    //Back away from the light slowly
                    self->velocity.x = -self->velocity.x / 4.0f;
                    self->velocity.z = -self->velocity.z / 4.0f;
                }
            }
        }
    } 
    
    //Check if the sidekick is using the Guard Command nearby
    if ((objData->flags & Crawler_FLAG_Hurt_by_Guard_Command) && (objData->state != Crawler_STATE_5_Hurt)) {
        sidekick = objGetSidekick();
        if (sidekick != NULL) {
            if ((vec3Distance(&self->globalPosition, &sidekick->globalPosition) < 30.0f) && 
                (((DLL_ISidekick*)sidekick->dll)->vtbl->Func25(sidekick))
            ) {
                objData->state = Crawler_STATE_5_Hurt;
                gDLL_6_AMSFX->vtbl->Play(self, SOUND_7FA_Crawler_Hurt, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
    }
    
    //Handle being hurt (by light sources or the Guard Command)
    if (objData->state == Crawler_STATE_5_Hurt) {
        //Check collision
        if (objData->flags & Crawler_FLAG_Check_Collision) {
            gDLL_27->vtbl->func_1E8(self, &objData->collision, gUpdateRateF);
            gDLL_27->vtbl->func_5A8(self, &objData->collision);
            gDLL_27->vtbl->func_624(self, &objData->collision, gUpdateRateF);
        }

        //Move laterally
        {
            speedSq = SQ(self->velocity.x) + SQ(self->velocity.z);
            if (speedSq != 0.0f) {
                speed = sqrtf(speedSq);
            }
            
            objData->animSpeed = -0.065f * speed;
            objAnimAdvance(self, objData->animSpeed, gUpdateRate, NULL);
            self->srt.transl.x += self->velocity.x * gUpdateRateF;
            self->srt.transl.z += self->velocity.z * gUpdateRateF;
        }

        objData->timerHurt -= gUpdateRate;
        
        //Set Y coord (and rotation if crawling over surfaces)
        if (objData->flags & Crawler_FLAG_Snap_to_Track) {
            closestDist = 10000.0f;
            count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &trackResult, 0, 0);
            for (i = 0; i < count; i++) {
                dy = trackResult[i]->y - self->srt.transl.f[1];
                if (dy < 0) {
                    dy *= -1.0f;
                }
                if (dy < closestDist) {
                    closestIdx = i;
                    closestDist = dy;
                }
            }
            
            if (trackResult != NULL) {
                self->srt.transl.y = trackResult[closestIdx]->y;
                crawler_setRotationFromTrackNormal(self, trackResult[closestIdx]);
            } else {
                self->srt.transl.y = objData->home.y;
            }
        } else {
            self->srt.transl.y = objData->home.y;
        }
        
        if (!(objData->flags & Crawler_FLAG_Immune_to_Light) && (objData->timerHurt <= 0)) {
            if (objData->flags & Crawler_FLAG_Play_Dying_Animation) {
                //Animate dying
                objData->state = Crawler_STATE_6_Dying;
            } else {
                //Reset
                objData->state = Crawler_STATE_0_Waiting_to_Fall;
                
                if (objData->soundHandle != 0) {
                    gDLL_6_AMSFX->vtbl->Stop(objData->soundHandle);
                    objData->soundHandle = 0;
                }
                
                self->srt.transl.x = objData->home.x;
                self->srt.transl.y = objData->home.y + objData->yOffset;
                self->srt.transl.z = objData->home.z;
            }
        } else if ((objData->flags & Crawler_FLAG_Hurt_by_Guard_Command) && (mathRnd(0, 20) == 0)) {
            objData->state = Crawler_STATE_3_Move;
            objData->timerPause = mathRnd(0, 20) + 50;
        }
    } else {
        range = vec3DistanceXZ(&target->globalPosition, &self->globalPosition);
        if ((range < objData->activateRange) || mainGetBits(BIT_Player_Immune_to_Rainbow_Scarabs)) {
            if (objData->state == Crawler_STATE_0_Waiting_to_Fall) {
                objData->state = Crawler_STATE_1_Falling;
                objData->timerPause = 1;
                self->srt.roll = 0;
            } else if (objData->state == Crawler_STATE_1_Falling) {
                //Apply gravity
                if (self->velocity.y > -10.0f) {
                    self->velocity.y += -0.1f * gUpdateRateF;
                }

                //Handle hitting the "ground" (just uses the objSetup's Y as a reference for this)
                if (self->srt.transl.y < objData->home.y) {
                    self->srt.transl.y = objData->home.y;
                    self->velocity.y = 0.0f;
                    objData->state = Crawler_STATE_3_Move;
                    objData->timerPause = mathRnd(0, 20) + 50;
                    objData->activateRange *= 2.0f;
                    objAnimSet(self, Crawler_MODANIM_0_Crawl_LOOP, 0.0f, 0);
                }
            } else if (objData->state == Crawler_STATE_3_Move) {
                //Play crawling sound loop
                if (objData->soundHandle == 0) {
                    gDLL_6_AMSFX->vtbl->Play(self, SOUND_669_Insect_Scurry_Loop, 0x39, &objData->soundHandle, NULL, 0, NULL);
                }
                
                //Check collision
                if (objData->flags & Crawler_FLAG_Check_Collision) {
                    gDLL_27->vtbl->func_1E8(self, &objData->collision, gUpdateRateF);
                    gDLL_27->vtbl->func_5A8(self, &objData->collision);
                    gDLL_27->vtbl->func_624(self, &objData->collision, gUpdateRateF);
                }
                
                //Set Y coord (and rotation if crawling over surfaces)
                if (objData->flags & Crawler_FLAG_Snap_to_Track) {
                    closestDist = 10000.0f;
                    count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &trackResult, 0, 0);
                    for (i = 0; i < count; i++) {
                        dy = trackResult[i]->y - self->srt.transl.y;
                        if (dy < 0) {
                            dy *= -1.0f;
                        }
                        if (dy < closestDist) {
                            closestIdx = i;
                            closestDist = dy;
                        }
                    }
                        
                    if (trackResult != NULL) {
                        self->srt.transl.y = trackResult[closestIdx]->y;
                        crawler_setRotationFromTrackNormal(self, trackResult[closestIdx]);
                    } else {
                        self->srt.transl.y = objData->home.y;
                    }
                } else {
                    self->srt.transl.y = objData->home.y;
                }
                
                //Correlate speed with the target's distance
                {
                    delta.x = target->srt.transl.x - self->srt.transl.x;
                    delta.y = target->srt.transl.y - self->srt.transl.y;
                    delta.z = target->srt.transl.z - self->srt.transl.z;
                    self->velocity.x = (delta.f[0] / 300.0f) * gUpdateRateF;
                    self->velocity.y = (delta.f[1] / 300.0f) * gUpdateRateF;
                    self->velocity.z = (delta.f[2] / 300.0f) * gUpdateRateF;
                    
                    if (objData->flags & Crawler_FLAG_Limit_Max_Speed) {
                        if (sqrtf(SQ(self->velocity.f[0]) + SQ(self->velocity.f[1]) + SQ(self->velocity.f[2])) > dMaxSpeed) {
                            vec3Normalize(&self->velocity);
                            self->velocity.x *= gUpdateRateF * dMaxSpeed;
                            self->velocity.y *= gUpdateRateF * dMaxSpeed;
                            self->velocity.z *= gUpdateRateF * dMaxSpeed;
                        }
                    }
                }
                
                //Attack when in range
                {
                    diPrintf(" Range %f  ", &range);
                    
                    //Play bite animation
                    if ((self->curModAnimId == Crawler_MODANIM_0_Crawl_LOOP) && (objData->flags & Crawler_FLAG_Bite_When_Nearby) && (range < 15.0f)) {
                        gDLL_6_AMSFX->vtbl->Play(self, SOUND_B5B_Crawler_Attack, MAX_VOLUME, NULL, NULL, 0, NULL);
                        objAnimSet(self, Crawler_MODANIM_2_Attack, 0.0f, 0);
                    }
                
                    //Hurt the target when nearby
                    if ((range < 13.0f) || 
                        ((objData->flags & Crawler_FLAG_Target_Vehicles) && (self->objhitInfo->unk58 & ObjHitInfo_FLAG_8) && (range < 50.0f))
                    ) {
                        dCountAtTarget++;

                        //Message the target during the attack animation
                        if (self->curModAnimId == Crawler_MODANIM_2_Attack) {
                            if ((self->animProgress > 0.3f) && (self->animProgress < 0.7f)) {
                                objSendMesg(target, 0x60004, self, (void* )1);
                                dCountAtTarget = 0;
                            }
                        }
                        
                        if (mainGetBits(BIT_Player_Immune_to_Rainbow_Scarabs)) {
                            dCountAtTarget = 0;
                        } else if ((dCountAtTarget >= 3) || ((objData->flags & Crawler_FLAG_Target_Vehicles) && (dCountAtTarget >= 3))) {
                            gDLL_6_AMSFX->vtbl->Play(self, SOUND_7F8_Crawler_Chomp, MAX_VOLUME, NULL, NULL, 0, NULL);

                            if (!(objData->flags & Crawler_FLAG_Target_Vehicles)) {
                                //Message the player
                                objSendMesg(target, 0x60004, self, (void* )1);
                            } else {
                                //Set up attacking the EarthWalker and dying in the process
                                objData->attackAndDie = TRUE;
                            }
                            dCountAtTarget = 0;
                        }
                        
                        //Get pushed back slightly
                        if ((objData->flags & Crawler_FLAG_Target_Vehicles) == FALSE) {
                            self->srt.transl.x += (-self->velocity.x * 26.0f);
                            self->srt.transl.z += (-self->velocity.z * 26.0f);
                        } else {
                            self->srt.transl.x += (-self->velocity.x * 8.0f);
                            self->srt.transl.z += (-self->velocity.z * 8.0f);
                        }

                        //Random delay
                        objData->timerPause = mathRnd(0, 20) + 100;
                    }
                }

                //Point yaw at target
                self->srt.yaw = mathAtan2f(target->srt.transl.x - self->srt.transl.x, target->srt.transl.z - self->srt.transl.z) + (M_180_DEGREES - 1);
                
                //Advance animation
                {
                    //Set animation speed
                    speedSq = SQ(self->velocity.x) + SQ(self->velocity.z);
                    if (speedSq != 0.0f) {
                        speed = sqrtf(speedSq);
                    }
                    
                    switch (self->curModAnimId) {
                    case Crawler_MODANIM_0_Crawl_LOOP:
                        objData->animSpeed = 0.065f * speed;
                        break;
                    case Crawler_MODANIM_2_Attack:
                        objData->animSpeed = 0.03f;
                        break;
                    case Crawler_MODANIM_1_Dying:
                        objData->animSpeed = 0.01f;
                        break;
                    }
                    
                    //Continue with crawl animation when the current animation is finished
                    if (objAnimAdvance(self, objData->animSpeed, gUpdateRate, NULL) && (self->curModAnimId != Crawler_MODANIM_0_Crawl_LOOP)) {
                        objAnimSet(self, Crawler_MODANIM_0_Crawl_LOOP, 0.0f, 0);
                    }
                }
                
                //Move laterally
                self->srt.transl.x += self->velocity.x * gUpdateRateF;
                self->srt.transl.z += self->velocity.z * gUpdateRateF;
            }
        } else if (objData->state == Crawler_STATE_1_Falling) {
            //Apply gravity
            if (self->velocity.y > -1.0f) {
                self->velocity.y += -0.01f * gUpdateRateF;
            }

            //Handle hitting the "ground" (just uses the objSetup's Y as a reference for this)
            if (self->srt.transl.y < objData->home.y) {
                self->srt.transl.y = objData->home.y;
                self->velocity.y = 0.0f;
                objData->state = Crawler_STATE_3_Move;
                objData->timerPause = mathRnd(0, 20) + 50;
                objData->activateRange = 2.0f * objData->activateRange;
                objAnimSet(self, Crawler_MODANIM_0_Crawl_LOOP, 0.0f, 0);
            }

            //Move vertically
            self->srt.transl.y += self->velocity.y * gUpdateRateF;
        }
        
        if (objData->state == Crawler_STATE_1_Falling) {
            self->srt.transl.y += self->velocity.y * gUpdateRateF; //@bug? Y can update twice per tick when falling
        }
        
        //Randomly play a random chirp sound
        if (mathRnd(0, 50) == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_7BB_Crawler_Chirp + mathRnd(0, 2), MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
}

static const char str1[] = " Set MOve ";
static const char str2[] = " BITE ";

// offset: 0x17B0 | func: 2 | export: 2
void crawler_obj_Update(Object* self) {
    Crawler_Data* objData;
    Object* target;
    f32 distance;

    objData = self->data;
    distance = 100000.0f;
    
    if (objData->attackAndDie) {
        //Hurt the target
        if (!(objData->flags & Crawler_FLAG_Target_Vehicles)) {
            target = objGetPlayer();
        } else {
            target = objGetNearestTypeTo(OBJTYPE_Vehicle, self, &distance);
        }
        func_8002635C(target, self, Damage_Type_Sword_Staff_Strike2, 1, 0);
        objData->state = Crawler_STATE_6_Dying;
        objData->attackAndDie = FALSE;
    }
}

// offset: 0x1888 | func: 3 | export: 3
void crawler_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Crawler_Data* objData = self->data;
    
    if (objData->flags & Crawler_FLAG_Fade_In) {
        if (self->opacity < OBJECT_OPACITY_MAX) {
            if (self->opacity > (OBJECT_OPACITY_MAX - gUpdateRate)) {
                self->opacity = OBJECT_OPACITY_MAX;
                objData->flags &= ~Crawler_FLAG_Fade_In;
            } else {
                self->opacity += gUpdateRate;
            }
        }
    }
    
    if (visibility && (objData->timerExpire == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x194C | func: 4 | export: 4
void crawler_obj_Free(Object* self, s32 onlySelf) {
    Crawler_Data* objData = self->data;

    if (objData->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
    
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0x19D8 | func: 5 | export: 5
u32 crawler_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x19E8 | func: 6 | export: 6
u32 crawler_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Crawler_Data);
}

// offset: 0x19FC | func: 7
void crawler_setRotationFromTrackNormal(Object* self, TrackHeightResult* trackResult) {
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
int crawler_animCallback(Object* self, Object* animObj, AnimObj_Data* animData/*, s8 prevCallbackValue */) { //@bug: missing last argument
    Crawler_Data* objData = self->data;

    objData->state = Crawler_STATE_1_Falling;

    return 0;
}

static const char str3[] = " Free CRawler";
static const char str4[] = " Deactivateing Crawler ";

// offset: 0x1AE8 | func: 9
void crawler_finishDying(Object* self) {
    ObjSetup* objSetup = self->setup;
    
    if (objSetup->uID == 0){
        //Free crawler
        func_800267A4(self);
        objFreeObject(self);
    } else {
        //Deactivate crawler
        objDisable(self);
        func_800267A4(self);
        objFreeObjectType(self, OBJTYPE_Baddie);    
        self->srt.flags |= OBJFLAG_INVISIBLE;
    }
}
