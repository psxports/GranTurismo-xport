#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002C7A4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint8 v1[8];
  sint8 v2[8184];
  gt_draft_unresolved_call(0x80070598u, gt_draft_buffer_address(v1, sizeof(v1)), 0x8009130Cu, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_8002C74C((sint32)(v1), 15139136, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x80070500u, gt_draft_buffer_address(v2, sizeof(v2)), 16712000, (sint32)(0u - (uint32)(1)), 0u, 0u, 0u, 0u, 0u);
  return gt_draft_8002C894(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

