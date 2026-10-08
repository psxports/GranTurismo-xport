#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80023FA4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v3 = 0;
    uint32 v4 = 0;
    uint64 v5 = 0;
    uint32 v6 = 0;
    uint64 v7 = 0;
    sint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    uint32 v16 = 0;
    sint32 v17 = 0;
    sint32 v18 = 0;
    sint32 result = 0;
    sint32 v21 = 0;
    sint32 v22 = 0;
    sint32 v23 = 0;
  v3 = r_u32(4 * gt_draft_80023DCC(a0, a2, 0, 0, 0, 0, 0, 0) + r_u32(0x800BC1A8u) + 12);
  v4 = r_u32(v3 + 8);
  v21 = r_u32(a2) - r_u32(v3 + 24);
  v22 = r_u32(a2 + 4u * (1)) - r_u32(v3 + 28);
  v23 = r_u32(a2 + 4u * (2)) - r_u32(v3 + 32);
  v5 = gt_draft_unresolved_call(0x8006E9CCu, gt_draft_buffer_address(&v21, sizeof(v21)) /* TODO Adapter lifecycle */, v3 + 36, 0, 0, 0, 0, 0, 0);
  v6 = (sint16)r_u16(v3 + 22);
  v7 = (uint32)v5 * (unsigned __int64)v6;
  v8 = v5 * (r_u16(v3 + 22) << 16 >> 31);
  v21 = r_u32(v4 + 4u * (6)) - r_u32(a2);
  v22 = r_u32(v4 + 4u * (7)) - r_u32(a2 + 4u * (1));
  v23 = r_u32(v4 + 4u * (8)) - r_u32(a2 + 4u * (2));
  v7 = (v7 & 0xffffffffull) | ((uint64)(uint32)(((uint32)(v7 >> 32)) + v8 + v6 * ((uint32)(v5 >> 32))) << 32);
  v9 = v7 >> 12;
  v10 = ((__int64)(gt_draft_unresolved_call(0x8006E9CCu, gt_draft_buffer_address(&v21, sizeof(v21)) /* TODO Adapter lifecycle */, v4 + 9, 0, 0, 0, 0, 0, 0) * (unsigned __int64)(uint32)(sint16)r_u16(v3 + 20)) >> 12) + v9;
  if ( v10 )
    v11 = gt_draft_unresolved_call(0x8001E4A8u, v9 << 12, (unsigned __int64)v9 >> 20, v10, 0, 0, 0, 0, 0);
  else
    v11 = 0;
  v12 = r_u32(v3 + 16);
  v13 = r_u32(r_u32(0x800BC1A8u));
  v14 = r_u32(v4 + 4u * (4)) - v12;
  if ( r_u32(r_u32(0x800BC1A8u)) >= v14 )
  {
    v17 = (unsigned __int64)(v14 * (__int64)v11) >> 32;
    v16 = v14 * v11;
    if ( v14 >= 0 )
      goto LABEL_9;
    v15 = v14 + v13;
  }
  else
  {
    v15 = v14 - v13;
  }
  v17 = (unsigned __int64)(v15 * (__int64)v11) >> 32;
  v16 = v15 * v11;
LABEL_9:
  v18 = ((v16 >> 12) | (v17 << 20)) + v12;
  if ( v13 < v18 )
    return v18 - v13;
  result = ((v16 >> 12) | (v17 << 20)) + v12;
  if ( v18 >= 0 )
    return result;
  return v18 + v13;
}
