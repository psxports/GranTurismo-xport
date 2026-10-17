#include "game_first2_registry.h"

static uint32 palette_component(uint32 product)
{
    sint64 multiplied = (sint64)(sint32)product * (sint64)(sint32)0x80808081u;
    uint32 high = (uint32)((uint64)multiplied >> 32);
    uint32 added = high + product;
    return ((uint32)((sint32)added >> 7) - (uint32)((sint32)product >> 31)) & 255u;
}
uint32 gtmain_menu_palette_color(sint32 palette, sint32 intensity)
{
    uint32 address = 0x80091A78u + (uint32)palette * 3u;
    uint32 red_product = (uint32)intensity * r_u8(address);
    uint32 green_product = (uint32)intensity * r_u8(address + 1u);
    uint32 red = palette_component(red_product), green = palette_component(green_product);
    uint32 blue_product = (uint32)intensity * r_u8(address + 2u);
    return red | (green << 8) | (palette_component(blue_product) << 16);
}
uint32 gtmain_menu_gray(uint8 intensity)
{
    return (uint32)intensity | ((uint32)intensity << 8) | ((uint32)intensity << 16);
}
uint32 gtmain_menu_text_draw(uint32 ordering, sint32 x, sint32 y, uint32 guest_text, sint32 spacing)
{
    return gtmain_font_spaced_draw(0x800C0070u, ordering, x, y, guest_text, spacing);
}
uint32 gtmain_menu_text_wave(uint32 ordering, sint32 x, sint32 y, uint32 guest_text)
{
    uint32 text = guest_text, horizontal = 0x800921A4u;
    uint32 vertical = 0x800921B0u, end = 0x800921ADu, result;
    if (gtmain_menu_wave_select(guest_text)) {
        horizontal = 0x800921BCu; vertical = 0x800921C8u; end = 0x800921C5u;
    }
    do {
        sint32 offset;
        result = r_u8(text++);
        w_u8(0x8008CDB0u, result);
        if (!result) break;
        offset = (sint8)r_u8(vertical++);
        (void)gtmain_font_text_draw(ordering, x, (sint32)((uint32)y + (uint32)offset), 0x8008CDB0u);
        offset = (sint8)r_u8(horizontal++);
        x = (sint32)((uint32)x + (uint32)offset);
        result = (sint32)horizontal < (sint32)end;
    } while (result);
    return result;
}
uint32 gtmain_menu_text_width(sint32 font, uint32 guest_text, sint32 spacing)
{
    uint32 cursor = guest_text, length = 0, width;
    if ((sint8)r_u8(cursor)) {
        do { ++cursor; ++length; } while ((sint8)r_u8(cursor));
    }
    width = (uint32)gtmain_text_width(font, guest_text);
    return width + length * ((uint32)spacing - 1u);
}
