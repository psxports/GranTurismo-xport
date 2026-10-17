#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* TODO Legacy raw ABI adapter retained for existing callers */
uint32 gt_draft_80030974(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_race_centered_label_box_draw(a0, a1, a2, a3, (sint32)a4);
}
