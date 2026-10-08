#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8004ACB8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint32 result = 0;
    uint32 i = 0;
    sint32 v5 = 0;
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008DD98u, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8008B7D8u, "\tcurrent car num <%d>    target car <%d>\n", r_u32(r_u32(0x800BEA64u)) & 0xFFF, r_u16(a1) & 0xFFF, 0, 0, 0, 0, 0);
  v2 = 1;
  if ( (r_u32(r_u32(0x800BEA64u)) & 0xFFF) != (r_u16(a1) & 0xFFF) )
    return -1;
  for ( i = r_u32(0x800935A4u); (sint16)r_u16(i) >= 0; i = (uint32)((uint32)i + 2) )
  {
    result = gt_draft_8004AB98(0, 0, 0, 0, 0, 0, 0, 0);
    if ( result )
      return result;
  }
  if ( r_u16(a1 + 2u * (15)) != r_u16(r_u32(0x800BEA64u) + 30) )
    v2 = 0;
  if ( r_u16(a1 + 2u * (17)) != r_u16(r_u32(0x800BEA64u) + 34) )
    v2 = 0;
  if ( r_u16(a1 + 2u * (2)) != r_u16(r_u32(0x800BEA64u) + 4) )
    v2 = 0;
  if ( r_u16(a1 + 2u * (6)) != r_u16(r_u32(0x800BEA64u) + 12) )
    v2 = 0;
  if ( r_u16(a1 + 2u * (8)) != r_u16(r_u32(0x800BEA64u) + 16) )
    v2 = 0;
  v5 = v2 ^ 1;
  if ( r_u16(a1 + 2u * (12)) != r_u16(r_u32(0x800BEA64u) + 24) )
    v5 = 1;
  return -v5 & 0xFFFFFFFE;
}
