#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include "game_local_stack.h"

uint32 gt_draft_8002A15C(void)
{
    uint32 label, ordering, time, text, result;
    sint32 x;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8002A15C, "GTMAIN.EXE");
    label = r_s8(0x8008F893) == 1 ? 0x80091290 : 0x80091298;
    ordering = r_u32(0x800B6368);
    frame = gt_local_stack_push(0x40, 0x8002A15C);
    gtmain_menu_heading_draw(ordering);
    gtmain_menu_status_draw(ordering);
    gtmain_text_font(2);
    gtmain_text_color(0x808080);
    x = r_s16(0x800911B8);
    text = r_u32(0x800911BC);
    gtmain_font_text_draw(ordering, x, 64, text);
    time = r_u32(0x800BF52C);
    if (time <= 0x36EE7F) {
        uint32 width, centered;
        gtmain_time_string(frame.address + 0x10, time);
        width = gtmain_text_width(2, frame.address + 0x10);
        centered = (sint32)(320 - width) >> 1;
        gtmain_font_text_draw(ordering, centered, 144, frame.address + 0x10);
    }
    x = r_s16(0x800911C0);
    text = r_u32(0x800911C4);
    gtmain_font_text_draw(ordering, x, 209, text);
    gtmain_text_font(3);
    x = r_s16(label);
    text = r_u32(label + 4);
    gtmain_font_text_draw(ordering, x, 96, text);
    result = gtmain_menu_result_footer_draw(ordering);
    gt_local_stack_pop(frame);
    return result;
}
