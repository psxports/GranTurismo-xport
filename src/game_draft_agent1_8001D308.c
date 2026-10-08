#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001D308(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
  result = 1;
  if ( !r_u16(a2) )
    return gt_draft_8001CD38((sint32)a2, a1, 0, 0, 0, 0, 0, 0);
  if ( r_u16(a2) == 1 )
    return gt_draft_unresolved_call(0x8001CE98u, (sint32)a2, a1, 0, 0, 0, 0, 0, 0);
  return result;
}
