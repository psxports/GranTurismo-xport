#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gtmain_race_standard_hud_draw(void)
{
    sint32 mode = (sint16)r_u16(0x800B63D6u);
    uint32 ordering = r_u32(0x800B6368u);
    if (mode == 6 || mode == 7)
        gtmain_race_label_font3_draw(ordering, 68, 0x00808080u, 1u);
    else if (mode >= 8 && mode < 10)
    {
        gtmain_race_time_roster_draw(ordering, 68,
            (sint32)((uint32)r_u16(0x800B63DAu) << 16) >> 19);
        gtmain_race_label_panel_draw(ordering, 15u, 112u);
        gtmain_race_screen_rectangle(ordering);
        return 1u;
    }
    gtmain_race_labels_draw(ordering, 68, r_u8(0x800CB7F6u), 0u);
    gtmain_race_times_draw(ordering);
    gtmain_race_record_label_draw();
    return gtmain_race_driver_hud_draw(ordering);
}
