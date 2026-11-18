#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
/* Original: GTMAIN.EXE:sub_800433E0. */ uint32 gt_draft_800433E0(void)
{
    uint32 object = r_u32(0x800BEA64u);
    uint32 value = r_u16(0x8009298Cu);
    w_u8(object + 68u, value);
    object = r_u32(0x800BEA64u);
    value = r_u16(0x8009299Cu);
    w_u8(object + 81u, value);
    return value;
}

