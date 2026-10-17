#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* Unverified compatibility entry for the corrected original header stride */
uint32 gt_draft_8002455C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_course_headers_relocate(a0);
}
