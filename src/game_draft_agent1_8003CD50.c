#include "game_first2_registry.h"

static sint32 status_text_center(sint32 x, sint32 width)
{
    return (sint32)((uint32)x - (uint32)(width >> 1) + 160u);
}

/* Unverified translation from complete pseudocode and original MIPS */
uint32 gtmain_menu_status_text_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    uint32 first_text = 0x800BCCAEu, second_text = 0x800BCCCAu;
    uint32 context = r_u32(0x800B52FCu);
    sint32 mode = r_s8(context + 9u);
    sint32 half_intensity, row, palette, width;
    uint32 color;
    if (mode == 1)
    {
        first_text = 0x800BCA78u;
        second_text = 0x800BCA93u;
    }
    gtmain_text_font(3);
    gtmain_text_flags(0u);
    context = r_u32(0x800B52FCu);
    row = r_s8(context + 8u);
    half_intensity = intensity >> 1;
    palette = r_s8(0x80092244u + (uint32)row);
    color = gtmain_menu_palette_color(palette, half_intensity);
    gtmain_text_color(color);
    width = gtmain_text_width(3, first_text);
    gtmain_font_text_draw(ordering, status_text_center(x, width),
        (sint32)((uint32)y + 96u), first_text);
    gtmain_text_font(2);
    color = gtmain_menu_gray((uint8)half_intensity);
    gtmain_text_color(color);
    width = gtmain_text_width(2, second_text);
    gtmain_font_text_draw(ordering, status_text_center(x, width),
        (sint32)((uint32)y + 124u), second_text);
    context = r_u32(0x800B52FCu);
    if (r_s8(context) >= 60)
    {
        gtmain_text_font(2);
        color = gtmain_menu_pulse_color(half_intensity);
        gtmain_text_color(color);
        width = gtmain_text_width(2, 0x800BCC0Cu);
        gtmain_font_text_draw(ordering, status_text_center(x, width),
            (sint32)((uint32)y + 204u), 0x800BCC0Cu);
    }
    return 0u;
}
