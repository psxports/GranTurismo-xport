#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Continued from the existing camera or packet translation without the guest save frame */
/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO GTE scalar temporaries retain inline operation order and require adapter integration */
/* TODO Original BREAK exception handler is an unresolved adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001C870(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 gte_a0;
  sint32 gte_s2;
  sint32 gte_s3;
  sint32 gte_s4;
  sint32 gte_s6;
  sint32 gte_v0;
  sint32 gte_v1;
  sint32 v2;
  sint32 v4;
  sint32 v5;
  sint32 v6;
  sint32 v9;
  sint32 v10;
  sint32 v14;
  sint32 v18;
  sint32 v19;
  sint32 v20;
  sint32 v21;
  sint32 v22;
  sint32 v29;
  sint32 v30;
  sint32 v31;
  sint32 v32;
  sint32 result;
  sint32 v34;
  sint32 v35;
  v2 = r_u32((uint32)(a1));
  v34 = r_u32((uint32)(a1));
  v4 = r_u32((uint32)((a1 + 4u * (uint32)(1))));
  v5 = r_u32((uint32)((a1 + 4u * (uint32)(2))));
  if ((r_u32((uint32)(a1)) < 0))
    v34 = (sint32)(0u - (uint32)(v2));
  v6 = r_u32((uint32)((a1 + 4u * (uint32)(1))));
  if ((v4 < 0))
    v6 = (sint32)(0u - (uint32)(v4));
  v35 = r_u32((uint32)((a1 + 4u * (uint32)(2))));
  if ((v5 < 0))
    v35 = (sint32)(0u - (uint32)(v5));
  gte_v0 = ((v34 | v6) | v35);
  gt_draft_gte_write(30, 0, gte_v0);
  gte_v1 = gt_draft_gte_read(31, 0);
  v9 = (sint32)((uint32)(17) - (uint32)(gte_v1));
  v10 = ((sint32)((uint32)(17) - (uint32)(gte_v1)) < 0);
  gte_v1 = ((sint32)(v2) >> ((uint32)((sint32)((uint32)(17) - (uint32)(gte_v1))) & 31u));
  if (v10)
  {
    v14 = (sint32)(0u - (uint32)(v9));
    gte_v1 = (sint32)((uint32)(v2) << ((uint32)(v14) & 31u));
    gte_s3 = (sint32)((uint32)(v4) << ((uint32)(v14) & 31u));
    gte_v0 = (sint32)((uint32)(v5) << ((uint32)(v14) & 31u));
  }
  else
  {
    gte_s3 = ((sint32)(v4) >> ((uint32)(v9) & 31u));
    gte_v0 = ((sint32)(v5) >> ((uint32)(v9) & 31u));
  }
  gt_draft_gte_write(9, 0, gte_v1);
  gt_draft_gte_write(10, 0, gte_s3);
  gt_draft_gte_write(11, 0, gte_v0);
  gt_draft_gte_command(0xA00428);
  gte_s6 = gt_draft_gte_read(25, 0);
  gte_a0 = gt_draft_gte_read(26, 0);
  gte_s4 = gt_draft_gte_read(27, 0);
  v18 = 0;
  v19 = 4096;
  v20 = gt_draft_unresolved_call(0x8006AFBCu, (sint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(gte_a0))) + (uint32)(gte_s4)), 0, 0u, 0u, 0u, 0u, 0u, 0u);
  if (v20)
  {
    v21 = gt_draft_unresolved_call(0x8006AFBCu, (sint32)((uint32)(gte_s6) + (uint32)(gte_s4)), 0, 0u, 0u, 0u, 0u, 0u, 0u);
    if (((v20 == (sint32)(0u - (uint32)(1))) && ((sint32)((uint32)(gte_s3) << ((uint32)(12) & 31u)) == 0x80000000)))
      gt_draft_exception(6u);
    v18 = gt3_sdiv((sint32)((uint32)(gte_s3) << ((uint32)(12) & 31u)), v20);
    v22 = (sint32)((uint32)(v21) << ((uint32)(12) & 31u));
    if (((v20 == (sint32)(0u - (uint32)(1))) && (v22 == 0x80000000)))
      gt_draft_exception(6u);
    v19 = gt3_sdiv(v22, v20);
  }
  gte_v0 = (v34 | v35);
  gt_draft_gte_write(30, 0, gte_v0);
  gte_v1 = gt_draft_gte_read(31, 0);
  gte_s3 = ((sint32)(v5) >> ((uint32)((sint32)((uint32)(17) - (uint32)(gte_v1))) & 31u));
  if (((sint32)((uint32)(17) - (uint32)(gte_v1)) < 0))
  {
    gte_s2 = (sint32)((uint32)(v2) << ((uint32)((sint32)((uint32)(gte_v1) - (uint32)(17))) & 31u));
    gte_s3 = (sint32)((uint32)(v5) << ((uint32)((sint32)((uint32)(gte_v1) - (uint32)(17))) & 31u));
  }
  else
  {
    gte_s2 = ((sint32)(v2) >> ((uint32)((sint32)((uint32)(17) - (uint32)(gte_v1))) & 31u));
  }
  gt_draft_gte_write(9, 0, gte_s2);
  gt_draft_gte_write(10, 0, gte_s3);
  gt_draft_gte_command(0xA00428);
  gte_v0 = gt_draft_gte_read(25, 0);
  gte_a0 = gt_draft_gte_read(26, 0);
  v29 = 0;
  v30 = 4096;
  v31 = gt_draft_unresolved_call(0x8006AFBCu, (sint32)((uint32)(gte_v0) + (uint32)(gte_a0)), 0, 0u, 0u, 0u, 0u, 0u, 0u);
  v32 = (sint32)((uint32)(gte_s2) << ((uint32)(12) & 31u));
  if (v31)
  {
    if (((v31 == (sint32)(0u - (uint32)(1))) && (v32 == 0x80000000)))
      gt_draft_exception(6u);
    v29 = gt3_sdiv(v32, v31);
    if (((v31 == (sint32)(0u - (uint32)(1))) && ((sint32)((uint32)(gte_s3) << ((uint32)(12) & 31u)) == 0x80000000)))
      gt_draft_exception(6u);
    v30 = gt3_sdiv((sint32)((uint32)(gte_s3) << ((uint32)(12) & 31u)), v31);
  }
  w_u16((uint32)(a0), (uint32)(v30));
  w_u16((uint32)((a0 + 2u * (uint32)(3))), (uint32)(0));
  w_u16((uint32)((a0 + 2u * (uint32)(4))), (uint32)(v19));
  w_u16((uint32)((a0 + 2u * (uint32)(5))), (uint32)(v18));
  w_u16((uint32)((a0 + 2u * (uint32)(6))), (uint32)((sint32)(0u - (uint32)((sint16)(v29)))));
  w_u16((uint32)((a0 + 2u * (uint32)(1))), (uint32)((sint32)(0u - (uint32)((sint16)(((sint32)((sint32)((uint32)(v29) * (uint32)(v18))) >> ((uint32)(12) & 31u)))))));
  w_u16((uint32)((a0 + 2u * (uint32)(2))), (uint32)(((sint32)((sint32)((uint32)(v29) * (uint32)(v19))) >> ((uint32)(12) & 31u))));
  w_u16((uint32)((a0 + 2u * (uint32)(7))), (uint32)((sint32)(0u - (uint32)((sint16)(((sint32)((sint32)((uint32)(v30) * (uint32)(v18))) >> ((uint32)(12) & 31u)))))));
  result = ((sint32)((sint32)((uint32)(v30) * (uint32)(v19))) >> ((uint32)(12) & 31u));
  w_u16((uint32)((a0 + 2u * (uint32)(8))), (uint32)(result));
  return result;
}

