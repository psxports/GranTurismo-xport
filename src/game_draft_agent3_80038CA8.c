#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80038CA8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  gt_draft_unresolved_call(0x8003A304u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_unresolved_call(0x8003C28Cu, (sint32)a0, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(136)), (sint32)(0u - (uint32)(512)), 0u, 0u, 0u, 0u);
}

