#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE:80034470 */
/* Original: GTMAIN.EXE:sub_80034470. */ sint32 gtmain_menu_reward_tick(void)
{
    uint32 input = r_u32(0x800BDD08u), animation;
    sint32 result = 0;
    gtmain_menu_stream_check();
    if (r_s8(input)) {
        sint32 phase = r_s16(0x801D3022u);
        if (phase == 130) {
            uint32 amount = r_u32(0x801D2FE0u);
            uint32 step = r_u32(0x801D2FF0u);
            amount -= step;
            w_u32(0x801D2FE0u, amount);
            if ((sint32)amount < 0) {
                w_u32(0x801D2FE0u, 0);
                w_u16(0x801D3022u, (uint32)phase + 1u);
                (void)gtmain_menu_sound(33);
            } else {
                uint32 ticks, increment = 1;
                (void)gtmain_menu_sound((r_u8(0x801D3024u) & 1u) + 31);
                ticks = r_u16(0x801D2FF4u) + 1u;
                w_u16(0x801D2FF4u, ticks);
                if (r_u16(input + 4u) & 0x6000u) increment = 50;
                if ((sint16)(uint16)ticks >= 11) {
                    step = r_u32(0x801D2FF0u);
                    w_u16(0x801D2FF4u, 0);
                    w_u32(0x801D2FF0u, step + increment);
                }
                if (r_u16(input + 6u) & 0x9000u) {
                    step = r_u32(0x801D2FF0u);
                    w_u32(0x801D2FE0u, step - 1u);
                }
            }
        } else if (phase == 170) {
            if (r_u16(input + 6u) & 0xFF0Fu) {
                uint32 flags = r_u8(0x801D2FDCu), descriptor;
                if (flags & 1u) descriptor = 0x80091A28u;
                else if (flags & 2u) {
                    (void)gt_draft_8003CC24(0x801D2FF8u, 0, 0x80092170u, 17, 0);
                    descriptor = 0x80092248u;
                } else descriptor = 0x80092170u;
                (void)gtmain_menu_push(descriptor);
                (void)gtmain_menu_sound(4);
                result = 3;
            }
        } else {
            if (phase == 54 || phase == 66 || phase == 78 || phase == 90)
                (void)gtmain_menu_sound(1);
            w_u16(0x801D3022u, r_u16(0x801D3022u) + 1u);
        }
    }
    animation = r_u8(0x801D3024u) + 1u;
    w_u8(0x801D3024u, animation);
    if ((uint8)animation >= 4u) w_u8(0x801D3024u, 0);
    return result;
}
