#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80045910(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    uint32 v3 = 0;
    uint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    uint32 v12 = 0;
    uint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    sint32 v16 = 0;
    sint32 v17 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
    sint32 v20 = 0;
    sint32 result = 0;
    sint32 v22 = 0;
    sint8 v23[96] = {0};
  int vars0; // [sp+70h] [+0h] BYREF
  v0 = (uint32)r_u32(0x800BDD08u);
  gt_draft_unresolved_call(0x8003A304u, 0, 0, 0, 0, 0, 0, 0, 0);
  v1 = r_u32(0x801D2FDCu);
  v2 = 0;
  if ( r_u32(0x801D2FDCu) >= 0 )
  {
    v3 = v23;
    if ( r_u8(r_u32(0x801D2FDCu) - 2145570852 + 4) )
    {
      v4 = (uint32)r_u32(0x800BEA64u);
      v5 = r_u32(0x800BEA64u) + 96;
      if ( ((r_u32(0x800BEA64u) | (uint8)gt_draft_buffer_address(v23, sizeof(v23)) /* TODO Adapter lifecycle */) & 3) != 0 )
      {
        do
        {
          v6 = r_u32(v4 + 4u * (1));
          v7 = r_u32(v4 + 4u * (2));
          v8 = r_u32(v4 + 4u * (3));
    w_u32(v3, r_u32(v4));
          w_u32(v3 + 4u, v6);
          w_u32(v3 + 8u, v7);
          w_u32(v3 + 12u, v8);
          v4 += 16;
          v3 += 16;
        }
        while ( v4 != (uint32)v5 );
      }
      else
      {
        do
        {
          v9 = r_u32(v4 + 4u * (1));
          v10 = r_u32(v4 + 4u * (2));
          v11 = r_u32(v4 + 4u * (3));
    w_u32(v3, r_u32(v4));
          w_u32(v3 + 4u, v9);
          w_u32(v3 + 8u, v10);
          w_u32(v3 + 12u, v11);
          v4 += 16;
          v3 += 16;
        }
        while ( v4 != (uint32)v5 );
      }
      gt_draft_8004AB30(r_u32(0x800B5374u), r_u16(8 * v1 - 2146741484), 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x80042C48u, r_u32(0x800B5374u), 0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8001EB08u, 0, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x80058BB0u, -2146715568, 60 * v1 - 2145569800, 108 * v1 - 2145570664, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8004D78Cu, -2145570840, 60 * v1 - 2145569800, 108 * v1 - 2145570664, 0, 0, 0, 0, 0);
      v12 = (uint32)r_u32(0x800BEA64u);
      v13 = (uint32)v23;
      if ( (((uint8)v23 | r_u32(0x800BEA64u)) & 3) != 0 )
      {
        do
        {
          v14 = (sint32)r_u32(v13 + 4u * (1));
          v15 = (sint32)r_u32(v13 + 4u * (2));
          v16 = (sint32)r_u32(v13 + 4u * (3));
    w_u32(v12, (sint32)r_u32(v13));
    w_u32(v12 + 4u * (1), v14);
    w_u32(v12 + 4u * (2), v15);
    w_u32(v12 + 4u * (3), v16);
          v13 += 16;
          v12 += 16;
        }
        while ( v13 != &vars0 );
      }
      else
      {
        do
        {
          v17 = (sint32)r_u32(v13 + 4u * (1));
          v18 = (sint32)r_u32(v13 + 4u * (2));
          v19 = (sint32)r_u32(v13 + 4u * (3));
    w_u32(v12, (sint32)r_u32(v13));
    w_u32(v12 + 4u * (1), v17);
    w_u32(v12 + 4u * (2), v18);
    w_u32(v12 + 4u * (3), v19);
          v13 += 16;
          v12 += 16;
        }
        while ( v13 != &vars0 );
      }
      gt_draft_unresolved_call(0x8001EB08u, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    v20 = v1 + 1;
    if ( v20 >= r_u32(0x801D2FDEu) )
    v20 = (v20 & 0xFFFF0000u) | (((uint32)(-1) & 0xFFFFu) << 0);
    w_u32(0x801D2FDCu, v20);
  }
  result = 0;
  if ( r_u8(v0) )
  {
    gt_draft_unresolved_call(0x80035388u, 1, 0, 0, 0, 0, 0, 0, 0);
    v22 = gt_draft_unresolved_call(0x8003B954u, 0x80093530u, (sint32)(v0 + 2), 0, 0, 0, 0, 0, 0);
    if ( v22 == -2 )
    {
      gt_draft_unresolved_call(0x80035778u, 2, 0, 0, 0, 0, 0, 0, 0);
      return 1;
    }
    else if ( v22 != -1 )
    {
      v2 = 1;
      gt_draft_8004AB30(r_u32(0x800B5374u), r_u16(8 * v22 - 2146741484), 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x80042C48u, r_u32(0x800B5374u), 0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8001EB08u, 0, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x80035778u, 4, 0, 0, 0, 0, 0, 0, 0);
    }
    return v2;
  }
  return result;
}
