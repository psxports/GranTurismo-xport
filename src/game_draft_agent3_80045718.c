#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void selection_dropdown(uint32 target, uint32 ordering, uint32 descriptor, sint32 x, sint32 y,
    sint32 selection_style, sint32 intensity, sint32 mode)
{
    /* TODO Bind unselected dropdown renderer with seven semantic arguments */
    (void)gt_draft_unresolved_call(target, ordering, descriptor, (uint32)x, (uint32)y,
        (uint32)selection_style, (uint32)intensity, (uint32)mode, 0u);
}

static void selection_centered_text(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity)
{
    /* TODO Bind unselected centered text renderer */
    (void)gt_draft_unresolved_call(0x80043F34u, ordering, text, (uint32)x, (uint32)y,
        (uint32)intensity, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80045718 */
uint32 gt_draft_80045718(uint32 ordering, sint32 x, sint32 y, sint32 selection_style, sint32 intensity)
{
    sint32 half = intensity >> 1, category, mode, index;
    uint32 name, descriptor, table;
    gtmain_text_font(1);
    gtmain_text_flags(0);
    (void)gtmain_text_color(gtmain_menu_palette_color(7, half));
    name = gtmain_resource_name((uint32)(sint32)(sint16)r_u16(0x800B63C4u));
    (void)gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 24u),
        (sint32)((uint32)y + 40u), name, 0);
    category = (sint8)r_u8(0x80093049u);
    (void)gtmain_text_color(gtmain_menu_gray((uint8)half));
    gtmain_text_font(2);
    gtmain_text_flags(2);
    mode = (sint8)r_u8(0x800B5304u);
    selection_dropdown(0x8004B2FCu, ordering, 0x8009303Cu,
        (sint32)((uint32)x + 32u), (sint32)((uint32)y + 56u),
        selection_style, intensity, mode == 0);
    descriptor = gt_draft_80045320((uint32)category, 0, 0, 0, 0, 0, 0, 0);
    mode = (sint8)r_u8(0x800B5304u);
    selection_dropdown(0x8004BDB0u, ordering, descriptor,
        (sint32)((uint32)x + 64u), (sint32)((uint32)y + 56u),
        selection_style, intensity, mode);
    mode = (sint8)r_u8(0x800B5304u);
    if (mode) {
        index = (sint8)r_u8(descriptor + 1u);
        table = r_u32(0x800931DCu + 4u * (uint32)category);
        name = r_u32(table + 4u * (uint32)index);
    } else {
        name = 0x800BE291u;
    }
    selection_centered_text(ordering, name, x, y, intensity);
    return 0u;
}
