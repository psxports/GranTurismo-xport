#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003A768(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v10;
  sint32 result;
  sint16 v14;
  uint32 v15;
  sint32 v16;
  sint32 v17;
  sint32 v18;
  uint32 v19;
  sint16 v20;
  sint32 v21;
  sint32 v22;
  uint32 v23;
  sint32 v24;
  sint32 v25;
  uint32 v26;
  sint8 v27;
  sint32 v28;
  v10 = a4;
  v14 = (sint32)((uint32)((sint16)a3) - (uint32)(4));
  result = gt_draft_unresolved_call(0x80071210u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v15 = (uint32)(r_u32((uint32)(0x800BFE4C)));
  v16 = (sint32)r_u32((uint32)(a0));
  v17 = (sint32)r_u32((uint32)((a0 + 4u * (uint32)(1))));
  v18 = (sint32)r_u32((uint32)((a0 + 4u * (uint32)(3))));
  v19 = (((uint32)(v19) & 0xFFFFFF00u) | (((uint32)(r_u8((uint32)(a4))) << 0u) & 0x000000FFu));
  v20 = r_u16((uint32)(((uint32)(a0) + 2u * (uint32)(12))));
  v21 = result;
  if (r_u8((uint32)(a4)))
  {
    v22 = ((uint32)(r_u32((uint32)(0x800BFE4C))) + (uint32)(2));
    do
    {
      v19 = (uint8)(v19);
      v23 = (uint32)(((uint32)(((uint32)(8) * (uint32)((uint8)(v19)))) + (uint32)(v16)));
      v24 = (sint32)r_u32((uint32)(v23));
      v25 = (sint32)r_u32((uint32)((v23 + 4u * (uint32)(1))));
      v26 = (((uint32)((sint32)r_u32((uint32)(v23))) >> 24u) & 0xFFu);
      if (v26)
      {
        w_u32((uint32)(v15), (uint32)(0x4000000));
        w_u16((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(8)))), (uint32)((sint32)((uint32)(v14) + (uint32)((((uint32)(v25) >> 8u) & 0xFFu)))));
        w_u16((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(14)))), (uint32)((((uint32)(v24) >> 16u) & 0xFFu)));
        w_u32((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(2)))), (uint32)(v18));
        w_u16((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(6)))), (uint32)((sint16)a2));
        w_u16((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(16)))), (uint32)(v26));
        w_u16((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(10)))), (uint32)(v24));
        w_u16((uint32)((uint32)((sint32)((uint32)(v22) + (uint32)(12)))), (uint32)(v20));
        v27 = r_u8((uint32)((uint32)((sint32)((uint32)(v21) + (uint32)(2)))));
        w_u16((uint32)((uint32)(v21)), (uint32)((uint16)(v15)));
        w_u8((uint32)((uint32)((sint32)((uint32)(v21) + (uint32)(2)))), (uint32)(((uint32)(((uint32)((uint32)(v15)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
        w_u8((uint32)((uint32)(v22)), (uint32)(v27));
        v22 += 20;
        v15 += 4u * (uint32)(5);
      }
      (v10 += 1u);
      v28 = r_u32((uint32)((uint32)(((uint32)(((uint32)(4) * (uint32)(v19))) + (uint32)(v17)))));
      v19 = (sint8)(r_u8((uint32)(v10)));
      result = ((uint32)(((uint32)((((uint32)(v25) >> 16u) & 0xFFu)) - (uint32)((((uint32)(r_u32((uint32)((uint32)(((uint32)((((uint32)(v19) >> ((uint32)(1) & 31u)) & 0x7C)) + (uint32)(v28)))))) >> ((uint32)(((uint32)(4) * (uint32)((v19 & 7)))) & 31u)) & 0xF)))) + (uint32)((sint32)a5));
      a2 += result;
    }
    while (r_u8((uint32)(v10)));
  }
  w_u32((uint32)(0x800BFE4C), (uint32)(v15));
  return result;
}

