#include "game_first2_registry.h"

static sint32 progress_format(char output[256], uint32 format, const uint32 *arguments, uint32 argument_count)
{
    /* TODO Bind native destination formatting with original argument words */
    gt_draft_failure("Native progress formatter", 0x8007C014u);
}

static sint32 progress_width(sint32 font, const char *text)
{
    /* TODO Bind native string storage to original proportional width */
    gt_draft_failure("Native progress string width", 0x8006C900u);
}

static uint32 progress_outline(uint32 ordering, const sint16 *corners, uint32 color)
{
    return gtmain_packet_outline_native(ordering, corners, color);
}

static sint32 progress_span(uint32 value, uint32 total)
{
    return (sint32)(value * 288u) / (sint32)total;
}

/* GTMAIN.EXE:80048660 */
uint32 gtmain_menu_progress_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 selection, uint32 show_pending)
{
    uint32 state = r_u32(0x800BEA34u);
    uint32 current = r_u16(state + 2u), total = r_u16(state + 4u);
    uint32 arguments[3], color;
    sint32 full_span, count, width;
    sint16 corners[4];
    char formatted[256];
    if (!total) gt_draft_failure("GTMAIN original progress division break7", 0x800486B8u);
    full_span = progress_span(current, total);
    count = (sint32)r_u8(state);
    gtmain_text_flags(0);
    color = gtmain_menu_palette_color(6,
        (sint32)((uint32)(intensity >> 1) - (uint32)(intensity >> 3)));
    (void)gtmain_text_color(color);
    (void)gtmain_text_font(1);
    if (show_pending) {
        arguments[0] = (uint32)r_s16(0x800B53B0u);
        (void)progress_format(formatted, 0x800BD697u, arguments, 1);
        width = progress_width(1, formatted);
        (void)gtmain_font_string_draw_native(0x800C0070u, ordering,
            (sint32)((uint32)x - ((uint32)width - 136u)), y, formatted);
    }
    arguments[0] = (uint32)count;
    arguments[1] = total - current;
    arguments[2] = total;
    (void)progress_format(formatted, 0x800BD6B1u, arguments, 3);
    width = progress_width(1, formatted);
    (void)gtmain_font_string_draw_native(0x800C0070u, ordering,
        (sint32)((uint32)x - (uint32)(width >> 1)), (sint32)((uint32)y + 9u), formatted);
    if (show_pending) {
        uint32 delta = current;
        uint32 pending;
        if (selection < count)
            delta -= r_u16(state + (uint32)selection * 52u + 42u);
        pending = delta + (uint32)r_s16(0x800B53B0u);
        if ((sint32)total >= (sint32)pending) {
            sint32 start = progress_span(delta, total);
            sint32 end = progress_span(pending, total);
            if (count >= selection) {
                corners[0] = (sint16)((uint32)x + (uint32)start - 144u);
                corners[1] = (sint16)((uint32)y + 19u);
                corners[2] = (sint16)((uint32)x + (uint32)end - 145u);
                corners[3] = (sint16)((uint32)y + 20u);
                color = (((uint32)intensity << 7) & 0xFF00u)
                    | ((uint32)(uint8)intensity << 16) | 0x02000000u;
                (void)gtmain_packet_rectangle_native(ordering, corners, color);
            }
        }
    }
    (void)gtmain_packet_page(ordering, 0x220);
    if (selection < count) {
        sint32 end = progress_span(current, total);
        uint32 delta = current - r_u16(state + (uint32)selection * 52u + 42u);
        sint32 start = progress_span(delta, total);
        if (count >= selection) {
            corners[0] = (sint16)((uint32)x + (uint32)start - 144u);
            corners[1] = (sint16)((uint32)y + 19u);
            corners[2] = (sint16)((uint32)x + (uint32)end - 145u);
            corners[3] = (sint16)((uint32)y + 20u);
            (void)gtmain_packet_rectangle_native(ordering, corners, 0x02000000u);
            (void)gtmain_packet_page(ordering, 0x200);
        }
    }
    if (full_span > 0) {
        corners[0] = (sint16)((uint32)x - 144u);
        corners[1] = (sint16)((uint32)y + 19u);
        corners[2] = (sint16)((uint32)x + (uint32)full_span - 145u);
        corners[3] = (sint16)((uint32)y + 20u);
        color = gtmain_menu_palette_color(4, intensity);
        (void)gtmain_packet_rectangle_native(ordering, corners, color);
    }
    corners[0] = (sint16)((uint32)x - 145u);
    corners[1] = (sint16)((uint32)y + 18u);
    corners[2] = (sint16)((uint32)x + 144u);
    corners[3] = (sint16)((uint32)y + 21u);
    color = gtmain_menu_gray((uint8)(intensity >> 2));
    (void)progress_outline(ordering, corners, color);
    color = gtmain_menu_gray(1);
    (void)gtmain_packet_rectangle_native(ordering, corners, color);
    return gtmain_packet_page(ordering, 0x220);
}
