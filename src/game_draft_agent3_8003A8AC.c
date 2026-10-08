#include "game_draft_registry.h"

/* Full exported listing supplies the missing pseudocode wrapper */
/* TODO The raw font and buffer address boundary remains provisional */
uint32 gt_draft_8003A8AC(uint32 a0, uint32 a1, uint32 a2, uint32 a3,
                       uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gt_draft_8003A768(0x800C0070u, a0, a1, a2, a3, a4, 0u, 0u);
}
