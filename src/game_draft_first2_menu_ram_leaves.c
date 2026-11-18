#include "game_first2_registry.h"

/* Shared zero-result callbacks */
sint32 gtmain_menu_default_callback(void)
{
    /* Original occurrences
     * GTMAIN.EXE:0x80035D88
     * GTMAIN.EXE:0x8003A3AC
     * GTMAIN.EXE:0x8003A41C
     * GTMAIN.EXE:0x80044B20
     * GTMAIN.EXE:0x8004A990
     * GTMENU.EXE:0x800185F0
     * GTMENU.EXE:0x800185F8
     */
    return 0;
}

/* Original: GTMAIN.EXE:sub_80048558. */ uint32 gtmain_menu_archive_buffer(void)
{
    return 0x800BEA40u;
}

/* Original: GTMAIN.EXE:sub_8003A258. */ void gtmain_menu_draw_callback_set(uint32 token)
{
    w_u32(0x800B52F4u, token);
}

/* Original: GTMAIN.EXE:sub_8003A424. */ sint32 gtmain_menu_effect_restart(void)
{
    w_u8(0x800B52F8u, 24u);
    return 0;
}

/* Original: GTMAIN.EXE:sub_8003A3B4. */ sint32 gtmain_menu_effect_reset(void)
{
    w_u8(0x800B52F8u, 24u);
    w_u8(0x800B52F9u, 0u);
    return 0;
}
