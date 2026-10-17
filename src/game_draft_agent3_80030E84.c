#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80030E84(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v2;
  sint32 v3;
  sint32 v4;
  sint32 result;
  v2 = 0;
  if ((r_u32((uint32)(0x800B49D8)) <= 0))
    return (sint32)(0u - (uint32)(1));
  v3 = 0;
  while (1)
  {
    v4 = (gtmain_string_compare(((uint32)((sint32)((uint32)(8) * (uint32)((sint32)((uint32)(v3) + (uint32)(v2))))) + (uint32)(r_u32((uint32)(0x800B49D4)))), (sint32)a0) == 0);
    result = v2;
    if (v4)
      break;
    (v2 += 1u);
    v3 = (sint32)((uint32)(2) * (uint32)(v2));
    if ((v2 >= r_u32((uint32)(0x800B49D8))))
      return (sint32)(0u - (uint32)(1));
  }

  return result;
}

