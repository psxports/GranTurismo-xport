#include "game_draft_registry.h"
#include "game_first2_registry.h"

static sint32 name_draw_add(sint32 a, sint32 b)
{
    return (sint32)((uint32)a + (uint32)b);
}

static void name_slots_line(uint32 ordering, const sint16 corners[4], uint32 color)
{
    /* TODO Bind native corners to original8006F624 line service */
    (void)ordering; (void)corners; (void)color;
    gt_draft_failure("Native name slot line", 0x8006F624u);
}

static sint32 name_glyph_width(const char glyph[2])
{
    /* TODO Bind the actual local glyph to original font3 width */
    gt_draft_failure("Native name glyph width", 0x8006C900u);
}

static void name_glyph_draw(uint32 ordering, sint32 x, sint32 y, const char glyph[2])
{
    /* TODO Bind the actual local glyph to original current font draw */
    (void)ordering; (void)x; (void)y; (void)glyph;
    gt_draft_failure("Native name glyph draw", 0x8006CA10u);
}

/* GTMAIN.EXE 8003C804 */
uint32 gtmain_menu_name_slots_draw(uint32 ordering, uint32 request, sint32 x,
    sint32 y, sint32 intensity)
{
    uint32 count = r_u16(request + 10u), index = 0u;
    uint32 result = count * 16u - 4u;
    sint32 slot_x = (sint32)((uint32)x - result);
    sint16 line_y;
    uint32 reading = 1u;
    char glyph[2] = { 0, 0 };
    gtmain_text_font(3);
    if (!count) return result;
    line_y = (sint16)((uint32)y + 26u);
    do {
        sint16 corners[4];
        uint32 color;
        corners[0] = (sint16)slot_x; corners[1] = line_y;
        corners[2] = (sint16)((uint32)slot_x + 23u); corners[3] = line_y;
        if ((sint32)index == r_s8(request))
            color = (uint8)gtmain_menu_pulse_component(2, intensity);
        else color = gtmain_menu_gray((uint8)(intensity >> 2));
        name_slots_line(ordering, corners, color);
        if (reading) {
            glyph[0] = (char)r_u8(r_u32(request + 4u) + index);
            if (glyph[0]) {
                sint32 width = name_glyph_width(glyph);
                sint32 offset = (sint32)(24u - (uint32)width) >> 1;
                name_glyph_draw(ordering, name_draw_add(slot_x, offset), y, glyph);
            } else reading = 0u;
        }
        ++index;
        result = index < count;
        slot_x = name_draw_add(slot_x, 32);
    } while (index < count);
    return result;
}

/* GTMAIN.EXE 8003C990 */
sint32 gtmain_menu_name_keyboard_draw(uint32 ordering, uint32 request, sint32 x,
    sint32 y, sint32 intensity)
{
    sint32 width = gtmain_text_width(2, 0x800BC962u);
    sint16 label_x = (sint16)((uint32)x - (uint32)((width >> 1) - 128));
    sint16 label_y;
    sint32 selected_row;
    sint16 corners[4];
    uint32 row, column;
    gtmain_text_flags(0u);
    gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
    label_y = (sint16)((uint32)y + 141u);
    if (r_u16(request + 8u)) {
        /* TODO Translate the unselected wide name field8003C6C8 */
        gt_draft_failure("Wide name field draw", 0x8003C6C8u);
    } else gtmain_menu_name_slots_draw(ordering, request, name_draw_add(x, 128), y, intensity);
    selected_row = r_s8(request + 13u);
    if (selected_row < 6) {
        sint32 column_offset = (sint8)r_u8(request + 12u) * 16;
        sint32 top = name_draw_add(y, selected_row * 17 + 36);
        corners[0] = (sint16)((uint32)x + (uint32)column_offset - 4u);
        corners[1] = (sint16)top;
        column_offset = (sint8)r_u8(request + 12u) * 16;
        corners[3] = (sint16)((uint32)top + 18u);
        corners[2] = (sint16)((uint32)x + (uint32)column_offset + 14u);
    } else {
        corners[0] = (sint16)((uint32)(uint16)label_x - 2u);
        corners[2] = (sint16)((uint32)(uint16)label_x + (uint32)width + 1u);
        corners[1] = label_y;
        corners[3] = (sint16)((uint32)(uint16)label_y + 18u);
    }
    gtmain_packet_rectangle_native(ordering, corners,
        (uint8)gtmain_menu_pulse_component(2, intensity) | 0x03000000u);
    gtmain_packet_page(ordering, 544u);
    gtmain_text_font(2);
    for (row = 0u; row < 6u; ++row) {
        uint32 columns = row == 5u ? 15u : 16u;
        sint16 row_y = (sint16)((uint32)y + 36u + row * 17u);
        for (column = 0u; column < columns; ++column) {
            sint32 glyph_width, glyph_x;
            w_u8(0x80092240u, gtmain_menu_glyph_index((sint8)column, (sint8)row));
            glyph_width = gtmain_text_width(2, 0x80092240u);
            glyph_x = (sint32)((uint32)x + 4u + column * 16u - (uint32)(glyph_width >> 1));
            gtmain_font_text_draw(ordering, glyph_x, row_y, 0x80092240u);
        }
    }
    return gtmain_font_text_draw(ordering, label_x, label_y, 0x800BC962u);
}

/* Preserve the existing legacy callers until their ABI is translated */
uint32 gt_draft_8003C990(uint32 a0, uint32 a1, uint32 a2, uint32 a3,
    uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    (void)a5; (void)a6; (void)a7;
    return (uint32)gtmain_menu_name_keyboard_draw(a0, a1, (sint32)a2, (sint32)a3, (sint32)a4);
}

/* GTMAIN.EXE 800339E4 */
uint32 gtmain_menu_result_name_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    gtmain_menu_name_keyboard_draw(ordering, 0x8009196Cu,
        name_draw_add(x, 32), name_draw_add(y, 56), intensity);
    return 0u;
}
