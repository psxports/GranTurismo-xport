#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_spu.h"

static uint8 decode_ring[32768];
static uint8 *decode_sectors[16], *decode_target;

/* GTMAIN.EXE 80071B5C */
sint32 gtmain_cd_decode_ready(void)
{
    uint32 next, available, remaining;
    uint8 *target;
    gtmain_cd_sector_native(decode_target, 512);
    next = r_u32(0x800B55A8u);
    available = r_u32(0x800B55ACu);
    remaining = r_u32(0x800B55B0u);
    ++next;
    if ((sint32)next >= 16) next = 0u;
    ++available;
    if (next >= 16u)
        gt_draft_failure("Native CD decode sector table extent", 0x80071BB4u);
    target = decode_sectors[next];
    --remaining;
    w_u32(0x800B55A8u, next);
    w_u32(0x800B55ACu, available);
    w_u32(0x800B55B0u, remaining);
    decode_target = target;
    return (sint32)available >= 16 || !remaining;
}

/* GTMAIN.EXE 80071C04 */
sint32 gtmain_cd_decode_wait(void)
{
    uint32 available;
    EnterCriticalSectionPSX();
    available = r_u32(0x800B55ACu);
    if (available) w_u32(0x800B55ACu, available - 1u);
    ExitCriticalSection();
    while (!r_u32(0x800B55ACu)) {
        if (gtmain_cd_stream_idle()) {
            if (r_u8(0x800994C1u) == 4u) return -1;
            gtmain_cd_stream_callback(0x80071B5Cu);
            gtmain_cd_stream_start(r_u32(0x800C0A50u));
        }
    }
    return 0;
}

static sint32 decode_byte(uint32 *cursor, uint8 *value, uint32 force_wait)
{
    if ((force_wait || !(*cursor & 2047u)) && gtmain_cd_decode_wait() < 0) return -1;
    *value = decode_ring[*cursor];
    *cursor = (*cursor + 1u) & 32767u;
    return 0;
}

/* GTMAIN.EXE 80071CA4 */
sint32 gtmain_cd_file_decode(uint8 *destination, uint32 capacity, uint32 byte_offset,
    uint32 compressed_bytes, uint32 output_bytes)
{
    uint32 cursor = byte_offset & 2047u, produced = 0, index, remaining;
    sint32 result = -1;
    if (output_bytes > capacity || (!destination && output_bytes))
        gt_draft_failure("Native CD decode destination bound", 0x80071CA4u);
    for (index = 0u; index < 16u; ++index)
        decode_sectors[index] = decode_ring + index * 2048u;
    decode_target = decode_ring;
    remaining = (cursor + compressed_bytes + 2047u) >> 11;
    w_u32(0x800B55A8u, 0); w_u32(0x800B55ACu, 0); w_u32(0x800B55B0u, remaining);
    gtmain_cd_stream_callback(0x80071B5Cu);
    gtmain_cd_stream_start(byte_offset >> 11);
    w_u32(0x800B1AB0u, 5u);
    while (produced < output_bytes) {
        uint8 flags;
        uint32 bit;
        if (decode_byte(&cursor, &flags, produced == 0u) < 0) goto Finish;
        for (bit = 0; bit < 8u && produced < output_bytes; ++bit, flags >>= 1) {
            uint8 first, second;
            uint32 distance, length, i;
            if (decode_byte(&cursor, &first, 0) < 0) goto Finish;
            if (!(flags & 1u)) { destination[produced++] = first; continue; }
            if (decode_byte(&cursor, &second, 0) < 0) goto Finish;
            distance = second & 127u;
            if (second & 128u) {
                uint8 low;
                if (decode_byte(&cursor, &low, 0) < 0) goto Finish;
                distance = (distance << 8) | low;
            }
            ++distance; length = (uint32)first + 3u;
            if (length > output_bytes - produced) length = output_bytes - produced;
            if (distance > produced) gt_draft_failure("Native CD decode backreference bound", 0x80071CA4u);
            for (i = 0; i < length; ++i) {
                uint8 value = destination[produced - distance];
                destination[produced++] = value;
            }
        }
    }
    result = 0; w_u32(0x800B1AB0u, 0);
Finish:
    w_u8(0x800994C0u, 1u);
    gtmain_cd_stream_wait();
    return result;
}

static uint32 cd_path_resolve(uint8 output[128], uint32 source)
{
    uint32 length = gtmain_string_length(0x800B1B54u), i, seen = 0;
    sint32 character;
    if ((sint32)length >= 124) { w_u32(0x800B1AB0u, 34); return 0; }
    for (i = 0; i <= length; ++i) output[i] = r_u8(0x800B1B54u + i);
    w_u32(0x800B1AB0u, 0);
    if (length != 1) output[length++] = 47;
    character = (sint8)r_u8(source);
    while (character) {
        if (gtmain_path_separator((sint8)character)) {
            if (!seen) { output[0] = 47; length = 1; }
            ++source;
        } else if (character == 46) {
            sint32 next = (sint8)r_u8(source + 1);
            if (!next) ++source;
            else if (gtmain_path_separator((sint8)next)) source += 2;
            else if (next == 46) {
                if (!gtmain_path_separator((sint8)r_u8(source + 2))) break;
                while ((sint32)length >= 2) {
                    --length;
                    if (gtmain_path_separator((sint8)output[length - 1])) break;
                }
                source += 3;
            } else goto CopyPart;
        } else {
CopyPart:
            for (;;) {
                character = (sint8)r_u8(source);
                if (!character) break;
                if ((sint32)length >= 124) { w_u32(0x800B1AB0u, 78); return 0; }
                ++source;
                if (character == 92) character = 47;
                output[length++] = (uint8)character;
                if (character == 47) break;
            }
        }
        character = (sint8)r_u8(source); seen = 1;
    }
    output[length] = 0; w_u32(0x800B1AB0u, 0);
    return 1;
}

/* GTMAIN.EXE 8007196C */
sint32 gtmain_cd_file_query(uint32 guest_path, uint8 descriptor[24])
{
    uint8 path[128], directory[128];
    uint32 i = 0, slash = 0, buffer, entry, sector, size;
    CdlLOC location;
    w_u32(0x800B1AB0u, 5);
    if (gtmain_cd_pending_command()) { gtmain_cd_directory_reset(); return -1; }
    if (r_u8(0x800994C4u) != 2u) return -1;
    if (!gtmain_cd_stream_idle() || !cd_path_resolve(path, guest_path)) return -1;
    do { directory[i] = path[i]; if (path[i] == 47) slash = i; } while (path[i++]);
    directory[slash + 1] = 0; directory[slash] = 0;
    if (gtmain_cd_directory_select(directory + 1) == -1) { gtmain_cd_directory_start(); return -1; }
    buffer = directory[0] ? 0x800C01D0u : r_u32(0x800AD998u);
    entry = gtmain_cd_directory_find(path + slash + 1, 0, buffer);
    if (!entry) {
        /* The original caller address is only a diagnostic argument */
        w_u32(0x800B1AB0u, 2); return -1;
    }
    sector = r_u32(entry + 2u);
    CdIntToPos((sint32)sector, &location);
    descriptor[0] = location.minute; descriptor[1] = location.second;
    descriptor[2] = location.sector; descriptor[3] = location.track;
    size = r_u32(entry + 10u);
    descriptor[4] = (uint8)size; descriptor[5] = (uint8)(size >> 8);
    descriptor[6] = (uint8)(size >> 16); descriptor[7] = (uint8)(size >> 24);
    size = gtmain_string_length(entry + 33u) + 1u;
    if (size > 16u) size = 16u;
    for (i = 0; i < size; ++i) descriptor[8u + i] = r_u8(entry + 33u + i);
    w_u32(0x800B1AB0u, 0); return 0;
}

/* GTMAIN.EXE 80071AE0 */
sint32 gtmain_cd_file_read(uint8 *destination, uint32 capacity, uint32 sector, uint32 bytes)
{
    sint32 result;
    gtmain_cd_stream_callback(0);
    gtmain_cd_stream_native_destination(destination, capacity, bytes);
    gtmain_cd_stream_start(sector);
    result = gtmain_cd_stream_wait();
    gtmain_cd_stream_native_destination(NULL, 0, 0);
    if (result) { w_u32(0x800B1AB0u, 5); return -1; }
    w_u32(0x800B1AB0u, 0); return 0;
}

/* GTMAIN.EXE 800798C0 */
sint32 gtmain_cd_directory_select(const uint8 *path)
{
    uint32 index = 0, word = r_u32(0x800C09D0u), equal = 1;
    for (;;) {
        uint8 cached = (uint8)(word >> (8u * (index & 3u)));
        uint8 incoming = path[index];
        if (cached != incoming) { equal = 0; break; }
        if (!cached) break;
        if (!(++index & 3u)) word = r_u32(0x800C09D0u + index);
    }
    if (equal || (!path[0] && r_u32(0x800AD998u))) return (sint32)r_u32(0x800AD990u);
    w_u32(0x800AD994u, 0x800C09D1u);
    index = 0;
    do { uint8 value = path[index]; w_u8(0x800C09D0u + index++, value); if (!value) break; } while (1);
    return -1;
}
