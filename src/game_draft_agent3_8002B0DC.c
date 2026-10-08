#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Original BREAK exception handler is an unresolved adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002B0DC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  sint32 v1;
  sint32 v2;
  sint32 v3;
  sint32 v4;
  result = r_u16((uint32)(0x800B6312));
  if (r_u16((uint32)(0x800B6312)))
  {
    v1 = gt3_sdiv((sint16)(((uint32)(r_u16((uint32)(0x800B6312))) - (uint32)(1))), (sint32)(r_u32((uint32)(0x800BF518))));
    if (!r_u32((uint32)(0x800BF518)))
      gt_draft_exception(7u);
    if (((r_u32((uint32)(0x800BF518)) == (sint32)(0u - (uint32)(1))) && ((sint16)(((uint32)(r_u16((uint32)(0x800B6312))) - (uint32)(1))) == 0x80000000)))
      gt_draft_exception(6u);
    v2 = r_u16((uint32)(0x800B63DE));
    (w_u16(0x800B6312, (uint32)(r_u16((uint32)(0x800B6312))) - 1u), r_u16((uint32)(0x800B6312)));
    w_u16((uint32)(0x800B63DE), (uint32)(v1));
    v3 = (v2 == v1);
    result = (v1 < 4);
    if ((!v3 && (v1 < 4)))
    {
      v4 = 8;
      if (!v1)
      {
        gt_draft_8002ABC0((sint8)r_u8((uint32)(0x800B63BF)), 1, 0u, 0u, 0u, 0u, 0u, 0u);
        v4 = 10;
      }
      return gt_draft_800313A4(v4, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
  }
  return result;
}

