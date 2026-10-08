#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003011C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{

  gt_draft_unresolved_call(0x8006C790u, 3, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, a2, 0, 0, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8006CA10u, a0, r_u16(0x800911F8u + 2u * (4 * a3)), a1, r_u32(0x800911FCu + 4u * (2 * a3)), 0, 0, 0, 0);
}
