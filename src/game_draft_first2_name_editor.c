#include "game_first2_registry.h"

/* GTMAIN.EXE 8003C338 */
/* Original: GTMAIN.EXE:sub_8003C338. */ uint32 gtmain_menu_name_text_set(uint32 request, uint32 text)
{
    uint32 length;
    gtmain_string_copy(r_u32(request + 4u), text);
    length = gtmain_string_length(text);
    w_u8(request + 1u, (uint8)length);
    w_u8(request, (uint8)length);
    return length;
}

/* GTMAIN.EXE 8003C3B0 */
/* Original: GTMAIN.EXE:sub_8003C3B0. */ sint32 gtmain_menu_name_edit(uint32 request, uint32 controls)
{
    sint32 result = -1, columns = 16, row, cursor, length;
    uint32 buttons = r_u16(controls + 4u);
    uint32 repeat = r_u16(controls + 8u);
    uint32 text, value;
    buttons |= repeat;
    if (r_u16(request + 8u)) {
        value = (uint32)gtmain_menu_selection((uint16)buttons,
            (sint8)r_u8(request), (sint8)r_u8(request + 1u) + 1,
            0x400u, 0x800u, 0, 0u);
        w_u8(request, (uint8)value);
    }
    row = (sint8)r_u8(request + 13u);
    if (row == 5) columns = 15;
    if (row != 6) {
        value = (uint32)gtmain_menu_selection((uint16)buttons,
            (sint8)r_u8(request + 12u), columns, 0x80u, 0x20u, 0, 1u);
        w_u8(request + 12u, (uint8)value);
    }
    value = (uint32)gtmain_menu_selection((uint16)buttons,
        (sint8)r_u8(request + 13u), 7, 0x10u, 0x40u, 0, 1u);
    w_u8(request + 13u, (uint8)value);
    if (r_u16(request + 12u) == 0x050Fu) w_u8(request + 12u, 14u);
    buttons = r_u16(controls + 4u);
    length = (sint8)r_u8(request + 1u);
    cursor = (sint8)r_u8(request);
    text = r_u32(request + 4u);
    if (buttons & 8u) return 0;
    if (buttons & 0x6000u) {
        sint32 glyph = gtmain_menu_glyph_index((sint8)r_u8(request + 12u), (sint8)r_u8(request + 13u));
        if (glyph < 0) {
            result = 0;
        } else {
            sint32 width, glyph_width, count;
            uint32 limit, address, total;
            if (length >= (sint32)r_u16(request + 10u)) goto reject;
            w_u8(0x80092240u, (uint8)glyph);
            width = gtmain_text_width(2, text);
            glyph_width = gtmain_text_width(2, 0x80092240u);
            total = (uint32)width + (uint32)glyph_width;
            limit = r_u16(request + 8u);
            if (limit && (sint32)total >= (sint32)limit) goto reject;
            count = length + 1;
            address = text + (uint32)count;
            while (cursor < count) {
                uint8 byte = r_u8(address - 1u);
                --count;
                w_u8(address, byte);
                --address;
            }
            w_u8(text + (uint32)cursor, (uint8)glyph);
            value = r_u8(request);
            limit = r_u8(request + 1u);
            w_u8(request, (uint8)(value + 1u));
            w_u8(request + 1u, (uint8)(limit + 1u));
            gtmain_menu_sound(5);
            if (!r_u16(request + 8u)) {
                sint32 new_length = (sint8)r_u8(request + 1u);
                uint32 maximum = r_u16(request + 10u);
                if (new_length >= (sint32)maximum) {
                    w_u8(request + 12u, 7u);
                    w_u8(request + 13u, 6u);
                    result = 0;
                }
            }
        }
    }
    if ((buttons & 0x1000u) && r_u16(request + 8u)) result = -2;
    value = r_u16(controls + 4u);
    repeat = r_u16(controls + 8u);
    if ((value | repeat) & 0x8000u) {
        sint32 count = cursor;
        uint32 address = text + (uint32)cursor, old_length;
        if (!cursor) goto reject;
        if (length >= cursor) {
            do {
                uint8 byte = r_u8(address);
                ++count;
                w_u8(address - 1u, byte);
                ++address;
            } while (length >= count);
        }
        value = r_u8(request);
        old_length = r_u8(request + 1u);
        w_u8(request, (uint8)(value - 1u));
        w_u8(request + 1u, (uint8)(old_length - 1u));
        gtmain_menu_sound(3);
    }
    return result;
reject:
    gtmain_menu_sound(6);
    return result;
}

/* GTMAIN.EXE 80048448 */
/* Original: GTMAIN.EXE:sub_80048448. */ uint32 gtmain_menu_name_request_prepare(uint32 owner, sint32 index, uint32 unchanged_result)
{
    uint32 entries = owner + 512u;
    gtmain_menu_resource_request_initialize(0x80092EACu, 17, 256, 0x800B5394u);
    if (index >= 0) return gtmain_menu_name_text_set(0x80092EACu, entries + 116u * (uint32)index);
    return unchanged_result;
}
