#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static uint32 parts_resource(sint32 category)
{
    /* TODO Bind unselected parts resource lookup 8001EB94 */
    return gt_draft_unresolved_call(0x8001EB94u, (uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 8004283C */
uint32 gt_draft_8004283C(void)
{
    uint32 table = parts_resource((sint16)r_u16(0x800BEA20u));
    sint32 category = (sint16)r_u16(0x800BEA20u);
    uint32 context = r_u32(0x800BEA64u);
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 part = r_u16(context + offset), value;
    if (!part) return 0xFFFFFFFFu;
    value = r_u8(table + 56u * (part - 1u) + 16u);
    w_u8(context + 63u, value);
    printf((const char *)psx_addr(0x8008D520u, 1u));
    category = (sint16)r_u16(0x800BEA20u);
    printf((const char *)psx_addr(0x8008D55Cu, 1u), category, part);
    context = r_u32(0x800BEA64u);
    value = r_u8(context + 63u);
    printf((const char *)psx_addr(0x8008D788u, 1u), value);
    return 3u;
}
