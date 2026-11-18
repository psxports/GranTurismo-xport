#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>
#include "xport_trace.h"

static void machine_record_reset(uint32 record)
{
    gtmain_menu_category_reset(record);
}

/* GTMAIN.EXE 800452D0 */
/* Original: GTMAIN.EXE:sub_800452D0. */ uint32 gtmain_menu_machine_status_initialize(void)
{
    FUNCTION_MARKER(0x800452D0, "GTMAIN.EXE");
    printf((const char *)psx_addr(0x8008D970u, 21u));
    machine_record_reset(0x8009303Cu);
    machine_record_reset(0x80092780u);
    w_u8(0x800B5304u, 0u);
    return 0u;
}
