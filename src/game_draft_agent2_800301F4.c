#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800301F4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    uint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    sint32 v16 = 0;
    sint32 v17 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
    sint8 v20[8] = {0};
    sint8 v21[96] = {0};
  result = r_u8(0x800CB75Cu);
  if ( !r_u8(0x800CB75Cu) )
  {
    v6 = a1 - 16;
    v7 = 0;
    v8 = 0;
    if ( (sint16)r_u16(0x800BC1A6u) > 0 )
    {
      v9 = (sint16)r_u16(0x800BC1A6u);
      v10 = 0;
      do
      {
        v11 = 260 * (4 * (v10 + v8) + v8) - 2146736048;
        v12 = gt_draft_buffer_address(&v20[16 * v8], sizeof(v20)) /* TODO Adapter lifecycle */;
        if ( (r_u32(v11 + 12) & 1) != 0 )
        {
          ++v7;
          v13 = (sint16)r_u16(v11 + 2238);
          v14 = v6 + ((163 * (sint16)r_u16(v11 + 2230)) >> 12) + 48;
    w_u16(v12, v14 - 2);
          v15 = a2 - (((163 * v13) >> 12) - 48);
    w_u32(v12 + 4u * (1), v15 - 2);
    w_u32(v12 + 4u * (2), v14 + 1);
    w_u32(v12 + 4u * (3), v15 + 1);
    w_u32(v12 + 4u * (4), v14 - 1);
    w_u32(v12 + 4u * (5), v15 - 1);
    w_u32(v12 + 4u * (6), v14);
    w_u32(v12 + 4u * (7), v15);
        }
        ++v8;
        v10 = 2 * v8;
      }
      while ( v8 < v9 );
    }
    gt_draft_unresolved_call(0x8006F4F0u, a0, gt_draft_buffer_address(v21, sizeof(v21)) /* TODO Adapter lifecycle */, 255, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006F4F0u, a0, gt_draft_buffer_address(v20, sizeof(v20)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0, 0);
    v16 = 1;
    v17 = v7 > 1;
    v18 = a0;
    if ( v7 > 1 )
    {
      do
      {
        gt_draft_unresolved_call(0x8006F4F0u, v18, gt_draft_buffer_address(&v20[16 * v16++ + 8], sizeof(v20)) /* TODO Adapter lifecycle */, 65280, 0, 0, 0, 0, 0);
        v18 = a0;
      }
      while ( v16 < v7 );
      v16 = 1;
      v17 = v7 > 1;
    }
    if ( v17 )
    {
      v19 = a0;
      do
      {
        gt_draft_unresolved_call(0x8006F4F0u, v19, gt_draft_buffer_address(&v20[16 * v16++], sizeof(v20)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0, 0);
        v19 = a0;
      }
      while ( v16 < v7 );
    }
    gt_draft_unresolved_call(0x800706BCu, a0, (uint16)v6 | ((a2 + 16) << 16), 404361344, 5242976, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x8006F590u, a0, 518, 0, 0, 0, 0, 0, 0);
  }
  return result;
}
