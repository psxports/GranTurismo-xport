#include "game_first2_registry.h"
#include <stdlib.h>

static uint32 roster_random(uint32 *seed)
{
    *seed = *seed * 17u + 17u;
    return *seed;
}

/* GTMAIN.EXE 8001F080 */
uint32 gtmain_roster_pool_count(uint32 list)
{
    uint32 count = 0u;
    if (r_u32(list)) {
        do { ++count; } while (r_u32(list + count * 4u));
    }
    return count;
}

/* GTMAIN.EXE 8001F0B8 */
sint32 gtmain_roster_variant_class(sint32 character)
{
    uint32 code = (uint32)character;
    if (character >= 97) code -= 32u;
    return (sint8)r_u8(0x8008F30Cu + code);
}

/* GTMAIN.EXE 8001F0DC */
void gtmain_roster_select(uint32 *selected, uint32 list, sint32 count,
    uint32 *seed, uint32 excluded, uint32 pools)
{
    sint8 kinds[5];
    uint16 choices[5];
    sint32 limit = count < 6 ? count : 5, i = 0;
    sint32 excluded_kind = (sint8)r_u8(gtmain_car_name(excluded));
    uint32 pool = list, pool_count = gtmain_roster_pool_count(list);
    while (i < limit) {
        uint32 chosen, candidate;
        sint32 kind, attempts = 0;
        if (pools) {
            pool = r_u32(pools + (uint32)i * 4u);
            pool_count = gtmain_roster_pool_count(pool);
        }
        roster_random(seed);
        for (;;) {
            uint32 value = roster_random(seed), entry;
            sint32 found, previous;
            if (!pool_count) gt_draft_exception(7u);
            chosen = value % pool_count;
            entry = r_u32(pool + chosen * 4u);
            kind = (sint8)r_u8(entry);
            found = gtmain_car_find(entry);
            candidate = found < 0 ? 0u : (uint32)found;
            if (found >= 0 && kind != excluded_kind && candidate != excluded) {
                previous = 0;
                while (previous < i) {
                    if (kinds[previous] == kind) break;
                    if (selected[previous] == candidate) break;
                    ++previous;
                }
                if (previous == i) break;
            }
            if (++attempts >= 256) break;
        }
        choices[i] = (uint16)chosen;
        kinds[i] = (sint8)kind;
        selected[i] = candidate;
        ++i;
    }
    while (i < count) {
        uint32 chosen = roster_random(seed) % 5u;
        selected[i++] = selected[chosen];
    }
    for (i = 0; i < limit; ++i) {
        sint32 key = (sint16)choices[i], j;
        uint32 value = selected[i];
        for (j = i; j < limit; ++j) {
            sint32 next = (sint16)choices[j];
            if (next < key) {
                uint32 saved;
                choices[j] = (uint16)key;
                saved = selected[j];
                key = next;
                selected[j] = value;
                value = saved;
            }
        }
        choices[i] = (uint16)key;
        selected[i] = value;
    }
}

/* GTMAIN.EXE 8001F3D0 */
void gtmain_roster_variants(uint32 *variants, const uint32 *selected,
    sint32 count, uint32 *seed, uint32 excluded_class)
{
    sint8 *classes;
    sint32 i;
    if (count <= 0) return;
    classes = (sint8 *)malloc((size_t)count);
    if (!classes) gt_draft_failure("Native roster variant scratch allocation", 0x8001F3D0u);
    for (i = 0; i < count; ++i) {
        uint32 car = selected[i], chosen;
        uint32 choices = gtmain_resource_pairs_count(car);
        uint32 pairs = gtmain_resource_pairs(car);
        sint32 attempts = 0, kind;
        for (;;) {
            uint32 value = roster_random(seed);
            sint32 character, previous;
            if (!choices) gt_draft_exception(7u);
            chosen = value % choices;
            character = gtmain_resource_pair_value(chosen, pairs);
            kind = gtmain_roster_variant_class(character);
            if ((sint32)choices < 2) break;
            if (character != 45 && (uint32)kind != excluded_class) {
                previous = 0;
                while (previous < i && classes[previous] != kind) ++previous;
                if (previous == i) break;
            }
            if (++attempts >= 256) break;
        }
        classes[i] = (sint8)kind;
        variants[i] = chosen;
    }
    free(classes);
}

/* GTMAIN.EXE 8001F564 */
void gtmain_roster_commit(const uint32 *selected, const uint32 *variants,
    sint32 count, uint32 seed)
{
    sint32 i;
    for (i = 0; i < count; ++i) {
        uint32 car = xport_load_le16((const uint8 *)selected + (uint32)i * 4u);
        uint32 variant = xport_load_le16((const uint8 *)variants + (uint32)i * 4u);
        uint16 packed = (uint16)((car & 511u) | ((variant & 127u) << 9));
        w_u16(0x800B6650u + (uint32)i * 2u, packed);
    }
    w_u32(0x800B6640u, seed);
}

/* GTMAIN.EXE 80025B68 */
/* TODO Resolve independent generator extent and variant owner limitations */
/* GTMAIN.EXE 80025B68 */
void gtmain_race_arcade_roster(void)
{
    uint32 player, count, table, group, mode, spec;
    sint32 index;
    gtmain_race_config_reset();
    player = r_u16(0x800CB7FAu);
    count = r_u8(0x800B6394u);
    index = gtmain_name_group_find(0x8008F798u, gtmain_car_name(player));
    if (index < 0) index = 0;
    w_u8(0x800B631Cu, (uint8)index);
    mode = r_u8(0x800B63BDu);
    table = r_u32(0x8008F78Cu + (uint32)index * 4u);
    group = r_u32(0x8008F798u + (uint32)index * 4u);
    player = r_u16(0x800CB7FAu);
    spec = r_u8(0x800CB7FCu);
    w_u32(0x800BC1B4u, r_u32(table + mode * 4u));
    gtmain_roster_generate(group, count, player, spec);
}
