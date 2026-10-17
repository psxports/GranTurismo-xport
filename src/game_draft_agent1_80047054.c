#include "game_first2_registry.h"

uint32 gtmain_menu_saved_entry_commit(uint32 owner, sint32 index)
{
    uint32 entry = owner + 512u + 116u * (uint32)index;
    uint32 source = r_u32(0x800BEA64u), destination = entry + 18u;
    uint32 offset;
    for (offset = 0; offset != 96u; offset += 16u)
    {
        uint32 first = r_u32(source + offset);
        uint32 second = r_u32(source + offset + 4u);
        uint32 third = r_u32(source + offset + 8u);
        uint32 fourth = r_u32(source + offset + 12u);
        w_u32(destination + offset, first);
        w_u32(destination + offset + 4u, second);
        w_u32(destination + offset + 8u, third);
        w_u32(destination + offset + 12u, fourth);
    }
    gtmain_string_copy(entry, 0x800B5394u);
    w_u16(entry + 114u, 1);
    return 1;
}
