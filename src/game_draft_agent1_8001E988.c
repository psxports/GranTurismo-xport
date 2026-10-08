#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8001E988(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 result = 0;
    w_u8(0x800CBC15u, a3);
  gt_draft_unresolved_call(0x8006C144u, 0, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8001E8DCu, a0, a3, 0, 0, 0, 0, 0, 0);
  v8 = 1;
  if ( a2 == 1 )
  {
    gt_draft_unresolved_call(0x8001E810u, 1, -2146640424, a0, 0, 0, 0, 0, 0);
    v8 = 2;
  }
  else if ( a2 == 2 )
  {
    if ( a1 == 1 )
    {
      gt_draft_unresolved_call(0x8001E600u, 0, a0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8001E600u, 1, a0, 0, 0, 0, 0, 0, 0);
      v8 = 2;
    }
    else
    {
      v8 = 2;
      if ( a1 == 2 )
      {
        gt_draft_unresolved_call(0x8001E810u, 0, -2146650118, a0, 0, 0, 0, 0, 0);
        gt_draft_unresolved_call(0x8001E810u, 1, -2146650022, a0, 0, 0, 0, 0, 0);
        v8 = 2;
      }
    }
    goto LABEL_14;
  }
  if ( a1 == 1 )
  {
    gt_draft_unresolved_call(0x8001E600u, 0, a0, 0, 0, 0, 0, 0, 0);
  }
  else if ( a1 == 2 )
  {
    gt_draft_8001E7A0(0, (sint16)r_u16(0x800C4C58u), a0, 0, 0, 0, 0, 0);
  }
LABEL_14:
  v9 = 0;
  v10 = 0;
  do
  {
    gt_draft_unresolved_call(0x8006BAD4u, 2 * (((v10 + v9) << 6) + v9) - 2146715568, 32 * (v10 + v9) - 2146715760, a0, 0, 0, 0, 0, 0);
    result = ++v9 < v8;
    v10 = 2 * v9;
  }
  while ( v9 < v8 );
  return result;
}
