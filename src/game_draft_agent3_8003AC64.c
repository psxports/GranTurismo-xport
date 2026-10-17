#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003AC64(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v7;
  uint32 v8;
  uint32 v9;
  uint32 v10;
  sint32 result;
  sint32 v12;
  sint32 v13;
  v7 = a3;
  v8 = 0x800921A4u;
  v9 = 0x800921B0u;
  v10 = 0x800921ADu;
  if (gt_draft_8003AB60(a3, 0u, 0u, 0u, 0u, 0u, 0u, 0u))
  {
    v8 = 0x800921BCu;
    v9 = 0x800921C8u;
    v10 = 0x800921C5u;
  }
  do
  {
    result = r_u8((uint32)(((v7 += 1u) - 1u)));
    w_u32((uint32)((0x8008CDB0u + 1u * (uint32)(0))), (uint32)(result));
    if (!result)
      break;
    v12 = (sint8)r_u8((uint32)((uint32)(v9)));
    v9 = (uint32)(((uint32)(v9) + 1u * (uint32)(1)));
    gt_draft_unresolved_call(0x8006CA10u, (sint32)a0, (sint32)a1, (sint32)((uint32)((sint32)a2) + (uint32)(v12)), r_u8((uint32)(0x8008CDB0u)), 0u, 0u, 0u, 0u);
    v13 = (sint8)r_u8((uint32)((uint32)(v8)));
    v8 = (uint32)(((uint32)(v8) + 1u * (uint32)(1)));
    a1 += v13;
    result = ((sint32)(v8) < (sint32)(v10));
  }
  while (((sint32)(v8) < (sint32)(v10)));
  return result;
}

