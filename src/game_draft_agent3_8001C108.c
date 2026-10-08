#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001C108(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v4;
  w_u32((uint32)((a0 + 4u * (uint32)(193))), (uint32)((sint32)a1));
  v4 = (sint32)((uint32)((sint32)a2) - (uint32)(1));
  if (!(sint32)a2)
    v4 = r_u32((uint32)(0x800BFF6C));
  gt_draft_8001A758((sint32)(a0), v4, 0u, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_8001A0E0(a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

