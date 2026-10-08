#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001F080(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v1;
  sint32 v2;
  v1 = 0;
  if (r_u32((uint32)(a0)))
  {
    v2 = 1;
    while (r_u32((uint32)((a0 + 4u * (uint32)(((v2 += 1u) - 1u))))))
      ;

    return (sint32)((uint32)(v2) - (uint32)(1));
  }
  return v1;
}

