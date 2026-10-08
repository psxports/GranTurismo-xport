#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80044678(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 v1;
  sint32 result;
  v0 = r_u32((uint32)(0x800BDD08));
  gt_draft_unresolved_call(0x8003A304u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v1 = 0;
  if (r_u8((uint32)((uint32)(v0))))
  {
    result = 0;
    if (((r_u16((uint32)((uint32)((sint32)((uint32)(v0) + (uint32)(6))))) & 0xFF0F) == 0))
      return result;
    gt_draft_unresolved_call(0x80035778u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return 1;
  }
  return v1;
}

