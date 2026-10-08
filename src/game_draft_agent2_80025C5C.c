#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80025C5C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    uint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 result = 0;
  v0 = 0;
  v1 = 0;
  v2 = 40 * r_u8(0x800B63BDu) - 2146652612;
  if ( !(sint8)r_u8(0x800B63D2u) )
  {
    gt_draft_unresolved_call(0x80035138u, 0, 0, 0, 0, 0, 0, 0, 0);
    v1 = gt_draft_unresolved_call(0x800351D4u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80035228u, 0, 0, 0, 0, 0, 0, 0, 0);
    v3 = (uint32)(4 * r_u8(0x800B63BDu) - 2146652612 + 2300 + r_u8(0x800B631Cu));
    w_u8(v3, r_u8(v3) | (1 << r_u32(0x800CB7F9u)));
    gt_draft_unresolved_call(0x80035138u, r_u8(0x800B63BDu), 0, 0, 0, 0, 0, 0, 0);
    v0 = gt_draft_unresolved_call(0x800351D4u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80035228u, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  v4 = 0;
  if ( (sint8)r_u8(0x800BDD01u) )
  {
    w_u32(v2 + 24, r_u32(0x800BDB4Cu));
    v4 = 2;
    w_u16(v2 + 28, r_u16(0x800B6670u));
  }
  if ( (sint8)r_u8(0x800BDD00u) )
  {
    w_u32(v2 + 4, r_u32(0x800BDB48u));
    v4 |= 1u;
    w_u16(v2 + 8, r_u16(0x800B6670u));
  }
  gt_draft_unresolved_call(0x80033CECu, v4, (sint8)r_u8(0x800B63D2u), v2, v2 + 20, 0, 0, 0, 0);
  v5 = 0;
  if ( v0 )
    v5 = v1 != v0;
  result = v5;
  if ( v5 )
  {
    w_u32(0x800B6390u, 0x8008BBF4u);
    return v5;
  }
  return result;
}
