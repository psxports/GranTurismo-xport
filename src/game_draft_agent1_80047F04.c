#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static void saved_entry_log(uint32 format, sint32 value)
{
    printf((const char *)psx_addr(format, 1u), (int)value);
}

static void saved_entry_refresh(uint32 owner)
{
    /* TODO Bind unselected saved entry list refresh 80047CA0 */
    (void)gt_draft_unresolved_call(0x80047CA0u, owner, 0, 0, 0, 0, 0, 0, 0);
}

/* GTMAIN.EXE 80047F04 */
/* Original: GTMAIN.EXE:sub_80047F04. */ sint32 gtmain_menu_saved_entry_input(uint32 owner, uint32 input)
{
    sint32 countdown = (sint8)r_u8(0x800B538Eu);
    sint32 selected, current, kind;
    uint16 first_buttons, second_buttons;
    uint32 buttons;
    if (countdown > 0)
    {
        w_u8(0x800B538Eu, (uint8)(countdown - 1));
        if (!(uint8)(countdown - 1))
        {
            selected = (sint8)r_u8(0x80092E9Du);
            saved_entry_log(0x8008DBC0u, selected);
            selected = (sint8)r_u8(0x80092E9Du);
            gtmain_menu_saved_entry_delete(owner, selected);
            saved_entry_refresh(owner);
            w_u8(0x800B538Du, 1);
        }
        return -3;
    }
    w_u8(0x800B538Du, 0);
    w_u8(0x800B538Eu, 0);
    if ((r_u16(input + 2u) & 0x0C00u) == 0x0C00u) w_u8(0x800B538Du, 1);
    first_buttons = r_u16(input + 4u);
    second_buttons = r_u16(input + 8u);
    current = (sint8)r_u8(0x80092E9Du);
    buttons = first_buttons | second_buttons;
    selected = -1;
    if (buttons & 0x0100u)
    {
        gtmain_menu_sound(0);
        gtmain_menu_list_select(0x80092E9Cu, current - 10);
    }
    else if (buttons & 0x0200u)
    {
        gtmain_menu_sound(0);
        gtmain_menu_list_select(0x80092E9Cu, current + 10);
    }
    else selected = gtmain_menu_selector_input(0x80092E9Cu, input);
    if ((sint8)r_u8(0x800B538Du))
    {
        if (selected == -2) return -2;
        if (selected == -1) return -3;
        kind = (sint8)r_u8(r_u32(0x800B5388u) + (uint32)selected);
        if (kind == 2) return 120;
        if (kind == 3) gtmain_menu_sound(6);
        else
        {
            w_u8(0x800B538Eu, 16);
            gtmain_menu_sound(1);
        }
        return -3;
    }
    if (selected == -2) return -2;
    if (selected == -1) return -1;
    kind = (sint8)r_u8(r_u32(0x800B5388u) + (uint32)selected);
    if (kind == 2) selected = 120;
    else selected = (sint32)r_u32(r_u32(0x800B5384u) + 8u * (uint32)selected + 4u);
    saved_entry_log(0x8008DB20u, selected);
    return selected;
}
