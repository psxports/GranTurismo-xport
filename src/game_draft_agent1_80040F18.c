#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80040F18(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    uint32 v7 = 0;
    uint32 v8 = 0;
    sint32 result = 0;
    sint16 v10 = 0;
    sint16 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint8 v14 = 0;
    sint32 v15 = 0;
    sint32 v16 = 0;
    uint32 v17 = 0;
    sint32 v18 = 0;
    sint8 v19 = 0;
    sint32 v20 = 0;
    sint32 v21 = 0;
    uint32 v22 = 0;
    sint32 v23 = 0;
  v3 = r_u8(a1 + 1u * (1));
  v4 = r_u8(a1 + 1u * (12)) & 0x1F;
  v5 = r_u32(a1 + 8u);
  v6 = -1;
  if ( !r_u8(a1 + 1u * (14)) )
  {
    w_u8(a1 + 1u * (13), v3);
    v10 = r_u16(a1 + 4);
    if ( (v10 & 0x6000) != 0 )
      return (sint8)v3;
    if ( (r_u16(a1 + 4) & 0x9000) != 0 )
      return -2;
    v11 = v10 | r_u16(a1 + 8);
    if ( v4 )
    {
      v12 = v11 & 0x80;
      if ( v4 == 1 )
      {
        v12 = v11 & 0x10;
        v13 = v11 & 0x40;
        goto LABEL_13;
      }
    }
    else
    {
      v12 = v11 & 0x80;
    }
    v13 = v11 & 0x20;
LABEL_13:
    if ( v12 != 0 && v13 != 0 )
      return v6;
    if ( v12 )
    {
      gt_draft_80031370(1, 0, 0, 0, 0, 0, 0, 0);
      if ( v4 )
      {
        v14 = 3;
        if ( v4 != 1 )
        {
          v15 = v3 - 1;
          do
          {
LABEL_20:
            v3 = v15;
            v8 = (v15 & 0x80) == 0;
            v16 = v15 << 24;
            if ( !v8 )
            {
              v3 = r_u8(a1) - 1;
              v16 = v3 << 24;
            }
            v17 = (uint32)(12 * (v16 >> 24) + v5);
            v15 = v3 - 1;
          }
          while ( (r_u8(v17) & 0x40) != 0 );
          v18 = v3 << 24;
          goto LABEL_35;
        }
      }
      else
      {
        v14 = 1;
      }
    w_u8(a1 + 1u * (14), v14);
      v15 = v3 - 1;
      goto LABEL_20;
    }
    if ( !v13 )
    {
LABEL_34:
      v18 = v3 << 24;
LABEL_35:
      v23 = (sint8)r_u8(a1 + 1u * (13));
    w_u8(a1 + 1u * (1), v3);
      if ( v23 != v18 >> 24 )
        return -3;
      return v6;
    }
    gt_draft_80031370(1, 0, 0, 0, 0, 0, 0, 0);
    if ( v4 )
    {
      v19 = 4;
      if ( v4 != 1 )
      {
LABEL_30:
        v20 = v3 + 1;
        do
        {
          v3 = v20;
          v8 = (sint8)v20 < (sint8)r_u8(a1);
          v21 = v20 << 24;
          if ( !v8 )
          {
            v3 = 0;
            v21 = 0;
          }
          v22 = (uint32)(12 * (v21 >> 24) + v5);
          v20 = v3 + 1;
        }
        while ( (r_u8(v22) & 0x40) != 0 );
        goto LABEL_34;
      }
    }
    else
    {
      v19 = 2;
    }
    w_u8(a1 + 1u * (14), v19);
    w_u8(a1 + 1u * (15), 0);
    goto LABEL_30;
  }
  v7 = r_u8(a1 + 1u * (15)) + 1;
    w_u8(a1 + 1u * (15), v7);
  v8 = v7 < 12;
  result = -1;
  if ( !v8 )
  {
    w_u8(a1 + 1u * (14), 0);
    w_u8(a1 + 1u * (15), 0);
  }
  return result;
}
