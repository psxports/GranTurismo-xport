#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 vehicle_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static sint32 vehicle_asr(uint32 value, uint32 shift)
{
    return (sint32)value >> (shift & 31u);
}

static uint32 vehicle_lz(uint32 value, uint32 address)
{
    xport_gte_write_data(30u, value);
    return xport_gte_read_data(31u);
}

static uint32 vehicle_flags(uint32 address)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return (uint32)state.flag;
}

static void vehicle_gpf(void)
{
    SVECTOR input;
    VECTOR output;
    input.vx = (sint16)xport_gte_read_data(9u);
    input.vy = (sint16)xport_gte_read_data(10u);
    input.vz = (sint16)xport_gte_read_data(11u);
    input.pad = 0;
    gte_gpf12(&input, (sint16)xport_gte_read_data(8u), &output);
}

static void vehicle_link(uint32 packet, uint32 link)
{
    uint32 old = r_u8(link) | ((uint32)r_u8(link + 1u) << 8) | ((uint32)r_u8(link + 2u) << 16);
    w_u8(link, packet); w_u8(link + 1u, packet >> 8); w_u8(link + 2u, packet >> 16);
    w_u8(packet, old); w_u8(packet + 1u, old >> 8); w_u8(packet + 2u, old >> 16);
}

static void vehicle_words(uint32 address, uint32 words[6])
{
    uint32 i;
    for (i = 0; i < 6; ++i) words[i] = r_u32(address + i * 4u);
}

static void vehicle_rotation(uint32 matrix)
{
    uint32 a = r_u32(matrix), b = r_u32(matrix + 4u);
    gte_ctc2(a, 0); a = r_u32(matrix + 8u); gte_ctc2(b, 1);
    b = r_u32(matrix + 12u); gte_ctc2(a, 2);
    a = r_u32(matrix + 16u); gte_ctc2(b, 3); gte_ctc2(a, 4);
}

static void vehicle_translation(uint32 context)
{
    uint32 a, b;
    gtmain_matrix_bind_packed_negation(psx_addr(context + 64u, 20u));
    xport_gte_write_data(0, r_u32(context + 280u));
    xport_gte_write_data(1, r_u32(context + 284u));
    xport_gte_execute(0x486012u);
    w_u32(context + 84u, xport_gte_read_data(9));
    w_u32(context + 88u, xport_gte_read_data(10));
    w_u32(context + 92u, xport_gte_read_data(11));
    a = r_u32(context + 84u); b = r_u32(context + 88u);
    gte_ctc2(a, 5); a = r_u32(context + 92u); gte_ctc2(b, 6); gte_ctc2(a, 7);
}

sint32 gtmain_driver_hidden(uint32 car)
{
    uint32 driver = r_u32(0x800B6638u);
    return car == r_u32(driver + 180u) ? (sint8)r_u8(driver + 254u) : 0;
}

sint32 gtmain_model_wheel_matrix(uint32 context, sint32 scale)
{
    sint32 x = (sint16)r_u16(context + 964u), y = (sint16)r_u16(context + 970u);
    sint32 z = (sint16)r_u16(context + 976u);
    xport_gte_write_data(8, (uint32)scale); xport_gte_write_data(9, (uint32)x);
    xport_gte_write_data(10, (uint32)y); xport_gte_write_data(11, (uint32)z);
    vehicle_gpf();
    x = (sint32)xport_gte_read_data(9); y = (sint32)xport_gte_read_data(10); z = (sint32)xport_gte_read_data(11);
    w_u16(context + 970u, y); w_u16(context + 964u, x); w_u16(context + 976u, z);
    return gtmain_matrix_multiply(psx_addr(context + 960u, 20u), psx_addr(context + 884u, 20u), psx_addr(context + 960u, 20u));
}

uint32 gtmain_model_wheel_draw(uint32 context, sint32 radius, uint32 detail)
{
    uint32 x = r_u32(context + 940u), cx = r_u32(context + 44u);
    uint32 y = r_u32(context + 944u), z = r_u32(context + 948u);
    uint32 cy = r_u32(context + 48u), cz = r_u32(context + 52u);
    uint32 table, cursor, packet, ordering, words[6], next[6], result, i;
    sint32 exponent, minimum, difference, shift, count, winding;
    x -= cx; y -= cy; z -= cz;
    w_u32(context + 268u, x); w_u32(context + 272u, y); w_u32(context + 276u, z);
    minimum = 31 - (sint32)vehicle_lz((uint32)radius, 0x8006D338u);
    exponent = 31 - (sint32)vehicle_lz(vehicle_abs(x) | vehicle_abs(y) | vehicle_abs(z), 0x8006D37Cu);
    ++minimum; if (exponent < minimum) exponent = minimum;
    difference = exponent - minimum; shift = exponent - 13; if (shift < 0) shift = 0;
    minimum -= 13;
    w_u16(context + 40u, shift); w_u16(context + 42u, difference);
    w_u16(context + 280u, vehicle_asr(x, shift)); w_u16(context + 282u, vehicle_asr(y, shift)); w_u16(context + 284u, vehicle_asr(z, shift));
    if (minimum > 0) radius = vehicle_asr((uint32)radius, minimum);
    else if (minimum < 0) radius = (sint32)((uint32)radius << ((0u - (uint32)minimum) & 31u));
    gtmain_matrix_transpose(psx_addr(context + 248u, 20u), psx_addr(context + 960u, 20u));
    gtmain_matrix_scale_gte(psx_addr(context + 248u, 20u), psx_addr(context + 248u, 20u), (uint16)(0u - (uint32)radius));
    gtmain_matrix_rotate_short(psx_addr(context + 56u, 8u), psx_addr(context + 248u, 20u), psx_addr(context + 280u, 8u));
    vehicle_translation(context);
    gtmain_matrix_multiply(psx_addr(context + 96u, 20u), psx_addr(context + 64u, 20u), psx_addr(context + 960u, 20u));
    gtmain_matrix_scale_gte(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), (uint16)radius);
    gtmain_matrix_shift(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), difference);
    gtmain_model_matrix_bind(context, psx_addr(context + 96u, 20u)); gtmain_lighting_bind(context);
    table = r_u32(0x80097470u + detail * 4u); result = table; count = (sint16)r_u16(table);
    cursor = table + 4u;
    if (!count) return result;
    ordering = r_u32(context + 928u); shift = r_u16(context + 40u); winding = (sint16)r_u16(context + 908u);
    packet = r_u32(0x800BFE4Cu); vehicle_words(cursor, words);
    do
    {
        uint32 fourth, flags, sum, first_area, second_area, link;
        sint32 depth;
        xport_gte_write_data(0, words[2]); xport_gte_write_data(1, words[3]);
        xport_gte_write_data(2, words[4]); xport_gte_write_data(3, words[5]);
        xport_gte_write_data(4, words[0]); xport_gte_write_data(5, words[1]);
        w_u8(packet + 12u, words[1] >> 16); w_u8(packet + 13u, words[1] >> 24);
        w_u8(packet + 20u, words[3] >> 16); w_u8(packet + 21u, words[3] >> 24);
        xport_gte_execute(0x280030u);
        for (i = 0; i < 4; ++i) next[i] = r_u32(cursor + 32u + i * 4u);
        fourth = r_u32(cursor + 28u); flags = vehicle_flags(0x8006D55Cu);
        w_u32(packet + 16u, xport_gte_read_data(12));
        sum = xport_gte_read_data(17); sum += xport_gte_read_data(18); sum += xport_gte_read_data(19);
        w_u8(packet + 36u, words[5] >> 16); w_u8(packet + 37u, words[5] >> 24);
        w_u8(packet + 28u, fourth >> 16); w_u8(packet + 29u, fourth >> 24);
        xport_gte_execute(0x1400006u);
        xport_gte_write_data(0, r_u32(cursor + 24u)); xport_gte_write_data(1, fourth);
        first_area = xport_gte_read_data(24);
        {
            uint32 clut = r_u16(context + 936u), page = r_u16(context + 938u);
            xport_gte_execute(0x180001u);
            next[4] = r_u32(cursor + 48u); next[5] = r_u32(cursor + 52u);
            w_u32(packet + 4u, 0x2D000000u); w_u32(packet, 0x09000000u);
            w_u16(packet + 14u, clut); w_u16(packet + 22u, page);
        }
        if (!winding) first_area = 0u - first_area;
        flags |= vehicle_flags(0x8006D5E0u); sum += xport_gte_read_data(19);
        --count; xport_gte_execute(0x1400006u); result = flags;
        if ((sint32)flags >= 0)
        {
            depth = vehicle_asr(sum << ((uint32)shift & 31u), 15u); if (depth > 4095) depth = 4095;
            link = ordering + (uint32)depth * 4u; second_area = xport_gte_read_data(24);
            w_u32(packet + 32u, xport_gte_read_data(12));
            if (winding) second_area = 0u - second_area;
            result = first_area & second_area; w_u32(packet + 8u, xport_gte_read_data(13));
            if ((sint32)result >= 0)
            {
                w_u32(packet + 24u, xport_gte_read_data(14)); result = packet << 8;
                vehicle_link(packet, link); packet += 40u;
            }
        }
        cursor += 32u; for (i = 0; i < 6; ++i) words[i] = next[i];
    } while (count);
    w_u32(0x800BFE4Cu, packet); return result;
}

static sint32 vehicle_fallback_area(uint32 context)
{
    sint32 cx = (sint16)r_u16(context + 56u), cy = (sint16)r_u16(context + 58u), cz = (sint16)r_u16(context + 60u);
    uint32 x, y, z, a, b, c, restore0, restore2, restore4;
    gte_ctc2((uint32)((sint16)r_u16(context + 356u) - cx), 0);
    gte_ctc2((uint32)((sint16)r_u16(context + 358u) - cy), 2);
    gte_ctc2((uint32)((sint16)r_u16(context + 360u) - cz), 4);
    xport_gte_write_data(9, (uint32)((sint16)r_u16(context + 388u) - cx));
    xport_gte_write_data(10, (uint32)((sint16)r_u16(context + 390u) - cy));
    xport_gte_write_data(11, (uint32)((sint16)r_u16(context + 392u) - cz));
    a = (uint32)((sint16)r_u16(context + 324u) - cx); b = (uint32)((sint16)r_u16(context + 326u) - cy);
    xport_gte_execute(0x170000Cu); c = (uint32)((sint16)r_u16(context + 328u) - cz);
    x = xport_gte_read_data(25); y = xport_gte_read_data(26); z = xport_gte_read_data(27);
    xport_gte_write_data(9, a); xport_gte_write_data(10, b); xport_gte_write_data(11, c);
    gte_ctc2((x & 4095u) | ((y & 4095u) << 16), 16);
    a = (uint32)vehicle_asr(x, 12); b = (uint32)vehicle_asr(y, 12);
    gte_ctc2((z & 4095u) | ((a & 4095u) << 16), 17);
    c = (uint32)vehicle_asr(z, 12); a = (uint32)vehicle_asr(a, 12);
    gte_ctc2((b & 4095u) | ((c & 4095u) << 16), 18);
    b = (uint32)vehicle_asr(b, 12); c = (uint32)vehicle_asr(c, 12);
    gte_ctc2((a & 65535u) | (b << 16), 19); gte_ctc2(c, 20);
    restore0 = r_u32(context + 96u); restore2 = r_u32(context + 104u);
    xport_gte_execute(0x45E012u); restore4 = r_u32(context + 112u);
    gte_ctc2(restore0, 0); gte_ctc2(restore2, 2); gte_ctc2(restore4, 4);
    x = xport_gte_read_data(25); y = xport_gte_read_data(26); z = xport_gte_read_data(27);
    return (sint32)(z + (uint32)vehicle_asr(y + (uint32)vehicle_asr(x, 12), 12));
}

static void vehicle_save_vertex(uint32 context, uint32 offset, uint32 reg)
{
    w_u32(context + offset, xport_gte_read_data(reg)); w_u32(context + offset + 4u, xport_gte_read_data(reg + 1u));
}

static uint32 vehicle_depth_link(uint32 ordering, uint32 depth, uint32 shift)
{
    sint32 index = vehicle_asr(depth << (shift & 31u), 13); if (index > 4095) index = 4095;
    return ordering + (uint32)index * 4u;
}

static sint32 vehicle_max(sint32 a, sint32 b) { return a < b ? b : a; }

static sint32 vehicle_span(uint32 context, uint32 count, uint32 flags)
{
    uint32 i, offsets[4] = {332u, 364u, 396u, 428u};
    sint32 minimum = (sint16)r_u16(context + offsets[0]), maximum = minimum, value;
    for (i = 1; i < count; ++i) { value = (sint16)r_u16(context + offsets[i]); if (value < minimum) minimum = value; if (value > maximum) maximum = value; }
    if (maximum <= 0 || minimum >= (sint16)r_u16(context + 4u)) return 0;
    flags |= (uint32)(1023 - (maximum - minimum));
    minimum = maximum = (sint16)r_u16(context + offsets[0] + 2u);
    for (i = 1; i < count; ++i) { value = (sint16)r_u16(context + offsets[i] + 2u); if (value < minimum) minimum = value; if (value > maximum) maximum = value; }
    if (maximum <= 0 || minimum >= (sint16)r_u16(context + 6u)) return 0;
    flags |= (uint32)(511 - (maximum - minimum));
    return (sint32)flags < 0 ? -1 : 1;
}

static uint32 vehicle_clip(uint32 context, uint32 packet, uint32 link, uint32 count, uint32 color)
{
    w_u32(context + 168u, link); w_u32(context + 172u, packet); w_u32(context + 184u, count); w_u32(context + 156u, color);
    gtmain_polygon_clip_flat(context);
    return r_u32(context + 180u);
}

uint32 gtmain_model_shadow_draw(uint32 context, uint32 model)
{
    uint32 x = r_u32(context + 864u), cx = r_u32(context + 44u);
    uint32 y = r_u32(context + 868u), z = r_u32(context + 872u);
    uint32 cy = r_u32(context + 48u), cz = r_u32(context + 52u), bounds, records, packet, i, kind;
    sint32 exponent, minimum, difference, shift;
    x -= cx; y -= cy; z -= cz;
    w_u32(context + 268u, x); w_u32(context + 272u, y); w_u32(context + 276u, z);
    exponent = 31 - (sint32)vehicle_lz(vehicle_abs(x) | vehicle_abs(y) | vehicle_abs(z), 0x8006D72Cu);
    minimum = r_u16(model + 4u); if (exponent < minimum) exponent = minimum;
    difference = exponent - minimum; shift = exponent - 12; if (shift < 0) shift = 0;
    w_u16(context + 40u, shift); w_u16(context + 42u, difference);
    w_u16(context + 280u, vehicle_asr(x, shift)); w_u16(context + 282u, vehicle_asr(y, shift)); w_u16(context + 284u, vehicle_asr(z, shift));
    vehicle_translation(context);
    gtmain_matrix_multiply(psx_addr(context + 96u, 20u), psx_addr(context + 64u, 20u), psx_addr(context + 884u, 20u));
    gtmain_matrix_scale_gte(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), 4096u);
    gtmain_matrix_shift(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), difference);
    gtmain_model_matrix_bind(context, psx_addr(context + 96u, 20u)); gtmain_lighting_bind(context);
    {
        uint32 a = r_u32(model + 8u), b = r_u32(model + 16u), low = r_u16(model + 12u), high = r_u16(model + 20u);
        for (i = 0; i < 8; ++i)
        {
            w_u16(context + 184u + i * 8u, i & 1u ? b : a);
            w_u16(context + 186u + i * 8u, (i & 2u ? b : a) >> 16);
            w_u16(context + 188u + i * 8u, i & 4u ? high : low);
        }
    }
    bounds = gtmain_model_bounds_classify(context, psx_addr(context + 184u, 72u), 8u);
    if (bounds & 31u) return bounds & 31u;
    gtmain_matrix_transpose(psx_addr(context + 248u, 20u), psx_addr(context + 884u, 20u));
    gtmain_matrix_scale_gte(psx_addr(context + 248u, 20u), psx_addr(context + 248u, 20u), (uint16)-4096);
    gtmain_matrix_rotate_short(psx_addr(context + 56u, 8u), psx_addr(context + 248u, 20u), psx_addr(context + 280u, 8u));
    vehicle_rotation(context + 96u); records = r_u32(model + 24u);
    packet = r_u32(0x800BFE4Cu); w_u32(context + 932u, packet);
    for (kind = 0; kind < 2; ++kind)
    {
        uint32 count = r_u16(model + kind * 2u), stride = kind ? 32u : 24u, words[6], next[6];
        uint32 cached_ordering = 0, cached_shift = 0;
        if (count) vehicle_words(records, words);
        if (count && !kind)
        {
            if (!((bounds >> 16) & 63u)) cached_ordering = r_u32(context + 928u);
            cached_shift = r_u16(context + 40u);
        }
        while (count)
        {
            uint32 flags = 0, first_area, area, link, ordering, threshold, clipped = (bounds >> 16) & 63u;
            sint32 depth, span;
            xport_gte_write_data(0, words[kind ? 2 : 4]); xport_gte_write_data(1, words[kind ? 3 : 5]);
            xport_gte_write_data(2, words[kind ? 4 : 2]); xport_gte_write_data(3, words[kind ? 5 : 3]);
            xport_gte_write_data(4, words[0]); xport_gte_write_data(5, words[1]);
            w_u32(packet, kind ? 0x05000000u : 0x04000000u); xport_gte_execute(0x280030u);
            --count; for (i = 0; i < 4; ++i) next[i] = r_u32(records + stride + i * 4u);
            if (clipped) flags = vehicle_flags(kind ? 0x8006DF1Cu : 0x8006DC00u);
            if (kind)
            {
                w_u32(packet + 12u, xport_gte_read_data(12));
                if (clipped) { w_u32(context + 364u, xport_gte_read_data(12)); vehicle_save_vertex(context, 356u, 0); }
            }
            depth = (sint32)xport_gte_read_data(17);
            ordering = !kind && !clipped ? cached_ordering : r_u32(context + 928u);
            threshold = clipped ? r_u16(context + 8u) : 0;
            xport_gte_execute(0x1400006u);
            if (kind) { xport_gte_write_data(0, r_u32(records + 24u)); xport_gte_write_data(1, r_u32(records + 28u)); }
            else { next[4] = r_u32(records + 40u); next[5] = r_u32(records + 44u); }
            first_area = xport_gte_read_data(24);
            if (!kind && !clipped)
            {
                w_u32(packet + 4u, 0x20000000u);
                if ((sint32)first_area < 0) goto vehicle_next;
            }
            depth = vehicle_max(depth, (sint32)xport_gte_read_data(18)); depth = vehicle_max(depth, (sint32)xport_gte_read_data(19));
            if (kind)
            {
                xport_gte_execute(0x180001u); w_u32(packet + 4u, 0x28000000u);
                first_area = 0u - first_area;
                next[4] = r_u32(records + 48u); next[5] = r_u32(records + 52u);
                if (clipped) flags |= vehicle_flags(0x8006DFACu);
                depth = vehicle_max(depth, (sint32)xport_gte_read_data(19)); xport_gte_execute(0x1400006u);
                area = first_area & xport_gte_read_data(24);
            }
            else
            {
                area = first_area;
                if (clipped)
                {
                    w_u32(packet + 16u, xport_gte_read_data(12)); w_u32(packet + 12u, xport_gte_read_data(13));
                }
            }
            if (clipped && depth < (sint32)threshold) goto vehicle_next;
            link = vehicle_depth_link(ordering, (uint32)depth, kind ? r_u16(context + 40u) : cached_shift);
            if (!clipped)
            {
                if (kind) { w_u32(packet + 20u, xport_gte_read_data(12)); w_u32(packet + 8u, xport_gte_read_data(13)); }
                if ((sint32)area >= 0)
                {
                    if (kind) w_u32(packet + 16u, xport_gte_read_data(14));
                    else { w_u32(packet + 16u, xport_gte_read_data(12)); w_u32(packet + 12u, xport_gte_read_data(13)); w_u32(packet + 8u, xport_gte_read_data(14)); }
                    vehicle_link(packet, link); packet += kind ? 24u : 20u;
                }
                goto vehicle_next;
            }
            if ((sint32)flags < 0)
            {
                vehicle_save_vertex(context, kind ? 420u : 388u, 0);
                vehicle_save_vertex(context, kind ? 388u : 356u, 2); vehicle_save_vertex(context, 324u, 4);
                area = (uint32)vehicle_fallback_area(context);
            }
            if (kind) w_u32(context + 396u, xport_gte_read_data(12));
            else w_u32(packet + 8u, xport_gte_read_data(14));
            if ((sint32)area < 0) goto vehicle_next;
            if (kind) { w_u32(context + 332u, xport_gte_read_data(13)); w_u32(context + 428u, xport_gte_read_data(14)); }
            else { w_u32(context + 396u, xport_gte_read_data(12)); w_u32(context + 364u, xport_gte_read_data(13)); w_u32(context + 332u, xport_gte_read_data(14)); }
            span = vehicle_span(context, kind ? 4u : 3u, flags); if (!span) goto vehicle_next;
            if (!kind) w_u32(packet + 4u, 0x20000000u);
            if (span > 0)
            {
                vehicle_link(packet, link);
                if (kind) { w_u32(packet + 20u, xport_gte_read_data(12)); w_u32(packet + 8u, xport_gte_read_data(13)); w_u32(packet + 16u, xport_gte_read_data(14)); }
                packet += kind ? 24u : 20u;
            }
            else
            {
                vehicle_save_vertex(context, kind ? 420u : 388u, 0); vehicle_save_vertex(context, kind ? 388u : 356u, 2); vehicle_save_vertex(context, 324u, 4);
                packet = vehicle_clip(context, packet, link, kind ? 4u : 3u, kind ? 0x28000000u : 0x20000000u);
                vehicle_words(records + stride, next);
            }
vehicle_next:
            records += stride; for (i = 0; i < 6; ++i) words[i] = next[i];
        }
        w_u32(context + 932u, packet);
    }
    packet = r_u32(context + 932u); w_u32(0x800BFE4Cu, packet); return packet;
}
