#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static sint32 geometry_divide(sint32 numerator, sint32 denominator, uint32 address)
{
    if (!denominator) gt_draft_failure("Original geometry BREAK 7", address);
    if (numerator == (sint32)0x80000000u && denominator == -1) gt_draft_failure("Original geometry BREAK 6", address);
    return numerator / denominator;
}

static uint32 geometry_lz_count(uint32 value)
{
    xport_gte_write_data(30u, value);
    return xport_gte_read_data(31u);
}

static void geometry_matrix(uint32 source, uint32 reg)
{
    uint32 first = r_u32(source), second = r_u32(source + 4u);
    gte_ctc2(first, reg); first = r_u32(source + 8u); gte_ctc2(second, reg + 1u);
    second = r_u32(source + 12u); gte_ctc2(first, reg + 2u);
    first = r_u32(source + 16u); gte_ctc2(second, reg + 3u); gte_ctc2(first, reg + 4u);
}

static uint32 geometry_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

uint32 gtmain_view_plane_intersections(uint32 view, const uint8 *plane)
{
    sint32 px = (sint16)xport_load_le16(plane), py = (sint16)xport_load_le16(plane + 2u);
    uint32 a = (uint32)(px * (sint16)r_u16(view + 36u));
    uint32 b = (uint32)(px * (sint16)r_u16(view + 40u));
    uint32 c = (uint32)(py * (sint16)r_u16(view + 42u));
    uint32 d = (uint32)(py * (sint16)r_u16(view + 38u));
    uint32 bias = xport_load_le16(plane + 4u), values[4], flags = 0;
    values[0] = bias + (uint32)((sint32)a >> 12) + (uint32)((sint32)d >> 12);
    values[1] = bias + (b >> 12) + (uint32)((sint32)d >> 12);
    values[2] = bias + (uint32)((sint32)a >> 12) + (c >> 12);
    values[3] = bias + (b >> 12) + (c >> 12);
    if (!(values[0] & 0x8000u)) flags = 0x31u;
    if (!(values[1] & 0x8000u)) flags = (flags | 2u) ^ 0x60u;
    if (!(values[2] & 0x8000u)) flags = (flags | 4u) ^ 0x90u;
    if (!(values[3] & 0x8000u)) flags = (flags | 8u) ^ 0xC0u;
    if (flags & 0x10u)
    {
        sint32 first = (sint16)values[0], second = (sint16)values[2];
        sint32 fraction = geometry_divide((sint16)r_u16(view + 20u) * first, second - first, 0x8006A48Cu);
        w_u16(view + 62u, r_u16(view + 2u) - (uint32)fraction);
    }
    if (flags & 0x20u)
    {
        sint32 first = (sint16)values[0], second = (sint16)values[1];
        sint32 fraction = geometry_divide((sint16)r_u16(view + 18u) * first, second - first, 0x8006A4F0u);
        w_u16(view + 64u, r_u16(view) - (uint32)fraction);
    }
    if (flags & 0x40u)
    {
        sint32 first = (sint16)values[1], second = (sint16)values[3];
        sint32 fraction = geometry_divide((sint16)r_u16(view + 20u) * first, second - first, 0x8006A554u);
        w_u16(view + 70u, r_u16(view + 2u) - (uint32)fraction);
    }
    if (flags & 0x80u)
    {
        sint32 first = (sint16)values[2], second = (sint16)values[3];
        sint32 fraction = geometry_divide((sint16)r_u16(view + 18u) * first, second - first, 0x8006A5B8u);
        w_u16(view + 72u, r_u16(view) - (uint32)fraction);
    }
    return flags & 15u;
}

uint32 gtmain_model_matrix_bind(uint32 context, const uint8 *matrix)
{
    uint32 negated = 0u - xport_load_le32(matrix + 8u);
    uint32 word1 = xport_load_le32(matrix + 4u), word0 = xport_load_le32(matrix);
    uint32 word3 = xport_load_le32(matrix + 12u), word4 = xport_load_le32(matrix + 16u);
    word1 = (word1 ^ 0xFFFF0000u) + 65536u;
    if (negated & 65535u) negated += 65536u;
    gtmain_matrix_bind_packed_negation(matrix);
    /* Readbacks retain the packed words written before any context stores */
    w_u32(context + 96u, word0); w_u32(context + 100u, word1);
    w_u32(context + 104u, negated); w_u32(context + 108u, word3);
    /* Rotation control4 exposes its signed low half */
    word4 = (uint32)(sint32)(sint16)word4;
    w_u32(context + 112u, word4);
    return word4;
}

sint32 gtmain_model_transform_draw(uint32 context, uint32 variants, uint32 mode)
{
    uint32 x, y, z, ax, ay, az, swap, distance, extent, selected, model, bits, corners, index;
    sint32 order, exponent, difference, shift;
    x = r_u32(context + 864u) - r_u32(context + 44u);
    y = r_u32(context + 868u) - r_u32(context + 48u); z = r_u32(context + 872u) - r_u32(context + 52u);
    w_u32(context + 268u, x); w_u32(context + 272u, y); w_u32(context + 276u, z);
    ax = geometry_abs(x); ay = geometry_abs(y); az = geometry_abs(z);
    if ((sint32)ax < (sint32)ay) { swap = ax; ax = ay; ay = swap; }
    if ((sint32)ax < (sint32)az) { swap = ax; ax = az; az = swap; }
    if ((sint32)ay < (sint32)az) { swap = ay; ay = az; az = swap; }
    distance = ax + (uint32)((sint32)ay >> 1) + (uint32)((sint32)az >> 2);
    extent = xport_load_le32(gtmain_render_source(context) + 28u) << 4;
    selected = (uint32)gtmain_model_range_index(variants, (const uint8 *)psx_addr(context + 268u, 12u), (sint32)extent);
    if (selected == 0xFFFFFFFFu) return -1;
    if (mode) selected = 2u * (mode != 1u);
    model = r_u32(variants + selected * 8u + 8u);
    exponent = (sint32)(31u - geometry_lz_count(geometry_abs(x) | geometry_abs(y) | geometry_abs(z)));
    order = (sint16)r_u16(model + 76u) + 1;
    if (exponent < order) exponent = order;
    difference = (sint32)((uint32)exponent - (uint32)order); shift = (sint32)((uint32)exponent - 13u);
    if (shift < 0) shift = 0;
    w_u16(context + 40u, shift); w_u16(context + 42u, difference);
    w_u16(context + 280u, (sint32)x >> ((uint32)shift & 31u));
    w_u16(context + 282u, (sint32)y >> ((uint32)shift & 31u));
    w_u16(context + 284u, (sint32)z >> ((uint32)shift & 31u));
    gtmain_matrix_bind_packed_negation(psx_addr(context + 64u, 20u));
    xport_gte_write_data(0, r_u32(context + 280u)); xport_gte_write_data(1, r_u32(context + 284u)); xport_gte_execute(0x486012u);
    for (index = 0; index < 3u; ++index) w_u32(context + 84u + index * 4u, xport_gte_read_data(9u + index));
    x = r_u32(context + 84u); y = r_u32(context + 88u); gte_ctc2(x, 5);
    x = r_u32(context + 92u); gte_ctc2(y, 6); gte_ctc2(x, 7);
    gtmain_matrix_multiply(psx_addr(context + 948u, 20u), psx_addr(context + 64u, 20u), psx_addr(context + 884u, 20u));
    gtmain_matrix_scale_gte(psx_addr(context + 96u, 20u), psx_addr(context + 948u, 20u), 4096);
    gtmain_matrix_shift(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), difference);
    gtmain_model_matrix_bind(context, psx_addr(context + 96u, 20u)); gtmain_lighting_bind(context);
    {
        uint32 a = r_u32(model + 60u), b = r_u32(model + 68u);
        uint32 low_z = r_u16(model + 64u), high_z = r_u16(model + 72u);
        for (index = 0; index < 8u; ++index)
        {
            uint32 point = context + 184u + index * 8u;
            w_u16(point, index & 1u ? b : a); w_u16(point + 2u, index & 2u ? b >> 16 : a >> 16); w_u16(point + 4u, index & 4u ? high_z : low_z);
        }
    }
    bits = gtmain_model_bounds_classify(context, psx_addr(context + 184u, 72u), 8);
    if (bits & 31u) return -1;
    gtmain_matrix_transpose(psx_addr(context + 248u, 20u), psx_addr(context + 884u, 20u));
    gtmain_matrix_scale_gte(psx_addr(context + 248u, 20u), psx_addr(context + 248u, 20u), (uint16)-4096);
    gtmain_matrix_rotate_short(psx_addr(context + 56u, 8u), psx_addr(context + 248u, 20u), psx_addr(context + 280u, 8u));
    if (r_u32(context + 912u))
    {
        sint32 denominator = (sint32)(uint32)(((sint64)((sint32)extent >> 8) * 2560) >> 8);
        sint32 ratio = geometry_divide((sint32)((uint32)((sint32)distance >> 8) << 12), denominator, 0x8006CF88u);
        sint32 factor = 4096 - (sint16)r_u16(context + 924u), intensity;
        uint32 rgb;
        if (factor < 0) factor = 0;
        intensity = (sint32)((80u - (uint32)((sint32)((uint32)ratio * 80u) >> 12)) * (uint32)factor) >> 12;
        if (intensity < 0) intensity = 0; else if (intensity >= 64) intensity = 64;
        rgb = (uint32)intensity | (uint32)intensity << 8 | (uint32)intensity << 16;
        w_u32(context + 968u, rgb | 0x26000000u); w_u32(context + 972u, rgb | 0x2E000000u);
        gtmain_matrix_shift(psx_addr(context + 948u, 20u), psx_addr(context + 948u, 20u), (sint16)r_u16(context + 910u));
        if (!r_u16(context + 908u)) gtmain_matrix_negate_axes(psx_addr(context + 948u, 20u), psx_addr(context + 948u, 20u));
        gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 920u), 13); gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 922u), 14);
        geometry_matrix(context + 948u, 8);
    }
    geometry_matrix(context + 96u, 0);
    w_u32(context + 936u, model); x = r_u32(model); y = r_u32(0x800BFE4Cu); w_u32(context + 940u, x);
    x = r_u32(model + 4u); w_u32(context + 944u, x); w_u32(context + 932u, y);
    corners = (bits >> 16) & 63u;
    if (corners) gtmain_model_primitives_clipped_draw(model, context);
    else gtmain_model_primitives_draw(model, context);
    w_u32(0x800BFE4Cu, r_u32(context + 932u));
    return (sint32)selected;
}

static void geometry_vertex_packet(uint32 packet, uint32 vertex, uint32 offset)
{
    uint32 screen = r_u32(vertex + 8u), uv = r_u32(vertex + 12u);
    w_u32(packet + offset, screen); uv >>= 7;
    w_u8(packet + offset + 4u, uv); w_u8(packet + offset + 5u, uv >> 16);
}

static void geometry_link24(uint32 address, uint32 value)
{
    w_u8(address, value); w_u8(address + 1u, value >> 8); w_u8(address + 2u, value >> 16);
}

uint32 gtmain_polygon_clip_draw(uint32 context)
{
    uint32 either = 0, every = 0xFFFFFFFFu, list = context + 204u, vertex = context + 324u;
    uint32 count, threshold, packet, plane, remaining, result = 0;
    xport_gte_write_data(0, r_u32(vertex)); xport_gte_write_data(1, r_u32(vertex + 4u)); xport_gte_execute(0x180001u);
    geometry_matrix(context + 116u, 8); geometry_matrix(context + 136u, 16);
    count = (uint32)(sint32)(sint16)r_u16(context + 184u); packet = r_u32(context + 172u); threshold = r_u16(context + 8u);
    w_u32(context + 192u, list); w_u32(context + 196u, context + 264u);
    w_u32(context + 200u, vertex + count * 32u); w_u32(context + 176u, packet); w_u32(context + 180u, packet);
    while (count)
    {
        uint32 x = xport_gte_read_data(9), y = xport_gte_read_data(10), z = xport_gte_read_data(11), bits, a, b;
        w_u32(vertex + 8u, xport_gte_read_data(14)); w_u32(list, vertex); list += 4u;
        xport_gte_execute(0x4BE012u); w_u16(vertex, x); w_u16(vertex + 2u, y); w_u16(vertex + 4u, z);
        xport_gte_write_data(0, r_u32(vertex)); xport_gte_write_data(1, r_u32(vertex + 4u));
        a = xport_gte_read_data(9); b = xport_gte_read_data(10); z -= threshold;
        xport_gte_execute(0x4C6012u); bits = z >> 31;
        w_u16(vertex + 20u, z); w_u16(vertex + 22u, a); if ((sint32)a < 0) bits |= 2u;
        w_u16(vertex + 24u, b); if ((sint32)b < 0) bits |= 4u;
        a = xport_gte_read_data(9); b = xport_gte_read_data(10);
        xport_gte_write_data(0, r_u32(vertex + 32u)); xport_gte_write_data(1, r_u32(vertex + 36u));
        --count; vertex += 32u; xport_gte_execute(0x180001u);
        w_u16(vertex - 6u, a); if ((sint32)a < 0) bits |= 8u;
        w_u16(vertex - 4u, b); if ((sint32)b < 0) bits |= 16u;
        w_u16(vertex - 2u, bits); either |= bits; every &= bits;
    }
    w_u16(context + 186u, either);
    if (every) return 0;
    gte_ctc2(1, 27); gte_ctc2(0, 28); gte_ctc2(0, 5); gte_ctc2(0, 6); gte_ctc2(0, 7);
    gte_ctc2(4096, 0); gte_ctc2(0, 1); gte_ctc2(4096, 2); gte_ctc2(0, 3); gte_ctc2(4096, 4);
    remaining = r_u16(context + 186u);
    for (plane = 0; plane < 5u && remaining; ++plane)
    {
        uint32 mask = 1u << plane, source, destination, destination_start;
        if (!(remaining & mask)) continue;
        remaining -= remaining & mask;
        source = r_u32(context + 192u); destination = r_u32(context + 196u);
        count = (uint32)(sint32)(sint16)r_u16(context + 184u); destination_start = destination;
        w_u32(context + 192u, destination); w_u32(context + 196u, source);
        w_u32(source + count * 4u, r_u32(source)); w_u16(context + 188u, 2);
        while (count)
        {
            uint32 inside = r_u32(source), outside = r_u32(source + 4u), first_flags = r_u16(inside + 30u) & mask, second_flags = r_u16(outside + 30u) & mask;
            if (!first_flags) { w_u32(destination, inside); destination += 4u; }
            --count;
            if (first_flags != second_flags)
            {
                uint32 budget = (uint32)(sint32)(sint16)r_u16(context + 188u) - 1u;
                if (first_flags) { uint32 swap = inside; inside = outside; outside = swap; }
                if (budget != 0xFFFFFFFFu)
                {
                    sint32 distance, other; uint32 a, b, new_vertex, fraction, x, y, z, bits;
                    w_u16(context + 188u, budget);
                    distance = (sint16)r_u16(inside + plane * 2u + 20u); other = (sint16)r_u16(outside + plane * 2u + 20u);
                    if (distance)
                    {
                        gte_ctc2((uint32)distance, 26); gte_ctc2((uint32)distance - (uint32)other, 4);
                        xport_gte_write_data(0, 0); xport_gte_write_data(1, 4096);
                        a = r_u32(inside); b = r_u32(outside); xport_gte_execute(0x180001u);
                        gte_ctc2(a, 8); gte_ctc2(b, 10); gte_ctc2((a & 0xFFFF0000u) | (b & 0xFFFFu), 9);
                        a = r_u32(inside + 4u); b = r_u32(outside + 4u); gte_ctc2(a, 11); gte_ctc2(b, 12);
                        a = r_u16(context + 32u); fraction = (uint32)((sint32)xport_gte_read_data(24) >> 4); gte_ctc2(a, 26);
                        xport_gte_write_data(2, (4096u - fraction) & 0xFFFFu); xport_gte_write_data(3, fraction);
                        new_vertex = r_u32(context + 200u); destination += 4u; xport_gte_execute(0x4AE012u);
                        w_u32(context + 200u, new_vertex + 32u);
                        a = r_u16(inside + 12u); b = r_u16(inside + 14u); gte_ctc2(a, 16);
                        a = r_u16(outside + 12u); gte_ctc2(b, 19); b = r_u16(outside + 14u); gte_ctc2(a, 17); gte_ctc2(b, 20);
                        x = xport_gte_read_data(9); y = xport_gte_read_data(10); z = xport_gte_read_data(11);
                        w_u16(new_vertex, x); if (!plane) z = r_u16(context + 8u);
                        xport_gte_execute(0x4CE012u); w_u16(new_vertex + 2u, y); w_u16(new_vertex + 4u, z);
                        gte_ctc2(4096, 4); xport_gte_write_data(0, r_u32(new_vertex)); xport_gte_write_data(1, r_u32(new_vertex + 4u));
                        a = xport_gte_read_data(9); b = xport_gte_read_data(11); w_u16(new_vertex + 12u, a); w_u16(new_vertex + 14u, b);
                        a = r_u32(context + 136u); b = r_u32(context + 140u); xport_gte_execute(0x180001u);
                        geometry_matrix(context + 116u, 8); gte_ctc2(a, 16); w_u32(new_vertex + 8u, xport_gte_read_data(14)); gte_ctc2(b, 17);
                        a = r_u32(context + 144u); b = r_u32(context + 148u); xport_gte_execute(0x4A6012u);
                        gte_ctc2(a, 18); a = r_u32(context + 152u); gte_ctc2(b, 19); gte_ctc2(a, 20);
                        a = xport_gte_read_data(9); b = xport_gte_read_data(10); destination_start = r_u32(context + 192u);
                        w_u32(destination - 4u, new_vertex); xport_gte_execute(0x4C6012u);
                        bits = (uint32)((sint32)a >> 31) & 2u; w_u16(new_vertex + 22u, a); w_u16(new_vertex + 24u, b);
                        if ((sint32)b < 0) bits |= 4u;
                        a = xport_gte_read_data(9); b = xport_gte_read_data(10);
                        w_u16(new_vertex + 26u, a); if ((sint32)a < 0) bits |= 8u;
                        w_u16(new_vertex + 28u, b); if ((sint32)b < 0) bits |= 16u;
                        w_u16(new_vertex + 30u, bits);
                    }
                }
            }
            source += 4u;
        }
        count = (uint32)((sint32)(destination - destination_start) >> 2); w_u16(context + 184u, count);
        if ((sint32)count < 3) goto restore;
    }
    {
        uint32 color = r_u32(context + 156u) | 0x08000000u, clut = r_u16(context + 164u), tpage = r_u16(context + 166u);
        uint32 left = r_u32(context + 192u), right, first, last, last_packet = 0, link, old_link;
        count = (uint32)(sint32)(sint16)r_u16(context + 184u); packet = r_u32(context + 172u);
        right = left + count * 4u - 4u; first = r_u32(left); left += 4u; last = r_u32(right); right -= 4u;
        while (left < right)
        {
            uint32 next = packet + 40u;
            geometry_link24(packet, next); w_u8(packet + 3u, 9);
            geometry_vertex_packet(packet, first, 8); geometry_vertex_packet(packet, last, 24);
            first = r_u32(left); left += 4u; last = r_u32(right); right -= 4u;
            geometry_vertex_packet(packet, first, 16); geometry_vertex_packet(packet, last, 32);
            last_packet = packet; w_u32(packet + 4u, color); w_u16(packet + 14u, clut); w_u16(packet + 22u, tpage); packet = next;
        }
        w_u32(context + 180u, packet);
        if (left == right)
        {
            uint32 middle;
            geometry_vertex_packet(packet, first, 8); geometry_vertex_packet(packet, last, 24); middle = r_u32(left);
            w_u32(packet, 0x07000000u); w_u32(packet + 4u, color & 0xF7FFFFFFu); w_u16(packet + 14u, clut); w_u16(packet + 22u, tpage);
            geometry_vertex_packet(packet, middle, 16); last_packet = packet; w_u32(context + 180u, packet + 32u);
        }
        link = r_u32(context + 168u); old_link = r_u8(link) | (uint32)r_u8(link + 1u) << 8 | (uint32)r_u8(link + 2u) << 16;
        geometry_link24(link, r_u32(context + 172u)); geometry_link24(last_packet, old_link); result = 1;
    }
restore:
    {
        uint32 a = r_u16(context + 34u), b = r_u32(context + 36u); gte_ctc2(a, 27); gte_ctc2(b, 28);
        a = r_u32(context + 12u); b = r_u32(context + 16u); gte_ctc2(a, 21); a = r_u32(context + 20u); gte_ctc2(b, 22); gte_ctc2(a, 23);
        a = r_u32(context + 84u); b = r_u32(context + 88u); gte_ctc2(a, 5); a = r_u32(context + 92u); gte_ctc2(b, 6); gte_ctc2(a, 7);
        geometry_matrix(context + 96u, 0);
    }
    return result;
}


