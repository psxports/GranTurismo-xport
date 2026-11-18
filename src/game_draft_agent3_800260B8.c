#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

uint32 gt_draft_800260B8(uint32 a0)
{
  sint32 v1;
  sint32 v2;
  sint32 v3;
  sint32 result;
  sint32 v5;
  FUNCTION_MARKER(0x800260B8, "GTMAIN.EXE");
  if ((sint32)a0)
  {
    v2 = ((uint32)(180) * (uint32)(r_u8((uint32)(0x800B63BD))));
    v1 = (sint32)(0u - (uint32)(2146650852));
  }
  else
  {
    v1 = (sint32)(0u - (uint32)(2146652292));
    v2 = ((uint32)(180) * (uint32)(r_u8((uint32)(0x800B63BD))));
  }
  v3 = (sint32)((uint32)(v2) + (uint32)(v1));
  result = r_u32((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(24)))));
  v5 = r_u32((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(4)))));
  w_u32((uint32)(0x800BDB4C), (uint32)(result));
  w_u32((uint32)(0x800BDB48), (uint32)(v5));
  return result;
}

