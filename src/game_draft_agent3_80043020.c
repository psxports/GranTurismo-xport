#include "game_first2_registry.h"
#include "game_draft_registry.h"

static uint32 parts_resource(sint32 category)
{
    /* TODO Bind unselected parts resource lookup 8001EB94 */
    return gt_draft_unresolved_call(0x8001EB94u, (uint32)category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80043020 */
uint32 gt_draft_80043020(void)
{
    uint32 table = parts_resource(17);
    uint32 offset = r_u8(0x800941CEu);
    uint32 context = r_u32(0x800BEA64u);
    uint32 part = r_u16(context + offset), record, value;
    value = r_u8(context + 74u);
    w_u16(0x80092A08u, 1u);
    w_u16(0x80092A0Cu, value + 1u);
    record = table + 28u * (part - 1u);
    value = r_u8(record + 4u);
    w_u16(0x80092A0Au, value);
    value = r_u8(context + 87u);
    w_u16(0x80092A18u, 1u);
    w_u16(0x80092A1Cu, value + 1u);
    value = r_u8(record + 7u);
    w_u16(0x80092A1Au, value);
    return value;
}
