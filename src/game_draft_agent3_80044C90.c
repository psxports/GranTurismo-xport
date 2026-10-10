#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8003BA30 has additional inferred arguments requiring integration review */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80044C90(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v13;
  sint32 v14;
  sint32 v15;
  sint32 vars0;
  sint32 vars0a;
  sint32 vars4;
  sint32 vars8;
  uint32 varsC;
  gt_draft_8003BA30(a0, 0x80092C28u, (sint32)((uint32)((sint32)a1) + (uint32)(74)), (sint32)((uint32)((sint32)a2) + (uint32)(80)), (sint32)a3, (sint32)a4, 0, 0);
  gt_draft_unresolved_call(0x80043F34u, (sint32)(a0), (uint32)(r_u32((uint32)((0x80092C38u + 4u * (uint32)((sint8)r_u8((uint32)(0x80092C29u))))))), (sint32)a1, (sint32)a2, (sint32)a4,0u,0u,0u);
  gtmain_menu_vehicle_course_labels_draw(a0, (sint32)a1, (sint32)a2, (sint32)a4);
  gt_draft_unresolved_call(0x800469F0u, a0, (sint32)a1, (sint32)a2, (sint32)a4, 0u, 0u, 0u, 0u);
  return 0;
}

