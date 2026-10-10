#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gtmain_menu_settings_mask(uint32 table, sint32 count)
{
    uint32 context = r_u32(0x800BEA64u);
    /* TODO Bind unselected continuation with table, count and carried context */
    return gt_draft_unresolved_call(0x8004A9C0u, table, (uint32)count, context, 0, 0, 0, 0, 0);
}
