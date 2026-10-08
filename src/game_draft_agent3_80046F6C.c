#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80046F6C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v3;
  sint32 v4;
  uint32 v5;
  uint32 v6;
  sint32 v7;
  sint32 v8;
  sint32 v9;
  sint32 v10;
  sint32 v11;
  sint32 v12;
  sint32 result;
  v3 = 0;
  v4 = (sint32)((uint32)((sint32)a0) + (uint32)(512));
  do
  {
    if ((v3 == (sint32)a1))
      w_u16((uint32)((uint32)((sint32)((uint32)(v4) + (uint32)(114)))), (uint32)(0));
    v5 = (uint32)((sint32)((uint32)(v4) - (uint32)(116)));
    if (((sint32)a1 < v3))
    {
      v6 = (uint32)(v4);
      if (((((uint8)(v4) | (uint8)(v5)) & 3) != 0))
      {
        do
        {
          v7 = r_u32((uint32)((v6 + 4u * (uint32)(1))));
          v8 = r_u32((uint32)((v6 + 4u * (uint32)(2))));
          v9 = r_u32((uint32)((v6 + 4u * (uint32)(3))));
          w_u32((uint32)(v5), (uint32)(r_u32((uint32)(v6))));
          w_u32((uint32)((v5 + 4u * (uint32)(1))), (uint32)(v7));
          w_u32((uint32)((v5 + 4u * (uint32)(2))), (uint32)(v8));
          w_u32((uint32)((v5 + 4u * (uint32)(3))), (uint32)(v9));
          v6 += 4u * (uint32)(4);
          v5 += 4u * (uint32)(4);
        }
        while ((v6 != (uint32)((sint32)((uint32)(v4) + (uint32)(112)))));
      }
      else
      {
        do
        {
          v10 = r_u32((uint32)((v6 + 4u * (uint32)(1))));
          v11 = r_u32((uint32)((v6 + 4u * (uint32)(2))));
          v12 = r_u32((uint32)((v6 + 4u * (uint32)(3))));
          w_u32((uint32)(v5), (uint32)(r_u32((uint32)(v6))));
          w_u32((uint32)((v5 + 4u * (uint32)(1))), (uint32)(v10));
          w_u32((uint32)((v5 + 4u * (uint32)(2))), (uint32)(v11));
          w_u32((uint32)((v5 + 4u * (uint32)(3))), (uint32)(v12));
          v6 += 4u * (uint32)(4);
          v5 += 4u * (uint32)(4);
        }
        while ((v6 != (uint32)((sint32)((uint32)(v4) + (uint32)(112)))));
      }
      w_u32((uint32)(v5), (uint32)(r_u32((uint32)(v6))));
      w_u16((uint32)((uint32)((sint32)((uint32)(v4) + (uint32)(114)))), (uint32)(0));
    }
    result = ((v3 += 1u) < 120);
    v4 += 116;
  }
  while ((v3 < 120));
  return result;
}

