#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80025C18(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 result = 0;
  v0 = 40 * r_u8(0x800B63BDu) - 2146652612;
  v1 = r_u32(v0 + 24);
  result = r_u32(v0 + 4);
    w_u32(0x800BDB4Cu, v1);
    w_u32(0x800BDB48u, result);
  return result;
}
