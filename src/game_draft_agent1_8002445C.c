#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002445C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v2 = 0;
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
  v2 = r_u32(r_u32(0x800BC1B0u) + 24);
  v3 = r_u32(r_u32(0x800BC1A8u));
  v4 = r_u16(v2 + 2u * (1));
  if ( r_u16(v2 + 2u * (1)) && v4 < a0 )
  {
    v5 = a0 - 1;
    if ( !r_u16(v2 + 2u * (1)) )
      gt_draft_exception(7u);
    a0 = v5 % v4 + 1;
  }
  v6 = 0;
  if ( r_u16(v2) )
  {
    v7 = 0;
    do
    {
      v8 = r_u32((v2 + 2u * (v7 + 2)));
      v9 = r_u16(v8 + 2);
      if ( a0 < v9 || r_u16(v8 + 4) < a0 )
        goto LABEL_17;
      v10 = r_u32(v8 + 12);
      v11 = r_u32(v8 + 8);
      v12 = a1;
      if ( v10 < v11 )
        v10 += v3;
      v13 = a1 < v11;
      if ( v9 < a0 )
      {
        v12 = a1 + v3;
        v13 = a1 + v3 < v11;
      }
      if ( v13 )
      {
LABEL_17:
        ++v6;
      }
      else
      {
        ++v6;
        if ( v12 < v10 )
          return v8;
      }
      v7 = 2 * v6;
    }
    while ( v6 < r_u16(v2) );
  }
  return 0;
}
