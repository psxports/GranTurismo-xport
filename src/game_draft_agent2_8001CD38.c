#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001CD38(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 result = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    uint32 v16 = 0;
  v4 = 0;
  v5 = r_u16(a0 + 2);
  v6 = 0;
  if ( r_u16(a0 + 2) )
  {
    v7 = 0;
    do
    {
      ++v6;
      v4 += r_u32(a0 + v7 + 4);
      v7 = 16 * v6;
    }
    while ( v6 < v5 );
  }
  if ( v4 && (v8 = 0, r_u16(a0 + 2) >= 2u) )
  {
    v12 = (v4 * (sint64)r_u32(0x800B3910u)) >> 12;
    v13 = v5 - 1;
    if ( v13 > 0 )
    {
      do
      {
        v4 = r_u32(a0 + 16 * (v8 + 1) + 4);
        v14 = v8 + 1 < v13;
        if ( v12 < v4 )
          break;
        ++v8;
        v12 -= v4;
      }
      while ( v14 );
    }
    if ( v4 )
      v15 = gt_draft_unresolved_call(0x8001E4A8u, v12 << 12, (uint64)v12 >> 20, v4, v4 >> 31, 0, 0, 0, 0);
    else
      v15 = 0;
    if ( v15 < 0 || (v16 = a1, v15 >= 4096) )
      v16 = a1;
    return gt_draft_unresolved_call(0x8006B0A8u, v16, 16 * v8 + a0 + 8, 16 * v8 + a0 + 24, 0, 0, 0, 0, 0);
  }
  else
  {
    result = r_u32(a0 + 8);
    v10 = r_u32(a0 + 12);
    v11 = r_u32(a0 + 16);
    w_u32(a1, result);
    w_u32(a1 + 4u * (1), v10);
    w_u32(a1 + 4u * (2), v11);
  }
  return result;
}
