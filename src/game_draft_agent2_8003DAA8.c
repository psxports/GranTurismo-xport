#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003DAA8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    sint32 v1 = 0;
    sint16 v2 = 0;
    sint32 v3 = 0;
    uint16 v4 = 0;
    sint16 v5 = 0;
    uint32 v6 = 0;
  gt_draft_unresolved_call(0x8007BD58u, -2145570852, 0, 126976, 0, 0, 0, 0, 0);
  result = (sint8)r_u8(0x801F1FE5u);
  v1 = -2145443860;
  if ( !(sint8)r_u8(0x801F1FE5u) )
  {
    v2 = r_u32(0x801F1FECu);
    w_u32(0x801D2FDCu, 17235);
    w_u8(0x801D2FDEu, 17);
    w_u8(0x801D2FDFu, r_u32(0x801F1FE0u));
    if ( r_u32(0x801F1FECu) )
    {
      v3 = -2145570852;
      do
      {
        v4 = r_u8(v1 + 1);
        v1 += 2;
        v5 = gt_draft_unresolved_call(0x80071640u, ((uint16)(v2 << 8) | v4) & 0x7F7F, 0, 0, 0, 0, 0, 0, 0);
    w_u8(v3 + 4, (((uint32)(v5) >> 24) & 0xFFu));
    w_u8(v3 + 5, v5);
        v2 = r_u8(v1);
        v3 += 2;
      }
      while ( r_u8(v1) );
    }
    if ( r_u32(0x801F1FE4u) == 1 )
    {
      v6 = r_u32(0x8009969Cu);
      return gt_draft_unresolved_call(0x80071698u, -2145570756, v6, 416, 0, 0, 0, 0, 0);
    }
    if ( r_u32(0x801F1FE4u) >= 2 && r_u32(0x801F1FE4u) == 2 )
    {
      v6 = r_u32(0x800997D4u);
      return gt_draft_unresolved_call(0x80071698u, -2145570756, v6, 416, 0, 0, 0, 0, 0);
    }
    gt_draft_unresolved_call(0x80071698u, -2145570756, r_u32(0x8009954Cu), 416, 0, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x8002CAD0u, r_u32(0x801F1FDCu) + 512, 0, 0, 0, 0, 0, 0, 0);
  }
  return result;
}
