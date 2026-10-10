#include "game_first2_registry.h"

/* GTMAIN.EXE 8001E600 */
uint32 gtmain_race_driver_selection(uint32 index, uint32 archive)
{
    uint32 input = 0x800CB7FAu + index * 6u;
    uint32 destination = 0x800BB790u + index * 96u;
    uint32 id = r_u16(input), section = 1u, record, variant, value;
    w_u16(0x800B63C4u + index * 2u, (uint16)id);
    if (r_u8(input + 5u)) section = 2u;
    record = gtmain_car_selected_record((uint32)(sint32)(sint16)id, archive, section);
    gtmain_memory_copy(destination, record, 96u);
    variant = r_u8(input + 2u);
    value = r_u16(destination);
    w_u16(destination, (uint16)(value | (variant << 12)));
    value = r_u8(input + 3u);
    w_u8(destination + 92u, (uint8)value);
    value = r_u8(input + 4u);
    w_u8(destination + 93u, (uint8)value);
    value = r_u8(input + 5u);
    w_u8(destination + 94u, (uint8)value);
    return value;
}

/* GTMAIN.EXE 8001E988 */
uint32 gtmain_race_driver_configurations(uint32 archive, uint32 kind,
    uint32 mode, uint32 section)
{
    uint32 count = 1u, i = 0u;
    w_u8(0x800CBC15u, (uint8)section);
    gtmain_car_archive_records_load(archive);
    gtmain_opponent_configurations(archive, section);
    /* TODO Bind unrecorded saved-selection call 8001E810 if actually reached */
    if (mode == 1u) {
        gt_draft_unresolved_call(0x8001E810u, 1u, 0x800CDDD8u, archive, 0, 0, 0, 0, 0);
        count = 2u;
    } else if (mode == 2u) {
        if (kind == 1u) {
            gtmain_race_driver_selection(0u, archive);
            gtmain_race_driver_selection(1u, archive);
        } else if (kind == 2u) {
            gt_draft_unresolved_call(0x8001E810u, 0u, 0x800CB7FAu, archive, 0, 0, 0, 0, 0);
            gt_draft_unresolved_call(0x8001E810u, 1u, 0x800CB85Au, archive, 0, 0, 0, 0, 0);
        }
        count = 2u;
        goto Build;
    }
    if (kind == 1u)
        gtmain_race_driver_selection(0u, archive);
    else if (kind == 2u)
        gtmain_car_saved_selection_load(0u, (uint32)(sint32)(sint16)r_u16(0x800C4C58u), archive);
Build:
    do {
        gtmain_car_config_build(0x800BB850u + i * 386u,
            0x800BB790u + i * 96u, archive);
        ++i;
    } while (i < count);
    return 0u;
}

/* GTMAIN.EXE 80022310 */
uint32 gtmain_car_palette_tint(uint8 *output, const uint8 *input,
    uint32 tint, uint32 masks)
{
    uint32 percent = r_u8(tint + 3u), first = r_u8(tint), second = r_u8(tint + 1u);
    uint32 color = (first >> 3) | ((second >> 3) << 5);
    uint32 row, column;
    sint16 weight = (sint16)((percent << 12) / 100u);
    color |= (r_u8(tint + 2u) >> 3) << 10;
    for (row = 0u; row < 16u; ++row) {
        uint32 mask = r_u16(masks + row * 2u);
        for (column = 0u; column < 16u; ++column) {
            uint32 offset = row * 32u + column * 2u;
            uint16 value = xport_load_le16(input + offset);
            if (value & 32767u) {
                if (!(mask & (1u << column)))
                    value = gtmain_color555_blend(value, (uint16)color, weight);
                if (!value) value |= 32768u;
            }
            xport_store_le16(output + offset, value);
        }
    }
    return 512u;
}
