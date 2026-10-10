#include "game_first2_registry.h"

/* Native compatibility boundary */
uint32 gt_draft_80030FFC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_texture_pixels_load(a0, a1, a2);
}
