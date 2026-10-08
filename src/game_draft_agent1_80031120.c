#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80031120(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    uint32 i = 0;
    uint32 v4 = 0;
    sint32 v5 = 0;
    uint32 v6 = 0;
  result = r_u32(a1);
  for ( i = 0; i < r_u32(a1); result = i < r_u32(a1) )
  {
    v4 = (uint32)(24 * i + r_u32(0x800B49D4u));
    gt_draft_unresolved_call(0x8007BE38u, v4, (a1 + 4u * (5 * i + 1)), 0, 0, 0, 0, 0, 0);
    v5 = (sint32)a1 + r_u32(a1 + 4u * (5 * i + 5));
    v6 = gt_draft_unresolved_call(0x80030F00u, (sint32)v4, (uint32)(v5 + 8), v5, 0, 0, 0, 0, 0);
    gt_draft_80030FFC(v4, (sint32)v6, v5, 0, 0, 0, 0, 0);
    ++i;
  }
    w_u32(0x800B49D8u, i);
  return result;
}
