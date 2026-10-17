#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static void machine_record_reset(uint32 record)
{
    /* TODO Bind unselected record reset 8004B24C */
    (void)gt_draft_unresolved_call(0x8004B24Cu, record, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 800452D0 */
uint32 gtmain_menu_machine_status_initialize(void)
{
    printf((const char *)psx_addr(0x8008D970u, 1u));
    machine_record_reset(0x8009303Cu);
    machine_record_reset(0x80092780u);
    w_u8(0x800B5304u, 0u);
    return 0u;
}
