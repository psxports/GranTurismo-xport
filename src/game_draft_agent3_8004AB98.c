#include "game_first2_registry.h"
#include "game_draft_registry.h"

sint32 gtmain_menu_part_check(uint32 target, sint32 part_type)
{
    uint32 context = r_u32(0x800BEA64u);
    return gt_draft_8004ABA0(target, part_type, context);
}
