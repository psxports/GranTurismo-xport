#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80042910(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    uint32 v3 = 0;
  v0 = gt_draft_unresolved_call(0x8001EB94u, r_u32(0x800BEA20u), 0, 0, 0, 0, 0, 0, 0);
  v1 = r_u16(r_u32(0x800BEA64u) + (uint8)r_u8(0x800941BDu + 1u * (r_u32(0x800BEA20u))));
  if ( v1 - 1 < 0 )
    return -1;
  v2 = 68 * (v1 - 1) + v0;
    w_u8(r_u32(0x800BEA64u) + 62, r_u8(v2));
  v3 = (uint32)r_u32(0x800BEA64u);
    w_u16(r_u32(0x800BEA64u) + 46, r_u16(v2 + 2));
    w_u16(v3 + 2u * (24), r_u16(v2 + 4));
    w_u16(v3 + 2u * (25), r_u16(v2 + 6));
    w_u16(v3 + 2u * (26), r_u16(v2 + 8));
    w_u16(v3 + 2u * (27), r_u16(v2 + 10));
    w_u16(v3 + 2u * (28), r_u16(v2 + 12));
    w_u16(v3 + 2u * (29), r_u16(v2 + 14));
    w_u16(v3 + 2u * (30), r_u16(v2 + 16));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D520u, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, " <CARINF___%d> parts No.<%d>  effects  as follows....\n", r_u32(0x800BEA20u), v1, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "          gear <%d>\n", r_u8(r_u32(0x800BEA64u) + 62), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           1st <%d>\n", r_u16(r_u32(0x800BEA64u) + 46), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           2nd <%d>\n", r_u16(r_u32(0x800BEA64u) + 48), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           3rd <%d>\n", r_u16(r_u32(0x800BEA64u) + 50), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           4th <%d>\n", r_u16(r_u32(0x800BEA64u) + 52), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           5th <%d>\n", r_u16(r_u32(0x800BEA64u) + 54), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           6th <%d>\n", r_u16(r_u32(0x800BEA64u) + 56), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "           7th <%d>\n", r_u16(r_u32(0x800BEA64u) + 58), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "         final <%d>\n", r_u16(r_u32(0x800BEA64u) + 60), 0, 0, 0, 0, 0, 0);
  return 4;
}
