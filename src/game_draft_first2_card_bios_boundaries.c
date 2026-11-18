#include "game_first2_registry.h"

void gtmain_card_native_absent_init(uint32 mode)
{
    MemCardInit((sint32)mode);
    if (!psx_memcard_mount(0, NULL, 0) || !psx_memcard_mount(16, NULL, 0))
        gt_draft_failure("Native empty memory card sockets", 0x80082698u);
}

uint32 gtmain_card_native_present(uint32 channel)
{
    sint32 command, result;
    if (!MemCardExist((sint32)channel)) return 0u;
    if (MemCardSync(0, &command, &result) != 1) return 0u;
    return result == 0 || result == 3;
}

/* Original: GTMAIN.EXE:sub_80082698. */ void gtmain_bios_card_init(uint32 mode)
{
    /* User-authorized native configuration has two empty card sockets */
    gtmain_card_native_absent_init(mode);
    (void)xport_bios_init_card(mode);
}
/* Original: GTMAIN.EXE:sub_80082878. */ void gtmain_bios_card_start(void)
{
    MemCardStart();
    (void)xport_bios_start_card();
}
/* Original: GTMAIN.EXE:sub_80082888. */ void gtmain_bios_card_stop(void)
{
    MemCardStop();
    w_u32(0x74BCu, 0u);
}
