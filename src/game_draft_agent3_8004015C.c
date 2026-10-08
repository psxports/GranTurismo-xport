#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8004015C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint8 v0;
  sint32 v1;
  sint32 v2;
  w_u8((uint32)(0x800B5300), (uint32)(0));
  gt_draft_8003DAA8(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  if (!(sint8)r_u8((uint32)(0x801F1FE5)))
  {
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008D274u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (((sint8)r_u8((uint32)(0x801F1FE4)) != 1))
    {
      if ((((sint8)r_u8((uint32)(0x801F1FE4)) >= 2) && ((sint8)r_u8((uint32)(0x801F1FE4)) == 2)))
      {
        gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2C0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u32((uint32)(0x80092274u), (uint32)((sint32)(0u - (uint32)(2146705112))));
        w_u8((uint32)(0x80092271u), (uint32)(0));
        v0 = 5;
        goto LABEL_28;
      }
      gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2E8u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v2 = (sint32)(0u - (uint32)(2146708407));
      LABEL_27:
      w_u32((uint32)(0x80092274u), (uint32)(v2));

      w_u8((uint32)(0x80092271u), (uint32)(2));
      v0 = 5;
      goto LABEL_28;
    }
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2D4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (((sint8)r_u8((uint32)(0x801F2034)) == 1))
    {
      v1 = (sint32)(0u - (uint32)(2146712802));
    }
    else
      if (((sint8)r_u8((uint32)(0x801F2034)) >= 2))
    {
      if (((sint8)r_u8((uint32)(0x801F2034)) != 2))
      {
        LABEL_15:
        w_u8((uint32)(0x80092271u), (uint32)(2));

        v0 = 15;
        goto LABEL_28;
      }
      v1 = (sint32)(0u - (uint32)(2146712784));
    }
    else
    {
      if ((sint8)r_u8((uint32)(0x801F2034)))
        goto LABEL_15;
      v1 = (sint32)(0u - (uint32)(2146712822));
    }
    w_u32((uint32)(0x80092274u), (uint32)(v1));
    goto LABEL_15;
  }
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2F8u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  if (((sint8)r_u8((uint32)(0x801F1FE4)) != 1))
  {
    if ((((sint8)r_u8((uint32)(0x801F1FE4)) >= 2) && ((sint8)r_u8((uint32)(0x801F1FE4)) == 2)))
    {
      gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2C0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      w_u32((uint32)(0x80092274u), (uint32)((sint32)(0u - (uint32)(2146705088))));
      w_u8((uint32)(0x80092271u), (uint32)(0));
      v0 = 5;
      goto LABEL_28;
    }
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2E8u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v2 = (sint32)(0u - (uint32)(2146708388));
    goto LABEL_27;
  }
  if ((((sint8)r_u8((uint32)(0x801F2034)) == 3) || ((sint8)r_u8((uint32)(0x801F2034)) != 4)))
  {
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2D4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32((uint32)(0x80092274u), (uint32)((sint32)(0u - (uint32)(2146712759))));
    w_u8((uint32)(0x80092271u), (uint32)(2));
    v0 = 15;
  }
  else
  {
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008D2D4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32((uint32)(0x80092274u), (uint32)((sint32)(0u - (uint32)(2146712743))));
    w_u8((uint32)(0x80092271u), (uint32)(2));
    v0 = 15;
  }
  LABEL_28:
  w_u8((uint32)(0x80092288u), (uint32)(v0));

  gt_draft_8003DC70(0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return 0;
}

