#ifndef _DLLS_307_H
#define _DLLS_307_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 gamebitOpenA;           //animCallback advances door to "Opening" state when this gamebit (and gamebitOpenB if specified) is set, or to "Closing" state when unset (obj must be in a sequence!)
    s16 gamebitRestoreState;    //Restores state when the door loads (either "Open" or "Closed" state) - objSeq will use a preemptTime to skip the door to its open position when restoring "Open" state.
    s16 preemptTime;            //The sequence time to jump to when the object loads in its "Open" state (state restored via `gamebitRestoreState`).
    s8 objSeqIdx;               //The door opening cutscene's objSeqIdx
    u8 yaw;
    u8 preemptEnabledActors;    //Configures which actors to include when the door's sequence is played using a preemptTime
    u8 scale;                   //Scale factor for the door (0x40 is 1x scale)
    s16 gamebitOpenB;           //When specified, animCallback advances door to "Opening" state when this gamebit and gamebitOpenA are set (obj must be in a sequence!)
    s16 gamebitCameraBack;      //When specified, animCallback flips part (`flipBitsCameraBack`) of this gamebit's value if the camera is behind the door when the door finishes opening/closing (obj must be in a sequence!)
    s16 gamebitCameraFront;     //When specified, animCallback flips part (`flipBitsCameraFront`) of this gamebit's value if the camera is in front of the door when the door finishes opening/closing (obj must be in a sequence!)
    u8 flipBitsCameraBack;      //The section of `gamebitCameraBack`'s value to flip when `gamebitCameraBack`'s value is updated
    u8 flipBitsCameraFront;     //The section of `gamebitCameraFront`'s value to flip when `gamebitCameraFront`'s value is updated
} SeqDoor_Setup;

typedef enum {
    SeqDoor_SEQCMD_1_Finished_Closing = 1,
    SeqDoor_SEQCMD_2_Finished_Opening = 2
} SeqDoor_ObjSeqMessages;

#endif // _DLLS_307_H

