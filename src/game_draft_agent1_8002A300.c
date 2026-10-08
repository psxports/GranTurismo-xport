#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002A300(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{

  if ( (sint16)r_u16(0x800B63D6u) == 2 )
    return gt_draft_8002A0D4(0, 0, 0, 0, 0, 0, 0, 0);
  if ( (sint16)r_u16(0x800B63D6u) >= 3 )
  {
    if ( (sint16)r_u16(0x800B63D6u) == 4 )
      return gt_draft_8002A15C(0, 0, 0, 0, 0, 0, 0, 0);
  }
  else if ( (sint16)r_u16(0x800B63D6u) == 1 )
  {
    return gt_draft_8002A084(0, 0, 0, 0, 0, 0, 0, 0);
  }
  return gt_draft_8002A270(0, 0, 0, 0, 0, 0, 0, 0);
}
