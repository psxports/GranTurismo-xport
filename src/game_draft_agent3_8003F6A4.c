#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static sint32 record_add(sint32 a, sint32 b)
{
    return (sint32)((uint32)a + (uint32)b);
}
static void record_one_line(uint32 ordering, sint32 x, sint32 y, uint32 text, sint32 intensity)
{
    /* TODO Bind unrecorded original single-line caption */
    (void)gt_draft_unresolved_call(0x8003D968u, ordering, x, y, text, intensity, 0, 0, 0);
}
static sint32 record_conditional_width(uint32 text)
{
    /* TODO Bind unrecorded original conditional width */
    gt_draft_failure("Record screen conditional width", 0x80035F7Cu);
}
static void record_conditional_draw(uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    /* TODO Bind unrecorded original conditional text draw */
    (void)gt_draft_unresolved_call(0x80035E40u, ordering, x, y, text, 0, 0, 0, 0);
}
static sint32 record_native_width(sint32 font, const char *text, sint32 spacing)
{
    /* TODO Bind native proportional string width with original spacing */
    gt_draft_failure("Record screen native string width", 0x8003A8F4u);
}
static void record_native_draw(uint32 ordering, sint32 x, sint32 y, const char *text, sint32 spacing)
{
    /* TODO Bind native string drawing with original spacing */
    gt_draft_failure("Record screen native string draw", 0x8003A8ACu);
}
static void record_background(uint32 ordering, sint32 x, sint32 y, sint32 width, sint32 height,
    sint32 intensity, uint32 first, uint32 second, uint32 third, uint32 red, uint32 green, uint32 blue)
{
    /* TODO Bind original twelve-argument background draw */
    gt_draft_failure("Record screen background 320x240", 0x8003B784u);
}

static void record_owner_panel(uint32 owner, uint32 ordering, sint32 x, sint32 y, uint32 carried_argument, sint32 intensity)
{
    /* TODO Resolve independent selected panel continuation */
    (void)gt_draft_80048134(owner, ordering, x, y, carried_argument, intensity);
}

static void record_panel(uint32 address, uint32 ordering, sint32 x, sint32 y, uint32 carried_argument, sint32 intensity)
{
    /* TODO Bind unselected five-argument panels 80047B24, 800499C0 and 8004A1C0 */
    (void)gt_draft_unresolved_call(address, ordering, (uint32)x, (uint32)y,
        carried_argument, (uint32)intensity, 0u, 0u, 0u);
}

static void record_replay_panel(uint32 ordering, sint32 x, sint32 y, uint32 carried_argument, sint32 intensity, sint32 replay, uint32 publish)
{
    /* TODO Bind unselected seven-argument panel 8004965C */
    (void)gt_draft_unresolved_call(0x8004965Cu, ordering, (uint32)x, (uint32)y,
        carried_argument, (uint32)intensity, (uint32)replay, publish, 0u);
}

static void record_callback(uint32 callback, uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    /* TODO Bind unselected five-argument callback adapter 8003D8F4 */
    (void)gt_draft_unresolved_call(0x8003D8F4u, callback, ordering,
        (uint32)x, (uint32)y, (uint32)intensity, 0u, 0u, 0u);
}

/* GTMAIN.EXE 8003F6A4 */
/* Original: GTMAIN.EXE:sub_8003F6A4. */ uint32 gtmain_menu_record_screen_draw(uint32 ordering, sint32 x, sint32 y,
    uint32 carried_argument, sint32 intensity)
{
    sint32 mode = (sint8)r_u8(0x801F2480u), kind, replay, width, left;
    sint32 header_y = 74, header_x = 160, selection = -1, publish = 0;
    uint32 first = 0, second = 0, descriptor = 0, pulse = 0, show_header = 1;
    uint32 text, red = 63, green = 12, blue = 0;
    sint16 corners[4];
    char saved_text[256];
    switch (mode) {
    case 0:
        first = 0x800BC3D6u; descriptor = 0x8009228Cu; publish = 1; show_header = 0;
        break;
    case 1:
        kind = (sint8)r_u8(0x801F2481u);
        if (kind > 0) w_u8(0x801F2481u, (uint32)kind - 1u);
        else first = 0x800BC43Bu;
        break;
    case 2: first = 0x800BC451u; descriptor = 0x800922A4u; publish = 1; break;
    case 3: first = 0x800BC473u; descriptor = 0x800922A4u; break;
    case 4: first = 0x800BC510u; descriptor = 0x800922A4u; publish = 1; break;
    case 5: first = 0x800BC490u; second = 0x800BC4B5u; descriptor = 0x800922ECu; break;
    case 6: first = 0x800BC490u; descriptor = 0x800922A4u; publish = 1; break;
    case 7: case 8: first = 0x800BC4D1u; second = 0x800BC7A8u; break;
    case 9: first = 0x800BC4E8u; descriptor = 0x800922A4u; break;
    case 10: first = 0x800BC578u; second = 0x800BC597u; descriptor = 0x800922BCu; break;
    case 11: case 12: case 29: case 30: case 33: case 34:
        first = 0x800BC5AFu; second = 0x800BC7A8u; break;
    case 13: case 31: first = 0x800BC5CAu; descriptor = 0x800922A4u; publish = 1; break;
    case 14: case 32: case 47:
        first = 0x800BC537u; second = 0x800BC55Bu; descriptor = 0x800922BCu; break;
    case 15: first = 0x800BC625u; descriptor = 0x800922BCu; break;
    case 16: case 17: case 18:
        kind = (sint8)r_u8(0x801F1FE4u);
        first = kind ? 0x800BC788u : 0x800BC73Au; second = 0x800BC7A8u; break;
    case 19: first = 0x800BC751u; descriptor = 0x800922A4u; publish = 1; break;
    case 20: first = 0x800BC69Eu; second = 0x800BC721u; descriptor = 0x800922BCu; break;
    case 21:
        kind = (sint8)r_u8(0x801F1FE4u);
        first = kind == 1 ? 0x800BC6BEu : kind == 2 ? 0x800BC6E2u : 0x800BC705u;
        second = 0x800BC721u; descriptor = 0x800922BCu; break;
    case 22:
        kind = (sint8)r_u8(0x801F1FE4u);
        first = kind == 1 ? 0x800BC643u : kind == 2 ? 0x800BC661u : 0x800BC683u;
        descriptor = 0x800922A4u; publish = 1; break;
    case 23: case 24: case 42: case 43: first = 0x800BC7D1u; second = 0x800BC7A8u; break;
    case 25:
        kind = (sint8)r_u8(0x801F1FE4u);
        first = kind == 1 ? 0x800BC7E8u : kind == 2 ? 0x800BC803u : 0x800BC826u;
        second = 0x800BC842u; descriptor = 0x800922BCu; break;
    case 26: first = 0x800BC872u; second = 0x800BC8B0u; descriptor = 0x800922BCu; break;
    case 27:
        header_y = -32;
        record_owner_panel(r_u32(0x801F1FDCu), ordering, x, y, carried_argument, intensity);
        break;
    case 28:
        header_y = -32;
        (void)gtmain_menu_name_keyboard_panel_draw(ordering, x, y, intensity);
        break;
    case 35:
        header_y = 28; header_x = 256;
        record_panel(0x80047B24u, ordering, x, y, carried_argument, intensity);
        break;
    case 36: first = 0x800BC77Cu; descriptor = 0x800922A4u; publish = 1; break;
    case 37:
        record_panel(0x800499C0u, ordering + 4u, x, y, carried_argument, intensity);
        break;
    case 38:
        header_y = -32;
        replay = (sint8)r_u8(0x801F2034u);
        record_replay_panel(ordering + 4u, x, y, carried_argument, intensity, replay, 1u);
        break;
    case 39:
        header_y = -32;
        record_panel(0x8004A1C0u, ordering + 4u, x, y, carried_argument, intensity);
        break;
    case 40: case 41:
        replay = (sint8)r_u8(0x801F2034u);
        first = replay == 1 ? 0x800BC90Du : replay == 2 ? 0x800BC925u : 0x800BC5AFu;
        second = 0x800BC7A8u;
        (void)gtmain_menu_two_line_caption_draw(ordering, x, y, first, second, intensity);
        first = 0; second = 0;
        if (replay != 1 && replay != 2 && (sint16)r_u16(0x801F2030u) >= 0) {
            (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
            (void)gtmain_text_font(1); gtmain_text_flags(0);
            kind = (sint16)r_u16(0x801F2030u);
            (void)sprintf_s(saved_text, sizeof(saved_text), "%d sector saved.\n", kind + 1);
            width = record_native_width(1, saved_text, 0);
            record_native_draw(ordering, record_add(x, 160 - (width >> 1)), record_add(y, 194), saved_text, 0);
        }
        break;
    case 44: first = 0x800BC895u; second = 0x800BC8B0u; descriptor = 0x800922BCu; break;
    case 45: case 46: first = 0x800BC8E9u; second = 0x800BC7A8u; break;
    case 48:
        replay = (sint8)r_u8(0x801F2034u);
        first = replay == 1 ? 0x800BC5FBu : replay == 2 ? 0x800BC5E2u : 0x800BC5CAu;
        descriptor = 0x800922A4u; publish = 1; break;
    case 49: case 50: first = 0x800BC73Au; second = 0x800BC7A8u; break;
    default: break;
    }
    if (first) {
        if (second) (void)gtmain_menu_two_line_caption_draw(ordering, x, y, first, second, intensity);
        else record_one_line(ordering, x, y, first, intensity);
    }
    if (descriptor) {
        (void)gtmain_menu_choice_transition_draw(ordering, descriptor, record_add(x, 160), record_add(y, 190), intensity, publish, 0, 0);
        selection = (sint8)r_u8(descriptor + 12u);
        pulse = (uint32)gtmain_menu_pulse_component(0, intensity) & 255u;
        if (publish) pulse <<= 16;
    }
    if (show_header) {
        text = (sint8)r_u8(0x801F2482u) ? 0x800BC3C1u : 0x800BC3ACu;
        (void)gtmain_text_font(2); gtmain_text_flags(2);
        (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
        width = record_conditional_width(text);
        record_conditional_draw(ordering, record_add(record_add(x, header_x), -(width >> 1)), record_add(y, header_y), text);
    }
    if (selection >= 0) {
        (void)gtmain_text_font(2); gtmain_text_flags(2);
        (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
        text = r_u32(r_u32(0x801F2484u) + (uint32)selection * 4u);
        width = record_conditional_width(text);
        left = record_add(x, 160 - (width >> 1));
        text = r_u32(r_u32(0x801F2484u) + (uint32)selection * 4u);
        record_conditional_draw(ordering, left, record_add(y, 171), text);
        corners[0] = (sint16)left; corners[1] = (sint16)record_add(y, 175);
        corners[2] = (sint16)((uint32)left + (uint32)width - 1u); corners[3] = (sint16)record_add(y, 187);
        (void)gtmain_packet_rectangle_native(ordering, corners, pulse | 0x02000000u);
        (void)gtmain_packet_page(ordering, 512);
    }
    record_callback(r_u32(0x801F2488u), ordering + 16u, x, y, intensity);
    if ((sint8)r_u8(0x801F1FE5u) == 1) { red = 0; green = 36; blue = 95; }
    record_background(ordering + 16000u, x, y, 320, 240, intensity, 0u, 0u, 0u, red, green, blue);
    return 0u;
}
