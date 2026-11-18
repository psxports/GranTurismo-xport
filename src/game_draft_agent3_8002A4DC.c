#include "game_draft_registry.h"

uint32 gt_draft_8002A4DC(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8002A4DC, "GTMAIN.EXE");
    index = r_u8(0x800CB7F8);
    return r_u32(0x8008FAE4 + 4 * index);
}
