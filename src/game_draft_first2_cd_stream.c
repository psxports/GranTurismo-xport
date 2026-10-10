#include "game_first2_registry.h"
#include "psx_spu.h"
#include <string.h>

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);


typedef struct GTNativeCdCompletion {
    CdlCB callback;
    uint8 status;
    uint8 response[8];
} GTNativeCdCompletion;

static GTNativeCdCompletion native_completions[8];
static uint32 native_completion_read, native_completion_count, native_polling;
static uint8 native_sector[2352];
static uint32 native_sector_offset, native_sector_valid, native_streaming;

static void gtmain_cd_completion_queue(CdlCB callback, sint32 status, const uint8 response[8])
{
    GTNativeCdCompletion *event;
    if (!callback) return;
    if (native_completion_count == 8u)
        gt_draft_failure("Native CD completion queue extent", 0x8007A2E4u);
    event = &native_completions[(native_completion_read + native_completion_count) % 8u];
    event->callback = callback;
    event->status = (uint8)status;
    memcpy(event->response, response, 8u);
    ++native_completion_count;
}

static sint32 gtmain_cd_control_native(uint8 command, uint8 *parameter)
{
    uint8 response[8] = { 0 };
    CdlCB callback;
    sint32 accepted, completion;
    if (command == 19u || command == 20u) {
        CdlLOC toc[100];
        sint32 tracks;
        uint32 track = 0;
        gt_project_disc_toc_bind();
        tracks = CdGetToc2(1, toc);
        if (tracks <= 0 || tracks >= 100)
            gt_draft_failure("Native mounted disc TOC extent", 0x8007A2E4u);
        if (command == 20u) {
            if (!parameter || (parameter[0] & 15u) > 9u || (parameter[0] >> 4) > 9u)
                gt_draft_failure("Native GetTD BCD track argument", 0x8007A2E4u);
            track = (parameter[0] >> 4) * 10u + (parameter[0] & 15u);
            if (track > (uint32)tracks)
                gt_draft_failure("Native GetTD track outside mounted TOC", 0x8007A2E4u);
        }
        callback = CdSyncCallback(NULL);
        accepted = CdControl(1u, NULL, response);
        completion = CdSync(0, NULL);
        CdSyncCallback(callback);
        if (accepted) {
            if (command == 19u) {
                response[1] = 1u;
                response[2] = (uint8)(((uint32)tracks / 10u) * 16u + (uint32)tracks % 10u);
            } else {
                response[1] = toc[track].minute;
                response[2] = toc[track].second;
            }
        }
    } else {
        /* Deliver actual completion after the caller publishes its next state */
        callback = CdSyncCallback(NULL);
        accepted = CdControl(command, parameter, response);
        completion = CdSync(0, NULL);
        CdSyncCallback(callback);
    }
    gtmain_cd_completion_queue(callback, completion, response);
    return accepted;
}

sint32 gtmain_cd_sector_native(void *destination, sint32 words)
{
    uint32 bytes;
    if (!native_sector_valid) return CdGetSector(destination, words);
    if (!destination || words < 0 || words > 585)
        gt_draft_failure("Native staged CD transfer argument", 0x800855C8u);
    bytes = (uint32)words * 4u;
    if (bytes > sizeof(native_sector) - native_sector_offset)
        gt_draft_failure("Native staged CD transfer extent", 0x800855C8u);
    memcpy(destination, native_sector + native_sector_offset, bytes);
    native_sector_offset += bytes;
    return 1;
}

void gtmain_cd_native_poll(void)
{
    if (native_polling) return;
    native_polling = 1u;
    if (native_completion_count) {
        GTNativeCdCompletion event = native_completions[native_completion_read];
        native_completion_read = (native_completion_read + 1u) % 8u;
        --native_completion_count;
        event.callback(event.status, event.response);
    } else if (native_streaming && r_u8(0x800994C2u) == 6u) {
        sint32 available = cd_stream_pop_sector_native(native_sector);
        if (available < 0)
            gt_draft_failure("Native CD physical stream error", 0x80079B6Cu);
        if (available) {
            uint8 response[8] = { 0 };
            if (!CdControlBPSX(1u, NULL, response))
                gt_draft_failure("Native CD ready status unavailable", 0x80079DC8u);
            native_sector_offset = 12u;
            native_sector_valid = 1u;
            gtmain_cd_stream_ready(CdlDataReady, response);
            native_sector_valid = 0u;
        }
    }
    native_polling = 0u;
}

sint32 gtmain_cd_command_native(uint8 command, uint8 *parameter)
{
    return gtmain_cd_control_native(command, parameter);
}


static uint8 *native_destination;
static uint32 native_capacity, native_remaining;

void gtmain_cd_stream_native_destination(uint8 *destination, uint32 capacity, uint32 bytes)
{
    uint32 aligned = (bytes + 3u) & ~3u;
    if (destination && aligned > capacity)
        gt_draft_failure("Native CD transfer capacity", 0x8007A340u);
    native_destination = destination;
    native_capacity = capacity; native_remaining = aligned;
    if (destination) w_u32(0x800C0A4Cu, aligned);
}

static sint32 gtmain_cd_stream_missing(const char *name, uint32 address)
{
    /* TODO Bind the selected stream's remaining native game dependencies */
    gt_draft_failure(name, address);
}

static void gtmain_cd_stream_default_sync(uint8 status, uint8 *result)
{
    gtmain_cd_default_sync(status, result);
}

static void gtmain_cd_stream_sync_thunk(uint8 status, uint8 *result)
{
    gtmain_cd_stream_sync(status, result);
}

static void gtmain_cd_stream_ready_thunk(uint8 status, uint8 *result)
{
    gtmain_cd_stream_ready(status, result);
}

/* GTMAIN.EXE 80079A70 */
sint32 gtmain_cd_stream_idle(void)
{
    gtmain_cd_native_poll();
    return r_u8(0x800994C2u) == 0u;
}

/* GTMAIN.EXE 80079A80 */
uint32 gtmain_cd_stream_callback(uint32 token)
{
    uint32 previous = r_u32(0x800AD9A4u);
    w_u32(0x800AD9A4u, token ? token : 0x80079E94u);
    return previous;
}

/* GTMAIN.EXE 8007A340 */
uint32 gtmain_cd_stream_destination(uint32 destination, uint32 size)
{
    uint32 aligned = (size + 3u) & ~3u;
    w_u32(0x800C0A4Cu, aligned);
    w_u32(0x800C0A54u, destination);
    return (aligned + 2047u) >> 11u;
}

/* GTMAIN.EXE 8007A2E4 */
sint32 gtmain_cd_command(uint8 command, uint32 parameter)
{
    w_u8(0x800C00BCu, command);
    w_u32(0x800C00B8u, parameter);
    return gtmain_cd_control_native(command, parameter ? psx_addr(parameter, 1u) : 0);
}

/* GTMAIN.EXE 80079B08 */
sint32 gtmain_cd_stream_reset(void)
{
    CdSyncCallback(gtmain_cd_stream_default_sync);
    CdReadyCallback(0);
    w_u8(0x800994C2u, 0u);
    return 0;
}

/* GTMAIN.EXE 80079B40 */
sint32 gtmain_cd_stream_pause(void)
{
    CdReadyCallback(0);
    native_streaming = 0u;
    cd_stream_consumer_native(0);
    return gtmain_cd_control_native(9u, 0);
}

/* GTMAIN.EXE 80079BCC */
sint32 gtmain_cd_stream_seek(void)
{
    CdlLOC location;
    w_u8(0x800994C1u, 0u);
    gtmain_cd_control_native(2u, (uint8 *)CdIntToPos((sint32)r_u32(0x800C0A50u), &location));
    w_u8(0x800994C2u, 3u);
    return 3;
}

/* GTMAIN.EXE 80079C14 */
sint32 gtmain_cd_stream_finish(void)
{
    gtmain_cd_stream_pause();
    w_u8(0x800994C2u, 1u);
    return 1;
}

/* GTMAIN.EXE 80079C40 */
sint32 gtmain_cd_stream_restart(void)
{
    gtmain_cd_stream_pause();
    w_u8(0x800994C2u, 2u);
    return 2;
}

/* GTMAIN.EXE 80079C6C */
void gtmain_cd_stream_sync(uint8 status, const uint8 *result)
{
    uint32 state, request;
    w_u8(0x800994BEu, status);
    w_u32(0x800C00C0u, (uint32)result[0] | ((uint32)result[1] << 8u) |
        ((uint32)result[2] << 16u) | ((uint32)result[3] << 24u));
    if (status != 2u) { gtmain_cd_stream_missing("GTMAIN CD error transition", 0x80079B6Cu); return; }
    state = r_u8(0x800994C2u);
    request = r_u8(0x800994BFu);
    if (state != 5u) {
        w_u8(0x800994BFu, 0u);
        if (request == 1u) state = state == 6u ? 7u : 2u;
        else if (request == 2u) state = state == 6u ? 8u : 1u;
    }
    switch (state) {
    case 1u: state = (uint32)gtmain_cd_stream_reset(); break;
    case 2u: state = (uint32)gtmain_cd_stream_seek(); break;
    case 3u:
        w_u8(0x800994C8u, 0xA0u);
        gtmain_cd_control_native(14u, psx_addr(0x800994C8u, 1u)); state = 4u; break;
    case 4u:
        CdReadyCallback(gtmain_cd_stream_ready_thunk);
        native_streaming = 1u;
        cd_stream_consumer_native(1);
        gtmain_cd_control_native(6u, 0); state = 5u; break;
    case 5u: state = 6u; break;
    case 6u: break;
    case 7u: state = (uint32)gtmain_cd_stream_restart(); break;
    case 8u: state = (uint32)gtmain_cd_stream_finish(); break;
    default: break;
    }
    w_u8(0x800994C2u, (uint8)state);
}

/* GTMAIN.EXE 80079E94 */
sint32 gtmain_cd_stream_copy_sector(void)
{
    if (native_destination) {
        uint32 amount = native_remaining < 2048u ? native_remaining : 2048u;
        if (amount > native_capacity) gt_draft_failure("Native CD transfer overrun", 0x80079E94u);
        gtmain_cd_sector_native(native_destination, (sint32)(amount >> 2));
        native_destination += amount; native_capacity -= amount; native_remaining -= amount;
        w_u32(0x800C0A4Cu, native_remaining);
        return native_remaining == 0u;
    }
    uint32 remaining = r_u32(0x800C0A4Cu);
    uint32 destination = r_u32(0x800C0A54u);
    uint32 amount = remaining < 2048u ? (remaining + 3u) & ~3u : 2048u;
    gtmain_cd_sector_native(psx_addr(destination, amount), (sint32)(amount >> 2u));
    remaining -= amount;
    w_u32(0x800C0A54u, destination + amount);
    w_u32(0x800C0A4Cu, remaining);
    return remaining == 0u;
}

/* GTMAIN.EXE 80079DC8 */
void gtmain_cd_stream_ready(uint8 status, const uint8 *result)
{
    uint8 header[128];
    uint32 sector, token, request, response;
    sint32 value;
    w_u8(0x800994BEu, status);
    response = (uint32)result[0] | ((uint32)result[1] << 8u) |
        ((uint32)result[2] << 16u) | ((uint32)result[3] << 24u);
    request = r_u8(0x800994BFu);
    w_u32(0x800C00C0u, response);
    if (status != 1u) { gtmain_cd_stream_missing("GTMAIN CD error transition", 0x80079B6Cu); return; }
    if (request == 1u) { gtmain_cd_stream_restart(); return; }
    if (request == 2u) { gtmain_cd_stream_finish(); return; }
    gtmain_cd_sector_native(header, 3);
    sector = r_u32(0x800C0A50u);
    if (sector != (uint32)CdPosToInt((CdlLOC *)header)) {
        w_u8(0x800994C1u, 3u);
        gtmain_cd_stream_restart(); return;
    }
    token = r_u32(0x800AD9A4u);
    w_u32(0x800C0A50u, sector + 1u);
    if (token == 0x800796A0u) value = gtmain_cd_directory_tick();
    else if (token == 0x80071B5Cu) value = gtmain_cd_decode_ready();
    else if (token == 0x80079E94u) value = gtmain_cd_stream_copy_sector();
    else { gtmain_cd_stream_missing("GTMAIN CD stream callback token", token); return; }
    if (value) gtmain_cd_stream_finish();
}

/* GTMAIN.EXE 80079F10 */
sint32 gtmain_cd_stream_can_read(uint32 state)
{
    return state == 0u || state - 1u < 8u;
}

/* GTMAIN.EXE 80079F30 */
void gtmain_cd_stream_start(uint32 sector)
{
    sint32 pending = gtmain_cd_pending_command();
    if (pending) { w_u8(0x800994C1u, 4u); return; }
    EnterCriticalSectionPSX();
    if (gtmain_cd_stream_can_read(r_u8(0x800994C2u))) {
        uint32 request = 1u;
        w_u32(0x800C0A50u, sector);
        if (!r_u8(0x800994C2u)) {
            CdSyncCallback(gtmain_cd_stream_sync_thunk);
            CdReadyCallback(0);
            gtmain_cd_stream_seek();
            request = 0u;
        }
        w_u8(0x800994BFu, (uint8)request);
    } else w_u8(0x800994C1u, 1u);
    ExitCriticalSection();
}

/* GTMAIN.EXE 80079AA8 */
void gtmain_cd_stream_read(uint32 destination, uint32 sector, uint32 size)
{
    gtmain_cd_stream_destination(destination, size);
    gtmain_cd_stream_start(sector);
}

/* GTMAIN.EXE 80079AD8 */
sint32 gtmain_cd_stream_wait(void)
{
    while (!gtmain_cd_stream_idle()) { }
    return r_u8(0x800994C1u);
}
