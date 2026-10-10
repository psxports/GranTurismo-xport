#include "game_first2_registry.h"

/* GTMAIN.EXE 80030828 */
sint32 gtmain_race_countdown_block_draw(uint32 ordering, sint32 y, sint32 label)
{
    uint32 next_y;
    if (label >= 0 && (sint16)r_u16(0x800B63DEu)) {
        next_y = (uint32)y + 44u;
        gtmain_race_label_font3_draw(ordering, y, 0x00808080u, (uint32)label);
    } else {
        next_y = (uint32)y + 28u;
    }
    return gtmain_race_countdown_text_draw(ordering, (sint32)next_y);
}

/* GTMAIN.EXE 80030974 */
uint32 gtmain_race_centered_label_box_draw(uint32 ordering, uint32 color, uint32 text, uint32 corners, sint32 center_x)
{
    sint32 width, right, left, top, half, x;
    uint32 gap;
    gtmain_text_font(1);
    gtmain_text_color(0x00808080u);
    width = gtmain_text_width(1, text);
    right = (sint16)r_u16(corners + 4u);
    left = (sint16)r_u16(corners);
    top = (sint16)r_u16(corners + 2u);
    gap = (uint32)right - (uint32)left - (uint32)width;
    half = (sint32)(gap + (gap >> 31)) >> 1;
    x = (sint32)((uint32)center_x + (uint32)half);
    gtmain_font_text_draw(ordering, x, (sint32)((uint32)top + 4u), text);
    gtmain_packet_outline(ordering, corners, 0x00404040u);
    return gtmain_packet_rectangle(ordering, corners, color);
}

/* GTMAIN.EXE 800380F0 */
sint32 gtmain_menu_machine_caption_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    sint16 corners[4];
    uint32 color;
    corners[0] = (sint16)x;
    corners[1] = (sint16)((uint32)y + 41u);
    corners[2] = (sint16)((uint32)x + 319u);
    corners[3] = corners[1];
    gtmain_packet_rectangle_native(ordering, corners, (uint8)intensity);
    color = gtmain_menu_gray((uint8)(intensity >> 1));
    gtmain_text_color(color);
    gtmain_text_font(1);
    gtmain_text_flags(0u);
    return gtmain_font_text_draw(ordering, (sint32)((uint32)x + 160u), (sint32)((uint32)y + 32u), 0x800BD92Fu);
}

/* GTMAIN.EXE 80030938 */
uint32 gtmain_race_choice_highlight(uint32 command)
{
    uint32 phase = r_u16(0x800912E2u);
    uint32 intensity = phase & 7u;
    if (phase & 8u) intensity ^= 7u;
    intensity = (intensity << 5) & 255u;
    return (command << 24) | intensity | (intensity << 8);
}

/* GTMAIN.EXE 80030CA0 */
uint32 gtmain_race_choice_boxes_draw(void)
{
    uint32 ordering = r_u32(0x800B6368u);
    uint32 first = gtmain_race_choice_highlight(0u), second = 0u;
    if ((sint16)r_u16(0x800912E0u)) {
        second = first;
        first = 0u;
    }
    gtmain_race_centered_label_box_draw(ordering, first, 0x800BD82Bu, 0x800912FCu, 64);
    return gtmain_race_centered_label_box_draw(ordering, second, 0x800BD959u, 0x80091304u, 160);
}
