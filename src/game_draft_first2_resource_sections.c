#include "game_first2_registry.h"

/* Guest-address variant of selected GTMAIN.EXE:8006A21C */
static uint32 sections_read_retry(uint32 path, uint32 destination, uint32 requested_bytes)
{
    for (;;) {
        uint32 result = gtmain_cd_file_load(path, destination, requested_bytes, 0);
        if (result || r_u32(0x800B1AB0u) == 12u) return result;
        gtmain_cd_file_retry_hook();
    }
}

static uint32 sections_align(uint32 address)
{
    return (address + 3u) & ~3u;
}

/* GTMAIN.EXE:80069FF0 */
uint32 gtmain_resource_sections_load(uint32 path, uint32 destination, uint32 requested_bytes)
{
    /* TODO Selected file loader dependencies remain partial */
    uint32 resource = sections_read_retry(path, destination, requested_bytes);
    uint32 cursor = (resource + 75u) & ~3u;
    sint32 count = r_s16(resource + 12u), next_count;
    w_u32(resource + 24u, cursor);
    cursor = sections_align(gtmain_packed_bytes_entry(cursor, (uint32)count));
    w_u32(resource + 28u, cursor);
    cursor = sections_align(cursor + (uint32)count * 16u);
    next_count = r_s16(resource + 14u);
    w_u32(resource + 32u, cursor);
    cursor = sections_align(gtmain_packed_bytes_entry(cursor, (uint32)count));
    count = next_count;
    w_u32(resource + 36u, cursor);
    cursor = sections_align(cursor + (uint32)count * 6u + 32u);
    w_u32(resource + 40u, cursor);
    cursor = sections_align(gtmain_packed_pairs_entry(cursor, (uint32)count));
    next_count = r_s16(resource + 16u);
    w_u32(resource + 44u, cursor);
    cursor = sections_align(gtmain_packed_bytes_entry(cursor, (uint32)count));
    count = next_count;
    w_u32(resource + 48u, cursor);
    cursor = sections_align(cursor + (uint32)count * 2u);
    count = r_s16(resource + 18u);
    w_u32(resource + 52u, cursor);
    cursor = sections_align(gtmain_packed_bytes_entry(cursor, (uint32)count));
    w_u32(resource + 56u, cursor);
    cursor = sections_align(cursor + (uint32)count * 8u);
    next_count = r_s16(resource + 20u);
    w_u32(resource + 60u, cursor);
    cursor = sections_align(gtmain_packed_bytes_entry(cursor, (uint32)count));
    count = next_count;
    w_u32(resource + 64u, cursor);
    cursor = sections_align(cursor + (uint32)count * 8u);
    w_u32(resource + 68u, cursor);
    return 1;
}
