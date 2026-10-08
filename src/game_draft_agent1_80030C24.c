#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80030C24(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
  v0 = gt_draft_80030B20(0, 0, 0, 0, 0, 0, 0, 0);
  if ( v0 >= 2 || v0 < 0 )
  {
    if ( !(sint16)r_u16(0x800B63DEu) )
    w_u8(0x800B63C2u, 0);
  }
  else
  {
    w_u8(0x800B63C2u, 0);
    gt_draft_800313A4(4, 0, 0, 0, 0, 0, 0, 0);
  }
  return gt_draft_unresolved_call(0x800204ECu, r_u16(0x800912E0u) != 0, 0, 0, 0, 0, 0, 0, 0);
}
