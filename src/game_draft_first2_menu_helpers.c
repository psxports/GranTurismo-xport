#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 menu_high_divide(sint32 value, uint32 multiplier, uint32 shift)
{
    sint64 product = (sint64)value * (sint64)(sint32)multiplier;
    sint32 high = (sint32)(uint32)((uint64)product >> 32);
    return (sint32)((uint32)(high >> shift) - (uint32)(value >> 31));
}
/* Original: GTMAIN.EXE:sub_8003AA10. */ uint32 gtmain_menu_number_scale(uint32 guest_destination, sint32 value, uint32 guest_suffix)
{
    sint32 product = (sint32)((uint32)value * 1000u);
    sint32 scaled = menu_high_divide(product, 0x2050C9F9u, 7);
    sint32 thousands = menu_high_divide(scaled, 0x10624DD3u, 6);
    return gtmain_menu_number_tail(guest_destination, scaled, guest_suffix, thousands);
}
/* Original: GTMAIN.EXE:sub_8003AA50. */ uint32 gtmain_menu_number_tail(uint32 guest_destination, sint32 value, uint32 guest_suffix, sint32 thousands)
{
    uint32 cursor = guest_destination, leading = 0;
    sint32 digit;
    if (thousands > 0) {
        w_u8(cursor++, (uint32)thousands + 48u);
        value = (sint32)((uint32)value - (uint32)thousands * 1000u); leading = 1;
    }
    digit = value / 100;
    if (digit >= 1 || leading) {
        w_u8(cursor++, (uint32)digit + 48u);
        value = (sint32)((uint32)value - (uint32)digit * 100u); leading = 1;
    }
    digit = value / 10;
    if (digit >= 1 || leading) {
        w_u8(cursor++, (uint32)digit + 48u);
        value = (sint32)((uint32)value - (uint32)digit * 10u);
    }
    w_u8(cursor, (uint32)value + 48u);
    return gtmain_string_copy(cursor + 1u, guest_suffix);
}
/* Original: GTMAIN.EXE:sub_8003AB80. */ uint32 gtmain_menu_text_digits(uint32 ordering, sint32 x, sint32 y, uint32 guest_text)
{
    uint32 text = guest_text, offsets = 0x8009218Cu, end, result;
    if (gtmain_menu_wave_select(guest_text)) offsets = 0x80092198u;
    end = offsets + 9u;
    do {
        sint32 character = (sint8)r_u8(text);
        sint32 draw_x;
        w_u8(0x8008CDB0u, (uint8)character); ++text;
        result = (uint32)character;
        if (!character) break;
        draw_x = character == 49 ? (sint32)((uint32)x + 2u) : x;
        (void)gtmain_font_text_draw(ordering, draw_x, y, 0x8008CDB0u);
        character = (sint8)r_u8(offsets++);
        x = (sint32)((uint32)x + (uint32)character);
        result = (sint32)offsets < (sint32)end;
    } while (result);
    return result;
}
/* Original: GTMAIN.EXE:sub_8003AD48. */ uint32 gtmain_menu_sprite(uint32 guest_descriptor, uint32 ordering, uint16 x, sint32 y, uint8 intensity)
{
    uint32 color = (uint32)intensity | ((uint32)intensity << 8) | ((uint32)intensity << 16) | 0x01000000u;
    uint32 uv = r_u32(guest_descriptor + 4u), dimensions = r_u32(guest_descriptor + 12u);
    (void)gtmain_packet_sprite(ordering, (uint32)x | ((uint32)y << 16), uv, dimensions, color);
    return gtmain_packet_page(ordering, (uint16)(r_u16(guest_descriptor) | 0x220u));
}
static uint32 menu_panel_callback_dispatch(uint32 token, uint32 ordering, sint32 x,
    sint32 y, uint32 display_y, uint32 fade)
{
    switch (token) {
    case 0x800365ECu:
        return gtmain_menu_race_list_draw(ordering, x, y, (sint32)fade);
    case 0x8003F6A4u:
        return gtmain_menu_record_screen_draw(ordering, x, y, display_y, (sint32)fade);
    case 0x80044A48u:
        return gtmain_menu_settings_rows_draw(ordering, x, y, (sint32)display_y, (sint32)fade);
    case 0x80045128u:
        return gt_draft_80045128(ordering, x, y, (sint32)display_y, (sint32)fade);
    case 0x80045718u:
        return gt_draft_80045718(ordering, x, y, (sint32)display_y, (sint32)fade);
    case 0x80045CF0u:
        return gt_draft_80045CF0(ordering, x, y, display_y, (sint32)fade);
    default:
        /* TODO Bind remaining original five-argument callback targets */
        gt_draft_failure("GTMAIN menu panel callback target", token);
    }
}

/* Original: GTMAIN.EXE:sub_8003AEE8. */ uint32 gtmain_menu_panel_callback(uint32 token, uint32 ordering, sint32 x, sint32 y, uint32 display_y, uint32 fade)
{
    return menu_panel_callback_dispatch(token, ordering, x, y, display_y, fade);
}
