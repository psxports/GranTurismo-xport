#include "game_draft_registry.h"
#include "game_first2_registry.h"

static sint32 parts_list_add(sint32 a, sint32 b)
{
    return (sint32)((uint32)a + (uint32)b);
}

static void parts_list_copy_summary(uint32 source)
{
    uint32 destination = 0x801D2FF0u, i;
    for (i = 0u; i < 6u; ++i) {
        uint32 first = r_u32(source), second = r_u32(source + 4u);
        uint32 third = r_u32(source + 8u), fourth = r_u32(source + 12u);
        w_u32(destination, first); w_u32(destination + 4u, second);
        w_u32(destination + 8u, third); w_u32(destination + 12u, fourth);
        source += 16u; destination += 16u;
    }
    {
        uint32 first = r_u32(source), second = r_u32(source + 4u);
        uint32 third = r_u32(source + 8u);
        w_u32(destination, first); w_u32(destination + 4u, second);
        w_u32(destination + 8u, third);
    }
}

static void parts_list_value_draw(uint32 ordering, sint32 x, sint32 y, uint32 value)
{
    /* TODO Bind unselected four-argument value renderer 80045BF4 */
    (void)gt_draft_unresolved_call(0x80045BF4u, ordering, (uint32)x,
        (uint32)y, value, 0u, 0u, 0u, 0u);
}

static void parts_list_detail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    uint32 summary, sint32 text_intensity, sint32 first_x, sint32 first_y,
    sint32 second_x, sint32 second_y, uint32 first_flags, uint32 second_flags)
{
    /* TODO Bind unselected twelve-argument detail renderer 8004D020 */
    gt_draft_failure("Parts list twelve-argument detail draw", 0x8004D020u);
}

static void parts_list_sprite(uint32 ordering, uint32 xy, uint32 texture, uint32 size, uint32 color)
{
    /* TODO Bind unselected five-argument sprite renderer 800706BC */
    (void)gt_draft_unresolved_call(0x800706BCu, ordering, xy, texture,
        size, color, 0u, 0u, 0u);
}

static void parts_list_caption(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity)
{
    /* TODO Bind unselected five-argument centered caption 80043F34 */
    (void)gt_draft_unresolved_call(0x80043F34u, ordering, text,
        (uint32)x, (uint32)y, (uint32)intensity, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80045CF0 */
uint32 gt_draft_80045CF0(uint32 ordering, sint32 x, sint32 y,
    uint32 carried_argument, sint32 intensity)
{
    uint8 output[152];
    sint32 extent = gtmain_menu_list_extent(0x80093530u);
    sint32 list_x, list_y, shade = intensity >> 1;
    uint32 category = r_u8(0x800B5374u), descriptor, texture, size, color, xy, page, text;
    if (category == 27u || category == 35u) {
        list_x = parts_list_add(x, 120);
        list_y = (sint32)((uint32)y - ((uint32)extent - 126u));
    } else {
        list_x = parts_list_add(x, 160);
        list_y = (sint32)((uint32)y - ((uint32)(extent >> 1) - 112u));
    }
    (void)gtmain_menu_descriptor_draw(ordering, 0x80093530u, list_x, list_y,
        intensity, 1u, output, sizeof(output));
    category = r_u8(0x800B5374u);
    if (category == 34u) {
        uint32 i;
        (void)gtmain_text_font(2);
        for (i = 0u; i < 9u; ++i) {
            uint32 brightness = output[26u + i];
            uint32 value;
            sint32 row_y;
            (void)gtmain_text_color(gtmain_menu_gray((uint8)brightness));
            value = r_u8(0x800931F9u + i * 8u);
            row_y = (sint16)((uint16)output[98u + i * 2u] |
                ((uint16)output[99u + i * 2u] << 8));
            parts_list_value_draw(ordering, parts_list_add(x, 152), row_y, value);
        }
    } else if (category == 27u || category == 35u) {
        if (category == 27u) shade = 0;
        shade >>= 2;
        if (r_s16(0x801D2FDCu) < 0) {
            sint32 selected = r_s8(0x80093531u);
            uint32 empty = r_s8(0x801D2FE0u + (uint32)selected) == 0;
            parts_list_copy_summary(0x801D35D8u);
            parts_list_detail_draw(ordering, parts_list_add(x, 176), parts_list_add(y, 80),
                intensity, 0x801D2FE8u, intensity >> 2, parts_list_add(x, 140),
                parts_list_add(y, 138), parts_list_add(x, 140), parts_list_add(y, 166), 1u, empty);
            if (!empty) {
                parts_list_copy_summary(0x801D3098u + (uint32)selected * 108u);
                parts_list_detail_draw(ordering, parts_list_add(x, 176), parts_list_add(y, 80),
                    intensity, 0x801D2FE8u, intensity, parts_list_add(x, 140),
                    parts_list_add(y, 138), parts_list_add(x, 140), parts_list_add(y, 166), 0u, 1u);
            }
        }
    }
    color = (uint8)shade;
    color |= (color << 8) | (color << 16) | 0x03000000u;
    descriptor = 0x80092F70u + (uint32)r_s8(0x800B5305u) * 16u;
    xy = ((uint32)x + 180u) & 65535u;
    xy |= ((uint32)y + 80u) << 16;
    texture = r_u32(descriptor + 12u);
    size = r_u32(descriptor + 8u);
    parts_list_sprite(ordering, xy, texture, size, color);
    page = (uint16)(sint8)(uint8)(r_u8(descriptor + 1u) | 32u);
    (void)gtmain_packet_page(ordering, page | 512u);
    {
        sint32 selected = r_s8(0x80093531u);
        uint32 names = r_u32(0x800B530Cu);
        text = r_u32(names + (uint32)selected * 4u);
    }
    parts_list_caption(ordering, text, x, y, intensity);
    return 0u;
}
