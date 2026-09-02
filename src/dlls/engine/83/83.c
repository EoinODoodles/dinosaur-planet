#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"

// offset: 0x0 | ctor
void dll_83_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_83_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_83_func_18(Object* triggerObj, Object* activatedBy, s8 direction, s32 activatorDistSquared) {
    if (direction == 1) {
        mainSetBits(BIT_Play_Seq_0107_Rocky_Intro_Unused, 1);
    } else if (direction == -1) {
        mainSetBits(BIT_Play_Seq_0107_Rocky_Intro_Unused, 0);
    }

}

// offset: 0xA4 | func: 1 | export: 1
void dll_83_func_A4(Object* triggerObj, Object* activatedBy, s8 direction, s32 activatorDistSquared) {
    if (direction == 1) {
        mainSetBits(BIT_Play_Seq_00D7_Swapped_to_Krystal, 1);
    } else if (direction == -1) {
        mainSetBits(BIT_Play_Seq_00D7_Swapped_to_Krystal, 0);
    }
}
