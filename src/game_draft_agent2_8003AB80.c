#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003AB80(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v7 = 0;
    uint32 v8 = 0;
    uint32 v9 = 0;
    sint32 result = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
  v7 = (uint32)a3;
  v8 = r_u32(0x8009218Cu);
  if ( gt_draft_8003AB60(a3, 0, 0, 0, 0, 0, 0, 0) )
    v8 = r_u32(0x80092198u);
  v9 = (uint32)v8 + 36;
  do
  {
    w_u8(0x8008CDB0u, r_u8(v7));
    result = r_u8(0x8008CDB0u);
    v7 += 1;
    if ( !r_u8(0x8008CDB0u) )
      break;
    v11 = r_u8(0x8008CDB0u) == 49 ? a1 + 2 : a1;
    gt_draft_unresolved_call(0x8006CA10u, a0, v11, a2, 0x8008CDB0u, 0, 0, 0, 0);
    v12 = (sint8)r_u8(v8);
    v8 = (uint32)((uint32)v8 + 4);
    a1 += v12;
    result = (sint32)v8 < (sint32)v9;
  }
  while ( (sint32)v8 < (sint32)v9 );
  return result;
}
