#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001DF84(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  gt_draft_unresolved_call(0x8007BD58u, (sint32)a0, 0, 276, 0u, 0u, 0u, 0u, 0u);
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4)))), (uint32)(320));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(8)))), (uint32)(160));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(12)))), (uint32)(164));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(14)))), (uint32)(48));
  w_u16((uint32)((uint32)((sint32)a0)), (uint32)(0));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(2)))), (uint32)(0));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(16)))), (uint32)(32));
  if ((r_u8((uint32)(0x800CB7F5)) == 2))
  {
    w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))), (uint32)(120));
    w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(10)))), (uint32)(52));
  }
  else
  {
    w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))), (uint32)(240));
    w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(10)))), (uint32)(104));
  }
  gt_draft_unresolved_call(0x8006B3E0u, (sint32)a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))), (uint32)((sint32)a1));
  w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)((sint32)a2));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(262)))), (uint32)(214));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(258)))), (uint32)(r_u8((uint32)(0x800CB75D))));
  result = r_u8((uint32)(0x800CB75D));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(260)))), (uint32)(r_u8((uint32)(0x800CB75D))));
  return result;
}

