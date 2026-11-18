#include "game_draft_registry.h"

uint32 gt_draft_8002A474(void)
{
    uint32 table, count, index;
    FUNCTION_MARKER(0x8002A474, "GTMAIN.EXE");
    table = gt_draft_8002A3AC();
    count = gt_draft_8002A390();
    if (count <= 1)
        return 1;
    for (index = 0; index < count - 1; ++index) {
        if (!r_u8(table + index))
            return 0;
    }
    return 1;
}
