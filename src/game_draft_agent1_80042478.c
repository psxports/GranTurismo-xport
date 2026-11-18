#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 80042478 */
/* Original: GTMAIN.EXE:sub_80042478. */ uint32 gt_draft_80042478(void)
{
    uint32 table = gtmain_parts_table((sint16)r_u16(0x800BEA20u));
    sint32 category = (sint16)r_u16(0x800BEA20u), level;
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 context = r_u32(0x800BEA64u);
    uint32 part = r_u16(context + offset), record, minimum, maximum, value, count;
    if (!part) return 0xFFFFFFFFu;
    record = table + 20u * (part - 1u);
    minimum = r_u8(record + 2u);
    maximum = r_u8(record + 3u);
    value = r_u8(record);
    count = r_u8(record + 1u);
    level = gtmain_parts_parameter_level(minimum, maximum, value, count);
    context = r_u32(0x800BEA64u);
    w_u8(context + 75u, (uint32)level);
    context = r_u32(0x800BEA64u);
    value = r_u8(record);
    w_u8(context + 66u, value);
    printf((const char *)psx_addr(0x8008D520, 59));
    category = (sint16)r_u16(0x800BEA20u);
    printf((const char *)psx_addr(0x8008D55C, 55), category, part);
    context = r_u32(0x800BEA64u);
    count = r_u8(record + 1u);
    value = r_u8(context + 66u);
    printf((const char *)psx_addr(0x8008D6D4, 37), value, (sint32)((uint32)level + 1u), count);
    minimum = r_u8(record + 2u);
    maximum = r_u8(record + 3u);
    printf((const char *)psx_addr(0x8008D6FC, 36), minimum, maximum);
    return 1u;
}
