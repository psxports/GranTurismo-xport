#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_800245A0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 result = 0;
  v2 = a0 + 68;
  v3 = r_u32(a0 + 44);
  v4 = 0;
    w_u32(a0, v2);
  v5 = 8 * v3 + v2;
  do
  {
    w_u32(4 * v4 + a0 + 4, v5);
    v6 = r_u16(2 * v4 + a0 + 48) * (uint8)r_u8(0x8008F720u + 1u * (v4));
    ++v4;
    v5 += v6;
  }
  while ( v4 < 8 );
  v7 = r_u16(a0 + 64);
  v8 = r_u16(a0 + 66);
    w_u32(a0 + 36, v5);
  v9 = 16 * v7 + v5;
  result = 20 * v8 + v9;
    w_u32(a0 + 40, v9);
  return result;
}
