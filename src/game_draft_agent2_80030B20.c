#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80030B20(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint16 v1 = 0;
    sint32 v2 = 0;
    sint32 result = 0;
  v0 = r_u16(0x800912E0u);
  v1 = r_u16(0x800BC1C8u);
  w_u16(0x800912E2u, r_u16(0x800912E2u) + 1u);
  v2 = r_u16(0x800BC1C8u) & 0x20;
  if ( (r_u16(0x800BC1C8u) & 0x80) != 0 )
  {
    gt_draft_800313A4(0, 0, 0, 0, 0, 0, 0, 0);
    w_u16(0x800912E2u, 0);
    --v0;
    v2 = v1 & 0x20;
    if ( v0 == -1 )
      v0 = 1;
  }
  if ( v2 )
  {
    gt_draft_800313A4(0, 0, 0, 0, 0, 0, 0, 0);
    w_u16(0x800912E2u, 0);
    if ( ++v0 >= 2 )
    v0 = (v0 & 0xFFFF0000u) | (((uint32)(0) & 0xFFFFu) << 0);
  }
    w_u16(0x800912E0u, v0);
  if ( (v1 & 0x6000) != 0 )
    return (sint16)v0;
  result = -1;
  if ( (v1 & 0x9000) == 0 )
    return -2;
  return result;
}
