#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80038C48(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    w_u16(0x801F3004u, 0);
    w_u8(0x801F2FE5u, 1);
  gt_draft_unresolved_call(0x8003D1A8u, -2145439720, 0, 0, 0, 0, 0, 0, 0);
    w_u32(0x801F3018u, r_u32(0x801F3018u) + (1024));
  gt_draft_unresolved_call(0x8003A2DCu, 0, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003A27Cu, 15, v0, 0, 0, 0, 0, 0, 0);
  return 0;
}
