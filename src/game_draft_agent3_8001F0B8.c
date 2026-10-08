#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001F0B8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  if (((sint32)a0 >= 97))
    a0 -= 32;
  return r_u32((uint32)((0x8008F30Cu + 1u * (uint32)((sint32)a0))));
}

