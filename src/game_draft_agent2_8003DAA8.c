#include "game_first2_registry.h"

/* GTMAIN.EXE 8003DAA8 */
uint32 gtmain_menu_record_header_prepare(void)
{
    uint32 source = 0x801F1FECu, destination = 0x801D2FDCu;
    uint32 character, value, icon;
    sint32 loading, kind;
    (void)gtmain_menu_record_workspace_clear();
    loading = (sint8)r_u8(0x801F1FE5u);
    if (loading) return (uint32)loading;
    character = r_u8(source);
    w_u16(destination, 0x4353u);
    value = r_u32(0x801F1FE0u);
    w_u8(destination + 2u, 17u);
    w_u8(destination + 3u, value);
    while (character) {
        value = r_u8(source + 1u);
        source += 2u;
        /* TODO Bind unrecorded original symbol converter */
        value = gt_draft_unresolved_call(0x80071640u,
            ((character << 8) | value) & 0x7F7Fu, 0, 0, 0, 0, 0, 0, 0) & 65535u;
        w_u8(destination + 4u, value >> 8);
        w_u8(destination + 5u, value);
        character = r_u8(source);
        destination += 2u;
    }
    kind = (sint8)r_u8(0x801F1FE4u);
    icon = kind == 1 ? 0x8009969Cu : kind == 2 ? 0x800997D4u : 0x8009954Cu;
    /* TODO Bind bounded source extent and original result of the icon decoder */
    value = gt_draft_unresolved_call(0x80071698u, 0x801D303Cu, icon, 416u, 0, 0, 0, 0, 0);
    if (kind == 1 || kind == 2) return value;
    /* TODO Bind unrecorded original game record payload tail */
    return gt_draft_unresolved_call(0x8002CAD0u, r_u32(0x801F1FDCu) + 512u, 0, 0, 0, 0, 0, 0, 0);
}
