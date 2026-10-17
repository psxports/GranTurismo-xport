#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void settings_inventory_select(uint32 inventory, uint32 kind)
{
    /* TODO Bind the unselected inventory selector */
    (void)gt_draft_unresolved_call(0x8006C12Cu, inventory, kind, 0u, 0u, 0u, 0u, 0u, 0u);
}

static sint32 settings_variant_count(uint32 kind)
{
    /* TODO Bind the unselected category variant count */
    return (sint32)gt_draft_unresolved_call(0x8001EBC4u, kind, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static uint32 settings_variant_available(uint32 car, uint32 kind, uint32 variant)
{
    /* TODO Bind the unselected equipment availability check */
    return gt_draft_unresolved_call(0x8006C058u, car, kind, variant, 0x800D6FDCu, 0u, 0u, 0u, 0u);
}

uint32 gtmain_menu_settings_mask(uint32 table, sint32 count)
{
    uint32 context = r_u32(0x800BEA64u);
    uint32 car = r_u16(context) & 0xFFFu, mask = 0u, bit;
    sint32 index;
    if (count <= 0) return mask;
    bit = 1u << ((uint32)count & 31u);
    for (index = 0; index < count; ++index, ++table) {
        uint32 kind = r_u8(table), variant = 1u;
        sint32 limit;
        settings_inventory_select(r_u32(0x800BEA68u), kind);
        limit = settings_variant_count(kind);
        if (limit > 0) {
            for (;;) {
                if (settings_variant_available(car, kind, variant)) {
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
