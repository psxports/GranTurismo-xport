#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Original BREAK exception handler is an unresolved adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002C568(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v2;
  sint32 v3;
  sint32 v4;
  sint32 v5;
  sint32 v6;
  sint32 result;
  sint32 v8;
  sint16 v9;
  sint32 v10;
  uint32 v11;
  sint32 v12;
  sint32 v13;
  v2 = (sint32)((uint32)(r_u32((uint32)((a0 + 4u * (uint32)(1))))) + (uint32)(999));
  v3 = 0;
  v4 = ((uint32)((uint16)(r_u32((uint32)(a0)))) << ((uint32)(16) & 31u));
  v5 = (sint32)((uint32)(4) * (uint32)(gt3_sdiv(v2, 1000)));
  v6 = (sint32)((uint32)(v5) - (uint32)(2));
  w_u32((uint32)((a0 + 4u * (uint32)(2))), (uint32)((sint32)((uint32)(1000) * (uint32)(gt3_sdiv(v2, 1000)))));
  w_u32((uint32)((a0 + 4u * (uint32)(3))), (uint32)((sint32)((uint32)(v5) - (uint32)(1))));
  result = gt3_sdiv(((sint32)(v4) >> ((uint32)(16) & 31u)), 250);
  w_u32((uint32)((a0 + 4u * (uint32)(4))), (uint32)(result));
  if (((sint32)((uint32)(v5) - (uint32)(2)) >= 0))
  {
    v8 = 0;
    do
    {
      if (v8)
      {
        v9 = 556;
        if (((v3 & 1) == 0))
          v9 = 534;
      }
      else
      {
        v9 = 512;
      }
      v10 = gt3_sdiv((sint32)((uint32)(2298) * (uint32)(v3)), v6);
      if (((v6 == (sint32)(0u - (uint32)(1))) && ((sint32)((uint32)(2298) * (uint32)(v3)) == 0x80000000)))
        gt_draft_exception(6u);
      v11 = (a0 + 4u * (uint32)((sint32)((uint32)(8) * (uint32)(v3))));
      w_u16((uint32)((v11 + 2u * (uint32)(8))), (uint32)(v9));
      w_u16((uint32)((v11 + 2u * (uint32)(10))), (uint32)(624));
      v12 = 0;
      w_u16((uint32)((v11 + 2u * (uint32)(9))), (uint32)(0));
      w_u16((uint32)((v11 + 2u * (uint32)(11))), (uint32)(0));
      w_u16((uint32)((v11 + 2u * (uint32)(12))), (uint32)(512));
      w_u16((uint32)((v11 + 2u * (uint32)(13))), (uint32)(0));
      v13 = 0;
      do
      {
        gt_draft_unresolved_call(0x8002BF84u, (a0 + 4u * (uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(8) * (uint32)(v3))) + (uint32)(8))) + (uint32)(v13)))), (sint32)((uint32)(v10) + (uint32)(1399)), 0, 0, 0u, 0u, 0u, 0u);
        (v12 += 1u);
        v13 = (sint32)((uint32)(2) * (uint32)(v12));
      }
      while ((v12 < 3));
      w_u32((uint32)((uint32)((a0 + 4u * (uint32)((sint32)((uint32)((sint32)((uint32)(8) * (uint32)(v3))) + (uint32)(6)))))), (uint32)(416));
      gt_draft_unresolved_call(0x8002BF84u, (a0 + 4u * (uint32)((sint32)((uint32)((sint32)((uint32)(8) * (uint32)(((v3 += 1u) - 1u)))) + (uint32)(6)))), (sint32)((uint32)(v10) + (uint32)(1399)), 0, 0, 0u, 0u, 0u, 0u);
      result = (v6 < v3);
      v8 = (v3 & 3);
    }
    while ((v6 >= v3));
  }
  return result;
}

