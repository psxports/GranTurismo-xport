#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80046BBC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v15 = 0;
    sint32 v16 = 0;
    sint32 v17 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
    sint32 v20 = 0;
    sint32 v21 = 0;
    sint32 v22 = 0;
    sint32 v23 = 0;
    sint32 v24 = 0;
    sint32 v25 = 0;
    sint32 v26 = 0;
    sint16 v27 = 0;
    sint16 v28 = 0;
    uint8 v29 = 0;
    sint32 v30 = 0;
    sint32 v32 = 0;
    sint32 v33 = 0;
    sint32 v34 = 0;
    sint32 v35 = 0;
    sint32 v36 = 0;
    sint16 v37 = 0;
    sint16 v38 = 0;
    sint16 v39 = 0;
  if ( r_u32(a2 + 52u) > 0 )
    w_u32(a2 + 52u, r_u32(a2 + 52u)-1u);
  if ( a6 )
    v15 = a2 - 160 * r_u32(a2 + 52u) / 12;
  else
    v15 = a2 + 160 * r_u32(a2 + 52u) / 12;
  v36 = (a4 >> 1) * (12 - r_u32(a2 + 52u)) / 12;
  v16 = gt_draft_unresolved_call(0x80035458u, v36, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v16, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  v17 = (sint32)r_u32(a2);
  v18 = gt_draft_unresolved_call(0x8006C900u, 1, (sint32)r_u32(a2), 0, 0, 0, 0, 0, 0);
  if ( a7 )
    v19 = a3 + 20;
  else
    v19 = a3 - 12;
  gt_draft_unresolved_call(0x8006CA10u, a0, (sint16)v15 - (v18 >> 1), v19, v17, 0, 0, 0, 0);
  v37 = v15 - 96;
  v38 = v15 - 1;
  v20 = (sint32)r_u32(a2 + 4u * (1));
  v39 = v15 + 95;
  v21 = gt_draft_unresolved_call(0x8006C900u, 1, v20, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, a0, (sint16)v15 - ((v21 + 96) >> 1), a3 + 3, v20, 0, 0, 0, 0);
  v22 = (sint32)r_u32(a2 + 4u * (2));
  v23 = gt_draft_unresolved_call(0x8006C900u, 1, v22, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, a0, (sint16)v15 + ((96 - v23) >> 1), a3 + 3, v22, 0, 0, 0, 0);
    v34 = (v34 & 0x0000FFFFu) | (((uint32)(a3) & 0xFFFFu) << 16);
    v35 = (v35 & 0x0000FFFFu) | (((uint32)(a3 + 15) & 0xFFFFu) << 16);
    v34 = (v34 & 0xFFFF0000u) | (((uint32)(v15 - 96) & 0xFFFFu) << 0);
  v24 = (51 * v36) >> 7;
    v35 = (v35 & 0xFFFF0000u) | (((uint32)(v15 - 1) & 0xFFFFu) << 0);
  v32 = v34;
  v33 = v35;
  v25 = gt_draft_unresolved_call(0x80035458u, v24, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F624u, a0, gt_draft_buffer_address(&v32, sizeof(v32)) /* TODO Adapter lifecycle */, v25, 0, 0, 0, 0, 0);
    v34 = (v34 & 0xFFFF0000u) | (((uint32)(v15) & 0xFFFFu) << 0);
    v34 = (v34 & 0x0000FFFFu) | (((uint32)(a3) & 0xFFFFu) << 16);
    v35 = (v35 & 0x0000FFFFu) | (((uint32)(a3 + 15) & 0xFFFFu) << 16);
    v35 = (v35 & 0xFFFF0000u) | (((uint32)(v39) & 0xFFFFu) << 0);
  v32 = v34;
  v33 = v35;
  v26 = gt_draft_unresolved_call(0x80035458u, v24, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F624u, a0, gt_draft_buffer_address(&v32, sizeof(v32)) /* TODO Adapter lifecycle */, v26, 0, 0, 0, 0, 0);
  v27 = v15 - 96;
  if ( !r_u32(a2 + 52u) )
  {
    if ( r_u32(a2 + 48u) )
    {
      v28 = v39;
    v34 = (v34 & 0xFFFF0000u) | (((uint32)(v15) & 0xFFFFu) << 0);
    }
    else
    {
    v34 = (v34 & 0xFFFF0000u) | (((uint32)(v37) & 0xFFFFu) << 0);
      v28 = v38;
    }
    v34 = (v34 & 0x0000FFFFu) | (((uint32)(a3) & 0xFFFFu) << 16);
    v35 = (v35 & 0x0000FFFFu) | (((uint32)(a3 + 15) & 0xFFFFu) << 16);
    v35 = (v35 & 0xFFFF0000u) | (((uint32)(v28) & 0xFFFFu) << 0);
    v32 = v34;
    v33 = v35;
    v29 = gt_draft_unresolved_call(0x80035608u, 2, a4, 0, 0, 0, 0, 0, 0);
    if ( a5 == 1 )
      v30 = v29 << 16;
    else
      v30 = v29;
    gt_draft_unresolved_call(0x8006F4F0u, a0, gt_draft_buffer_address(&v32, sizeof(v32)) /* TODO Adapter lifecycle */, v30, 0, 0, 0, 0, 0);
    v27 = v15 - 96;
  }
    v34 = (v34 & 0xFFFF0000u) | (((uint32)(v27) & 0xFFFFu) << 0);
    v34 = (v34 & 0x0000FFFFu) | (((uint32)(a3) & 0xFFFFu) << 16);
    v35 = (v35 & 0xFFFF0000u) | (((uint32)(v15 + 95) & 0xFFFFu) << 0);
    v35 = (v35 & 0x0000FFFFu) | (((uint32)(a3 + 15) & 0xFFFFu) << 16);
  v32 = v34;
  v33 = v35;
  gt_draft_unresolved_call(0x8006F4F0u, a0, gt_draft_buffer_address(&v32, sizeof(v32)) /* TODO Adapter lifecycle */, 0x2000000, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8006F590u, a0, 512, 0, 0, 0, 0, 0, 0);
}
