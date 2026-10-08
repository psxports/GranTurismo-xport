#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003CC24(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  w_u32((uint32)(0x800B52FC), (uint32)((sint32)a0));
  w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(1)))), (uint32)((sint8)a1));
  result = r_u32((uint32)(0x800B52FC));
  w_u32((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800B52FC))) + (uint32)(4)))), (uint32)((sint32)a2));
  w_u8((uint32)((uint32)((sint32)((uint32)(result) + (uint32)(9)))), (uint32)((sint8)a4));
  w_u8((uint32)(0x80092260u), (uint32)((sint8)a3));
  return result;
}

