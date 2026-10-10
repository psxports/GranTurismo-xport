#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80023FA4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_course_progress(a0, (const uint8 *)psx_addr(a1, 12u));
}
