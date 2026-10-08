#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80023DCC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 draft_buffer_v9[3];
  sint32 v2;
  uint32 i;
  uint32 v5;
  sint32 v6;
  sint32 v7;
  sint32 result;
  v2 = (sint32)a0;
  for (i = r_u32((uint32)((uint32)(((uint32)(((uint32)((sint32)((uint32)(4) * (uint32)((sint32)a0))) + (uint32)(r_u32((uint32)(0x800BC1A8))))) + (uint32)(12)))));; i = (uint32)(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(1))))))
  {
    draft_buffer_v9[0] = ((uint32)(r_u32((uint32)(a1))) - (uint32)(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(6))))));
    draft_buffer_v9[1] = ((uint32)(r_u32((uint32)((a1 + 4u * (uint32)(1))))) - (uint32)(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(7))))));
    draft_buffer_v9[2] = ((uint32)(r_u32((uint32)((a1 + 4u * (uint32)(2))))) - (uint32)(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(8))))));
    if ((gt_draft_unresolved_call(0x8006E9CCu, gt_draft_buffer_address(&draft_buffer_v9[0], sizeof(draft_buffer_v9)), (i + 2u * (uint32)(18)), 0u, 0u, 0u, 0u, 0u, 0u) >= 0))
      break;
    if (!(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(1))))))
      return v2;
    v2 = r_u16((uint32)(i));
  }

  while (1)
  {
    v5 = (uint32)(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(2)))));
    if (!v5)
      break;
    v6 = r_u16((uint32)((i + 2u * (uint32)(1))));
    i = (uint32)(r_u32((uint32)(((uint32)(i) + 4u * (uint32)(2)))));
    draft_buffer_v9[0] = ((uint32)(r_u32((uint32)(a1))) - (uint32)(r_u32((uint32)((v5 + 4u * (uint32)(6))))));
    draft_buffer_v9[1] = ((uint32)(r_u32((uint32)((a1 + 4u * (uint32)(1))))) - (uint32)(r_u32((uint32)((v5 + 4u * (uint32)(7))))));
    draft_buffer_v9[2] = ((uint32)(r_u32((uint32)((a1 + 4u * (uint32)(2))))) - (uint32)(r_u32((uint32)((v5 + 4u * (uint32)(8))))));
    v7 = (gt_draft_unresolved_call(0x8006E9CCu, gt_draft_buffer_address(&draft_buffer_v9[0], sizeof(draft_buffer_v9)), (v5 + 4u * (uint32)(9)), 0u, 0u, 0u, 0u, 0u, 0u) < 0);
    result = v2;
    if (v7)
      return result;
    v2 = v6;
  }

  return v2;
}

