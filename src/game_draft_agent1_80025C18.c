#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
/* Original: GTMAIN.EXE:sub_80025C18. */ uint32 gt_draft_80025C18(void)
{
    uint32 record = 0x800CAE3Cu + 40u * r_u8(0x800B63BDu);
    uint32 split = r_u32(record + 24u);
    uint32 best = r_u32(record + 4u);
    w_u32(0x800BDB4Cu, split);
    w_u32(0x800BDB48u, best);
    return best;
}
