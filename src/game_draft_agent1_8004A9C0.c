#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8004A9C0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v2 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 i = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
  v4 = 0;
  v5 = 0;
  for ( i = r_u16(v2) & 0xFFF; v4 < a1; a1 += 1 )
  {
    v8 = 1;
    v9 = r_u8(a1);
    gt_draft_unresolved_call(0x8006C12Cu, r_u32(0x800BEA68u), v9, 0, 0, 0, 0, 0, 0);
    v10 = gt_draft_unresolved_call(0x8001EBC4u, 0, 0, 0, 0, 0, 0, 0, 0);
    v11 = i;
    if ( v10 > 0 )
    {
      while ( !gt_draft_unresolved_call(0x8006C058u, v11, v9, v8, -2146603044, 0, 0, 0, 0) )
      {
        ++v8;
        v11 = i;
        if ( v10 < v8 )
          goto LABEL_6;
      }
      v5 |= 1 << a1;
    }
LABEL_6:
    v5 >>= 1;
    ++v4;
  }
  return v5;
}
