#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8004A964(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
  v0 = r_u32(0x800B53BCu) - 1;
  w_u32(0x800B53BCu, r_u32(0x800B53BCu)-1u);
  return v0 << 16 < 1 ? 7 : 0;
}
