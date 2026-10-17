#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

/* TODO Resolve independent inventory and equipment child services */
/* GTMAIN.EXE 8004ACB8 */
sint32 gtmain_menu_parts_check(uint32 target)
{
    static const uint8 fields[6] = { 30, 34, 4, 12, 16, 24 };
    uint32 context, table, target_number, current_number, i;
    sint32 type, result, equal = 1;
    printf((const char *)psx_addr(0x8008DD98u, 1u));
    context = r_u32(0x800BEA64u);
    target_number = r_u16(target);
    current_number = r_u16(context);
    printf((const char *)psx_addr(0x8008DDB0u, 42u),
        (int)(current_number & 4095u), (int)(target_number & 4095u));
    context = r_u32(0x800BEA64u);
    current_number = r_u16(context);
    target_number = r_u16(target);
    if ((current_number & 4095u) != (target_number & 4095u)) return -1;
    table = 0x800935A4u;
    for (;;)
    {
        type = (sint16)r_u16(table);
        if (type < 0) break;
        result = gtmain_menu_part_check(target, type);
        table += 2u;
        if (result) return result;
    }
    for (i = 0; i < 6u; ++i)
    {
        context = r_u32(0x800BEA64u);
        target_number = r_u16(target + fields[i]);
        current_number = r_u16(context + fields[i]);
        if (target_number != current_number) equal = 0;
    }
    return equal ? 0 : -2;
}
