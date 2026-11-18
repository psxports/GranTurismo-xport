#include "game_first2_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

uint32 gtmain_menu_settings_row_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 entry, sint32 row)
{
    uint32 descriptor, resource, label, kind, staged, corners, text, first, second, color;
    uint32 arguments[1], product, units, result;
    sint32 palette, length, number, alternate, index, high;
    sint64 wide;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800446DC, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x130, 0x800446DC);
    descriptor = 0x800BB850 + (uint32)entry * 386;
    kind = r_u16(0x800B6670 + (uint32)entry * 76);
    resource = gtmain_resource_name(kind);
    kind = r_u8(descriptor + 138);
    label = r_u32(0x80092D5C + kind * 4);
    staged = frame.address + 0xF0;
    corners = frame.address + 0xE8;
    text = frame.address + 0xC8;
    w_u16(staged, (uint32)x + 24);
    w_u16(staged + 2, (uint32)y + 25);
    w_u16(staged + 4, (uint32)x + 343);
    w_u16(staged + 6, (uint32)y + 25);
    w_u32(frame.address + 0x100, label);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(corners, first);
    w_u32(corners + 4, second);
    w_u32(frame.address + 0xF8, resource);
    color = gtmain_menu_palette_color(5, (uint32)intensity << 1);
    gtmain_packet_outline(ordering, corners, color | 0x02000000);
    gtmain_text_flags(2);
    alternate = r_s8(0x800BEA24);
    w_u8(text, (uint32)row + 49);
    w_u8(text + 1, 46);
    w_u8(text + 2, 0);
    palette = 6;
    if (alternate) {
        index = row < 4 ? row : 3;
        kind = r_u8(0x80091B40) & 3;
        palette = r_s8(0x80091B28 + (uint32)index * 4 + kind);
    }
    gtmain_text_color(gtmain_menu_palette_color(palette, intensity));
    gtmain_menu_text_cells_draw(ordering, x, (uint32)y + 4, text, 8, 1);
    gtmain_text_color(gtmain_menu_gray(intensity));
    if (!entry)
        gtmain_text_color(gtmain_menu_palette_color(7, intensity));
    resource = r_u32(frame.address + 0xF8);
    gtmain_menu_text_draw(ordering, (uint32)x + 24, (uint32)y - 1, resource, 0);
    arguments[0] = r_u16(0x800B6840 + (uint32)entry * 2);
    gtmain_string_format(text, 0x800BCF24, arguments, 1);
    length = gtmain_string_length(text);
    gtmain_menu_text_cells_draw(ordering, (uint32)x - ((uint32)length * 8 - 266),
        (uint32)y + 4, text, 8, 2);
    gtmain_text_flags(1);
    gtmain_text_color(gtmain_menu_palette_color(0, intensity));
    label = r_u32(frame.address + 0x100);
    gtmain_menu_text_cells_draw(ordering, (uint32)x + 32, (uint32)y + 16, label, 8, 1);
    number = gtmain_race_specification_summary(descriptor, frame.address + 0x18, frame.address + 0x58);
    gtmain_menu_number_scale(text, number, 0x800BCF1E);
    gtmain_menu_text_cells_draw(ordering, (uint32)x + 88, (uint32)y + 16, text, 8, 1);
    units = r_u8(0x800C4C3D);
    number = r_s16(descriptor + 90);
    if (!units) {
        product = (uint32)number * 22046;
        wide = (sint64)(sint32)product * 0x68DB8BAD;
        high = (uint64)wide >> 32;
        number = (uint32)(high >> 12) - (uint32)((sint32)product >> 31);
    }
    gtmain_menu_number_scale(text, number, 0x800BCF21);
    result = gtmain_menu_text_cells_draw(ordering, (uint32)x + 152, (uint32)y + 16, text, 8, 1);
    gt_local_stack_pop(frame);
    return result;
}
