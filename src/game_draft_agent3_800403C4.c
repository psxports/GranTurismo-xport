#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* TODO Legacy raw ABI adapter retained for existing callers */
uint32 gt_draft_800403C4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_menu_machine_context_bind((uint8)a0, a1, a2, (uint8)a3, a4, a5);
}
