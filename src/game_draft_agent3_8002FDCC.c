#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002FDCC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v3;
  v3 = r_u32((uint32)(0x800B6368));
  gt_draft_unresolved_call(0x8006C874u, 128, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_unresolved_call(0x8006C6C8u, v3, 120, 66, a1, 0u, 0u, 0u, 0u);
}

