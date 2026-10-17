#include "game_first2_registry.h"

static sint32 choice_add(sint32 a, sint32 b)
{
    return (sint32)((uint32)a + (uint32)b);
}

static sint32 choice_sub(sint32 a, sint32 b)
{
    return (sint32)((uint32)a - (uint32)b);
}

uint32 gtmain_menu_choice_transition_draw(uint32 ordering, uint32 descriptor,
    sint32 x, sint32 y, sint32 intensity, uint32 publish, uint32 reverse,
    uint32 lower_title)
{
    sint32 step = (sint8)r_u8(descriptor + 13u);
    sint32 offset, center, fade, width, title_y, text_y, center16, outline_intensity;
    sint16 left, middle_left, right, corners[4];
    uint32 text, color;
    if (step > 0) w_u8(descriptor + 13u, (uint8)(step - 1));
    step = (sint8)r_u8(descriptor + 13u);
    offset = 160 * step / 12;
    center = reverse ? choice_sub(x, offset) : choice_add(x, offset);
    step = (sint8)r_u8(descriptor + 13u);
    fade = (sint32)((uint32)(intensity >> 1) * (uint32)(12 - step)) / 12;
    gtmain_text_color(gtmain_menu_gray((uint8)fade));
    gtmain_text_font(1);
    gtmain_text_flags(0);
    text = r_u32(descriptor);
    width = gtmain_text_width(1, text);
    title_y = (sint16)y + (lower_title ? 20 : -12);
    center16 = (sint16)center;
    gtmain_font_text_draw(ordering, choice_sub(center16, width >> 1), title_y, text);
    left = (sint16)choice_add(center, -96);
    middle_left = (sint16)choice_add(center, -1);
    text = r_u32(descriptor + 4u);
    right = (sint16)choice_add(center, 95);
    width = gtmain_text_width(1, text);
    text_y = (sint16)y + 3;
    gtmain_font_text_draw(ordering, choice_sub(center16, choice_add(width, 96) >> 1), text_y, text);
    text = r_u32(descriptor + 8u);
    width = gtmain_text_width(1, text);
    gtmain_font_text_draw(ordering, choice_add(center16, choice_sub(96, width) >> 1), text_y, text);
    outline_intensity = (sint32)((uint32)fade * 51u) >> 7;
    corners[0] = left; corners[1] = (sint16)y;
    corners[2] = middle_left; corners[3] = (sint16)choice_add(y, 15);
    color = gtmain_menu_gray((uint8)outline_intensity);
    gtmain_packet_outline_native(ordering, corners, color);
    corners[0] = (sint16)center; corners[1] = (sint16)y;
    corners[2] = right; corners[3] = (sint16)choice_add(y, 15);
    color = gtmain_menu_gray((uint8)outline_intensity);
    gtmain_packet_outline_native(ordering, corners, color);
    if (!(sint8)r_u8(descriptor + 13u))
    {
        if ((sint8)r_u8(descriptor + 12u))
        {
            corners[0] = (sint16)center; corners[2] = right;
        }
        else
        {
            corners[0] = left; corners[2] = middle_left;
        }
        corners[1] = (sint16)y; corners[3] = (sint16)choice_add(y, 15);
        color = (uint32)gtmain_menu_pulse_component(2, intensity) & 255u;
        if (publish == 1u) color <<= 16;
        gtmain_packet_rectangle_native(ordering, corners, color);
    }
    corners[0] = (sint16)choice_add(center, -96); corners[1] = (sint16)y;
    corners[2] = (sint16)choice_add(center, 95); corners[3] = (sint16)choice_add(y, 15);
    gtmain_packet_rectangle_native(ordering, corners, 0x02000000u);
    return gtmain_packet_page(ordering, 512);
}
