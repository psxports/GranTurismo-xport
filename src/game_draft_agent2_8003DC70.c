#include "game_first2_registry.h"

static uint32 menu_request(uint32 address, uint32 owner)
{
    /* TODO Bind unselected request bodies 80047CA0 and 800477E0 */
    return gt_draft_unresolved_call(address, owner, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static uint32 menu_index_request(uint32 owner, sint32 index)
{
    /* TODO Bind unselected indexed request 80049898 */
    return gt_draft_unresolved_call(0x80049898u, owner, (uint32)index, 0u, 0u, 0u, 0u, 0u, 0u);
}

static uint32 menu_detail_request(uint32 owner, uint32 index, uint32 first, uint32 second, uint32 flags, uint32 kind)
{
    /* TODO Bind unselected six-argument request 800492EC */
    return gt_draft_unresolved_call(0x800492ECu, owner, index, first, second, flags, kind, 0u, 0u);
}

/* GTMAIN.EXE 8003DC70 */
/* Original: GTMAIN.EXE:sub_8003DC70. */ uint32 gtmain_menu_mode_dispatch(sint32 mode)
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
    case 27:
        return menu_request(0x80047CA0u, r_u32(0x801F1FDCu));
    case 35:
        return menu_request(0x800477E0u, r_u32(0x801F1FDCu));
    case 37:
        owner = r_u32(0x801F1FDCu);
        index = (sint16)r_u16(0x801F229Eu);
        w_u8(0x801F202Cu, 1u);
        return menu_index_request(owner, index);
    case 38:
        fourth = r_u8(0x801F2037u);
        owner = r_u32(0x801F1FDCu);
        first = (uint32)(sint32)(sint16)r_u16(0x801F202Eu);
        fifth = r_u8(0x801F2040u);
        second = (uint32)(sint32)(sint8)r_u8(0x801F2034u);
        third = (uint32)(sint32)(sint8)r_u8(0x801F2035u);
        return menu_detail_request(owner, first, second, third, fourth, fifth);
    default:
        return result;
    }
    w_u32(0x801F2484u, result);
    return result;
}
