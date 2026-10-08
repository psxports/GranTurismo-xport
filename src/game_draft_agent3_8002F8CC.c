#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002F8CC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v4;
  sint32 i;
  sint32 v6;
  sint32 v7;
  uint32 v8;
  v4 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(4)))));
  for (i = 0; v4; v4 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(8) * (uint32)(i))) + (uint32)((sint32)a1))) + (uint32)(4))))))
  {
    v6 = gt_draft_unresolved_call(0x8006C900u, (sint32)a0, v4, 0u, 0u, 0u, 0u, 0u, 0u);
    v7 = (sint32)((uint32)(8) * (uint32)(((i += 1u) - 1u)));
    v8 = (uint32)((sint32)((uint32)(v7) + (uint32)((sint32)a1)));
    w_u16((uint32)((v8 + 2u * (uint32)(1))), (uint32)(v6));
    w_u16((uint32)(v8), (uint32)(gt3_sdiv((sint32)((uint32)(320) - (uint32)(v6)), 2)));
  }

  /* TODO Original void return has no required result value */
  return 0u;
}

