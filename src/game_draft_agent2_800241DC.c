#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800241DC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    sint32 v3 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    uint32 v7 = 0;
    uint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint16 v13 = 0;
    sint32 v14 = 0;
    sint32 v17 = 0;
    sint32 v22 = 0;
    sint32 v23 = 0;
    sint32 v27 = 0;
    sint32 v28 = 0;
    sint32 v29 = 0;
    sint32 v33 = 0;
    uint32 carrier_a0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_a1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s3 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s5 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s6 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t7 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
  result = r_u32(a1 + 4u * (38));
  v3 = r_u32(result);
  carrier_s6 = (uint32)(result + 4);
  if ( r_u32(result) )
  {
    v5 = 4;
    v6 = a0;
    v7 = r_u32(a1 + 4u * (12)) & 0xFFF00000;
    v8 = r_u32(a1 + 4u * (14)) & 0xFFF00000;
    do
    {
      v9 = (sint32)(r_u32(v6) - v7) >> 8;
      v10 = (sint32)(r_u32(v6 + 4) - v8) >> 8;
      v11 = (sint32)(r_u32(v6 + 12) - v7) >> 8;
      v12 = (sint32)(r_u32(v6 + 16) - v8) >> 8;
      v13 = v11 - v9;
      v14 = v12 - v10;
    w_u16(v6 + 32, v9);
    w_u16(v6 + 34, v10);
    w_u16(v6 + 36, v12);
    w_u16(v6 + 38, v11);
      if ( (uint32)(v9 + 0x7FFF) > 0xFFFE
        || (uint32)(v11 + 0x7FFF) > 0xFFFE
        || (uint32)(v10 + 0x7FFF) > 0xFFFE
        || (uint32)(v12 + 0x7FFF) > 0xFFFE )
      {
        v14 = 0;
        v13 = 0;
      }
      --v5;
      result = -v14;
    w_u16(v6 + 40, -(sint16)v14);
    w_u16(v6 + 42, v13);
      v6 += 44;
    }
    while ( v5 );
    carrier_t7 = 0;
gt_draft_gte_write(1, 0, carrier_t7);
gt_draft_gte_write(3, 0, carrier_t7);
gt_draft_gte_write(4, 1, carrier_t7);
gt_draft_gte_write(12, 1, carrier_t7);
    carrier_s1 = a0;
    v17 = r_u32(a1 + 4u * (41));
    carrier_s5 = r_u32(a0 + 32);
    for ( carrier_s3 = r_u32(a0 + 36); v3; carrier_s6 += 8 )
    {
      --v3;
      carrier_v1 = r_u32(8 * (uint16)(sint32)r_u32(carrier_s6) + v17);
      carrier_v0 = r_u32(8 * (((uint32)((sint32)r_u32(carrier_s6)) >> 16) & 0xffffu) + v17);
gt_draft_gte_write(0, 0, r_u32(carrier_s6 + (4)));
gt_draft_gte_write(3, 1, carrier_v1);
gt_draft_gte_write(8, 1, carrier_v1);
gt_draft_gte_write(11, 1, carrier_v0);
      v22 = 4;
      do
      {
gt_draft_gte_write(0, 1, carrier_s5);
gt_draft_gte_write(1, 1, carrier_s3);
gt_draft_gte_write(2, 1, carrier_s3);
        --v22;
gt_draft_gte_command(0x406012);
gt_draft_gte_write(9, 1, carrier_s3);
gt_draft_gte_write(10, 1, carrier_s3);
gt_draft_gte_write(2, 0, r_u32(carrier_s1 + (0x28)));
        v23 = a0;
        if ( v22 )
          v23 = carrier_s1 + 176;
        carrier_s5 = r_u32(v23 + 32);
        carrier_s3 = r_u32(v23 + 36);
carrier_a0 = gt_draft_gte_read(27, 0);
carrier_a1 = gt_draft_gte_read(25, 0);
carrier_v1 = gt_draft_gte_read(26, 0);
        v27 = carrier_a1 - carrier_a0;
        v28 = carrier_v1 - carrier_a0;
gt_draft_gte_command(0x42E012);
        result = v27 ^ v28;
        v29 = v27 - v28;
        if ( (v27 ^ v28) < 0 )
        {
carrier_v1 = gt_draft_gte_read(26, 0);
carrier_a0 = gt_draft_gte_read(25, 0);
carrier_v0 = gt_draft_gte_read(27, 0);
          result = (carrier_a0 - carrier_v1) ^ (carrier_v0 - carrier_v1);
          if ( result < 0 )
          {
            v33 = r_u32(carrier_s1 + 96);
            result = gt_draft_unresolved_call(0x8001E4A8u, v27 << 12, (uint64)v27 >> 20, v29, 0, 0, 0, 0, 0);
            if ( result < v33 )
            {
    w_u32(carrier_s1 + 96, result);
w_u32(carrier_s1 + (0x1C), gt_draft_gte_read(0, 0));
              result = -(sint16)r_u16(carrier_s1 + 112);
    w_u16(carrier_s1 + 112, r_u16(carrier_s1 + 120));
    w_u16(carrier_s1 + 120, result);
            }
          }
        }
        carrier_s1 = v23;
      }
      while ( v22 );
    }
  }
  return result;
}
