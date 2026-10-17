#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void part_parameters(uint32 category)
{
    /* TODO Bind unselected parts parameters 80042C48 */
    (void)gt_draft_unresolved_call(0x80042C48u, category, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void parts_refresh(void)
{
    /* TODO Bind unselected parts refresh 8001EB08 */
    (void)gt_draft_unresolved_call(0x8001EB08u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void parts_summary(uint32 destination, uint32 summary, uint32 detail)
{
    /* TODO Bind unselected parts summary 80058BB0 */
    (void)gt_draft_unresolved_call(0x80058BB0u, destination, summary, detail, 0u, 0u, 0u, 0u, 0u);
}

static void parts_detail(uint32 destination, uint32 summary, uint32 detail)
{
    /* TODO Bind unselected parts detail 8004D78C */
    (void)gt_draft_unresolved_call(0x8004D78Cu, destination, summary, detail, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80045910 */
uint32 gt_draft_80045910(void)
{
    uint32 input = r_u32(0x800BDD08u), context, category, part, i;
    sint32 pending, selected, limit;
    uint32 snapshot[24], summary, detail;
    gtmain_menu_stream_check();
    pending = (sint16)r_u16(0x801D2FDCu);
    if (pending >= 0) {
        if ((sint8)r_u8(0x801D2FE0u + (uint32)pending)) {
            context = r_u32(0x800BEA64u);
            for (i = 0u; i < 24u; i += 4u) {
                snapshot[i] = r_u32(context + 4u * i);
                snapshot[i + 1u] = r_u32(context + 4u * i + 4u);
                snapshot[i + 2u] = r_u32(context + 4u * i + 8u);
                snapshot[i + 3u] = r_u32(context + 4u * i + 12u);
            }
            category = r_u8(0x800B5374u);
            part = r_u16(0x800B5314u + 8u * (uint32)pending);
            (void)gt_draft_8004AB30((sint32)category, (uint16)part);
            category = r_u8(0x800B5374u);
            part_parameters(category);
            parts_refresh();
            summary = 0x801D33F8u + 60u * (uint32)pending;
            detail = 0x801D3098u + 108u * (uint32)pending;
            parts_summary(0x800BB850u, summary, detail);
            parts_detail(0x801D2FE8u, summary, detail);
            context = r_u32(0x800BEA64u);
            for (i = 0u; i < 24u; i += 4u) {
                w_u32(context + 4u * i, snapshot[i]);
                w_u32(context + 4u * i + 4u, snapshot[i + 1u]);
                w_u32(context + 4u * i + 8u, snapshot[i + 2u]);
                w_u32(context + 4u * i + 12u, snapshot[i + 3u]);
            }
            parts_refresh();
        }
        limit = (sint16)r_u16(0x801D2FDEu);
        ++pending;
        if (pending >= limit) pending = -1;
        w_u16(0x801D2FDCu, (uint16)pending);
    }
    if (!(sint8)r_u8(input)) return 0u;
    (void)gtmain_menu_flags(1u);
    selected = gtmain_menu_selector_input(0x80093530u, input + 2u);
    if (selected == -2) {
        (void)gtmain_menu_sound(2);
        return 1u;
    }
    if (selected == -1) return 0u;
    category = r_u8(0x800B5374u);
    part = r_u16(0x800B5314u + 8u * (uint32)selected);
    (void)gt_draft_8004AB30((sint32)category, (uint16)part);
    category = r_u8(0x800B5374u);
    part_parameters(category);
    parts_refresh();
    (void)gtmain_menu_sound(4);
    return 1u;
}
