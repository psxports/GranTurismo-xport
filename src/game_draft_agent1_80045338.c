#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void resource_prepare(void)
{
    /* TODO Bind unselected resource preparation 8004AF94 */
    (void)gt_draft_unresolved_call(0x8004AF94u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static sint32 machine_select(uint32 selector, uint32 input)
{
    /* TODO Bind unselected machine selector 8004B25C */
    return (sint32)gt_draft_unresolved_call(0x8004B25Cu, selector, input, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void item_prepare(uint32 item)
{
    /* TODO Bind unselected item preparation 8004B6A0 */
    (void)gt_draft_unresolved_call(0x8004B6A0u, item, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static sint32 item_select(uint32 item, uint32 input)
{
    /* TODO Bind unselected item selector 8004B6A8 */
    return (sint32)gt_draft_unresolved_call(0x8004B6A8u, item, input, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void menu_cancel(uint32 descriptor)
{
    /* TODO Bind unselected menu transition 80035364 */
    (void)gt_draft_unresolved_call(0x80035364u, descriptor, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void resource_copy(uint32 source, uint32 destination, uint32 count, uint32 width)
{
    /* TODO Bind unselected resource copy 8004AFCC */
    (void)gt_draft_unresolved_call(0x8004AFCCu, source, destination, count, width, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80045338 */
uint32 gt_draft_80045338(void)
{
    static const uint32 tables[4][6] = {
        { 0x80093540u, 0x800934B8u, 0x800934F4u, 0x8009357Cu, 0x800934A4u, 0x80093590u },
        { 0x80093558u, 0x800934D0u, 0x8009350Cu, 0x80093584u, 0x800934ACu, 0x80093598u },
        { 0x80093564u, 0x800934DCu, 0x80093518u, 0x80093588u, 0x800934B0u, 0x8009359Cu },
        { 0x80093574u, 0x800934ECu, 0x80093528u, 0x8009358Cu, 0x800934B4u, 0x800935A0u }
    };
    uint32 input = r_u32(0x800BDD08u), item, buttons;
    sint32 selection, category;
    uint32 index, source, kind, count, width, table;
    resource_prepare();
    gtmain_menu_stream_check();
    if (!(sint8)r_u8(input)) return 0u;
    if (!(sint8)r_u8(0x800B5304u)) {
        (void)gtmain_menu_flags(0u);
        selection = machine_select(0x8009303Cu, input + 2u);
        if (selection == -2) {
            (void)gtmain_menu_sound(2);
            return 1u;
        }
        if (selection == -1) {
            buttons = r_u16(input + 6u);
            if (buttons & 0x400u) goto cancel;
            if (!(buttons & 0x20u)) return 0u;
        }
        (void)gtmain_menu_sound(5);
        category = (sint8)r_u8(0x80093049u);
        item = gt_draft_80045320((uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        item_prepare(item);
        w_u8(0x800B5304u, 1u);
        return 0u;
    }
    (void)gtmain_menu_flags(1u);
    category = (sint8)r_u8(0x80093049u);
    item = gt_draft_80045320((uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    selection = item_select(item, input + 2u);
    if (selection == -2) goto back;
    if (selection == -1) {
        buttons = r_u16(input + 6u);
        if (buttons & 0x400u) goto cancel;
        if (!(buttons & 0x80u)) return 0u;
        goto back;
    }
    category = (sint8)r_u8(0x80093049u);
    table = category == 0 ? 0u : category == 1 ? 1u : category == 2 ? 2u : 3u;
    index = (uint32)selection;
    w_u32(0x80092D28u, r_u32(tables[table][0] + 4u * index));
    source = r_u32(tables[table][1] + 4u * index);
    w_u32(0x800B530Cu, r_u32(tables[table][2] + 4u * index));
    kind = r_u8(tables[table][3] + index);
    count = r_u8(tables[table][4] + index);
    w_u8(0x800B5305u, kind);
    width = r_u8(tables[table][5] + index);
    resource_copy(source, 0x800B5314u, count, width);
    w_u8(0x800B5374u, width);
    w_u32(0x80093534u, source);
    w_u8(0x80093530u, count);
    w_u8(0x80093532u, count);
    (void)gtmain_menu_sound(4);
    (void)gtmain_menu_push(0x80092D24u);
    return 3u;

cancel:
    (void)gtmain_menu_sound(1);
    menu_cancel(0x80092CB4u);
    w_u8(0x800B5304u, 0u);
    return 5u;

back:
    (void)gtmain_menu_sound(3);
    w_u8(0x800B5304u, 0u);
    return 0u;
}
