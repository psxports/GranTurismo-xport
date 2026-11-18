#include "game_draft_registry.h"

/* Clear the menu state byte */
/* Original: GTMAIN.EXE:sub_80049D98. */ void gt_draft_80049D98(void)
{

    w_u8(0x800B53B4u, 0);
}
