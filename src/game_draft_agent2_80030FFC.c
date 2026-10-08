#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80030FFC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v5 = 0;
    sint16 v7 = 0;
    sint16 v8 = 0;
    sint16 v10[4] = {0};
  v5 = r_u32(a2 + 4) & 7;
  if ( v5 == 1 )
  {
    v7 = 1;
  }
  else
  {
    v7 = 0;
    if ( (r_u32(a2 + 4) & 7u) >= 2 )
    {
      v7 = 2;
      if ( v5 != 2 )
        v7 = 0;
    }
  }
  v10[0] = r_u16(a1 + 4);
  v10[1] = r_u16(a1 + 6);
  v10[2] = r_u16(a1 + 8);
  v10[3] = r_u16(a1 + 10);
  gt_draft_unresolved_call(0x80086DA0u, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, a1 + 12, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x80086AF4u, 0, 0, 0, 0, 0, 0, 0, 0);
  v8 = r_u16(a0 + 2u * (9)) & 0xFFF0 | (r_u16(a1 + 4) >> 6) & 0xF;
    w_u16(a0 + 2u * (9), v8);
    w_u16(a0 + 2u * (9), v8 & 0xFE6F | (r_u16(a1 + 6) >> 4) & 0x10 | (v7 << 7));
    w_u16(a0 + 2u * (10), (r_u16(a1 + 4) & 0x3F) << (2 - v7));
    w_u16(a0 + 2u * (11), r_u16(a1 + 6));
  return a1 + r_u32(a1);
}
