#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gtmain_race_standard_hud_draw(void)
{
    uint32 ordering = r_u32(0x800B6368u);
    sint32 mode = (sint16)r_u16(0x800B63D6u);
    if (mode == 6 || mode == 7)
        gt_draft_8003011C(ordering, 68u, 0x00808080u, 1u, 0u, 0u, 0u, 0u);
    else if (mode >= 8 && mode < 10)
    {
        /* TODO Bind the selected 800214C8 native text owner */
        gt_draft_unresolved_call(0x800214C8u, ordering, 68u,
            (uint32)((sint32)((uint32)r_u16(0x800B63DAu) << 16) >> 19), 0u, 0u, 0u, 0u, 0u);
        gt_draft_800308D0(ordering, 15u, 112u, 0u, 0u, 0u, 0u, 0u);
        gtmain_race_screen_rectangle(ordering);
        return 1u;
    }
    gtmain_race_labels_draw(ordering, 68, r_u8(0x800CB7F6u), 0u);
    gtmain_race_times_draw(ordering);
    gtmain_race_record_label_draw();
    return gtmain_race_driver_hud_draw(ordering);
}
