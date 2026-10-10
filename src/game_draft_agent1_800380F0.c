#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* TODO Legacy raw ABI adapter retained for existing callers */
uint32 gt_draft_800380F0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_menu_machine_caption_draw(a0, (sint32)a1, (sint32)a2, (sint32)a3);
}
