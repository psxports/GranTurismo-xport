#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static uint32 parts_resource(sint32 category)
{
    /* TODO Bind unselected parts resource lookup 8001EB94 */
    return gt_draft_unresolved_call(0x8001EB94u, (uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static sint32 parts_level(uint32 minimum, uint32 maximum, uint32 value, uint32 count)
{
    /* TODO Bind unselected level calculation 800417AC */
    return (sint32)gt_draft_unresolved_call(0x800417ACu, minimum, maximum, value, count, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 800426B8 */
uint32 gt_draft_800426B8(void)
{
    uint32 table = parts_resource((sint16)r_u16(0x800BEA20u));
    sint32 category = (sint16)r_u16(0x800BEA20u), level;
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 context = r_u32(0x800BEA64u);
    uint32 part = r_u16(context + offset), record, minimum, maximum, value, count;
    if (!part) return 0xFFFFFFFFu;
    printf((const char *)psx_addr(0x8008D520u, 1u));
    category = (sint16)r_u16(0x800BEA20u);
    printf((const char *)psx_addr(0x8008D55Cu, 1u), category, part);
    record = table + 28u * (part - 1u);
    minimum = r_u8(record + 2u);
    maximum = r_u8(record + 3u);
    value = r_u8(record);
    count = r_u8(record + 4u);
    level = parts_level(minimum, maximum, value, count);
    context = r_u32(0x800BEA64u);
    w_u8(context + 74u, (uint32)level);
    context = r_u32(0x800BEA64u);
    value = r_u8(record);
    w_u8(context + 64u, value);
    context = r_u32(0x800BEA64u);
    count = r_u8(record + 4u);
    value = r_u8(context + 64u);
    printf((const char *)psx_addr(0x8008D748u, 1u), value, (sint32)((uint32)level + 1u), count);
    minimum = r_u8(record + 2u);
    maximum = r_u8(record + 3u);
    printf((const char *)psx_addr(0x8008D6FCu, 1u), minimum, maximum);
    minimum = r_u8(record + 5u);
    maximum = r_u8(record + 6u);
    value = r_u8(record + 1u);
    count = r_u8(record + 7u);
    level = parts_level(minimum, maximum, value, count);
    context = r_u32(0x800BEA64u);
    w_u8(context + 87u, (uint32)level);
    context = r_u32(0x800BEA64u);
    value = r_u8(record + 1u);
    w_u8(context + 77u, value);
    context = r_u32(0x800BEA64u);
    count = r_u8(record + 7u);
    value = r_u8(context + 77u);
    printf((const char *)psx_addr(0x8008D768u, 1u), value, (sint32)((uint32)level + 1u), count);
    minimum = r_u8(record + 5u);
    maximum = r_u8(record + 6u);
    printf((const char *)psx_addr(0x8008D6FCu, 1u), minimum, maximum);
    return 2u;
}
