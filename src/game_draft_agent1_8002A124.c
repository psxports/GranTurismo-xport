#include "game_draft_registry.h"

static uint32 race_caption_submit(uint32 ordering)
{
    /* TODO Bind unselected caption submitter 8006F798 */
    return gt_draft_unresolved_call(0x8006F798u, ordering,
        0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 8002A124 */
uint32 gt_draft_8002A124(void)
{
    uint32 mode = r_u16(0x800B63D6u);
    if (mode - 1u < 2u) return race_caption_submit(0x801A2FDCu);
    return 0u;
}
