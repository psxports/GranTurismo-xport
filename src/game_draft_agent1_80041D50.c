#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 80041D50 */
/* Original: GTMAIN.EXE:sub_80041D50. */ uint32 gt_draft_80041D50(sint32 category)
{
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 context = r_u32(0x800BEA64u);
    uint32 part = r_u16(context + offset), level = 0u;
    if (part) {
        uint32 record = gtmain_parts_table(32) + 20u * part;
        uint32 minimum = r_u8(record - 18u);
        uint32 source = r_u32(0x800BEA60u);
        uint32 maximum = r_u8(record - 17u);
        uint32 count = r_u8(record - 19u);
        uint32 value = r_u8(source + 123u), flags;
        sint32 index = gtmain_menu_range_index((sint32)minimum, (sint32)maximum,
            (sint32)value, (sint32)count);
        flags = r_u8(0x800927FCu);
        level = (uint32)index + 1u;
        w_u8(0x800927FCu, flags & 31u);
    }
    w_u16(0x800927CAu, level);
    return 1u;
}
