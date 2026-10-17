#include "game_first2_registry.h"

static uint32 roster_panel_table(void)
{
    /* TODO Unselected table provider remains typed fail-fast */
    gt_draft_failure("GTMAIN roster panel table", 0x8002A4DCu);
}

static sint32 roster_panel_header_width(uint32 text)
{
    /* TODO Unselected header width remains typed fail-fast */
    gt_draft_failure("GTMAIN roster panel header width", 0x80035F7Cu);
}

static uint32 roster_panel_header_draw(uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    /* TODO Unselected conditional header draw remains typed fail-fast */
    gt_draft_failure("GTMAIN roster panel header draw", 0x80035E40u);
}

static uint32 roster_panel_left_arrow(uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    /* TODO Unselected left arrow remains typed fail-fast */
    gt_draft_failure("GTMAIN roster panel left arrow", 0x8003B690u);
}

static uint32 roster_panel_right_arrow(uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    /* TODO Unselected right arrow remains typed fail-fast */
    gt_draft_failure("GTMAIN roster panel right arrow", 0x8003B648u);
}

static sint32 roster_panel_format(char output[256], uint32 format, sint32 number)
{
    /* TODO Bind native formatted destination using the original format */
    gt_draft_failure("Native roster panel formatter", 0x8007C014u);
}

static uint32 roster_panel_native_text(uint32 ordering, sint32 x, sint32 y, const char *text, sint32 spacing)
{
    /* TODO Bind native string storage to the original spaced text service */
    gt_draft_failure("Native roster panel spaced text", 0x8003A8ACu);
}

/* GTMAIN.EXE:80039030 */
uint32 gtmain_menu_roster_panel_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    char formatted[256];
    sint16 corners[4];
    uint32 table = roster_panel_table(), arrow_color, entry, shade, row = 0, offset = 0;
    uint32 unsigned_count, selected, highlight, width, active;
    sint32 count, selection, header_width, half = intensity >> 1;
    uint32 label_x = (uint32)x + 72u, label_y = (uint32)y + 132u;
    arrow_color = gtmain_menu_palette_color(0, intensity);
    (void)gtmain_menu_machine_caption_draw(ordering, x, y, intensity);
    (void)gtmain_text_font(2);
    gtmain_text_flags(0);
    (void)gtmain_text_color(gtmain_menu_palette_color(6, half));
    selected = r_u8(0x800CB7F9u);
    (void)roster_panel_format(formatted, 0x800BCCA2u, (sint32)selected + 1);
    entry = table + selected * 4u;
    header_width = roster_panel_header_width(r_u32(entry));
    (void)roster_panel_header_draw(ordering,
        (sint32)((uint32)x - ((uint32)(header_width >> 1) - 160u)),
        (sint32)((uint32)y + 40u), r_u32(entry));
    corners[0] = (sint16)x;
    corners[1] = (sint16)((uint32)y + 42u);
    corners[2] = (sint16)((uint32)x + 319u);
    corners[3] = (sint16)((uint32)y + 57u);
    shade = (uint8)(intensity >> 3);
    (void)gtmain_packet_rectangle_native(ordering, corners,
        shade | (shade << 8) | ((uint32)(uint8)(intensity >> 2) << 16));
    count = r_s8(0x801F2FE9u);
    unsigned_count = r_u8(0x801F2FE9u);
    if (count > 0) {
        do {
            (void)gtmain_menu_roster_slot_draw(ordering,
                (sint32)(offset - ((uint32)(sint32)(sint8)unsigned_count * 12u - 168u)),
                (sint32)((uint32)y + 62u), intensity, row);
            ++row;
            count = r_s8(0x801F2FE9u);
            unsigned_count = r_u8(0x801F2FE9u);
            offset += 24u;
        } while ((sint32)row < count);
    }
    (void)gtmain_text_font(2);
    highlight = r_s8(0x8009207Du) == 2;
    selection = r_s8(0x801F2FE7u);
    (void)gtmain_text_color(gtmain_menu_gray((uint8)half));
    width = gtmain_menu_text_width(2, 0x800BCC8Bu, 0);
    (void)gtmain_menu_text_draw(ordering, (sint32)label_x, (sint32)label_y, 0x800BCC8Bu, 0);
    active = r_s8(0x80091B41u) != 0;
    if (active && highlight) {
        uint32 arrow_x = (uint32)x + width + 72u;
        (void)roster_panel_left_arrow(ordering, (sint32)(arrow_x - 9u),
            (sint32)((uint32)y + 140u), arrow_color);
        (void)roster_panel_right_arrow(ordering, (sint32)(arrow_x + 19u),
            (sint32)((uint32)y + 140u), arrow_color);
    }
    (void)roster_panel_format(formatted, 0x800BCC9Fu, selection + 1);
    (void)gtmain_text_color(gtmain_menu_gray((uint8)half));
    if (highlight) {
        sint32 pulse = gtmain_menu_pulse_component(0, half);
        (void)gtmain_text_color(gtmain_menu_gray((uint8)pulse));
    }
    (void)roster_panel_native_text(ordering, (sint32)((uint32)x + width + 72u),
        (sint32)label_y, formatted, 0);
    (void)gtmain_text_font(1);
    (void)gtmain_text_color(gtmain_menu_palette_color(6, half));
    entry = table + (uint32)selection * 4u;
    (void)roster_panel_header_width(r_u32(entry));
    (void)roster_panel_header_draw(ordering, (sint32)((uint32)x + 80u),
        (sint32)((uint32)y + 149u), r_u32(entry));
    arrow_color = gtmain_menu_palette_color(4, intensity);
    corners[0] = (sint16)label_x;
    corners[1] = (sint16)((uint32)y + 162u);
    corners[2] = (sint16)((uint32)x + 391u);
    corners[3] = corners[1];
    (void)gtmain_packet_rectangle_native(ordering + 4u, corners, arrow_color);
    corners[0] = (sint16)((uint32)x + 52u);
    corners[1] = (sint16)((uint32)y + 142u);
    corners[2] = (sint16)((uint32)x + 62u);
    corners[3] = (sint16)((uint32)y + 152u);
    (void)gtmain_packet_rectangle_native(ordering + 4u, corners, arrow_color);
    (void)gtmain_menu_list_extent(0x8009207Cu);
    (void)gtmain_menu_descriptor_draw(ordering, 0x8009207Cu, (sint32)label_x,
        (sint32)((uint32)y + 96u), intensity, 0, 0, 0);
    (void)gtmain_menu_backdrop_draw(ordering + 8000u, (uint32)x, (uint32)y,
        (uint32)half, (uint32)half, (uint32)half);
    return 0;
}
