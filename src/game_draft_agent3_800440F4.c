#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_800440F4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v0;
  sint32 v1;
  sint32 v2;
  sint32 result;
  uint32 v4;
  sint32 v5;
  sint32 v6;
  uint32 v7;
  v0 = (uint32)(r_u32((uint32)(0x800BDD08)));
  gt_draft_unresolved_call(0x8003A304u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v1 = 0;
  if (!(r_u8((uint32)(v0))))
    return v1;
  gt_draft_unresolved_call(0x80035388u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  switch (r_u8((uint32)(0x800B5304)))
  {
    case 1:
      v6 = gt_draft_80046AF4(0x80092C78u, (v0 + 1u * (uint32)(2)), 0u, 0u, 0u, 0u, 0u, 0u);
      if ((v6 == (sint32)(0u - (uint32)(2))))
      goto LABEL_13;
      if ((v6 == (sint32)(0u - (uint32)(1))))
      return v1;
      if (v6)
      goto LABEL_16;
      gt_draft_unresolved_call(0x80035778u, 5, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_80046AC4(r_u32((uint32)(0x80092C68u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      w_u8((uint32)(0x800B5304), (uint32)(2));
      return 0;

    case 2:

    case 3:
      v2 = gt_draft_80046AF4(r_u32((uint32)(0x80092C68u)), (v0 + 1u * (uint32)(2)), 0u, 0u, 0u, 0u, 0u, 0u);
      if ((v2 == (sint32)(0u - (uint32)(2))))
      goto LABEL_13;
      result = 0;
      if ((v2 == (sint32)(0u - (uint32)(1))))
      return result;
      gt_draft_unresolved_call(0x80035778u, 4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_8001EE00(v2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      if ((r_u8((uint32)(0x800B5304)) == 2))
    {
      w_u8((uint32)(0x800BEA24), (uint32)(1));
      v4 = 0x80092170u;
      goto LABEL_23;
    }
      w_u8((uint32)(0x800BEA24), (uint32)(0));
      return 1;

    case 4:
      v5 = gt_draft_80046AF4(0x80092C88u, (v0 + 1u * (uint32)(2)), 0u, 0u, 0u, 0u, 0u, 0u);
      if ((v5 == (sint32)(0u - (uint32)(2))))
    {
      LABEL_13:
      gt_draft_unresolved_call(0x80035778u, 3, 0u, 0u, 0u, 0u, 0u, 0u, 0u);

      w_u8((uint32)(0x800B5304), (uint32)(0));
      result = 0;
    }
    else
    {
      if ((v5 == (sint32)(0u - (uint32)(1))))
        return v1;
      if (v5)
      {
        LABEL_16:
        w_u8((uint32)(0x800B5304), (uint32)(0));

        gt_draft_unresolved_call(0x80035778u, 5, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        result = 0;
      }
      else
      {
        w_u8((uint32)(0x800BEA24), (uint32)((sint32)(0u - (uint32)(1))));
        gt_draft_unresolved_call(0x80035778u, 4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        v4 = 0x80092170u;
        LABEL_23:
        gt_draft_unresolved_call(0x800352DCu, v4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);

        result = 3;
      }
    }
      break;

    default:
      switch (gt_draft_unresolved_call(0x8003B954u, 0x80092BDCu, (sint32)((v0 + 1u * (uint32)(2))), 0u, 0u, 0u, 0u, 0u, 0u))
    {
      case (sint32)(0u - (uint32)(2)):

      case 4:
        w_u8((uint32)(0x800B5304), (uint32)(4));
        gt_draft_unresolved_call(0x80035778u, 5, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        v7 = 0x80092C88u;
        goto LABEL_25;

      case 0:
        w_u8((uint32)(0x800B5304), (uint32)(1));
        gt_draft_unresolved_call(0x80035778u, 5, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        v7 = 0x80092C78u;
        LABEL_25:
      gt_draft_80046AD4(v7, 0u, 0u, 0u, 0u, 0u, 0u, 0u);

        return v1;

      case 1:
        gt_draft_unresolved_call(0x80035778u, 5, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u8((uint32)(0x800B5304), (uint32)(3));
        gt_draft_80046AC4(r_u32((uint32)(0x80092C68u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return 0;

      case 2:
        gt_draft_unresolved_call(0x80035778u, 4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        v4 = 0x80092C98u;
        goto LABEL_23;

      case 3:
        gt_draft_unresolved_call(0x80035778u, 4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        v4 = 0x80092D40u;
        goto LABEL_23;

      default:
        return v1;

    }


  }

  return result;
}

