#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80026138(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    uint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    uint32 v9 = 0;
    uint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    uint32 v15 = 0;
    sint32 v16 = 0;
    sint32 v17 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
    sint32 v20 = 0;
  uint32 v21; // $t0
  if ( a0 )
  {
    v3 = 180 * r_u8(0x800B63BDu);
    v2 = -2146650852;
  }
  else
  {
    v2 = -2146652292;
    v3 = 180 * r_u8(0x800B63BDu);
  }
  v4 = v3 + v2;
  v5 = v3 + v2 + 20;
  v6 = r_u32(0x800CB8E0u);
  v7 = 0;
  v8 = 0;
  do
  {
    v9 = r_u32(4 * (v8 + v7) + v5 + 4);
    v10 = v9 == 0;
    v11 = r_u32(0x800CB8E0u) < v9;
    if ( v10 )
      break;
    v10 = v11;
    v12 = v7 < 7;
    if ( v10 )
      goto LABEL_9;
    ++v7;
    v8 = 4 * v7;
  }
  while ( v7 < 8 );
  v12 = v7 < 7;
LABEL_9:
  v13 = 7;
  if ( v12 )
  {
    v14 = 28;
    do
    {
      v15 = (uint32)(4 * (v14 + v13) + v5);
      v16 = r_u32(v15 - 4u * 4);
      v17 = r_u32(v15 - 4u * 3);
      v18 = r_u32(v15 - 4u * 2);
    w_u32(v15, r_u32(v15 - 4u * 5));
    w_u32(v15 + 4u * (1), v16);
    w_u32(v15 + 4u * (2), v17);
    w_u32(v15 + 4u * (3), v18);
    w_u32(v15 + 4u * (4), r_u32(v15 - 4u * 1));
      --v13;
      v14 = 4 * v13;
    }
    while ( v7 < v13 );
  }
  if ( v7 >= 8 )
  {
    v7 = -1;
  }
  else
  {
    v19 = 20 * v7 + v5;
    w_u32(v19 + 4, v6);
    w_u16(v19 + 8, r_u16(0x800B6670u));
  }
  v20 = 2 * ((sint8)r_u8(0x800BDD01u) != 0);
  if ( (sint8)r_u8(0x800BDD00u) )
  {
    w_u32(v4 + 4, r_u32(0x800BDB48u));
    v20 |= 1u;
    w_u16(v4 + 8, r_u16(0x800B6670u));
  }
  gt_draft_unresolved_call(0x80022090u, v6, 0, 0, 0, 0, 0, 0, 0);
  v21 = 0x80033C44u;
  if ( !a0 )
    v21 = 0x80033D9Cu;
  gt_draft_unresolved_call(v21, v20, v7, v4, v5, 0, 0, 0, 0);
  return 0;
}
