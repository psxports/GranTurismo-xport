#include "game_first2_registry.h"

/* GTMAIN.EXE 80043FE4 */
uint32 gtmain_menu_vehicle_course_labels_draw(uint32 ordering, sint32 x,
    sint32 y, sint32 intensity)
{
    uint32 text, width, course_name;
    sint32 center;
    gtmain_text_font(1);
    gtmain_text_flags(0);
    gtmain_text_color(gtmain_menu_palette_color(7, intensity >> 1));
    text = gtmain_resource_name((uint32)r_s16(0x800B63C4u));
    width = gtmain_menu_text_width(1, text, 0);
    center = (sint32)((uint32)x - ((uint32)((sint32)width >> 1) - 160u));
    gtmain_menu_text_draw(ordering, center, (sint32)((uint32)y + 44u), text, 0);
    course_name = gtmain_course_resource_name(r_u8(0x800B63BCu));
    text = gtmain_menu_name(course_name);
    if (r_u8(0x800CB7F5u) == 5u) text = 0x800BD4DAu;
    width = gtmain_menu_text_width(1, text, 0);
    center = (sint32)((uint32)x - ((uint32)((sint32)width >> 1) - 160u));
    return gtmain_menu_text_draw(ordering, center, (sint32)((uint32)y + 52u), text, 0);
}
