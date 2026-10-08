#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001E07C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint16 v3 = 0;
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
    sint32 result = 0;
  if ( r_u32(0x800B630Cu) == 1 && (r_u16(r_u32(a0 + 184) + 8) & 0xF000) != 0 )
  {
    v2 = (sint16)r_u16(a0 + 246);
    if ( v2 == r_u32(0x800B630Cu) )
    {
      v3 = 2;
      if ( r_u16(a0 + 248) )
      {
    w_u16(a0 + 248, 0);
        goto LABEL_10;
      }
    }
    else
    {
      v3 = 1;
      if ( v2 != 2 )
        goto LABEL_10;
    w_u16(a0 + 248, 1);
    }
    w_u16(a0 + 246, v3);
  }
LABEL_10:
  v4 = r_u32(a0 + 124);
  v5 = r_u32(a0 + 128);
    w_u32(a0 + 208, r_u32(a0 + 120));
    w_u32(a0 + 212, v4);
    w_u32(a0 + 216, v5);
  v6 = (sint16)r_u16(a0 + 246);
    w_u8(a0 + 254, 0);
    w_u8(a0 + 255, 0);
  if ( v6 == 1 )
  {
    gt_draft_unresolved_call(0x8001CB64u, a0, 0, 0, 0, 0, 0, 0, 0);
  }
  else if ( v6 >= 2 && v6 == 2 )
  {
    gt_draft_unresolved_call(0x8001DEB4u, a0, 0, 0, 0, 0, 0, 0, 0);
  }
  gt_draft_unresolved_call(0x8006B40Cu, a0, -2146697344, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006B474u, a0 + 220, a0 + 120, a0 + 208, 0, 0, 0, 0, 0);
  v8 = gt_draft_unresolved_call(0x8006B4ACu, a0 + 232, a0 + 220, 0, 0, 0, 0, 0, 0);
  v7 = gt_draft_unresolved_call(0x8006B530u, a0 + 232, a0 + 232, 0, 0, 0, 0, 0, 0);
  if ( v8 < 0 )
    v9 = v7 >> v8;
  else
    v9 = v7 << v8;
  v10 = v9 >> 4;
  if ( v10 >= 11605 )
    w_u32(a0 + 240, 0);
  else
    w_u32(a0 + 240, v10);
  v11 = r_u32(a0 + 164);
  v12 = r_u32(a0 + 168);
  v13 = r_u32(a0 + 172);
    w_u32(a0 + 188, r_u32(a0 + 160));
    w_u32(a0 + 192, v11);
    w_u32(a0 + 196, v12);
    w_u32(a0 + 200, v13);
  result = r_u32(a0 + 176);
    w_u32(a0 + 204, result);
  return result;
}
