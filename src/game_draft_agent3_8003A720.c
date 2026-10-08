#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003A720(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v1[10];
  w_u8((uint32)(0x800B52FA), (uint32)(0));
  gt_draft_unresolved_call(0x8003A540u, gt_draft_buffer_address(v1, sizeof(v1)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8003A5E8u, (sint32)(0u - (uint32)(2146706160)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return (sint32)(0u - (uint32)((gt_draft_unresolved_call(0x8006F174u, gt_draft_buffer_address(v1, sizeof(v1)), 0u, 0u, 0u, 0u, 0u, 0u, 0u) == 2)));
}

