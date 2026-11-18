#include "game_first2_registry.h"
#include "xport_trace.h"

static sint32 menu_overlay_draw_missing(uint32 context)
{
    return (sint32)gtmain_menu_image_upload(context);
}

/* GTMAIN.EXE 80036B34 */
/* Original: GTMAIN.EXE:sub_80036B34. */ sint32 gtmain_menu_parameter_page_tick(void)
{
    uint32 input;
    sint32 selected;
    FUNCTION_MARKER(0x80036B34, "GTMAIN.EXE");
    input = r_u32(0x800BDD08u);
    gtmain_menu_stream_check();
    if (!r_s8(input)) return 0;
    gtmain_menu_parameter_selector_input(0x80092324u, input + 2u);
    selected = gtmain_menu_selector_input(0x80092314u, input + 2u);
    gtmain_menu_selector_layout(0x80092314u);
    if (selected == -1) return 0;
    gtmain_menu_sound(selected == -2 ? 2 : 4);
    return 1;
}

/* GTMAIN.EXE 80035D90 */
/* Original: GTMAIN.EXE:sub_80035D90. */ sint32 gtmain_menu_overlay_tick(void)
{
    sint32 enabled = r_s8(0x80091B39u);
    if (enabled) return menu_overlay_draw_missing(0x801F3FDCu);
    return enabled;
}

/* GTMAIN.EXE 8003B954 */
/* Original: GTMAIN.EXE:sub_8003B954. */ sint32 gtmain_menu_selector_input(uint32 selector, uint32 input)
{
    uint8 previous = r_u8(selector + 1u);
    uint16 buttons;
    w_u8(selector + 12u, previous);
    buttons = r_u16(input + 4u);
    if (buttons & 0x9000u) return -2;
    if (buttons & 0x6000u) {
        uint32 entry = r_u32(selector + 4u) + 8u * (uint32)(sint32)(sint8)previous;
        if (!(r_u8(entry) & 0x40u)) return (sint8)previous;
        gtmain_menu_sound(6);
    }
    buttons = r_u16(input + 4u) | r_u16(input + 8u);
    w_u8(selector + 1u, (uint8)gtmain_menu_selection(buttons, (sint8)previous,
        r_s8(selector), 0x10u, 0x40u, 0, (uint32)(sint32)r_s8(selector + 11u)));
    return -1;
}

/* GTMAIN.EXE 800367D0 */
/* Original: GTMAIN.EXE:sub_800367D0. */ sint32 gtmain_menu_angle_selector_tick(void)
{
    uint32 input = r_u32(0x800BDD08u);
    sint32 selected;
    gtmain_menu_stream_check();
    gtmain_menu_angle_tick();
    w_u8(0x801F2FE9u, 0u);
    if (!r_s8(input)) return 0;
    w_u8(0x801F2FE9u, 1u);
    selected = gtmain_menu_selector_input(0x80091BD8u, input + 2u);
    if (selected == -1) return 0;
    w_u8(0x801F2FE9u, 0u);
    gtmain_menu_sound(selected == -2 ? 2 : 4);
    return 1;
}

/* GTMAIN.EXE 80048AF8 */
/* Original: GTMAIN.EXE:sub_80048AF8. */ uint32 gtmain_menu_text_fit(uint32 text, sint32 width)
{
    sint32 remaining = (sint32)gtmain_string_length(text);
    uint32 tail = text + (uint32)remaining, changed = 0u, result;
    for (;;) {
        result = (sint32)gtmain_menu_text_width(1, text, 0) < width;
        remaining = (sint32)((uint32)remaining - 1u);
        if (result) break;
        --tail;
        if (remaining < 0) break;
        w_u8(tail, 0u);
        changed = 1u;
    }
    if (changed) return gtmain_string_append(text, 0x8008DC38u);
    return result;
}

/* GTMAIN.EXE 8003A304 */
/* Original: GTMAIN.EXE:sub_8003A304. */ sint32 gtmain_menu_stream_check(void)
{
    sint32 result = 1;
    if (r_s8(0x800B52FAu) == 1) {
        result = gtmain_stream_stop_async();
        if (!result) return gtmain_stream_pause_if_loaded();
    }
    return result;
}

/* GTMAIN.EXE 80044678 */
/* Original: GTMAIN.EXE:sub_80044678. */ uint32 gtmain_menu_any_button_accept(void)
{
    uint32 input = r_u32(0x800BDD08u);
    gtmain_menu_stream_check();
    if (!(sint8)r_u8(input)) return 0u;
    if (!(r_u16(input + 6u) & 0xFF0Fu)) return 0u;
    gtmain_menu_sound(2);
    return 1u;
}
