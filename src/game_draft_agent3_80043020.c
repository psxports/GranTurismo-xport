#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80043020(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 v1;
  sint16 v2;
  sint32 v3;
  sint32 result;
  v0 = gt_draft_unresolved_call(0x8001EB94u, 17, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v1 = r_u16((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)((uint8)(r_u8((uint32)(0x800941CEu))))))));
  v2 = r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(74)))));
  w_u16((uint32)(0x80092A08u), (uint32)(1));
  w_u16((uint32)(0x80092A0Cu), (uint32)((sint32)((uint32)(v2) + (uint32)(1))));
  v3 = (sint32)((uint32)((sint32)((uint32)(28) * (uint32)((sint32)((uint32)(v1) - (uint32)(1))))) + (uint32)(v0));
  w_u16((uint32)(0x80092A0Au), (uint32)(r_u8((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(4)))))));
  v0 = (((uint32)(v0) & 0xFFFF0000u) | (((uint32)(r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(87)))))) << 0u) & 0x0000FFFFu));
  w_u16((uint32)(0x80092A18u), (uint32)(1));
  w_u16((uint32)(0x80092A1Cu), (uint32)((sint32)((uint32)(v0) + (uint32)(1))));
  result = r_u8((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(7)))));
  w_u16((uint32)(0x80092A1Au), (uint32)(r_u8((uint32)((uint32)((sint32)((uint32)(v3) + (uint32)(7)))))));
  return result;
}

