#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

static uint32 settings_text(uint32 ordering, sint32 x, sint32 y, const char *text, sint32 spacing, uint32 style)
{
    /* TODO Bind unselected text layout to a native string */
    gt_draft_failure("Settings native text layout adapter", 0x8003A95Cu);
}
static uint32 settings_text_guest(uint32 ordering, sint32 x, sint32 y, uint32 text, sint32 spacing, uint32 style)
{
    return settings_text(ordering, x, y, (const char *)psx_addr(text, 1u), spacing, style);
}
static void settings_identifier(char *text, uint32 format, uint32 number)
{
    /* TODO Bind the original dynamic format to a native output buffer */
    gt_draft_failure("Settings native identifier format adapter", 0x8007C014u);
}
static sint32 settings_summary(uint32 record, uint8 *values64, uint8 *detail112)
{
    /* TODO Bind unselected summary outputs without a guest frame */
    gt_draft_failure("Settings summary output adapter", 0x80058BB0u);
}
static void settings_number(char *text, sint32 number, uint32 suffix)
{
    /* TODO Reuse selected number formatting with a native destination */
    gt_draft_failure("Settings native scaled number adapter", 0x8003AA10u);
}
uint32 gtmain_menu_settings_row_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 entry, sint32 row)
{
    uint32 descriptor = 0x800BB850u + 386u * (uint32)entry;
    uint32 resource = gtmain_resource_name(r_u16(0x800B6670u + 76u * (uint32)entry));
    uint32 label = r_u32(0x80092D5Cu + 4u * r_u8(descriptor + 138u));
    sint16 corners[4];
    char text[32];
    uint8 values[64], detail[112];
    sint32 palette, length, number;
    uint32 product, high;
    sint32 text_y = (sint32)((uint32)y + 4u), label_y = (sint32)((uint32)y + 16u);
    corners[0] = (sint16)((uint32)x + 24u);
    corners[1] = (sint16)((uint32)y + 25u);
    corners[2] = (sint16)((uint32)x + 343u);
    corners[3] = corners[1];
    gtmain_packet_outline_native(ordering, corners,
        gtmain_menu_palette_color(5, (sint32)((uint32)intensity << 1u)) | 0x02000000u);
    gtmain_text_font(2);
    text[0] = (char)((uint32)row + 49u); text[1] = '.'; text[2] = 0;
    palette = 6;
    if ((sint8)r_u8(0x800BEA24u))
    {
        sint32 index = row < 4 ? row : 3;
        palette = (sint8)r_u8(0x80091B28u + 4u * (uint32)index + (r_u8(0x80091B40u) & 3u));
    }
    gtmain_text_color(gtmain_menu_palette_color(palette, intensity));
    settings_text(ordering, x, text_y, text, 8, 1);
    gtmain_text_color(gtmain_menu_gray((uint8)intensity));
    if (!entry) gtmain_text_color(gtmain_menu_palette_color(7, intensity));
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 24u),
        (sint32)((uint32)y - 1u), resource, 0);
    settings_identifier(text, 0x800BCF24u, r_u16(0x800B6840u + 2u * (uint32)entry));
    length = (sint32)strlen(text);
    settings_text(ordering, (sint32)((uint32)x - ((uint32)length * 8u - 266u)), text_y, text, 8, 2);
    gtmain_text_font(1);
    gtmain_text_color(gtmain_menu_palette_color(0, intensity));
    settings_text_guest(ordering, (sint32)((uint32)x + 32u), label_y, label, 8, 1);
    number = settings_summary(descriptor, values, detail);
    settings_number(text, number, 0x800BCF1Eu);
    settings_text(ordering, (sint32)((uint32)x + 88u), label_y, text, 8, 1);
    number = (sint16)r_u16(descriptor + 90u);
    if (!r_u8(0x800C4C3Du))
    {
        product = (uint32)number * 22046u;
        high = (uint32)((uint64)((sint64)(sint32)product * 0x68DB8BAD) >> 32u);
        number = (sint32)((uint32)((sint32)high >> 12) - (uint32)((sint32)product >> 31));
    }
    settings_number(text, number, 0x800BCF21u);
    return settings_text(ordering, (sint32)((uint32)x + 152u), label_y, text, 8, 1);
}
