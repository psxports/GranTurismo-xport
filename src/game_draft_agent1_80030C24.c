#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Compatibility entry delegates to the integrated native owner */
uint32 gt_draft_80030C24(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_race_assist_tick();
}
