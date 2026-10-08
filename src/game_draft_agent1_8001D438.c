#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001D438(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v1 = 0;
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 result = 0;
  v1 = r_u32(0x800B6400u);
  gt_draft_8001DF30(a0, r_u32(r_u32(0x800B6400u) + 16), 0, 0, 0, 0, 0, 0);
  v3 = r_u32(v1 + 24);
  v4 = r_u32(v1 + 28);
    w_u32(a0 + 120, r_u32(v1 + 20));
    w_u32(a0 + 124, v3);
    w_u32(a0 + 128, v4);
  gt_draft_unresolved_call(0x8006B134u, a0 + 140, (sint16)r_u16(v1 + 34), (sint16)r_u16(v1 + 32), (sint16)r_u16(v1 + 36), 0, 0, 0, 0);
  result = gt_draft_80023DCC(r_u16(v1 + 6), a0 + 120, 0, 0, 0, 0, 0, 0);
    w_u16(a0 + 244, result);
  return result;
}
