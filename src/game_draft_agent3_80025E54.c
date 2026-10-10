#include "game_first2_registry.h"

/* GTMAIN.EXE 80025E54 */
void gtmain_race_mode0_prepare(void)
{
    uint32 count, mode;
    w_u16(0x800B63D6u, 7u);
    count = r_u32(0x800BF518u);
    mode = r_u32(0x800B630Cu);
    w_u16(0x800B63DAu, count << 3);
    if (!mode) {
        gtmain_race_rank_list_sort();
        gtmain_race_player_positions();
    }
}
