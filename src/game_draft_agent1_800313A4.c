#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800313A4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v1 = 0;
  v1 = 0;
  if ( a0 == 8 || a0 == 10 )
    v1 = 1;
  return gt_draft_unresolved_call(0x80070C2Cu, 20 * a0 + r_u32(0x800B4A08u), v1, 0, 0, 0, 0, 0, 0);
}
