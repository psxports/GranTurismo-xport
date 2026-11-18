#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 80044D50 */
/* Original: GTMAIN.EXE:sub_80044D50. */ uint32 gtmain_menu_settings_initialize(void)
{
    sint32 mode;
    w_u8(0x800B5376u, 0u);
    gtmain_parts_settings_bind();
    gtmain_menu_selector_reset(0x80092C28u);
    mode = (sint8)r_u8(0x800BEA24u);
    w_u32(0x80092C24u, 0x800BD098u);
    if (mode == 2) {
        w_u32(0x80092C24u, 0x800BCA94u);
        w_u8(0x80092C29u, 4u);
        gtmain_menu_state_capture(0x80092C28u);
        (void)gtmain_menu_asset_activate(16u);
        (void)gtmain_menu_settings_masks_update();
    }
    printf((const char *)psx_addr(0x8008D960, 15));
    return 0u;
}
