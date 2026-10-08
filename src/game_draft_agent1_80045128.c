#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80045128(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    sint32 v16 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
  uint32 vars0=0; /* TODO Pseudocode phantom stack local */ // [sp+20h] [+0h]
  gt_draft_unresolved_call(0x8006C790u, 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  v12 = gt_draft_unresolved_call(0x80035398u, 7, a4 >> 1, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v12, 0, 0, 0, 0, 0, 0, 0);
  v13 = gt_draft_unresolved_call(0x8006C1CCu, (sint16)r_u16(0x800B63C4u), 0, 0, 0, 0, 0, 0, 0);
  gt_draft_8003A8AC(a0, a1 + 24, a2 + 40, v13, 0, 0, 0, 0);
  v14 = r_u8(0x8009278Du);
  v15 = gt_draft_unresolved_call(0x80035458u, a4 >> 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v15, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C860u, 2, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8004B2FCu, a0, r_u32(0x80092780u), a1 + 32, a2 + 56, 0, 0, 0, 0);
  v16 = gt_draft_unresolved_call(0x80044DECu, v14, 0, 0, 0, 0, 0, 0, 0);
  v18 = r_u32(0x800B5304u);
  gt_draft_unresolved_call(0x8004BDB0u, a0, v16, a1 + 64, a2 + 56, 0, 0, 0, 0);
  if ( r_u32(0x800B5304u) )
    gt_draft_unresolved_call(0x80043F34u, a0, r_u32(r_u32(0x800928F8u + 4u * v14) + 4u * (uint32)(sint32)(sint8)r_u8(v16 + 1)), a1, a2, a4, 0, 0, 0);
  else
    gt_draft_unresolved_call(0x80043F34u, a0, (uint32)0x800BE2B0, a1, a2, a4, 0, 0, 0);
  return 0;
}
