#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003C990(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint16 v15 = 0;
    sint16 v16 = 0;
    uint8 v17 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
    sint32 v20 = 0;
    sint32 v21 = 0;
    sint32 v22 = 0;
    sint32 v23 = 0;
    sint32 v24 = 0;
    sint32 v26 = 0;
    sint32 v27 = 0;
    sint32 v28 = 0;
    sint32 v29 = 0;
    sint32 v30 = 0;
    uint32 v31 = 0;
    sint32 v32 = 0;
    sint16 v33 = 0;
    sint16 v34 = 0;
  v31 = a0;
  v32 = a3;
  v11 = gt_draft_unresolved_call(0x8006C900u, 2, -2146711198, 0, 0, 0, 0, 0, 0);
  v33 = a2 - ((v11 >> 1) - 128);
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  v12 = gt_draft_unresolved_call(0x80035458u, a4 >> 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v12, 0, 0, 0, 0, 0, 0, 0);
  v13 = r_u16(a1 + 8);
  v34 = v32 + 141;
  if ( v13 )
    gt_draft_unresolved_call(0x8003C6C8u, v31, (uint32)a1, a2 + 128, v32, a4, v26, v27, v28);
  else
    gt_draft_unresolved_call(0x8003C804u, (sint32)v31, (uint32)a1, a2 + 128, v32, a4, v26, v27, v28);
  v14 = (sint8)r_u8(a1 + 13);
  if ( v14 < 6 )
  {
    v15 = a2 + ((sint16)(r_u8(a1 + 12) << 8) >> 4) - 4;
    v29 = (v29 & 0x0000FFFFu) | (((uint32)(v32 + 17 * v14 + 36) & 0xFFFFu) << 16);
    v29 = (v29 & 0xFFFF0000u) | (((uint32)(v15) & 0xFFFFu) << 0);
    v16 = r_u8(a1 + 12);
    v30 = (v30 & 0x0000FFFFu) | (((uint32)((((uint32)(v29) >> 16) & 0xFFFFu) + 18) & 0xFFFFu) << 16);
    v30 = (v30 & 0xFFFF0000u) | (((uint32)(a2 + ((sint16)(v16 << 8) >> 4) + 14) & 0xFFFFu) << 0);
  }
  else
  {
    v29 = (v29 & 0xFFFF0000u) | (((uint32)(v33 - 2) & 0xFFFFu) << 0);
    v30 = (v30 & 0xFFFF0000u) | (((uint32)(v33 + v11 + 1) & 0xFFFFu) << 0);
    v29 = (v29 & 0x0000FFFFu) | (((uint32)(v34) & 0xFFFFu) << 16);
    v30 = (v30 & 0x0000FFFFu) | (((uint32)(v34 + 18) & 0xFFFFu) << 16);
  }
  v27 = v29;
  v28 = v30;
  v17 = gt_draft_unresolved_call(0x80035608u, 2, a4, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F4F0u, v31, gt_draft_buffer_address(&v27, sizeof(v27)) /* TODO Adapter lifecycle */, v17 | 0x3000000, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F590u, v31, 544, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
  v18 = 0;
  v19 = 36;
  do
  {
    v20 = 16;
    if ( v18 == 5 )
      v20 = 15;
    v21 = 0;
    v22 = (v32 + v19) << 16;
    v23 = 4;
    do
    {
    w_u8(0x80092240u, gt_draft_unresolved_call(0x8003C37Cu, v21, (sint8)v18, 0, 0, 0, 0, 0, 0));
      v24 = gt_draft_unresolved_call(0x8006C900u, 2, 0x80092240u, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8006CA10u, v31, a2 + v23 - (v24 >> 1), v22 >> 16, 0x80092240u, 0, 0, 0, 0);
      ++v21;
      v23 += 16;
    }
    while ( v21 < v20 );
    ++v18;
    v19 += 17;
  }
  while ( v18 < 6 );
  return gt_draft_unresolved_call(0x8006CA10u, v31, v33, v34, -2146711198, 0, 0, 0, 0);
}
