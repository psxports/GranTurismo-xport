#include "game_draft_registry.h"
#include "psx_gte.h"

static void gouraud_texture_vertex(uint32 slot, uint32 address)
{
    gte_lwc2(slot * 2u, r_u32(address));
    gte_lwc2(slot * 2u + 1u, r_u32(address + 4u));
}

static void gouraud_texture_planes(uint32 context)
{
    uint32 i;
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 116u + 4u * i), 8u + i);
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 136u + 4u * i), 16u + i);
}

static void gouraud_texture_restore(uint32 context)
{
    uint32 i;
    gte_ctc2(r_u16(context + 34u), 27u);
    gte_ctc2(r_u32(context + 36u), 28u);
    for (i = 0u; i < 3u; ++i) gte_ctc2(r_u32(context + 12u + i * 4u), 21u + i);
    for (i = 0u; i < 3u; ++i) gte_ctc2(r_u32(context + 84u + i * 4u), 5u + i);
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 96u + i * 4u), i);
}

static uint32 gouraud_texture_intersection(uint32 context, uint32 positive, uint32 negative,
    uint32 plane, uint32 destination, uint32 output)
{
    uint32 distance = (uint32)(sint32)(sint16)r_u16(positive + 20u + plane * 2u);
    uint32 other = (uint32)(sint32)(sint16)r_u16(negative + 20u + plane * 2u);
    uint32 fraction, first, second, x, y, z, u, v, side[4], flags;
    if (!distance) return 0u;
    gte_ctc2(distance, 26u); gte_ctc2(distance - other, 4u);
    gte_lwc2(0u, 0u); gte_lwc2(1u, 4096u);
    first = r_u32(positive); second = r_u32(negative);
    xport_gte_execute(0x180001u);
    gte_ctc2(first, 8u); gte_ctc2(second, 10u);
    gte_ctc2((first & 0xFFFF0000u) | (second & 65535u), 9u);
    gte_ctc2(r_u32(positive + 4u), 11u);
    gte_ctc2(r_u32(negative + 4u), 12u);
    fraction = xport_gte_read_data(24u);
    gte_ctc2(r_u16(context + 32u), 26u);
    fraction = (uint32)((sint32)fraction >> 4);
    gte_lwc2(2u, (4096u - fraction) & 65535u);
    gte_lwc2(3u, fraction);
    xport_gte_execute(0x4AE012u);
    w_u32(context + 200u, destination + 32u);
    gte_ctc2(r_u16(positive + 12u), 16u);
    gte_ctc2(r_u16(positive + 14u), 19u);
    gte_ctc2(r_u16(negative + 12u), 17u);
    gte_ctc2(r_u16(negative + 14u), 20u);
    x = xport_gte_read_data(9u); y = xport_gte_read_data(10u); z = xport_gte_read_data(11u);
    gte_lwc2(8u, fraction);
    w_u16(destination, x);
    if (!plane) z = r_u16(context + 8u);
    xport_gte_execute(0x4CE012u);
    w_u16(destination + 2u, y); w_u16(destination + 4u, z);
    gte_ctc2(4096u, 4u);
    gouraud_texture_vertex(0u, destination);
    u = xport_gte_read_data(9u); v = xport_gte_read_data(11u);
    w_u16(destination + 12u, u); w_u16(destination + 14u, v);
    xport_gte_execute(0x180001u);
    gte_ctc2(r_u32(context + 116u), 8u);
    gte_ctc2(r_u32(context + 120u), 9u);
    gte_ctc2(r_u32(context + 124u), 10u);
    gte_ctc2(r_u32(context + 128u), 11u);
    gte_ctc2(r_u32(context + 132u), 12u);
    gte_ctc2(r_u32(context + 136u), 16u);
    w_u32(destination + 8u, xport_gte_read_data(14u));
    gte_ctc2(r_u32(context + 140u), 17u);
    xport_gte_execute(0x4A6012u);
    gte_ctc2(r_u32(context + 144u), 18u);
    gte_ctc2(r_u32(context + 148u), 19u);
    gte_ctc2(r_u32(context + 152u), 20u);
    side[0] = xport_gte_read_data(9u); side[1] = xport_gte_read_data(10u);
    w_u32(output, destination);
    xport_gte_execute(0x4C6012u);
    gte_lwc2(6u, r_u32(negative + 16u));
    gte_ctc2(r_u8(positive + 16u) << 4u, 21u);
    gte_ctc2(r_u8(positive + 17u) << 4u, 22u);
    gte_ctc2(r_u8(positive + 18u) << 4u, 23u);
    side[2] = xport_gte_read_data(9u); side[3] = xport_gte_read_data(10u);
    flags = (side[0] >> 31u) << 1u;
    xport_gte_execute(0x780010u);
    w_u16(destination + 22u, side[0]); w_u16(destination + 24u, side[1]);
    if ((sint32)side[1] < 0) flags |= 4u;
    w_u16(destination + 26u, side[2]);
    if ((sint32)side[2] < 0) flags |= 8u;
    w_u16(destination + 28u, side[3]);
    if ((sint32)side[3] < 0) flags |= 16u;
    w_u16(destination + 30u, flags);
    w_u32(destination + 16u, xport_gte_read_data(22u));
    return 1u;
}

static void gouraud_texture_packet_corner(uint32 packet, uint32 offset, uint32 vertex)
{
    uint32 color = r_u32(vertex + 16u), coordinates = r_u32(vertex + 8u), uv;
    w_u32(packet + offset, color);
    uv = r_u32(vertex + 12u);
    w_u32(packet + offset + 4u, coordinates);
    uv >>= 7u;
    w_u8(packet + offset + 8u, uv);
    w_u8(packet + offset + 9u, uv >> 16u);
}

static void gouraud_texture_link24(uint32 address, uint32 value)
{
    w_u16(address, value); w_u8(address + 2u, value >> 16u);
}

uint32 gtos_model_clip_gouraud_texture(uint32 context)
{
    uint32 any = 0u, common = 0xFFFFFFFFu, list, vertex, packet, near_plane, plane;
    sint32 count = (sint16)r_u16(context + 184u);
    uint32 result = 0u;
    gouraud_texture_vertex(0u, context + 324u);
    xport_gte_execute(0x180001u);
    gouraud_texture_planes(context);
    list = context + 204u; vertex = context + 324u;
    packet = r_u32(context + 172u); near_plane = r_u16(context + 8u);
    w_u32(context + 192u, context + 204u);
    w_u32(context + 196u, context + 264u);
    w_u32(context + 200u, context + 324u + (uint32)count * 32u);
    w_u32(context + 176u, packet); w_u32(context + 180u, packet);
    while (count) {
        uint32 x = xport_gte_read_data(9u), y = xport_gte_read_data(10u), z = xport_gte_read_data(11u);
        uint32 side0, side1, side2, side3, bits, distance;
        w_u32(vertex + 8u, xport_gte_read_data(14u));
        w_u32(list, vertex); list += 4u;
        xport_gte_execute(0x4BE012u);
        w_u16(vertex, x); w_u16(vertex + 2u, y); w_u16(vertex + 4u, z);
        gouraud_texture_vertex(0u, vertex);
        side0 = xport_gte_read_data(9u); side1 = xport_gte_read_data(10u);
        distance = z - near_plane;
        xport_gte_execute(0x4C6012u);
        bits = distance >> 31u;
        w_u16(vertex + 20u, distance); w_u16(vertex + 22u, side0);
        if ((sint32)side0 < 0) bits |= 2u;
        w_u16(vertex + 24u, side1);
        if ((sint32)side1 < 0) bits |= 4u;
        side2 = xport_gte_read_data(9u); side3 = xport_gte_read_data(10u);
        gouraud_texture_vertex(0u, vertex + 32u);
        --count;
        xport_gte_execute(0x180001u);
        w_u16(vertex + 26u, side2);
        if ((sint32)side2 < 0) bits |= 8u;
        w_u16(vertex + 28u, side3);
        if ((sint32)side3 < 0) bits |= 16u;
        w_u16(vertex + 30u, bits);
        any |= bits; common &= bits; vertex += 32u;
    }
    w_u16(context + 186u, any);
    if (common) return 0u;
    gte_ctc2(1u, 27u); gte_ctc2(0u, 28u);
    gte_ctc2(0u, 5u); gte_ctc2(0u, 6u); gte_ctc2(0u, 7u);
    gte_ctc2(4096u, 0u); gte_ctc2(0u, 1u); gte_ctc2(4096u, 2u);
    gte_ctc2(0u, 3u); gte_ctc2(4096u, 4u);
    any = r_u16(context + 186u);
    for (plane = 0u; plane < 5u && any; ++plane) {
        uint32 mask = 1u << plane, input, output, output_start;
        if (!(any & mask)) continue;
        any -= any & mask;
        input = r_u32(context + 192u); output = r_u32(context + 196u);
        count = (sint16)r_u16(context + 184u); output_start = output;
        w_u32(context + 192u, output); w_u32(context + 196u, input);
        w_u32(input + (uint32)count * 4u, r_u32(input));
        w_u16(context + 188u, 2u);
        while (count) {
            uint32 first = r_u32(input), second = r_u32(input + 4u);
            uint32 first_bits = r_u16(first + 30u) & mask, second_bits = r_u16(second + 30u) & mask;
            if (!first_bits) { w_u32(output, first); output += 4u; }
            --count;
            if (first_bits != second_bits) {
                sint32 intersections = (sint16)r_u16(context + 188u) - 1;
                if (first_bits) { uint32 swap = first; first = second; second = swap; }
                if (intersections != -1) {
                    w_u16(context + 188u, (uint32)intersections);
                    if (r_u16(first + 20u + plane * 2u)) {
                        uint32 destination = r_u32(context + 200u);
                        gouraud_texture_intersection(context, first, second, plane, destination, output);
                        output += 4u;
                        output_start = r_u32(context + 192u);
                    }
                }
            }
            input += 4u;
        }
        count = (sint32)(output - output_start) >> 2;
        w_u16(context + 184u, (uint32)count);
        if (count < 3) goto restore;
    }
    {
        uint32 front = r_u32(context + 192u), back;
        uint32 first, last, last_packet = 0u;
        uint32 clut = r_u16(context + 164u), tpage = r_u16(context + 166u);
        uint32 opcode = r_u8(context + 159u) | 8u;
        count = (sint16)r_u16(context + 184u);
        back = front + (uint32)count * 4u - 4u;
        first = r_u32(front); front += 4u; last = r_u32(back); back -= 4u;
        packet = r_u32(context + 172u);
        while (front < back) {
            gouraud_texture_link24(packet, packet + 52u); w_u8(packet + 3u, 12u);
            gouraud_texture_packet_corner(packet, 4u, first);
            gouraud_texture_packet_corner(packet, 28u, last);
            first = r_u32(front); front += 4u; last = r_u32(back); back -= 4u;
            gouraud_texture_packet_corner(packet, 16u, first);
            gouraud_texture_packet_corner(packet, 40u, last);
            last_packet = packet;
            w_u8(packet + 7u, opcode); w_u16(packet + 14u, clut); w_u16(packet + 26u, tpage);
            packet += 52u;
        }
        w_u32(context + 180u, packet);
        if (front == back) {
            uint32 middle;
            gouraud_texture_packet_corner(packet, 4u, first);
            gouraud_texture_packet_corner(packet, 28u, last);
            middle = r_u32(front);
            w_u32(packet, 9u << 24u); w_u8(packet + 7u, opcode & 0xF7u);
            w_u16(packet + 14u, clut); w_u16(packet + 26u, tpage);
            gouraud_texture_packet_corner(packet, 16u, middle);
            last_packet = packet; w_u32(context + 180u, packet + 40u);
        }
        {
            uint32 tag = r_u32(context + 168u), old = r_u32(tag) & 0xFFFFFFu;
            gouraud_texture_link24(tag, r_u32(context + 172u));
            gouraud_texture_link24(last_packet, old);
        }
        result = 1u;
    }
restore:
    gouraud_texture_restore(context);
    return result;
}
