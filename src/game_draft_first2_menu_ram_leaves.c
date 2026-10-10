#include "game_first2_registry.h"

sint32 gtmain_menu_default_callback(void)
{
    return 0;
}

uint32 gtmain_menu_archive_buffer(void)
{
    return 0x800BEA40u;
}

void gtmain_menu_draw_callback_set(uint32 token)
{
    w_u32(0x800B52F4u, token);
}

sint32 gtmain_menu_effect_restart(void)
{
    w_u8(0x800B52F8u, 24u);
    return 0;
}

sint32 gtmain_menu_effect_reset(void)
{
    w_u8(0x800B52F8u, 24u);
    w_u8(0x800B52F9u, 0u);
    return 0;
}
