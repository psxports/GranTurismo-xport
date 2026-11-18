#include "game_first2_registry.h"
#include "xport_trace.h"

uint32 gtmain_menu_settings_mask(uint32 table, sint32 count)
{
    uint32 context;
    FUNCTION_MARKER(0x8004A9B8, "GTMAIN.EXE");
    context = r_u32(0x800BEA64);
    return gtmain_menu_settings_mask_context(table, count, context);
}
