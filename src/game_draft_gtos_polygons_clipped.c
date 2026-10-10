#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 clipped_flag(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return (uint32)state.flag;
}

static void clipped_vector(uint32 slot, uint32 address)
{
    gte_lwc2(slot * 2u, r_u32(address));
    gte_lwc2(slot * 2u + 1u, r_u32(address + 4u));
}

static void clipped_save_vector(uint32 slot, uint32 address)
{
    w_u32(address, xport_gte_read_data(slot * 2u));
    w_u32(address + 4u, xport_gte_read_data(slot * 2u + 1u));
}

static void clipped_link(uint32 tag, uint32 packet)
{
    uint32 old = r_u32(tag) & 0xFFFFFFu;
    w_u16(tag, packet);
    w_u8(tag + 2u, packet >> 16u);
    w_u16(packet, old);
    w_u8(packet + 2u, old >> 16u);
}

static void clipped_chain_next(uint32 packet, uint32 next)
{
    w_u16(packet, next);
    w_u8(packet + 2u, next >> 16u);
}

static uint32 clipped_absolute(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static uint32 clipped_facing_overflow(uint32 context)
{
    uint32 cross[3], i, a, b, c, first, second, third;
    for (i = 0u; i < 3u; ++i) {
        uint32 origin = (uint32)(sint32)(sint16)r_u16(context + 56u + 2u * i);
        gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 356u + 2u * i) - origin, 2u * i);
        gte_lwc2(9u + i, (uint32)(sint32)(sint16)r_u16(context + 388u + 2u * i) - origin);
    }
    xport_gte_execute(0x170000Cu);
    for (i = 0u; i < 3u; ++i) {
        uint32 origin = (uint32)(sint32)(sint16)r_u16(context + 56u + 2u * i);
        cross[i] = xport_gte_read_data(25u + i);
        gte_lwc2(9u + i, (uint32)(sint32)(sint16)r_u16(context + 324u + 2u * i) - origin);
    }
    a = cross[0]; b = cross[1]; c = cross[2];
    gte_ctc2((a & 4095u) | ((b & 4095u) << 16u), 16u);
    a = (uint32)((sint32)a >> 12); b = (uint32)((sint32)b >> 12);
    gte_ctc2((c & 4095u) | ((a & 4095u) << 16u), 17u);
    c = (uint32)((sint32)c >> 12); a = (uint32)((sint32)a >> 12);
    gte_ctc2((b & 4095u) | ((c & 4095u) << 16u), 18u);
    b = (uint32)((sint32)b >> 12); c = (uint32)((sint32)c >> 12);
    gte_ctc2((a & 65535u) | (b << 16u), 19u);
    gte_ctc2(c, 20u);
    xport_gte_execute(0x45E012u);
    gte_ctc2(r_u32(context + 96u), 0u);
    gte_ctc2(r_u32(context + 104u), 2u);
    gte_ctc2(r_u32(context + 112u), 4u);
    first = xport_gte_read_data(25u);
    second = xport_gte_read_data(26u);
    third = xport_gte_read_data(27u);
    second += (uint32)((sint32)first >> 12);
    third += (uint32)((sint32)second >> 12);
    return third ^ (uint32)(sint32)(sint16)r_u16(context + 908u);
}

static uint32 clipped_screen(uint32 context, uint32 count, uint32 *flags, uint32 *result)
{
    uint32 axis, i;
    for (axis = 0u; axis < 2u; ++axis) {
        sint32 minimum = (sint16)r_u16(context + 332u + axis * 2u);
        sint32 maximum = minimum;
        for (i = 1u; i < count; ++i) {
            sint32 value = (sint16)r_u16(context + 332u + i * 32u + axis * 2u);
            if (value < minimum) minimum = value;
            if (value > maximum) maximum = value;
        }
        *result = (uint32)minimum;
        if (maximum <= 0) return 0u;
        *result = (axis ? 511u : 1023u) - (uint32)(maximum - minimum);
        if (minimum >= (sint16)r_u16(context + 4u + axis * 2u)) return 0u;
        *flags |= *result;
    }
    return 1u;
}

static void clipped_restore_lights(uint32 context)
{
    uint32 i;
    gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 920u), 13u);
    gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 922u), 14u);
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 948u + 4u * i), 8u + i);
}

static uint32 clipped_reflection(uint32 context, uint32 source, uint32 count,
    uint32 tag, uint32 packet, uint32 flags, uint32 restore_vectors)
{
    uint32 normals = r_u32(context + 944u), indices[4], uv[4][2], i, j;
    uint32 packed = r_u32(source + 4u), packed2 = r_u32(source + 8u);
    uint32 original[6], color = r_u32(context + (count == 3u ? 968u : 972u));
    uint32 texture = r_u32(context + 916u), offsets[4] = {12u, 20u, 36u, 28u};
    if (count == 3u) offsets[2] = 28u;
    for (i = 0u; i < 6u; ++i) original[i] = xport_gte_read_data(i);
    if (count == 4u) {
        clipped_save_vector(0u, context + 420u);
        clipped_save_vector(1u, context + 388u);
    } else {
        clipped_save_vector(0u, context + 388u);
        clipped_save_vector(1u, context + 356u);
    }
    if ((sint32)flags < 0) {
        clipped_save_vector(2u, context + 324u);
        if (count == 4u) {
            for (i = 0u; i < 6u; ++i) w_u32(context + (i < 2u ? 796u : i < 4u ? 788u : 772u) + 4u * (i & 1u), original[i]);
            w_u32(context + 780u, r_u32(context + 356u));
            w_u32(context + 784u, r_u32(context + 360u));
        } else {
            for (i = 0u; i < 6u; ++i) w_u32(context + 788u - 8u * (i / 2u) + 4u * (i & 1u), original[i]);
        }
    }
    indices[0] = (packed >> 6u) & 0x1FF8u;
    indices[1] = (packed >> 16u) & 0x1FF8u;
    indices[2] = (packed2 & 1023u) << 3u;
    indices[3] = (packed2 >> 7u) & 0x1FF8u;
    clipped_vector(0u, normals + indices[0]);
    clipped_vector(1u, normals + indices[1]);
    xport_gte_execute(0x4A2012u);
    for (i = 0u; i < count; ++i) {
        uv[i][0] = xport_gte_read_data(9u);
        uv[i][1] = xport_gte_read_data(10u);
        w_u8(packet + offsets[i], uv[i][0]);
        w_u8(packet + offsets[i] + 1u, uv[i][1]);
        if (i + 1u < count) {
            if (i + 2u < count) clipped_vector(i & 1u, normals + indices[i + 2u]);
            xport_gte_execute(i & 1u ? 0x4A2012u : 0x4AA012u);
        }
    }
    if ((sint32)flags >= 0) {
        w_u32(packet, (count == 3u ? 7u : 9u) << 24u);
        w_u32(packet + 4u, color);
        w_u16(packet + 14u, texture); w_u16(packet + 22u, texture >> 16u);
        if (count == 3u) {
            w_u32(packet + 24u, xport_gte_read_data(12u));
            w_u32(packet + 16u, xport_gte_read_data(13u));
            w_u32(packet + 8u, xport_gte_read_data(14u));
        } else {
            w_u32(packet + 32u, xport_gte_read_data(12u));
            w_u32(packet + 8u, xport_gte_read_data(13u));
            w_u32(packet + 24u, xport_gte_read_data(14u));
            w_u32(packet + 16u, r_u32(context + 1004u));
        }
        clipped_link(tag, packet);
        packet += count == 3u ? 32u : 40u;
    } else {
        w_u16(context + 184u, count);
        w_u32(context + 168u, tag); w_u32(context + 172u, packet);
        w_u32(context + 156u, color); w_u32(context + 164u, texture);
        for (i = 0u; i < count; ++i)
            for (j = 0u; j < 2u; ++j) w_u16(context + 336u + i * 32u + j * 2u, (uv[i][j] << 7u) | 64u);
        gtos_model_clip_texture(context);
        packet = r_u32(context + 180u);
    }
    /* The textured source pass reuses V0/V1 after the reflection UV pass */
    if (restore_vectors) for (i = 0u; i < 4u; ++i) gte_lwc2(i, original[i]);
    return packet;
}

static uint32 clipped_record_flags(uint32 context, uint32 record, uint32 depth, uint32 flag)
{
    sint32 x = (sint16)r_u16(record + 12u), y = (sint16)r_u16(record + 14u);
    uint32 bits = (sint32)depth < (sint32)r_u16(context + 8u) ? 1u : 0u;
    if (x < 0) bits |= 2u;
    if ((sint16)r_u16(context + 4u) < x) bits |= 4u;
    if (y < 0) bits |= 8u;
    if ((sint16)r_u16(context + 6u) < y) bits |= 16u;
    if ((sint32)flag < 0) bits |= 32u;
    w_u16(record, bits);
    return bits;
}

static void clipped_project_records(uint32 context, uint32 records, uint32 count,
    uint32 use_sz, uint32 *minimum, uint32 *maximum, uint32 *common)
{
    uint32 i;
    clipped_vector(0u, records + 4u);
    xport_gte_execute(0x180001u);
    for (i = 0u; i < count; ++i) {
        uint32 flag = clipped_flag();
        uint32 depth = xport_gte_read_data(use_sz ? 19u : 11u);
        uint32 record = records + 16u * i;
        w_u32(record + 12u, xport_gte_read_data(14u));
        clipped_vector(0u, record + 20u);
        xport_gte_execute(0x180001u);
        *common &= clipped_record_flags(context, record, depth, flag);
        if ((sint32)depth < (sint32)*minimum) *minimum = depth;
        if ((sint32)*maximum < (sint32)depth) *maximum = depth;
    }
}

static void clipped_midpoint(uint32 output, uint32 first, uint32 second)
{
    uint32 i;
    for (i = 0u; i < 3u; ++i) {
        sint32 a = (sint16)r_u16(first + 4u + 2u * i);
        sint32 b = (sint16)r_u16(second + 4u + 2u * i);
        w_u16(output + 4u + 2u * i, (uint32)((a + b) >> 1));
    }
    for (i = 0u; i < 2u; ++i)
        w_u8(output + 2u + i, (r_u8(first + 2u + i) + r_u8(second + 2u + i)) >> 1u);
}

static uint32 clipped_subdivision_packet(uint32 context, uint32 first, uint32 second,
    uint32 third, uint32 fourth, uint32 quad, uint32 bridge, uint32 descend, uint32 *area)
{
    uint32 packet = r_u32(context + 180u), color = r_u32(context + (bridge && quad ? 160u : 156u));
    uint32 clut = (uint32)r_u16(context + 164u) << 16u, texture = r_u32(context + 164u) & 0xFFFF0000u;
    uint32 saved_area, size = quad && !bridge ? 40u : 32u;
    gte_lwc2(12u, r_u32(first + 12u));
    gte_lwc2(13u, r_u32(second + 12u));
    gte_lwc2(14u, r_u32(third + 12u));
    xport_gte_execute(0x1400006u);
    saved_area = xport_gte_read_data(24u);
    w_u32(packet + 12u, clut | r_u16(first + 2u));
    w_u32(packet + 20u, texture | r_u16(second + 2u));
    w_u32(packet + 28u, (quad ? clut : (bridge ? 0u : color & 0xFFFF0000u)) | r_u16(third + 2u));
    w_u32(packet + 8u, xport_gte_read_data(12u));
    if (bridge) w_u32(packet + 16u, xport_gte_read_data(13u));
    if (quad && !bridge) {
        gte_lwc2(12u, r_u32(fourth + 12u));
        xport_gte_execute(0x1400006u);
        w_u32(packet + 36u, texture | r_u16(fourth + 2u));
        w_u32(packet + 8u, r_u32(first + 12u));
        w_u32(packet + 4u, color);
        w_u8(packet + 3u, 9u); clipped_chain_next(packet, packet + 40u);
        w_u32(packet + 32u, xport_gte_read_data(12u));
        w_u32(packet + 16u, xport_gte_read_data(13u));
        w_u32(packet + 24u, xport_gte_read_data(14u));
        *area = clipped_absolute(saved_area - xport_gte_read_data(24u));
    } else {
        *area = clipped_absolute(saved_area);
        if (!bridge) w_u32(packet + 4u, color);
        if (bridge) {
            w_u8(packet + 3u, 7u); clipped_chain_next(packet, packet + 32u);
        }
        if (bridge && saved_area) {
            w_u32(packet + 24u, xport_gte_read_data(14u));
            w_u32(packet + 4u, color);
        }
    }
    if (bridge && !saved_area) return 0u;
    if (!bridge && descend && (sint32)*area > (sint32)r_u32(context + 184u)) return 1u;
    if (!bridge && !quad) {
        w_u32(packet + 16u, xport_gte_read_data(13u));
        w_u32(packet + 24u, xport_gte_read_data(14u));
    }
    if (!bridge) { w_u8(packet + 3u, size == 40u ? 9u : 7u); clipped_chain_next(packet, packet + size); }
    w_u32(context + 176u, packet); w_u32(context + 180u, packet + size);
    return 0u;
}

static void clipped_subdivide_triangle(uint32 context, const uint32 root[3], uint32 midpoint, uint32 depth)
{
    uint32 mid[3] = {midpoint, midpoint + 16u, midpoint + 32u};
    uint32 common = 65535u, minimum = 0u, maximum = 0u, area, i;
    sint32 index;
    for (i = 0u; i < 3u; ++i) w_u32(context + 204u + depth * 12u + i * 4u, root[i]);
    for (i = 0u; i < 3u; ++i) clipped_midpoint(mid[i], root[i], root[(i + 1u) % 3u]);
    clipped_project_records(context, midpoint, 3u, 1u, &minimum, &maximum, &common);
    for (index = 2; index >= 0; --index) {
        uint32 previous = index ? (uint32)index - 1u : 2u;
        if (!((r_u16(root[index]) | r_u16(mid[previous]) | r_u16(root[previous])) & 32u))
            clipped_subdivision_packet(context, root[index], mid[previous], root[previous], 0u, 0u, 1u, 0u, &area);
    }
    for (index = 3; index >= 0; --index) {
        uint32 child[3], bits, combined;
        if (index == 3) { child[0] = mid[0]; child[1] = mid[1]; child[2] = mid[2]; }
        else { child[0] = root[index]; child[1] = mid[index]; child[2] = mid[index ? index - 1 : 2]; }
        bits = r_u16(child[0]) & r_u16(child[1]) & r_u16(child[2]);
        if (bits & 31u) continue;
        combined = r_u16(child[0]) | r_u16(child[1]) | r_u16(child[2]);
        w_u32(context + 216u + depth * 12u, child[0]);
        if (!(combined & 32u)) {
            uint32 ordered[3] = {child[1], child[0], child[2]};
            if (!clipped_subdivision_packet(context, ordered[0], ordered[1], ordered[2], 0u,
                0u, 0u, depth < r_u16(context + 188u), &area)) continue;
        } else if (depth >= r_u16(context + 188u)) continue;
        w_u8(context + 196u + depth, (uint32)(index - 1));
        clipped_subdivide_triangle(context, child, midpoint + 48u, depth + 1u);
    }
}

static void clipped_subdivide_quad(uint32 context, const uint32 root[4], uint32 midpoint, uint32 depth)
{
    uint32 mid[5], i, component, common = 65535u, minimum = 0u, maximum = 0u, area;
    sint32 index;
    for (i = 0u; i < 5u; ++i) mid[i] = midpoint + 16u * i;
    for (i = 0u; i < 4u; ++i) {
        w_u32(context + 204u + depth * 16u + i * 4u, root[i]);
        clipped_midpoint(mid[i + 1u], root[i], root[(i + 1u) & 3u]);
    }
    for (component = 0u; component < 3u; ++component) {
        sint32 total = 0;
        for (i = 0u; i < 4u; ++i) total += (sint16)r_u16(root[i] + 4u + 2u * component);
        w_u16(mid[0] + 4u + 2u * component, (uint32)(total >> 2));
    }
    for (component = 0u; component < 2u; ++component) {
        uint32 total = 0u;
        for (i = 0u; i < 4u; ++i) total += r_u8(root[i] + 2u + component);
        w_u8(mid[0] + 2u + component, total >> 2u);
    }
    clipped_project_records(context, midpoint, 5u, 0u, &minimum, &maximum, &common);
    for (i = 0u; i < 4u; ++i) {
        uint32 next = (i + 1u) & 3u;
        if (!((r_u16(root[i]) | r_u16(root[next]) | r_u16(mid[i + 1u])) & 32u))
            clipped_subdivision_packet(context, root[i], root[next], mid[i + 1u], 0u, 1u, 1u, 0u, &area);
    }
    for (index = 3; index >= 0; --index) {
        uint32 previous = ((uint32)index - 1u) & 3u, child[4], bits = 65535u, combined = 0u;
        if (index & 1) {
            child[0] = mid[previous + 1u]; child[1] = root[index]; child[2] = mid[index + 1u]; child[3] = mid[0];
        } else {
            child[0] = root[index]; child[1] = mid[index + 1u]; child[2] = mid[0]; child[3] = mid[previous + 1u];
        }
        for (i = 0u; i < 4u; ++i) { bits &= r_u16(child[i]); combined |= r_u16(child[i]); }
        if (bits & 31u) continue;
        w_u32(context + 220u + depth * 16u, child[0]);
        if (!(combined & 32u)) {
            if (!clipped_subdivision_packet(context, child[1], child[0], child[2], child[3],
                1u, 0u, depth < r_u16(context + 188u), &area)) continue;
        } else if (depth >= r_u16(context + 188u)) continue;
        w_u8(context + 196u + depth, (uint32)(index - 1));
        clipped_subdivide_quad(context, child, midpoint + 80u, depth + 1u);
    }
}

static uint32 clipped_inline_texture(uint32 context, uint32 source, uint32 count,
    uint32 tag, uint32 packet)
{
    uint32 i, root[4], common = 65535u, minimum = 0u, maximum = 0u;
    uint32 uv0 = r_u32(source + 16u) + r_u32(context + 904u);
    uint32 uv1 = r_u32(source + 20u), uv2 = r_u32(source + 24u);
    w_u16(context + 188u, 5u);
    w_u32(context + 168u, tag); w_u32(context + 172u, packet);
    w_u32(context + 184u, count == 3u ? 1152u : 2048u);
    w_u32(context + 156u, xport_gte_read_data(6u));
    w_u32(context + 164u, uv1);
    w_u16(context + 164u, uv0 >> 16u);
    if (count == 3u) {
        clipped_save_vector(0u, context + 340u);
        clipped_save_vector(1u, context + 324u);
        clipped_save_vector(2u, context + 308u);
    } else {
        w_u32(context + 324u, r_u32(context + 356u));
        w_u32(context + 328u, r_u32(context + 360u));
        clipped_save_vector(0u, context + 356u);
        clipped_save_vector(1u, context + 340u);
        clipped_save_vector(2u, context + 308u);
        w_u32(context + 352u, uv2);
    }
    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2);
    for (i = 0u; i < count; ++i) { root[i] = context + 304u + 16u * i; w_u32(context + 204u + 4u * i, root[i]); }
    w_u32(context + 176u, packet); w_u32(context + 180u, packet);
    w_u32(context + 160u, count == 3u ? r_u32(context + 156u) | 0x08000000u : r_u32(context + 156u) & 0xF7FFFFFFu);
    clipped_project_records(context, root[0], count, 0u, &minimum, &maximum, &common);
    if (!(common & 31u) && (sint32)(maximum -
        (uint32)((sint32)(maximum - minimum) >> ((r_u8(context + 188u) + 1u) & 31u))) >= (sint32)r_u16(context + 8u)) {
        if (count == 3u) clipped_subdivide_triangle(context, root, context + 352u, 0u);
        else clipped_subdivide_quad(context, root, context + 368u, 0u);
        if (packet != r_u32(context + 180u)) {
            uint32 last = r_u32(context + 176u), old = r_u32(tag) & 0xFFFFFFu;
            clipped_chain_next(tag, packet); clipped_chain_next(last, old);
        }
    }
    return r_u32(context + 180u);
}

static void clipped_save_polygon(uint32 context, uint32 count)
{
    if (count == 3u) {
        clipped_save_vector(0u, context + 388u);
        clipped_save_vector(1u, context + 356u);
    } else {
        clipped_save_vector(0u, context + 420u);
        clipped_save_vector(1u, context + 388u);
    }
    clipped_save_vector(2u, context + 324u);
}

static uint32 clipped_surface(uint32 context, uint32 source, uint32 group,
    uint32 count, uint32 tag, uint32 packet, uint32 flags, uint32 area)
{
    uint32 color, colors[4], uv[3], i, size;
    gte_lwc2(6u, r_u32(source + 12u));
    colors[0] = xport_gte_read_data(6u);
    if (group == 2u || group == 3u || group >= 6u) {
        uint32 first_color = group >= 6u ? 28u : 16u;
        gte_lwc2(20u, r_u32(source + first_color));
        gte_lwc2(21u, r_u32(source + first_color + 4u));
        colors[1] = xport_gte_read_data(20u); colors[2] = xport_gte_read_data(21u);
        if (count == 4u) { gte_lwc2(22u, r_u32(source + first_color + 8u)); colors[3] = xport_gte_read_data(22u); }
    }
    if (group >= 4u) {
        uv[0] = r_u32(source + 16u) + r_u32(context + 904u);
        uv[1] = r_u32(source + 20u); uv[2] = r_u32(source + 24u);
    }
    if ((group == 4u && (sint32)area >= 800) || (group == 5u && (sint32)area >= 1568)) flags = 0xFFFFFFFFu;
    if ((sint32)flags < 0) {
        w_u32(context + 168u, tag); w_u32(context + 172u, packet);
        if (group == 4u || group == 5u) return clipped_inline_texture(context, source, count, tag, packet);
        w_u16(context + 184u, count);
        clipped_save_polygon(context, count);
        w_u32(context + 156u, colors[0]);
        if (group == 0u || group == 1u) {
            gtos_model_clip_flat(context);
        } else {
            for (i = 0u; i < count; ++i) w_u32(context + 340u + i * 32u, colors[i]);
            if (group >= 6u) {
                w_u32(context + 164u, uv[1]); w_u16(context + 164u, uv[0] >> 16u);
                for (i = 0u; i < count; ++i) {
                    uint32 value = i < 3u ? uv[i] : uv[2] >> 16u;
                    w_u16(context + 336u + 32u * i, ((value << 7u) & 0x7F80u) | 64u);
                    w_u16(context + 338u + 32u * i, ((value >> 1u) & 0x7F80u) | 64u);
                }
                gtos_model_clip_gouraud_texture(context);
            } else gtos_model_clip_gouraud(context);
        }
        return r_u32(context + 180u);
    }
    color = colors[0];
    if (group < 2u) {
        size = count == 3u ? 20u : 24u;
        w_u32(packet, (size / 4u - 1u) << 24u); w_u32(packet + 4u, color);
        if (count == 3u) {
            w_u32(packet + 8u, xport_gte_read_data(14u));
            w_u32(packet + 12u, xport_gte_read_data(13u));
            w_u32(packet + 16u, xport_gte_read_data(12u));
        } else {
            w_u32(packet + 20u, xport_gte_read_data(12u));
            w_u32(packet + 8u, xport_gte_read_data(13u));
            w_u32(packet + 16u, xport_gte_read_data(14u));
            w_u32(packet + 12u, r_u32(context + 1004u));
        }
    } else if (group < 4u) {
        size = count == 3u ? 28u : 36u;
        w_u32(packet, (size / 4u - 1u) << 24u);
        w_u32(packet + 4u, colors[0]); w_u32(packet + 12u, colors[1]);
        if (count == 3u) {
            w_u32(packet + 8u, xport_gte_read_data(14u));
            w_u32(packet + 16u, xport_gte_read_data(13u));
            w_u32(packet + 24u, xport_gte_read_data(12u));
            w_u32(packet + 20u, colors[2]);
        } else {
            w_u32(packet + 32u, xport_gte_read_data(12u));
            w_u32(packet + 8u, xport_gte_read_data(13u));
            w_u32(packet + 24u, xport_gte_read_data(14u));
            w_u32(packet + 16u, r_u32(context + 1004u));
            w_u32(packet + 28u, colors[2]); w_u32(packet + 20u, colors[3]);
        }
    } else {
        uint32 gouraud = group >= 6u;
        size = gouraud ? (count == 3u ? 40u : 52u) : (count == 3u ? 32u : 40u);
        w_u32(packet, (size / 4u - 1u) << 24u); w_u32(packet + 4u, color);
        w_u32(packet + 12u, uv[0]);
        w_u32(packet + (gouraud ? 24u : 20u), uv[1]);
        w_u32(packet + (count == 3u ? (gouraud ? 36u : 28u) : (gouraud ? 48u : 36u)), uv[2]);
        if (count == 3u) {
            w_u32(packet + 8u, xport_gte_read_data(14u));
            w_u32(packet + (gouraud ? 20u : 16u), xport_gte_read_data(13u));
            w_u32(packet + (gouraud ? 32u : 24u), xport_gte_read_data(12u));
        } else {
            uint32 fourth_uv = gouraud ? 36u : 28u;
            /* SWL at an odd halfword writes the upper two source bytes */
            w_u16(packet + fourth_uv, uv[2] >> 16u);
            w_u32(packet + (gouraud ? 44u : 32u), xport_gte_read_data(12u));
            w_u32(packet + 8u, xport_gte_read_data(13u));
            w_u32(packet + (gouraud ? 32u : 24u), xport_gte_read_data(14u));
            w_u32(packet + (gouraud ? 20u : 16u), r_u32(context + 1004u));
        }
        if (gouraud) {
            w_u32(packet + 16u, colors[1]);
            w_u32(packet + (count == 3u ? 28u : 40u), colors[2]);
            if (count == 4u) w_u32(packet + 28u, colors[3]);
        }
    }
    clipped_link(tag, packet);
    return packet + size;
}

uint32 gtos_model_polygons_clipped(uint32 model, uint32 context)
{
    static const uint32 strides[8] = {16u, 16u, 24u, 28u, 28u, 28u, 36u, 40u};
    uint32 source = r_u32(model + 8u), result = source, group;
    w_u32(context + 992u, source);
    for (group = 0u; group < 8u; ++group) {
        uint32 count = r_u16(model + 44u + 2u * group), packet, packed;
        uint32 vertices, vertex[3], remaining, corner_count = (group & 1u) ? 4u : 3u;
        if (!count) continue;
        packet = r_u32(context + 932u); packed = r_u32(source); vertices = r_u32(context + 940u);
        vertex[0] = vertices + ((packed & 511u) << 3u);
        vertex[1] = vertices + ((packed >> 6u) & 0xFF8u);
        vertex[2] = vertices + ((packed >> 15u) & 0xFF8u);
        result = (packed >> 15u) & 0xFF8u;
        for (remaining = count; remaining; --remaining, source += strides[group]) {
            uint32 word = packed, next, flags, area0, area1 = 0u, facing, tag, shift;
            uint32 depth[4], minimum, maximum, i, screen[3];
            uint32 reflection = r_u16(context + 908u), auxiliary;
            if (corner_count == 3u) {
                clipped_vector(0u, vertex[2]); clipped_vector(1u, vertex[1]); clipped_vector(2u, vertex[0]);
            } else {
                clipped_vector(0u, vertex[1]); clipped_vector(1u, vertex[2]);
                clipped_save_vector(0u, context + 356u);
                clipped_vector(2u, vertex[0]);
            }
            xport_gte_execute(0x280030u);
            next = r_u32(source + strides[group]); auxiliary = r_u32(source + 4u);
            vertices = r_u32(context + 940u);
            vertex[0] = vertices + ((next & 511u) << 3u);
            vertex[1] = vertices + ((next >> 6u) & 0xFF8u);
            vertex[2] = vertices + ((next >> 15u) & 0xFF8u); packed = next;
            flags = clipped_flag();
            for (i = 0u; i < 3u; ++i) { depth[i] = xport_gte_read_data(17u + i); screen[i] = xport_gte_read_data(12u + i); }
            if (corner_count == 4u) {
                w_u32(context + 1004u, screen[0]); w_u32(context + 364u, screen[0]);
            }
            xport_gte_execute(0x1400006u); area0 = xport_gte_read_data(24u);
            minimum = maximum = depth[0];
            for (i = 1u; i < 3u; ++i) { if (depth[i] < minimum) minimum = depth[i]; if (depth[i] > maximum) maximum = depth[i]; }
            if (corner_count == 4u) {
                clipped_vector(0u, vertices + ((auxiliary & 511u) << 3u));
                xport_gte_execute(0x180001u); flags |= clipped_flag();
                depth[3] = xport_gte_read_data(19u);
                if (depth[3] < minimum) minimum = depth[3]; if (depth[3] > maximum) maximum = depth[3];
                xport_gte_execute(0x1400006u); area1 = xport_gte_read_data(24u);
                if (!reflection) area0 = 0u - area0;
            }
            if (reflection) {
                if (corner_count == 3u) area0 = 0u - area0;
                else area1 = 0u - area1;
            }
            result = corner_count == 3u ? minimum : (maximum < r_u16(context + 8u));
            if (maximum < r_u16(context + 8u)) continue;
            shift = r_u16(context + 40u) & 31u;
            tag = ((auxiliary & 0x80000000u) ? maximum : minimum) << shift;
            facing = word & area0; if (corner_count == 4u) facing &= area1;
            if ((sint32)flags < 0) {
                facing = 0u;
                if ((sint32)word < 0) { clipped_save_polygon(context, corner_count); facing = clipped_facing_overflow(context); }
            }
            tag = (uint32)((sint32)tag >> 13);
            if ((sint32)facing < 0) continue;
            if ((sint32)tag >= 4096) tag = 4095u;
            tag = r_u32(context + 928u) + tag * 4u;
            if (corner_count == 3u) {
                w_u32(context + 388u + 8u, xport_gte_read_data(12u));
                w_u32(context + 356u + 8u, xport_gte_read_data(13u));
                w_u32(context + 324u + 8u, xport_gte_read_data(14u));
            } else {
                w_u32(context + 396u, xport_gte_read_data(12u));
                w_u32(context + 332u, xport_gte_read_data(13u));
                w_u32(context + 428u, xport_gte_read_data(14u));
            }
            if (!clipped_screen(context, corner_count, &flags, &result)) continue;
            if (r_u32(context + 912u) & word) {
                packet = clipped_reflection(context, source, corner_count, tag, packet, flags, group == 4u || group == 5u);
            }
            if (group == 4u) area0 = clipped_absolute(area0);
            if (group == 5u) area0 = clipped_absolute(area0) + clipped_absolute(area1);
            packet = clipped_surface(context, source, group, corner_count, tag, packet, flags, area0);
            if ((sint32)flags < 0 || (group == 4u && (sint32)area0 >= 800) || (group == 5u && (sint32)area0 >= 1568)) {
                clipped_restore_lights(context); result = r_u32(context + 964u);
            } else result = (packet - (group < 2u ? (corner_count == 3u ? 20u : 24u) :
                group < 4u ? (corner_count == 3u ? 28u : 36u) : group < 6u ? (corner_count == 3u ? 32u : 40u) :
                (corner_count == 3u ? 40u : 52u))) << 8u;
        }
        w_u32(context + 992u, source); w_u32(context + 932u, packet);
        model = r_u32(context + 936u);
    }
    /* TODO Refine the unused V0 carrier on rejected polygons */
    return result;
}
