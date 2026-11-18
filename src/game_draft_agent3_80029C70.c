#include "game_draft_registry.h"
#include "game_first2_registry.h"

uint32 gt_draft_80029C70(uint32 ordering)
{
    FUNCTION_MARKER(0x80029C70, "GTMAIN.EXE");
    gtmain_menu_heading_draw(ordering);
    gtmain_menu_status_draw(ordering);
    return gtmain_menu_result_footer_draw(ordering);
}
