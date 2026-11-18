#include "game_first2_registry.h"

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

/* Original: GTMAIN.EXE:sub_8002E494. */ uint32 gtmain_race_effect_state_clear(void)
{
    return sub_8007BD58(0x800B39C4u, 0u, 16u);
}

uint32 gtmain_bios_card_patch_copy(void)
{
    uint32 destination = 0xDF80u;
    uint32 source = 0x800826A8u;
    do {
        uint32 word = r_u32(source);
        w_u32(destination, word);
        source += 4u;
        destination += 4u;
    } while (source != 0x80082718u);
    return destination;
}
