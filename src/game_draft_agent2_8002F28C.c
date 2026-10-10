#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002F28C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v2 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint64 v16 = 0;
    sint32 v17 = 0;
    sint32 v18 = 0;
    sint32 v20 = 0;
    sint32 v21 = 0;
    sint32 v22 = 0;
    sint32 v23 = 0;
    sint32 v24 = 0;
    sint32 v27 = 0;
    sint32 v28 = 0;
    sint16 v29 = 0;
    sint32 v30 = 0;
    sint32 v31 = 0;
    sint32 v32 = 0;
    sint16 v36 = 0;
    sint16 v37 = 0;
    sint16 v38 = 0;
    sint16 v39 = 0;
    sint16 v40 = 0;
    sint16 v41 = 0;
    sint32 v42 = 0;
    uint32 v48 = 0;
    uint32 v49 = 0;
    sint32 v50 = 0;
    sint32 v51 = 0;
    sint32 v52 = 0;
    sint32 v53 = 0;
    sint32 v54 = 0;
    sint32 v55 = 0;
    sint32 v56 = 0;
    uint32 carrier_s0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s7 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
  v2 = (uint32)(a1 + 4);
  v50 = 0;
  v51 = r_u32(a1);
  if ( (sint32)r_u32(a1) > 0 )
  {
    carrier_s0 = a0;
    v52 = a0 + 892;
    v53 = a0 + 64;
    v54 = a0 + 96;
    v55 = a0 + 892;
    v56 = a0 + 268;
    do
    {
      v5 = r_u32(v2 + 20u);
      v6 = r_u32(v2 + 24u);
    w_u32(a0 + 872, r_u32(v2 + 16u));
    w_u32(a0 + 876, v5);
    w_u32(a0 + 880, v6);
      gtmain_model_rotation(psx_addr(v52, 18u), (sint16)r_u16(v2 + 2u * (1)), (sint16)r_u16(v2), (sint16)r_u16(v2 + 2u * (2)));
      gtmain_matrix_scale_axes(psx_addr(v52, 20u), psx_addr(v52, 20u), psx_addr(v2 + 8u, 8u));
      v7 = r_u32(a0 + 872) - r_u32(a0 + 44);
      v8 = r_u32(a0 + 876) - r_u32(a0 + 48);
      v9 = (sint16)r_u16(v2 + 2u * (7));
      v10 = r_u32(a0);
      v11 = r_u32(4 * (sint16)r_u16(v2 + 2u * (3)) + r_u32(r_u32(0x800BC1B0u) + 20) + 4);
      v12 = r_u32(a0 + 880);
      v13 = r_u32(a0 + 52);
    w_u32(a0 + 248, v7);
    w_u32(a0 + 252, v8);
      v14 = (sint32)((uint32)v12 - (uint32)v13);
    w_u32(a0 + 256, (uint32)v12 - (uint32)v13);
      carrier_s7 = 16 * v9;
      v16 = 16 * v9 * (sint64)(sint32)xport_load_le32(gtmain_render_source(a0) + 28u);
      v17 = gtmain_model_range_index(v11, psx_addr(a0 + 248u, 12u), (sint32)(uint32)(v16 >> 12));
      if ( v17 != -1 )
      {
        v18 = r_u32(v11 + 8 * v17 + 8);
xport_gte_write_data(30u, carrier_s7);
carrier_v1 = xport_gte_read_data(31u);
        v20 = 31 - carrier_v1;
        v21 = v7;
        if ( v7 < 0 )
          v21 = (sint32)(0u - (uint32)v7);
        v22 = v8;
        if ( v8 < 0 )
          v22 = (sint32)(0u - (uint32)v8);
        v23 = v21 | v22;
        v24 = v14;
        if ( v14 < 0 )
          v24 = (sint32)(0u - (uint32)v14);
        carrier_v1 = v23 | v24;
xport_gte_write_data(30u, carrier_v1);
carrier_v0 = xport_gte_read_data(31u);
        v27 = 31 - carrier_v0;
        v28 = (sint16)r_u16(v18 + 84) + v20 - 16;
        v29 = r_u16(v18 + 84) + v20 - 16;
        if ( v27 < v28 )
          v27 = (sint16)r_u16(v18 + 84) + v20 - 16;
        v30 = v27 - v28;
        v31 = v27 - 12;
        if ( v31 < 0 )
    v31 = 0;
        v32 = v20 - 12;
    w_u16(a0 + 40, v31);
    w_u16(a0 + 42, v30);
    w_u16(a0 + 260, v7 >> ((uint32)v31 & 31u));
    w_u16(a0 + 262, v8 >> ((uint32)v31 & 31u));
    w_u16(a0 + 264, v14 >> ((uint32)v31 & 31u));
    w_u16(a0 + 864, 20 - v28 - v30);
        if ( v32 <= 0 )
        {
          if ( v32 < 0 )
            carrier_s7 <<= (0u - (uint32)v32) & 31u;
        }
        else
        {
          carrier_s7 = (uint32)((sint32)carrier_s7 >> ((uint32)v32 & 31u));
        }
        gtmain_matrix_bind_packed_negation(psx_addr(v53, 20u));
xport_gte_write_data(0u, r_u32(carrier_s0 + (0x104)));
xport_gte_write_data(1u, r_u32(carrier_s0 + (0x108)));
xport_gte_execute(0x486012u);
w_u32(carrier_s0 + (0x54), xport_gte_read_data(9u));
w_u32(carrier_s0 + (0x58), xport_gte_read_data(10u));
w_u32(carrier_s0 + (0x5C), xport_gte_read_data(11u));
        carrier_v1 = r_u32(a0 + 84);
        carrier_v0 = r_u32(a0 + 88);
gte_ctc2(carrier_v1, 5u);
        carrier_v1 = r_u32(a0 + 92);
gte_ctc2(carrier_v0, 6u);
gte_ctc2(carrier_v1, 7u);
        gtmain_matrix_multiply(psx_addr(v54, 20u), psx_addr(v53, 20u), psx_addr(v55, 20u));
        gtmain_matrix_scale_gte(psx_addr(v54, 20u), psx_addr(v54, 20u), (uint16)carrier_s7);
        gtmain_matrix_shift(psx_addr(v54, 18u), psx_addr(v54, 18u), v30);
        gtmain_model_matrix_bind(a0, psx_addr(v54, 20u));
        gtmain_lighting_bind(a0);
        v36 = r_u16(v18 + 68);
        v37 = r_u16(v18 + 70);
        v38 = r_u16(v18 + 72);
        v39 = r_u16(v18 + 76);
        v40 = r_u16(v18 + 78);
        v41 = r_u16(v18 + 80);
    w_u16(a0 + 184, v36);
    w_u16(a0 + 186, v37);
    w_u16(a0 + 188, v38);
    w_u16(a0 + 192, v39);
    w_u16(a0 + 194, v37);
    w_u16(a0 + 196, v38);
    w_u16(a0 + 200, v36);
    w_u16(a0 + 202, v40);
    w_u16(a0 + 204, v38);
    w_u16(a0 + 208, v39);
    w_u16(a0 + 210, v40);
    w_u16(a0 + 212, v38);
    w_u16(a0 + 216, v36);
    w_u16(a0 + 218, v37);
    w_u16(a0 + 220, v41);
    w_u16(a0 + 224, v39);
    w_u16(a0 + 226, v37);
    w_u16(a0 + 228, v41);
    w_u16(a0 + 232, v36);
    w_u16(a0 + 234, v40);
    w_u16(a0 + 236, v41);
    w_u16(a0 + 240, v39);
    w_u16(a0 + 242, v40);
    w_u16(a0 + 244, v41);
        v42 = gtmain_model_bounds_classify(a0, psx_addr(a0 + 184u, 76u), 8u);
        if ( (v42 & 0x1F) == 0 )
        {
          gtmain_matrix_transpose(psx_addr(v56, 18u), psx_addr(v55, 18u));
          gtmain_matrix_scale_gte(psx_addr(v56, 20u), psx_addr(v56, 20u), (uint16)(0u - carrier_s7));
          gtmain_matrix_rotate_short(psx_addr(a0 + 56u, 8u), psx_addr(v56, 20u), psx_addr(a0 + 260u, 8u));
          carrier_v0 = r_u32(a0 + 96);
          carrier_v1 = r_u32(a0 + 100);
gte_ctc2(carrier_v0, 0u);
          carrier_v0 = r_u32(a0 + 104);
gte_ctc2(carrier_v1, 1u);
          carrier_v1 = r_u32(a0 + 108);
gte_ctc2(carrier_v0, 2u);
          carrier_v0 = r_u32(a0 + 112);
gte_ctc2(carrier_v1, 3u);
gte_ctc2(carrier_v0, 4u);
    w_u32(a0 + 912, v18);
    w_u32(a0 + 916, r_u32(v18));
    w_u32(a0 + 932, r_u32(v18 + 4));
          if ( ((v42 >> 16) & 0x3F) != 0 )
            v48 = gtmain_model_depth_cued_clipped_draw(a0, (uint32)v18);
          else
            v48 = gtmain_model_depth_cued_draw(a0, (uint32)v18);
          v49 = gtmain_model_sprites_draw(a0, r_u32(a0 + 932), r_u16(v48 + 64u));
          gtmain_model_radial_effect_draw(a0, v49, r_u16(v48 + 66u));
        }
      }
      v2 += 28;
      ++v50;
    }
    while ( v50 < v51 );
  }
    /* TODO Original void-like return carrier requires ABI refinement */
    return 0;
}
