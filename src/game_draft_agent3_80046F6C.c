#include "game_first2_registry.h"

uint32 gtmain_menu_saved_entry_delete(uint32 owner, sint32 index)
{
    uint32 entry = owner + 512u;
    sint32 current;
    for (current = 0; current < 120; ++current, entry += 116u)
    {
        if (current == index) w_u16(entry + 114u, 0);
        if (index < current)
        {
            uint32 offset, destination = entry - 116u;
            for (offset = 0; offset != 112u; offset += 16u)
            {
                uint32 first = r_u32(entry + offset);
                uint32 second = r_u32(entry + offset + 4u);
                uint32 third = r_u32(entry + offset + 8u);
                uint32 fourth = r_u32(entry + offset + 12u);
                w_u32(destination + offset, first);
                w_u32(destination + offset + 4u, second);
                w_u32(destination + offset + 8u, third);
                w_u32(destination + offset + 12u, fourth);
            }
            w_u32(destination + 112u, r_u32(entry + 112u));
            w_u16(entry + 114u, 0);
        }
    }
    return 0;
}
