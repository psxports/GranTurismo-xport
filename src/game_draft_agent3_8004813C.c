#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8003BA30 has additional inferred arguments requiring integration review */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8004813C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v10 = (sint32)gt_draft_incoming_register(0x8004813Cu, 2u);
  /* TODO Nonstandard incoming v0 requires a caller adapter */
  sint32 v13;
  sint32 v14;
  sint32 v15;
  sint32 v16;
  sint32 v17;
  sint32 v18;
  uint8 v19;
  sint32 v20;
  sint32 v21;
  uint32 v22;
  uint32 i;
  uint32 result;
  uint32 v25;
  sint32 v26;
  sint32 v27;
  sint32 v28;
  sint32 v29;
  sint32 v30;
  sint32 v31;
  sint32 v32;
  sint32 v33;
  sint32 v34;
  sint32 v35;
  sint32 v36;
  sint32 v37;
  uint32 v38;
  sint8 v39;
  sint8 v40[128];
  sint32 v41;
  v41 = (sint32)a3;
  v13 = (sint32)(0u - (uint32)(2146707876));
  v14 = 0;
  if (((r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(152)))))) + (uint32)(1))))) & 0xF0) == 32))
    v13 = (sint32)(0u - (uint32)(2146707845));
  v15 = 0;
  v16 = (sint32)((uint32)((sint32)a0) + (uint32)(512));
  do
  {
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v16) + (uint32)(114))))))
      (v14 += 1u);
    (v15 += 1u);
    v16 += 116;
  }
  while ((v15 < 120));
  gt_draft_unresolved_call(0x8007C014u, gt_draft_buffer_address(v40, sizeof(v40)), (uint32)(0x800BCF7F), (sint32)((uint32)(120) - (uint32)(v14)), 120, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C860u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v17 = gt_draft_unresolved_call(0x80035398u, 6, ((sint32)((sint32)a5) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C874u, v17, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C790u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v18 = gt_draft_unresolved_call(0x8006C900u, 1, gt_draft_buffer_address(v40, sizeof(v40)), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006CA10u, a1, (sint32)((uint32)((sint32)a2) - (uint32)((sint32)((uint32)(v18) - (uint32)(304)))), (sint32)((uint32)(v41) + (uint32)(180)), gt_draft_buffer_address(v40, sizeof(v40)), 0u, 0u, 0u, 0u);
  if (((sint8)r_u8((uint32)(0x800B538D)) != 1))
  {
    v19 = gt_draft_unresolved_call(0x80035608u, 0, ((sint32)((sint32)a5) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u);
    v20 = gt_draft_unresolved_call(0x80035458u, v19, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8006C874u, v20, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_8003A8AC(a1, (sint32)((uint32)((sint32)a2) + (uint32)(24)), (sint32)((uint32)(v41) + (uint32)(41)), v13, 0u, 0u, 0u, 0u);
  }
  gt_draft_8003BA30(a1, 0x80092E9Cu, (sint32)((uint32)((sint32)a2) + (uint32)(160)), (sint32)((uint32)(v41) + (uint32)(59)), (sint32)a4, (sint32)a5, 1, (sint32)(gt_draft_buffer_address(&v35, sizeof(v35))));
  gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v21 = 0;
  v22 = gt_draft_buffer_address(&v39, sizeof(v39));
  for (i = gt_draft_buffer_address(&v35, sizeof(v35));; i = (uint32)(((uint32)(i) + 1u * (uint32)(2))))
  {
    result = ((uint32)(gt_draft_buffer_address(&v35, sizeof(v35))) + 1u * (uint32)(2));
    v25 = (((uint32)(gt_draft_buffer_address(&v35, sizeof(v35))) + 1u * (uint32)(v21)) + 1u * (uint32)(2));
    v26 = (sint8)r_u8((uint32)((((uint32)(gt_draft_buffer_address(&v35, sizeof(v35))) + 1u * (uint32)(v21)) + 1u * (uint32)(2))));
    if ((v26 < 0))
      break;
    v27 = 0;
    if ((v26 == (sint8)r_u8((uint32)(0x80092E9Du))))
    {
      gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v27 = 1;
      v28 = gt_draft_unresolved_call(0x80035458u, ((sint32)((sint32)a5) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, v28, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x800472C8u, (sint32)(a1), (sint32)((uint32)((sint32)a2) + (uint32)(160)), (sint32)((uint32)(v41) + (uint32)(192)), (sint8)r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800B5388))) + (uint32)((sint8)r_u8((uint32)(v25))))))), 0u, 0u, 0u, 0u);
    }
    if (((v26 == r_u16((uint32)(0x800B537C))) && ((sint8)r_u8((uint32)(0x800B538C)) == 1)))
    {
      v29 = (uint8)((sint8)r_u8((uint32)(v22)));
      if ((sint8)r_u8((uint32)(0x800B538D)))
        v29 >>= 1;
      v30 = (sint16)r_u16((uint32)(((uint32)(i) + 2u * (uint32)(25))));
      v31 = (sint16)r_u16((uint32)(((uint32)(i) + 2u * (uint32)(49))));
      v32 = (sint32)(0u - (uint32)(1));
      v33 = 1;
      v34 = v27;
    }
    else
    {
      v30 = (sint16)r_u16((uint32)(((uint32)(i) + 2u * (uint32)(25))));
      v31 = (sint16)r_u16((uint32)(((uint32)(i) + 2u * (uint32)(49))));
      v29 = (uint8)((sint8)r_u8((uint32)(v22)));
      v32 = r_u32((uint32)((uint32)(((uint32)(((uint32)((sint32)((uint32)(8) * (uint32)((sint8)r_u8((uint32)(v25))))) + (uint32)(r_u32((uint32)(0x800B5384))))) + (uint32)(4)))));
      v34 = v27;
      v33 = (sint8)r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800B5388))) + (uint32)((sint8)r_u8((uint32)(v25)))))));
    }
    (v22 += 1u);
    gt_draft_unresolved_call(0x800473D4u, (sint32)(a1), v30, v31, v29, v32, v33, v34, 1);
    (v21 += 1u);
  }

  return result;
}

