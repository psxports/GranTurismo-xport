#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80041D50(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    sint16 v3 = 0;
    sint32 v4 = 0;
  v1 = (uint8)r_u8(0x800941BDu + 1u * (v0));
  v2 = r_u16(r_u32(0x800BEA64u) + v1);
  v3 = 0;
  if ( r_u16(r_u32(0x800BEA64u) + v1) )
  {
    v4 = gt_draft_unresolved_call(0x8001EB94u, 32, 0, 0, 0, 0, 0, 0, 0);
    v3 = gt_draft_80041980(r_u8(20 * v2 + v4 - 18), r_u8(20 * v2 + v4 - 17), r_u8(r_u32(0x800BEA60u) + 123), r_u8(20 * v2 + v4 - 19), 0, 0, 0, 0)
       + 1;
    w_u8(0x800927FCu, r_u8(0x800927FCu) & (0x1Fu));
  }
    w_u16(0x800927CAu, v3);
  return 1;
}
