#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80044D50(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  w_u8((uint32)(0x800B5376), (uint32)(0));
  gt_draft_unresolved_call(0x80043970u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8003B87Cu, 0x80092C28u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u32((uint32)(0x80092C24u), (uint32)((sint32)(0u - (uint32)(2146709352))));
  if (((sint8)r_u8((uint32)(0x800BEA24)) == 2))
  {
    w_u32((uint32)(0x80092C24u), (uint32)((sint32)(0u - (uint32)(2146710892))));
    w_u8((uint32)(0x80092C29u), (uint32)(4));
    gt_draft_unresolved_call(0x8003B91Cu, (sint32)(0x80092C28u), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8003A27Cu, 16, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_8004AAB8(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  }
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D960u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return 0;
}

