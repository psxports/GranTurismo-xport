#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80045338(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v0 = 0;
    sint32 result = 0;
    sint32 v2 = 0;
    sint32 v3 = 0;
    sint16 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
    sint16 v8 = 0;
    uint32 v9 = 0;
    uint32 v10 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
  int (*v11)[18]; // $s2
  int (*v14)[18]; // $a0
  v0 = r_u32(0x800BDD08u);
  gt_draft_unresolved_call(0x8004AF94u, 0, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8003A304u, 0, 0, 0, 0, 0, 0, 0, 0);
  result = 0;
  if ( r_u8(v0) )
  {
    if ( !r_u8(0x800B5304u) )
    {
      gt_draft_unresolved_call(0x80035388u, 0, 0, 0, 0, 0, 0, 0, 0);
      v2 = gt_draft_unresolved_call(0x8004B25Cu, r_u32(0x8009303Cu), v0 + 2, 0, 0, 0, 0, 0, 0);
      if ( v2 == -2 )
      {
        gt_draft_unresolved_call(0x80035778u, 2, 0, 0, 0, 0, 0, 0, 0);
        return 1;
      }
      if ( v2 != -1 )
      {
LABEL_9:
        gt_draft_unresolved_call(0x80035778u, 5, 0, 0, 0, 0, 0, 0, 0);
        v5 = gt_draft_80045320(r_u8(0x80093049u), 0, 0, 0, 0, 0, 0, 0);
        gt_draft_unresolved_call(0x8004B6A0u, v5, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x800B5304u, 1);
        return 0;
      }
      v4 = r_u16(v0 + 6);
      if ( (v4 & 0x400) == 0 )
      {
        result = 0;
        if ( (v4 & 0x20) == 0 )
          return result;
        goto LABEL_9;
      }
LABEL_13:
      gt_draft_unresolved_call(0x80035778u, 1, 0, 0, 0, 0, 0, 0, 0);
      v3 = 5;
      gt_draft_unresolved_call(0x80035364u, (sint32)0x80092CB4u, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x800B5304u, 0);
      return v3;
    }
    gt_draft_unresolved_call(0x80035388u, 1, 0, 0, 0, 0, 0, 0, 0);
    v6 = gt_draft_80045320(r_u8(0x80093049u), 0, 0, 0, 0, 0, 0, 0);
    v7 = gt_draft_unresolved_call(0x8004B6A8u, v6, v0 + 2, 0, 0, 0, 0, 0, 0);
    if ( v7 == -2 )
    {
LABEL_15:
      gt_draft_unresolved_call(0x80035778u, 3, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x800B5304u, 0);
      return 0;
    }
    if ( v7 == -1 )
    {
      v8 = r_u16(v0 + 6);
      if ( (v8 & 0x400) != 0 )
        goto LABEL_13;
      result = 0;
      if ( (v8 & 0x80) == 0 )
        return result;
      goto LABEL_15;
    }
    if ( r_u8(0x80093049u) == 1 )
    {
    w_u32(0x80092D28u, r_u32(0x80093558u + 4u * (v7)));
      v11 = r_u32(0x800934D0u + 4u * v7);
    w_u32(0x800B530Cu, r_u32(0x8009350Cu + 4u * v7));
      v12 = (uint8)r_u8(0x800934ACu + 1u * (v7));
    w_u8(0x800B5305u, r_u8(0x80093584u + 1u * (v7)));
      v13 = (uint8)r_u8(0x80093598u + 1u * (v7));
      v14 = v11;
    }
    else if ( r_u8(0x80093049u) >= 2 )
    {
      v10 = 4 * v7;
      if ( r_u8(0x80093049u) != 2 )
        goto LABEL_25;
    w_u32(0x80092D28u, r_u32(0x80093564u + 4u * (v10 / 4)));
      v11 = r_u32(0x800934DCu + v10);
    w_u32(0x800B530Cu, r_u32(0x80093518u + v10));
      v12 = (uint8)r_u8(0x800934B0u + 1u * (v7));
    w_u8(0x800B5305u, r_u8(0x80093588u + 1u * (v7)));
      v13 = (uint8)r_u8(0x8009359Cu + 1u * (v7));
      v14 = v11;
    }
    else
    {
      v9 = 4 * v7;
      if ( r_u8(0x80093049u) )
      {
LABEL_25:
    w_u32(0x80092D28u, r_u32(0x80093574u + 4u * (v7)));
        v11 = r_u32(0x800934ECu + 4u * v7);
    w_u32(0x800B530Cu, r_u32(0x80093528u + 4u * v7));
        v12 = (uint8)r_u8(0x800934B4u + 1u * (v7));
    w_u8(0x800B5305u, r_u8(0x8009358Cu + 1u * (v7)));
        v13 = (uint8)r_u8(0x800935A0u + 1u * (v7));
        v14 = v11;
        goto LABEL_26;
      }
    w_u32(0x80092D28u, r_u32(0x80093540u + 4u * (v9 / 4)));
      v11 = r_u32(0x800934B8u + v9);
    w_u32(0x800B530Cu, r_u32(0x800934F4u + v9));
      v12 = (uint8)r_u8(0x800934A4u + 1u * (v7));
    w_u8(0x800B5305u, r_u8(0x8009357Cu + 1u * (v7)));
      v13 = (uint8)r_u8(0x80093590u + 1u * (v7));
      v14 = v11;
    }
LABEL_26:
    gt_draft_unresolved_call(0x8004AFCCu, v14, -2146741484, v12, v13, 0, 0, 0, 0);
    w_u8(0x800B5374u, v13);
    w_u32(0x80093534u, v11);
    w_u8(0x80093530u, v12);
    w_u8(0x80093532u, v12);
    gt_draft_unresolved_call(0x80035778u, 4, 0, 0, 0, 0, 0, 0, 0);
    v3 = 3;
    gt_draft_unresolved_call(0x800352DCu, 0x80092D24u, 0, 0, 0, 0, 0, 0, 0);
    return v3;
  }
  return result;
}
