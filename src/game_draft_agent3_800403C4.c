#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_800403C4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 result;
  w_u32((uint32)(0x801F1FDC), (uint32)((sint32)a1));
  w_u8((uint32)(0x801F1FE4), (uint32)((sint8)a3));
  w_u8((uint32)(0x801F1FE5), (uint32)((sint8)a0));
  w_u32((uint32)(0x801F1FE8), (uint32)((sint32)a2));
  w_u32((uint32)(0x801F1FE0), (uint32)((sint32)a4));
  gt_draft_unresolved_call(0x8007BE38u, (sint32)(0u - (uint32)(2145443860)), (sint32)a5, 0u, 0u, 0u, 0u, 0u, 0u);
  result = 0x80035D88u;
  w_u32((uint32)(0x801F2488), (uint32)(0x80035D88u));
  w_u32((uint32)(0x800C0A80), (uint32)(0));
  return result;
}

