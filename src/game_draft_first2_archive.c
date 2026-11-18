#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_sound_stack.h"

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

static uint32 gtmain_archive_sector(const uint8 descriptor[24])
{
    return (uint32)CdPosToInt((CdlLOC *)(void *)descriptor);
}

static uint32 archive_stack_sector(uint32 offset)
{
    uint32 launch = r_u32(0x8008F1D8u), alternate = r_u32(0x800B1AA4u);
    uint32 baseline = alternate ? alternate : launch;
    uint32 top = gt_local_stack_top();
    uint32 reserve = r_u32(0x800B1B4Cu);
    uint32 heap = r_u32(0x800B1AACu), bytes = r_u32(0x800B1AA8u);
    uint32 extent = bytes < 16u ? 16u : bytes, address;
    /* Archive locals reuse stack storage after the loading frame has returned */
    if (top < 0x80010018u + offset || top > 0x80200000u || (top & 7u) || offset < 2048u)
        gt_draft_failure("GTMAIN archive stack range", 0x8006AD20u);
    address = top - offset;
    if (top != baseline && address - 24u < baseline - 0x41D8u)
        gt_draft_failure("GTMAIN nested archive stack overlap", 0x8006AD20u);
    if (address - 24u < 0x801FB724u || heap < 0x80010000u || heap > address - 24u ||
        extent > address - 24u - heap || (!alternate && (reserve < offset + 24u || address - 24u < launch - reserve)))
        gt_draft_failure("GTMAIN archive stack overlap", 0x8006AD20u);
    return address;
}

static GTMainArchiveTable archive_table_load_into(uint32 path, uint32 *sector, uint32 sector_guest,
    uint32 sector_is_guest, uint8 *buffer, uint32 buffer_guest, uint32 capacity)
{
    GTMainArchiveTable result = {buffer, buffer_guest};
    uint32 temporary_address = archive_stack_sector(0x1800u);
    uint8 *temporary = (uint8 *)psx_addr(temporary_address, 2048u);
    uint8 *descriptor = (uint8 *)psx_addr(temporary_address - 24u, 24u);
    uint32 current_sector, bytes;
    uint32 provided = buffer && capacity;
    for (;;)
    {
        if (sector_is_guest) current_sector = r_u32(sector_guest);
        else if (!path) current_sector = *sector;
        if (path)
        {
            if (gtmain_cd_file_query(path, descriptor) == -1) continue;
            current_sector = gtmain_archive_sector(descriptor);
            if (sector_is_guest) w_u32(sector_guest, current_sector);
            else *sector = current_sector;
        }
        if (provided)
        {
            if (gtmain_cd_file_read(result.bytes, capacity, current_sector, capacity) == -1) continue;
            return result;
        }
        if (gtmain_cd_file_read(temporary, 2048u, current_sector, 2048u) == -1) continue;
        bytes = (uint32)(sint32)(sint16)xport_load_le16(temporary + 14u) * 12u + 16u;
        result.guest_address = gtmain_heap_alloc(bytes);
        if (!result.guest_address) { result.bytes = NULL; return result; }
        result.bytes = (uint8 *)psx_addr(result.guest_address, bytes);
        gtmain_memory_copy(result.guest_address, temporary_address, bytes);
        if (bytes <= 2048u) return result;
        if (gtmain_cd_file_read(result.bytes + 2048u, bytes - 2048u, current_sector + 1u, bytes - 2048u) == -1) continue;
        return result;
    }
}

/* Original: GTMAIN.EXE:sub_8006AD20. */ GTMainArchiveTable gtmain_archive_table_load(uint32 path, uint32 *sector, uint8 *buffer, uint32 buffer_guest, uint32 capacity)
{
    if (!sector) gt_draft_failure("Native archive sector output unavailable", 0x8006AD20u);
    return archive_table_load_into(path, sector, 0u, 0u, buffer, buffer_guest, capacity);
}

uint32 gtmain_archive_table_load_guest(uint32 path, uint32 sector_out, uint32 buffer, uint32 capacity)
{
    GTMainArchiveTable result;
    result = archive_table_load_into(path, NULL, sector_out, 1u,
        buffer ? (uint8 *)psx_addr(buffer, capacity) : NULL, buffer, capacity);
    return result.guest_address;
}

/* Original: GTMAIN.EXE:sub_8006AE5C. */ uint32 gtmain_archive_section_load(uint32 path, const uint8 *table, uint32 sector, uint32 index, uint32 destination, uint32 limit)
{
    uint32 temporary_address = archive_stack_sector(0x2100u);
    uint8 *temporary = (uint8 *)psx_addr(temporary_address, 2048u);
    uint8 *descriptor = (uint8 *)psx_addr(temporary_address - 24u, 24u);
    const uint8 *section;
    uint32 fallback_displacement, cached_offset, fallback_cached = 0u;
    uint32 section_offset = index * 12u, file_offset, compressed, bytes, aligned, flags;
    for (;;)
    {
        if (gtmain_cd_file_query(path, descriptor) == -1) continue;
        if (!table && !fallback_cached)
        {
            sint32 offset = (sint32)(section_offset + 16u);
            sint32 chunk = offset / 2048;
            uint32 displacement = (uint32)chunk << 11;
            sector = gtmain_archive_sector(descriptor);
            if (gtmain_cd_file_read(temporary, 2048u, sector + (uint32)chunk, 2048u) == -1) continue;
            cached_offset = section_offset - displacement;
            fallback_displacement = displacement;
            fallback_cached = 1u;
        }
        if (table) {
            section = table + section_offset;
            bytes = xport_load_le32(section + 24u);
            file_offset = xport_load_le32(section + 16u);
            compressed = xport_load_le32(section + 20u);
        } else {
            bytes = r_u32(temporary_address + cached_offset + 24u);
            file_offset = r_u32(temporary_address + cached_offset + 16u);
            compressed = r_u32(temporary_address + cached_offset + 20u);
        }
        if (limit && limit < bytes) bytes = limit;
        if (!destination)
        {
            destination = gtmain_heap_alloc(bytes);
            if (!destination) return 0;
        }
        if (table) flags = xport_load_le16(table + 12u);
        else
        {
            flags = r_u16(temporary_address + 12u - fallback_displacement);
        }
        aligned = (bytes + 3u) & ~3u;
        if (flags & 0x8000u)
        {
            if (gtmain_cd_file_decode((uint8 *)psx_addr(destination, bytes), bytes,
                (sector << 11) + file_offset, compressed, bytes) == -1) continue;
        }
        else if (gtmain_cd_file_read((uint8 *)psx_addr(destination, aligned), aligned,
            sector + (file_offset >> 11), bytes) == -1) continue;
        return destination;
    }
}

/* Original: GTMAIN.EXE:sub_80070DD8. */ uint32 gtmain_sound_bank_load_original(uint32 destination, uint32 spu_base, uint32 buffer, uint32 mode)
{
    uint32 temporary = gt_sound_stack_workspace(0x3000u, 2052u, 0x80070DD8u);
    uint32 table;
    uint32 path = gtmain_sound_path(), image, relocated, address, source, size;
    table = gtmain_archive_table_load_guest(path, temporary + 2048u, temporary, 2048u);
    image = gtmain_archive_section_load(path, table ? (uint8 *)psx_addr(table, 1u) : NULL,
        r_u32(temporary + 2048u), mode, buffer, 0u);
    gtmain_memory_copy(destination, image, r_u32(image + 20u));
    gtmain_sound_instrument_relocate(image, spu_base);
    relocated = gtmain_sound_instrument_relocate(destination, spu_base);
    address = r_u32(destination + 12u);
    source = r_u32(image + 20u);
    size = r_u32(image + 16u);
    return gtmain_sound_sample_upload(address, source, size, relocated);
}
