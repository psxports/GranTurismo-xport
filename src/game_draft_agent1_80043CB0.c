#include "game_first2_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

/* Original GTMAIN.EXE:80043CB0 */
uint32 gtmain_menu_decimal_draw(uint32 ordering, sint32 x, sint32 y,
    sint32 value, sint32 precision, uint32 suffix, uint8 intensity, uint32 style)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 end, cursor, count = 0, descriptor = 0, icon_x = (uint32)x, result;
    uint32 color = intensity | ((uint32)intensity << 8) | ((uint32)intensity << 16) | 0x01000000;
    sint32 quotient;
    uint32 draw_x = (uint32)x, text;
    FUNCTION_MARKER(0x80043CB0, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x50, 0x80043CB0);
    end = frame.address + 0x28;
    w_u8(end + 1, 0);
    while (precision < 0) {
        ++precision;
        value = (sint32)((uint32)value * 10);
    }
    cursor = end;
    do {
        quotient = value / 10;
        precision = (sint32)((uint32)precision - 1);
        w_u8(cursor, (uint32)value + 48 - (uint32)quotient * 10);
        value = quotient;
        if (!precision) {
            --cursor;
            ++count;
            w_u8(cursor, 46);
        }
        if (!value) break;
        ++count;
        --cursor;
    } while ((sint32)count < 16);
    if (precision > 0) {
        cursor = end - count - 1;
        do {
            ++count;
            --precision;
            w_u8(cursor, 48);
            if (!precision) {
                --cursor;
                ++count;
                w_u8(cursor, 46);
            }
            --cursor;
        } while (precision > 0);
    }
    if (!precision) {
        ++count;
        w_u8(end - count, 48);
    }
    text = end - count;
    if (style != 1) draw_x -= gtmain_menu_fixed_text_width((sint32)style, text);
    gtmain_menu_fixed_text_draw(ordering, (sint32)draw_x, y, text, (sint32)style);
    switch (suffix) {
    case 1: icon_x -= 4; descriptor = 12; break;
    case 2: icon_x -= 6; descriptor = 13; break;
    case 3: icon_x -= 6; descriptor = 14; break;
    case 4: icon_x -= 4; descriptor = 15; break;
    case 5: icon_x -= 6; descriptor = 16; break;
    default: break;
    }
    if (descriptor) {
        uint32 record = 0x80092334 + 16 * descriptor;
        uint32 uv = r_u32(record + 12);
        uint32 dimensions = r_u32(record + 8);
        gtmain_packet_sprite(ordering, (uint16)icon_x | ((uint32)y << 16), uv, dimensions, color);
    }
    result = gtmain_packet_page(ordering, 552);
    gt_local_stack_pop(frame);
    return result;
}
