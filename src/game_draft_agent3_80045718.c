#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80045718(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v12;
  sint32 v13;
  sint32 v14;
  sint32 v15;
  sint32 v16;
  sint32 v18;
  sint32 v19;
  sint32 vars0;
  gt_draft_unresolved_call(0x8006C790u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C860u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v12 = gt_draft_unresolved_call(0x80035398u, 7, ((sint32)((sint32)a4) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C874u, v12, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v13 = gt_draft_unresolved_call(0x8006C1CCu, (sint16)r_u16((uint32)(0x800B63C4)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_8003A8AC((sint32)a0, (sint32)((uint32)((sint32)a1) + (uint32)(24)), (sint32)((uint32)((sint32)a2) + (uint32)(40)), v13, 0u, 0u, 0u, 0u);
  v14 = (sint8)r_u8((uint32)(0x80093049u));
  v15 = gt_draft_unresolved_call(0x80035458u, ((sint32)((sint32)a4) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C874u, v15, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8004B2FCu, (sint32)a0, r_u32((uint32)(0x8009303Cu)), (sint32)((uint32)((sint32)a1) + (uint32)(32)), (sint32)((uint32)((sint32)a2) + (uint32)(56)), 0u, 0u, 0u, 0u);
  v16 = gt_draft_80045320(v14, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v18 = (sint8)r_u8((uint32)(0x800B5304));
  gt_draft_unresolved_call(0x8004BDB0u, (sint32)a0, v16, (sint32)((uint32)((sint32)a1) + (uint32)(64)), (sint32)((uint32)((sint32)a2) + (uint32)(56)), 0u, 0u, 0u, 0u);
  if ((sint8)r_u8((uint32)(0x800B5304)))
    gt_draft_unresolved_call(0x80043F34u, (sint32)a0, (uint32)(r_u32((uint32)((r_u32((uint32)(r_u32((uint32)((0x800931DCu + 4u * (uint32)(v14)))))) + 4u * (uint32)((sint8)r_u8((uint32)((uint32)((sint32)((uint32)(v16) + (uint32)(1)))))))))), (sint32)a1, (sint32)a2, (sint32)a4, 0u, 0u, 0u);
  else
    gt_draft_unresolved_call(0x80043F34u, (sint32)a0, (uint32)(0x800BE291), (sint32)a1, (sint32)a2, (sint32)a4, 0u, 0u, 0u);
  return 0;
}

