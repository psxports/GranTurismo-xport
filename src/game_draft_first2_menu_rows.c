#include "game_first2_registry.h"

static void row_format(char output[16], sint32 number)
{
    /* TODO Bind bounded native destination formatting using the original format */
    (void)output; (void)number;
    gt_draft_failure("Native menu formatted string adapter", 0x8007C014u);
}
static uint32 row_native_character(uint32 ordering, uint32 x, uint32 y, const char text[8])
{
    /* TODO Bind the original text service with an actual native character buffer */
    (void)ordering; (void)x; (void)y; (void)text;
    gt_draft_failure("Native menu character string adapter", 0x8003A8ACu);
}
static sint32 row_half(uint32 value) { return (sint32)value >> 1; }
uint32 gtmain_menu_row_draw(uint32 ordering, sint32 x, sint32 y, uint32 guest_text, sint32 span, sint32 row, uint32 animated, sint32 number, uint32 alternate)
{
    char formatted[16], character[8];
    uint32 position = (uint32)x + (uint32)row * 24u, color, result, i;
    sint32 product = (sint32)((uint32)span * (uint32)row);
    sint32 intensity = row_half((uint32)span - (uint32)(product / 6));
    character[0] = 0; character[1] = 0;
    if (animated) {
        sint32 phase = (sint8)r_u8(0x801D2FEAu);
        color = gtmain_menu_palette_color((sint8)r_u8(0x80091950u + (uint32)phase), intensity);
        (void)gtmain_text_color(color);
        (void)gtmain_menu_text_draw(ordering, (sint32)(position + 92u), y, 0x800BCA2Au, 0);
        phase = (sint8)r_u8(0x801D2FEAu);
        color = gtmain_menu_palette_color((sint8)r_u8(0x8009194Cu + (uint32)phase), intensity);
    } else color = gtmain_menu_gray((uint8)intensity);
    (void)gtmain_text_color(color);
    result = alternate ? gtmain_menu_text_draw(ordering, (sint32)(position + 8u), y, guest_text, 0) : gtmain_menu_text_wave(ordering, (sint32)(position + 8u), y, guest_text);
    if (number >= 0) {
        (void)gtmain_menu_text_draw(ordering, (sint32)(position - 63u), y, 0x800BCA19u, 0);
        position += gtmain_menu_text_width(2, 0x800BCA19u, 0) - 60u;
        row_format(formatted, number);
        i = 0; result = (uint32)(sint32)(sint8)formatted[0];
        while (result) {
            if (i >= sizeof(formatted)) gt_draft_failure("Native menu formatted buffer extent", 0x800321A0u);
            character[0] = formatted[i++];
            (void)row_native_character(ordering, position, (uint32)y, character);
            if (i >= sizeof(formatted)) gt_draft_failure("Native menu formatted terminator extent", 0x800321A0u);
            result = (uint32)(sint32)(sint8)formatted[i];
            position += 8u;
        }
    }
    return result;
}
uint32 gtmain_menu_row_transition(uint32 ordering, sint32 x, sint32 y, uint32 guest_text, sint32 span, sint32 row, sint32 palette)
{
    uint32 remaining, position, color;
    sint32 product, intensity;
    if (row >= 7) row = 6;
    remaining = 6u - (uint32)row;
    position = (uint32)x + remaining * 24u;
    product = (sint32)((uint32)span * remaining);
    intensity = row_half((uint32)span - (uint32)(product / 6));
    color = gtmain_menu_palette_color(palette, intensity);
    (void)gtmain_text_color(color);
    return gtmain_menu_text_draw(ordering, (sint32)position, y, guest_text, 0);
}
uint32 gtmain_menu_footer_draw(uint32 ordering, sint32 x, sint32 y, sint32 span)
{
    uint32 color, text;
    sint32 width;
    color = gtmain_menu_palette_color(7, row_half((uint32)span));
    (void)gtmain_text_color(color);
    text = r_u32(0x801D3004u); width = gtmain_text_width(1, text);
    text = r_u32(0x801D3004u);
    (void)gtmain_font_string_draw(0x800C0070u, ordering, (sint32)((uint32)x - (uint32)row_half((uint32)width)), y, text);
    text = r_u32(0x801D3008u); width = gtmain_text_width(1, text);
    text = r_u32(0x801D3008u);
    return (uint32)gtmain_font_string_draw(0x800C0070u, ordering, (sint32)((uint32)x - (uint32)row_half((uint32)width)), (sint32)((uint32)y + 8u), text);
}
