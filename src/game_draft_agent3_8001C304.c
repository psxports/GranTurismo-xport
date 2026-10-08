#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Continued from the existing camera or packet translation without the guest save frame */
/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001C304(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 i;
  uint32 v9;
  uint32 v10;
  sint8 v11;
  uint32 v12;
  sint32 v13;
  sint8 v14;
  sint8 v15;
  uint32 v16;
  sint8 v17;
  sint8 v18;
  sint32 v19;
  sint8 v20;
  uint32 v21;
  uint32 v22;
  sint8 v23;
  sint8 v24;
  sint8 v25;
  sint8 v26;
  for (i = 0; (i < (sint32)a2); (i += 1u))
  {
    v9 = (uint32)(((uint32)(((uint32)(4) * (uint32)(r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(22)))))))) + (uint32)((sint32)a0)));
    v10 = (uint32)(gt_draft_unresolved_call(0x80030DFCu, r_u32((uint32)(v9)), 0u, 0u, 0u, 0u, 0u, 0u, 0u));
    v12 = (uint32)(gt_draft_80030E1C(r_u32((uint32)(v9)), 0u, 0u, 0u, 0u, 0u, 0u, 0u));
    v11 = gt_draft_80030E3C(r_u32((uint32)(v9)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v13 = r_u32((uint32)(v9));
    v15 = v11;
    v14 = gt_draft_80030E60(v13, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v16 = (r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(32))))) & 0xFFE0FFFF);
    v17 = v14;
    w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(22)))), (uint32)(r_u16((uint32)(v10))));
    v18 = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(44)))));
    v19 = (v16 | ((uint32)((r_u16((uint32)(v12)) & 0x1F)) << ((uint32)(16) & 31u)));
    v20 = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(20)))));
    v21 = (v19 & 0xFE7FFFFF);
    w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(32)))), (uint32)(v19));
    v22 = (uint16)(r_u16((uint32)(v12)));
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(44)))), (uint32)((sint32)((uint32)(v18) + (uint32)(v15))));
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(20)))), (uint32)((sint32)((uint32)(v20) + (uint32)(v15))));
    v23 = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(21)))));
    w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(32)))), (uint32)((v21 | ((uint32)((((uint32)(v22) >> ((uint32)(7) & 31u)) & 3)) << ((uint32)(23) & 31u)))));
    v24 = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(32)))));
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(21)))), (uint32)((sint32)((uint32)(v23) + (uint32)(v17))));
    v25 = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(45)))));
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(32)))), (uint32)((sint32)((uint32)(v24) + (uint32)(v15))));
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(45)))), (uint32)((sint32)((uint32)(v25) + (uint32)(v17))));
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(33)))), (uint32)(((uint32)(r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(33)))))) + (uint32)(v17))));
    if (((sint32)a3 == 4))
    {
      v26 = ((uint32)(r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(57)))))) + (uint32)(v17));
      w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(56)))), (uint32)(((uint32)(r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(56)))))) + (uint32)(v15))));
      w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(57)))), (uint32)(v26));
    }
    if (((sint32)a3 == 3))
      a1 += 40;
    else
      a1 += 48;
  }

  return (sint32)a1;
}

