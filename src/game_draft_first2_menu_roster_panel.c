#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

uint32 gtmain_menu_roster_panel_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    uint32 table, arrow_color, entry, formatted, staged, corners, first, second, shade;
    uint32 arguments[1], selected, row = 0, unsigned_count, width, highlight, active;
    uint32 label_x, label_y;
    sint32 count, selection, header_width, half = intensity >> 1, offset = 0, pulse;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80039030, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x1A8, 0x80039030);
    formatted = frame.address + 0x20;
    staged = frame.address + 0x128;
    corners = frame.address + 0x120;
    table = gt_draft_8002A4DC();
    w_u32(frame.address + 0x138, table);
    arrow_color = gtmain_menu_palette_color(0, intensity);
    w_u32(frame.address + 0x148, arrow_color);
    gtmain_menu_machine_caption_draw(ordering, x, y, intensity);
    gtmain_text_flags(2);
    gtmain_text_font(0);
    gtmain_text_color(gtmain_menu_palette_color(6, half));
    selected = r_u8(0x800CB7F9);
    arguments[0] = selected + 1;
    gtmain_string_format(formatted, 0x800BCCA2, arguments, 1);
    table = r_u32(frame.address + 0x138);
    entry = table + selected * 4;
    header_width = gtmain_menu_caption_width(r_u32(entry));
    gtmain_menu_caption_plain_draw(ordering, (uint32)x - ((uint32)(header_width >> 1) - 160),
        (uint32)y + 40, r_u32(entry));
    w_u16(staged, x);
    w_u16(staged + 2, (uint32)y + 42);
    w_u16(staged + 4, (uint32)x + 319);
    w_u16(staged + 6, (uint32)y + 57);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(corners, first);
    w_u32(corners + 4, second);
    shade = (intensity >> 3) & 255;
    gtmain_packet_rectangle(ordering, corners, shade | (shade << 8) | (((intensity >> 2) & 255) << 16));
    count = r_s8(0x801F2FE9);
    unsigned_count = r_u8(0x801F2FE9);
    if (count > 0) {
        do {
            gtmain_menu_roster_slot_draw(ordering,
                (uint32)offset - ((uint32)(sint32)(sint8)unsigned_count * 12 - 168),
                (uint32)y + 62, intensity, row);
            ++row;
            count = r_s8(0x801F2FE9);
            unsigned_count = r_u8(0x801F2FE9);
            offset += 24;
        } while ((sint32)row < count);
    }
    gtmain_text_flags(2);
    w_u32(frame.address + 0x158, half);
    highlight = r_s8(0x8009207D) == 2;
    selection = r_s8(0x801F2FE7);
    gtmain_text_color(gtmain_menu_gray(half));
    width = gtmain_menu_text_width(2, 0x800BCC8B, 0);
    label_x = (uint32)x + 72;
    label_y = (uint32)y + 132;
    w_u32(frame.address + 0x160, label_x);
    w_u32(frame.address + 0x168, label_y);
    gtmain_menu_text_draw(ordering, label_x, label_y, 0x800BCC8B, 0);
    active = r_s8(0x80091B41) != 0;
    w_u32(frame.address + 0x140, highlight);
    if (active && highlight) {
        arrow_color = r_u32(frame.address + 0x148);
        gtmain_packet_triangle_left(ordering, (uint32)x + width + 72 - 9, (uint32)y + 140, arrow_color);
        arrow_color = r_u32(frame.address + 0x148);
        gtmain_packet_triangle_right(ordering, (uint32)x + width + 72 + 19, (uint32)y + 140, arrow_color);
    }
    arguments[0] = (uint32)selection + 1;
    gtmain_string_format(formatted, 0x800BCC9F, arguments, 1);
    half = r_s32(frame.address + 0x158);
    gtmain_text_color(gtmain_menu_gray(half));
    highlight = r_u32(frame.address + 0x140);
    if (highlight) {
        half = r_s32(frame.address + 0x158);
        pulse = gtmain_menu_pulse_component(0, half);
        gtmain_text_color(gtmain_menu_gray(pulse));
    }
    label_y = r_u32(frame.address + 0x168);
    gtmain_menu_text_draw(ordering, (uint32)x + width + 72, label_y, formatted, 0);
    gtmain_text_flags(1);
    half = r_s32(frame.address + 0x158);
    gtmain_text_color(gtmain_menu_palette_color(6, half));
    table = r_u32(frame.address + 0x138);
    entry = table + (uint32)selection * 4;
    gtmain_menu_caption_width(r_u32(entry));
    gtmain_menu_caption_plain_draw(ordering, (uint32)x + 80, (uint32)y + 149, r_u32(entry));
    arrow_color = gtmain_menu_palette_color(4, intensity);
    w_u32(frame.address + 0x148, arrow_color);
    label_x = r_u16(frame.address + 0x160);
    w_u16(staged + 2, (uint32)y + 162);
    w_u16(staged + 4, (uint32)x + 391);
    w_u16(staged + 6, (uint32)y + 162);
    w_u16(staged, label_x);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(corners, first);
    w_u32(corners + 4, second);
    arrow_color = r_u32(frame.address + 0x148);
    gtmain_packet_rectangle(ordering + 4, corners, arrow_color);
    w_u16(staged, (uint32)x + 52);
    w_u16(staged + 2, (uint32)y + 142);
    w_u16(staged + 4, (uint32)x + 62);
    w_u16(staged + 6, (uint32)y + 152);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(corners, first);
    w_u32(corners + 4, second);
    arrow_color = r_u32(frame.address + 0x148);
    gtmain_packet_rectangle(ordering + 4, corners, arrow_color);
    gtmain_menu_list_extent(0x8009207C);
    label_x = r_u32(frame.address + 0x160);
    gtmain_menu_descriptor_draw(ordering, 0x8009207C, label_x, (uint32)y + 96, intensity, 0, 0, 0);
    half = r_s32(frame.address + 0x158);
    gtmain_menu_backdrop_draw(ordering + 8000, x, y, half, half, half);
    gt_local_stack_pop(frame);
    return 0;
}
