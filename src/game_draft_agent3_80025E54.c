#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80025E54(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  w_u16((uint32)(0x800B63D6), (uint32)(7));
  result = ((uint32)(8) * (uint32)(r_u32((uint32)(0x800BF518))));
  w_u16((uint32)(0x800B63DA), (uint32)(((uint32)(8) * (uint32)(r_u32((uint32)(0x800BF518))))));
  if (!r_u32((uint32)(0x800B630C)))
  {
    gt_draft_unresolved_call(0x80021154u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return gt_draft_unresolved_call(0x80021290u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  }
  return result;
}

