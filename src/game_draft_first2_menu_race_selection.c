#include "game_first2_registry.h"

static uint32 selection_transition(uint32 check_token, uint32 complete_token)
{
    /* TODO Bind the two-argument transition without expanding unselected game code */
    gt_draft_failure("Native selection transition 408A4", 0x800408A4u);
}

static uint32 selection_ghost_transition(uint32 check_token, uint32 complete_token, uint32 exec_mode, uint32 course, uint32 run_mode)
{
    /* TODO Bind all five meaningful transition arguments */
    gt_draft_failure("Native selection ghost transition 40598", 0x80040598u);
}

/* GTMAIN.EXE:80036350 */
sint32 gtmain_menu_race_selection_tick(void)
{
    uint32 input = r_u32(0x800BDD08u), selector, descriptor;
    sint32 selected, mode;
    /* TODO Stream pause dependencies remain partial */
    gtmain_menu_stream_check();
    (void)gtmain_menu_angle_tick();
    if (!r_s8(input)) return 0;
    selector = r_u32(0x801F2FE0u);
    w_u8(0x801F2FE8u, 1);
    selected = gtmain_menu_selector_input(selector, input + 2u);
    if (selected == -2) {
        if (r_s8(0x801F2FEBu) == 6) {
            (void)gtmain_menu_sound(6);
            return 0;
        }
        (void)gtmain_menu_sound(2);
        w_u8(0x801F2FDDu, 255);
        descriptor = 0x80092170u;
    } else {
        if (selected == -1) return 0;
        switch (r_s8(r_u32(0x801F2FE4u) + (uint32)selected)) {
        case 0:
            (void)gtmain_menu_sound(4);
            w_u8(0x801F2FDDu, 255);
            descriptor = 0x80092170u;
            break;
        case 2:
            if (!r_s8(0x801F2FDCu)) {
                (void)gtmain_menu_sound(6);
                return 0;
            }
            (void)gtmain_menu_sound(4);
            w_u8(0x801F2FDDu, 0);
            descriptor = 0x80092170u;
            break;
        case 3:
            w_u8(0x801F2FDDu, 1);
            (void)gtmain_menu_sound(4);
            descriptor = 0x80092170u;
            break;
        case 4:
            (void)gtmain_menu_sound(5);
            w_u8(0x800CB764u, r_u8(0x800CB764u) ^ 1u);
            return 0;
        case 5:
            w_u8(0x801F2FE8u, 0);
            (void)gtmain_menu_sound(4);
            (void)selection_transition(0x8003A304u, 0x800362E4u);
            descriptor = 0x80092270u;
            break;
        case 6:
            w_u8(0x801F2FE8u, 0);
            (void)gtmain_menu_sound(4);
            descriptor = 0x80091BE8u;
            break;
        case 7:
            mode = r_s8(0x801F2FEBu);
            (void)gtmain_menu_push(mode == 7 || mode == 9 ? 0x80091D0Cu : 0x80091CF0u);
            (void)gtmain_menu_sound(4);
            return 3;
        case 8:
            {
                uint32 run_mode, exec_mode, course;
                w_u8(0x801F2FE8u, 0);
                (void)gtmain_menu_sound(4);
                run_mode = r_u8(0x800CB7F5u);
                exec_mode = r_u8(0x800CB7F4u);
                course = r_u8(0x800B63BCu);
                (void)selection_ghost_transition(0x8003A304u, 0x800362E4u, exec_mode, course, run_mode);
                descriptor = 0x80092270u;
                break;
            }
        case 9:
            (void)gtmain_menu_sound(4);
            descriptor = 0x80091DD4u;
            break;
        default:
            return 0;
        }
    }
    (void)gtmain_menu_push(descriptor);
    return 3;
}
