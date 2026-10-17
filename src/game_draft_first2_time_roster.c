#include "game_first2_registry.h"

static void time_roster_fixed_text(uint32 ordering, sint32 x, sint32 y, const char text[64])
{
    /* TODO Bind native string to fixed-advance text service */
    gt_draft_failure("GTMAIN time roster fixed text", 0x8006C720u);
}

static void time_roster_time_format(char output[64], uint32 milliseconds)
{
    /* TODO Bind native destination to selected time formatter */
    gt_draft_failure("GTMAIN time roster time format", 0x8006CA50u);
}

static void time_roster_format(char output[64], uint32 format, const sint32 *arguments, uint32 count)
{
    /* TODO Bind native destination and actual format argument words */
    gt_draft_failure("GTMAIN time roster formatting", 0x8007C014u);
}

static sint32 time_roster_width(sint32 font, const char text[64])
{
    /* TODO Bind native string to font2 width service */
    gt_draft_failure("GTMAIN time roster native width", 0x8006C900u);
}

/* GTMAIN.EXE:800214C8 */
void gtmain_race_time_roster_draw(uint32 ordering, sint32 y, sint32 count)
{
    uint32 node, leader_time, row = 0u;
    char text[64];
    gtmain_text_font(2);
    gtmain_text_advance(10);
    node = r_u32(0x8008F3E4u);
    leader_time = r_u32(node + 32u);
    while (node) {
        uint32 color = 0x00808080u, name;
        if ((sint32)row >= count) return;
        if ((sint16)r_u16(node + 26u) == 1) color = 0x00808000u;
        gtmain_text_color(color);
        text[0] = (char)(uint8)(row + 49u);
        text[1] = 0;
        time_roster_fixed_text(ordering, 16, y, text);
        name = gtmain_resource_name(r_u32(node + 2332u));
        gtmain_font_text_draw(ordering, 32, y, name);
        if (!row) {
            time_roster_time_format(text, leader_time);
            time_roster_fixed_text(ordering, 204, y, text);
        } else if (r_u32(node + 32u) == 0xFFFFFFFFu) {
            sint32 width;
            time_roster_format(text, 0x800BD9FFu, NULL, 0u);
            width = time_roster_width(2, text);
            gtmain_font_string_draw_native(0x800C0070u, ordering,
                (sint32)(304u - (uint32)width), y, text);
        } else {
            sint32 difference = (sint32)(r_u32(node + 32u) - leader_time);
            sint32 arguments[2];
            if (difference > 99999999) difference = 99999999;
            arguments[0] = difference / 1000;
            arguments[1] = difference % 1000;
            time_roster_format(text, 0x8008B9D4u, arguments, 2u);
            time_roster_fixed_text(ordering, 204, y, text);
        }
        y = (sint32)((uint32)y + 18u);
        node = r_u32(node + 4u);
        ++row;
    }
}
