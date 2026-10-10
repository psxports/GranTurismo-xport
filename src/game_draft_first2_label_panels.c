#include "game_first2_registry.h"

/* GTMAIN.EXE 800300A0 */
sint32 gtmain_race_label_font2_draw(uint32 ordering, sint32 y, uint32 color, uint32 label)
{
    sint32 x;
    uint32 text;
    gtmain_text_font(2);
    gtmain_text_color(color);
    label <<= 3;
    x = (sint16)r_u16(0x800911A8u + label);
    text = r_u32(0x800911ACu + label);
    return gtmain_font_text_draw(ordering, x, y, text);
}

/* GTMAIN.EXE 8003011C */
sint32 gtmain_race_label_font3_draw(uint32 ordering, sint32 y, uint32 color, uint32 label)
{
    sint32 x;
    uint32 text;
    gtmain_text_font(3);
    gtmain_text_color(color);
    label <<= 3;
    x = (sint16)r_u16(0x800911F8u + label);
    text = r_u32(0x800911FCu + label);
    return gtmain_font_text_draw(ordering, x, y, text);
}

/* GTMAIN.EXE 800308D0 */
uint32 gtmain_race_label_panel_draw(uint32 ordering, uint32 label, uint32 color)
{
    gtmain_race_label_font3_draw(ordering, 12, 0x00808080u, label);
    gtmain_packet_rectangle(ordering, 0x800912D0u, color);
    return gtmain_packet_rectangle(ordering, 0x800912D8u, 0u);
}

/* GTMAIN.EXE 8002A500 */
uint32 gtmain_course_route_caption_update(void)
{
    uint32 number = r_u8(0x800CB7F9u) + 1u;
    uint32 route = r_u8(0x800CB7F8u);
    uint32 arguments[3];
    arguments[0] = 0x800BD926u;
    arguments[1] = r_u32(0x8008FAF8u + (route << 2));
    arguments[2] = number;
    gtmain_string_format(0x800BC2E0u, 0x8008BFE0u, arguments, 3u);
    return gtmain_course_caption_copy(0x800BC2E0u);
}
