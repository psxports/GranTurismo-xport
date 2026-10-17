#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_800313F8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  sint32 v7;
  sint32 i;
  sint32 v9;
  result = (sint16)r_u16((uint32)(0x800BC1A6));
  v7 = (sint32)(0u - (uint32)(2146736048));
  for (i = 0; (i < (sint16)r_u16((uint32)(0x800BC1A6))); v7 += 3380)
  {
    v9 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)(v7) + (uint32)(26)))));
    if ((v9 == 1))
    {
      if (!a0)
        goto LABEL_11;
      gt_draft_unresolved_call(a0, v7, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      (i += 1u);
    }
    else
    {
      if ((v9 != 2))
      {
        if (a2)
          gt_draft_unresolved_call(a2, v7, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        LABEL_11:
        (i += 1u);

        goto LABEL_12;
      }
      if (!a1)
        goto LABEL_11;
      gt_draft_unresolved_call(a1, v7, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      (i += 1u);
    }
    LABEL_12:
    result = (i < (sint16)r_u16((uint32)(0x800BC1A6)));

  }

  return result;
}

