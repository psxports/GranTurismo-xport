#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80046AF4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v4 = 0;
    sint16 v5 = 0;
    sint32 result = 0;
  v4 = -1;
  if ( (sint8)r_u8(a0 + 13) > 0 )
    return v4;
  v5 = r_u16(a1 + 4) | r_u16(a1 + 8);
  if ( (v5 & 0x80) != 0 )
  {
    gt_draft_80031370(0, 0, 0, 0, 0, 0, 0, 0);
    w_u8(a0 + 12, 0);
  }
  if ( (v5 & 0x20) != 0 )
  {
    gt_draft_80031370(0, 0, 0, 0, 0, 0, 0, 0);
    w_u8(a0 + 12, 1);
  }
  if ( (r_u16(a1 + 4) & 0x6000) != 0 )
    v4 = (sint8)r_u8(a0 + 12);
  result = v4;
  if ( (r_u16(a1 + 4) & 0x9000) != 0 )
    return -2;
  return result;
}
