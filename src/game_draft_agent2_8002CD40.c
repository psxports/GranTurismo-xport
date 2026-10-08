#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002CD40(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    uint32 v1 = 0;
  result = 1;
  if ( r_u32(0x800B630Cu) == 1 )
  {
    w_u8(0x8008FBEFu, 0);
    w_u8(0x8008FBF0u, 0);
    return result;
  }
  if ( gt_draft_8002CD1C(0, 0, 0, 0, 0, 0, 0, 0) )
  {
    w_u8(0x8008FBEFu, 0);
  }
  else if ( (w_u8(0x8008FBEFu, r_u8(0x8008FBEFu) + 1u), r_u8(0x8008FBEFu)) >= 3 )
  {
    w_u8(0x8008FBEFu, 3);
    result = 0;
LABEL_11:
    w_u8(0x800B63D8u, 1);
    return result;
  }
  result = 1;
  if ( r_u8(0x800CB7F5u) != 2 )
    return result;
  v1 = gt_draft_8002CD1C(1, 0, 0, 0, 0, 0, 0, 0);
  result = 1;
  if ( !v1 )
  {
    w_u8(0x8008FBF0u, r_u8(0x8008FBF0u) + 1u);
    result = 1;
    if ( r_u8(0x8008FBF0u) < 3 )
      return result;
    result = 0;
    w_u8(0x8008FBF0u, 3);
    goto LABEL_11;
  }
    w_u8(0x8008FBF0u, 0);
  return result;
}
