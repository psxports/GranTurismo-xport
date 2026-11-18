#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 80030B20 */
/* Original: GTMAIN.EXE:sub_80030B20. */ sint32 gt_draft_80030B20(void)
{
    uint32 timer = r_u16(0x800912E2u);
    sint32 selected = r_s16(0x800912E0u);
    uint32 buttons = r_u16(0x800BC1C8u);
    w_u16(0x800912E2u, (uint16)(timer + 1u));
    if (buttons & 0x80u) {
        gtmain_race_sound_effect_default(0);
        w_u16(0x800912E2u, 0);
        --selected;
        if (selected == -1) selected = 1;
    }
    if (buttons & 0x20u) {
        gtmain_race_sound_effect_default(0);
        w_u16(0x800912E2u, 0);
        if (++selected >= 2) selected = 0;
    }
    w_u16(0x800912E0u, (uint16)selected);
    if (buttons & 0x6000u) return (sint16)selected;
    return buttons & 0x9000u ? -1 : -2;
}
