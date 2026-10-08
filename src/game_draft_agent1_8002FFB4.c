#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002FFB4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    uint32 v7 = 0;
    uint32 v8 = 0;
    sint32 v10 = 0;
  int vars0; // [sp+18h] [+0h]
  int vars4; // [sp+1Ch] [+4h]
  int vars8; // [sp+20h] [+8h]
  v4 = 8;
  v5 = a0 == 0 ? 0xC : 0;
  v6 = v5 ^ 0xC;
  v7 = (uint32)(r_u32(0x800B6368u) + 4);
  if ( a0 )
    v4 = 20;
  gt_draft_8002FAA0(r_u32(0x800B6368u) + 4, v4, (sint32)a2, 0, 0, 0, 0, 0);
  gt_draft_8002FCD4((sint32)v7, 16, 16 - v5, (sint32)a2, 0, 0, 0, 0);
  v8 = r_u32(0x800912BCu);
  if ( a0 )
    v8 = r_u32(0x800912B4u);
  gt_draft_unresolved_call(0x8002FBA8u, (sint32)v7, 64, 16 - v5, (sint32)v8, (sint32)a2, 0u, 0u, 0u);
  gt_draft_8002C318(v7, 238, v6 + 30, a2, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8002C994u, (sint32)v7, 0x10u, v6 + 72, (uint32)a2, 0, 0, 0, 0);
}
