#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80025B68(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 v1;
  sint32 v2;
  sint32 v3;
  sint32 v4;
  uint32 v5;
  gt_draft_unresolved_call(0x8001EFACu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v0 = r_u8((uint32)(0x800B6394));
  v1 = gt_draft_unresolved_call(0x8006B668u, r_u16((uint32)(0x800CB7FA)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v2 = gt_draft_unresolved_call(0x8001EFD8u, r_u32((uint32)(0x8008F798u)), v1, 0u, 0u, 0u, 0u, 0u, 0u);
  v3 = v2;
  if ((v2 < 0))
  {
    v2 = (((uint32)(v2) & 0xFFFFFF00u) | (((uint32)(0) << 0u) & 0x000000FFu));
    v3 = 0;
  }
  w_u8((uint32)(0x800B631C), (uint32)(v2));
  v4 = (sint32)r_u32((uint32)((uint32)(((uint32)(0x8008F78Cu) + 1u * (uint32)((sint32)((uint32)(v3) * (uint32)(4)))))));
  v5 = r_u32((uint32)((0x8008F798u + 4u * (uint32)(v3))));
  w_u32((uint32)(0x800BC1B4), (uint32)(r_u32((uint32)((uint32)(((uint32)(((uint32)(4) * (uint32)(r_u8((uint32)(0x800B63BD))))) + (uint32)(v4)))))));
  return gt_draft_unresolved_call(0x8001F5C8u, v5, v0, r_u16((uint32)(0x800CB7FA)), r_u8((uint32)(0x800CB7FC)), 0u, 0u, 0u, 0u);
}

