#include "game_draft_registry.h"
#include <string.h>
#include <stdlib.h>
#include "psx_spu.h"


typedef struct GTOSMovieState {
    uint8 reader[28];
    uint8 decoder[28];
    uint32 blocks[76];
    uint32 output[2];
    uint32 decode_callback;
    uint8 workspace[2][12288];
    uint32 active;
    uint8 sector[2352];
} GTOSMovieState;

static GTOSMovieState movie;

static uint32 gtos_movie_u32(const uint8 *bytes)
{
    return (uint32)bytes[0] | ((uint32)bytes[1] << 8u) | ((uint32)bytes[2] << 16u) | ((uint32)bytes[3] << 24u);
}

static uint32 gtos_movie_u16(const uint8 *bytes)
{
    return (uint32)bytes[0] | ((uint32)bytes[1] << 8u);
}

static void gtos_movie_store(uint8 *bytes, uint32 value, uint32 count)
{
    uint32 i;
    for (i = 0u; i < count; ++i) bytes[i] = (uint8)(value >> (8u * i));
}

static void gtos_movie_chunk(void)
{
    const uint8 *header = movie.sector + 24u;
    uint32 flags, fragments, frame, destination, next, index, ready, count, part, total, limit;
    if (gtos_movie_u16(header) != 0x160u || gtos_movie_u16(header + 2u) != 0x5349u) return;
    flags = gtos_movie_u32(movie.reader);
    fragments = gtos_movie_u32(movie.reader + 24u);
    frame = gtos_movie_u32(movie.reader + 20u);
    destination = gtos_movie_u32(movie.reader + 16u);
    next = destination;
    index = gtos_movie_u16(movie.reader + 6u);
    ready = gtos_movie_u16(movie.reader + 8u);
    count = gtos_movie_u16(movie.reader + 10u);
    part = gtos_movie_u16(header + 4u);
    total = gtos_movie_u16(header + 6u);
    limit = gtos_movie_u32(header + 16u) & 0x3FFFFFFFu;
    if (!part) {
        flags &= ~2u;
        if ((sint16)ready == (sint16)count) goto rejected;
        frame = gtos_movie_u32(header + 8u);
        if (index >= 76u) goto invalid;
        destination = movie.blocks[index];
        fragments = 0u;
    } else if ((flags & 2u) || frame != gtos_movie_u32(header + 8u) || fragments != part) {
rejected:
        flags |= 2u;
        if (gtos_movie_u32(header + 8u) >= limit) flags |= 4u;
        goto saved;
    }
    ++fragments;
    next = destination + 2016u;
    if (part == total - 1u) {
        ++index;
        ++ready;
        if ((sint16)index == (sint16)count) index = 0u;
        if (index >= 76u) goto invalid;
        next = movie.blocks[index];
        if (gtos_movie_u32(header + 8u) >= limit) flags |= 4u;
    }
    memcpy(psx_addr(destination, 2016u), movie.sector + 56u, 2016u);
saved:
    gtos_movie_store(movie.reader, flags, 4u);
    gtos_movie_store(movie.reader + 24u, fragments, 4u);
    gtos_movie_store(movie.reader + 20u, frame, 4u);
    gtos_movie_store(movie.reader + 16u, next, 4u);
    gtos_movie_store(movie.reader + 6u, index, 2u);
    gtos_movie_store(movie.reader + 8u, ready, 2u);
    return;
invalid:
    gt_draft_unresolved_call(0x8002F56Cu, index, count, part, total, 0u, 0u, 0u, 0u);
}

typedef struct GTOSMovieBits {
    uint32 value;
    uint32 cursor;
    sint32 remaining;
} GTOSMovieBits;

static void gtos_movie_consume(GTOSMovieBits *bits, uint32 count)
{
    bits->remaining -= (sint32)count;
    bits->value <<= count & 31u;
    if (bits->remaining <= 0) {
        bits->remaining += 16;
        bits->value |= r_u16(bits->cursor) << ((16 - bits->remaining) & 31);
        bits->cursor += 2u;
    }
}

void gtos_movie_unpack(uint32 frame, uint32 output)
{
    uint32 blocks = 6u * ((r_u16(frame + 4u) + 15u) >> 4u) * ((r_u16(frame + 6u) + 15u) >> 4u);
    uint32 dc_size = blocks * 2u, input = frame + 10u, written = 0u;
    uint32 flags = 0u, flag_count = 0u, block, out = output + 4u, end;
    uint8 *dc = (uint8 *)malloc(dc_size ? dc_size : 1u);
    GTOSMovieBits bits;
    if (!dc || dc_size > 49152u)
        gt_draft_unresolved_call(0x8002BFE8u, dc_size, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    /* Expand the two DC byte planes with overlapping LZSS copies */
    while (written < dc_size) {
        if (!flag_count) { flags = r_u8(input++); flag_count = 8u; }
        if (flags & 1u) {
            uint32 length = r_u8(input++) + 3u;
            uint32 part = r_u8(input++), distance = part & 127u;
            if (part & 128u) distance = (distance << 8u) | r_u8(input++);
            ++distance;
            if (distance > written)
                gt_draft_unresolved_call(0x8002BFE8u, distance, written, 0u, 0u, 0u, 0u, 0u, 0u);
            while (length-- && written < dc_size) { dc[written] = dc[written - distance]; ++written; }
        } else dc[written++] = r_u8(input++);
        flags >>= 1u;
        --flag_count;
    }
    w_u16(output, r_u16(frame));
    w_u16(output + 2u, r_u16(frame + 2u));
    bits.cursor = frame + 10u + r_u16(frame + 8u);
    bits.value = (r_u16(bits.cursor) << 16u) | r_u16(bits.cursor + 2u);
    bits.cursor += 4u;
    bits.remaining = 16;
    end = output + 4u + 4u * r_u16(output);
    if (end > output + 49152u)
        gt_draft_unresolved_call(0x8002AF9Cu, frame, end, output, 0u, 0u, 0u, 0u, 0u);
    for (block = 0u; block < blocks; ++block) {
        w_u16(out, dc[block + blocks] | ((uint32)dc[block] << 8u));
        out += 2u;
        for (;;) {
            uint32 count, value, zeros = 0u, shift, table_zeros;
            if (out >= output + 49152u)
                gt_draft_unresolved_call(0x8002AF9Cu, frame, out, blocks, 0u, 0u, 0u, 0u, 0u);
            if (bits.value & 0x80000000u) {
                if ((bits.value >> 29u) < 6u) {
                    w_u16(out, 0xFE00u); out += 2u;
                    gtos_movie_consume(&bits, 2u);
                    break;
                }
                value = (bits.value >> 29u) == 6u ? 1u : 1023u;
                count = 3u;
            } else {
                uint32 scan = bits.value;
                while (zeros < 32u && !(scan & 0x80000000u)) { ++zeros; scan <<= 1u; }
                table_zeros = zeros;
                if (!(bits.value >> 23u)) {
                    gtos_movie_consume(&bits, 9u);
                    shift = 23u - (zeros - 9u);
                } else if (zeros == 5u) {
                    gtos_movie_consume(&bits, 6u);
                    value = bits.value >> 16u;
                    count = 16u;
                    goto emit;
                } else shift = 23u - zeros;
                value = r_u32(0x800526CCu + 256u * table_zeros + ((bits.value >> (shift & 31u)) & 252u));
                count = value >> 26u;
                if (!count)
                    gt_draft_unresolved_call(0x8002AF9Cu, bits.value, table_zeros, out, 0u, 0u, 0u, 0u, 0u);
            }
emit:
            w_u16(out, value); out += 2u;
            gtos_movie_consume(&bits, count);
        }
    }
    free(dc);
    while (out < end) { w_u16(out, 0xFE00u); out += 2u; }
}

static uint32 gtos_movie_dequeue(void)
{
    uint32 index = gtos_movie_u16(movie.reader + 4u), frame, output;
    if (!gtos_movie_u16(movie.reader + 8u)) return 0u;
    frame = movie.blocks[index];
    if (++index == gtos_movie_u16(movie.reader + 10u)) index = 0u;
    gtos_movie_store(movie.reader + 4u, index, 2u);
    output = movie.output[0];
    movie.output[0] = movie.output[1];
    movie.output[1] = output;
    gtos_movie_unpack(frame, output);
    gtos_movie_store(movie.reader + 8u, gtos_movie_u16(movie.reader + 8u) - 1u, 2u);
    return output;
}

void gtos_movie_vblank(void)
{
    sint16 count;
    if (!movie.active) return;
    count = (sint16)(r_u16(0x800757A6u) + 1u);
    w_u16(0x800757A6u, count);
    if (count >= 2 && (sint16)r_u16(0x800757A4u) < (sint16)r_u16(0x8004D9BCu)) {
        w_u16(0x8004D9BCu, r_u16(0x8004D9BCu) - 1u);
        uint32 index = r_u16(0x800757A0u);
        PutDispEnv((DISPENV *)psx_addr(0x80075750u + 20u * index, 20u));
        w_u32(0x8007A140u, 1u);
        SetDispMask(1);
        w_u16(0x800757A0u, (index + 1u) & 3u);
        w_u16(0x800757A4u, 0u);
        w_u16(0x800757A6u, 0u);
    }
}

void gtos_movie_prepare(void)
{
    uint32 i, streams, start, sectors;
    uint32 sector_cursor;
    uint32 black[720] = { 0 };
    PSX_RECT line = { 0, 0, 960, 1 };
    memset(movie.reader, 0, sizeof(movie.reader));
    memset(movie.decoder, 0, sizeof(movie.decoder));
    movie.decoder[8] = 1u;
    movie.decoder[9] = 1u;
    movie.decode_callback = 0x8001087Cu;
    movie.output[0] = 0x8008CF7Cu;
    movie.output[1] = 0x80098F7Cu;
    movie.active = 1u;
    for (i = 0u; i < 4u; ++i) {
        uint32 rect = 0x8004D98Cu + i * 8u;
        uint32 env = 0x80075750u + i * 20u;
        SetDefDispEnv((DISPENV *)psx_addr(env, 20u),
            (sint16)r_u16(rect), (sint16)r_u16(rect + 2u),
            (sint16)r_u16(rect + 4u), (sint16)r_u16(rect + 6u));
        w_u8(env + 17u, 1u);
        w_u8(env + 16u, 1u);
    }
    for (i = 0u; i < 76u; ++i) movie.blocks[i] = 0x800A4F7Cu + i * 8064u;
    movie.reader[10] = 76u;
    movie.reader[11] = 0u;
    streams = r_u32(0x801B1700u) + 10u * 8u;
    start = r_u32(streams) + r_u32(0x8004F6B0u);
    sectors = r_u32(streams + 4u);
    w_u8(0x8007A174u, 0x80u ^ 0x68u);
    w_u8(0x800671F4u, 1u);
    w_u8(0x800671F5u, 7u);
    w_u16(0x800671F0u, 0u);
    w_u32(0x8007A16Cu, start);
    w_u32(0x8007A170u, start + sectors - 1u);
    w_u32(0x8007A168u, 0xFFFFFFFEu);
    memset(psx_addr(0x800756E0u, 8u), 0, 8u);
    w_u16(0x800756E8u, 0u);
    for (i = 0u; i < 16u; ++i)
        w_u32(0x800756EAu + i * 4u, r_u32(0x8007370Cu));
    VSync(0);
    w_u32(0x8007A140u, 0u);
    SetDispMask(0);
    DrawSync(0);
    for (i = 0u; i < 480u; ++i) {
        line.y = (sint16)i;
        if (LoadImagePSX(&line, black) < 0)
            gt_draft_unresolved_call(0x80046838u, i, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        DrawSync(0);
    }
    while (gtos_cd_dispatch()) VSync(0);
    gtos_cd_movie_mode();
    w_u32(0x800671ECu, 0x8002A748u);
    /* Reader counters and pointers live in C rather than a guest stack frame */
    memset(movie.reader + 4u, 0, 6u);
    movie.reader[0] &= 1u;
    memset(movie.reader + 1u, 0, 3u);
    memcpy(movie.reader + 16u, &movie.blocks[0], sizeof(uint32));
    memset(movie.reader + 20u, 0, 8u);
    /* The shared CD stream decodes XA audio and queues physical video sectors */
    cd_stream_consumer_native(1);
    gtos_cd_stream_start();
    sector_cursor = start;
    w_u16(0x8004D9BCu, 0u);
    w_u16(0x800757A2u, 0u);
    w_u16(0x800757A0u, 0u);
    w_u16(0x800757A4u, 1u);
    for (;;) {
        if (gtos_input_poll() & 8u) break;
        while (!gtos_movie_u16(movie.reader + 8u) &&
            !(gtos_movie_u32(movie.reader) & 4u) && sector_cursor < start + sectors) {
            CdlLOC position;
            sint32 available;
            /* Keep Start responsive while assembling a streamed frame */
            if (gtos_input_poll() & 8u) goto finished;
            available = cd_stream_pop_sector_native(movie.sector);
            if (available < 0)
                gt_draft_unresolved_call(0x80030CF4u, start, sectors, sector_cursor, 0u, 0u, 0u, 0u, 0u);
            if (!available) {
                VSync(0);
                if (gtos_input_poll() & 8u) goto finished;
                continue;
            }
            memcpy(&position, movie.sector + 12u, 3u);
            position.track = 0u;
            sector_cursor = (uint32)CdPosToInt(&position) + 1u;
            w_u32(0x8007A164u, sector_cursor - 1u);
            gtos_movie_chunk();
        }
        if (!gtos_movie_u16(movie.reader + 8u)) break;
        if (r_u16(0x8004D9BCu) < 3u) {
            uint32 frame = movie.blocks[gtos_movie_u16(movie.reader + 4u)];
            uint32 width = r_u16(frame + 4u), height = r_u16(frame + 6u);
            uint32 slot = r_u16(0x800757A2u);
            i = gtos_movie_dequeue();
            gtos_mdec_frame(i, width, height,
                (sint16)r_u16(0x8004D9ACu + 4u * slot),
                (sint16)r_u16(0x8004D9AEu + 4u * slot) + 24);
            w_u16(0x800757A2u, (slot + 1u) & 3u);
        } else VSync(0);

    }
finished:
    cd_stream_consumer_native(0);
    gtos_mdec_reset(1u);
    movie.decoder[8] = 2u;
    movie.active = 0u;
    w_u8(0x8004F6A3u, 2u);
    {
        uint8 response[8] = { 0 };
        CdControl(9u, NULL, response);
        w_u8(0x8004F6A6u, 0u);
    }
    VSync(0);
    w_u32(0x8007A140u, 0u);
    SetDispMask(0);
    DrawSync(0);
    for (i = 0u; i < 480u; ++i) {
        line.y = (sint16)i;
        LoadImagePSX(&line, black);
        DrawSync(0);
    }
    /* TODO Unregister the original movie controller callback */
}
