#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8004AB30 */
uint32 gt_draft_8004AB30(sint32 category, uint16 part)
{
    uint32 offset = r_u8(0x800941BDu + (uint32)category);
    uint32 context = r_u32(0x800BEA64u), result;
    w_u16(context + offset, part);
    if (category == 27) offset = r_u8(0x800941E0u);
    else if (category == 35) offset = r_u8(0x800941D8u);
    else return 35u;
    context = r_u32(0x800BEA64u);
    result = context + offset;
    w_u16(result, 0u);
    return result;
}
