#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80048448(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v3 = 0;
  v3 = a0 + 512;
  gt_draft_unresolved_call(0x8003C314u, (sint32)r_u32(0x80092EACu), 17, 256, (uint32)0x800B5394, 0, 0, 0, 0);
  if ( a1 >= 0 )
    gt_draft_unresolved_call(0x8003C338u, (sint32)r_u32(0x80092EACu), v3 + 116 * a1, 0, 0, 0, 0, 0, 0);
}
