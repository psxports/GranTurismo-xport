#include "game_first2_registry.h"
#include "xport_trace.h"

/* GTMAIN.EXE 80032EE4 */
/* Original: GTMAIN.EXE:sub_80032EE4. */ uint32 gtmain_menu_result_plateau_tick(void)
{
    uint32 input, result = 0u;
    sint32 phase;
    FUNCTION_MARKER(0x80032EE4, "GTMAIN.EXE");
    input = r_u32(0x800BDD08u);
    gtmain_menu_phase_tick();
    gtmain_menu_stream_check();
    phase = r_s16(0x801D2FE4u);
    if (phase == 112) {
        uint16 next;
        gtmain_menu_sound(1);
        next = r_u16(0x801D2FE4u);
        w_u16(0x801D2FDCu, 6u);
        w_u16(0x801D2FE4u, next + 1u);
    } else if (phase == 118) {
        gtmain_menu_selector_layout(0x801D3AECu);
        if (r_u16(input + 6u) & 0xFF0Fu) {
            gtmain_menu_sound(4);
            result = 3u;
            gtmain_menu_push(0x80092170u);
        }
    } else {
        if (phase == 100) gtmain_menu_sound(1);
        w_u16(0x801D2FE4u, r_u16(0x801D2FE4u) + 1u);
    }
    gtmain_menu_counters_tick();
    return result;
}

/* GTMAIN.EXE 80032FF8 */
/* Original: GTMAIN.EXE:sub_80032FF8. */ uint32 gtmain_menu_result_plateau_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    sint32 center = (sint32)((uint32)x + 160u);
    gtmain_text_font(1);
    gtmain_text_flags(0u);
    gtmain_menu_footer_draw(ordering, center, (sint32)((uint32)y + 48u), intensity);
    gtmain_text_font(2);
    gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
    if (r_s16(0x801D2FE4u) >= 100) {
        sint32 row = r_s16(0x801D2FE4u) - 100;
        gtmain_menu_row_transition(ordering, (sint32)((uint32)x + 32u),
            (sint32)((uint32)y + 80u), 0x800BC9FBu, intensity, row, 3);
    }
    if (r_s16(0x801D2FDCu) >= 0) {
        sint32 row = r_s16(0x801D2FDCu);
        gtmain_menu_row_draw(ordering, (sint32)((uint32)x + 148u),
            (sint32)((uint32)y + 96u), 0x801D3118u, intensity, row, 0u, -1, 0u);
    }
    {
        sint32 phase = r_s16(0x801D2FE8u);
        uint32 descriptor = r_u8(0x801D2FEDu);
        gtmain_menu_indicator_draw(ordering + 8u, center, (sint32)((uint32)y + 136u),
            phase, 0x276u, descriptor, 3u, 2u);
    }
    return 0u;
}

/* GTMAIN.EXE 800331FC */
/* Original: GTMAIN.EXE:sub_800331FC. */ uint32 gtmain_menu_result_reveal_tick(void)
{
    uint32 input = r_u32(0x800BDD08u), result = 0u, phase;
    gtmain_menu_phase_tick();
    gtmain_menu_stream_check();
    phase = r_u16(0x801D2FE4u);
    switch (phase) {
    case 54u:
    case 78u:
        gtmain_menu_sound(1);
        w_u16(0x801D2FE4u, r_u16(0x801D2FE4u) + 1u);
        break;
    case 66u:
        gtmain_menu_sound(1);
        phase = r_u16(0x801D2FE4u);
        w_u16(0x801D2FDCu, 6u);
        w_u16(0x801D2FE4u, phase + 1u);
        break;
    case 90u: {
        uint32 first, second;
        gtmain_menu_sound(1);
        w_u16(0x801D2FDEu, 6u);
        first = r_u16(0x801D2FE2u);
        second = r_u16(0x801D2FE4u);
        w_u16(0x801D2FE2u, first + 1u);
        w_u16(0x801D2FE4u, second + 1u);
        break;
    }
    case 103u:
        if (r_u16(input + 6u) & 0xFF0Fu) {
            uint32 descriptor;
            gtmain_menu_sound(4);
            descriptor = (sint16)r_u16(0x801D2FEEu) < 0 ? 0x80092170u : 0x800919D0u;
            result = 3u;
            gtmain_menu_push(descriptor);
        }
        break;
    default:
        w_u16(0x801D2FE4u, r_u16(0x801D2FE4u) + 1u);
        break;
    }
    gtmain_menu_counters_tick();
    return result;
}
