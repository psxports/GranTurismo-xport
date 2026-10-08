#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002F998(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v2;
  sint32 v3;
  v2 = gt_draft_unresolved_call(0x8006EABCu, r_u8((uint32)(0x800B63BC)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  if (gt_draft_unresolved_call(0x8008B7E8u, v2, 0x8008C7C8u, 7, 0u, 0u, 0u, 0u, 0u))
  {
    if (!gt_draft_unresolved_call(0x8008B7E8u, v2, r_u32((uint32)(0x8008C7D0u)), 3, 0u, 0u, 0u, 0u, 0u))
      v2 += 3;
  }
  else
  {
    v2 += 7;
  }
  gt_draft_unresolved_call(0x8007C014u, r_u32((uint32)(0x80091224u)), 0x8008C7D4u, (uint32)(r_u32((uint32)((0x80091190u + 4u * (uint32)((sint32)a0))))), (uint32)(0x800BDA65), 0u, 0u, 0u, 0u);
  v3 = gt_draft_unresolved_call(0x8006C1CCu, r_u32((uint32)(0x800B716C)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8007BE38u, r_u32((uint32)(0x8009126Cu)), v3, 0u, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_unresolved_call(0x8007BE38u, r_u32((uint32)(0x8009122Cu)), v2, 0u, 0u, 0u, 0u, 0u, 0u);
}

