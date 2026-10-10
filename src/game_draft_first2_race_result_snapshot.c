#include "game_first2_registry.h"

/* GTMAIN.EXE 8002B078 */
uint32 gtmain_race_result_snapshot_store(void)
{
    uint32 result = r_u32(0x800B630Cu);
    if (!result) {
        w_u32(0x800CB8E0u, r_u32(0x800B6870u));
        gtmain_memory_copy(0x800CB8E4u, 0x800BDB70u, 400u);
        result = r_u32(0x800B7028u);
        w_u32(0x800B6348u, result);
    }
    return result;
}
