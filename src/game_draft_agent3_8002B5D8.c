#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8002B5D8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 result;
  if ((sint8)r_u8((uint32)(0x800B633C)))
    gt_draft_unresolved_call(0x800702F8u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  result = 4;
  if ((r_u8((uint32)(0x800CB7F5)) == 4))
    return gt_draft_8002A124(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  return result;
}

