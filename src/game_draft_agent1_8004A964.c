#include "game_draft_registry.h"

/* Decrement the halfword counter before testing its signed low half */
/* Original: GTMAIN.EXE:sub_8004A964. */ uint32 gt_draft_8004A964(void)
{
    uint32 value = r_u16(0x800B53BCu) - 1u;
    w_u16(0x800B53BCu, value);
    return (sint32)(value << 16u) < 1 ? 7u : 0u;
}
