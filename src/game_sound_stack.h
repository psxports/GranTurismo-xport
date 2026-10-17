#ifndef GT_GAME_SOUND_STACK_H
#define GT_GAME_SOUND_STACK_H

#include "game_local_stack.h"

static uint32 gt_sound_stack_workspace(uint32 offset, uint32 size, uint32 owner)
{
    return gt_local_stack_workspace(offset, size, owner);
}

#endif
