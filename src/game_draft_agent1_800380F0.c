#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800380F0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v8 = 0;
    sint32 v10[2] = {0};
    sint32 v11 = 0;
    sint32 v12 = 0;
    v11 = (v11 & 0xFFFF0000u) | (((uint32)(a1) & 0xFFFFu) << 0);
    v11 = (v11 & 0x0000FFFFu) | (((uint32)(a2 + 41) & 0xFFFFu) << 16);
    v12 = (v12 & 0xFFFF0000u) | (((uint32)(a1 + 319) & 0xFFFFu) << 0);
    v12 = (v12 & 0x0000FFFFu) | (((uint32)(a2 + 41) & 0xFFFFu) << 16);
  v10[0] = v11;
  v10[1] = v12;
  gt_draft_unresolved_call(0x8006F4F0u, a0, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, (uint8)a3, 0, 0, 0, 0, 0);
  v8 = gt_draft_unresolved_call(0x80035458u, (uint32)((sint32)a3 >> 1), 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, v8, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8006CA10u, a0, a1 + 160, a2 + 32, -2146707153, 0, 0, 0, 0);
}
