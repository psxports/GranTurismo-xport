#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002FF48(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  result = 1;
  if ((r_u32((uint32)(0x800B630C)) == 1))
  {
    result = (sint8)r_u8((uint32)(0x800912A8u));
    if (((sint8)r_u8((uint32)(0x800912A8u)) >= 0))
    {
      gt_draft_unresolved_call(0x8006C790u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, 8421504, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      return gt_draft_unresolved_call(0x8006CA10u, r_u32((uint32)(0x800B6368)), 16, 216, (sint32)(0u - (uint32)(2146706963)), 0u, 0u, 0u, 0u);
    }
  }
  return result;
}

