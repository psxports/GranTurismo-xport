#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_800426B8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 v1;
  sint32 v2;
  uint32 v3;
  sint32 v4;
  sint32 v5;
  v0 = gt_draft_unresolved_call(0x8001EB94u, (sint16)r_u16((uint32)(0x800BEA20)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v1 = r_u16((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)((uint8)(r_u32((uint32)((0x800941BDu + 1u * (uint32)((sint16)r_u16((uint32)(0x800BEA20))))))))))));
  v2 = v0;
  if (((sint32)((uint32)(v1) - (uint32)(1)) < 0))
    return (sint32)(0u - (uint32)(1));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D520u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D55Cu, (sint16)r_u16((uint32)(0x800BEA20)), v1, 0u, 0u, 0u, 0u, 0u);
  v3 = (uint32)((sint32)((uint32)((sint32)((uint32)(28) * (uint32)((sint32)((uint32)(v1) - (uint32)(1))))) + (uint32)(v2)));
  v4 = gt_draft_unresolved_call(0x800417ACu, r_u8((uint32)((v3 + 1u * (uint32)(2)))), r_u8((uint32)((v3 + 1u * (uint32)(3)))), r_u8((uint32)(v3)), r_u8((uint32)((v3 + 1u * (uint32)(4)))), 0u, 0u, 0u, 0u);
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(74)))), (uint32)(v4));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(64)))), (uint32)(r_u8((uint32)(v3))));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D748u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(64))))), (sint32)((uint32)(v4) + (uint32)(1)), r_u8((uint32)((v3 + 1u * (uint32)(4)))), 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D6FCu, r_u8((uint32)((v3 + 1u * (uint32)(2)))), r_u8((uint32)((v3 + 1u * (uint32)(3)))), 0u, 0u, 0u, 0u, 0u);
  v5 = gt_draft_unresolved_call(0x800417ACu, r_u8((uint32)((v3 + 1u * (uint32)(5)))), r_u8((uint32)((v3 + 1u * (uint32)(6)))), r_u8((uint32)((v3 + 1u * (uint32)(1)))), r_u8((uint32)((v3 + 1u * (uint32)(7)))), 0u, 0u, 0u, 0u);
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(87)))), (uint32)(v5));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(77)))), (uint32)(r_u8((uint32)((v3 + 1u * (uint32)(1))))));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D768u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(77))))), (sint32)((uint32)(v5) + (uint32)(1)), r_u8((uint32)((v3 + 1u * (uint32)(7)))), 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D6FCu, r_u8((uint32)((v3 + 1u * (uint32)(5)))), r_u8((uint32)((v3 + 1u * (uint32)(6)))), 0u, 0u, 0u, 0u, 0u);
  return 2;
}

