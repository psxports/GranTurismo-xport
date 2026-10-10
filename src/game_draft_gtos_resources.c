#include "game_draft_registry.h"
#include <string.h>

void gtos_music_tick(void)
{
    uint8 header[8] = { 0 }, response[8] = { 0 };
    CdlLOC location;
    uint32 current;
    if (r_u8(0x800762E1u) != 1u) return;
    if (!r_u8(0x8004F6A6u)) {
        if (r_u32(0x8004F6B8u)) {
            gtos_cd_stream_start();
            w_u8(0x8004F6A6u, 16u);
            printf("restart xa\n");
        }
        return;
    }
    /* Poll the shared native reader instead of guest CD interrupts */
    if (r_u8(0x8004F6A6u) != 16u || !CdControl(16u, NULL, header)) return;
    memcpy(&location, header, 3u);
    location.track = 0u;
    current = ((uint32)CdPosToInt(&location) & ~r_u32(0x8007A168u)) |
        (r_u32(0x8007A16Cu) & r_u32(0x8007A168u));
    w_u32(0x8007A164u, current);
    if (current < r_u32(0x8007A170u)) return;
    CdControl(9u, NULL, response);
    w_u32(0x8007A164u, r_u32(0x8007A16Cu));
    w_u8(0x8004F6A4u, 1u);
    w_u8(0x8004F6A6u, 0u);
    if (r_u16(0x800671F0u)) {
        gtos_cd_stream_start();
        w_u8(0x8004F6A6u, 16u);
    } else w_u8(0x800762E1u, 0u);
}

void gtos_music_prepare(uint32 name_index, uint32 repeat)
{
    uint32 wanted = r_u32(0x8004FAD8u + 4u * name_index);
    uint32 name = r_u32(0x801B16F4u), index, entry, start, bytes;
    sint32 count = (sint16)r_u16(0x801B16D2u), found = -1;
    for (index = 0u; (sint32)index < count; ++index) {
        uint32 text = name + 1u, offset = 0u;
        while (r_u8(wanted + offset) == r_u8(text + offset)) {
            if (!r_u8(wanted + offset)) { found = (sint32)index; break; }
            ++offset;
        }
        if (found >= 0) break;
        name += r_u8(name) + 1u;
    }
    if (found < 0) {
        /* TODO Preserve the original failure path for missing music names */
        gt_draft_unresolved_call(0x800307FCu, wanted, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    gtos_cd_mode(0u);
    entry = r_u32(0x801B16F8u) + 8u * (uint32)found;
    start = r_u32(entry) + r_u32(0x8004F6B4u);
    bytes = r_u32(entry + 4u) << 11u;
    w_u32(0x8004F6B8u, start);
    w_u32(0x8004F6BCu, bytes);
    w_u8(0x8007A174u, 0x68u);
    w_u8(0x800671F4u, 1u);
    w_u8(0x800671F5u, (uint32)found & 3u);
    w_u16(0x800671F0u, repeat);
    w_u32(0x8007A16Cu, start);
    w_u32(0x8007A170u, start + ((bytes + 2047u) >> 11u));
    w_u32(0x8007A168u, 3u);
    w_u32(0x8007A164u, start);
}

void gtos_music_location_init(void)
{
    uint32 entry = r_u32(0x801B1704u), path = 0u;
    CdlFILE file;
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 1u);
    if (!(sint16)r_u16(0x801B16D2u)) return;
    while (r_u8(entry)) {
        uint32 length = 0u;
        while (r_u8(entry + length)) ++length;
        if (length > 10u && !memcmp(psx_addr(entry, 11u), "music.file=", 11u)) {
            path = entry + 11u;
            break;
        }
        entry += length + 1u;
    }
    if (!path || !CdSearchFile(&file, (const char *)psx_addr(path, 1u))) {
        w_u32(0x80073AC0u, 2u);
        gt_draft_unresolved_call(0x80029D78u, path, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    w_u32(0x80073AC0u, 0u);
    w_u32(0x8004F6B4u, (uint32)CdPosToInt(&file.pos));
}

static void gtos_saved_defaults(uint32 destination)
{
    if (r_u8(destination)) return;
    w_u8(destination, 1u);
    memcpy(psx_addr(destination + 1u, 9u), psx_addr(0x8005237Cu, 9u), 9u);
    memcpy(psx_addr(destination + 10u, 16u), psx_addr(0x80052388u, 16u), 16u);
    memcpy(psx_addr(destination + 26u, 15u), psx_addr(0x80052398u, 15u), 15u);
}

void gtos_resources_init(void)
{
    size_t size = 0u;
    CdlFILE stream;
    w_u8(0x80081795u, 0u);
    gtos_saved_defaults(0x80081722u);
    gtos_saved_defaults(0x8008174Bu);
    if (!xport_cd_file_read(0x8004B2E0u, "DATA", 0x801B16C0u, 0x4000u, &size)) {
        w_u32(0x80073AC0u, 5u);
        gt_draft_unresolved_call(0x80029DD4u, 0x8004B2E0u, 0x801B16C0u, 0x4000u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    w_u32(0x80073AC0u, 0u);
    gt_resource_relocate(0x801B16C0u);
    /* Resolve the actual stream location from the mounted original disc */
    if (!CdSearchFile(&stream, "stream.dat")) {
        w_u32(0x80073AC0u, 2u);
        gt_draft_unresolved_call(0x80029CF8u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    w_u32(0x8004F6B0u, (uint32)CdPosToInt(&stream.pos));
}
