#include "game_first2_registry.h"

/* GTMAIN.EXE:800365EC */
uint32 gtmain_menu_race_list_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    uint8 descriptor[152];
    uint32 index = 0;
    sint32 extent, descriptor_y;
    (void)gtmain_menu_race_caption_draw(ordering, x, y, intensity, r_s8(0x801F2FEBu) >= 3);
    extent = gtmain_menu_list_extent(r_u32(0x801F2FE0u));
    descriptor_y = (sint32)((uint32)y - ((uint32)(extent >> 1) - 128u));
    (void)gtmain_menu_descriptor_draw(ordering + 4u, r_u32(0x801F2FE0u),
        (sint32)((uint32)x + 32u), descriptor_y, intensity, 1, descriptor, sizeof descriptor);
    for (;;) {
        sint32 row;
        if (index >= sizeof descriptor - 2u)
            gt_draft_failure("Native race list descriptor sentinel extent", 0x800365ECu);
        row = (sint8)descriptor[2u + index];
        if (row < 0) break;
        if (r_s8(r_u32(0x801F2FE4u) + (uint32)row) == 4) {
            uint32 label = 0x800BC305u;
            uint32 width;
            sint32 label_x, label_y;
            if (r_u8(0x800CB764u) == 1) label = 0x800BC300u;
            gtmain_text_flags(0);
            (void)gtmain_text_font(2);
            (void)gtmain_text_color(gtmain_menu_palette_color(6, intensity >> 1));
            width = gtmain_menu_text_width(2, 0x800BD7DDu, 1);
            if (index >= (sizeof descriptor - 98u) / 2u)
                gt_draft_failure("Native race list descriptor coordinates extent", 0x800365ECu);
            label_x = (sint16)xport_load_le16(descriptor + 50u + 2u * index);
            label_y = (sint16)xport_load_le16(descriptor + 98u + 2u * index);
            (void)gtmain_menu_text_draw(ordering, (sint32)((uint32)label_x + width + 16u), label_y, label, 1);
        }
        ++index;
    }
    if (r_s8(0x801F2FE8u) == 1)
        (void)gtmain_menu_indicator_draw(ordering + 8u, (sint32)((uint32)x + 196u),
            (sint32)((uint32)y + 136u), r_s16(0x801F2FDEu), 520, 0, 3, 2);
    return 0;
}
