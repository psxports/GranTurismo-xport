#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_80032164. */ uint32 gtmain_menu_phase_tick(void)
{
    uint32 previous = r_u16(0x801D2FE8u);
    uint32 next = previous + 10u, result = previous - 4086u;
    w_u16(0x801D2FE8u, next);
    if ((sint16)(uint16)next >= 4096) w_u16(0x801D2FE8u, result);
    return result;
}
/* Original: GTMAIN.EXE:sub_80032460. */ uint32 gtmain_menu_counters_tick(void)
{
    sint32 value = (sint16)r_u16(0x801D2FDEu);
    uint32 phase;
    if (value > 0) w_u16(0x801D2FDEu, (uint32)value - 1u);
    value = (sint16)r_u16(0x801D2FDCu);
    if (value > 0) w_u16(0x801D2FDCu, (uint32)value - 1u);
    phase = (r_u8(0x801D2FEAu) + 1u) & 3u;
    w_u8(0x801D2FEAu, phase);
    return phase;
}
/* Original: GTMAIN.EXE:sub_800324C4. */ sint32 gtmain_menu_descriptor_lookup(uint32 guest_descriptor, sint32 selector, uint32 guest_value_out)
{
    uint32 value_cursor = guest_descriptor, result_cursor = guest_descriptor;
    sint32 key = (sint8)r_u8(value_cursor + 2u);
    if (key < 0) return -256;
    key = (sint8)r_u8(value_cursor + 2u);
    for (;;) {
        if (selector == key) {
            uint32 value = r_u8(value_cursor + 26u);
            w_u32(guest_value_out, value);
            return (sint16)r_u16(result_cursor + 98u);
        }
        value_cursor += 1u;
        key = (sint8)r_u8(value_cursor + 2u);
        result_cursor += 2u;
        if (key < 0) return -256;
    }
}
