#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8003ADC0 has additional inferred arguments requiring integration review */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80041150(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v12;
  sint32 result;
  uint32 v15;
  sint32 v16;
  sint32 v17;
  sint32 v18;
  sint32 v19;
  sint32 v20;
  uint32 v21;
  sint32 v22;
  sint32 v23;
  sint32 v24;
  sint32 v25;
  sint32 v26;
  sint32 vars0;
  sint32 vars4;
  uint8 vars8;
  sint32 varsC;
  v12 = (r_u8((uint32)((uint32)((sint32)a1))) & 0x3F);
  if ((v12 == 1))
  {
    v15 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(4)))));
    v16 = (sint32)a0;
    v17 = ((uint32)((sint32)a2) - (uint32)(((uint32)(r_u16((uint32)((v15 + 2u * (uint32)(4))))) >> ((uint32)(1) & 31u))));
    v18 = ((uint32)((sint32)a3) - (uint32)(((uint32)(r_u16((uint32)((v15 + 2u * (uint32)(5))))) >> ((uint32)(1) & 31u))));
    return gt_draft_8003AD48(v15, v16, v17, v18, 0u, 0u, 0u, 0u);
  }
  result = ((r_u8((uint32)((uint32)((sint32)a1))) & 0x3Fu) < 2);
  if (((r_u8((uint32)((uint32)((sint32)a1))) & 0x3Fu) >= 2))
  {
    result = 2;
    if ((v12 != 2))
      return result;
    v19 = gt_draft_unresolved_call(0x80035458u, (sint32)a4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8006C874u, v19, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v20 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(8)))));
    v21 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(4)))));
    v22 = gt_draft_unresolved_call(0x8006C900u, 2, v20, 0u, 0u, 0u, 0u, 0u, 0u);
    v23 = v20;
    v24 = r_u16((uint32)((v21 + 2u * (uint32)(4))));
    v25 = (sint32)((uint32)((sint32)a2) - (uint32)(((sint32)((sint32)((uint32)(v22) + (uint32)(v24))) >> ((uint32)(1) & 31u))));
    v26 = (sint32)((uint32)((sint32)a3) - (uint32)(((sint32)((sint32)(r_u16((uint32)((v21 + 2u * (uint32)(5)))))) >> ((uint32)(1) & 31u))));
    gt_draft_unresolved_call(0x8006CA10u, (sint32)a0, (sint32)((uint32)(v25) + (uint32)(v24)), v26, v23, 0u, 0u, 0u, 0u);
    v15 = v21;
    v16 = (sint32)a0;
    v17 = (((uint32)(v17) & 0xFFFF0000u) | (((uint32)(v25) << 0u) & 0x0000FFFFu));
    v18 = v26;
    return gt_draft_8003AD48(v15, v16, v17, v18, 0u, 0u, 0u, 0u);
  }
  if (((r_u8((uint32)((uint32)((sint32)a1))) & 0x3F) == 0))
    return gt_draft_8003ADC0(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(4))))), (sint32)a0, ((uint32)((sint32)a2) - (uint32)(((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(8)))))) * (uint32)(((uint32)(r_u16((uint32)((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(4)))))) + (uint32)(8)))))) >> ((uint32)(1) & 31u))))) >> ((uint32)(7) & 31u)))), ((uint32)((sint32)a3) - (uint32)(((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(8)))))) * (uint32)(((uint32)(r_u16((uint32)((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(4)))))) + (uint32)(10)))))) >> ((uint32)(1) & 31u))))) >> ((uint32)(7) & 31u)))), (sint32)a4, r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(8))))),0u,0u);
  return result;
}

