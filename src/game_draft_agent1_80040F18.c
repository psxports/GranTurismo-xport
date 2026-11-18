#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* GTMAIN.EXE 80040F18 */
/* Original: GTMAIN.EXE:sub_80040F18. */ sint32 gtmain_menu_parameter_selector_input(uint32 selector, uint32 input)
{
    uint32 mode = r_u8(selector + 12u) & 31u;
    uint32 selected = r_u8(selector + 1u);
    sint8 animation = r_s8(selector + 14u);
    uint32 rows = r_u32(selector + 8u);
    uint16 buttons;
    uint32 backward, forward;
    sint8 previous;
    if (animation) {
        uint8 timer = (uint8)(r_u8(selector + 15u) + 1u);
        w_u8(selector + 15u, timer);
        if ((sint8)timer >= 12) {
            w_u8(selector + 14u, 0u);
            w_u8(selector + 15u, 0u);
        }
        return -1;
    }
    w_u8(selector + 13u, selected);
    buttons = r_u16(input + 4u);
    if (buttons & 0x6000u) return (sint8)selected;
    if (buttons & 0x9000u) return -2;
    buttons |= r_u16(input + 8u);
    backward = buttons & (mode == 1u ? 0x10u : 0x80u);
    forward = buttons & (mode == 1u ? 0x40u : 0x20u);
    if (backward && forward) return -1;
    if (backward) {
        gtmain_race_sound_effect(1, 0x4000, 0x40, 0);
        if (mode < 2u) w_u8(selector + 14u, mode ? 3u : 1u);
        do {
            --selected;
            if ((sint8)selected < 0) selected = r_u8(selector) - 1u;
        } while (r_u8(rows + 12u * (uint32)(sint32)(sint8)selected) & 0x40u);
    } else if (forward) {
        sint8 count;
        gtmain_race_sound_effect(1, 0x4000, 0x40, 0);
        if (mode < 2u) {
            w_u8(selector + 14u, mode ? 4u : 2u);
            w_u8(selector + 15u, 0u);
        }
        count = r_s8(selector);
        do {
            ++selected;
            if ((sint8)selected >= count) selected = 0u;
        } while (r_u8(rows + 12u * (uint32)(sint32)(sint8)selected) & 0x40u);
    }
    previous = r_s8(selector + 13u);
    w_u8(selector + 1u, selected);
    return previous == (sint8)selected ? -1 : -3;
}

/* Preserve existing legacy callers during ABI migration */
uint32 gt_draft_80040F18(uint32 a0, uint32 a1, uint32 a2, uint32 a3,
    uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7;
    return (uint32)gtmain_menu_parameter_selector_input(a0, a1);
}
