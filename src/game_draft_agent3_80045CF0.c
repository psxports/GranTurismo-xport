#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8003BA30 has additional inferred arguments requiring integration review */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80045CF0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint16 v13;
  sint32 v14;
  sint32 v15;
  uint32 v16;
  sint32 v17;
  uint32 v18;
  uint32 v19;
  uint32 v20;
  uint8 v21;
  sint32 v22;
  sint8 v23;
  sint32 v24;
  sint32 v25;
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
  uint32 v36;
  uint32 v37;
  sint32 v38;
  sint32 v39;
  sint32 v40;
  sint32 v41;
  sint32 v42;
  sint32 v43;
  sint32 v44;
  sint32 v45;
  uint32 v46;
  sint32 v48;
  sint32 v49;
  sint32 v50;
  sint32 v51;
  sint32 v52;
  sint32 v53;
  sint32 v54;
  uint32 v55;
  sint8 v56[26];
  sint8 v57;
  v13 = (sint32)((uint32)((sint32)a1) + (uint32)(160));
  v14 = gtmain_menu_list_extent(0x80093530u);
  if (((r_u8((uint32)(0x800B5374)) == 27) || (r_u8((uint32)(0x800B5374)) == 35)))
  {
    v13 = (sint32)((uint32)((sint32)a1) + (uint32)(120));
    v15 = (sint32)((uint32)(v14) - (uint32)(126));
  }
  else
  {
    v15 = (sint32)((uint32)(((sint32)(v14) >> ((uint32)(1) & 31u))) - (uint32)(112));
  }
  gt_draft_8003BA30(a0, 0x80093530u, v13, (sint32)((uint32)((sint32)a2) - (uint32)(v15)), (sint32)a3, (sint32)a4, 1, (sint32)(v56));
  if ((r_u8((uint32)(0x800B5374)) == 34))
  {
    gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v17 = 0;
    v18 = 0x800931F8u;
    v19 = gt_draft_buffer_address(v56, sizeof(v56));
    v20 = (uint32)(gt_draft_buffer_address(&v57, sizeof(v57)));
    do
    {
      v21 = r_u8((uint32)(((v20 += 1u) - 1u)));
      (v17 += 1u);
      v22 = gt_draft_unresolved_call(0x80035458u, v21, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, v22, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v23 = r_u8((uint32)(((uint32)(v18) + 1u * (uint32)(1))));
      v18 += 4u * (uint32)(2);
      v24 = (sint16)r_u16((uint32)(((uint32)(v19) + 2u * (uint32)(49))));
      v19 += 1u * (uint32)(2);
      gt_draft_unresolved_call(0x80045BF4u, (sint32)(a0), (sint32)((uint32)((sint32)a1) + (uint32)(152)), v24, v23, 0u, 0u, 0u, 0u);
    }
    while ((v17 < 9));
    v16 = a0;
  }
  else
  {
    if ((r_u8((uint32)(0x800B5374)) >= 0x23u))
    {
      v16 = a0;
      if ((r_u8((uint32)(0x800B5374)) != 35))
        goto LABEL_24;
    }
    else
    {
      v16 = a0;
      if ((r_u8((uint32)(0x800B5374)) != 27))
        goto LABEL_24;
    }
    if ((r_u32((uint32)(0x801D2FDC)) >= 0))
      goto LABEL_23;
    v25 = (sint32)(0u - (uint32)(2145570832));
    v26 = (sint32)(0u - (uint32)(2145569320));
    v27 = (sint8)r_u8((uint32)(0x80093531u));
    v28 = ((sint8)r_u8((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint8)r_u8((uint32)(0x80093531u))) - (uint32)(2145570852))) + (uint32)(4))))) == 0);
    do
    {
      v29 = r_u32((uint32)((uint32)((sint32)((uint32)(v26) + (uint32)(4)))));
      v30 = r_u32((uint32)((uint32)((sint32)((uint32)(v26) + (uint32)(8)))));
      v31 = r_u32((uint32)((uint32)((sint32)((uint32)(v26) + (uint32)(12)))));
      w_u32((uint32)((uint32)(v25)), (uint32)(r_u32((uint32)((uint32)(v26)))));
      w_u32((uint32)((uint32)((sint32)((uint32)(v25) + (uint32)(4)))), (uint32)(v29));
      w_u32((uint32)((uint32)((sint32)((uint32)(v25) + (uint32)(8)))), (uint32)(v30));
      w_u32((uint32)((uint32)((sint32)((uint32)(v25) + (uint32)(12)))), (uint32)(v31));
      v26 += 16;
      v25 += 16;
    }
    while ((v26 != (sint32)(0u - (uint32)(2145569224))));
    v32 = r_u32((uint32)(0x801D363C));
    v33 = r_u32((uint32)(0x801D3640));
    w_u32((uint32)((uint32)(v25)), (uint32)(r_u32((uint32)(0x801D3638))));
    w_u32((uint32)((uint32)((sint32)((uint32)(v25) + (uint32)(4)))), (uint32)(v32));
    w_u32((uint32)((uint32)((sint32)((uint32)(v25) + (uint32)(8)))), (uint32)(v33));
    v48 = ((sint32)((sint32)a4) >> ((uint32)(2) & 31u));
    v50 = (sint32)((uint32)((sint32)a2) + (uint32)(138));
    v49 = (sint32)((uint32)((sint32)a1) + (uint32)(140));
    v52 = (sint32)((uint32)((sint32)a1) + (uint32)(140));
    gt_draft_unresolved_call(0x8004D020u, a0, (sint32)((uint32)((sint32)a1) + (uint32)(176)), (sint32)((uint32)((sint32)a2) + (uint32)(80)), (sint32)a4, 0u, 0u, 0u, 0u);
    v16 = a0;
    if (!v28)
    {
      v34 = (sint32)(0u - (uint32)(2145570832));
      v35 = (sint32)((uint32)((sint32)((uint32)(108) * (uint32)(v27))) - (uint32)(2145570852));
      v36 = (uint32)((sint32)((uint32)(v35) + (uint32)(188)));
      v37 = (uint32)((sint32)((uint32)(v35) + (uint32)(284)));
      if (((((uint32)(((uint32)(108) * (uint32)((uint8)(v27)))) - (uint32)(104)) & 3) != 0))
      {
        do
        {
          v38 = r_u32((uint32)((v36 + 4u * (uint32)(1))));
          v39 = r_u32((uint32)((v36 + 4u * (uint32)(2))));
          v40 = r_u32((uint32)((v36 + 4u * (uint32)(3))));
          w_u32((uint32)((uint32)(v34)), (uint32)(r_u32((uint32)(v36))));
          w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(4)))), (uint32)(v38));
          w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(8)))), (uint32)(v39));
          w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(12)))), (uint32)(v40));
          v36 += 4u * (uint32)(4);
          v34 += 16;
        }
        while ((v36 != v37));
      }
      else
      {
        do
        {
          v41 = r_u32((uint32)((v36 + 4u * (uint32)(1))));
          v42 = r_u32((uint32)((v36 + 4u * (uint32)(2))));
          v43 = r_u32((uint32)((v36 + 4u * (uint32)(3))));
          w_u32((uint32)((uint32)(v34)), (uint32)(r_u32((uint32)(v36))));
          w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(4)))), (uint32)(v41));
          w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(8)))), (uint32)(v42));
          w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(12)))), (uint32)(v43));
          v36 += 4u * (uint32)(4);
          v34 += 16;
        }
        while ((v36 != v37));
      }
      v44 = r_u32((uint32)((v36 + 4u * (uint32)(1))));
      v45 = r_u32((uint32)((v36 + 4u * (uint32)(2))));
      w_u32((uint32)((uint32)(v34)), (uint32)(r_u32((uint32)(v36))));
      w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(4)))), (uint32)(v44));
      w_u32((uint32)((uint32)((sint32)((uint32)(v34) + (uint32)(8)))), (uint32)(v45));
      v50 = (sint32)((uint32)((sint32)a2) + (uint32)(138));
      v48 = (sint32)a4;
      v49 = (sint32)((uint32)((sint32)a1) + (uint32)(140));
      v52 = (sint32)((uint32)((sint32)a1) + (uint32)(140));
      gt_draft_unresolved_call(0x8004D020u, a0, (sint32)((uint32)((sint32)a1) + (uint32)(176)), (sint32)((uint32)((sint32)a2) + (uint32)(80)), (sint32)a4, 0u, 0u, 0u, 0u);
      LABEL_23:
      v16 = a0;

    }
  }
  LABEL_24:
  v46 = (0x80092F70u + 4u * (uint32)((sint32)((uint32)(4) * (uint32)((sint8)r_u8((uint32)(0x800B5305))))));

  gt_draft_unresolved_call(0x800706BCu, v16, ((uint16)((sint32)((uint32)((sint32)a1) + (uint32)(180))) | (sint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(80))) << ((uint32)(16) & 31u))), (sint32)r_u32((uint32)((v46 + 4u * (uint32)(3)))), (sint32)r_u32((uint32)((v46 + 4u * (uint32)(2)))), 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006F590u, a0, ((uint16)((sint8)((r_u8((uint32)(((uint32)(v46) + 1u * (uint32)(1)))) | 0x20))) | 0x200), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call9(0x80043F34u, (sint32)(a0), r_u32((uint32)((uint32)(((uint32)((sint32)((uint32)(4) * (uint32)((sint8)r_u8((uint32)(0x80093531u))))) + (uint32)(r_u32((uint32)(0x800B530C))))))), (sint32)a1, (sint32)a2, (sint32)a4, v48, v49, v50, v52);
  return 0;
}

