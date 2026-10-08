#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001CCD4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{

  return gt_draft_unresolved_call(0x8006AFBCu, ((sint32)r_u32(a1) >> 10) * ((sint32)r_u32(a1) >> 10) + ((sint32)r_u32(a1 + 4u * (1)) >> 10) * ((sint32)r_u32(a1 + 4u * (1)) >> 10) + ((sint32)r_u32(a1 + 4u * (2)) >> 10) * ((sint32)r_u32(a1 + 4u * (2)) >> 10), 0, 0, 0, 0, 0, 0, 0);
}
