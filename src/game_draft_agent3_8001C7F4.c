#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Continued from the existing camera or packet translation without the guest save frame */
/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001C7F4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  sint32 v4;
  result = 2;
  if ((r_u8((uint32)(0x800CB7F5)) != 2))
  {
    v4 = 0;
    if ((((sint8)a1 & 0x10) != 0))
      v4 = (sint32)(0u - (uint32)(1));
    result = ((sint8)a1 & 0x40);
    if ((((sint8)a1 & 0x40) != 0))
      (v4 += 1u);
    if (v4)
    {
      result = gt_draft_unresolved_call(0x80021F80u, (sint32)((uint32)((sint8)r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))) + (uint32)(1737)))))) + (uint32)(v4)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      if (result)
        w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))), (uint32)(result));
    }
  }
  return result;
}

