#include "game_first2_registry.h"
#include <stdio.h>

static sint32 garage_name_index(uint32 guest_name)
{
    /* TODO Bind unselected name-index service */
    gt_draft_failure("GTMAIN garage name index", 0x8006B828u);
}

static uint32 garage_variant_count(sint32 index)
{
    /* TODO Bind unselected variant-count service */
    gt_draft_failure("GTMAIN garage variant count", 0x8006C2ECu);
}

static uint32 garage_seed_step(uint32 *seed)
{
    /* TODO Bind existing guest random service to native seed word */
    gt_draft_failure("GTMAIN garage native seed adapter", 0x8006C31Cu);
}

/* GTMAIN.EXE:8001EE20 */
uint32 gt_draft_8001EE20(uint32 guest_name)
{
    uint32 seed = gtmain_timer_random();
    sint32 index = garage_name_index(guest_name);
    uint32 variants = garage_variant_count(index);
    uint32 random = garage_seed_step(&seed);
    sint32 slot = (sint16)r_u16(0x800C4C5Au);
    uint32 variant, source, destination, table, model, count;
    if (!variants) gt_draft_failure("GTMAIN garage original BREAK7", 0x8001EE84u);
    variant = random % variants;
    if (slot >= 100) return 0u;
    gtmain_car_archive_records_load(0x800D6FDCu);
    gtmain_race_track_invalidate();
    index = gtmain_car_record_find_variant(guest_name, 0x800D6FDCu, 0u);
    if (index < 0) {
        printf((const char *)psx_addr(0x8008B8E4u, 1u),
            (const char *)psx_addr(guest_name, 1u));
        index = 0;
    }
    source = 0x800D6FDCu + r_u32(0x800D6FECu) + 24u + (uint32)index * 108u;
    destination = 0x800C4C4Cu + (uint32)slot * 96u + 416u;
    gtmain_memory_copy(destination, source, 96u);
    w_u16(destination, (uint16)(r_u16(destination) | (variant << 12)));
    table = r_u32(0x800D7088u);
    model = r_u16(source) & 0xFFFu;
    w_u32(0x800C4C4Cu + (uint32)slot * 4u + 16u,
        r_u32(0x800D6FDCu + table + model * 424u - 12u));
    count = r_u16(0x800C4C5Au);
    w_u16(0x800C4C5Au, (uint16)(count + 1u));
    return 1u;
}
