#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* Original signed countdown load precedes secondary entry */
uint32 gt_draft_800380AC(void)
{
    sint32 counter = (sint16)r_u16(0x801F300Au);
    return gtmain_menu_countdown_toggle_tail(counter);
}
