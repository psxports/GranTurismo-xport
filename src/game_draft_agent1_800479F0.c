#include "game_first2_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 800479F0 */
sint32 gtmain_menu_roster_item_select(uint32 input)
{
    uint16 first_buttons = r_u16(input + 4u);
    uint16 repeat_buttons = r_u16(input + 8u);
    sint32 previous = r_s8(0x80092E9Du), selected = -1, result = -1;
    uint32 buttons = first_buttons | repeat_buttons, table;
    sint32 kind;
    if (buttons & 0x100u) {
        gtmain_menu_sound(0);
        gtmain_menu_list_select(0x80092E9Cu, previous - 10);
    } else if (buttons & 0x200u) {
        gtmain_menu_sound(0);
        gtmain_menu_list_select(0x80092E9Cu, previous + 10);
    } else selected = gtmain_menu_selector_input(0x80092E9Cu, input);
    if (selected == -2) return -2;
    if (selected == -1) return result;
    table = r_u32(0x800B5388u);
    kind = r_s8(table + (uint32)selected);
    if (!kind) {
        table = r_u32(0x800B5384u);
        result = (sint32)r_u32(table + ((uint32)selected << 3) + 4u);
    } else if (kind == 2) result = 120;
    else gtmain_menu_sound(6);
    printf((const char *)psx_addr(0x8008DB20u, 1u), result);
    return result;
}
