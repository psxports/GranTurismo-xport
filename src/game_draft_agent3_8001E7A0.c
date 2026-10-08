#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001E7A0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  gt_draft_unresolved_call(0x8007BDFCu, (sint32)((uint32)((sint32)((uint32)(96) * (uint32)((sint32)a0))) - (uint32)(2146715760)), (sint32)((uint32)((sint32)((uint32)(96) * (uint32)((sint32)a1))) - (uint32)(2146677268)), 96, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_8001E6D4((sint32)a0, (sint32)a2, 0u, 0u, 0u, 0u, 0u, 0u);
}

