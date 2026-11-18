#include "game_first2_registry.h"

/* GTMAIN.EXE 80041150 */
/* Original: GTMAIN.EXE:sub_80041150. */ uint32 gtmain_menu_item_center_draw(uint32 ordering, uint32 descriptor,
    sint32 x, sint32 y, sint32 intensity)
{
    uint32 kind = r_u8(descriptor) & 63u;
    uint32 sprite, width, height, scale, text;
    sint32 left, top, text_width;
    if (kind == 0u) {
        sprite = r_u32(descriptor + 4u);
        width = r_u16(sprite + 8u);
        scale = r_u32(descriptor + 8u);
        height = r_u16(sprite + 10u);
        left = (sint32)((uint32)x - (uint32)((sint32)(scale * (width >> 1)) >> 7));
        top = (sint32)((uint32)y - (uint32)((sint32)(scale * (height >> 1)) >> 7));
        return gtmain_menu_scaled_sprite(sprite, ordering, (sint16)left,
            (sint16)top, (uint8)intensity, (sint32)scale);
    }
    if (kind == 1u) {
        sprite = r_u32(descriptor + 4u);
        width = r_u16(sprite + 8u);
        height = r_u16(sprite + 10u);
        left = (sint32)((uint32)x - (width >> 1));
        top = (sint32)((uint32)y - (height >> 1));
        return gtmain_menu_sprite(sprite, ordering, (uint16)left, top, (uint8)intensity);
    }
    if (kind != 2u) return 2u;
    (void)gtmain_text_color(gtmain_menu_gray((uint8)intensity));
    (void)gtmain_text_font(2);
    gtmain_text_flags(2);
    text = r_u32(descriptor + 8u);
    sprite = r_u32(descriptor + 4u);
    text_width = gtmain_text_width(2, text);
    width = r_u16(sprite + 8u);
    height = r_u16(sprite + 10u);
    left = (sint32)((uint32)x - (uint32)((sint32)((uint32)text_width + width) >> 1));
    top = (sint32)((uint32)y - (height >> 1));
    (void)gtmain_font_text_draw(ordering, (sint32)((uint32)left + width), top, text);
    return gtmain_menu_sprite(sprite, ordering, (uint16)left, top, (uint8)intensity);
}
