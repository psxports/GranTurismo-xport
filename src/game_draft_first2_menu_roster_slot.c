#include "game_first2_registry.h"

static uint32 roster_slot_table(void)
{
    /* TODO Unselected 8002A3AC remains a typed fail-fast dependency */
    gt_draft_failure("GTMAIN roster slot table", 0x8002A3ACu);
}

static uint32 roster_slot_outline(uint32 ordering, const sint16 *corners, uint32 color)
{
    return gtmain_packet_outline_native(ordering, corners, color);
}

static sint32 roster_slot_width(sint32 font, const char *text)
{
    /* TODO Bind native string storage to the original proportional width service */
    gt_draft_failure("Native roster slot width adapter", 0x8006C900u);
}

/* GTMAIN.EXE:80038F08 */
sint32 gtmain_menu_roster_slot_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 slot)
{
    char digit[8];
    uint32 table, descriptor, shade;
    sint16 corners[4];
    sint32 width;
    digit[0] = 0;
    digit[1] = 0;
    table = roster_slot_table();
    descriptor = 0x8009200Cu + ((uint32)r_u8(table + slot) << 4);
    (void)gtmain_menu_sprite(descriptor, ordering, (uint16)((uint32)x - 8u),
        (sint32)((uint32)y + 10u), (uint8)intensity);
    corners[0] = (sint16)((uint32)x - 9u);
    corners[1] = (sint16)((uint32)y + 9u);
    corners[2] = (sint16)((uint32)x + 8u);
    corners[3] = (sint16)((uint32)y + 26u);
    shade = (uint8)(intensity >> 2);
    (void)roster_slot_outline(ordering, corners, shade | (shade << 8) | (shade << 16));
    (void)gtmain_text_color(gtmain_menu_gray((uint8)(intensity >> 1)));
    (void)gtmain_text_font(1);
    gtmain_text_flags(0);
    digit[0] = (char)(uint8)(slot + 49u);
    width = roster_slot_width(2, digit);
    return gtmain_font_string_draw_native(0x800C0070u, ordering,
        (sint32)((uint32)x - (uint32)(width >> 1)), y, digit);
}
