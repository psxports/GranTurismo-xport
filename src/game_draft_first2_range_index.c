#include "game_first2_registry.h"

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

/* GTMAIN.EXE 80041980 */
/* Original: GTMAIN.EXE:sub_80041980. */ sint32 gtmain_menu_range_index(sint32 start, sint32 end, sint32 value, sint32 count)
{
    sint32 span = (sint32)((uint32)end - (uint32)start);
    uint32 upper = (uint32)count - 1u;
    sint32 numerator, index;
    if (!span) return 0;
    numerator = (sint32)(((uint32)value - (uint32)start) * upper);
    if (span == -1 && (uint32)numerator == 0x80000000u)
        gt_draft_failure("Original MIPS BREAK6 signed division overflow", 0x800419CCu);
    index = numerator / span;
    if (index < 0) index = 0;
    if (index >= count) index = (sint32)upper;
    return index;
}
