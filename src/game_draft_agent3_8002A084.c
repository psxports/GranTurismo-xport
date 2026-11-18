#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gt_draft_8002A084(void)
{
    uint32 record, request, ordering, result;
    FUNCTION_MARKER(0x8002A084, "GTMAIN.EXE");
    record = gtmain_race_mode_record();
    request = r_u32(record + 16);
    ordering = r_u32(0x800B6368);
    gtmain_menu_request_draw(ordering, 0x800BD82B, 0x800BD959, request);
    result = gtmain_menu_scroll_limit(request);
    w_u8(0x8008F890, result);
    return result;
}
