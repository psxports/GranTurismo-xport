#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001ECDC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  gt_draft_unresolved_call(0x8001FCA0u, r_u32((uint32)(0x800B7170)), (sint32)(0u - (uint32)(2146715568)), 0u, 0u, 0u, 0u, 0u, 0u);
  v0 = gt_draft_unresolved_call(0x80023C44u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8001FD1Cu, (sint32)(0u - (uint32)(2146736048)), r_u8((uint32)(0x800B6673)), v0, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8007BD58u, (sint32)(0u - (uint32)(2146736008)), 0, 1984, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x80051A80u, (sint32)(0u - (uint32)(2146736008)), (sint32)(0u - (uint32)(2146715568)), r_u32((uint32)(0x800B6864)), ((sint32)((sint32)(r_u32((uint32)(0x800B7104)))) >> ((uint32)(4) & 31u)), 0u, 0u, 0u, 0u);
  return gt_draft_unresolved_call(0x80051D30u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

