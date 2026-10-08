#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002A0D4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 result = 0;
  v0 = r_u32(gt_draft_80029A74(0, 0, 0, 0, 0, 0, 0, 0) + 20u);
  gt_draft_unresolved_call(0x80029740u, r_u32(0x800B6368u), -2146707413, -2146707111, v0, 0, 0, 0, 0);
  result = gt_draft_unresolved_call(0x800298B4u, v0, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x8008F890u, result);
  return result;
}
