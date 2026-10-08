#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002332C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 result = 0;
    sint32 v2 = 0;
    sint32 v3 = 0;
    sint32 v4 = 0;
  v0 = r_u32(0x800B6368u);
  switch ( r_u16(0x800B63D6u) )
  {
    case 6:
    case 9:
      gt_draft_8003011C(r_u32(0x800B6368u), 68, 8421504, 1, 0, 0, 0, 0);
      result = 0;
      break;
    case 7:
    case 8:
      v2 = r_u32(0x800B6368u);
      v3 = 0;
      goto LABEL_8;
    case 0xA:
      v2 = r_u32(0x800B6368u);
      v3 = 0;
      v4 = 0;
      goto LABEL_9;
    case 0xB:
      v2 = r_u32(0x800B6368u);
      v3 = 1;
      v4 = 0;
      goto LABEL_9;
    case 0xC:
      v2 = r_u32(0x800B6368u);
      v3 = 2;
      v4 = 0;
      goto LABEL_9;
    case 0xD:
      v2 = r_u32(0x800B6368u);
      v3 = 2;
LABEL_8:
      v4 = 1;
LABEL_9:
      gt_draft_unresolved_call(0x80023280u, v2, v3, v4, 0, 0, 0, 0, 0);
      result = 1;
      break;
    default:
      result = 0;
      break;
  }
  if ( !result )
  {
    gt_draft_unresolved_call(0x800306E8u, v0, 68, r_u8(0x800CB7F6u), r_u8(0x800CB7F6u) == 2, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80030198u, v0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8002E32Cu, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80030424u, v0, 0, 0, 0, 0, 0, 0, 0);
    result = r_u8(0x800CB7F8u) < 0x13u;
    if ( r_u8(0x800CB7F8u) < 0x16u && r_u8(0x800CB7F8u) >= 0x13u )
      return gt_draft_unresolved_call(0x8002C994u, v0, 216, 192, -2146736048, 0, 0, 0, 0);
  }
  return result;
}
