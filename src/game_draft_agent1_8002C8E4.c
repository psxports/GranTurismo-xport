#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002C8E4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v1 = 0;
    sint32 result = 0;
  v1 = 4 * a0;
  result = 0;
  if ( v1 >= -255 )
  {
    if ( v1 >= 0 )
    {
      if ( v1 < 256 )
        return (uint8)v1 | 0xFF00;
      else
        return ((v1 & 0xFC ^ 0xFF) << 8) | 0xFF;
    }
    else
    {
      return ((uint8)(v1 - 1) << 8) | ((uint8)-(sint8)v1 << 16);
    }
  }
  return result;
}
