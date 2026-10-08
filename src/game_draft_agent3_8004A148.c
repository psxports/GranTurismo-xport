#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8004A148(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  gt_draft_unresolved_call(0x8003C314u, (sint32)(r_u32((uint32)(0x80092F44u))), 31, 256, (uint32)(0x800BEA40), 0u, 0u, 0u, 0u);
  result = (sint16)r_u16((uint32)(0x800B53BA));
  if (((sint16)r_u16((uint32)(0x800B53BA)) >= 0))
    return gt_draft_unresolved_call(0x8003C338u, (sint32)(r_u32((uint32)(0x80092F44u))), ((uint32)(((uint32)(r_u32((uint32)(0x800BEA34))) + (uint32)((sint32)((uint32)(52) * (uint32)((sint16)r_u16((uint32)(0x800B53BA))))))) + (uint32)(8)), 0u, 0u, 0u, 0u, 0u, 0u);
  return result;
}

