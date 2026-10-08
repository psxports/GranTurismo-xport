#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001DB0C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint16 v5 = 0;
    sint32 v6 = 0;
    uint32 v7 = 0;
    sint16 v8 = 0;
    sint16 v9 = 0;
    sint16 v10 = 0;
    sint16 v11 = 0;
    sint32 result = 0;
    sint16 v13 = 0;
    sint16 v14 = 0;
    sint8 v15[24] = {0};
    sint32 v16[4] = {0};
  v5 = a2;
  v6 = r_u32(a0 + 180);
  gt_draft_8001DF30(a0, a2, 0, 0, 0, 0, 0, 0);
  v7 = (uint32)(a0 + 120);
    w_u16(a0 + 244, r_u16(v6 + 20));
  v8 = r_u16(v6 + 2240);
  v9 = r_u16(v6 + 2244);
  v10 = r_u16(v6 + 80);
  v11 = r_u16(v6 + 2242);
  switch ( a1 )
  {
    case 0:
    w_u16(a0 + 260, 0);
    w_u8(a0 + 254, 1);
    w_u16(a0 + 262, v5);
      return gt_draft_8001D8A0(a0, 0, 0, 0, 0, 0, 0, 0);
    case 1:
    case 2:
    case 3:
      v9 = 0;
      v8 = 0;
      goto LABEL_5;
    case 4:
    case 5:
      v10 = 0;
      goto LABEL_5;
    default:
LABEL_5:
      v13 = v11 + v10 + r_u16(0x8008F2DCu + 2u * (8 * a1));
      v14 = v8 + r_u16(0x8008F2DEu + 2u * (8 * a1));
      gt_draft_unresolved_call(0x8006B134u, gt_draft_buffer_address(v15, sizeof(v15)) /* TODO Adapter lifecycle */, (sint16)(v11 + v10), v8, v9, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8006B134u, v7 + 5, v13, v14, v9, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8006B2CCu, gt_draft_buffer_address(v16, sizeof(v16)) /* TODO Adapter lifecycle */, gt_draft_buffer_address(v15, sizeof(v15)) /* TODO Adapter lifecycle */, (0x8008F2E0u + 4u * (4 * a1)), 0, 0, 0, 0, 0);
    w_u32(v7, r_u32(v6 + 2228) + v16[0]);
    w_u32(v7 + 4u * (1), r_u32(v6 + 2232) + v16[1]);
      result = r_u32(v6 + 2236) + v16[2];
    w_u32(v7 + 4u * (2), result);
      return result;
  }
}
