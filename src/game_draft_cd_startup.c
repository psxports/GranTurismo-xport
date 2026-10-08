#include "game_draft_registry.h"

uint32 gtmain_cd_init(void)
{
    uint32 attempt;
    for (attempt = 0u; attempt < 5u; ++attempt) {
        if (CdReset(1) == 1) {
            CdSyncCallbackPSX(0x8008317Cu);
            CdReadyCallbackPSX(0x800831A4u);
            /* Original data callback and debug-level storage */
            w_u32(0x800B1FE4u, 0x800831CCu);
            w_u32(0x800B2018u, 0u);
            return 1u;
        }
    }
    /* TODO Map the SDK diagnostic print on the original failure path */
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008EBA8u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return 0u;
}
