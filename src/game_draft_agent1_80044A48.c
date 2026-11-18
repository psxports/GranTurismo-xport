#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Original: GTMAIN.EXE:sub_80044A48. */ uint32 gtmain_menu_settings_rows_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    sint32 row, offset = 48;
    for (row = 0; row < 6; ++row)
    {
        sint32 entry = row;
        if ((sint8)r_u8(0x800BEA24u))
        {
            uint32 record = 0x800B6670u;
            entry = 0;
            do
            {
                if (r_u8(record + 3u) == (uint32)row) break;
                ++entry;
                record += 76u;
            } while (entry < 6);
        }
        gtmain_menu_settings_row_draw(ordering, (sint32)((uint32)x + 32u),
            (sint32)((uint32)y + (uint32)offset), intensity >> 1, entry, row);
        offset += 26;
    }
    return 0;
}
