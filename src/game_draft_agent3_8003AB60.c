#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003AB60(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  return ((((uint32)((uint32)(r_u32((uint32)(a0)))) - (uint32)(48)) < 0xA) && (r_u32((uint32)((a0 + 4u * (uint32)(1)))) == 0x2D));
}

