#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002A8B4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 v1;
  w_u32((uint32)(0x800B62F8), (uint32)(1));
  w_u32((uint32)(0x800B62FC), (uint32)(1));
  gt_draft_unresolved_call(0x8006F838u, (sint32)(0u - (uint32)(2146737424)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u32((uint32)(0x800B6358), (uint32)(2));
  w_u32((uint32)(0x800B635C), (uint32)(2));
  gt_draft_unresolved_call(0x8006F838u, (sint32)(0u - (uint32)(2146737328)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u32((uint32)(0x800B6378), (uint32)(4));
  w_u32((uint32)(0x800B637C), (uint32)(4194));
  gt_draft_unresolved_call(0x8006F838u, (sint32)(0u - (uint32)(2146737296)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u32((uint32)(0x800B6328), (uint32)(5));
  w_u32((uint32)(0x800B632C), (uint32)(4192));
  gt_draft_unresolved_call(0x8006F838u, (sint32)(0u - (uint32)(2146737376)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u32((uint32)(0x800B63E8), (uint32)(6));
  w_u32((uint32)(0x800B63EC), (uint32)(2));
  gt_draft_unresolved_call(0x8006F838u, (sint32)(0u - (uint32)(2146737184)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006F900u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x80027288u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_8002C7A4(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u8((uint32)(0x800B633C), (uint32)(0));
  w_u8((uint32)(0x800B63C8), (uint32)(1));
  w_u8((uint32)(0x800B63C2), (uint32)(0));
  if (r_u32((uint32)(0x800B630C)))
    goto LABEL_11;
  v0 = r_u8((uint32)(0x800CB7F5));
  if ((r_u8((uint32)(0x800CB7F5)) == 2))
  {
    w_u8((uint32)(0x800B633C), (uint32)(1));
    goto LABEL_11;
  }
  if ((r_u8((uint32)(0x800CB7F5)) >= 3u))
  {
    if ((r_u8((uint32)(0x800CB7F5)) != 5))
      goto LABEL_11;
    goto LABEL_9;
  }
  if ((r_u8((uint32)(0x800CB7F5)) == 1))
  {
    if ((r_u8((uint32)(0x800CB7F4)) == 1))
    {
      LABEL_11:
      v0 = r_u8((uint32)(0x800CB7F5));

      goto LABEL_12;
    }
    LABEL_9:
    w_u8((uint32)(0x800B63C2), (uint32)(1));

    gt_draft_unresolved_call(0x80030C08u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    goto LABEL_11;
  }
  LABEL_12:
  if ((v0 == 2))
  {
    LABEL_20:
    w_u8((uint32)(0x800B63C8), (uint32)(0));

    goto LABEL_21;
  }

  if ((v0 >= 3))
  {
    if (((v0 < 6) && (v0 >= 4)))
      goto LABEL_20;
  }
  else
    if (((v0 == 1) && (r_u32((uint32)(0x800B6318)) < 2)))
  {
    goto LABEL_20;
  }
  LABEL_21:
  gt_draft_8001DF84((sint32)(0u - (uint32)(2146737136)), (sint32)(0u - (uint32)(2146736048)), (sint32)(0u - (uint32)(2146713152)), 0u, 0u, 0u, 0u, 0u);

  gt_draft_8001DF84((sint32)(0u - (uint32)(2146736860)), (sint32)(0u - (uint32)(2146732668)), (sint32)(0u - (uint32)(2146712960)), 0u, 0u, 0u, 0u, 0u);
  v1 = 2;
  if (r_u32((uint32)(0x800B630C)))
  {
    if ((r_u8((uint32)(0x800CB7F5)) != 2))
    {
      w_u16((uint32)(0x800B6508), (uint32)(1));
      w_u16((uint32)(0x800B661C), (uint32)(1));
    }
    v1 = 1;
  }
  gt_draft_unresolved_call(0x8001E324u, (sint32)(0u - (uint32)(2146736048)), (sint32)(0u - (uint32)(2146737136)), v1, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8001E324u, (sint32)(0u - (uint32)(2146732668)), (sint32)(0u - (uint32)(2146736860)), v1, 0u, 0u, 0u, 0u, 0u);
  return gt_draft_80029E3C(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

