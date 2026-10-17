#include "game_first2_registry.h"

static void selection_pose_reset(uint32 state)
{
    /* TODO Bind the unselected pose initializer with its real state argument */
    gt_draft_unresolved_call(0x8004D7F8u, state, 0, 0, 0, 0, 0, 0, 0);
}

static void selection_record_copy(uint32 source, uint32 first, uint32 second)
{
    /* TODO Bind the unselected record copy with both guest destinations */
    gt_draft_unresolved_call(0x80058BB0u, source, first, second, 0, 0, 0, 0, 0);
}

static void selection_pose_bind(uint32 state, uint32 first, uint32 second)
{
    /* TODO Bind the unselected pose setup with both copied record addresses */
    gt_draft_unresolved_call(0x8004D78Cu, state, first, second, 0, 0, 0, 0, 0);
}

/* GTMAIN.EXE 800461A0 */
uint32 gtmain_menu_selection_snapshot_initialize(void)
{
    uint32 mode = r_u8(0x800B5374u);
    if (mode == 27u || mode == 35u) {
        sint32 count = r_s8(0x80093530u), i;
        w_u16(0x801D2FDEu, (uint16)count);
        for (i = 0; i < count; ++i) {
            uint32 rows;
            w_u8(0x801D2FE0u + (uint32)i, 0u);
            rows = r_u32(0x80093534u);
            if (!(r_u8(rows + 8u * (uint32)i) & 0x40u))
                w_u8(0x801D2FE0u + (uint32)i, 1u);
        }
        w_u16(0x801D2FDCu, 0u);
        selection_pose_reset(0x801D2FE8u);
        selection_record_copy(0x800BB850u, 0x801D3644u, 0x801D35D8u);
        selection_pose_bind(0x801D2FE8u, 0x801D3644u, 0x801D35D8u);
    } else {
        w_u16(0x801D2FDCu, 0xFFFFu);
    }
    gtmain_menu_selector_reset(0x80093530u);
    return 0u;
}
