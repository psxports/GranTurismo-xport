#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001D8A0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    uint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint16 v8 = 0;
    sint16 v9 = 0;
    sint16 v10 = 0;
    sint16 v11 = 0;
    sint16 v12 = 0;
    sint32 v13 = 0;
    sint16 v14 = 0;
    sint32 result = 0;
    sint32 v16[4] = {0};
    sint8 v17[24] = {0};
  v2 = r_u32(a0 + 180);
  v3 = (uint32)(a0 + 120);
  gt_draft_8001DF30(a0, (sint16)r_u16(a0 + 262), 0, 0, 0, 0, 0, 0);
  v4 = (sint16)r_u16(a0 + 260);
    w_u16(a0 + 244, r_u16(v2 + 20));
  v5 = v4 + 1;
  if ( v4 == 2 )
    v5 = 2;
  if ( v4 )
  {
    v8 = r_u16(v2 + 80);
    v6 = (v6 & 0xFFFF0000u) | (((uint32)(r_u16(0x8008F24Cu + 2u * (v4))) & 0xFFFFu) << 0);
    v7 = (v7 & 0xFFFF0000u) | (((uint32)(0) & 0xFFFFu) << 0);
    v9 = 0;
    v10 = 0;
  }
  else
  {
    v6 = r_u16(v2 + 2240);
    v7 = r_u16(v2 + 2244);
    v8 = 0;
    if ( r_u8(a0 + 255) )
    {
      v6 = -v6;
      v7 = -v7;
    }
    v9 = v6;
    v10 = v7;
  }
  v11 = v6;
  v12 = v7;
  v13 = (sint8)r_u8(a0 + 255);
  v14 = v8 + r_u16(v2 + 2242);
  if ( v13 )
    v14 = v8 + r_u16(v2 + 2242) + 2048;
  gt_draft_unresolved_call(0x8006B134u, gt_draft_buffer_address(v17, sizeof(v17)) /* TODO Adapter lifecycle */, v14, v9, v10, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006B134u, v3 + 20, v14, v11, v12, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006B0A8u, gt_draft_buffer_address(v16, sizeof(v16)) /* TODO Adapter lifecycle */, (0x8008F228u + 4u * (3 * v4)), (0x8008F228u + 4u * (3 * v5)), 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006B2CCu, gt_draft_buffer_address(v16, sizeof(v16)) /* TODO Adapter lifecycle */, gt_draft_buffer_address(v17, sizeof(v17)) /* TODO Adapter lifecycle */, gt_draft_buffer_address(v16, sizeof(v16)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0);
    w_u32(v3, r_u32(v2 + 2228) + v16[0]);
    w_u32(v3 + 4u * (1), r_u32(v2 + 2232) + v16[1]);
  result = r_u32(v2 + 2236) + v16[2];
    w_u32(v3 + 4u * (2), result);
  return result;
}
