#include "game_draft_registry.h"

/* TODO The secondary entry consumes a nonstandard incoming v0 counter in a4 */
uint32 gt_draft_800380AC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 counter = (sint16)r_u16(0x801F300Au);
    return gt_draft_800380B4(a0, a1, a2, a3, (uint32)counter, a5, a6, a7);
}
