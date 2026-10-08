#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8004ABA0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint32 result = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
  if ( !r_u8(v2 + 95) || (result = 0, a1 == 34) )
  {
    v5 = r_u16(a0 + (uint8)r_u8(0x800941BDu + 1u * (a1)));
    result = 0;
    if ( r_u16(a0 + (uint8)r_u8(0x800941BDu + 1u * (a1))) )
    {
      v6 = r_u8(gt_draft_unresolved_call(0x8006C12Cu, r_u32(0x800BEA68u), a1, 0, 0, 0, 0, 0, 0) + ((v5 - 1) >> 3)) & (1 << ((v5 - 1) & 7));
      gt_draft_unresolved_call(0x8008B7D8u, "\t    check a parts : type %d --- <%d> :", a1, v5, 0, 0, 0, 0, 0);
      if ( v6 )
      {
        if ( gt_draft_unresolved_call(0x8006C058u, r_u32(r_u32(0x800BEA64u)) & 0xFFF, a1, v5, -2146603044, 0, 0, 0, 0) )
        {
          gt_draft_unresolved_call(0x8008B7D8u, 0x8008DD94u, 0, 0, 0, 0, 0, 0, 0);
          return 0;
        }
        else
        {
          gt_draft_unresolved_call(0x8008B7D8u, 0 /* TODO Address for 0x8008DD7Cu */, 0, 0, 0, 0, 0, 0, 0);
          return -3;
        }
      }
      else
      {
        gt_draft_unresolved_call(0x8008B7D8u, 0 /* TODO Address for 0x8008DD70u */, 0, 0, 0, 0, 0, 0, 0);
        return -2;
      }
    }
  }
  return result;
}
