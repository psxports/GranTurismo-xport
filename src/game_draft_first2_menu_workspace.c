#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

/* GTMAIN.EXE 8003DA78 */
uint32 gtmain_menu_record_workspace_clear(void)
{
    return sub_8007BD58(0x801D2FDCu, 0u, 0x1F000u);
}

/* GTMAIN.EXE 8003CEE4 */
uint32 gtmain_menu_machine_get_init(void)
{
    w_u8(r_u32(0x800B52FCu), 0u);
    /* TODO Missing BIOS printf adapter */
    gt_draft_unresolved_call(0x8008B7D8u, 0x8008CE14u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return 0u;
}

/* GTMAIN.EXE 800380B4 */
uint32 gtmain_menu_countdown_toggle_tail(sint32 countdown)
{
    uint32 toggle;
    if (countdown > 0) w_u16(0x801F300Au, (uint16)((uint32)countdown - 1u));
    toggle = (r_u8(0x801F3008u) + 1u) & 1u;
    w_u8(0x801F3008u, toggle);
    return toggle;
}
