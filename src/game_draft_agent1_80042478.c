#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80042478(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    uint32 v2 = 0;
    sint32 v3 = 0;
  v0 = gt_draft_unresolved_call(0x8001EB94u, r_u32(0x800BEA20u), 0, 0, 0, 0, 0, 0, 0);
  v1 = r_u16(r_u32(0x800BEA64u) + (uint8)r_u8(0x800941BDu + 1u * (r_u32(0x800BEA20u))));
  if ( v1 - 1 < 0 )
    return -1;
  v2 = (uint32)(20 * (v1 - 1) + v0);
  v3 = gt_draft_unresolved_call(0x800417ACu, r_u8(v2 + 1u * (2)), r_u8(v2 + 1u * (3)), r_u8(v2), r_u8(v2 + 1u * (1)), 0, 0, 0, 0);
    w_u8(r_u32(0x800BEA64u) + 75, v3);
    w_u8(r_u32(0x800BEA64u) + 66, r_u8(v2));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D520u, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, " <CARINF___%d> parts No.<%d>  effects  as follows....\n", r_u32(0x800BEA20u), v1, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "     Stabilizer F <%d> level<%d/%d>\n", r_u8(r_u32(0x800BEA64u) + 66), v3 + 1, r_u8(v2 + 1u * (1)), 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "                   min<%d> max<%d>\n", r_u8(v2 + 1u * (2)), r_u8(v2 + 1u * (3)), 0, 0, 0, 0, 0);
  return 1;
}
