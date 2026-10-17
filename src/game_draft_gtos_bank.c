#include "game_draft_registry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void gtos_sound_bank_section(uint32 destination, uint32 spu_address, uint32 buffer, uint32 capacity, uint32 section)
{
    uint32 entry = r_u32(0x801B1704u), path_address = 0u;
    uint32 header_bytes, metadata, samples;
    uint8 first[16], *header;
    char path[260];
    FILE *file = NULL;
    while (r_u8(entry)) {
        uint32 length = 0u;
        while (r_u8(entry + length)) ++length;
        if (length > 5u && !memcmp(psx_addr(entry, 6u), "sound=", 6u)) {
            path_address = entry + 6u;
            break;
        }
        entry += length + 1u;
    }
    if (!path_address || sprintf_s(path, sizeof(path), "DATA/%s", (const char *)psx_addr(path_address, 1u)) < 0) goto failed;
    if (fopen_s(&file, path, "rb") || !file) goto failed;
    if (fread(first, 1u, 16u, file) != 16u) { fclose(file); goto failed; }
    header_bytes = 16u + ((uint32)first[14] | ((uint32)first[15] << 8u)) * 12u;
    header = (uint8 *)malloc(header_bytes);
    if (!header) { fclose(file); goto failed; }
    if (fseek(file, 0, SEEK_SET) || fread(header, 1u, header_bytes, file) != header_bytes) {
        free(header); fclose(file); goto failed;
    }
    gt_archive_section(file, header, header_bytes, section, buffer, 1u);
    free(header);
    fclose(file);
    metadata = r_u32(buffer + 20u);
    samples = r_u32(buffer + 16u);
    if (metadata > capacity || r_u32(buffer) != 0x54534E49u) goto failed;
    memcpy(psx_addr(destination, metadata), psx_addr(buffer, metadata), metadata);
    gt_sound_bank_relocate(buffer, spu_address);
    gt_sound_bank_relocate(destination, spu_address);
    if (samples) {
        w_u32(0x8007412Cu, r_u32(destination + 12u) >> 3u);
        w_u32(0x80074184u, 0u);
        w_u32(0x80074130u, 0u);
        if (SpuWrite(psx_addr(r_u32(buffer + 20u), samples), samples) != samples) goto failed;
    }
    return;
failed:
    gt_draft_unresolved_call(0x8002AE80u, destination, spu_address, buffer, capacity, section, 0u, 0u, 0u);
}

void gtos_sound_bank_load(void)
{
    gtos_sound_bank_section(0x800757A8u, 0x1010u, 0x801716ACu, 2048u, 0u);
}

void gtos_arcade_sound_bank_load(void)
{
    gtos_sound_bank_section(0x8009777Cu, 0x2C800u, 0x8009E77Cu, 4096u, 2u);
    w_u32(0x80075FE4u, 0x8009777Cu);
}
