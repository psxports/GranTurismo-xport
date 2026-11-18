#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

static sint32 name_draw_add(sint32 a, sint32 b)
{
    return (sint32)((uint32)a + (uint32)b);
}

uint32 gtmain_menu_name_slots_draw(uint32 ordering, uint32 request, sint32 x,
    sint32 y, sint32 intensity)
{
    uint32 count, index = 0, result, slot_x, line_y, reading = 1;
    uint32 glyph, staged, corners, first_word, second_word, color, value;
    sint32 width, offset;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003C804, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x60, 0x8003C804);
    glyph = frame.address + 0x18;
    staged = frame.address + 0x20;
    corners = frame.address + 0x10;
    w_u32(frame.address + 0x28, y);
    w_u8(glyph, 0);
    w_u8(glyph + 1, 0);
    count = r_u16(request + 10);
    gtmain_text_flags(3);
    result = count * 16 - 4;
    slot_x = (uint32)x - result;
    if (count) {
        line_y = r_u32(frame.address + 0x28) + 26;
        do {
            w_u16(staged, slot_x);
            w_u16(staged + 2, line_y);
            w_u16(staged + 4, slot_x + 23);
            w_u16(staged + 6, line_y);
            first_word = r_u32(staged);
            second_word = r_u32(staged + 4);
            w_u32(corners, first_word);
            w_u32(corners + 4, second_word);
            if ((sint32)index == r_s8(request))
                color = (uint8)gtmain_menu_pulse_component(2, intensity);
            else color = gtmain_menu_gray(intensity >> 2);
            gtmain_packet_outline(ordering, corners, color);
            if (reading) {
                value = r_u8(r_u32(request + 4) + index);
                w_u8(glyph, value);
                if (value) {
                    width = gtmain_text_width(3, glyph);
                    offset = (sint32)(24 - (uint32)width) >> 1;
                    y = r_s32(frame.address + 0x28);
                    gtmain_font_text_draw(ordering, slot_x + (uint32)offset, y, glyph);
                } else reading = 0;
            }
            ++index;
            result = index < count;
            slot_x += 32;
        } while (index < count);
    }
    gt_local_stack_pop(frame);
    return result;
}

sint32 gtmain_menu_name_keyboard_draw(uint32 ordering, uint32 request, sint32 x,
    sint32 y, sint32 intensity)
{
    sint32 width, selected_row, column_offset, glyph_width, glyph_x, result;
    sint16 row_y, label_x, label_y;
    uint32 staged, corners, first_word, second_word, top, color, row, column;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003C990, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x78, 0x8003C990);
    staged = frame.address + 0x20;
    corners = frame.address + 0x18;
    w_u32(frame.address + 0x28, ordering);
    w_u32(frame.address + 0x30, y);
    width = gtmain_text_width(2, 0x800BC962);
    w_u16(frame.address + 0x38, (uint32)x - ((uint32)(width >> 1) - 128));
    gtmain_text_font(0);
    gtmain_text_color(gtmain_menu_gray(intensity >> 1));
    y = r_s32(frame.address + 0x30);
    first_word = r_u16(request + 8);
    w_u16(frame.address + 0x40, (uint32)y + 141);
    ordering = r_u32(frame.address + 0x28);
    y = r_s32(frame.address + 0x30);
    if (first_word)
        gtmain_menu_name_field_draw(ordering, request, (uint32)x + 128, y, intensity);
    else gtmain_menu_name_slots_draw(ordering, request, (uint32)x + 128, y, intensity);
    selected_row = r_s8(request + 13);
    if (selected_row < 6) {
        y = r_s32(frame.address + 0x30);
        column_offset = (sint8)r_u8(request + 12) * 16;
        top = (uint32)y + (uint32)(selected_row * 17) + 36;
        w_u16(staged + 2, top);
        w_u16(staged, (uint32)x + (uint32)column_offset - 4);
        column_offset = (sint8)r_u8(request + 12) * 16;
        w_u16(staged + 6, top + 18);
        w_u16(staged + 4, (uint32)x + (uint32)column_offset + 14);
    } else {
        first_word = r_u16(frame.address + 0x38);
        second_word = first_word + (uint32)width + 1;
        first_word -= 2;
        top = r_u16(frame.address + 0x40);
        w_u16(staged, first_word);
        w_u16(staged + 4, second_word);
        w_u16(staged + 2, top);
        w_u16(staged + 6, top + 18);
    }
    first_word = r_u32(staged);
    second_word = r_u32(staged + 4);
    w_u32(corners, first_word);
    w_u32(corners + 4, second_word);
    color = (uint8)gtmain_menu_pulse_component(2, intensity) | 0x03000000;
    ordering = r_u32(frame.address + 0x28);
    gtmain_packet_rectangle(ordering, corners, color);
    ordering = r_u32(frame.address + 0x28);
    gtmain_packet_page(ordering, 544);
    gtmain_text_flags(2);
    for (row = 0; row < 6; ++row) {
        uint32 columns = row == 5 ? 15 : 16;
        y = r_s32(frame.address + 0x30);
        row_y = (sint16)((uint32)y + 36 + row * 17);
        for (column = 0; column < columns; ++column) {
            w_u8(0x80092240, gtmain_menu_glyph_index((sint8)column, (sint8)row));
            glyph_width = gtmain_text_width(2, 0x80092240);
            glyph_x = (sint32)((uint32)x + 4 + column * 16 - (uint32)(glyph_width >> 1));
            ordering = r_u32(frame.address + 0x28);
            gtmain_font_text_draw(ordering, glyph_x, row_y, 0x80092240);
        }
    }
    label_x = r_s16(frame.address + 0x38);
    ordering = r_u32(frame.address + 0x28);
    label_y = r_s16(frame.address + 0x40);
    result = gtmain_font_text_draw(ordering, label_x, label_y, 0x800BC962);
    gt_local_stack_pop(frame);
    return result;
}

/* Preserve the existing legacy callers until their ABI is translated */
uint32 gt_draft_8003C990(uint32 a0, uint32 a1, uint32 a2, uint32 a3,
    uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    (void)a5; (void)a6; (void)a7;
    return (uint32)gtmain_menu_name_keyboard_draw(a0, a1, (sint32)a2, (sint32)a3, (sint32)a4);
}

/* GTMAIN.EXE 800339E4 */
/* Original: GTMAIN.EXE:sub_800339E4. */ uint32 gtmain_menu_result_name_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    gtmain_menu_name_keyboard_draw(ordering, 0x8009196Cu,
        name_draw_add(x, 32), name_draw_add(y, 56), intensity);
    return 0u;
}
