#include "game_first2_registry.h"

/* GTMAIN.EXE:80033CEC */
uint32 gtmain_menu_result_mode0_run(uint8 flags, sint16 index, uint32 first, uint32 second,
    uint32 metric, sint16 threshold, uint32 extra, uint8 panel)
{
    /* TODO Existing result panel retains menu setup dependencies */
    gtmain_menu_draw_callback_set(0x8009197Cu);
    return gtmain_menu_result_panel_run(flags, index, first, second,
        metric, threshold, extra, 0u, 12u, panel);
}
