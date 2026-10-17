#include "game_first2_registry.h"

/* GTMAIN.EXE:8002332C */
uint32 gt_draft_8002332C(void)
{
    uint32 mode = r_u16(0x800B63D6u);
    uint32 ordering = r_u32(0x800B6368u);
    uint32 indication;
    switch (mode) {
    case 6u:
    case 9u:
        gtmain_race_label_font3_draw(ordering, 68, 0x00808080u, 1u);
        break;
    case 7u:
    case 8u:
        gtmain_race_results_panel_draw(ordering, 0u, 1u);
        return 1u;
    case 10u:
        gtmain_race_results_panel_draw(ordering, 0u, 0u);
        return 1u;
    case 11u:
        gtmain_race_results_panel_draw(ordering, 1u, 0u);
        return 1u;
    case 12u:
        gtmain_race_results_panel_draw(ordering, 2u, 0u);
        return 1u;
    case 13u:
        gtmain_race_results_panel_draw(ordering, 2u, 1u);
        return 1u;
    default:
        break;
    }
    indication = r_u8(0x800CB7F6u);
    gtmain_race_labels_draw(ordering, 68, indication, indication == 2u);
    gtmain_race_times_draw(ordering);
    gtmain_race_record_label_draw();
    gtmain_race_driver_hud_draw(ordering);
    indication = r_u8(0x800CB7F8u);
    if (indication >= 19u && indication < 22u)
        return gtmain_race_meter_panel_draw(ordering, 216u, 192u, 0x800B6850u);
    return indication < 19u;
}
