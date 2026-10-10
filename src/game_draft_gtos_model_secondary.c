#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 secondary_flag(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return (uint32)state.flag;
}

static uint32 secondary_leading(uint32 value)
{
    uint32 count = 0u;
    if (value & 0x80000000u) value = ~value;
    while (count < 32u && !(value & 0x80000000u)) {
        ++count;
        value <<= 1u;
    }
    return count;
}

static void secondary_link(uint32 bucket, uint32 first, uint32 last)
{
    uint32 old = r_u8(bucket) | ((uint32)r_u8(bucket + 1u) << 8u) | ((uint32)r_u8(bucket + 2u) << 16u);
    w_u8(bucket, first);
    w_u8(bucket + 1u, first >> 8u);
    w_u8(bucket + 2u, first >> 16u);
    w_u8(last, old);
    w_u8(last + 1u, old >> 8u);
    w_u8(last + 2u, old >> 16u);
}

static void secondary_load_vertex(uint32 address)
{
    gte_lwc2(0u, r_u32(address));
    gte_lwc2(1u, r_u32(address + 4u));
}

static void secondary_controls(uint32 context, uint32 offset, uint32 reg, uint32 count)
{
    uint32 i;
    for (i = 0u; i < count; ++i)
        gte_ctc2(r_u32(context + offset + 4u * i), reg + i);
}

static void secondary_restore(uint32 context)
{
    gte_ctc2(r_u16(context + 34u), 27u);
    gte_ctc2(r_u32(context + 36u), 28u);
    secondary_controls(context, 12u, 21u, 3u);
    secondary_controls(context, 84u, 5u, 3u);
    secondary_controls(context, 96u, 0u, 5u);
}

/* GTOS.EXE:80038680 clips flat polygons against the five camera planes */
uint32 gtos_model_clip_flat(uint32 context)
{
    uint32 vertex = context + 324u, list = context + 204u;
    uint32 packet = r_u32(context + 172u), mask_all = 0xFFFFFFFFu, mask_any = 0u;
    sint32 count = (sint16)r_u16(context + 184u);
    uint32 near_z = r_u16(context + 8u), plane, success = 0u;
    secondary_load_vertex(vertex);
    xport_gte_execute(0x180001u);
    secondary_controls(context, 116u, 8u, 5u);
    secondary_controls(context, 136u, 16u, 5u);
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
        secondary_load_vertex(vertex);
        a = xport_gte_read_data(9u); b = xport_gte_read_data(10u);
        z -= near_z;
        xport_gte_execute(0x4C6012u);
        mask = z >> 31u;
        w_u16(vertex + 20u, z); w_u16(vertex + 22u, a); w_u16(vertex + 24u, b);
        if ((sint32)a < 0) mask |= 2u;
        if ((sint32)b < 0) mask |= 4u;
        c = xport_gte_read_data(9u); d = xport_gte_read_data(10u);
        secondary_load_vertex(vertex + 32u);
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
                        w_u16(new_vertex, x);
                        if (plane == 0u) z = r_u16(context + 8u);
                        w_u16(new_vertex + 2u, y); w_u16(new_vertex + 4u, z);
                        gte_ctc2(4096u, 4u);
                        secondary_load_vertex(new_vertex);
                        xport_gte_execute(0x180001u);
                        secondary_controls(context, 116u, 8u, 5u);
                        gte_ctc2(r_u32(context + 136u), 16u);
                        w_u32(new_vertex + 8u, xport_gte_read_data(14u));
                        gte_ctc2(r_u32(context + 140u), 17u);
                        xport_gte_execute(0x4A6012u);
                        secondary_controls(context, 144u, 18u, 3u);
                        p = xport_gte_read_data(9u); q = xport_gte_read_data(10u);
                        w_u32(output - 4u, new_vertex);
                        xport_gte_execute(0x4C6012u);
                        flags = (sint32)p < 0 ? 2u : 0u;
                        w_u16(new_vertex + 22u, p); w_u16(new_vertex + 24u, q);
                        if ((sint32)q < 0) flags |= 4u;
                        r = xport_gte_read_data(9u); s = xport_gte_read_data(10u);
                        w_u16(new_vertex + 26u, r); w_u16(new_vertex + 28u, s);
                        if ((sint32)r < 0) flags |= 8u;
                        if ((sint32)s < 0) flags |= 16u;
                        w_u16(new_vertex + 30u, flags);
                    }
                }
            }
            input += 4u;
        }
        count = (sint32)(output - base) >> 2;
        w_u16(context + 184u, count);
        if (count < 3) { secondary_restore(context); return 0u; }
    }
    {
        uint32 left = r_u32(context + 192u), right = left + 4u * (uint32)((sint16)r_u16(context + 184u) - 1);
        uint32 a = r_u32(left), b = r_u32(right), last = 0u;
        uint32 color = r_u32(context + 156u) | 0x08000000u;
        left += 4u; right -= 4u;
        packet = r_u32(context + 172u);
        while (left < right) {
            w_u8(packet, packet + 24u); w_u8(packet + 1u, (packet + 24u) >> 8u); w_u8(packet + 2u, (packet + 24u) >> 16u);
            w_u8(packet + 3u, 5u);
            w_u32(packet + 8u, r_u32(a + 8u)); w_u32(packet + 16u, r_u32(b + 8u));
            a = r_u32(left); left += 4u; b = r_u32(right); right -= 4u;
            last = packet;
            w_u32(packet + 12u, r_u32(a + 8u)); w_u32(packet + 4u, color); w_u32(packet + 20u, r_u32(b + 8u));
            packet += 24u;
        }
        w_u32(context + 180u, packet);
        if (left == right) {
            uint32 middle = r_u32(left);
            w_u32(packet + 8u, r_u32(a + 8u)); w_u32(packet + 16u, r_u32(b + 8u));
            w_u32(packet, 0x04000000u); w_u32(packet + 4u, color & 0xF7FFFFFFu);
            last = packet;
            w_u32(packet + 12u, r_u32(middle + 8u)); w_u32(context + 180u, packet + 20u);
        }
        secondary_link(r_u32(context + 168u), r_u32(context + 172u), last);
        success = 1u;
    }
    secondary_restore(context);
    return success;
}

static void secondary_save_vertices(uint32 context, uint32 quad)
{
    uint32 offsets[3] = { quad ? 420u : 388u, quad ? 388u : 356u, 324u }, i;
    for (i = 0u; i < 3u; ++i) {
        w_u32(context + offsets[i], xport_gte_read_data(2u * i));
        w_u32(context + offsets[i] + 4u, xport_gte_read_data(2u * i + 1u));
    }
}

static sint32 secondary_facing(uint32 context)
{
    uint32 vector[3], cross[3], i, a, b, c;
    for (i = 0u; i < 3u; ++i) {
        uint32 origin = (uint32)(sint32)(sint16)r_u16(context + 56u + 2u * i);
        gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 356u + 2u * i) - origin, 2u * i);
        gte_lwc2(9u + i, (uint32)(sint32)(sint16)r_u16(context + 388u + 2u * i) - origin);
        vector[i] = (uint32)(sint32)(sint16)r_u16(context + 324u + 2u * i) - origin;
    }
    xport_gte_execute(0x170000Cu);
    for (i = 0u; i < 3u; ++i) { cross[i] = xport_gte_read_data(25u + i); gte_lwc2(9u + i, vector[i]); }
    a = cross[0]; b = cross[1]; c = cross[2];
    gte_ctc2((a & 4095u) | ((b & 4095u) << 16u), 16u);
    a = (uint32)((sint32)a >> 12); b = (uint32)((sint32)b >> 12);
    gte_ctc2((c & 4095u) | ((a & 4095u) << 16u), 17u);
    c = (uint32)((sint32)c >> 12); a = (uint32)((sint32)a >> 12);
    gte_ctc2((b & 4095u) | ((c & 4095u) << 16u), 18u);
    b = (uint32)((sint32)b >> 12); c = (uint32)((sint32)c >> 12);
    gte_ctc2((a & 65535u) | (b << 16u), 19u); gte_ctc2(c, 20u);
    xport_gte_execute(0x45E012u);
    gte_ctc2(r_u32(context + 96u), 0u); gte_ctc2(r_u32(context + 104u), 2u); gte_ctc2(r_u32(context + 112u), 4u);
    a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    return (sint32)(c + (uint32)((sint32)(b + (uint32)((sint32)a >> 12)) >> 12));
}

static uint32 secondary_bounds(uint32 context, uint32 vertices, uint32 *flags)
{
    uint32 axis, i;
    for (axis = 0u; axis < 2u; ++axis) {
        sint32 low = (sint16)r_u16(context + 332u + 2u * axis), high = low;
        for (i = 1u; i < vertices; ++i) {
            sint32 value = (sint16)r_u16(context + 332u + 32u * i + 2u * axis);
            if (value < low) low = value;
            if (value > high) high = value;
        }
        if (high <= 0 || low >= (sint16)r_u16(context + 4u + 2u * axis)) return 0u;
        *flags |= (axis == 0u ? 1023u : 511u) - (uint32)(high - low);
    }
    return 1u;
}

/* GTOS.EXE:8002D91C draws the secondary flat triangle and quad model */
uint32 gtos_model_secondary(uint32 context, uint32 model)
{
    uint32 i, displacement[3], magnitude = 0u, clip, stream, packet, group;
    sint32 highest, shift, model_shift;
    for (i = 0u; i < 3u; ++i) {
        uint32 value;
        displacement[i] = r_u32(context + 864u + 4u * i) - r_u32(context + 44u + 4u * i);
        w_u32(context + 268u + 4u * i, displacement[i]);
        value = displacement[i]; if ((sint32)value < 0) value = 0u - value;
        magnitude |= value;
    }
    highest = 31 - (sint32)secondary_leading(magnitude);
    if (highest < r_u16(model + 4u)) highest = r_u16(model + 4u);
    model_shift = highest - r_u16(model + 4u);
    shift = highest - 12; if (shift < 0) shift = 0;
    w_u16(context + 40u, shift); w_u16(context + 42u, model_shift);
    for (i = 0u; i < 3u; ++i) w_u16(context + 280u + 2u * i, (sint32)displacement[i] >> ((uint32)shift & 31u));
    gtos_model_geometry_rotation(psx_addr(context + 64u, 20u));
    secondary_load_vertex(context + 280u); xport_gte_execute(0x486012u);
    for (i = 0u; i < 3u; ++i) w_u32(context + 84u + 4u * i, xport_gte_read_data(9u + i));
    secondary_controls(context, 84u, 5u, 3u);
    gtos_model_multiply(psx_addr(context + 96u, 20u), psx_addr(context + 64u, 20u), psx_addr(context + 884u, 20u));
    gtos_model_scale(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), 4096);
    gtos_model_shift(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), model_shift);
    gtos_model_bind_geometry_rotation(context, psx_addr(context + 96u, 20u));
    gtos_model_clip_planes(context);
    for (i = 0u; i < 8u; ++i) {
        uint32 address = context + 184u + 8u * i;
        w_u16(address, r_u16(model + ((i & 1u) ? 16u : 8u)));
        w_u16(address + 2u, r_u16(model + ((i & 2u) ? 18u : 10u)));
        w_u16(address + 4u, r_u16(model + ((i & 4u) ? 20u : 12u)));
    }
    clip = gtos_model_clip_points(context, context + 184u, 8u);
    if (clip & 31u) return clip & 31u;
    gtos_model_transpose(psx_addr(context + 248u, 20u), psx_addr(context + 884u, 20u));
    gtos_model_scale(psx_addr(context + 248u, 20u), psx_addr(context + 248u, 20u), -4096);
    gtos_model_rotate_short(psx_addr(context + 56u, 8u), psx_addr(context + 248u, 20u), psx_addr(context + 280u, 8u));
    secondary_controls(context, 96u, 0u, 5u);
    stream = r_u32(model + 24u); packet = r_u32(0x8007A024u); w_u32(context + 932u, packet);
    for (group = 0u; group < 2u; ++group) {
        uint32 count = r_u16(model + 2u * group), words[6];
        uint32 quad = group, stride = quad ? 32u : 24u;
        if (count == 0u) continue;
        for (i = 0u; i < 6u; ++i) words[i] = r_u32(stream + 4u * i);
        while (count-- != 0u) {
            uint32 next[6], depth, bucket, flags, clipped = ((clip >> 16u) & 63u) != 0u;
            sint32 facing;
            gte_lwc2(0u, words[quad ? 2u : 4u]); gte_lwc2(1u, words[quad ? 3u : 5u]);
            gte_lwc2(2u, words[quad ? 4u : 2u]); gte_lwc2(3u, words[quad ? 5u : 3u]);
            gte_lwc2(4u, words[0]); gte_lwc2(5u, words[1]);
            w_u32(packet, quad ? 0x05000000u : 0x04000000u);
            xport_gte_execute(0x280030u);
            for (i = 0u; i < 4u; ++i) next[i] = r_u32(stream + stride + 4u * i);
            flags = clipped ? secondary_flag() : 0u;
            if (quad) {
                w_u32(packet + 12u, xport_gte_read_data(12u));
                if (clipped) {
                    w_u32(context + 364u, xport_gte_read_data(12u));
                    w_u32(context + 356u, xport_gte_read_data(0u)); w_u32(context + 360u, xport_gte_read_data(1u));
                }
            }
            xport_gte_execute(0x1400006u);
            facing = (sint32)xport_gte_read_data(24u);
            depth = xport_gte_read_data(17u);
            if ((sint32)depth < (sint32)xport_gte_read_data(18u)) depth = xport_gte_read_data(18u);
            if ((sint32)depth < (sint32)xport_gte_read_data(19u)) depth = xport_gte_read_data(19u);
            if (quad) {
                secondary_load_vertex(stream + 24u);
                xport_gte_execute(0x180001u);
                facing = (sint32)(0u - (uint32)facing);
                w_u32(packet + 4u, 0x28000000u);
                for (i = 4u; i < 6u; ++i) next[i] = r_u32(stream + stride + 4u * i);
                if (clipped) flags |= secondary_flag();
                if ((sint32)depth < (sint32)xport_gte_read_data(19u)) depth = xport_gte_read_data(19u);
                xport_gte_execute(0x1400006u);
                facing = (sint32)((uint32)facing & xport_gte_read_data(24u));
                if (!clipped) {
                    w_u32(packet + 20u, xport_gte_read_data(12u));
                    w_u32(packet + 8u, xport_gte_read_data(13u));
                }
            } else {
                for (i = 4u; i < 6u; ++i) next[i] = r_u32(stream + stride + 4u * i);
                if (clipped) {
                    w_u32(packet + 16u, xport_gte_read_data(12u)); w_u32(packet + 12u, xport_gte_read_data(13u));
                } else w_u32(packet + 4u, 0x20000000u);
            }
            if (!clipped && facing >= 0) {
                sint32 index;
                if (!quad) {
                    w_u32(packet + 16u, xport_gte_read_data(12u));
                    w_u32(packet + 12u, xport_gte_read_data(13u));
                    w_u32(packet + 8u, xport_gte_read_data(14u));
                }
                index = (sint32)(depth << (r_u16(context + 40u) & 31u)) >> 13;
                if (index >= 4096) index = 4095;
                bucket = r_u32(context + 928u) + ((uint32)index << 2u);
                if (quad) w_u32(packet + 16u, xport_gte_read_data(14u));
                secondary_link(bucket, packet, packet);
                packet += quad ? 24u : 20u;
            }
            if (clipped && (sint32)depth >= r_u16(context + 8u)) {
                sint32 index = (sint32)(depth << (r_u16(context + 40u) & 31u)) >> 13;
                if (index >= 4096) index = 4095;
                bucket = r_u32(context + 928u) + ((uint32)index << 2u);
                if (clipped && (sint32)flags < 0) { secondary_save_vertices(context, quad); facing = secondary_facing(context); }
                if (!quad && clipped) w_u32(packet + 8u, xport_gte_read_data(14u));
                if (quad && clipped) w_u32(context + 396u, xport_gte_read_data(12u));
                if (facing >= 0) {
                    uint32 accept = 1u;
                    if (clipped) {
                        if (quad) { w_u32(context + 332u, xport_gte_read_data(13u)); w_u32(context + 428u, xport_gte_read_data(14u)); }
                        else { w_u32(context + 396u, xport_gte_read_data(12u)); w_u32(context + 364u, xport_gte_read_data(13u)); w_u32(context + 332u, xport_gte_read_data(14u)); }
                        accept = secondary_bounds(context, quad ? 4u : 3u, &flags);
                    }
                    if (accept) {
                        if (!quad) w_u32(packet + 4u, 0x20000000u);
                        if (clipped && (sint32)flags < 0) {
                            secondary_save_vertices(context, quad);
                            w_u32(context + 168u, bucket); w_u32(context + 172u, packet);
                            w_u16(context + 184u, quad ? 4u : 3u); w_u32(context + 156u, quad ? 0x28000000u : 0x20000000u);
                            gtos_model_clip_flat(context);
                            packet = r_u32(context + 180u);
                            for (i = 0u; i < 6u; ++i) next[i] = r_u32(stream + stride + 4u * i);
                        } else {
                            secondary_link(bucket, packet, packet);
                            if (quad && clipped) { w_u32(packet + 20u, xport_gte_read_data(12u)); w_u32(packet + 8u, xport_gte_read_data(13u)); w_u32(packet + 16u, xport_gte_read_data(14u)); }
                            packet += quad ? 24u : 20u;
                        }
                    }
                }
            }
            stream += stride;
            for (i = 0u; i < 6u; ++i) words[i] = next[i];
        }
        w_u32(context + 932u, packet);
    }
    w_u32(0x8007A024u, packet);
    return packet;
}
