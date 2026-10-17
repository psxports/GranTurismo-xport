#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003AD48(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  gt_draft_unresolved_call(0x800706BCu, (sint32)a1, (a2 | (sint32)((uint32)((sint32)a3) << ((uint32)(16) & 31u))), r_u32((uint32)(((uint32)(a0) + 4u * (uint32)(1)))), r_u32((uint32)(((uint32)(a0) + 4u * (uint32)(3)))), 0u, 0u, 0u, 0u);
  return gt_draft_unresolved_call(0x8006F590u, (sint32)a1, (r_u16((uint32)(a0)) | 0x220), 0u, 0u, 0u, 0u, 0u, 0u);
}

