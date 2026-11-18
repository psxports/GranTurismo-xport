#include "game_first2_registry.h"
#include <stdio.h>

/* Observed dependency GTMAIN.EXE 8001F900 */
static uint32 menu_bootstrap_finish(void)
{
    return gtmain_memory_copy(0x800BC160u, 0x80093C08u, 69u);
}

/* Original: GTMAIN.EXE:sub_800272A8. */ uint32 gtmain_menu_status_reset(void)
{
    uint32 status = r_u16(0x800CB758u), first, second;
    first = r_u32(0x80099490u); second = r_u32(0x80099494u);
    w_u32(0x800BFF80u, first); w_u32(0x800BFF84u, second);
    first = r_u32(0x80099498u); second = r_u32(0x8009949Cu);
    w_u32(0x800BFF88u, first); w_u32(0x800BFF8Cu, second);
    first = r_u32(0x800994A0u); w_u32(0x800BFF90u, first);
    if (!status) w_u16(0x800CB758u, 214);
    w_u32(0x800B6318u, 2); w_u8(0x800B6310u, 0);
    w_u8(0x800B63BFu, 0); w_u8(0x800B63BEu, 6);
    return 6;
}
/* Original: GTMAIN.EXE:sub_80027330. */ uint32 gtmain_menu_bootstrap(void)
{
    uint32 area = r_u32(0x800B1AACu), bytes = r_u32(0x800B1AA8u), end = area + bytes;
    printf("Heap: area = %08X, size = %08X, end = %08X\n", area, bytes, end);
    printf("Menu: other = %08X\n", 165680u);
    if (end > 0x801EFF00u) printf("Warning: not enough memory.\n");
    printf("GAMESTATUS: %d byte\n", 27560);
    (void)gtmain_display_mode_init(1); gtmain_display_mask(0); (void)VSync(0);
    gtmain_race_frame_reset(5); (void)gtmain_race_loading_screen();
    (void)gtmain_music_sector_init(); (void)gtmain_race_sound_volume_apply();
    gtmain_race_config_reset();
    return menu_bootstrap_finish();
}
