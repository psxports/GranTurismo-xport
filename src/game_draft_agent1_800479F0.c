#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800479F0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v1 = 0;
    sint32 v2 = 0;
    sint32 v3 = 0;
    sint32 result = 0;
  v1 = -1;
  v2 = r_u8(0x80092E9Du);
  v3 = -1;
  if ( ((r_u16(a0 + 4) | r_u16(a0 + 8)) & 0x100) != 0 )
  {
    gt_draft_unresolved_call(0x80035778u, 0, 0, 0, 0, 0, 0, 0, 0);
    gtmain_menu_list_select(0x80092E9Cu, v2 - 10);
  }
  else if ( ((r_u16(a0 + 4) | r_u16(a0 + 8)) & 0x200) != 0 )
  {
    gt_draft_unresolved_call(0x80035778u, 0, 0, 0, 0, 0, 0, 0, 0);
    gtmain_menu_list_select(0x80092E9Cu, v2 + 10);
  }
  else
  {
    v1 = gt_draft_unresolved_call(0x8003B954u, 0x80092E9Cu, a0, 0, 0, 0, 0, 0, 0);
  }
  if ( v1 == -2 )
    return -2;
  result = -1;
  if ( v1 == -1 )
    return result;
  if ( r_u8(r_u32(0x800B5388u) + v1) )
  {
    if ( r_u8(r_u32(0x800B5388u) + v1) == 2 )
      v3 = 120;
    else
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
  }
  else
  {
    v3 = r_u32(8 * v1 + r_u32(0x800B5384u) + 4);
  }
  gt_draft_unresolved_call(0x8008B7D8u, "select %d\n", v3, 0, 0, 0, 0, 0, 0);
  return v3;
}
