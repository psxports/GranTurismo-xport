#include "game_draft_registry.h"

uint32 gt_draft_8002A3AC(void)
{
    uint32 index, offset;
    FUNCTION_MARKER(0x8002A3AC, "GTMAIN.EXE");
    index = r_u8(0x800CB7F8);
    offset = r_u8(0x8008FB34 + index);
    return 0x800C7794 + offset;
}
