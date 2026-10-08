#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002A474(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    sint32 v3 = 0;
    uint32 v4 = 0;
  v0 = gt_draft_8002A3AC(0, 0, 0, 0, 0, 0, 0, 0);
  v1 = gt_draft_8002A390(0, 0, 0, 0, 0, 0, 0, 0);
  v2 = 0;
  if ( v1 - 1 <= 0 )
    return 1;
  v3 = v1 - 1;
  v4 = (uint32)v0;
  while ( 1 )
  {
    ++v2;
    if ( !r_u8(v4) )
      break;
    v4 = (uint32)(v0 + v2);
    if ( v2 >= v3 )
      return 1;
  }
  return 0;
}
