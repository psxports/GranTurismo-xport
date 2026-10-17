#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void mode_request_draw(uint32 ordering, uint32 first_text, uint32 second_text, uint32 request)
{
    /* TODO Bind unselected four-argument mode request renderer 80029740 */
    (void)gt_draft_unresolved_call(0x80029740u, ordering, first_text,
        second_text, request, 0u, 0u, 0u, 0u);
}

static uint32 mode_request_input(uint32 request)
{
    /* TODO Bind unselected one-argument mode request handler 800298B4 */
    return gt_draft_unresolved_call(0x800298B4u, request,
        0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 8002A0D4 */
uint32 gt_draft_8002A0D4(void)
{
    uint32 record = gtmain_race_mode_record();
    uint32 request = r_u32(record + 20u);
    uint32 ordering = r_u32(0x800B6368u), result;
    mode_request_draw(ordering, 0x800BD82Bu, 0x800BD959u, request);
    result = mode_request_input(request);
    w_u8(0x8008F890u, result);
    return result;
}
