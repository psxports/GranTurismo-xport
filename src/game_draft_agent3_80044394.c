#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void menu_selection_centered_text(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity)
{
    gtmain_parts_caption_draw(ordering, text, x, y, intensity);
}

/* GTMAIN.EXE 80044394 */
/* Original: GTMAIN.EXE:sub_80044394. */ uint32 gt_draft_80044394(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    sint32 mode;
    uint32 descriptor, publish;
    gtmain_menu_descriptor_draw(ordering, 0x80092BDCu,
        (sint32)((uint32)x + 74u), (sint32)((uint32)y + 80u),
        intensity, 0u, NULL, 0u);
    mode = (sint8)r_u8(0x800B5304u);
    if (mode >= 1 && mode <= 4)
    {
        descriptor = mode == 1 ? 0x80092C78u : mode == 4 ? 0x80092C88u : 0x80092C68u;
        publish = mode == 4 ? 0u : 1u;
        gtmain_menu_choice_transition_draw(ordering, descriptor,
            (sint32)((uint32)x + 160u), (sint32)((uint32)y + 192u),
            intensity, publish, 0u, 0u);
    }
    else
    {
        sint32 choice = (sint8)r_u8(0x80092BDDu);
        descriptor = r_u32(0x80092BECu + (uint32)choice * 4u);
        menu_selection_centered_text(ordering, descriptor, x, y, intensity);
    }
    gtmain_menu_vehicle_course_labels_draw(ordering, x, y, intensity);
    return 0u;
}
