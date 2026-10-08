#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80043410(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    uint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 result = 0;
  v0 = gt_draft_unresolved_call(0x8001EB94u, 33, 0, 0, 0, 0, 0, 0, 0);
  v1 = r_u16(0x800929ACu) - 1;
  v2 = r_u16(r_u32(0x800BEA64u) + (uint8)r_u8(0x800941DEu));
    w_u8(r_u32(0x800BEA64u) + 76, v1);
  v3 = (uint32)(72 * (v2 - 1) + v0);
  v4 = r_u8(v3 + 1u * (38));
    w_u8(r_u32(0x800BEA64u) + 70, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (30)), r_u8(v3 + 1u * (31)), v1, v4, 0, 0, 0, 0));
    w_u8(r_u32(0x800BEA64u) + 72, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (34)), r_u8(v3 + 1u * (35)), v1, v4, 0, 0, 0, 0));
    w_u8(r_u32(0x800BEA64u) + 71, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (32)), r_u8(v3 + 1u * (33)), v1, v4, 0, 0, 0, 0));
    w_u8(r_u32(0x800BEA64u) + 73, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (36)), r_u8(v3 + 1u * (37)), v1, v4, 0, 0, 0, 0));
  v5 = r_u16(0x800929BCu) - 1;
    w_u8(r_u32(0x800BEA64u) + 89, v5);
  v6 = r_u8(v3 + 1u * (53));
    w_u8(r_u32(0x800BEA64u) + 83, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (45)), r_u8(v3 + 1u * (46)), v5, v6, 0, 0, 0, 0));
    w_u8(r_u32(0x800BEA64u) + 85, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (49)), r_u8(v3 + 1u * (50)), v5, v6, 0, 0, 0, 0));
    w_u8(r_u32(0x800BEA64u) + 84, gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (47)), r_u8(v3 + 1u * (48)), v5, v6, 0, 0, 0, 0));
  result = gt_draft_unresolved_call(0x800419FCu, r_u8(v3 + 1u * (51)), r_u8(v3 + 1u * (52)), v5, v6, 0, 0, 0, 0);
    w_u8(r_u32(0x800BEA64u) + 86, result);
  return result;
}
