#include "game_first2_registry.h"
#include <string.h>

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

static uint32 gtmain_cd_response(const uint8 *result)
{
    uint32 response;
    memcpy(&response, result, sizeof(response));
    return response;
}

static void gtmain_cd_unselected_error(void)
{
    /* User-authorized fixed disc has no shell-open transition */
}

static void gtmain_cd_init_sync_thunk(uint8 status, uint8 *result)
{
    gtmain_cd_init_sync(status, result);
}

/* GTMAIN.EXE 8007A248 */
/* Original: GTMAIN.EXE:sub_8007A248. */ uint32 gtmain_bcd_decode(uint32 packed)
{
    uint32 shift = 32u, value = 0u;
    do { shift -= 4u; value = value * 10u + ((packed >> shift) & 15u); } while (shift);
    return value;
}

/* GTMAIN.EXE 8007A278 */
/* Original: GTMAIN.EXE:sub_8007A278. */ uint32 gtmain_bcd_encode(uint32 value)
{
    uint32 shift = 0u, packed = 0u;
    while (value && shift < 32u) {
        packed |= (uint32)r_u8(0x800B19BCu + value % 100u) << shift;
        value /= 100u;
        shift += 8u;
    }
    return packed;
}

/* GTMAIN.EXE 80078C30 */
/* Original: GTMAIN.EXE:sub_80078C30. */ void gtmain_cd_default_sync(uint8 status, const uint8 *result)
{
    uint32 response;
    if (result && (result[0] & 0x10u)) return;
    w_u8(0x800994BEu, status);
    response = gtmain_cd_response(result);
    w_u32(0x800C00C0u, response);
    if (response & 16u) gtmain_cd_unselected_error();
}

/* GTMAIN.EXE 80078CDC */
/* Original: GTMAIN.EXE:sub_80078CDC. */ void gtmain_cd_init_sync(uint8 status, const uint8 *result)
{
    if (result && (result[0] & 0x10u)) return;
    uint32 phase = r_u8(0x800994C3u), response, state;
    w_u8(0x800994BEu, status);
    response = gtmain_cd_response(result);
    state = r_u8(0x800994C2u);
    w_u8(0x800994C2u, 0u);
    w_u32(0x800C00C0u, response);
    if (response & 16u) { gtmain_cd_unselected_error(); return; }
    if (state == 27u) {
        if (response & 2u) w_u8(0x800994C3u, 2u);
        return;
    }
    if (status != 2u && !(status == 5u && state == 32u)) return;
    switch (state) {
    case 27u: phase = 2u; break;
    case 28u: {
        uint32 first = result[1];
        w_u8(0x800C0100u, (uint8)first);
        response = result[2];
        phase = 3u;
        w_u8(0x800AD964u, (uint8)first);
        w_u8(0x800C0101u, (uint8)response);
        break;
    }
    case 29u: {
        uint32 track = r_u8(0x800AD964u), next;
        if (track) {
            track = gtmain_bcd_decode(track);
            next = gtmain_bcd_encode(track + 1u);
            if ((sint32)r_u8(0x800C0101u) < (sint32)next) next = 0u;
            w_u8(0x800AD964u, (uint8)next);
        } else {
            phase = 4u;
            track = gtmain_bcd_decode(r_u8(0x800C0101u)) + 1u;
        }
        w_u8(0x800C0100u + 2u * track, result[1]);
        w_u8(0x800C0101u + 2u * track, result[2]);
        break;
    }
    case 30u: phase = 5u; break;
    case 31u: phase = 6u; break;
    case 32u:
        w_u8(0x800994C4u, (response & 0x4001u) == 0x4001u ? 1u : 2u);
        phase = 7u; break;
    case 33u: w_u8(0x800994C1u, 0u); phase = 0u; break;
    default: break;
    }
    w_u8(0x800994C3u, (uint8)phase);
}

/* GTMAIN.EXE 80078EC4 */
/* Original: GTMAIN.EXE:sub_80078EC4. */ sint32 gtmain_cd_pending_command(void)
{
    uint32 phase, state, command = 0u, parameter = 0u;
    gtmain_cd_native_poll();
    EnterCriticalSectionPSX();
    state = r_u8(0x800994C2u);
    phase = r_u8(0x800994C3u);
    if (!state && phase >= 1u && phase <= 7u) {
        switch (phase) {
        case 1u: command = 1u; break;
        case 2u: command = 19u; break;
        case 3u: command = 20u; parameter = 0x800AD964u; break;
        case 4u: command = 2u; parameter = 0x800AD96Cu; break;
        case 5u: command = 14u; parameter = 0x800AD968u; break;
        case 6u: command = 6u; break;
        case 7u: command = 9u; break;
        }
        w_u8(0x800994C2u, (uint8)(26u + phase));
        CdSyncCallback(gtmain_cd_init_sync_thunk);
        CdReadyCallback(0);
        gtmain_cd_command((uint8)command, parameter);
    }
    ExitCriticalSection();
    return (sint32)phase;
}

/* GTMAIN.EXE 80078FD0 */
/* Original: GTMAIN.EXE:sub_80078FD0. */ sint32 gtmain_cd_stop_status(uint32 stop)
{
    uint32 state;
    sint32 value = 2;
    if (gtmain_cd_pending_command()) return 3;
    EnterCriticalSectionPSX();
    state = r_u8(0x800994C2u);
    if (!state) value = 0;
    else if (state == 16u) {
        if (stop) { w_u8(0x800994C2u, 15u); gtmain_cd_command(16u, 0); }
        value = 1;
    }
    ExitCriticalSection();
    return value;
}

/* GTMAIN.EXE 800790D4 */
/* Original: GTMAIN.EXE:sub_800790D4. */ uint32 gtmain_cd_sector_end(uint32 sector, uint32 size)
{
    return sector + ((size + 2047u) >> 11u);
}

/* GTMAIN.EXE 800790E4 */
/* Original: GTMAIN.EXE:sub_800790E4. */ void gtmain_cd_mode(uint8 mode) { w_u8(0x800C00B0u, mode ^ 0x68u); }

/* GTMAIN.EXE 800790F8 */
/* Original: GTMAIN.EXE:sub_800790F8. */ void gtmain_cd_filter(uint8 file, uint8 channel)
{
    w_u8(0x800994B8u, file);
    w_u8(0x800994B9u, channel);
}

/* GTMAIN.EXE 80079110 */
/* Original: GTMAIN.EXE:sub_80079110. */ void gtmain_cd_parameter(uint16 value) { w_u16(0x800994B4u, value); }

/* GTMAIN.EXE 80079120 */
/* Original: GTMAIN.EXE:sub_80079120. */ void gtmain_cd_buffers(uint32 first, uint32 second, uint32 mode)
{
    EnterCriticalSectionPSX();
    w_u32(0x800C00A8u, first);
    w_u32(0x800C00ACu, second);
    w_u32(0x800C00A4u, mode);
    ExitCriticalSection();
}

/* GTMAIN.EXE 80079224 */
/* Original: GTMAIN.EXE:sub_80079224. */ sint32 gtmain_cd_stop(void)
{
    w_u8(0x800994BFu, 2u);
    return gtmain_cd_stop_status(1u);
}

/* GTMAIN.EXE 80079854 */
/* Original: GTMAIN.EXE:sub_80079854. */ sint32 gtmain_cd_directory_reset(void)
{
    w_u8(0x800AD9A0u, 0u);
    w_u32(0x800AD990u, 0xFFFFFFFFu);
    w_u8(0x800C09D0u, 0u);
    return -1;
}

/* GTMAIN.EXE 8007994C */
/* Original: GTMAIN.EXE:sub_8007994C. */ uint32 gtmain_cd_directory_find(const uint8 *name, uint32 directory, uint32 buffer)
{
    uint8 upper[128];
    uint32 length = 0u, entry, end;
    while (name[length]) {
        sint32 byte = (sint8)name[length];
        if ((uint32)(byte - 97) < 26u) byte -= 32;
        upper[length++] = (uint8)byte;
    }
    if (!directory) { upper[length++] = ';'; upper[length++] = '1'; }
    upper[length] = 0u;
    entry = buffer ? buffer : 0x800C01D0u;
    end = entry + 2048u;
    for (;;) {
        uint32 record_size = r_u8(entry), next = entry + record_size;
        if (next >= end || !record_size) return 0u;
        if (r_u8(entry + 32u) == length &&
            !strncmp((const char *)upper, psx_addr(entry + 33u, length), length)) return entry;
        entry = next;
    }
}

/* GTMAIN.EXE 800796A0 */
/* Original: GTMAIN.EXE:sub_800796A0. */ sint32 gtmain_cd_directory_tick(void)
{
    uint32 buffer = r_u32(0x800AD99Cu), state, done = 0u;
    w_u32(0x800AD990u, r_u32(0x800C0A50u) - 1u);
    gtmain_cd_sector_native(psx_addr(buffer, 2048u), 512);
    state = (uint32)(sint32)(sint8)r_u8(0x800AD9A0u);
    if (state == 1u) {
        uint32 type = r_u8(0x800C01D0u);
        if (type == 255u || strncmp(psx_addr(0x800C01D1u, 5u), "CD001", 5u)) {
            state = 0u; gtmain_cd_directory_reset(); done = 1u;
        } else if (type == 1u) {
            uint32 sector = r_u32(0x800C026Eu);
            uint32 alternate = r_u32(0x800AD998u);
            w_u32(0x800C0A50u, sector);
            w_u32(0x800AD994u, 0x800C09D0u);
            state = 2u;
            if (alternate) w_u32(0x800AD99Cu, alternate);
        }
    } else if (state == 2u) {
        uint8 name[144];
        uint32 path = r_u32(0x800AD994u), length = 0u, entry;
        if (!r_u8(path)) { state = 0u; done = 1u; }
        else {
            do {
                uint8 byte = r_u8(path++);
                if (byte == '/') break;
                name[length++] = byte;
            } while (r_u8(path));
            w_u32(0x800AD994u, path);
            name[length] = 0u;
            entry = gtmain_cd_directory_find(name, 1u, buffer);
            if (!entry) { state = 0u; done = 1u; }
            else {
                w_u32(0x800C0A50u, r_u32(entry + 2u));
                w_u32(0x800AD99Cu, 0x800C01D0u);
            }
        }
    } else { state = 0u; done = 1u; }
    w_u8(0x800AD9A0u, (uint8)state);
    return (sint32)done;
}

/* GTMAIN.EXE 80079878 */
/* Original: GTMAIN.EXE:sub_80079878. */ sint32 gtmain_cd_directory_start(void)
{
    w_u8(0x800AD9A0u, 1u);
    w_u32(0x800AD99Cu, 0x800C01D0u);
    gtmain_cd_stream_callback(0x800796A0u);
    return gtmain_cd_stream_start(16u);
}
