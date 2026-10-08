#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003CD50(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    sint32 v16 = 0;
    sint32 v17 = 0;
    sint32 result = 0;
    sint32 v19 = 0;
    sint32 v20 = 0;
  v12 = -2146710354;
  v13 = -2146710326;
  if ( r_u8(r_u32(0x800B52FCu) + 9) == 1 )
  {
    v12 = -2146710920;
    v13 = -2146710893;
  }
  gt_draft_unresolved_call(0x8006C790u, 3, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  v14 = gt_draft_unresolved_call(0x80035398u, r_u8(0x80092244u + 1u * ((sint8)r_u8(r_u32(0x800B52FCu) + 8))), a4 >> 1, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v14, 0, 0, 0, 0, 0, 0, 0);
  v15 = gt_draft_unresolved_call(0x8006C900u, 3, v12, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, a0, a1 - ((v15 >> 1) - 160), a2 + 96, v12, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
  v16 = gt_draft_unresolved_call(0x80035458u, a4 >> 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v16, 0, 0, 0, 0, 0, 0, 0);
  v17 = gt_draft_unresolved_call(0x8006C900u, 2, v13, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, a0, a1 - ((v17 >> 1) - 160), a2 + 124, v13, 0, 0, 0, 0);
  result = 0;
  if ( (sint8)r_u32(r_u32(0x800B52FCu)) >= 60 )
  {
    gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
    v19 = gt_draft_unresolved_call(0x8003553Cu, a4 >> 1, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006C874u, v19, 0, 0, 0, 0, 0, 0, 0);
    v20 = gt_draft_unresolved_call(0x8006C900u, 2, -2146710516, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006CA10u, a0, a1 - ((v20 >> 1) - 160), a2 + 204, -2146710516, 0, 0, 0, 0);
    return 0;
  }
  return result;
}
