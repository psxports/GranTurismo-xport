#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002C74C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v2;
  v2 = (uint32)((sint32)((uint32)((sint32)a0) + (uint32)(8)));
  if (((r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4))))) & 8) != 0))
    v2 = (uint32)(((uint32)(((uint32)((sint32)a0) + (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(8)))))))) + (uint32)(8)));
  gt_draft_unresolved_call(0x80070500u, v2, (sint32)a1, (sint32)(0u - (uint32)(1)), 0u, 0u, 0u, 0u, 0u);
  return ((uint32)(v2) + 1u * (uint32)(r_u32((uint32)(v2))));
}

