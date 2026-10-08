#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 80041150 has additional inferred arguments requiring integration review */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_800412F0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v15;
  sint32 v16;
  sint32 v17;
  sint32 v18;
  sint32 v19;
  sint32 v20;
  sint16 v21;
  sint32 v22;
  sint32 v23;
  sint32 v24;
  sint32 v25;
  sint16 v26;
  sint32 v27;
  sint32 v28;
  sint16 v29;
  sint32 v30;
  sint32 v31;
  sint32 v32;
  sint32 v33;
  sint32 v34;
  sint32 result;
  sint32 v36;
  sint16 v37;
  sint32 v38;
  sint32 v39;
  sint16 v40;
  sint16 v41;
  sint16 v42;
  v36 = (((uint32)(v36) & 0xFFFF0000u) | (((uint32)((sint16)a3) << 0u) & 0x0000FFFFu));
  v15 = 0;
  v41 = (sint16)a3;
  v16 = gt_draft_unresolved_call(0x80035398u, 0, (sint32)a5, 0u, 0u, 0u, 0u, 0u, 0u);
  v17 = (sint32)a5;
  v39 = (((uint32)(v39) & 0xFFFF0000u) | (((uint32)((sint16)a4) << 0u) & 0x0000FFFFu));
  v42 = (sint16)a4;
  w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(1)))), (uint32)((sint32)(0u - (uint32)(1))));
  v18 = v16;
  if (r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(14))))))
  {
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(1)))), (uint32)(r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(13)))))));
    v19 = (sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(14)))));
    if ((v19 == 2))
    {
      v32 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(2)))));
      v33 = (sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(15)))));
      v36 = (((uint32)(v36) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint16)a3) - (uint32)(gt3_sdiv((sint32)((uint32)(v32) * (uint32)(v33)), 12)))) << 0u) & 0x0000FFFFu));
      v15 = gt3_sdiv((sint32)((uint32)((sint32)a5) * (uint32)((sint32)((uint32)(12) - (uint32)(v33)))), 12);
      v41 = (sint32)((uint32)(v36) + (uint32)(v32));
    }
    else
    {
      if ((v19 >= 3))
      {
        if ((v19 == 3))
        {
          v26 = r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(4)))));
          v27 = (sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(15)))));
          v28 = (sint32)((uint32)((sint32)a5) * (uint32)((sint32)((uint32)(12) - (uint32)(v27))));
          v24 = ((uint64)((uint64)(((sint64)(715827883LL) * (sint32)(v28)))) >> ((uint32)(32) & 63u));
          v25 = ((sint32)(v28) >> ((uint32)(31) & 31u));
          v39 = (((uint32)(v39) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint16)a4) + (uint32)(gt3_sdiv((sint16)((sint32)((uint32)(v26) * (uint32)(v27))), 12)))) << 0u) & 0x0000FFFFu));
          v42 = (sint32)((uint32)(v39) - (uint32)(v26));
        }
        else
        {
          v20 = (sint32)(a1);
          if ((v19 != 4))
            goto LABEL_14;
          v21 = r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(4)))));
          v22 = (sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(15)))));
          v23 = (sint32)((uint32)((sint32)a5) * (uint32)((sint32)((uint32)(12) - (uint32)(v22))));
          v24 = ((uint64)((uint64)(((sint64)(715827883LL) * (sint32)(v23)))) >> ((uint32)(32) & 63u));
          v25 = ((sint32)(v23) >> ((uint32)(31) & 31u));
          v39 = (((uint32)(v39) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint16)a4) - (uint32)(gt3_sdiv((sint16)((sint32)((uint32)(v21) * (uint32)(v22))), 12)))) << 0u) & 0x0000FFFFu));
          v42 = (sint32)((uint32)(v39) + (uint32)(v21));
        }
      }
      else
      {
        if ((v19 != 1))
        {
          v20 = (sint32)(a1);
          LABEL_14:
          gt_draft_80041150(v20, ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(8)))))) + (uint32)((sint32)((uint32)(12) * (uint32)((sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(13))))))))), (sint16)(v36), (sint16)(v39), ((sint32)(v15) >> ((uint32)(1) & 31u)), 0, 0, 0);

          v17 = gt3_sdiv((sint32)((uint32)(255) * (uint32)((sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(15))))))), 12);
          LABEL_23:
          v34 = (sint32)(a1);

          goto LABEL_24;
        }
        v29 = r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(2)))));
        v30 = (sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(15)))));
        v31 = (sint32)((uint32)((sint32)a5) * (uint32)((sint32)((uint32)(12) - (uint32)(v30))));
        v24 = ((uint64)((uint64)(((sint64)(715827883LL) * (sint32)(v31)))) >> ((uint32)(32) & 63u));
        v25 = ((sint32)(v31) >> ((uint32)(31) & 31u));
        v36 = (((uint32)(v36) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint16)a3) + (uint32)(gt3_sdiv((sint16)((sint32)((uint32)(v29) * (uint32)(v30))), 12)))) << 0u) & 0x0000FFFFu));
        v41 = (sint32)((uint32)(v36) - (uint32)(v29));
      }
      v15 = (sint32)((uint32)(((sint32)(v24) >> ((uint32)(1) & 31u))) - (uint32)(v25));
    }
    v20 = (sint32)(a1);
    goto LABEL_14;
  }
  v34 = (sint32)(a1);
  if (((((r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(12))))) & 0x20) == 0) && (sint8)r_u8((uint32)(0x80091B41u))) && (((sint8)a6 & 1) != 0)))
  {
    if (((r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(12))))) & 0x1F) != 0))
    {
      if (((r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(12))))) & 0x1F) == 1))
      {
        gt_draft_unresolved_call(0x8003B5B8u, a1, (sint16)a3, (sint32)((uint32)((sint32)((uint32)((sint16)a4) - (uint32)(((sint32)((sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(4)))))) >> ((uint32)(1) & 31u))))) - (uint32)(12)), v16, 0u, 0u, 0u, 0u);
        gt_draft_unresolved_call(0x8003B600u, a1, (sint16)a3, (sint32)((uint32)((sint32)((uint32)((sint16)a4) + (uint32)(((sint32)((sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(4)))))) >> ((uint32)(1) & 31u))))) + (uint32)(12)), v18, 0u, 0u, 0u, 0u);
        v34 = (sint32)(a1);
      }
    }
    else
    {
      gt_draft_unresolved_call(0x8003B690u, a1, (sint32)((uint32)((sint32)((uint32)((sint16)a3) - (uint32)(((sint32)((sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(2)))))) >> ((uint32)(1) & 31u))))) - (uint32)(12)), (sint16)a4, v16, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8003B648u, a1, (sint32)((uint32)((sint32)((uint32)((sint16)a3) + (uint32)(((sint32)((sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(2)))))) >> ((uint32)(1) & 31u))))) + (uint32)(12)), (sint16)a4, v18, 0u, 0u, 0u, 0u);
      v34 = (sint32)(a1);
    }
    gt_draft_unresolved_call(0x8006F590u, v34, 544, 0u, 0u, 0u, 0u, 0u, 0u);
    goto LABEL_23;
  }
  LABEL_24:
  gt_draft_80041150(v34, ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(8)))))) + (uint32)((sint32)((uint32)(12) * (uint32)((sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(1))))))))), v41, v42, ((sint32)(v17) >> ((uint32)(1) & 31u)), 1, 0, 0);

  v37 = (sint16)v36;
  v40 = (sint16)v39;
  /* Original overlapping halfwords are local aliases */
  result = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(1)))));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(2)))), (uint32)(v41));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4)))), (uint32)(v42));
  w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(10)))), (uint32)(v17));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))), (uint32)(v37));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(8)))), (uint32)(v40));
  w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(11)))), (uint32)(v15));
  w_u8((uint32)((uint32)((sint32)a0)), (uint32)(result));
  return result;
}

