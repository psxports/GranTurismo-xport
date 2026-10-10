#include "game_draft_registry.h"
#include "psx_mdec.h"
#include "psx_spu.h"
#include <string.h>
#include <stdio.h>

static MdecState preview_decoder;
static uint32 preview_decoder_ready;
static uint32 preview_stream_active;

static uint32 preview_u16(const uint8 *p)
{
    return (uint32)p[0] | ((uint32)p[1] << 8);
}

static uint32 preview_u32(const uint8 *p)
{
    return preview_u16(p) | (preview_u16(p + 2) << 16);
}

static void preview_chunk(uint32 reader, const uint8 sector[2352])
{
    const uint8 *header = sector + 24;
    uint32 flags, fragments, frame, destination, next, index, ready, count;
    uint32 part, total, limit, incoming;
    if (preview_u16(header) != 0x160u || preview_u16(header + 2) != 0x5349u) return;
    flags = r_u32(reader); fragments = r_u32(reader + 24); frame = r_u32(reader + 20);
    destination = r_u32(reader + 16); next = destination;
    index = r_u16(reader + 6); ready = r_u16(reader + 8); count = r_u16(reader + 10);
    part = preview_u16(header + 4); total = preview_u16(header + 6);
    incoming = preview_u32(header + 8); limit = preview_u32(header + 16) & 0x3FFFFFFFu;
    if (!part) {
        flags &= ~2u;
        if ((sint16)ready == (sint16)count) goto rejected;
        frame = incoming;
        destination = r_u32(r_u32(reader + 12) + (uint32)((sint32)(sint16)index * 4));
        fragments = 0;
    } else if ((flags & 2u) || frame != incoming || fragments != part) {
rejected:
        flags |= 2u;
        if (incoming >= limit) flags |= 4u;
        goto saved;
    }
    ++fragments;
    next = destination + 2016u;
    if (part == total - 1u) {
        ++index; ++ready;
        if ((sint16)index == (sint16)count) index = 0;
        next = r_u32(r_u32(reader + 12) + (uint32)((sint32)(sint16)index * 4));
        if (incoming >= limit) flags |= 4u;
    }
    memcpy(psx_addr(destination, 2016u), sector + 56, 2016u);
saved:
    w_u32(reader, flags); w_u32(reader + 24, fragments); w_u32(reader + 20, frame);
    w_u32(reader + 16, next); w_u16(reader + 6, index); w_u16(reader + 8, ready);
}

static void preview_start(void)
{
    uint32 reader = r_u32(0x80079F10u);
    uint8 response[8] = { 0 };
    /* Replace the current native CD stream with the requested sector range */
    cd_stream_consumer_native(0);
    if (!CdControl(9u, NULL, response))
        gt_draft_unresolved_call(0x8002FAD8u, 1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u8(0x8004F6A6u, 0u);
    w_u32(0x800671ECu, 0x8002A748u);
    w_u16(reader + 4, 0); w_u16(reader + 6, 0); w_u16(reader + 8, 0);
    w_u32(reader, r_u32(reader) & 1u);
    w_u32(reader + 16, r_u32(r_u32(reader + 12)));
    w_u32(reader + 20, 0); w_u32(reader + 24, 0); w_u32(reader + 64, 0);
    cd_stream_consumer_native(1);
    gtos_cd_stream_start();
    preview_stream_active = 1;
}

void gtos_arcade_track_preview_request(uint32 track)
{
    uint32 entry = r_u32(0x801B1700u) + (track & 255u) * 8u;
    uint32 start = r_u32(entry) + r_u32(0x8004F6B0u);
    w_u8(0x80079250u, 1); w_u8(0x80079258u, 15);
    w_u8(0x80079251u, 0); w_u8(0x80079264u, 2);
    w_u8(0x8007A174u, 0x80u ^ 0x68u);
    w_u8(0x800671F4u, 1); w_u8(0x800671F5u, 7); w_u16(0x800671F0u, 0);
    w_u32(0x8007A16Cu, start);
    w_u32(0x8007A170u, start + r_u32(entry + 4) - 1u);
    w_u32(0x8007A168u, 0xFFFFFFFEu);
}

static void preview_restart(void)
{
    w_u32(0x800762D8u, 0); w_u32(0x800762DCu, 1); w_u32(0x80079260u, 1);
    preview_start();
}

static void preview_image(uint32 input, uint32 width, uint32 height, sint16 x, sint16 y)
{
    uint32 columns = (width + 15u) >> 4, rows = (height + 15u) & ~15u;
    uint32 words = r_u16(input), fed = 0, column;
    uint32 command = (r_u32(input) & ~0x02000000u) | 0x08000000u;
    uint32 strip[2048];
    uint32 tables[2] = { 0x80073BE4u, 0x80073C68u };
    PSX_RECT rect = { x, y, 16, (sint16)rows };
    if (!columns || rows > 256u)
        gt_draft_unresolved_call(0x8002F7FCu, width, height, 0u, 0u, 0u, 0u, 0u, 0u);
    if (!preview_decoder_ready) {
        uint32 i;
        mdec_initialize(&preview_decoder);
        for (i = 0; i < 2; ++i) {
            mdec_write_data(&preview_decoder, r_u32(tables[i]));
            mdec_dma_write(&preview_decoder, (const uint32 *)psx_addr(tables[i] + 4u, 128u), 32u);
        }
        preview_decoder_ready = 1;
    }
    mdec_control(&preview_decoder, 0x80000000u);
    mdec_control(&preview_decoder, 0x60000000u);
    w_u32(input, command);
    mdec_write_data(&preview_decoder, command);
    for (column = 0; column < columns; ++column) {
        uint32 received = 0, required = 8u * rows;
        while (received < required) {
            uint32 old_fed = fed, old_received = received;
            while (fed < words && mdec_write_data(&preview_decoder, r_u32(input + 4u + 4u * fed))) ++fed;
            if (preview_decoder.copy_ticks) mdec_advance(&preview_decoder, preview_decoder.copy_ticks);
            received += mdec_dma_read(&preview_decoder, strip + received, required - received);
            if (fed == old_fed && received == old_received && !preview_decoder.copy_ticks)
                gt_draft_unresolved_call(0x800426E0u, fed, words, received, required, column, 0u, 0u, 0u);
        }
        if (LoadImagePSX(&rect, strip) < 0)
            gt_draft_unresolved_call(0x80046838u, column, rows, 0u, 0u, 0u, 0u, 0u, 0u);
        DrawSync(0);
        rect.x += 16;
    }
}

void gtos_arcade_track_preview_tick(void)
{
    uint32 reader, count, index, frame, output, slot, decoder, width, height;
    uint8 sector[2352];
    sint32 available;
    sint8 delay;
    if (!r_u8(0x80079250u)) return;
    delay = (sint8)r_u8(0x80079258u);
    if (delay > 0) {
        w_u8(0x80079258u, (uint8)(delay - 1));
        if (delay == 1) preview_restart();
        return;
    }
    reader = r_u32(0x80079F10u);
    if (!preview_stream_active) preview_restart();
    while (r_u16(reader + 8) < r_u16(reader + 10) && !(r_u32(reader) & 4u)) {
        CdlLOC location;
        available = cd_stream_pop_sector_native(sector);
        if (available < 0)
            gt_draft_unresolved_call(0x80030CF4u, r_u32(0x8007A164u), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        if (!available) break;
        memcpy(&location, sector + 12, 3); location.track = 0;
        w_u32(0x8007A164u, (uint32)CdPosToInt(&location));
        preview_chunk(reader, sector);
    }
    count = r_u16(reader + 8);
    if (!count) {
        if (r_u32(reader) & 4u) preview_restart();
        return;
    }
    decoder = reader + 28;
    if (!r_u8(decoder + 8)) return;
    index = r_u16(reader + 4);
    frame = r_u32(r_u32(reader + 12) + index * 4u);
    if (++index == r_u16(reader + 10)) index = 0;
    w_u16(reader + 4, index);
    slot = r_u32(0x800762DCu);
    w_u16(reader + 68, r_u16(0x8004E328u + slot * 4u));
    w_u16(reader + 70, r_u16(0x8004E32Au + slot * 4u));
    width = r_u16(frame + 4); height = r_u16(frame + 6);
    w_u16(reader + 72, width); w_u16(reader + 74, height);
    output = r_u32(reader + 56);
    w_u32(reader + 56, r_u32(reader + 60)); w_u32(reader + 60, output); w_u32(reader + 64, output);
    gtos_movie_unpack(frame, output);
    w_u16(reader + 8, count - 1u);
    w_u16(decoder + 12, (width + 15u) >> 4); w_u16(decoder + 14, (width + 15u) >> 4);
    w_u16(decoder + 4, 16); w_u16(decoder + 6, (height + 15u) & ~15u);
    w_u16(decoder + 10, 8u * ((height + 15u) & ~15u));
    w_u16(decoder, r_u16(reader + 68)); w_u16(decoder + 2, r_u16(reader + 70)); w_u8(decoder + 8, 0);
    preview_image(output, width, height, (sint16)r_u16(reader + 68), (sint16)r_u16(reader + 70));
    w_u16(decoder, r_u16(decoder) + 16u * ((width + 15u) >> 4)); w_u16(decoder + 14, 0); w_u8(decoder + 8, 1);
    if (r_u32(decoder + 24) && r_u32(decoder + 24) != 0x80018A20u)
        gt_draft_unresolved_call(r_u32(decoder + 24), 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (r_u32(decoder + 24)) {
    index = r_u32(0x800762D8u) + 1u;
    w_u32(0x800762D8u, index == 2u ? 0u : index);
    w_u8(0x80079251u, 1); w_u32(0x80079260u, r_u32(0x80079260u) + 1u);
    }
    w_u32(0x800762DCu, slot + 1u == 2u ? 0u : slot + 1u);
}

sint32 gtos_arcade_track_preview_visible(void)
{
    sint8 delay;
    if (!(sint8)r_u8(0x80079251u) || !(sint8)r_u8(0x80079250u)) return -1;
    delay = (sint8)r_u8(0x80079264u);
    if (delay > 0) {
        w_u8(0x80079264u, (uint8)(delay - 1));
        return -1;
    }
    return (sint32)r_u32(0x800762D8u);
}

void gtos_arcade_track_preview_pause(void)
{
    uint8 response[8] = { 0 };
    uint32 reader = r_u32(0x80079F10u);
    w_u8(0x80079250u, 0);
    if (preview_decoder_ready) mdec_control(&preview_decoder, 0x80000000u);
    w_u8(reader + 36u, 2);
    w_u8(0x8004F6A3u, 2);
    cd_stream_consumer_native(0);
    if (!CdControl(9u, NULL, response))
        gt_draft_unresolved_call(0x8002FAD8u, 1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u8(0x8004F6A6u, 0);
    preview_stream_active = 0;
    printf("movie stop\n");
}

void gtos_arcade_track_preview_stop(void)
{
    gtos_arcade_track_preview_pause();
    w_u32(0x800671ECu, 0);
    /* The native decoder uses direct completion instead of an SDK DMA callback */
}


