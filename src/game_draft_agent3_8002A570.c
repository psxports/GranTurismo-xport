#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002A570(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v0;
  sint32 v1;
  sint32 result;
  v0 = gt_draft_80029A74(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v1 = (sint32)r_u32((uint32)((v0 + 4u * (uint32)(2))));
  w_u8((uint32)(0x800B63BC), (uint32)(gt_draft_unresolved_call(0x8006E95Cu, (sint32)r_u32((uint32)((v0 + 4u * (uint32)(3)))), 0u, 0u, 0u, 0u, 0u, 0u, 0u)));
  w_u16((uint32)(0x800CB7FA), (uint32)(gt_draft_unresolved_call(0x8006B828u, v1, 0u, 0u, 0u, 0u, 0u, 0u, 0u)));
  w_u8((uint32)(0x800CB7FC), (uint32)(0));
  w_u8((uint32)(0x800CB7FD), (uint32)(0));
  w_u8((uint32)(0x800CB7FE), (uint32)(0));
  w_u8((uint32)(0x800CB7FF), (uint32)(0));
  w_u8((uint32)(0x800B6394), (uint32)(1));
  gt_draft_unresolved_call(0x80024D38u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_8001E988((sint32)(0u - (uint32)(2146603044)), 1, 4, 3, 0u, 0u, 0u, 0u);
  gt_draft_8002A500(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  result = 1;
  if (((sint32)r_u32((uint32)(v0)) == 4))
    result = 5;
  w_u16((uint32)(0x800B634C), (uint32)(result));
  return result;
}

