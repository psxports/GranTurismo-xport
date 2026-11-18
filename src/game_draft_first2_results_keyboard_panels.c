#include "game_first2_registry.h"

/* GTMAIN.EXE:80023280 */
/* Original: GTMAIN.EXE:sub_80023280. */ uint32 gtmain_race_results_panel_draw(uint32 ordering, uint32 mode, uint32 show_hints)
{
    sint32 count;
    FUNCTION_MARKER(0x80023280, "GTMAIN.EXE");
    count = (sint32)((uint32)r_u16(0x800B63DAu) << 16u) >> 19;
    if (mode) gtmain_event_points_table_draw(ordering, 64, count, mode - 1u);
    else gtmain_race_time_roster_draw(ordering, 64, count);
    gtmain_race_label_panel_draw(ordering, 15u, 112u);
    if (show_hints && count >= 6)
        gtmain_race_result_hints_draw(ordering, 0x800BD9EDu, 0x800BD989u);
    return gtmain_race_screen_rectangle(ordering);
}

/* GTMAIN.EXE:800484C0 */
/* Original: GTMAIN.EXE:sub_800484C0. */ sint32 gtmain_menu_name_keyboard_panel_draw(uint32 ordering, sint32 x, sint32 y,
    sint32 intensity)
{
    return gtmain_menu_name_keyboard_draw(ordering, 0x80092EACu,
        (sint32)((uint32)x + 32u), (sint32)((uint32)y + 48u), intensity);
}
