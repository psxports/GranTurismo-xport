#include "game_draft_registry.h"

/* GTMAIN.EXE:8003CC24 */
uint32 gt_draft_8003CC24(uint32 request, uint32 selection, uint32 target, uint32 style, uint32 flags)
{
    uint32 result;
    w_u32(0x800B52FCu, request);
    w_u8(request + 1u, selection);
    result = r_u32(0x800B52FCu);
    w_u32(result + 4u, target);
    w_u8(result + 9u, flags);
    w_u8(0x80092260u, style);
    return result;
}
