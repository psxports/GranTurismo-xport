#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002FE20(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v2;
  sint32 v3;
  sint32 v4;
  sint32 v5;
  sint8 v6;
  sint32 v7;
  sint32 result;
  v2 = 8421504;
  v3 = 8421504;
  v4 = r_u32((uint32)(a0));
  v5 = r_u32((uint32)(0x800B6368));
  v6 = r_u32((uint32)(a0));
  if ((v4 == 43))
  {
    v2 = 32896;
    v3 = 0x8000;
    v6 = 45;
  }
  else
    if ((v4 == 45))
  {
    v2 = 0x8000;
    v3 = 32896;
    v6 = 43;
  }
  gt_draft_unresolved_call(0x8006C790u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C874u, v2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  if ((sint8)r_u8((uint32)(0x800B633C)))
  {
    gt_draft_unresolved_call(0x8006C720u, v5, 120, 82, a0, 0u, 0u, 0u, 0u);
    w_u32((uint32)(a0), (uint32)(v6));
    gt_draft_unresolved_call(0x8006C874u, v3, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    result = gt_draft_unresolved_call(0x8006C720u, v5, 120, 190, a0, 0u, 0u, 0u, 0u);
    w_u32((uint32)(a0), (uint32)(v4));
  }
  else
  {
    v7 = 82;
    if ((sint16)r_u16((uint32)(0x800B63D6)))
      v7 = 106;
    return gt_draft_unresolved_call(0x8006C720u, v5, 120, v7, a0, 0u, 0u, 0u, 0u);
  }
  return result;
}

