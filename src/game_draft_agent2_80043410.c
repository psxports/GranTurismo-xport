#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 80043410 */
/* Original: GTMAIN.EXE:sub_80043410. */ uint32 gt_draft_80043410(void)
{
    static const uint8 source_offsets[8] = {30, 34, 32, 36, 45, 49, 47, 51};
    static const uint8 destination_offsets[8] = {70, 72, 71, 73, 83, 85, 84, 86};
    uint32 table = gtmain_parts_table(33);
    uint32 offset = r_u8(0x800941DEu);
    sint32 level = (sint16)r_u16(0x800929ACu) - 1;
    uint32 context = r_u32(0x800BEA64u);
    uint32 part = r_u16(context + offset), record, count, minimum, maximum, result, index;
    w_u8(context + 76u, (uint32)level);
    record = table + 72u * (part - 1u);
    count = r_u8(record + 38u);
    for (index = 0u; index < 4u; ++index) {
        minimum = r_u8(record + source_offsets[index]);
        maximum = r_u8(record + source_offsets[index] + 1u);
        result = gtmain_parts_parameter_value(minimum, maximum, level, count);
        context = r_u32(0x800BEA64u);
        w_u8(context + destination_offsets[index], result);
    }
    level = (sint16)r_u16(0x800929BCu) - 1;
    context = r_u32(0x800BEA64u);
    w_u8(context + 89u, (uint32)level);
    count = r_u8(record + 53u);
    for (index = 4u; index < 8u; ++index) {
        minimum = r_u8(record + source_offsets[index]);
        maximum = r_u8(record + source_offsets[index] + 1u);
        result = gtmain_parts_parameter_value(minimum, maximum, level, count);
        context = r_u32(0x800BEA64u);
        w_u8(context + destination_offsets[index], result);
    }
    return result;
}
