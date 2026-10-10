#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002A084(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 result;
  v0 = r_u32((uint32)((gtmain_race_mode_record() + 4u * (uint32)(4))));
  gt_draft_unresolved_call(0x80029740u, r_u32((uint32)(0x800B6368)), (sint32)(0u - (uint32)(2146707413)), (sint32)(0u - (uint32)(2146707111)), v0, 0u, 0u, 0u, 0u);
  result = gt_draft_unresolved_call(0x800298B4u, v0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u8((uint32)(0x8008F890u), (uint32)(result));
  return result;
}

