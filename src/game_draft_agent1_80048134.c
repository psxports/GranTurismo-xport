#include "game_draft_registry.h"

/* Pass the original V0 context explicitly to the secondary entry */
/* GTMAIN.EXE 80048134 */
/* Original: GTMAIN.EXE:sub_80048134. */ void gt_draft_80048134(uint32 owner, uint32 ordering, sint32 x, sint32 y,
    uint32 carried_argument, sint32 intensity)
{
    uint32 context = r_u32(0x800BDD08u);
    gt_draft_8004813C(owner, ordering, x, y,
        carried_argument, intensity, context);
}
