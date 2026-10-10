#include "game_draft_registry.h"
#include <stdio.h>
#include <stdlib.h>

static uint32 gtmain_skip_records(uint32 address, uint32 count, uint32 stride)
{
    while (count) {
        uint32 size = r_u8(address);
        --count;
        address += size * stride + 1u;
    }
    return address;
}

uint32 gtmain_resource_init(uint32 path, uint32 buffer, uint32 capacity)
{
    size_t size = 0u;
    /* Load the extracted original file through the host filesystem */
    if (!buffer || !xport_cd_file_read(path, "DATA", buffer, capacity, &size)) {
        w_u32(0x800B1AB0u, 5u);
        return gt_draft_unresolved_call(0x80071844u, path, buffer, capacity, 0u, 0u, 0u, 0u, 0u);
    }
    w_u32(0x800B1AB0u, 0u);
    return gt_resource_relocate(buffer);
}

uint32 gt_resource_relocate(uint32 buffer)
{
    uint32 current, count, next_count;
    count = (uint32)(sint32)(sint16)r_u16(buffer + 12u);
    current = (buffer + 75u) & ~3u;
    w_u32(buffer + 24u, current);
    current = (gtmain_skip_records(current, count, 1u) + 3u) & ~3u;
    w_u32(buffer + 28u, current);
    current = (current + count * 16u + 3u) & ~3u;
    w_u32(buffer + 32u, current);
    next_count = (uint32)(sint32)(sint16)r_u16(buffer + 14u);
    current = (gtmain_skip_records(current, count, 1u) + 3u) & ~3u;
    w_u32(buffer + 36u, current);
    current = (current + next_count * 6u + 32u + 3u) & ~3u;
    w_u32(buffer + 40u, current);
    current = (gtmain_skip_records(current, next_count, 2u) + 3u) & ~3u;
    w_u32(buffer + 44u, current);
    count = (uint32)(sint32)(sint16)r_u16(buffer + 16u);
    current = (gtmain_skip_records(current, next_count, 1u) + 3u) & ~3u;
    w_u32(buffer + 48u, current);
    current = (current + count * 2u + 3u) & ~3u;
    w_u32(buffer + 52u, current);
    count = (uint32)(sint32)(sint16)r_u16(buffer + 18u);
    current = (gtmain_skip_records(current, count, 1u) + 3u) & ~3u;
    w_u32(buffer + 56u, current);
    current = (current + count * 8u + 3u) & ~3u;
    w_u32(buffer + 60u, current);
    next_count = (uint32)(sint32)(sint16)r_u16(buffer + 20u);
    current = (gtmain_skip_records(current, count, 1u) + 3u) & ~3u;
    w_u32(buffer + 64u, current);
    w_u32(buffer + 68u, (current + next_count * 8u + 3u) & ~3u);
    return 1u;
}


static uint32 gtmain_host_u32(const uint8 *data)
{
    return (uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24);
}

uint32 gt_archive_section(FILE *file, const uint8 *header, uint32 header_bytes, uint32 index, uint32 destination, uint32 gtos)
{
    uint32 offset, stored, bytes, i;
    uint32 compressed = header[13] & 0x80u;
    uint32 count = (uint32)header[14] | ((uint32)header[15] << 8);
    uint8 *input;
    if (index >= count || 16u + (index + 1u) * 12u > header_bytes)
        goto failed;
    offset = gtmain_host_u32(header + 16u + index * 12u);
    stored = gtmain_host_u32(header + 20u + index * 12u);
    bytes = gtmain_host_u32(header + 24u + index * 12u);
    if (!compressed) {
        offset &= ~2047u;
        if (fseek(file, (long)offset, SEEK_SET) || fread(psx_addr(destination, bytes), 1u, bytes, file) != bytes)
            goto failed;
    } else {
        uint32 cursor = 0u, output = 0u;
        input = (uint8 *)malloc(stored ? stored : 1u);
        if (!input) goto failed;
        if (fseek(file, (long)offset, SEEK_SET) || fread(input, 1u, stored, file) != stored) {
            free(input);
            goto failed;
        }
        while (output < bytes) {
            uint32 flags;
            if (cursor >= stored) goto decode_failed;
            flags = input[cursor++];
            for (i = 0u; i < 8u && output < bytes; ++i, flags >>= 1) {
                if (flags & 1u) {
                    uint32 length, back, part;
                    if (stored - cursor < 2u) goto decode_failed;
                    length = input[cursor++] + 3u;
                    part = input[cursor++];
                    back = part & 127u;
                    if (part & 128u) {
                        if (cursor >= stored) goto decode_failed;
                        back = (back << 8) | input[cursor++];
                    }
                    ++back;
                    if (length > bytes - output) length = bytes - output;
                    while (length--) {
                        w_u8(destination + output, r_u8(destination + output - back));
                        ++output;
                    }
                } else {
                    if (cursor >= stored) goto decode_failed;
                    w_u8(destination + output++, input[cursor++]);
                }
            }
        }
        free(input);
        w_u8(gtos ? 0x8004F6A4u : 0x800994C0u, 1u);
    }
    w_u32(gtos ? 0x80073AC0u : 0x800B1AB0u, 0u);
    return destination;
decode_failed:
    free(input);
failed:
    w_u32(gtos ? 0x80073AC0u : 0x800B1AB0u, 5u);
    return gt_draft_unresolved_call(gtos ? 0x8002B85Cu : 0x8006AE5Cu, 0x8008C01Cu, index, destination, 0u, 0u, 0u, 0u, 0u);
}

static uint32 gtmain_archive_section(FILE *file, const uint8 *header, uint32 header_bytes, uint32 index, uint32 destination)
{
    return gt_archive_section(file, header, header_bytes, index, destination, 0u);
}

uint32 gtmain_messages_init(void)
{
    uint8 header[2048];
    uint32 language = r_u8(0x800C4C3Du);
    uint32 result;
    FILE *file = NULL;
    if (fopen_s(&file, "DATA/MESSAGES.DAT", "rb") || !file)
        return gt_draft_unresolved_call(0x8006AD20u, 0x8008C01Cu, 0u, 0u, 2048u, 0u, 0u, 0u, 0u);
    if (fread(header, 1u, sizeof(header), file) != sizeof(header)) {
        fclose(file);
        return gt_draft_unresolved_call(0x8006AD20u, 0x8008C01Cu, 0u, 0u, 2048u, 0u, 0u, 0u, 0u);
    }
    /* Preserve the destination arguments recovered from MIPS */
    gtmain_archive_section(file, header, sizeof(header), language, 0x800BD7F0u);
    gtmain_archive_section(file, header, sizeof(header), language + 6u, 0x800BC300u);
    result = 1026u;
    if (r_u16(0x800CB7F4u) == 1026u)
        result = gtmain_archive_section(file, header, sizeof(header), language + 18u, 0x8019CFDCu);
    fclose(file);
    return result;
}

uint32 gtmain_file_load(uint32 path, uint32 destination, uint32 stored_limit, uint32 output_limit)
{
    size_t bytes = 0u, received = 0u;
    uint32 capacity;
    if (output_limit) {
        /* TODO Support decompression requested by other file load callers */
        return gt_draft_unresolved_call(0x8006A21Cu, path, destination, stored_limit, output_limit, 0u, 0u, 0u, 0u);
    }
    if (!xport_cd_file_size(path, "DATA", &bytes) || bytes > 0x200000u) goto failed;
    capacity = stored_limit ? stored_limit : (uint32)bytes;
    if (capacity > bytes) capacity = (uint32)bytes;
    if (!destination) destination = gtmain_heap_alloc(capacity);
    if (!destination || (destination & 0x1FFFFFFFu) >= 0x200000u || capacity > 0x200000u - (destination & 0x1FFFFFFFu)) goto failed;
    if (!xport_cd_file_read(path, "DATA", destination, capacity, &received) || received != capacity) goto failed;
    w_u32(0x800B1AB0u, 0u);
    return destination;
failed:
    w_u32(0x800B1AB0u, 5u);
    return gt_draft_unresolved_call(0x8006A21Cu, path, destination, stored_limit, output_limit, 0u, 0u, 0u, 0u);
}

uint32 gt_sound_bank_relocate(uint32 bank, uint32 spu_address)
{
    uint32 delta, i, count;
    if (r_u32(bank) != 0x54534E49u) return 0u;
    delta = bank - r_u32(bank + 4u);
    w_u32(bank + 4u, bank);
    w_u32(bank + 20u, r_u32(bank + 20u) + delta);
    w_u32(bank + 28u, r_u32(bank + 28u) + delta);
    w_u32(bank + 36u, r_u32(bank + 36u) + delta);
    count = r_u32(bank + 32u);
    for (i = 0u; i < count; ++i) {
        uint32 slot = r_u32(bank + 36u) + i * 4u;
        w_u32(slot, r_u32(slot) + delta);
    }
    if (spu_address) {
        delta = (spu_address - r_u32(bank + 12u)) >> 3u;
        w_u32(bank + 12u, spu_address);
        count = r_u32(bank + 24u);
        for (i = 0u; i < count; ++i) {
            uint32 sample = r_u32(bank + 28u) + i * 20u;
            w_u16(sample, r_u16(sample) + delta);
        }
    }
    return bank;
}

uint32 gtmain_sound_bank_load(uint32 destination, uint32 spu_address, uint32 buffer, uint32 capacity, uint32 section)
{
    uint8 *header;
    uint8 first[16];
    uint32 header_bytes, metadata_bytes, sample_bytes;
    uint32 entry = r_u32(0x800C0C34u), path_address = 0u;
    char path[260];
    FILE *file = NULL;
    while (r_u8(entry)) {
        uint32 length = 0u, i;
        while (r_u8(entry + length)) ++length;
        if (length > 5u && r_u8(entry + 5u) == '=') {
            for (i = 0u; i < 5u; ++i)
                if (r_u8(entry + i) != (uint8)"sound"[i]) break;
            if (i == 5u) { path_address = entry + 6u; break; }
        }
        entry += length + 1u;
    }
    if (!path_address || sprintf_s(path, sizeof(path), "DATA/%s", (const char *)psx_addr(path_address, 1u)) < 0) goto failed;
    if (fopen_s(&file, path, "rb") || !file) goto failed;
    if (fread(first, 1u, sizeof(first), file) != sizeof(first)) { fclose(file); goto failed; }
    header_bytes = 16u + ((uint32)first[14] | ((uint32)first[15] << 8u)) * 12u;
    header = (uint8 *)malloc(header_bytes);
    if (!header) { fclose(file); goto failed; }
    if (fseek(file, 0, SEEK_SET) || fread(header, 1u, header_bytes, file) != header_bytes) {
        free(header); fclose(file); goto failed;
    }
    gtmain_archive_section(file, header, header_bytes, section, buffer);
    free(header);
    fclose(file);
    metadata_bytes = r_u32(buffer + 20u);
    sample_bytes = r_u32(buffer + 16u);
    if (metadata_bytes > capacity || r_u32(buffer) != 0x54534E49u) goto failed;
    {
        uint32 i;
        for (i = 0u; i < metadata_bytes; ++i) w_u8(destination + i, r_u8(buffer + i));
    }
    gt_sound_bank_relocate(buffer, spu_address);
    gt_sound_bank_relocate(destination, spu_address);
    if (sample_bytes) {
        uint32 address = r_u32(destination + 12u);
        w_u32(0x800B235Cu, address >> 3u);
        w_u32(0x800B23B4u, 0u);
        w_u32(0x800B2360u, 0u);
        if (SpuWrite(psx_addr(r_u32(buffer + 20u), sample_bytes), sample_bytes) != sample_bytes) goto failed;
    }
    return destination;
failed:
    return gt_draft_unresolved_call(0x80070DD8u, destination, spu_address, buffer, capacity, section, 0u, 0u, 0u);
}
