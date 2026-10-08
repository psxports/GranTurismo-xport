#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003A8F4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v3;
  uint32 v4;
  v3 = 0;
  if (r_u8((uint32)(a1)))
  {
    v4 = a1;
    do
    {
      (v4 += 1u);
      (v3 += 1u);
    }
    while (r_u8((uint32)(v4)));
  }
  return (sint32)((uint32)(gt_draft_unresolved_call(0x8006C900u, (sint32)a0, a1, 0u, 0u, 0u, 0u, 0u, 0u)) + (uint32)((sint32)((uint32)(v3) * (uint32)((sint32)((uint32)((sint32)a2) - (uint32)(1))))));
}

