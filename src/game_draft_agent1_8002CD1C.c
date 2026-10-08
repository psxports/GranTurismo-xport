#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002CD1C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{

  return r_u8(gt_draft_unresolved_call(0x8007A87Cu, a0 << 2, a1, a2, a3, a4, a5, a6, a7)) == 0;
}
