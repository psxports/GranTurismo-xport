#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800308D0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{

  gt_draft_8003011C(a0, 12, 8421504, a1, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F4F0u, a0, 0x800912D0u, a2, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8006F4F0u, a0, r_u32(0x800912D8u), 0, 0, 0, 0, 0, 0);
}
