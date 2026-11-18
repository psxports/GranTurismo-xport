#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gt_draft_80027AE8(void)
{
    sint64 numerator;
    sint32 value;
    uint32 result;
    FUNCTION_MARKER(0x80027AE8, "GTMAIN.EXE");
    numerator = (sint64)r_s32(0x800B6348) * 4096;
    value = gtmain_signed_divide64(numerator, 600000, 0x80027B1C);
    result = value < 4096;
    if (!result)
        value = 4096;
    w_u16(0x800BB7EA, value);
    return result;
}
