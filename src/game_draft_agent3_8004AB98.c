#include "game_first2_registry.h"
#include "game_draft_registry.h"

sint32 gtmain_menu_part_check(uint32 target, sint32 part_type)
{
    uint32 context = r_u32(0x800BEA64u);
    /* TODO Bind unselected continuation with the loaded context */
    return (sint32)gt_draft_unresolved_call(0x8004ABA0u, target, (uint32)part_type, context, 0, 0, 0, 0, 0);
}
