#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80029928(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint16 v0;
  sint32 v1;
  sint8 v2;
  sint32 v3;
  sint32 v4;
  sint32 v5;
  sint8 v6;
  sint32 v7;
  sint32 result;
  gt_draft_unresolved_call(0x80029898u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x80029830u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v0 = r_u16((uint32)(0x800BC1C8));
  v1 = r_u8((uint32)(0x8008F88Du));
  v2 = r_u16((uint32)(0x800BC1C6));
  v3 = (sint8)r_u8((uint32)(0x8008F88Eu));
  v4 = (r_u16((uint32)(0x800BC1C8)) & 0x20);
  if (((r_u16((uint32)(0x800BC1C8)) & 0x80) != 0))
  {
    gt_draft_800313A4(0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u8((uint32)(0x8008F88Cu), (uint32)(7));
    (v1 -= 1u);
    v4 = (v0 & 0x20);
    if ((v1 == (sint32)(0u - (uint32)(1))))
      v1 = (sint32)((uint32)(v3) - (uint32)(1));
  }
  if (v4)
  {
    gt_draft_800313A4(0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u8((uint32)(0x8008F88Cu), (uint32)(7));
    if (((v1 += 1u) >= v3))
      v1 = (((uint32)(v1) & 0xFFFFFF00u) | (((uint32)(0) << 0u) & 0x000000FFu));
  }
  w_u8((uint32)(0x8008F88Du), (uint32)(v1));
  v5 = (v0 & 0x6000);
  if (!r_u8((uint32)(0x8008F892u)))
  {
    if (((v2 & 0x10) != 0))
    {
      v6 = (sint32)(0u - (uint32)(3));
      if (!(sint8)r_u8((uint32)(0x8008F88Fu)))
      {
        LABEL_14:
        v5 = (v0 & 0x6000);

        goto LABEL_15;
      }
    }
    else
    {
      v5 = (v0 & 0x6000);
      if (((v2 & 0x40) == 0))
        goto LABEL_15;
      v6 = 3;
      if (((sint8)r_u8((uint32)(0x8008F88Fu)) >= (sint8)r_u8((uint32)(0x8008F890u))))
        goto LABEL_14;
    }
    w_u8((uint32)(0x8008F892u), (uint32)(v6));
    goto LABEL_14;
  }
  LABEL_15:
  v7 = (v5 == 0);

  result = 0;
  if (!v7)
  {
    gt_draft_800313A4(4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return 1;
  }
  return result;
}

