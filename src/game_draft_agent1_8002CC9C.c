#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002CC9C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v2 = 0;
    sint32 result = 0;
    uint32 v4 = 0;
  v2 = gt_draft_unresolved_call(0x8002CB28u, -2146649888, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8007BDFCu, a0, -2146649888, v2, 0, 0, 0, 0, 0);
  result = 2;
  if ( r_u8(0x800CBC11u) == 2 )
  {
    v4 = gt_draft_unresolved_call(0x8002CB28u, -2146640844, 0, 0, 0, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x8007BDFCu, a0 + v2, -2146640844, v4, 0, 0, 0, 0, 0);
  }
  return result;
}
