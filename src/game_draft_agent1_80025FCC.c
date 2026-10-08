#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80025FCC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 result = 0;
  v0 = r_u32(0x800B6368u);
  if ( (sint16)r_u16(0x800B63D6u) == 7 )
  {
    v1 = r_u32(0x800B6368u);
LABEL_8:
    gt_draft_8003011C(v1, 68, 8421504, 1, 0, 0, 0, 0);
    result = 0;
    goto LABEL_9;
  }
  if ( (sint16)r_u16(0x800B63D6u) < 8 )
  {
    v1 = r_u32(0x800B6368u);
    if ( (sint16)r_u16(0x800B63D6u) != 6 )
    {
      result = 0;
      goto LABEL_9;
    }
    goto LABEL_8;
  }
  result = 0;
  if ( (sint16)r_u16(0x800B63D6u) < 10 )
  {
    gt_draft_unresolved_call(0x800214C8u, r_u32(0x800B6368u), 68, r_u16(0x800B63DAu) << 16 >> 19, 0, 0, 0, 0, 0);
    gt_draft_800308D0(v0, 15, 112, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80030AE4u, v0, 0, 0, 0, 0, 0, 0, 0);
    result = 1;
  }
LABEL_9:
  if ( !result )
  {
    gt_draft_unresolved_call(0x800306E8u, v0, 68, r_u8(0x800CB7F6u), 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80030198u, v0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8002E32Cu, 0, 0, 0, 0, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x80030424u, v0, 0, 0, 0, 0, 0, 0, 0);
  }
  return result;
}
