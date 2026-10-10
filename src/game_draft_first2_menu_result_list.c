#include "game_first2_registry.h"
#include "game_draft_registry.h"

static uint32 result_list_clamp(uint32 list)
{
    /* TODO Unselected list clamp remains a typed fail-fast dependency */
    gt_draft_failure("GTMAIN result list clamp", 0x8003B88Cu);
}

/* GTMAIN.EXE:800325C4 */
sint32 gtmain_menu_result_list_tick(void)
{
    uint32 input = r_u32(0x800BDD08u);
    sint32 result = 0;
    uint32 phase;
    (void)gtmain_menu_phase_tick();
    /* TODO Stream pause dependencies remain partial */
    gtmain_menu_stream_check();
    phase = r_u16(0x801D2FE4u);
    switch (phase) {
    case 54: {
        sint32 mode = r_s8(0x801D2FECu);
        if (mode >= 7 && mode < 9) w_u16(0x801D2FE4u, 78);
        (void)gtmain_menu_sound(1);
        goto capture_and_advance;
    }
    case 78:
    case 102:
        (void)gtmain_menu_sound(1);
        goto capture_and_advance;
    case 66:
        (void)gtmain_menu_sound(1);
        phase = r_u16(0x801D2FE4u);
        w_u16(0x801D2FE0u, 6);
        w_u16(0x801D2FE4u, phase + 1u);
        break;
    case 90:
        (void)gtmain_menu_sound(1);
        phase = r_u16(0x801D2FE4u);
        w_u16(0x801D2FDCu, 6);
        w_u16(0x801D2FE4u, phase + 1u);
        break;
    case 114: {
        uint32 count;
        sint32 signed_count, step = 1;
        (void)gtmain_menu_sound(1);
        count = r_u16(0x801D2FE2u);
        w_u16(0x801D2FDEu, 6);
        count += 1u;
        w_u16(0x801D2FE2u, count);
        signed_count = (sint16)(uint16)count;
        if (signed_count >= 3) {
            step = (signed_count - 2) / 2 + 1;
            if (r_s16(0x801D2FE4u) + step >= 126) {
                w_u16(0x801D2FE4u, 126);
                step = 0;
            }
        }
        phase = r_u16(0x801D2FE4u);
        w_u16(0x801D2FE4u, phase + (uint32)step);
        break;
    }
    case 126: {
        sint32 count = r_s16(0x801D2FE2u);
        sint32 limit = r_s16(0x801D3000u);
        if (count < limit) {
            count = r_s16(0x801D2FE2u);
            w_u16(0x801D2FE4u, 114);
            (void)gtmain_menu_list_select(0x801D3AECu, count - 1);
        } else w_u16(0x801D2FE4u, 127);
        break;
    }
    case 127:
        if (r_s16(0x801D3000u) >= 4) {
            uint32 buttons = r_u16(input + 6u);
            uint32 repeat = r_u16(input + 10u);
            sint32 row = r_s8(0x801D3AEDu);
            buttons |= repeat;
            if (buttons & 0x400u) {
                (void)gtmain_menu_sound(0);
                (void)gtmain_menu_list_select(0x801D3AECu, row - 10);
            } else if (buttons & 0x800u) {
                (void)gtmain_menu_sound(0);
                (void)gtmain_menu_list_select(0x801D3AECu, row + 10);
            } else (void)gtmain_menu_selector_input(0x801D3AECu, input + 2u);
        }
        (void)result_list_clamp(0x801D3AECu);
        if (r_u16(input + 6u) & 0xFF0Fu) {
            sint32 mode;
            uint32 descriptor = 0x80092170u;
            (void)gtmain_menu_sound(4);
            mode = r_s8(0x801D2FECu);
            if (mode != 5) {
                if (mode == 1 || mode == 6) {
                    sint32 index = r_s16(0x801D2FEEu);
                    uint32 flags = r_u8(0x801D2FEDu);
                    if (index >= 0 || flags) descriptor = 0x800919D0u;
                } else if (r_u8(0x801D2FEDu)) descriptor = 0x800919D0u;
            }
            result = 3;
            (void)gtmain_menu_push(descriptor);
        }
        break;
    default:
capture_and_advance:
        (void)gtmain_menu_state_capture(0x801D3AECu);
        phase = r_u16(0x801D2FE4u);
        w_u16(0x801D2FE4u, phase + 1u);
        break;
    }
    (void)gtmain_menu_counters_tick();
    return result;
}
