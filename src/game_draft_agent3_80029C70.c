#include "game_draft_registry.h"

static uint32 race_panel_boundary(uint32 owner, uint32 ordering)
{
    /* TODO Bind unselected 8002955C, 800295A8 and 80029614 services */
    return gt_draft_unresolved_call(owner, ordering, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80029C70 */
uint32 gt_draft_80029C70(uint32 ordering)
{
    (void)race_panel_boundary(0x8002955Cu, ordering);
    (void)race_panel_boundary(0x800295A8u, ordering);
    return race_panel_boundary(0x80029614u, ordering);
}
