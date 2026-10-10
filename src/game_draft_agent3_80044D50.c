#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

uint32 gtmain_menu_settings_initialize(void)
{
    sint32 mode;
    w_u8(0x800B5376u, 0);
    /* TODO Bind unselected setting initializer */
    gt_draft_unresolved_call(0x80043970u, 0, 0, 0, 0, 0, 0, 0, 0);
    gtmain_menu_selector_reset(0x80092C28u);
    mode = (sint8)r_u8(0x800BEA24u);
    w_u32(0x80092C24u, 0x800BD098u);
    if (mode == 2)
    {
        w_u32(0x80092C24u, 0x800BCA94u);
        w_u8(0x80092C29u, 4);
        gtmain_menu_state_capture(0x80092C28u);
        gtmain_menu_asset_activate(16);
        gtmain_menu_settings_masks_update();
    }
    fputs((const char *)psx_addr(0x8008D960u, 15u), stdout);
    return 0;
}
