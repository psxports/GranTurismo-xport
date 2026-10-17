#include "game_first2_registry.h"

uint32 gt_draft_8003BA30(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    /* TODO Bind the legacy published buffer to a real native 152 byte output */
    if (a6 != 0u)
        gt_draft_failure("Native legacy descriptor output binding and provenance", 0x8003BA30u);
    return (uint32)gtmain_menu_descriptor_draw(a0, a1, (sint32)a2, (sint32)a3,
        (sint32)a5, 0u, NULL, 0u);
}
