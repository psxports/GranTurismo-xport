#include "game_first2_registry.h"
#include "game_draft_registry.h"

static uint32 parts_resource(sint32 category)
{
    /* TODO Bind unselected parts resource lookup 8001EB94 */
    return gt_draft_unresolved_call(0x8001EB94u, (uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

#include <stdio.h>

static uint32 parts_level(uint32 minimum, uint32 maximum, uint32 value, uint32 count)
{
    /* TODO Bind unselected level calculation 800417AC */
    return gt_draft_unresolved_call(0x800417ACu, minimum, maximum, value, count, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80042158 */
uint32 gt_draft_80042158(void)
{
    static const uint8 initial_sources[6] = {12, 19, 11, 18, 10, 17};
    static const uint8 initial_destinations[6] = {69, 82, 68, 81, 67, 80};
    static const uint8 damper_sources[8] = {13, 15, 14, 16, 20, 22, 21, 23};
    static const uint8 damper_destinations[8] = {70, 72, 71, 73, 83, 85, 84, 86};
    static const uint8 diagnostic_offsets[14] = {67, 80, 68, 81, 69, 82, 70, 72, 71, 73, 83, 85, 84, 86};
    static const uint32 diagnostic_formats[14] = {
        0x8008D594u, 0x8008D5ACu, 0x8008D5C4u, 0x8008D5ACu, 0x8008D5DCu, 0x8008D5ACu,
        0x8008D5F4u, 0x8008D610u, 0x8008D62Cu, 0x8008D648u, 0x8008D664u, 0x8008D680u,
        0x8008D69Cu, 0x8008D6B8u
    };
    uint32 table = parts_resource((sint16)r_u16(0x800BEA20u));
    sint32 category = (sint16)r_u16(0x800BEA20u);
    uint32 context = r_u32(0x800BEA64u);
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 part = r_u16(context + offset), record, index, minimum, maximum, value, count, result;
    if (!part) return 0xFFFFFFFFu;
    record = table + 72u * (part - 1u);
    for (index = 0u; index < 6u; ++index) {
        if (index) context = r_u32(0x800BEA64u);
        value = r_u8(record + initial_sources[index]);
        w_u8(context + initial_destinations[index], value);
    }
    minimum = r_u8(record + 30u);
    maximum = r_u8(record + 31u);
    value = r_u8(record + 13u);
    count = r_u8(record + 38u);
    result = parts_level(minimum, maximum, value, count);
    context = r_u32(0x800BEA64u);
    w_u8(context + 76u, result);
    minimum = r_u8(record + 45u);
    maximum = r_u8(record + 46u);
    value = r_u8(record + 20u);
    count = r_u8(record + 53u);
    result = parts_level(minimum, maximum, value, count);
    context = r_u32(0x800BEA64u);
    w_u8(context + 89u, result);
    for (index = 0u; index < 8u; ++index) {
        context = r_u32(0x800BEA64u);
        value = r_u8(record + damper_sources[index]);
        w_u8(context + damper_destinations[index], value);
    }
    printf((const char *)psx_addr(0x8008D520u, 1u));
    category = (sint16)r_u16(0x800BEA20u);
    printf((const char *)psx_addr(0x8008D55Cu, 1u), category, part);
    for (index = 0u; index < 14u; ++index) {
        context = r_u32(0x800BEA64u);
        value = r_u8(context + diagnostic_offsets[index]);
        printf((const char *)psx_addr(diagnostic_formats[index], 1u), value);
    }
    return 0u;
}
