#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80029CA4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 i;
  sint32 v2;
  sint32 v3;
  sint32 j;
  sint32 v5;
  sint32 v6;
  sint32 k;
  sint32 v8;
  sint32 result;
  gt_draft_unresolved_call(0x80029B7Cu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v0 = 0;
  for (i = 0; (i < 8); (i += 1u))
  {
    v2 = (sint32)((uint32)(2) * (uint32)(i));
    w_u32((uint32)((0x800B3920u + 4u * (uint32)(((v0 += 1u) - 1u)))), (uint32)(((uint32)(((uint32)(r_u16((uint32)((uint32)((sint32)((uint32)(v2) - (uint32)(2145792036)))))) - (uint32)(2145792035))) + (uint32)(2))));
  }

  w_u32((uint32)((0x800B3920u + 4u * (uint32)(v0))), (uint32)(0));
  v3 = (sint32)((uint32)(v0) + (uint32)(1));
  for (j = 0; (j < 8); (j += 1u))
  {
    v5 = (sint32)((uint32)(2) * (uint32)(j));
    w_u32((uint32)((0x800B3920u + 4u * (uint32)(((v3 += 1u) - 1u)))), (uint32)(((uint32)(((uint32)(r_u16((uint32)((uint32)((sint32)((uint32)(v5) - (uint32)(2145792020)))))) - (uint32)(2145792035))) + (uint32)(2))));
  }

  w_u32((uint32)((0x800B3920u + 4u * (uint32)(v3))), (uint32)(0));
  v6 = (sint32)((uint32)(v3) + (uint32)(1));
  for (k = 0; (k < 8); (k += 1u))
  {
    v8 = (sint32)((uint32)(2) * (uint32)(k));
    w_u32((uint32)((0x800B3920u + 4u * (uint32)(((v6 += 1u) - 1u)))), (uint32)(((uint32)(((uint32)(r_u16((uint32)((uint32)((sint32)((uint32)(v8) - (uint32)(2145792004)))))) - (uint32)(2145792035))) + (uint32)(2))));
  }

  result = (sint32)((uint32)(4) * (uint32)(v6));
  w_u32((uint32)((0x800B3920u + 4u * (uint32)(v6))), (uint32)(0));
  return result;
}

