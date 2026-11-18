#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "xport_trace.h"

static void selection_centered_text(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity)
{
    gtmain_parts_caption_draw(ordering, text, x, y, intensity);
}

static uint32 selection_descriptor(sint32 category)
{
    return gtmain_parts_category_descriptor(category);
}

/* GTMAIN.EXE 80045128 */
/* Original: GTMAIN.EXE:sub_80045128. */ uint32 gt_draft_80045128(uint32 ordering, sint32 x, sint32 y, sint32 selection_style, sint32 intensity)
{
    sint32 half = intensity >> 1, category, mode, index;
    uint32 name, descriptor, table;
    FUNCTION_MARKER(0x80045128, "GTMAIN.EXE");
    gtmain_text_flags(1);
    gtmain_text_font(0);
    (void)gtmain_text_color(gtmain_menu_palette_color(7, half));
    name = gtmain_resource_name((uint32)(sint32)(sint16)r_u16(0x800B63C4u));
    (void)gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 24u),
        (sint32)((uint32)y + 40u), name, 0);
    category = (sint8)r_u8(0x8009278Du);
    (void)gtmain_text_color(gtmain_menu_gray((uint8)half));
    gtmain_text_flags(2);
    gtmain_text_font(2);
    mode = (sint8)r_u8(0x800B5304u);
    gtmain_menu_category_draw(ordering, 0x80092780u,
        (sint32)((uint32)x + 32u), (sint32)((uint32)y + 56u),
        selection_style, intensity, mode == 0);
    descriptor = selection_descriptor(category);
    mode = (sint8)r_u8(0x800B5304u);
    gtmain_menu_dropdown_draw(ordering, descriptor,
        (sint32)((uint32)x + 64u), (sint32)((uint32)y + 56u),
        selection_style, intensity, mode);
    mode = (sint8)r_u8(0x800B5304u);
    if (mode) {
        index = (sint8)r_u8(descriptor + 1u);
        table = r_u32(0x800928F8u + 4u * (uint32)category);
        name = r_u32(table + 4u * (uint32)index);
    } else {
        name = 0x800BE2B0u;
    }
    selection_centered_text(ordering, name, x, y, intensity);
    return 0u;
}
