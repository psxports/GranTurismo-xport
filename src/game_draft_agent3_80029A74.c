#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80029A74(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  return r_u32((uint32)((r_u32((uint32)((0x8008FAD8u + 4u * (uint32)(r_u8((uint32)(0x800CB7F8)))))) + 4u * (uint32)(((uint32)(3) * (uint32)(r_u8((uint32)(0x800CB7F9))))))));
}

