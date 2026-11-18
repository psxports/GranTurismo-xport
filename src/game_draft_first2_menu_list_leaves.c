#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_8003B8EC. */ sint32 gtmain_menu_list_select(uint32 list, sint32 selection)
{
    uint8 previous = r_u8(list + 1u);
    w_u8(list + 12u, previous);
    if (selection < 0) selection = 0;
    sint32 count = (sint8)r_u8(list);
    sint32 in_range = selection < count;
    if (!in_range) selection = count - 1;
    w_u8(list + 1u, (uint8)selection);
    return in_range;
}

/* Original: GTMAIN.EXE:sub_8003B928. */ sint32 gtmain_menu_list_extent(uint32 list)
{
    sint32 count = (sint8)r_u8(list + 2u);
    sint32 height = (sint8)r_u8(list + 9u);
    sint32 spacing = (sint8)r_u8(list + 10u);
    return count * height + (count - 1) * spacing;
}
