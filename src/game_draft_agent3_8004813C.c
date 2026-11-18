#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 owner_panel_add(sint32 a, sint32 b)
{
    return (sint32)((uint32)a + (uint32)b);
}
static void owner_panel_format(char output[128], uint32 format, sint32 free_records, sint32 total)
{
    /* TODO Bind the native label destination to the original two-value formatter */
    gt_draft_failure("Owner panel native label formatter", 0x8007C014u);
}
static sint32 owner_panel_width(sint32 font, const char output[128])
{
    /* TODO Bind native label provenance to the width service */
    gt_draft_failure("Owner panel native label width", 0x8006C900u);
}
static void owner_panel_label(uint32 ordering, sint32 x, sint32 y, const char output[128])
{
    /* TODO Bind native label provenance to the drawing service */
    gt_draft_failure("Owner panel native label draw", 0x8006CA10u);
}
static void owner_panel_caption(uint32 ordering, sint32 x, sint32 y, sint32 name)
{
    /* TODO Bind unselected four-argument caption 800472C8 */
    (void)gt_draft_unresolved_call(0x800472C8u, ordering, (uint32)x, (uint32)y,
        (uint32)name, 0u, 0u, 0u, 0u);
}
static void owner_panel_row(uint32 ordering, sint32 x, sint32 y, uint32 brightness,
    uint32 resource, sint32 name, uint32 selected, uint32 publish)
{
    /* TODO Bind unselected eight-argument row 800473D4 */
    (void)gt_draft_unresolved_call(0x800473D4u, ordering, (uint32)x, (uint32)y,
        brightness, resource, (uint32)name, selected, publish);
}
static uint32 owner_panel_byte(const uint8 descriptor[152], uint32 offset)
{
    if (offset >= 152u) gt_draft_failure("Owner panel descriptor byte extent", 0x8004813Cu);
    return descriptor[offset];
}
static sint32 owner_panel_half(const uint8 descriptor[152], uint32 offset)
{
    uint32 low = owner_panel_byte(descriptor, offset);
    uint32 high = owner_panel_byte(descriptor, offset + 1u);
    return (sint16)(uint16)(low | (high << 8));
}

/* TODO Resolve independent native label, descriptor and unselected row services */
/* GTMAIN.EXE 8004813C */
/* Original: GTMAIN.EXE:sub_8004813C. */ void gt_draft_8004813C(uint32 owner, uint32 ordering, sint32 x, sint32 y,
    uint32 carried_argument, sint32 intensity, uint32 context)
{
    char label[128];
    uint8 descriptor[152];
    uint32 caption = 0x800BD65Cu, occupied = 0u, i, cursor = owner + 512u;
    sint32 width;
    uint32 packet = r_u32(context + 152u);
    if ((r_u8(packet + 1u) & 0xF0u) == 0x20u) caption += 31u;
    for (i = 0u; i < 120u; ++i, cursor += 116u)
        if (r_s16(cursor + 114u)) ++occupied;
    owner_panel_format(label, 0x800BCF7Fu, (sint32)(120u - occupied), 120);
    gtmain_text_flags(0);
    (void)gtmain_text_color(gtmain_menu_palette_color(6, intensity >> 1));
    (void)gtmain_text_font(1);
    width = owner_panel_width(1, label);
    owner_panel_label(ordering, owner_panel_add(x, (sint32)(304u - (uint32)width)),
        owner_panel_add(y, 180), label);
    if (r_s8(0x800B538Du) != 1) {
        uint8 component = (uint8)gtmain_menu_pulse_component(0, intensity >> 1);
        (void)gtmain_text_color(gtmain_menu_gray(component));
        (void)gtmain_menu_text_draw(ordering, owner_panel_add(x, 24),
            owner_panel_add(y, 41), caption, 0);
    }
    (void)gtmain_menu_descriptor_draw(ordering, 0x80092E9Cu, owner_panel_add(x, 160),
        owner_panel_add(y, 59), intensity, 1u, descriptor, sizeof(descriptor));
    gtmain_text_flags(2);
    for (i = 0u; ; ++i) {
        sint32 key = (sint8)owner_panel_byte(descriptor, 2u + i);
        uint32 selected = 0u, target, brightness, resource;
        sint32 state, row_x, row_y, name;
        if (key < 0) return;
        if (key == r_s8(0x80092E9Du)) {
            sint32 reloaded_key;
            uint32 names;
            gtmain_text_flags(2);
            (void)gtmain_text_font(2);
            selected = 1u;
            (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
            reloaded_key = (sint8)owner_panel_byte(descriptor, 2u + i);
            names = r_u32(0x800B5388u);
            name = r_s8(names + (uint32)reloaded_key);
            owner_panel_caption(ordering, owner_panel_add(x, 160), owner_panel_add(y, 192), name);
        }
        target = r_u16(0x800B537Cu);
        state = r_s8(0x800B538Cu);
        if ((uint32)key == target && state == 1) {
            sint32 muted = r_s8(0x800B538Du);
            brightness = owner_panel_byte(descriptor, 26u + i);
            if (muted) brightness >>= 1;
            row_x = owner_panel_half(descriptor, 50u + 2u * i);
            row_y = owner_panel_half(descriptor, 98u + 2u * i);
            resource = 0xFFFFFFFFu;
            name = 1;
        } else {
            sint32 reloaded_key = (sint8)owner_panel_byte(descriptor, 2u + i);
            uint32 resources = r_u32(0x800B5384u), names;
            row_x = owner_panel_half(descriptor, 50u + 2u * i);
            row_y = owner_panel_half(descriptor, 98u + 2u * i);
            brightness = owner_panel_byte(descriptor, 26u + i);
            resource = r_u32(resources + (uint32)reloaded_key * 8u + 4u);
            reloaded_key = (sint8)owner_panel_byte(descriptor, 2u + i);
            names = r_u32(0x800B5388u);
            name = r_s8(names + (uint32)reloaded_key);
        }
        owner_panel_row(ordering, row_x, row_y, brightness, resource, name, selected, 1u);
    }
}
