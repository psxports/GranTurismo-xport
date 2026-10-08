#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002A270(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
  v0 = r_u32(0x800B6368u);
  gt_draft_80030828(r_u32(0x800B6368u), 68, -1, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8002D8F8u, v0, 16, 16, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8002DA30u, v0, 16, 38, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8002E32Cu, 0, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_8002C318(v0, 238, 150, -2146736048, 0, 0, 0, 0);
  return gt_draft_8002FAA0(v0, 68, -2146736048, 0, 0, 0, 0, 0);
}
