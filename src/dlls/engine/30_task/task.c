#include "PR/ultratypes.h"
#include "dll.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "sys/main.h"

static u8 sRecentlyCompleted[5];
static u8 sCompletionIdx;
static s8 sRecentlyCompletedNextIdx;

// offset: 0x0 | ctor
void task_ctor(void *self) {

}

// offset: 0xC | dtor
void task_dtor(void *self) {

}

// offset: 0x18 | func: 0 | export: 0
void task_LoadRecentlyCompleted(void) {
    s32 i;
    u8 val;

    for (i = 0; i != 5; i++) {
        val = mainGetBits(BIT_Recent_Task_1 + i);
        sRecentlyCompleted[i] = val;

        if (val != 0) {
            sRecentlyCompletedNextIdx = i;
        }
    }

    val = mainGetBits(BIT_Next_Game_Task);
    sCompletionIdx = val;
    if (val == 0) {
        sCompletionIdx = 1;
        sRecentlyCompletedNextIdx = -1;
    }
}

// offset: 0xDC | func: 1 | export: 1
void task_MarkTaskCompleted(u8 task) {
    s16 i;
    s16 bs_entry;
    s16 bit_idx;
    u32 bs_value;
    s16 bs_entry2;

    // Bail if already recently completed
    for (i = 0; i < 5; i++) {
        if (task == sRecentlyCompleted[i]) {
            return;
        }
    }

    // Add task to recently completed list
    if (sRecentlyCompletedNextIdx != 4) {
        // Append if there's room
        sRecentlyCompletedNextIdx++;
        sRecentlyCompleted[sRecentlyCompletedNextIdx] = task;

        mainSetBits(BIT_Recent_Task_1 + sRecentlyCompletedNextIdx, task);
    } else {
        // Otherwise, shift everything down and add to the end
        for (i = 0; i < 4; i++) {
            sRecentlyCompleted[i] = sRecentlyCompleted[i + 1];
        }

        sRecentlyCompleted[4] = task;

        for (i = 0; i < 5; i++) {
            mainSetBits(BIT_Recent_Task_1 + i, sRecentlyCompleted[i]);
        }
    }

    // Set bit for task in bitstring
    //
    // This is a 256-bit bitstring from bit entry 303 to 315 (8 entries)
    bs_entry = (task / 32) + BIT_Task_Bits_1;

    bs_value = mainGetBits(bs_entry);
    bit_idx = task % 32;
    bs_value = (1 << (bit_idx)) | bs_value;

    mainSetBits(bs_entry, bs_value);

    // Determine new completion index
    if (sCompletionIdx == task) {
        do {
            sCompletionIdx++;

            bs_entry2 = (sCompletionIdx / 32) + BIT_Task_Bits_1;
            if (bs_entry2 != bs_entry) {
                bs_entry = bs_entry2;

                bs_value = mainGetBits(bs_entry2);
            }
            bit_idx = sCompletionIdx % 32;

        } while ((bs_value >> bit_idx) & 1);

        mainSetBits(BIT_Next_Game_Task, sCompletionIdx);
    }

    // hmm
    if (!task) {
        gDLL_29_Gplay->vtbl->savepoint(NULL, 0, GPLAY_SAVEPOINT_SkipMapSave, mapGetLayer());
    }
}

// offset: 0x368 | func: 2 | export: 2
u8 task_GetNumRecentlyCompleted(void) {
    return sRecentlyCompletedNextIdx + 1;
}

// offset: 0x390 | func: 3 | export: 3
/**
  * Get descriptive text for a recently completed gameplay task. 
  */
char *task_GetRecentlyCompletedTaskText(u8 idx) {
    return gDLL_21_Gametext->vtbl->get_text(GAMETEXT_0F4_Task_Header + sRecentlyCompleted[idx], 0);
}

// offset: 0x3F8 | func: 4 | export: 4
/**
  * Get hint text about the upcoming gameplay task. 
  */
char *task_GetCompletionTaskText(void) {
    return gDLL_21_Gametext->vtbl->get_text(GAMETEXT_0F4_Task_Header + sCompletionIdx, 1);
}

// offset: 0x454 | func: 5 | export: 5
/**
  * Get hint text about the upcoming gameplay task. 
  */
s16 task_GetCompletionPercentage(void) {
    f32 fraction = sCompletionIdx / 206.0f;
    return fraction * 100.0f;
}
