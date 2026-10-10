#include "game_first2_registry.h"

/* GTMAIN.EXE 80029A74 */
uint32 gtmain_race_mode_record(void)
{
    uint32 entry = r_u8(0x800CB7F9u);
    uint32 group = r_u8(0x800CB7F8u);
    uint32 table = r_u32(0x8008FAD8u + (group << 2));
    return table + entry * 24u;
}
