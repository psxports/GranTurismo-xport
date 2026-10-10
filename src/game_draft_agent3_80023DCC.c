#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80023DCC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_course_segment_find(a0, (const uint8 *)psx_addr(a1, 12u));
}

