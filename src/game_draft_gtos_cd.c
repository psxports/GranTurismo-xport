#include "game_draft_registry.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "psx_spu.h"

void gtos_cd_stream_start(void)
{
    uint8 response[8] = { 0 }, filter[2];
    CdlLOC location;
    uint32 pending;
    if (gtos_cd_dispatch()) {
        gt_draft_unresolved_call(0x80030E0Cu, r_u8(0x8004F6A7u), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    pending = r_u8(0x8004F6A6u);
    w_u32(0x8007A164u, r_u32(0x8007A16Cu));
    if (pending) {
        /* TODO Resume an already active stream command sequence */
        gt_draft_unresolved_call(0x8002FAD8u, pending, 1u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    CdSyncCallback(NULL);
    CdReadyCallback(NULL);
    w_u8(0x8004F6A4u, 0u);
    filter[0] = r_u8(0x800671F4u);
    filter[1] = r_u8(0x800671F5u);
    w_u16(0x1F801DB0u, r_u16(0x8004F6A0u));
    w_u16(0x1F801DB2u, r_u16(0x8004F6A0u));
    if (!CdControl(13u, filter, response)) goto failed;
    w_u8(0x8004F6A6u, 11u);
    w_u8(0x8004F6ACu, r_u8(0x8007A174u));
    filter[0] = r_u8(0x8004F6ACu);
    if (!CdControl(14u, filter, response)) goto failed;
    w_u8(0x8004F6A6u, 12u);
    if (!CdIntToPos((sint32)r_u32(0x8007A164u), &location) ||
        !CdControl(2u, (uint8 *)&location, response)) goto failed;
    w_u8(0x8004F6A6u, 13u);
    if (!CdControl(27u, NULL, response)) goto failed;
    w_u8(0x8004F6A6u, 14u);
    w_u8(0x8004F6A3u, 0u);
    w_u8(0x8004F6A2u, CdlComplete);
    w_u32(0x80079EF8u, response[0]);
    return;
failed:
    gt_draft_unresolved_call(0x80030AF8u, r_u8(0x8004F6A6u), response[0], 0u, 0u, 0u, 0u, 0u, 0u);
}

static CdlLOC gtos_toc[2];
static uint32 gtos_toc_loaded;

static uint8 gtos_bcd(uint32 value)
{
    return (uint8)((value / 10u) * 16u + value % 10u);
}

static void gtos_toc_load(void)
{
    FILE *file = NULL;
    char line[512], name[260], path[512];
    uint32 track = 0u, minute = 0u, second = 0u, frame = 0u, index = 0u, found = 0u;
    struct _stat64 info;
    uint64 sectors;
    if (gtos_toc_loaded) return;
    if (fopen_s(&file, "../iso/Gran Turismo (USA) (v1.0).cue", "rb") || !file) goto failed;
    name[0] = 0;
    while (fgets(line, sizeof(line), file)) {
        char *text = line;
        while (*text == ' ' || *text == '\t') ++text;
        if (!strncmp(text, "FILE \"", 6u)) {
            char *end = strchr(text + 6u, '"');
            size_t length = end ? (size_t)(end - text - 6u) : sizeof(name);
            if (length >= sizeof(name)) { fclose(file); goto failed; }
            memcpy(name, text + 6u, length);
            name[length] = 0;
        } else if (!strncmp(text, "TRACK ", 6u)) {
            char mode[32];
            if (sscanf_s(text, "TRACK %u %31s", &track, mode, (unsigned)sizeof(mode)) != 2 ||
                track != 1u || strcmp(mode, "MODE2/2352")) { fclose(file); goto failed; }
        } else if (sscanf_s(text, "INDEX %u %u:%u:%u", &index, &minute, &second, &frame) == 4 && index == 1u) {
            if (track != 1u || second >= 60u || frame >= 75u) { fclose(file); goto failed; }
            found = 1u;
        }
    }
    fclose(file);
    if (!found || !name[0] || sprintf_s(path, sizeof(path), "../iso/%s", name) < 0 || _stat64(path, &info)) goto failed;
    if (info.st_size <= 0 || info.st_size % 2352) goto failed;
    sectors = (uint64)info.st_size / 2352u + 150u;
    if (sectors / 4500u >= 100u) goto failed;
    gtos_toc[0].minute = gtos_bcd((uint32)(sectors / 4500u));
    gtos_toc[0].second = gtos_bcd((uint32)(sectors / 75u % 60u));
    gtos_toc[0].sector = gtos_bcd((uint32)(sectors % 75u));
    sectors = (minute * 60u + second) * 75u + frame + 150u;
    gtos_toc[1].minute = gtos_bcd((uint32)(sectors / 4500u));
    gtos_toc[1].second = gtos_bcd((uint32)(sectors / 75u % 60u));
    gtos_toc[1].sector = gtos_bcd((uint32)(sectors % 75u));
    if (!CdSetToc(gtos_toc, 1)) goto failed;
    gtos_toc_loaded = 1u;
    return;
failed:
    /* TODO Support other CUE layouts when another disc is selected */
    gt_draft_unresolved_call(0x8002FC7Cu, 19u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

void gt_project_disc_toc_bind(void)
{
    gtos_toc_load();
}

static void gtos_cd_sync(uint8 status, uint8 *result)
{
    uint32 value, pending = r_u8(0x8004F6A6u), state = r_u8(0x8004F6A7u);
    if (!result) {
        gt_draft_unresolved_call(0x8002AB38u, status, 0u, pending, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    value = (uint32)result[0] | ((uint32)result[1] << 8u) |
        ((uint32)result[2] << 16u) | ((uint32)result[3] << 24u);
    w_u8(0x8004F6A2u, status);
    w_u8(0x8004F6A6u, 0u);
    w_u32(0x80079EF8u, value);
    if (value & 0x10u) {
        w_u8(0x8004F6A5u, 4u);
        w_u8(0x8004F6ACu, 0u);
        w_u8(0x8004F6A7u, 1u);
        w_u8(0x8004F6A8u, 0u);
        w_u8(0x8006CC30u, 0u);
        w_u32(0x8006CC20u, 0xFFFFFFFFu);
        w_u32(0x8007A980u, 0u);
        return;
    }
    if (pending == 27u) {
        if (value & 2u) w_u8(0x8004F6A7u, 2u);
        return;
    }
    if (status != 2u && !(status == 5u && pending == 32u)) return;
    switch (pending) {
    case 28u:
        w_u8(0x80079F20u, result[1]);
        w_u8(0x80079F21u, result[2]);
        w_u8(0x8004F6C0u, result[1]);
        state = 3u;
        break;
    case 29u: {
        uint32 bcd = r_u8(0x8004F6C0u), track;
        if (bcd) {
            uint32 next;
            track = (bcd >> 4u) * 10u + (bcd & 15u);
            next = track + 1u;
            next = r_u8(0x80052768u + next % 100u);
            if ((sint8)r_u8(0x80079F21u) < (sint32)next) next = 0u;
            w_u8(0x8004F6C0u, next);
        } else {
            bcd = r_u8(0x80079F21u);
            track = (bcd >> 4u) * 10u + (bcd & 15u) + 1u;
            state = 4u;
        }
        w_u8(0x80079F20u + track * 2u, result[1]);
        w_u8(0x80079F21u + track * 2u, result[2]);
        break;
    }
    case 30u: state = 5u; break;
    case 31u: state = 6u; break;
    case 32u:
        w_u8(0x8004F6A8u, (value & 0x4001u) == 0x4001u ? 1u : 2u);
        state = 7u;
        break;
    case 33u:
        w_u8(0x8004F6A5u, 0u);
        state = 0u;
        break;
    default: break;
    }
    w_u8(0x8004F6A7u, state);
}

uint32 gtos_cd_dispatch(void)
{
    static const uint8 commands[8] = { 0u, 1u, 19u, 20u, 2u, 14u, 6u, 9u };
    static const uint32 parameters[8] = {
        0u, 0u, 0u, 0x8004F6C0u, 0x8004F6C8u, 0x8004F6C4u, 0u, 0u
    };
    uint32 state = r_u8(0x8004F6A7u);
    if (!r_u8(0x8004F6A6u) && state >= 1u && state <= 7u) {
        uint32 parameter = parameters[state];
        w_u8(0x8004F6A6u, 26u + state);
        uint8 response[8] = { 0 };
        sint32 accepted;
        CdSyncCallback(NULL);
        CdReadyCallback(NULL);
        w_u8(0x80079EF4u, commands[state]);
        w_u32(0x80079EF0u, parameter);
        if (state == 2u || state == 3u) {
            gtos_toc_load();
            response[0] = 2u;
            if (state == 2u) { response[1] = 1u; response[2] = 1u; }
            else {
                uint32 track = r_u8(parameter);
                track = (track >> 4u) * 10u + (track & 15u);
                if (track > 1u)
                    gt_draft_unresolved_call(0x8002FC7Cu, 20u, track, 0u, 0u, 0u, 0u, 0u, 0u);
                response[1] = gtos_toc[track].minute;
                response[2] = gtos_toc[track].second;
            }
            accepted = 1;
        } else accepted = CdControl(commands[state], parameter ? (uint8 *)psx_addr(parameter, 4u) : NULL, response);
        if (!accepted)
            gt_draft_unresolved_call(0x8002FC7Cu, commands[state], parameter, 0u, 0u, 0u, 0u, 0u, 0u);
        gtos_cd_sync(CdlComplete, response);
    }
    return state;
}

void gtos_cd_mode(uint8 mode)
{
    uint32 changed = (r_u8(0x8004F6ACu) ^ mode) & 0x80u;
    w_u8(0x8004F6ACu, mode);
    if (changed) {
        uint8 response[8] = { 0 };
        if (!CdControl(14u, &mode, response))
            gt_draft_unresolved_call(0x8002AE2Cu, mode, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        VSync(0);
        VSync(3);
    }
}

void gtos_cd_movie_mode(void)
{
    gtos_cd_mode(0x80u);
}
