#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 800440F4 */
/* Original: GTMAIN.EXE:sub_800440F4. */ uint32 gt_draft_800440F4(void)
{
    uint32 input = r_u32(0x800BDD08u);
    sint32 choice, mode;
    gtmain_menu_stream_check();
    if (!(sint8)r_u8(input)) return 0u;
    (void)gtmain_menu_flags(1u);
    mode = (sint8)r_u8(0x800B5304u);
    switch (mode) {
    case 1:
        choice = (sint32)gt_draft_80046AF4(0x80092C78u, input + 2u, 0u, 0u, 0u, 0u, 0u, 0u);
        if (choice == -2) goto back;
        if (choice == -1) return 0u;
        if (choice) goto dismiss;
        (void)gtmain_menu_sound(5);
        (void)gtmain_menu_transition_enter(0x80092C68u);
        w_u8(0x800B5304u, 2u);
        return 0u;
    case 2:
    case 3:
        choice = (sint32)gt_draft_80046AF4(0x80092C68u, input + 2u, 0u, 0u, 0u, 0u, 0u, 0u);
        if (choice == -2) goto back;
        if (choice == -1) return 0u;
        (void)gtmain_menu_sound(4);
        (void)gt_draft_8001EE00((uint32)choice);
        mode = (sint8)r_u8(0x800B5304u);
        if (mode == 2) {
            w_u8(0x800BEA24u, 1u);
            (void)gtmain_menu_push(0x80092170u);
            return 3u;
        }
        w_u8(0x800BEA24u, 0u);
        return 1u;
    case 4:
        choice = (sint32)gt_draft_80046AF4(0x80092C88u, input + 2u, 0u, 0u, 0u, 0u, 0u, 0u);
        if (choice == -2) goto back;
        if (choice == -1) return 0u;
        if (choice) goto dismiss;
        w_u8(0x800BEA24u, 255u);
        (void)gtmain_menu_sound(4);
        (void)gtmain_menu_push(0x80092170u);
        return 3u;
    default:
        choice = gtmain_menu_selector_input(0x80092BDCu, input + 2u);
        switch (choice) {
        case -2:
        case 4:
            w_u8(0x800B5304u, 4u);
            (void)gtmain_menu_sound(5);
            (void)gtmain_menu_transition_exit(0x80092C88u);
            return 0u;
        case 0:
            w_u8(0x800B5304u, 1u);
            (void)gtmain_menu_sound(5);
            (void)gtmain_menu_transition_exit(0x80092C78u);
            return 0u;
        case 1:
            (void)gtmain_menu_sound(5);
            w_u8(0x800B5304u, 3u);
            (void)gtmain_menu_transition_enter(0x80092C68u);
            return 0u;
        case 2:
            (void)gtmain_menu_sound(4);
            (void)gtmain_menu_push(0x80092C98u);
            return 3u;
        case 3:
            (void)gtmain_menu_sound(4);
            (void)gtmain_menu_push(0x80092D40u);
            return 3u;
        default:
            return 0u;
        }
    }

back:
    (void)gtmain_menu_sound(3);
    w_u8(0x800B5304u, 0u);
    return 0u;

dismiss:
    w_u8(0x800B5304u, 0u);
    (void)gtmain_menu_sound(5);
    return 0u;
}
