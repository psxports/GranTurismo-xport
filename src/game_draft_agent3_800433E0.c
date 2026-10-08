#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_800433E0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(68)))), (uint32)(r_u16((uint32)(0x8009298Cu))));
  result = (uint16)(r_u16((uint32)(0x8009299Cu)));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(81)))), (uint32)(r_u16((uint32)(0x8009299Cu))));
  return result;
}

