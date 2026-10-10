#include "game_first2_registry.h"

/* GTMAIN.EXE 800403C4 */
uint32 gtmain_menu_machine_context_bind(uint8 kind, uint32 owner, uint32 resource, uint8 state, uint32 value, uint32 text)
{
    w_u32(0x801F1FDCu, owner);
    w_u8(0x801F1FE4u, state);
    w_u8(0x801F1FE5u, kind);
    w_u32(0x801F1FE8u, resource);
    w_u32(0x801F1FE0u, value);
    gtmain_string_copy(0x801F1FECu, text);
    w_u32(0x801F2488u, 0x80035D88u);
    w_u32(0x800C0A80u, 0u);
    return 0x80035D88u;
}

/* GTMAIN.EXE 8004A148 */
uint32 gtmain_menu_entry_name_request_prepare(void)
{
    sint32 index;
    gtmain_menu_resource_request_initialize(0x80092F44u, 31, 256, 0x800BEA40u);
    index = (sint16)r_u16(0x800B53BAu);
    if (index >= 0) {
        uint32 entries = r_u32(0x800BEA34u);
        return gtmain_menu_name_text_set(0x80092F44u, entries + 52u * (uint32)index + 8u);
    }
    return (uint32)index;
}
