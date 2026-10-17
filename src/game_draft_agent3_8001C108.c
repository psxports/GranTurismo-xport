#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* GTMAIN.EXE 8001C108 */
uint32 gt_draft_8001C108(uint32 context, uint32 ordering, sint32 view_index)
{
    uint32 group;
    w_u32(context + 772u, ordering);
    group = (uint32)view_index - 1u;
    if (!view_index) group = r_u32(0x800BFF6Cu);
    gt_draft_8001A758(context, group);
    return gt_draft_8001A0E0(context);
}
