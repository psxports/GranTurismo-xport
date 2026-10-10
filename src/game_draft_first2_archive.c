#include "game_first2_registry.h"
#include "game_draft_registry.h"

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

static uint32 gtmain_archive_sector(const uint8 descriptor[24])
{
    return (uint32)CdPosToInt((CdlLOC *)(void *)descriptor);
}

GTMainArchiveTable gtmain_archive_table_load(uint32 path, uint32 *sector, uint8 *buffer, uint32 buffer_guest, uint32 capacity)
{
    GTMainArchiveTable result = {buffer, buffer_guest};
    uint8 descriptor[24], temporary[2048];
    uint32 current_sector, bytes;
    uint32 provided = buffer && capacity;
    for (;;)
    {
        if (path)
        {
            if (gtmain_cd_file_query(path, descriptor) == -1) continue;
            current_sector = gtmain_archive_sector(descriptor);
            *sector = current_sector;
        }
        else current_sector = *sector;
        if (provided)
        {
            if (gtmain_cd_file_read(result.bytes, capacity, current_sector, capacity) == -1) continue;
            return result;
        }
        if (gtmain_cd_file_read(temporary, sizeof(temporary), current_sector, sizeof(temporary)) == -1) continue;
        bytes = (uint32)(sint32)(sint16)xport_load_le16(temporary + 14u) * 12u + 16u;
        result.guest_address = gtmain_heap_alloc(bytes);
        if (!result.guest_address) { result.bytes = NULL; return result; }
        result.bytes = (uint8 *)psx_addr(result.guest_address, bytes);
        /* TODO: The original oversized copy consumes bytes beyond its local header */
        if (bytes > sizeof(temporary)) gt_draft_failure("GTMAIN archive table local copy extent", 0x8006AE00u);
        {
            uint32 index;
            for (index = 0u; index < bytes; ++index) result.bytes[index] = temporary[index];
        }
        if (bytes <= 2048u) return result;
        if (gtmain_cd_file_read(result.bytes + 2048u, bytes - 2048u, current_sector + 1u, bytes - 2048u) == -1) continue;
        return result;
    }
}

uint32 gtmain_archive_table_load_guest(uint32 path, uint32 sector_out, uint32 buffer, uint32 capacity)
{
    uint32 sector = r_u32(sector_out);
    GTMainArchiveTable result = gtmain_archive_table_load(path, &sector,
        buffer ? (uint8 *)psx_addr(buffer, capacity) : NULL, buffer, capacity);
    w_u32(sector_out, sector);
    return result.guest_address;
}

uint32 gtmain_archive_section_load(uint32 path, const uint8 *table, uint32 sector, uint32 index, uint32 destination, uint32 limit)
{
    uint8 descriptor[24], temporary[2048];
    const uint8 *section;
    uint32 section_offset = index * 12u, file_offset, compressed, bytes, aligned, flags;
    for (;;)
    {
        if (gtmain_cd_file_query(path, descriptor) == -1) continue;
        if (table) section = table + section_offset;
        else
        {
            sint32 offset = (sint32)(section_offset + 16u);
            sint32 chunk = offset / 2048;
            uint32 displacement = (uint32)chunk << 11;
            sector = gtmain_archive_sector(descriptor);
            if (gtmain_cd_file_read(temporary, sizeof(temporary), sector + (uint32)chunk, sizeof(temporary)) == -1) continue;
            /* TODO: The original fallback reads header flags before its local sector for nonzero chunks */
            if (chunk || section_offset > sizeof(temporary) - 28u)
                gt_draft_failure("GTMAIN archive fallback header extent", 0x8006AF44u);
            section = temporary + section_offset - displacement;
            table = temporary;
        }
        bytes = xport_load_le32(section + 24u);
        file_offset = xport_load_le32(section + 16u);
        compressed = xport_load_le32(section + 20u);
        if (limit && limit < bytes) bytes = limit;
        if (!destination)
        {
            destination = gtmain_heap_alloc(bytes);
            if (!destination) return 0;
        }
        flags = xport_load_le16(table + 12u);
        aligned = (bytes + 3u) & ~3u;
        if (!aligned && bytes) gt_draft_failure("GTMAIN archive transfer size wrap", 0x80071AE0u);
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

void gtmain_sound_bank_load_original(uint32 destination, uint32 spu_base, uint32 buffer, uint32 capacity, uint32 mode)
{
    uint8 temporary[2048];
    uint32 sector = 0u;
    uint32 path = gtmain_sound_path(), image;
    GTMainArchiveTable table = gtmain_archive_table_load(path, &sector, temporary, 0u, sizeof(temporary));
    image = gtmain_archive_section_load(path, table.bytes, sector, mode, buffer, 0u);
    gtmain_memory_copy(destination, image, r_u32(image + 20u));
    gtmain_sound_instrument_relocate(image, spu_base);
    gtmain_sound_instrument_relocate(destination, spu_base);
    gtmain_sound_sample_upload(r_u32(destination + 12u), r_u32(image + 20u), r_u32(image + 16u));
}
