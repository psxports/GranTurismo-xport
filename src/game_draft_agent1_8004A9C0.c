#include "game_first2_registry.h"
#include "xport_trace.h"

uint32 gtmain_menu_settings_mask_context(uint32 table, sint32 count, uint32 context)
{
    uint32 car = r_u16(context) & 0xFFFu, mask = 0u, bit;
    sint32 index;
    FUNCTION_MARKER(0x8004A9C0, "GTMAIN.EXE");
    if (count <= 0) return mask;
    bit = 1u << ((uint32)count & 31u);
    for (index = 0; index < count; ++index, ++table) {
        uint32 kind = r_u8(table), variant = 1u;
        sint32 limit;
        gtmain_parts_inventory_flags(r_u32(0x800BEA68u), kind);
        limit = (sint32)gtmain_parts_count(kind);
        if (limit > 0) {
            for (;;) {
                if (gtmain_parts_record_matches_car(car, kind, variant, 0x800D6FDC)) {
                    mask |= bit;
                    break;
                }
                ++variant;
                if (limit < (sint32)variant) break;
            }
        }
        mask = (uint32)((sint32)mask >> 1);
    }
    return mask;
}
