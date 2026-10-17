#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdint.h>
#include <string.h>

static const uint8 *lz_read_span(const uint8 *owner, uint32 capacity,
    uintptr_t address, uint32 bytes)
{
    uintptr_t base = (uintptr_t)owner;
    uintptr_t dram = (uintptr_t)psx_addr(0x80000000u, 0x200000u);
    uintptr_t scratch = (uintptr_t)psx_addr(0x1F800000u, 0x400u);
    if (!owner) gt_draft_failure("Native LZ readable memory owner", 0x80071698u);
    if (base >= dram && base - dram < 0x200000u) {
        base = dram;
        capacity = 0x200000u;
    } else if (base >= scratch && base - scratch < 0x400u) {
        base = scratch;
        capacity = 0x400u;
    }
    if (address < base || address - base > capacity ||
        bytes > capacity - (uint32)(address - base))
        gt_draft_failure("Native LZ readable memory span", 0x80071698u);
    return (const uint8 *)address;
}

static uint32 lz_source_word(const uint8 *source, uint32 capacity, uintptr_t address)
{
    uint32 word;
    memcpy(&word, lz_read_span(source, capacity, address, 4u), 4u);
    return word;
}

/* GTMAIN.EXE 80071698 */
uint8 *gtmain_lz_decode(uint8 *destination, uint32 destination_capacity,
    const uint8 *source, uint32 source_capacity, sint32 output_bytes)
{
    uint32 produced = 0, cursor = 0, flags = 0, bit = 0;
    sint32 remaining = output_bytes;
    if (remaining <= 0) return destination;
    if (!destination || (uint32)remaining > destination_capacity)
        gt_draft_failure("Native LZ output extent", 0x80071698u);
    (void)lz_read_span(destination, destination_capacity, (uintptr_t)destination, (uint32)remaining);
    while (remaining > 0) {
        uint32 word, amount, distance, i;
        uintptr_t address;
        flags >>= 1;
        if (!bit) {
            flags = *lz_read_span(source, source_capacity, (uintptr_t)source + cursor, 1u);
            ++cursor;
        }
        address = (uintptr_t)source + cursor;
        word = lz_source_word(source, source_capacity, address & ~(uintptr_t)3u) >> (8u * (uint32)(address & 3u));
        ++cursor;
        if (!(flags & 1u)) {
            destination[produced++] = (uint8)word;
            --remaining;
        } else {
            uint32 upper = lz_source_word(source, source_capacity, (address + 3u) & ~(uintptr_t)3u);
            uint32 offset = (uint32)(address & 3u);
            if (offset) word |= upper << (8u * (4u - offset));
            else word = upper;
            ++cursor;
            distance = (word >> 8) & 127u;
            if (word & 0x8000u) { distance = (distance << 8) | ((word >> 16) & 255u); ++cursor; }
            amount = (word & 255u) + 3u;
            remaining -= (sint32)amount;
            if (remaining < 0) amount = (uint32)((sint32)amount + remaining);
            ++distance;
            if (distance >= 4u) while (amount >= 4u) {
                uint8 bytes[4];
                memcpy(bytes, lz_read_span(destination, destination_capacity,
                    (uintptr_t)destination + produced - distance, 4u), 4u);
                memcpy(destination + produced, bytes, 4u);
                produced += 4u; amount -= 4u;
            }
            for (i = 0; i < amount; ++i) {
                uint8 value = *lz_read_span(destination, destination_capacity,
                    (uintptr_t)destination + produced - distance, 1u);
                destination[produced++] = value;
            }
        }
        bit = (bit + 1u) & 7u;
    }
    return destination;
}

static uint8 *cd_file_load_native(uint32 guest_path, uint8 *destination, uint32 capacity,
    uint32 requested_bytes, uint32 decoded_bytes, uint32 *guest_address)
{
    uint8 descriptor[24];
    uint32 file_size, sector, allocated = 0, allocation_size;
    sint32 status;
    if (gtmain_cd_file_query(guest_path, descriptor) == -1) return NULL;
    file_size = (uint32)descriptor[4] | ((uint32)descriptor[5] << 8) |
        ((uint32)descriptor[6] << 16) | ((uint32)descriptor[7] << 24);
    if (!requested_bytes) requested_bytes = decoded_bytes ? decoded_bytes : file_size;
    allocation_size = requested_bytes;
    if (!destination) {
        *guest_address = gtmain_heap_alloc(allocation_size); allocated = 1;
        if (!*guest_address) { w_u32(0x800B1AB0u, 12u); return NULL; }
        capacity = (allocation_size + 15u) & ~15u;
        destination = psx_addr(*guest_address, capacity);
    } else if (*guest_address) {
        /* Preserve the original supplied guest buffer caller contract */
        capacity = (allocation_size + 15u) & ~15u;
        destination = psx_addr(*guest_address, capacity);
    }
    if (file_size < requested_bytes) requested_bytes = file_size;
    sector = (uint32)CdPosToInt((CdlLOC *)descriptor);
    if (decoded_bytes) status = gtmain_cd_file_decode(destination, capacity,
        sector << 11, requested_bytes, decoded_bytes);
    else status = gtmain_cd_file_read(destination, capacity, sector, requested_bytes);
    if (status == -1 && allocated) {
        /* TODO Bind the unrecorded original heap free dependency */
        gt_draft_failure("Unrecorded GTMAIN heap free", 0x8007CFE0u);
        destination = NULL; *guest_address = 0;
    }
    return r_u32(0x800B1AB0u) ? NULL : destination;
}

/* GTMAIN.EXE 80071844 */
uint32 gtmain_cd_file_load(uint32 guest_path, uint32 guest_destination,
    uint32 requested_bytes, uint32 decoded_bytes)
{
    uint32 capacity = (requested_bytes + 15u) & ~15u;
    uint8 *destination = guest_destination ? psx_addr(guest_destination, 1u) : NULL;
    return cd_file_load_native(guest_path, destination, capacity, requested_bytes, decoded_bytes,
        &guest_destination) ? guest_destination : 0;
}

uint8 *gtmain_cd_file_load_native(uint32 guest_path, uint8 *destination, uint32 capacity,
    uint32 requested_bytes, uint32 decoded_bytes)
{
    uint32 guest_address = 0;
    return cd_file_load_native(guest_path, destination, capacity, requested_bytes, decoded_bytes, &guest_address);
}
