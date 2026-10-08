#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002BA60(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint8 v0;
  sint32 result;
  v0 = 0;
  if (((sint8)r_u8((uint32)(0x800B633C)) == 1))
  {
    w_u32((uint32)(0x800B6638), (uint32)((sint32)(0u - (uint32)(2146736860))));
    gt_draft_unresolved_call(0x8002B8D8u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8002B620u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x80086AF4u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x800702F8u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x80070240u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x800702BCu, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32((uint32)(0x800B6638), (uint32)((sint32)(0u - (uint32)(2146737136))));
    gt_draft_unresolved_call(0x8002B8D8u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v0 = 1;
  }
  else
    if (((sint8)r_u8((uint32)(0x800B633C)) >= 2))
  {
    if (((sint8)r_u8((uint32)(0x800B633C)) == 2))
    {
      gt_draft_unresolved_call(0x80086AF4u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x80070240u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x800702BCu, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v0 = 1;
      w_u32((uint32)(0x800B6638), (uint32)(gt_draft_unresolved_call(0x8001E2D4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u)));
      gt_draft_unresolved_call(0x8002B8D8u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8002B620u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
  }
  else
    if (!(sint8)r_u8((uint32)(0x800B633C)))
  {
    w_u32((uint32)(0x800B6638), (uint32)(gt_draft_unresolved_call(0x8001E2D4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u)));
    gt_draft_unresolved_call(0x8002B8D8u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x8002B620u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  }
  result = r_u32((uint32)(0x800B3990u));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800B3990u))) + (uint32)(38)))), (uint32)(v0));
  return result;
}

