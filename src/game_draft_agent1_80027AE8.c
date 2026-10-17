#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80027AE8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 result = 0;
  v0 = gt_draft_unresolved_call(0x8001E4A8u, r_u32(0x800B6348u) << 12, (unsigned __int64)r_u32(0x800B6348u) >> 20, 600000i64, 0, 0, 0, 0, 0);
  result = v0 < 4096;
  if ( v0 >= 4096 )
    v0 = (v0 & 0xFFFF0000u) | (((uint32)(4096) & 0xFFFFu) << 0);
    w_u16(0x800BB7EAu, v0);
  return result;
}
