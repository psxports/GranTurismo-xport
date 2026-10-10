#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8003BA30 has additional inferred arguments requiring integration review */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80044394(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v12;
  uint32 v13;
  sint32 v14;
  sint32 v15;
  sint32 v16;
  sint32 v18;
  sint32 v19;
  sint32 v20;
  sint32 vars0;
  sint32 vars0a;
  sint32 vars4;
  sint32 vars8;
  uint32 varsC;
  gt_draft_8003BA30(a0, 0x80092BDCu, (sint32)((uint32)((sint32)a1) + (uint32)(74)), (sint32)((uint32)((sint32)a2) + (uint32)(80)), (sint32)a3, (sint32)a4, 0, 0);
  if (((sint8)r_u8((uint32)(0x800B5304)) >= 4))
  {
    v12 = a0;
    if (((sint8)r_u8((uint32)(0x800B5304)) != 4))
      goto LABEL_11;
    v13 = 0x80092C88u;
    v14 = (sint32)((uint32)((sint32)a1) + (uint32)(160));
    v15 = (sint32)((uint32)((sint32)a2) + (uint32)(192));
  }
  else
    if (((sint8)r_u8((uint32)(0x800B5304)) >= 2))
  {
    v12 = a0;
    v13 = 0x80092C68u;
    v14 = (sint32)((uint32)((sint32)a1) + (uint32)(160));
    v15 = (sint32)((uint32)((sint32)a2) + (uint32)(192));
  }
  else
  {
    v12 = a0;
    if (((sint8)r_u8((uint32)(0x800B5304)) != 1))
    {
      LABEL_11:
      gt_draft_unresolved_call(0x80043F34u, (sint32)(a0), (uint32)(r_u32((uint32)((0x80092BECu + 4u * (uint32)((sint8)r_u8((uint32)(0x80092BDDu))))))), (sint32)a1, (sint32)a2, (sint32)a4,0u,0u,0u);

      v16 = (sint32)(a0);
      goto LABEL_12;
    }
    v13 = 0x80092C78u;
    v14 = (sint32)((uint32)((sint32)a1) + (uint32)(160));
    v15 = (sint32)((uint32)((sint32)a2) + (uint32)(192));
  }
  gtmain_menu_choice_transition_draw(v12, v13, v14, v15, (sint32)a4, v13 == 0x80092C88u ? 0u : 1u, 0u, 0u);
  v16 = (sint32)(a0);
  LABEL_12:
  gtmain_menu_vehicle_course_labels_draw(v16, (sint32)a1, (sint32)a2, (sint32)a4);

  return 0;
}

