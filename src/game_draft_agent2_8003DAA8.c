#include "game_first2_registry.h"

static uint32 record_symbol_convert(uint32 symbol)
{
    /* TODO Bind the unselected original symbol converter */
    return gt_draft_unresolved_call(0x80071640u, symbol, 0, 0, 0, 0, 0, 0, 0);
}

static uint32 record_icon_decode(uint32 icon)
{
    uint32 source_capacity = 0x800B38F4u - icon;
    uint8 *destination = psx_addr(0x801D303Cu, 416u);
    const uint8 *source = psx_addr(icon, source_capacity);
    uint8 *result = gtmain_lz_decode(destination, 416u, source, source_capacity, 416);
    return 0x801D303Cu + (uint32)(result - destination);
}

static uint32 record_payload_prepare(uint32 record)
{
    /* TODO Bind the unselected original record payload tail */
    return gt_draft_unresolved_call(0x8002CAD0u, record, 0, 0, 0, 0, 0, 0, 0);
}

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
        value = record_symbol_convert(((character << 8) | value) & 0x7F7Fu) & 65535u;
        w_u8(destination + 4u, value >> 8);
        w_u8(destination + 5u, value);
        character = r_u8(source);
        destination += 2u;
    }
    kind = (sint8)r_u8(0x801F1FE4u);
    icon = kind == 1 ? 0x8009969Cu : kind == 2 ? 0x800997D4u : 0x8009954Cu;
    value = record_icon_decode(icon);
    if (kind == 1 || kind == 2) return value;
    return record_payload_prepare(r_u32(0x801F1FDCu) + 512u);
}
