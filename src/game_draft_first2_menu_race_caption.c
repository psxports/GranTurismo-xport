#include "game_first2_registry.h"

/* GTMAIN.EXE 80033154 */
uint32 gtmain_menu_replay_caption_initialize(void)
{
    uint32 mode, source, name, course;
    w_u32(0x8009199Cu, 0x800BCA6Au);
    mode = r_u8(0x801D2FEDu);
    if (mode == 0u) w_u32(0x8009199Cu, 0x800BCA3Au);
    else if (mode == 1u) w_u32(0x8009199Cu, 0x800BCA52u);
    gtmain_time_string(0x801D3118u, r_u32(0x801D2FFCu));
    source = r_u32(0x801D2FF4u);
    name = gtmain_resource_name(r_u16(source + 8u));
    course = r_u8(0x800B63BCu);
    w_u32(0x801D3004u, name);
    name = gtmain_course_resource_name(course);
    w_u32(0x801D3008u, gtmain_menu_name(name));
    gtmain_menu_mode_setup(2);
    return 0u;
}

/* GTMAIN.EXE 800361D8 */
uint32 gtmain_menu_race_caption_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 course_line)
{
    uint32 text, result;
    sint32 width, centered;
    gtmain_text_font(1);
    gtmain_text_flags(0);
    gtmain_text_color(gtmain_menu_palette_color(7, intensity >> 1));
    text = gtmain_course_name((uint32)(sint32)(sint16)r_u16(0x800B63C4u));
    width = gtmain_menu_text_width(1, text, 0);
    centered = (sint32)((uint32)x - ((uint32)(width >> 1) - 160u));
    result = gtmain_menu_text_draw(ordering, centered, (sint32)((uint32)y + 48u), text, 0);
    if (course_line) {
        text = gtmain_menu_name(gtmain_course_resource_name(r_u8(0x800B63BCu)));
        width = gtmain_menu_text_width(1, text, 0);
        centered = (sint32)((uint32)x - ((uint32)(width >> 1) - 160u));
        return gtmain_menu_text_draw(ordering, centered, (sint32)((uint32)y + 57u), text, 0);
    }
    return result;
}
