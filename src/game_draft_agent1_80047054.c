#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80047054(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v3 = 0;
    uint32 v4 = 0;
    uint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    sint32 result = 0;
  v3 = a0 + 512;
  v4 = (uint32)r_u32(0x800BEA64u);
  v5 = (uint32)(116 * a1 + a0 + 512 + 18);
  if ( ((r_u32(0x800BEA64u) | (uint8)(116 * a1 + a0 + 18)) & 3) != 0 )
  {
    v6 = r_u32(0x800BEA64u) + 96;
    do
    {
      v7 = r_u32(v4 + 4u * (1));
      v8 = r_u32(v4 + 4u * (2));
      v9 = r_u32(v4 + 4u * (3));
    w_u32(v5, r_u32(v4));
    w_u32(v5 + 4u * (1), v7);
    w_u32(v5 + 4u * (2), v8);
    w_u32(v5 + 4u * (3), v9);
      v4 += 16;
      v5 += 16;
    }
    while ( v4 != (uint32)v6 );
    v10 = 8 * a1;
  }
  else
  {
    v11 = r_u32(0x800BEA64u) + 96;
    do
    {
      v12 = r_u32(v4 + 4u * (1));
      v13 = r_u32(v4 + 4u * (2));
      v14 = r_u32(v4 + 4u * (3));
    w_u32(v5, r_u32(v4));
    w_u32(v5 + 4u * (1), v12);
    w_u32(v5 + 4u * (2), v13);
    w_u32(v5 + 4u * (3), v14);
      v4 += 16;
      v5 += 16;
    }
    while ( v4 != (uint32)v11 );
    v10 = 8 * a1;
  }
  v15 = v3 + 4 * (4 * (v10 - a1) + a1);
  gt_draft_unresolved_call(0x8007BE38u, v15, -2146741356, 0, 0, 0, 0, 0, 0);
  result = 1;
    w_u16(v15 + 114, 1);
  return result;
}
