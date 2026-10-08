#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80022C4C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v1;
  sint32 v3;
  sint32 v4;
  v1 = gt_draft_unresolved_call(0x8002278Cu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  if ((sint32)a0)
    v3 = (sint32)((uint32)(v1) + (uint32)(56));
  else
    v3 = (sint32)((uint32)(v1) + (uint32)(28));
  v4 = 0;
  if ((sint8)r_u8((uint32)(0x800BDD01)))
  {
    w_u32((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(12)))), (uint32)(r_u32((uint32)(0x800BDB4C))));
    v4 = 2;
    w_u16((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(16)))), (uint32)(r_u16((uint32)(0x800B6670))));
  }
  if ((sint8)r_u8((uint32)(0x800BDD00)))
  {
    w_u32((uint32)((uint32)((sint32)((uint32)(v1) + (uint32)(12)))), (uint32)(r_u32((uint32)(0x800BDB48))));
    v4 |= 1u;
    w_u16((uint32)((uint32)((sint32)((uint32)(v1) + (uint32)(16)))), (uint32)(r_u16((uint32)(0x800B6670))));
  }
  return gt_draft_unresolved_call(0x80033FF8u, v4, (sint8)r_u8((uint32)(0x800B63D2)), 0, (sint32)a0, 0u, 0u, 0u, 0u);
}

