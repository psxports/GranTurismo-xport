#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void settings_centered_text(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity)
{
    gtmain_parts_caption_draw(ordering, text, x, y, intensity);
}

static void settings_overlay(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    gtmain_parts_overlay_draw(ordering, x, y, intensity);
}

/* GTMAIN.EXE 80044C90 */
/* Original: GTMAIN.EXE:sub_80044C90. */ uint32 gt_draft_80044C90(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    sint32 selection;
    uint32 text;
    (void)gtmain_menu_descriptor_draw(ordering, 0x80092C28u,
        (sint32)((uint32)x + 74u), (sint32)((uint32)y + 80u),
        intensity, 0u, NULL, 0u);
    selection = (sint8)r_u8(0x80092C29u);
    text = r_u32(0x80092C38u + 4u * (uint32)selection);
    settings_centered_text(ordering, text, x, y, intensity);
    (void)gtmain_menu_vehicle_course_labels_draw(ordering, x, y, intensity);
    settings_overlay(ordering, x, y, intensity);
    return 0u;
}
