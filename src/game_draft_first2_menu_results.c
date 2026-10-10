#include "game_first2_registry.h"
#include <stdio.h>

static sint32 result_add(sint32 a, sint32 b) { return (sint32)((uint32)a + (uint32)b); }
static sint32 result_phase(void) { return (sint16)r_u16(0x801D2FE4u); }
static void result_mode(sint32 mode) { (void)gtmain_menu_mode_setup(mode); }
static void result_format_native(char output[32], sint32 number)
{
    /* TODO Bind the bounded native destination to original format 800BCE21 */
    (void)output; (void)number;
    gt_draft_failure("Native result label formatter", 0x8007C014u);
}
static sint32 result_native_width(const char label[32])
{
    /* TODO Preserve native string provenance at the width service */
    gt_draft_failure("Native result label width", 0x8006C900u);
}
static void result_native_transition(uint32 ordering, sint32 x, sint32 y, const char label[32], sint32 intensity, sint32 phase, sint32 palette)
{
    /* TODO Preserve the real native label at the transition service */
    (void)ordering; (void)x; (void)y; (void)label; (void)intensity; (void)phase; (void)palette;
    gt_draft_failure("Native result label transition", 0x800323A0u);
}
static void result_descriptor(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint8 output[152])
{
    (void)gtmain_menu_descriptor_draw(ordering,0x801D3AECu,x,y,intensity,1,output,152);
}
static sint32 result_descriptor_lookup(const uint8 descriptor[152], sint32 selector, uint32 *value, uint32 *has_value)
{
    uint32 i = 0;
    sint32 key = (sint8)descriptor[2];
    if (key < 0) return -256;
    key = (sint8)descriptor[2];
    for (;;) {
        if (selector == key) {
            uint32 offset = 98u + 2u * i;
            if (offset + 1u >= 152u || i + 26u >= 152u)
                gt_draft_failure("Native result descriptor extent", 0x800324C4u);
            *value = descriptor[i + 26u];
            *has_value = 1u;
            return (sint16)(uint16)(descriptor[offset] | ((uint16)descriptor[offset + 1u] << 8));
        }
        ++i;
        if (i + 2u >= 152u) gt_draft_failure("Native result descriptor sentinel", 0x800324C4u);
        key = (sint8)descriptor[i + 2u];
        if (key < 0) return -256;
    }
}
static void result_circle(uint32 ordering, sint32 x, sint32 y)
{
    sint32 phase = (sint16)r_u16(0x801D2FE8u);
    (void)gtmain_menu_indicator_draw(ordering,x,y,phase,630,0,3,2);
}
static void result_label(uint32 ordering, sint32 x, sint32 y, uint32 label, const char native_label[32], sint32 intensity, sint32 threshold, sint32 palette)
{
    sint32 width = native_label ? result_native_width(native_label) : gtmain_text_width(3, label);
    sint32 centered = (sint32)((uint32)x - (uint32)((width >> 1) - 160));
    sint32 phase = result_phase() - threshold;
    if (native_label) result_native_transition(ordering, centered, y, native_label, intensity, phase, palette);
    else (void)gtmain_menu_row_transition(ordering, centered, y, label, intensity, phase, palette);
}
uint32 gtmain_menu_result_prepare(void)
{
    return gtmain_menu_result_tail_prepare((sint16)r_u16(0x801D2FEEu));
}

/* GTMAIN.EXE 800335F4 receives the competitor predicate in v0 */
uint32 gtmain_menu_result_tail_prepare(sint32 competitor)
{
    sint32 mode;
    uint32 name;
    w_u32(0x800919B8u, 0x800BC9C3u);
    if (competitor >= 0) w_u32(0x800919B8u, 0x800BC9CDu);
    name = gtmain_resource_name((uint32)(sint32)(sint16)r_u16(0x800B63C4u));
    mode = (sint8)r_u8(0x801D2FECu);
    w_u32(0x801D3004u, name); w_u32(0x801D3008u, 0x800921D4u);
    if (mode == 3) {
        uint32 argument;
        if (r_u32(0x801D2FFCu) >= 10001u) w_u32(0x801D2FFCu, 9999);
        argument = r_u32(0x801D2FFCu);
        (void)gtmain_string_format(0x801D3118u, 0x800BCE26u, &argument, 1);
    } else (void)gtmain_time_string(0x801D3118u, r_u32(0x801D2FFCu));
    result_mode((sint8)r_u8(0x801D2FECu));
    return 0;
}
uint32 gtmain_menu_lap_results_prepare(void)
{
    return gtmain_menu_lap_results_tail_prepare(r_u8(0x801D2FEDu));
}

/* GTMAIN.EXE 80032D24 receives record flags in v0 */
uint32 gtmain_menu_lap_results_tail_prepare(uint32 record_flags)
{
    uint32 active = record_flags & 1u, record = 0, flag, name, course, i = 0, offset = 332;
    sint32 laps, mode;
    if (active) {
        mode = (sint8)r_u8(0x801D2FECu);
        record = r_u32(r_u32(0x801D2FF0u) + ((mode == 5 || (mode >= 7 && mode < 9)) ? 12u : 4u));
    }
    flag = r_u8(0x801D2FEDu);
    w_u32(0x80091980u, 0x800BC9C3u);
    if (flag) w_u32(0x80091980u, 0x800BC9CDu);
    name = gtmain_resource_name((uint32)(sint32)(sint16)r_u16(0x800B63C4u));
    course = r_u8(0x800B63BCu); w_u32(0x801D3004u, name);
    name = gtmain_menu_name(gtmain_course_resource_name(course));
    laps = (sint16)r_u16(0x801D3000u); w_u32(0x801D3008u, name);
    if (laps > 0) do {
        uint32 value = r_u32(r_u32(0x801D2FF8u) + i * 4u);
        (void)gtmain_time_string(0x801D2FDCu + offset, value);
        w_u8(0x801D3768u + i, 0);
        if (active && record == value) w_u8(0x801D3768u + i, 1);
        laps = (sint16)r_u16(0x801D3000u); ++i; offset += 16u;
    } while ((sint32)i < laps);
    (void)gtmain_time_string(0x801D3118u, r_u32(0x801D2FFCu));
    flag = r_u16(0x801D3000u); laps = (sint16)r_u16(0x801D3000u);
    w_u8(0x801D3AECu, flag); w_u8(0x801D3AEEu, laps > 3 ? 3u : (uint32)laps);
    gtmain_menu_selector_reset(0x801D3AECu);
    printf("record %d\n", (int)r_u8(0x801D2FEDu));
    printf("laps %d\n", (int)(sint16)r_u16(0x801D3000u));
    result_mode((sint8)r_u8(0x801D2FECu));
    return 0;
}
uint32 gtmain_menu_result_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    sint32 competitor = (sint16)r_u16(0x801D2FEEu);
    sint32 mode = (sint8)r_u8(0x801D2FECu);
    return gtmain_menu_result_tail_draw(ordering, x, y, intensity, mode, competitor);
}

/* GTMAIN.EXE 80033340 receives mode and competitor in v0 and v1 */
uint32 gtmain_menu_result_tail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 mode, sint32 competitor)
{
    uint32 alternate = mode == 3, present = competitor >= 0, label;
    sint32 row_x, palette;
    char formatted[32]; const char *native_label = 0;
    if (!present) { row_x = result_add(x, 124); label = 0x800BCE08u; palette = 5; }
    else {
        row_x = result_add(x, 100);
        if (competitor >= 3) {
            label = 0; palette = 3; native_label = formatted;
            result_format_native(formatted, (sint16)r_u16(0x801D2FEEu) + 1);
        } else {
            label = r_u32(0x80091954u + (uint32)competitor * 4u);
            palette = (sint8)r_u8(0x80091B28u + (uint32)competitor * 4u + (uint32)(sint32)(sint8)r_u8(0x801D2FEAu));
        }
    }
    (void)gtmain_text_font(1); gtmain_text_flags(0);
    (void)gtmain_menu_footer_draw(ordering, result_add(x, 160), result_add(y, 48), intensity);
    (void)gtmain_text_font(2); (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
    if (result_phase() >= 54) (void)gtmain_menu_row_transition(ordering, result_add(x,40), result_add(y,80), alternate ? 0x800BCDF2u : 0x800BCDE0u, intensity, result_phase()-54, 3);
    if (result_phase() >= 78) (void)gtmain_menu_row_transition(ordering, result_add(x,40), result_add(y,128), 0x800BCDE7u, intensity, result_phase()-78, 3);
    if ((sint16)r_u16(0x801D2FDCu) >= 0) (void)gtmain_menu_row_draw(ordering,row_x,result_add(y,96),0x801D3118u,intensity,(sint16)r_u16(0x801D2FDCu),present,-1,alternate);
    (void)gtmain_text_font(3);
    if (result_phase() >= 90) result_label(ordering,x,result_add(y,144),label,native_label,intensity,90,palette);
    result_circle(ordering + 8u, result_add(x,160), result_add(y,136));
    return 0;
}
uint32 gtmain_menu_lap_results_draw(uint32 ordering, sint32 x, sint32 y, uint32 descriptor_mode, sint32 intensity)
{
    sint32 initial_mode = r_s8(0x801D2FECu);
    /* The descriptor owner does not read this original argument */
    return gtmain_menu_lap_results_tail_draw(ordering, x, y, intensity, initial_mode);
}

/* GTMAIN.EXE 800328B4 receives its initial mode in v1 */
uint32 gtmain_menu_lap_results_tail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 initial_mode)
{
    sint32 mode = initial_mode, vertical_a = 0, vertical_b = 0, competitor, palette, list_x, scroll_x;
    uint32 label, flag, animated, i = 0, offset = 332;
    uint32 descriptor_value, has_descriptor_value = 0u;
    char formatted[32]; uint8 descriptor[152]; const char *native_label = 0;
    if (mode >= 7 && mode < 9) { vertical_a = 24; vertical_b = 16; }
    competitor = (sint16)r_u16(0x801D2FEEu);
    if (competitor < 0) { label = 0x800BCE08u; palette = 5; }
    else if (competitor >= 3) {
        label = 0; palette = 3; native_label = formatted;
        result_format_native(formatted, (sint16)r_u16(0x801D2FEEu) + 1);
    } else {
        label = r_u32(0x80091954u + (uint32)competitor * 4u);
        palette = (sint8)r_u8(0x80091B28u + (uint32)competitor * 4u + (uint32)(sint32)(sint8)r_u8(0x801D2FEAu));
    }
    flag = r_u8(0x801D2FEDu); animated = flag & 2u;
    list_x = result_add(x, (flag & 1u) ? 100 : 148);
    scroll_x = result_add(x, (flag & 1u) ? 164 : 176);
    if (result_phase() != 127) scroll_x = result_add(x,-256);
    result_descriptor(ordering,scroll_x,result_add(y,168-vertical_b),intensity,descriptor);
    (void)gtmain_text_font(1); gtmain_text_flags(0);
    (void)gtmain_menu_footer_draw(ordering,result_add(x,160),result_add(y,48),intensity);
    (void)gtmain_text_font(2);
    mode = (sint8)r_u8(0x801D2FECu);
    if ((mode < 7 || mode >= 9) && result_phase() >= 54) (void)gtmain_menu_row_transition(ordering,result_add(x,32),result_add(y,72),0x800BCDE7u,intensity,result_phase()-54,3);
    (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
    if (result_phase() >= 78) (void)gtmain_menu_row_transition(ordering,result_add(x,32),result_add(y,114-vertical_a),0x800BC9FBu,intensity,result_phase()-78,3);
    if (result_phase() >= 102) (void)gtmain_menu_row_transition(ordering,result_add(x,32),result_add(y,152-vertical_b),0x800BCA08u,intensity,result_phase()-102,3);
    if ((sint16)r_u16(0x801D2FDCu) >= 0) (void)gtmain_menu_row_draw(ordering,result_add(x,animated ? 100 : 148),result_add(y,130-vertical_a),0x801D3118u,intensity,(sint16)r_u16(0x801D2FDCu),animated,-1,0);
    if ((sint16)r_u16(0x801D2FE2u) > 0) do {
        sint32 row = 0, position = (sint16)result_descriptor_lookup(descriptor,
            (sint32)i, &descriptor_value, &has_descriptor_value);
        if ((sint32)i == (sint16)r_u16(0x801D2FE2u)-1) {
            row = (sint16)r_u16(0x801D2FDEu);
            if (row) position = result_add(position,(sint16)(uint16)(descriptor[0] | ((uint16)descriptor[1] << 8)));
        }
        if (position != -256) {
            /* TODO Recover original untouched output provenance after an initial miss */
            if (!has_descriptor_value)
                gt_draft_failure("Lap result descriptor output provenance", 0x800328B4u);
            (void)gtmain_menu_row_draw(ordering,list_x,position,0x801D2FDCu+offset,
                (sint32)(descriptor_value*2u),row,
                (uint32)(sint32)(sint8)r_u8(0x801D3768u+i),(sint32)i+1,0);
        }
        ++i; offset += 16u;
    } while ((sint32)i < (sint16)r_u16(0x801D2FE2u));
    mode = (sint8)r_u8(0x801D2FECu);
    if (mode < 7 || mode >= 9) {
        (void)gtmain_text_font(3);
        if (result_phase() >= 66) result_label(ordering,x,result_add(y,88),label,native_label,intensity,66,palette);
    }
    result_circle(ordering+8u,result_add(x,160),result_add(y,136));
    return 0;
}
