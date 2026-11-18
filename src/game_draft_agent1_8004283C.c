#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 8004283C */
/* Original: GTMAIN.EXE:sub_8004283C. */ uint32 gt_draft_8004283C(void)
{
    uint32 table = gtmain_parts_table((sint16)r_u16(0x800BEA20u));
    sint32 category = (sint16)r_u16(0x800BEA20u);
    uint32 context = r_u32(0x800BEA64u);
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 part = r_u16(context + offset), value;
    if (!part) return 0xFFFFFFFFu;
    value = r_u8(table + 56u * (part - 1u) + 16u);
    w_u8(context + 63u, value);
    printf((const char *)psx_addr(0x8008D520, 59));
    category = (sint16)r_u16(0x800BEA20u);
    printf((const char *)psx_addr(0x8008D55C, 55), category, part);
    context = r_u32(0x800BEA64u);
    value = r_u8(context + 63u);
    printf((const char *)psx_addr(0x8008D788, 21), value);
    return 3u;
}
