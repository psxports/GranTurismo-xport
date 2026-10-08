#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80043FE4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v8 = 0;
    uint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
  gt_draft_unresolved_call(0x8006C790u, 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  v8 = gt_draft_unresolved_call(0x80035398u, 7, a3 >> 1, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v8, 0, 0, 0, 0, 0, 0, 0);
  v9 = (uint32)gt_draft_unresolved_call(0x8006C1CCu, (sint16)r_u16(0x800B63C4u), 0, 0, 0, 0, 0, 0, 0);
  v10 = gt_draft_8003A8F4(1, v9, 0, 0, 0, 0, 0, 0);
  gt_draft_8003A8AC(a0, a1 - ((v10 >> 1) - 160), a2 + 44, v9, 0, 0, 0, 0);
  v11 = gt_draft_unresolved_call(0x8006EABCu, r_u8(0x800B63BCu), 0, 0, 0, 0, 0, 0, 0);
  v12 = gt_draft_unresolved_call(0x8003529Cu, v11, 0, 0, 0, 0, 0, 0, 0);
  if ( r_u8(0x800CB7F5u) == 5 )
    v12 = -2146708262;
  v13 = gt_draft_8003A8F4(1, (uint32)v12, 0, 0, 0, 0, 0, 0);
  return gt_draft_8003A8AC(a0, a1 - ((v13 >> 1) - 160), a2 + 52, v12, 0, 0, 0, 0);
}
