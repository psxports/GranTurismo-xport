#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static uint32 part_inventory(uint32 inventory, sint32 type)
{
    /* TODO Bind unselected inventory bit-table lookup 8006C12C */
    return gt_draft_unresolved_call(0x8006C12Cu, inventory, (uint32)type,
        0u, 0u, 0u, 0u, 0u, 0u);
}
static uint32 part_equipment_fit(uint32 car, sint32 type, uint32 item, uint32 output)
{
    /* TODO Bind unselected four-argument equipment check 8006C058 */
    return gt_draft_unresolved_call(0x8006C058u, car, (uint32)type,
        item, output, 0u, 0u, 0u, 0u);
}
static void part_status_log(uint32 format)
{
    printf((const char *)psx_addr(format, 1u));
}

/* TODO Resolve independent inventory and equipment services */
/* GTMAIN.EXE 8004ABA0 */
sint32 gt_draft_8004ABA0(uint32 target, sint32 part_type, uint32 context)
{
    uint32 offset, item, bits, index, available, current, car;
    if (r_u8(context + 95u) && part_type != 34) return 0;
    offset = r_u8(0x800941BDu + (uint32)part_type);
    item = r_u16(target + offset);
    if (!item) return 0;
    bits = part_inventory(r_u32(0x800BEA68u), part_type);
    index = item - 1u;
    available = r_u8(bits + (index >> 3)) & (1u << (index & 7u));
    printf((const char *)psx_addr(0x8008DD48u, 1u), (int)part_type, (int)item);
    if (!available) {
        part_status_log(0x8008DD70u);
        return -2;
    }
    current = r_u32(0x800BEA64u);
    car = r_u16(current) & 4095u;
    if (!part_equipment_fit(car, part_type, item, 0x800D6FDCu)) {
        part_status_log(0x8008DD7Cu);
        return -3;
    }
    part_status_log(0x8008DD94u);
    return 0;
}
