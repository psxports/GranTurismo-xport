#include "game_draft_registry.h"
#include "psx_gte.h"

static void texture_clip_load(uint32 point)
{
    gte_lwc2(0u, r_u32(point));
    gte_lwc2(1u, r_u32(point + 4u));
}

static void texture_clip_planes(uint32 context)
{
    uint32 i;
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 116u + 4u * i), 8u + i);
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 136u + 4u * i), 16u + i);
}

static uint32 texture_clip_restore(uint32 context, uint32 result)
{
    uint32 i;
    gte_ctc2(r_u16(context + 34u), 27u);
    gte_ctc2(r_u32(context + 36u), 28u);
    for (i = 0u; i < 3u; ++i) gte_ctc2(r_u32(context + 12u + 4u * i), 21u + i);
    for (i = 0u; i < 3u; ++i) gte_ctc2(r_u32(context + 84u + 4u * i), 5u + i);
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 96u + 4u * i), i);
    return result;
}

static uint32 texture_clip_intersection(uint32 context, uint32 inside, uint32 outside, uint32 plane)
{
    sint32 distance = (sint16)r_u16(inside + 20u + 2u * plane);
    uint32 denominator = (uint32)distance - (uint32)(sint32)(sint16)r_u16(outside + 20u + 2u * plane);
    uint32 first_xy, second_xy, ratio, point, x, y, z, flags;
    if (distance == 0) return 0u;
    gte_ctc2((uint32)distance, 26u);
    gte_ctc2(denominator, 4u);
    gte_lwc2(0u, 0u);
    gte_lwc2(1u, 4096u);
    first_xy = r_u32(inside);
    second_xy = r_u32(outside);
    xport_gte_execute(0x180001u);
    gte_ctc2(first_xy, 8u);
    gte_ctc2(second_xy, 10u);
    gte_ctc2((first_xy & 0xFFFF0000u) | (second_xy & 0xFFFFu), 9u);
    gte_ctc2(r_u32(inside + 4u), 11u);
    gte_ctc2(r_u32(outside + 4u), 12u);
    ratio = (uint32)((sint32)xport_gte_read_data(24u) >> 4);
    gte_ctc2(r_u16(context + 32u), 26u);
    gte_lwc2(2u, (4096u - ratio) & 0xFFFFu);
    gte_lwc2(3u, ratio);
    point = r_u32(context + 200u);
    xport_gte_execute(0x4AE012u);
    w_u32(context + 200u, point + 32u);
    gte_ctc2(r_u16(inside + 12u), 16u);
    gte_ctc2(r_u16(inside + 14u), 19u);
    gte_ctc2(r_u16(outside + 12u), 17u);
    gte_ctc2(r_u16(outside + 14u), 20u);
    x = xport_gte_read_data(9u);
    y = xport_gte_read_data(10u);
    z = xport_gte_read_data(11u);
    w_u16(point, (uint16)x);
    if (plane == 0u) z = r_u16(context + 8u);
    xport_gte_execute(0x4CE012u);
    w_u16(point + 2u, (uint16)y);
    w_u16(point + 4u, (uint16)z);
    gte_ctc2(4096u, 4u);
    texture_clip_load(point);
    w_u16(point + 12u, (uint16)xport_gte_read_data(9u));
    w_u16(point + 14u, (uint16)xport_gte_read_data(11u));
    xport_gte_execute(0x180001u);
    /* Restore only the control words consumed by the first plane operation */
    for (x = 0u; x < 5u; ++x) gte_ctc2(r_u32(context + 116u + 4u * x), 8u + x);
    gte_ctc2(r_u32(context + 136u), 16u);
    w_u32(point + 8u, xport_gte_read_data(14u));
    gte_ctc2(r_u32(context + 140u), 17u);
    xport_gte_execute(0x4A6012u);
    gte_ctc2(r_u32(context + 144u), 18u);
    gte_ctc2(r_u32(context + 148u), 19u);
    gte_ctc2(r_u32(context + 152u), 20u);
    x = xport_gte_read_data(9u);
    y = xport_gte_read_data(10u);
    xport_gte_execute(0x4C6012u);
    flags = (sint32)x < 0 ? 2u : 0u;
    w_u16(point + 22u, (uint16)x);
    w_u16(point + 24u, (uint16)y);
    if ((sint32)y < 0) flags |= 4u;
    x = xport_gte_read_data(9u);
    y = xport_gte_read_data(10u);
    w_u16(point + 26u, (uint16)x);
    if ((sint32)x < 0) flags |= 8u;
    w_u16(point + 28u, (uint16)y);
    if ((sint32)y < 0) flags |= 16u;
    w_u16(point + 30u, (uint16)flags);
    return point;
}

static void texture_clip_packet_vertex(uint32 packet, uint32 xy_offset, uint32 uv_offset, uint32 point)
{
    uint32 uv = r_u32(point + 12u) >> 7u;
    w_u32(packet + xy_offset, r_u32(point + 8u));
    w_u8(packet + uv_offset, (uint8)uv);
    w_u8(packet + uv_offset + 1u, (uint8)(uv >> 16u));
}

/* Original: GTOS.EXE:sub_800390B4. */ uint32 gtos_model_clip_texture(uint32 context)
{
    uint32 any = 0u, all = 0xFFFFFFFFu, near_plane = r_u16(context + 8u);
    uint32 list = context + 204u, point = context + 324u, plane, packet, last = 0u;
    sint32 count = (sint16)r_u16(context + 184u), remaining = count;
    texture_clip_load(point);
    xport_gte_execute(0x180001u);
    texture_clip_planes(context);
    packet = r_u32(context + 172u);
    w_u32(context + 192u, list);
    w_u32(context + 196u, context + 264u);
    w_u32(context + 200u, point + ((uint32)count << 5u));
    w_u32(context + 176u, packet);
    w_u32(context + 180u, packet);
    while (remaining != 0) {
        uint32 x = xport_gte_read_data(9u), y = xport_gte_read_data(10u), z = xport_gte_read_data(11u);
        uint32 left, right, top, bottom, flags, distance = z - near_plane;
        w_u32(point + 8u, xport_gte_read_data(14u));
        w_u32(list, point); list += 4u;
        xport_gte_execute(0x4BE012u);
        w_u16(point, (uint16)x); w_u16(point + 2u, (uint16)y); w_u16(point + 4u, (uint16)z);
        texture_clip_load(point);
        left = xport_gte_read_data(9u); right = xport_gte_read_data(10u);
        xport_gte_execute(0x4C6012u);
        flags = distance >> 31u;
        w_u16(point + 20u, (uint16)distance);
        w_u16(point + 22u, (uint16)left); if ((sint32)left < 0) flags |= 2u;
        w_u16(point + 24u, (uint16)right); if ((sint32)right < 0) flags |= 4u;
        top = xport_gte_read_data(9u); bottom = xport_gte_read_data(10u);
        texture_clip_load(point + 32u);
        --remaining;
        xport_gte_execute(0x180001u);
        w_u16(point + 26u, (uint16)top); if ((sint32)top < 0) flags |= 8u;
        w_u16(point + 28u, (uint16)bottom); if ((sint32)bottom < 0) flags |= 16u;
        w_u16(point + 30u, (uint16)flags);
        any |= flags; all &= flags;
        point += 32u;
    }
    w_u16(context + 186u, (uint16)any);
    if (all != 0u) return 0u;
    gte_ctc2(1u, 27u); gte_ctc2(0u, 28u);
    gte_ctc2(0u, 5u); gte_ctc2(0u, 6u); gte_ctc2(0u, 7u);
    gte_ctc2(4096u, 0u); gte_ctc2(0u, 1u); gte_ctc2(4096u, 2u); gte_ctc2(0u, 3u); gte_ctc2(4096u, 4u);
    for (plane = 0u; plane < 5u && any != 0u; ++plane) {
        uint32 mask = 1u << plane, input, output, output_start;
        if ((any & mask) == 0u) continue;
        any -= any & mask;
        input = r_u32(context + 192u); output = r_u32(context + 196u); output_start = output;
        count = (sint16)r_u16(context + 184u);
        w_u32(context + 192u, output); w_u32(context + 196u, input);
        w_u32(input + ((uint32)count << 2u), r_u32(input));
        w_u16(context + 188u, 2u);
        remaining = count;
        while (remaining != 0) {
            uint32 a = r_u32(input), b = r_u32(input + 4u);
            uint32 flags_a = r_u16(a + 30u) & mask, flags_b = r_u16(b + 30u) & mask;
            if (flags_a == 0u) { w_u32(output, a); output += 4u; }
            --remaining;
            if (flags_a != flags_b) {
                sint32 allowance = (sint16)r_u16(context + 188u) - 1;
                if (flags_a != 0u) { uint32 swap = a; a = b; b = swap; }
                if (allowance != -1) {
                    uint32 intersection;
                    w_u16(context + 188u, (uint16)allowance);
                    intersection = texture_clip_intersection(context, a, b, plane);
                    if (intersection != 0u) { w_u32(output, intersection); output += 4u; }
                }
            }
            input += 4u;
        }
        count = (sint32)(output - output_start) >> 2;
        w_u16(context + 184u, (uint16)count);
        if (count < 3) return texture_clip_restore(context, 0u);
    }
    {
        uint32 front, back, a, b, color = r_u32(context + 156u) | 0x08000000u;
        uint16 clut = r_u16(context + 164u), tpage = r_u16(context + 166u);
        packet = r_u32(context + 172u);
        front = r_u32(context + 192u);
        back = front + ((uint32)(sint32)(sint16)r_u16(context + 184u) << 2u) - 4u;
        a = r_u32(front); front += 4u;
        b = r_u32(back); back -= 4u;
        while (front < back) {
            uint32 next = packet + 40u;
            w_u16(packet, (uint16)next); w_u8(packet + 2u, (uint8)(next >> 16u)); w_u8(packet + 3u, 9u);
            texture_clip_packet_vertex(packet, 8u, 12u, a);
            texture_clip_packet_vertex(packet, 24u, 28u, b);
            a = r_u32(front); front += 4u;
            b = r_u32(back); back -= 4u;
            texture_clip_packet_vertex(packet, 16u, 20u, a);
            texture_clip_packet_vertex(packet, 32u, 36u, b);
            last = packet;
            w_u32(packet + 4u, color); w_u16(packet + 14u, clut); w_u16(packet + 22u, tpage);
            packet = next;
        }
        w_u32(context + 180u, packet);
        if (front == back) {
            texture_clip_packet_vertex(packet, 8u, 12u, a);
            texture_clip_packet_vertex(packet, 24u, 28u, b);
            a = r_u32(front);
            w_u32(packet, 0x07000000u); w_u32(packet + 4u, color & 0xF7FFFFFFu);
            w_u16(packet + 14u, clut); w_u16(packet + 22u, tpage);
            texture_clip_packet_vertex(packet, 16u, 20u, a);
            last = packet;
            w_u32(context + 180u, packet + 32u);
        }
        {
            uint32 table = r_u32(context + 168u), first = r_u32(context + 172u), old = r_u32(table);
            w_u16(table, (uint16)first); w_u8(table + 2u, (uint8)(first >> 16u));
            w_u16(last, (uint16)old); w_u8(last + 2u, (uint8)(old >> 16u));
        }
    }
    return texture_clip_restore(context, 1u);
}
