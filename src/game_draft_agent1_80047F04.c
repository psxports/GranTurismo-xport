#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80047F04(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 result = 0;
    sint32 v7 = 0;
    sint32 v8 = 0;
  v3 = -1;
  v4 = r_u8(0x800B538Eu);
  if ( r_u8(0x800B538Eu) > 0 )
  {
    w_u8(0x800B538Eu, r_u8(0x800B538Eu) - 1u);
    v5 = -3;
    if ( !((v4 - 1) << 24) )
    {
      gt_draft_unresolved_call(0x8008B7D8u, "delete<%d>\n", r_u8(0x80092E9Du), 0, 0, 0, 0, 0, 0);
      gt_draft_80046F6C(a0, r_u8(0x80092E9Du), 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x80047CA0u, a0, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x800B538Du, 1);
      return -3;
    }
    return v5;
  }
    w_u8(0x800B538Du, 0);
    w_u8(0x800B538Eu, 0);
  if ( (r_u16(a2 + 2u * (1)) & 0xC00) == 3072 )
    w_u8(0x800B538Du, 1);
  v7 = r_u8(0x80092E9Du);
  if ( ((r_u16(a2 + 2u * (2)) | r_u16(a2 + 2u * (4))) & 0x100) != 0 )
  {
    gt_draft_unresolved_call(0x80035778u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8003B8ECu, 0x80092E9Cu, v7 - 10, 0, 0, 0, 0, 0, 0);
  }
  else if ( ((r_u16(a2 + 2u * (2)) | r_u16(a2 + 2u * (4))) & 0x200) != 0 )
  {
    gt_draft_unresolved_call(0x80035778u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8003B8ECu, 0x80092E9Cu, v7 + 10, 0, 0, 0, 0, 0, 0);
  }
  else
  {
    v3 = gt_draft_unresolved_call(0x8003B954u, 0x80092E9Cu, (sint32)a2, 0, 0, 0, 0, 0, 0);
  }
  if ( !(sint8)r_u8(0x800B538Du) )
  {
    if ( v3 != -2 )
    {
      result = -1;
      if ( v3 == -1 )
        return result;
      if ( r_u8(r_u32(0x800B5388u) + v3) == 2 )
        v5 = 120;
      else
        v5 = r_u32(8 * v3 + r_u32(0x800B5384u) + 4);
      gt_draft_unresolved_call(0x8008B7D8u, "select %d\n", v5, 0, 0, 0, 0, 0, 0);
      return v5;
    }
    return -2;
  }
  if ( v3 == -2 )
    return -2;
  if ( v3 == -1 )
    return -3;
  v8 = (sint8)r_u8(r_u32(0x800B5388u) + v3);
  if ( v8 == 2 )
    return 120;
  if ( v8 == 3 )
  {
    gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
  }
  else
  {
    w_u8(0x800B538Eu, 16);
    gt_draft_unresolved_call(0x80035778u, 1, 0, 0, 0, 0, 0, 0, 0);
  }
  return -3;
}
