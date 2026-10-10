#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* Original signed countdown load precedes secondary entry */
uint32 gt_draft_800380AC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 counter = (sint16)r_u16(0x801F300Au);
    return gtmain_menu_countdown_toggle_tail(counter);
}
