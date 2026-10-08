#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80025EAC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 result;
  sint32 v2;
  sint32 v3;
  v0 = (((uint32)(v0) & 0xFFFF0000u) | (((uint32)(r_u16((uint32)(0x800B63DA))) << 0u) & 0x0000FFFFu));
  if ((r_u16((uint32)(0x800B63D6)) == 8))
  {
    v0 = ((uint32)(r_u16((uint32)(0x800B63DA))) + (uint32)(1));
    result = 9;
    if ((v0 >= 56))
      w_u16((uint32)(0x800B63D6), (uint32)(9));
  }
  else
  {
    result = (r_u16((uint32)(0x800B63D6)) < 6);
    if ((r_u16((uint32)(0x800B63D6)) < 9))
    {
      if ((r_u16((uint32)(0x800B63D6)) >= 6))
      {
        v0 = (((uint32)(v0) & 0xFFFF0000u) | (((uint32)(((uint32)(r_u16((uint32)(0x800B63DA))) - (uint32)(1))) << 0u) & 0x0000FFFFu));
        result = 1;
        if ((r_u16((uint32)(0x800B63DA)) == 1))
        {
          result = 3;
          if ((r_u32((uint32)(0x800B630C)) == 1))
          {
            LABEL_15:
            w_u8((uint32)(0x800B63C0), (uint32)(result));

            goto LABEL_16;
          }
          v2 = 16;
          if (!r_u16((uint32)(0x800B6874)))
            v2 = 18;
          gt_draft_8002ABC0(v2, 1, 0u, 0u, 0u, 0u, 0u, 0u);
          w_u16((uint32)(0x800B63D6), (uint32)(8));
          w_u8((uint32)(0x800B63C8), (uint32)(0));
          result = gt_draft_unresolved_call(0x80020534u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
      }
    }
    else
    {
      result = ((uint32)(16) * (uint32)(r_u32((uint32)(0x800BF518))));
      if ((r_u16((uint32)(0x800B63D6)) == 9))
      {
        v0 = ((uint32)(r_u16((uint32)(0x800B63DA))) + (uint32)(1));
        result = 2;
        if ((v0 >= ((uint32)(((uint32)(15) * (uint32)(r_u32((uint32)(0x800BF518))))) + (uint32)(56))))
          goto LABEL_15;
        v3 = (gt_draft_80030B20(r_u32((uint32)(0x800BF518)), 0u, 0u, 0u, 0u, 0u, 0u, 0u) < 0);
        result = 2;
        if (!v3)
          goto LABEL_15;
      }
    }
  }
  LABEL_16:
  w_u16((uint32)(0x800B63DA), (uint32)(v0));

  return result;
}

