#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80030974(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v13 = 0;
  gt_draft_unresolved_call(0x8006C790u, 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, 8421504, 0, 0, 0, 0, 0, 0, 0);
  v13 = gt_draft_unresolved_call(0x8006C900u, 1, a2, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, a0, a4 + ((sint16)r_u16(a4 + 2u * (2)) - (sint16)r_u16(a4) - v13) / 2, (sint16)r_u16(a4 + 2u * (1)) + 4, a2, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F624u, a0, a4, 4210752, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8006F4F0u, a0, a4, a1, 0, 0, 0, 0, 0);
}
