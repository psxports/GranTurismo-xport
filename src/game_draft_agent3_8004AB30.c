#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8004AB30(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  sint32 v3;
  w_u16((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)((uint8)(r_u32((uint32)((0x800941BDu + 1u * (uint32)((sint32)a0))))))))), (uint32)((sint16)a1));
  if (((sint32)a0 == 27))
  {
    v3 = (uint8)(r_u8((uint32)(0x800941E0u)));
  }
  else
  {
    result = 35;
    if (((sint32)a0 != 35))
      return result;
    v3 = (uint8)(r_u8((uint32)(0x800941D8u)));
  }
  result = ((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(v3));
  w_u16((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(v3)))), (uint32)(0));
  return result;
}

