#include "game_first2_registry.h"

/* GTMAIN.EXE 8003DC70 */
uint32 gtmain_menu_mode_dispatch(sint32 mode)
{
    uint32 result, owner, first, second, third, fourth, fifth;
    sint32 index;
    w_u8(0x801F2480u, (uint32)mode);
    if ((uint32)mode >= 51u) return (uint32)mode << 2;
    result = r_u32(0x8008CE30u + (uint32)mode * 4u);
    switch (mode) {
    case 0:
        (void)gtmain_menu_transition_enter(0x8009228Cu);
        result = 0x8009229Cu;
        break;
    case 1:
        w_u8(0x801F2481u, 8u);
        w_u8(0x801F2483u, 0u);
        return 8u;
    case 4:
        (void)gtmain_menu_sound(6);
        (void)gtmain_menu_transition_enter(0x800922A4u);
        w_u32(0x801F2484u, 0x800922B4u);
        /* Fall through to the second original transition */
    case 3: case 9: case 22:
        (void)gtmain_menu_sound(6);
        (void)gtmain_menu_transition_enter(0x800922A4u);
        result = 0x800922B4u;
        break;
    case 2: case 6:
        (void)gtmain_menu_transition_enter(0x800922A4u);
        result = 0x800922B4u;
        break;
    case 5:
        (void)gtmain_menu_transition_exit(0x800922ECu);
        result = 0x800922FCu;
        break;
    case 13: case 19: case 31: case 48:
        w_u8(0x800B5300u, 1u);
        (void)gtmain_menu_transition_exit(0x800922A4u);
        result = 0x800922D4u;
        break;
    case 36:
        (void)gtmain_menu_transition_exit(0x800922A4u);
        result = 0x800922D4u;
        break;
    case 14: case 32: case 47:
        (void)gtmain_menu_sound(6);
        (void)gtmain_menu_transition_enter(0x800922BCu);
        result = 0x800922DCu;
        break;
    case 15:
        (void)gtmain_menu_transition_exit(0x800922BCu);
        result = 0x800922E4u;
        break;
    case 20: case 21:
        (void)gtmain_menu_sound(6);
        (void)gtmain_menu_transition_enter(0x800922BCu);
        result = 0x80092304u;
        break;
    case 25:
        (void)gtmain_menu_sound(6);
        (void)gtmain_menu_transition_enter(0x800922BCu);
        result = 0x8009230Cu;
        break;
    case 26: case 44:
        (void)gtmain_menu_sound(6);
        /* Fall through */
    case 10:
        (void)gtmain_menu_transition_exit(0x800922BCu);
        result = 0x800922CCu;
        break;
    case 28:
        owner = r_u32(0x801F1FDCu);
        index = (sint16)r_u16(0x801F202Eu);
        return gtmain_menu_name_request_prepare(owner, index, result);
    case 39:
        return gtmain_menu_entry_name_request_prepare();
    case 40: case 41: case 42: case 43: case 45: case 46:
        w_u16(0x801F2030u, 65535u);
        return 0xFFFFFFFFu;
    /* TODO Bind unrecorded original game request bodies */
    case 27:
        return gt_draft_unresolved_call(0x80047CA0u, r_u32(0x801F1FDCu), 0, 0, 0, 0, 0, 0, 0);
    case 35:
        return gt_draft_unresolved_call(0x800477E0u, r_u32(0x801F1FDCu), 0, 0, 0, 0, 0, 0, 0);
    case 37:
        owner = r_u32(0x801F1FDCu);
        index = (sint16)r_u16(0x801F229Eu);
        w_u8(0x801F202Cu, 1u);
        return gt_draft_unresolved_call(0x80049898u, owner, (uint32)index, 0, 0, 0, 0, 0, 0);
    case 38:
        fourth = r_u8(0x801F2037u);
        owner = r_u32(0x801F1FDCu);
        first = (uint32)(sint32)(sint16)r_u16(0x801F202Eu);
        fifth = r_u8(0x801F2040u);
        second = (uint32)(sint32)(sint8)r_u8(0x801F2034u);
        third = (uint32)(sint32)(sint8)r_u8(0x801F2035u);
        return gt_draft_unresolved_call(0x800492ECu, owner, first, second, third, fourth, fifth, 0, 0);
    default:
        return result;
    }
    w_u32(0x801F2484u, result);
    return result;
}
