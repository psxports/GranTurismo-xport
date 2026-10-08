#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80029C70(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  gt_draft_unresolved_call(0x8002955Cu, (sint32)a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x800295A8u, (sint32)a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_unresolved_call(0x80029614u, (sint32)a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

