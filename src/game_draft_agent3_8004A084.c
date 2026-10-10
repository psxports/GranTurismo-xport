#include "game_first2_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 8004A084 */
sint32 gtmain_menu_saved_slot_select(uint32 input)
{
    sint32 selected, result = -1;
    uint32 table;
    uint8 kind;
    w_u8(0x800B53B5u, 0);
    w_u8(0x800B53B6u, 0);
    w_u8(0x800B53B8u, 0);
    w_u8(0x800B53B7u, 0);
    selected = gtmain_menu_selector_input(0x80092EBCu, input);
    if (selected == -2) return -2;
    if (selected == -1) return result;
    table = r_u32(0x800B53ACu);
    kind = r_u8(table + ((uint32)selected << 3) + 1u);
    if (!kind) result = selected;
    else if (kind == 4) result = 64;
    else gtmain_menu_sound(6);
    printf((const char *)psx_addr(0x8008DCC0u, 1u), result);
    return result;
}
