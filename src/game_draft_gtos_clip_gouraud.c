#include "game_draft_registry.h"
#include "psx_gte.h"

static void gouraud_link(uint32 bucket, uint32 first, uint32 last)
{
    uint32 old = r_u8(bucket) | ((uint32)r_u8(bucket + 1u) << 8u) | ((uint32)r_u8(bucket + 2u) << 16u);
    w_u8(bucket, first);
    w_u8(bucket + 1u, first >> 8u);
    w_u8(bucket + 2u, first >> 16u);
    w_u8(last, old);
    w_u8(last + 1u, old >> 8u);
    w_u8(last + 2u, old >> 16u);
}

static void gouraud_load_vertex(uint32 address)
{
    gte_lwc2(0u, r_u32(address));
    gte_lwc2(1u, r_u32(address + 4u));
}

static void gouraud_controls(uint32 context, uint32 offset, uint32 reg, uint32 count)
{
    uint32 i;
    for (i = 0u; i < count; ++i)
        gte_ctc2(r_u32(context + offset + 4u * i), reg + i);
}

static void gouraud_restore(uint32 context)
{
    gte_ctc2(r_u16(context + 34u), 27u);
    gte_ctc2(r_u32(context + 36u), 28u);
    gouraud_controls(context, 12u, 21u, 3u);
    gouraud_controls(context, 84u, 5u, 3u);
    gouraud_controls(context, 96u, 0u, 5u);
}

/* GTOS.EXE:80039738 clips Gouraud polygons against the five camera planes */
uint32 gtos_model_clip_gouraud(uint32 context)
{
    uint32 vertex = context + 324u, list = context + 204u;
    uint32 packet = r_u32(context + 172u), mask_all = 0xFFFFFFFFu, mask_any = 0u;
    sint32 count = (sint16)r_u16(context + 184u);
    uint32 near_z = r_u16(context + 8u), plane, success = 0u;
    gouraud_load_vertex(vertex);
    xport_gte_execute(0x180001u);
    gouraud_controls(context, 116u, 8u, 5u);
    gouraud_controls(context, 136u, 16u, 5u);
    w_u32(context + 192u, list);
    w_u32(context + 196u, context + 264u);
    w_u32(context + 200u, vertex + ((uint32)count << 5u));
    w_u32(context + 176u, packet);
    w_u32(context + 180u, packet);
    while (count != 0) {
        uint32 x = xport_gte_read_data(9u), y = xport_gte_read_data(10u), z = xport_gte_read_data(11u);
        uint32 screen = xport_gte_read_data(14u), a, b, c, d, mask;
        w_u32(vertex + 8u, screen);
        w_u32(list, vertex); list += 4u;
        xport_gte_execute(0x4BE012u);
        w_u16(vertex, x); w_u16(vertex + 2u, y); w_u16(vertex + 4u, z);
        gouraud_load_vertex(vertex);
        a = xport_gte_read_data(9u); b = xport_gte_read_data(10u);
        z -= near_z;
        xport_gte_execute(0x4C6012u);
        mask = z >> 31u;
        w_u16(vertex + 20u, z); w_u16(vertex + 22u, a); w_u16(vertex + 24u, b);
        if ((sint32)a < 0) mask |= 2u;
        if ((sint32)b < 0) mask |= 4u;
        c = xport_gte_read_data(9u); d = xport_gte_read_data(10u);
        gouraud_load_vertex(vertex + 32u);
        --count;
        xport_gte_execute(0x180001u);
        w_u16(vertex + 26u, c); w_u16(vertex + 28u, d);
        if ((sint32)c < 0) mask |= 8u;
        if ((sint32)d < 0) mask |= 16u;
        w_u16(vertex + 30u, mask);
        mask_any |= mask; mask_all &= mask;
        vertex += 32u;
    }
    w_u16(context + 186u, mask_any);
    if (mask_all != 0u) return 0u;
    gte_ctc2(1u, 27u); gte_ctc2(0u, 28u);
    gte_ctc2(0u, 5u); gte_ctc2(0u, 6u); gte_ctc2(0u, 7u);
    gte_ctc2(4096u, 0u); gte_ctc2(0u, 1u); gte_ctc2(4096u, 2u);
    gte_ctc2(0u, 3u); gte_ctc2(4096u, 4u);
    for (plane = 0u; plane < 5u && mask_any != 0u; ++plane) {
        uint32 bit = 1u << plane, input, output, base;
        if (!(mask_any & bit)) continue;
        mask_any -= mask_any & bit;
        input = r_u32(context + 192u); output = r_u32(context + 196u); base = output;
        count = (sint16)r_u16(context + 184u);
        w_u32(context + 192u, output); w_u32(context + 196u, input);
        w_u32(input + ((uint32)count << 2u), r_u32(input));
        w_u16(context + 188u, 2u);
        while (count != 0) {
            uint32 inside = r_u32(input), outside = r_u32(input + 4u);
            uint32 a = r_u16(inside + 30u) & bit, b = r_u16(outside + 30u) & bit;
            if (a == 0u) { w_u32(output, inside); output += 4u; }
            --count;
            if (a != b) {
                sint32 allowance = (sint16)r_u16(context + 188u) - 1;
                if (a != 0u) { uint32 swap = inside; inside = outside; outside = swap; }
                if (allowance != -1) {
                    sint32 distance, difference;
                    w_u16(context + 188u, allowance);
                    distance = (sint16)r_u16(inside + 20u + 2u * plane);
                    difference = distance - (sint16)r_u16(outside + 20u + 2u * plane);
                    if (distance != 0) {
                        uint32 fraction, new_vertex, x, y, z, p, q, r, s, flags;
                        gte_ctc2((uint32)distance, 26u); gte_ctc2((uint32)difference, 4u);
                        gte_lwc2(0u, 0u); gte_lwc2(1u, 4096u);
                        xport_gte_execute(0x180001u);
                        gte_ctc2(r_u32(inside), 8u); gte_ctc2(r_u32(outside), 10u);
                        gte_ctc2((r_u32(inside) & 0xFFFF0000u) | (r_u32(outside) & 0xFFFFu), 9u);
                        gte_ctc2(r_u32(inside + 4u), 11u); gte_ctc2(r_u32(outside + 4u), 12u);
                        fraction = (uint32)((sint32)xport_gte_read_data(24u) >> 4);
                        gte_ctc2(r_u16(context + 32u), 26u);
                        gte_lwc2(2u, (4096u - fraction) & 0xFFFFu); gte_lwc2(3u, fraction);
                        new_vertex = r_u32(context + 200u);
                        output += 4u;
                        xport_gte_execute(0x4AE012u);
                        w_u32(context + 200u, new_vertex + 32u);
                        x = xport_gte_read_data(9u); y = xport_gte_read_data(10u); z = xport_gte_read_data(11u);
                        gte_lwc2(8u, fraction);
                        w_u16(new_vertex, x);
                        if (plane == 0u) z = r_u16(context + 8u);
                        w_u16(new_vertex + 2u, y); w_u16(new_vertex + 4u, z);
                        gte_ctc2(4096u, 4u);
                        gouraud_load_vertex(new_vertex);
                        xport_gte_execute(0x180001u);
                        gouraud_controls(context, 116u, 8u, 5u);
                        gte_ctc2(r_u32(context + 136u), 16u);
                        w_u32(new_vertex + 8u, xport_gte_read_data(14u));
                        gte_ctc2(r_u32(context + 140u), 17u);
                        xport_gte_execute(0x4A6012u);
                        gouraud_controls(context, 144u, 18u, 3u);
                        p = xport_gte_read_data(9u); q = xport_gte_read_data(10u);
                        w_u32(output - 4u, new_vertex);
                        xport_gte_execute(0x4C6012u);
                        gte_lwc2(6u, r_u32(outside + 16u));
                        gte_ctc2((uint32)r_u8(inside + 16u) << 4u, 21u);
                        gte_ctc2((uint32)r_u8(inside + 17u) << 4u, 22u);
                        gte_ctc2((uint32)r_u8(inside + 18u) << 4u, 23u);
                        r = xport_gte_read_data(9u); s = xport_gte_read_data(10u);
                        flags = (sint32)p < 0 ? 2u : 0u;
                        xport_gte_execute(0x780010u);
                        w_u16(new_vertex + 22u, p); w_u16(new_vertex + 24u, q);
                        if ((sint32)q < 0) flags |= 4u;
                        w_u16(new_vertex + 26u, r); w_u16(new_vertex + 28u, s);
                        if ((sint32)r < 0) flags |= 8u;
                        if ((sint32)s < 0) flags |= 16u;
                        w_u16(new_vertex + 30u, flags);
                        w_u32(new_vertex + 16u, xport_gte_read_data(22u));
                    }
                }
            }
            input += 4u;
        }
        count = (sint32)(output - base) >> 2;
        w_u16(context + 184u, count);
        if (count < 3) { gouraud_restore(context); return 0u; }
    }
    {
        uint32 left = r_u32(context + 192u), right = left + 4u * (uint32)((sint16)r_u16(context + 184u) - 1);
        uint32 a = r_u32(left), b = r_u32(right), last = 0u;
        uint32 command = r_u8(context + 159u) | 8u;
        left += 4u; right -= 4u;
        packet = r_u32(context + 172u);
        while (left < right) {
            w_u8(packet, packet + 36u); w_u8(packet + 1u, (packet + 36u) >> 8u); w_u8(packet + 2u, (packet + 36u) >> 16u);
            w_u8(packet + 3u, 8u);
            w_u32(packet + 4u, r_u32(a + 16u)); w_u32(packet + 8u, r_u32(a + 8u));
            w_u32(packet + 20u, r_u32(b + 16u)); w_u32(packet + 24u, r_u32(b + 8u));
            a = r_u32(left); left += 4u; b = r_u32(right); right -= 4u;
            last = packet;
            w_u32(packet + 12u, r_u32(a + 16u)); w_u32(packet + 16u, r_u32(a + 8u));
            w_u32(packet + 28u, r_u32(b + 16u)); w_u8(packet + 7u, command); w_u32(packet + 32u, r_u32(b + 8u));
            packet += 36u;
        }
        w_u32(context + 180u, packet);
        if (left == right) {
            uint32 middle = r_u32(left);
            w_u32(packet + 4u, r_u32(a + 16u)); w_u32(packet + 8u, r_u32(a + 8u));
            w_u32(packet + 20u, r_u32(b + 16u)); w_u32(packet + 24u, r_u32(b + 8u));
            w_u32(packet, 0x06000000u); w_u8(packet + 7u, command & 0xF7u);
            last = packet;
            w_u32(packet + 12u, r_u32(middle + 16u)); w_u32(packet + 16u, r_u32(middle + 8u));
            w_u32(context + 180u, packet + 28u);
        }
        gouraud_link(r_u32(context + 168u), r_u32(context + 172u), last);
        success = 1u;
    }
    gouraud_restore(context);
    return success;
}
