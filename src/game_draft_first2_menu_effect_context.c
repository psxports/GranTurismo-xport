#include "game_first2_registry.h"

/* GTMAIN.EXE 8003A6A4 */
sint32 gtmain_menu_effect_context_bind(uint32 context, uint32 descriptor)
{
    gtmain_menu_callback(r_u32(descriptor + 16u));
    w_u8(0x800BDE10u, 0);
    w_u32(context + 192u, descriptor);
    w_u16(0x800BDE1Au, 320);
    w_u16(0x800BDE1Cu, 208);
    w_u16(0x800BDE1Eu, 20);
    w_u8(0x800BDE22u, 0);
    w_u16(0x800BDE20u, 13);
    return 13;
}
