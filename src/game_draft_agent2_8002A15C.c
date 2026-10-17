#include "game_draft_registry.h"
#include "game_first2_registry.h"

static uint32 race_result_panel(uint32 address, uint32 ordering)
{
    /* TODO Bind unselected panel boundaries 8002955C, 800295A8 and 80029614 */
    return gt_draft_unresolved_call(address, ordering,
        0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static sint32 race_result_time_width(sint32 font, const char *text)
{
    /* TODO Bind native time string to the original width algorithm 8006C900 */
    gt_draft_failure("Native race result time width", 0x8006C900u);
}

/* GTMAIN.EXE 8002A15C */
uint32 gt_draft_8002A15C(void)
{
    uint32 label = r_s8(0x8008F893u) == 1 ? 0x80091290u : 0x80091298u;
    uint32 ordering = r_u32(0x800B6368u), time, text;
    sint32 x;
    char formatted[32];
    (void)race_result_panel(0x8002955Cu, ordering);
    (void)race_result_panel(0x800295A8u, ordering);
    (void)gtmain_text_font(2);
    (void)gtmain_text_color(0x808080u);
    x = r_s16(0x800911B8u);
    text = r_u32(0x800911BCu);
    (void)gtmain_font_text_draw(ordering, x, 64, text);
    time = r_u32(0x800BF52Cu);
    if (time <= 0x36EE7Fu) {
        sint32 width, centered;
        gtmain_time_string_native(formatted, sizeof(formatted), time);
        width = race_result_time_width(2, formatted);
        centered = (sint32)(320u - (uint32)width) >> 1;
        (void)gtmain_font_string_draw_native(0x800C0070u, ordering, centered, 144, formatted);
    }
    x = r_s16(0x800911C0u);
    text = r_u32(0x800911C4u);
    (void)gtmain_font_text_draw(ordering, x, 209, text);
    (void)gtmain_text_font(3);
    x = r_s16(label);
    text = r_u32(label + 4u);
    (void)gtmain_font_text_draw(ordering, x, 96, text);
    return race_result_panel(0x80029614u, ordering);
}
