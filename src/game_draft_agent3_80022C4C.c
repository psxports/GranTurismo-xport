#include "game_first2_registry.h"

static uint32 result_record_missing(void)
{
    /* TODO Bind unselected no-argument result record selector */
    gt_draft_failure("GTMAIN result record selector", 0x8002278Cu);
}

static uint32 result_mode5_missing(uint8 flags, sint16 index, uint32 unused,
    uint32 alternate, uint32 record, uint32 metric, sint16 threshold,
    uint32 extra, uint8 panel)
{
    /* TODO Bind unselected nine-argument result mode wrapper */
    gt_draft_failure("GTMAIN result mode5 wrapper", 0x80033FF8u);
}

/* GTMAIN.EXE:80022C4C */
uint32 gt_draft_80022C4C(uint32 alternate)
{
    uint32 object = result_record_missing();
    uint32 second = object + (alternate ? 56u : 28u);
    uint8 flags = 0u;
    sint16 threshold, index;
    uint32 extra;
    if ((sint8)r_u8(0x800BDD01u)) {
        w_u32(second + 12u, r_u32(0x800BDB4Cu));
        flags = 2u;
        w_u16(second + 16u, r_u16(0x800B6670u));
    }
    if ((sint8)r_u8(0x800BDD00u)) {
        w_u32(object + 12u, r_u32(0x800BDB48u));
        flags |= 1u;
        w_u16(object + 16u, r_u16(0x800B6670u));
    }
    threshold = (sint16)r_u16(0x800B634Cu);
    extra = r_u32(0x800CB8E0u);
    index = (sint8)r_u8(0x800B63D2u);
    return result_mode5_missing(flags, index, 0u, alternate, object,
        0x800BDB70u, threshold, extra, 0u);
}
