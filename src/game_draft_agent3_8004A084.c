#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8004A084(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v1;
  sint32 v2;
  w_u8((uint32)(0x800B53B5), (uint32)(0));
  w_u8((uint32)(0x800B53B6), (uint32)(0));
  w_u8((uint32)(0x800B53B8), (uint32)(0));
  w_u8((uint32)(0x800B53B7), (uint32)(0));
  v1 = (sint32)(0u - (uint32)(1));
  v2 = gt_draft_unresolved_call(0x8003B954u, 0x80092EBCu, (sint32)a0, 0u, 0u, 0u, 0u, 0u, 0u);
  if ((v2 == (sint32)(0u - (uint32)(2))))
    return (sint32)(0u - (uint32)(2));
  if ((v2 != (sint32)(0u - (uint32)(1))))
  {
    if (r_u8((uint32)((uint32)(((uint32)(((uint32)((sint32)((uint32)(8) * (uint32)(v2))) + (uint32)(r_u32((uint32)(0x800B53AC))))) + (uint32)(1))))))
    {
      if ((r_u8((uint32)((uint32)(((uint32)(((uint32)((sint32)((uint32)(8) * (uint32)(v2))) + (uint32)(r_u32((uint32)(0x800B53AC))))) + (uint32)(1))))) == 4))
        v1 = 64;
      else
        gt_draft_unresolved_call(0x80035778u, 6, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    else
    {
      v1 = v2;
    }
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008DCC0u, v1, 0u, 0u, 0u, 0u, 0u, 0u);
  }
  return v1;
}

