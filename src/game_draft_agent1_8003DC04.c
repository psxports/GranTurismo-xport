#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003DC04(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint32 result = 0;
  v2 = gt_draft_80046AF4(a1, a0, 0, 0, 0, 0, 0, 0);
  if ( v2 == -2 )
  {
    result = -1;
    if ( !(sint8)r_u8(0x801F2041u) )
      return result;
    return -2;
  }
  else
  {
    result = -1;
    if ( v2 == -1 )
      return result;
    return v2;
  }
}
