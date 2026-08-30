#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitEnable;
/*1A*/ s16 unk1A;
/*1C*/ u8 maxOpacity;
/*1D*/ u8 minOpacity;
/*1E*/ s8 animatorID;
/*1E*/ s8 unk1F;
/*20*/ s8 unk20;
/*21*/ u8 removeCollision; //When enabled, shape is tangible while visible
/*22*/ u16 minDistance;
} PortalTexAnimator_Setup;

typedef struct {
/*00*/ s32 animatedVertexCount;
/*04*/ f32 minDistance;
/*08*/ f32 maxDistance;
/*0C*/ f32 minDistanceCopy;
/*10*/ f32* unk10;
/*14*/ s16 vertexOpacity;
/*16*/ s8 animatorID;
/*17*/ s8 enabled;              //unfinished, doesn't affect behaviour
/*18*/ s8 blockFound;
/*19*/ s8 unk19;
} PortalTexAnimator_Data;

static void PortalTexAnimator_animateVertices(PortalTexAnimator_Data* objdata, PortalTexAnimator_Setup* setup, Block* block);

// offset: 0x0 | ctor
void PortalTexAnimator_ctor(void* dll){ }

// offset: 0xC | dtor
void PortalTexAnimator_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void PortalTexAnimator_obj_Setup(Object* self, PortalTexAnimator_Setup* objSetup, s32 reset) {
    PortalTexAnimator_Data* objdata;

    objdata = self->data;

    objdata->unk19 = -1;
    objdata->minDistance = objSetup->minDistance;
    objdata->maxDistance = objSetup->base.loadDistance * 8;
}

// offset: 0x78 | func: 1 | export: 1
void PortalTexAnimator_obj_Control(Object* self) {
    PortalTexAnimator_Data* objdata;
    PortalTexAnimator_Setup* setup;
    Block* block;
    f32 distance;
    f32 blendValue;

    setup = (PortalTexAnimator_Setup*)self->setup;
    objdata = self->data;

    //Get the object's local BLOCKS model
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objdata->blockFound = FALSE;
        return;
    }

    //Bail if the BLOCKS model isn't animatable
    if (!(block->vtxFlags & 8)) {
        return;
    }

    //Animate tagged shapes' vertex opacity based on player distance
    if (objdata->animatedVertexCount == 0) {
        objdata->animatorID = setup->animatorID;
        objdata->animatedVertexCount = blockGetAnimatorVertexCount(self, objdata->animatorID);

        if (objdata->animatedVertexCount == 0) {
            objdata->animatorID = 0;
        }

        //Bail if no shape animation tag is specified
        if (!objdata->animatorID){
            return;
        }

        objdata->minDistance = setup->minDistance;
        objdata->maxDistance = setup->base.loadDistance * 8;
        objdata->minDistanceCopy = setup->minDistance;

        //Check gamebits (unfinished: doesn't do anything with this)
        if (setup->gamebitEnable == NO_GAMEBIT) {
            objdata->enabled = TRUE;
        } else {
            objdata->enabled = mainGetBits(setup->gamebitEnable);
        }

        //Set both vertex animation buffers' animated vertices to max opacity
        objdata->vertexOpacity = setup->maxOpacity;
        PortalTexAnimator_animateVertices(objdata, setup, block); 
        block->vtxFlags ^= 1;
        PortalTexAnimator_animateVertices(objdata, setup, block);
        block->vtxFlags ^= 1;
    } 

    if (objdata->animatorID) {
        distance = vec3Distance(&self->globalPosition, &objGetPlayer()->globalPosition) - objdata->minDistance;
        if (distance < 0.0f) {
            distance = 0.0f;
        }

        blendValue = distance / (objdata->maxDistance - objdata->minDistance);
        if (blendValue < 0.0f) {
            blendValue = 0.0f;
        }

        objdata->vertexOpacity = setup->minOpacity + (setup->maxOpacity - setup->minOpacity) * blendValue;
        PortalTexAnimator_animateVertices(objdata, setup, block);
    }
}

// offset: 0x310 | func: 2
void PortalTexAnimator_animateVertices(PortalTexAnimator_Data* objdata, PortalTexAnimator_Setup* setup, Block* block) {
    BlockShape* shapes;
    Vtx_t* vertices;
    s32 shapeIdx;
    s32 vertexIndex;

    vertices = block->vertices2[block->vtxFlags & 1];

    //Iterate over shapes, and update all vertices' alpha on shapes with matching animatorID tag
    for (shapeIdx = 0, shapes = block->shapes; shapeIdx < block->shapeCount; shapeIdx++){
        if (objdata->animatorID == shapes[shapeIdx].animatorID){
            for (vertexIndex = shapes[shapeIdx].vtxBase; vertexIndex < shapes[shapeIdx + 1].vtxBase; vertexIndex++){
                vertices[vertexIndex].cn[3] = objdata->vertexOpacity; //@bug: setting a 16-bit value on an 8-bit colour field
            }

            //Switch shape's draw flags when opacity is zero
            if (objdata->vertexOpacity == 0){
                shapes[shapeIdx].flags |= RENDER_SHAPE_HIDE;
                if (setup->removeCollision){
                    shapes[shapeIdx].flags |= RENDER_UNK800;
                }
            } else {
                shapes[shapeIdx].flags &= ~RENDER_SHAPE_HIDE;
                if (setup->removeCollision){
                    shapes[shapeIdx].flags &= ~RENDER_UNK800;
                }
            }
        }
    }

}

// offset: 0x414 | func: 3 | export: 2
void PortalTexAnimator_obj_Update(Object *self) { }

// offset: 0x420 | func: 4 | export: 3
void PortalTexAnimator_obj_Print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x474 | func: 5 | export: 4
void PortalTexAnimator_obj_Free(Object* self, s32 onlySelf) {
    PortalTexAnimator_Data* objdata = self->data;

    if (objdata->unk10) {
        mmFree(objdata->unk10);
    }
}

// offset: 0x4C4 | func: 6 | export: 5
u32 PortalTexAnimator_obj_GetModelFlags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x4D4 | func: 7 | export: 6
u32 PortalTexAnimator_obj_GetDataSize(Object* self, u32 offsetAddr){
    return sizeof(PortalTexAnimator_Data);
}

/*0x0*/ static const char str_0[] = "%d:%d,%d %d:%d\n";
