#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

uint32 gtmain_menu_machine_status_initialize(void)
{
    fputs((const char *)psx_addr(0x8008D970u, 21u), stdout);
    /* TODO Bind unselected reset of actual record fields */
    gt_draft_unresolved_call(0x8004B24Cu, 0x8009303Cu, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8004B24Cu, 0x80092780u, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x800B5304u, 0);
    return 0;
}
