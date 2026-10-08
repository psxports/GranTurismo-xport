#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800446DC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v14 = 0;
    sint32 v15 = 0;
    uint32 v16 = 0;
    sint32 v17 = 0;
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
    sint8 v30[112] = {0};
    sint8 v31 = 0;
    sint8 v32[4] = {0};
    sint32 v33[2] = {0};
    sint32 v34 = 0;
    sint32 v35 = 0;
    sint32 v36 = 0;
    uint32 v37 = 0;
  v15 = 386 * a4 - 2146715568;
  v14 = gt_draft_unresolved_call(0x8006C1CCu, r_u16(76 * a4 - 2146736528), 0, 0, 0, 0, 0, 0, 0);
  v16 = (uint32)r_u32(0x80092D5Cu + 4u * (r_u8(v15 + 138)));
    v34 = (v34 & 0xFFFF0000u) | (((uint32)(a1 + 24) & 0xFFFFu) << 0);
    v34 = (v34 & 0x0000FFFFu) | (((uint32)(a2 + 25) & 0xFFFFu) << 16);
    v35 = (v35 & 0xFFFF0000u) | (((uint32)(a1 + 343) & 0xFFFFu) << 0);
    v35 = (v35 & 0x0000FFFFu) | (((uint32)(a2 + 25) & 0xFFFFu) << 16);
  v37 = v16;
  v33[0] = v34;
  v33[1] = v35;
  v36 = v14;
  v17 = gt_draft_unresolved_call(0x80035398u, 5, 2 * a3, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F624u, a0, gt_draft_buffer_address(v33, sizeof(v33)) /* TODO Adapter lifecycle */, v17 | 0x2000000, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
  v31 = a5 + 49;
  v32[0] = '.'; v32[1] = 0;
  if ( (sint8)r_u8(0x800BEA24u) )
  {
    v19 = a5;
    if ( a5 >= 4 )
      v19 = 3;
    v18 = r_u8(0x80091B28u + 1u * (4 * v19 + (r_u8(0x80091B40u) & 3)));
  }
  else
  {
    v18 = 6;
  }
  v20 = gt_draft_unresolved_call(0x80035398u, v18, a3, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v20, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003A95Cu, a0, a1, a2 + 4, gt_draft_buffer_address(&v31, sizeof(v31)) /* TODO Adapter lifecycle */, 8, 1, v26, v27);
  v21 = gt_draft_unresolved_call(0x80035458u, a3, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v21, 0, 0, 0, 0, 0, 0, 0);
  if ( !a4 )
  {
    v22 = gt_draft_unresolved_call(0x80035398u, 7, a3, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006C874u, v22, 0, 0, 0, 0, 0, 0, 0);
  }
  gt_draft_8003A8AC(a0, a1 + 24, a2 - 1, v36, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8007C014u, gt_draft_buffer_address(&v31, sizeof(v31)) /* TODO Adapter lifecycle */, (uint32)0x800BCF24, r_u16(2 * a4 - 2146736064), 0, 0, 0, 0, 0);
  v23 = gt_draft_unresolved_call(0x8007C2B0u, gt_draft_buffer_address(&v31, sizeof(v31)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003A95Cu, a0, a1 - (8 * v23 - 266), a2 + 4, gt_draft_buffer_address(&v31, sizeof(v31)) /* TODO Adapter lifecycle */, 8, 2, v26, v27);
  gt_draft_unresolved_call(0x8006C790u, 1, 0, 0, 0, 0, 0, 0, 0);
  v24 = gt_draft_unresolved_call(0x80035398u, 0, a3, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v24, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003A95Cu, a0, a1 + 32, a2 + 16, v37, 8, 1, v26, v27);
  gt_draft_unresolved_call(0x80058BB0u, v15, gt_draft_buffer_address(&v26, sizeof(v26)) /* TODO Adapter lifecycle */, gt_draft_buffer_address(v30, sizeof(v30)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003AA10u, 0, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003A95Cu, a0, a1 + 88, a2 + 16, gt_draft_buffer_address(&v31, sizeof(v31)) /* TODO Adapter lifecycle */, 8, 1, v26, v27);
  gt_draft_unresolved_call(0x8003AA10u, 0, 0, 0, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8003A95Cu, a0, a1 + 152, a2 + 16, gt_draft_buffer_address(&v31, sizeof(v31)) /* TODO Adapter lifecycle */, 8, 1, v26, v27);
}
