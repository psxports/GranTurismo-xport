#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static uint32 parts_resource(sint32 category)
{
    /* TODO Bind unselected parts resource lookup 8001EB94 */
    return gt_draft_unresolved_call(0x8001EB94u, (uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80042910 */
uint32 gt_draft_80042910(void)
{
    uint32 table = parts_resource((sint16)r_u16(0x800BEA20u));
    sint32 category = (sint16)r_u16(0x800BEA20u);
    uint32 context = r_u32(0x800BEA64u);
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 part = r_u16(context + offset), record, value, index;
    if (!part) return 0xFFFFFFFFu;
    record = table + 68u * (part - 1u);
    value = r_u8(record);
    w_u8(context + 62u, value);
    context = r_u32(0x800BEA64u);
    for (index = 0u; index < 8u; ++index) {
        value = r_u16(record + 2u + 2u * index);
        w_u16(context + 46u + 2u * index, value);
    }
    printf((const char *)psx_addr(0x8008D520u, 1u));
    category = (sint16)r_u16(0x800BEA20u);
    printf((const char *)psx_addr(0x8008D55Cu, 1u), category, part);
    context = r_u32(0x800BEA64u);
    value = r_u8(context + 62u);
    printf((const char *)psx_addr(0x8008D7A0u, 1u), value);
    for (index = 0u; index < 8u; ++index) {
        context = r_u32(0x800BEA64u);
        value = r_u16(context + 46u + 2u * index);
        printf((const char *)psx_addr(0x8008D7B8u + 24u * index, 1u), value);
    }
    return 4u;
}
