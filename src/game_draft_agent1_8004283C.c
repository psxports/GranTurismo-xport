#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8004283C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
  v0 = gt_draft_unresolved_call(0x8001EB94u, r_u32(0x800BEA20u), 0, 0, 0, 0, 0, 0, 0);
  v1 = r_u16(r_u32(0x800BEA64u) + (uint8)r_u8(0x800941BDu + 1u * (r_u32(0x800BEA20u))));
  if ( v1 - 1 < 0 )
    return -1;
    w_u8(r_u32(0x800BEA64u) + 63, r_u8(56 * (v1 - 1) + v0 + 16));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D520u, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, " <CARINF___%d> parts No.<%d>  effects  as follows....\n", r_u32(0x800BEA20u), v1, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "     WasteGate <%d>\n", r_u8(r_u32(0x800BEA64u) + 63), 0, 0, 0, 0, 0, 0);
  return 3;
}
