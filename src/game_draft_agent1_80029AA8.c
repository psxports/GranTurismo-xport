#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80029AA8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    sint32 v3 = 0;
    w_u16(0x800B63D6u, 0);
  gt_draft_unresolved_call(0x8001E324u, -2146736048, -2146737136, 2, 0, 0, 0, 0, 0);
    w_u8(0x800B63D8u, 0);
  v0 = gt_draft_80029A74(0, 0, 0, 0, 0, 0, 0, 0);
  v1 = 0;
  v2 = (sint32)r_u32(v0);
    w_u8(0x8008F893u, 0);
  v3 = 0;
  if ( v2 == 3 )
  {
    v3 = (sint16)r_u16(0x800B634Cu);
    v1 = 150;
  }
  else if ( v2 >= 4 )
  {
    if ( v2 == 4 )
      v3 = 5;
  }
  else if ( v2 == 1 )
  {
    v3 = r_u32(r_u32(0x800BC1B0u) + 32);
    v1 = r_u32(r_u32(0x800BC1B0u) + 36);
  }
  return gt_draft_unresolved_call(0x80056E48u, v2, -2146736008, v3, v1, 0, 0, 0, 0);
}
