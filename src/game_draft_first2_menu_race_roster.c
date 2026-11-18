#include "game_first2_registry.h"

static sint32 roster_native_width(sint32 font, const char *text, sint32 spacing)
{
    /* TODO Bind proportional native string width with spacing */
    gt_draft_failure("Native roster string width adapter", 0x8003A8F4u);
}

static uint32 roster_native_draw(uint32 ordering, sint32 x, sint32 y, const char *text, sint32 spacing)
{
    /* TODO Bind native string drawing with the original spacing contract */
    gt_draft_failure("Native roster string draw adapter", 0x8003A8ACu);
}

/* GTMAIN.EXE:8003687C */
/* Original: GTMAIN.EXE:sub_8003687C. */ uint32 gtmain_menu_race_roster_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    uint8 descriptor[152];
    char digit[8];
    uint32 row;
    digit[0] = 0;
    digit[1] = 0;
    (void)gtmain_menu_descriptor_draw(ordering + 4u, 0x80091BD8u,
        (sint32)((uint32)x + 32u), (sint32)((uint32)y + 72u), intensity, 1, descriptor, sizeof descriptor);
    gtmain_text_flags(0);
    (void)gtmain_text_font(2);
    for (row = 0; row < 6u; ++row) {
        sint32 brightness;
        uint32 selected;
        uint32 color, slot, record = 0x800B6670u;
        sint32 width, row_y;
        digit[0] = (char)(row + 49u);
        brightness = descriptor[26u + row];
        selected = row == (uint32)(sint32)r_s8(0x80091BD9u);
        (void)gtmain_text_color(gtmain_menu_palette_color(6, brightness));
        color = gtmain_menu_gray((uint8)brightness);
        if (selected) {
            sint32 pulse = gtmain_menu_pulse_component(0, brightness);
            (void)gtmain_text_color(gtmain_menu_palette_color(6, pulse));
            pulse = gtmain_menu_pulse_component(0, brightness);
            color = gtmain_menu_gray((uint8)pulse);
        }
        width = roster_native_width(2, digit, 0);
        row_y = (sint16)xport_load_le16(descriptor + 98u + 2u * row);
        (void)roster_native_draw(ordering, (sint32)((uint32)x - ((uint32)(width >> 1) - 32u)), row_y, digit, 0);
        for (slot = 0; slot < 6u; ++slot, record += 76u) {
            uint32 name;
            if (r_u8(record + 3u) != row) continue;
            name = gtmain_resource_name(r_u16(record));
            (void)gtmain_text_color(color);
            row_y = (sint16)xport_load_le16(descriptor + 98u + 2u * row);
            (void)gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 48u), row_y, name, 0);
            if (r_s8(0x801F2FE9u) == 1 && selected)
                (void)gtmain_menu_indicator_draw(ordering + 8u, (sint32)((uint32)x + 196u),
                    (sint32)((uint32)y + 136u), r_s16(0x801F2FDEu), 520, slot, 3, 2);
            break;
        }
    }
    return 0;
}
