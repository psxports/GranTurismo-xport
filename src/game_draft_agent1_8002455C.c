#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002455C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v1 = 0;
    sint32 i = 0;
  v1 = (uint32)(a0 + 404);
  for ( i = 0; i < 33; ++i )
  {
    w_u32(4 * i + a0 + 272, v1);
    v1 += 7 * r_u32(v1) + 1;
  }
  return v1;
}
