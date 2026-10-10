#include "game_draft_registry.h"
#include "psx_gte.h"

/* GTOS.EXE 80031F64 */
static void polygon_link(uint32 table, uint32 packet)
{
    uint32 old = r_u32(table);
    w_u16(table, (uint16)packet);
    w_u8(table + 2u, (uint8)(packet >> 16u));
    w_u16(packet, (uint16)old);
    w_u8(packet + 2u, (uint8)(old >> 16u));
}

static void polygon_load_vector(uint32 slot, uint32 vector)
{
    gte_lwc2(slot * 2u, r_u32(vector));
    gte_lwc2(slot * 2u + 1u, r_u32(vector + 4u));
}

static void polygon_store_data(uint32 index, uint32 destination)
{
    w_u32(destination, xport_gte_read_data(index));
}

static uint32 polygon_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static uint32 polygon_uv(uint32 point, uint32 carrier)
{
    return (carrier & 0xFFFF0000u) | r_u16(point + 2u);
}

static uint32 polygon_area(uint32 a, uint32 b, uint32 c)
{
    gte_lwc2(12u, r_u32(a + 12u));
    gte_lwc2(13u, r_u32(b + 12u));
    gte_lwc2(14u, r_u32(c + 12u));
    xport_gte_execute(0x1400006u);
    return xport_gte_read_data(24u);
}

static void polygon_chain_accept(uint32 context, uint32 packet, uint32 bytes)
{
    w_u32(context + 176u, packet);
    w_u32(context + 180u, packet + bytes);
}

static void polygon_chain_tag(uint32 packet, uint32 bytes)
{
    w_u8(packet + 3u, (uint8)(bytes / 4u - 1u));
    w_u16(packet, (uint16)(packet + bytes));
    w_u8(packet + 2u, (uint8)((packet + bytes) >> 16u));
}

static void polygon_triangle_split(uint32 context, uint32 points, uint32 mids, uint32 depth)
{
    uint32 p[3], m[3], i, component, packet, area, carrier_a, carrier_b;
    sint32 child;
    for (i = 0u; i < 3u; ++i) {
        p[i] = r_u32(points + 4u * i);
        m[i] = mids + 16u * i;
    }
    for (component = 4u; component <= 8u; component += 2u) {
        sint32 v[3];
        for (i = 0u; i < 3u; ++i) v[i] = (sint16)r_u16(p[i] + component);
        for (i = 0u; i < 3u; ++i)
            w_u16(m[i] + component, (uint16)((v[i] + v[(i + 1u) % 3u]) >> 1));
    }
    for (i = 0u; i < 3u; ++i) polygon_load_vector(i, m[i] + 4u);
    xport_gte_execute(0x280030u);
    for (component = 2u; component <= 3u; ++component)
        for (i = 0u; i < 3u; ++i)
            w_u8(m[i] + component, (uint8)((r_u8(p[i] + component) + r_u8(p[(i + 1u) % 3u] + component)) >> 1u));
    for (i = 0u; i < 3u; ++i) polygon_store_data(12u + i, m[i] + 12u);
    carrier_a = (r_u32(context + 164u) & 0xFFFFu) << 16u;
    carrier_b = r_u32(context + 164u);
    packet = r_u32(context + 180u);
    for (child = 2; child >= 0; --child) {
        uint32 previous = child == 0 ? 2u : (uint32)child - 1u;
        area = polygon_area(p[child], m[previous], p[previous]);
        w_u8(packet + 3u, 7u);
        w_u32(packet + 28u, polygon_uv(p[previous], p[previous]));
        w_u32(packet + 12u, polygon_uv(p[child], carrier_a));
        w_u32(packet + 20u, polygon_uv(m[previous], carrier_b));
        polygon_chain_tag(packet, 32u);
        polygon_store_data(12u, packet + 8u);
        polygon_store_data(13u, packet + 16u);
        if (area != 0u) {
            polygon_store_data(14u, packet + 24u);
            w_u32(packet + 4u, r_u32(context + 156u));
            w_u32(context + 176u, packet);
            packet += 32u;
        }
    }
    w_u32(context + 180u, packet);
    for (child = 3; child >= 0; --child) {
        uint32 a, b, c;
        if (child == 3) { a = m[1]; b = m[0]; c = m[2]; }
        else { a = m[child]; b = p[child]; c = m[child == 0 ? 2 : child - 1]; }
        area = polygon_area(a, b, c);
        packet = r_u32(context + 180u);
        w_u32(packet + 12u, polygon_uv(a, carrier_a));
        w_u32(packet + 20u, polygon_uv(b, carrier_b));
        polygon_store_data(12u, packet + 8u);
        polygon_store_data(13u, packet + 16u);
        polygon_store_data(14u, packet + 24u);
        if ((sint32)polygon_abs(area) >= (sint32)r_u32(context + 184u) && depth < r_u16(context + 188u)) {
            w_u8(context + 196u + depth, (uint8)(child - 1));
            w_u32(points + 12u, b);
            w_u32(points + 16u, a);
            w_u32(points + 20u, c);
            polygon_triangle_split(context, points + 12u, mids + 48u, depth + 1u);
            carrier_a = (r_u32(context + 164u) & 0xFFFFu) << 16u;
            carrier_b = r_u32(context + 164u);
        } else {
            w_u32(packet + 4u, r_u32(context + 156u));
            w_u32(packet + 28u, polygon_uv(c, carrier_b));
            polygon_chain_tag(packet, 32u);
            polygon_chain_accept(context, packet, 32u);
        }
    }
}

static void polygon_quad_split(uint32 context, uint32 points, uint32 mids, uint32 depth)
{
    uint32 p[4], edge[4], i, component, packet, first, second, carrier_a, carrier_b;
    sint32 child;
    for (i = 0u; i < 4u; ++i) { p[i] = r_u32(points + 4u * i); edge[i] = mids + 16u + 16u * i; }
    for (component = 4u; component <= 8u; component += 2u) {
        sint32 v[4], sum = 0;
        for (i = 0u; i < 4u; ++i) { v[i] = (sint16)r_u16(p[i] + component); sum += v[i]; }
        for (i = 0u; i < 4u; ++i) w_u16(edge[i] + component, (uint16)((v[i] + v[(i + 1u) & 3u]) >> 1));
        w_u16(mids + component, (uint16)(sum >> 2));
    }
    polygon_load_vector(0u, mids + 4u);
    polygon_load_vector(1u, edge[0] + 4u);
    polygon_load_vector(2u, edge[1] + 4u);
    xport_gte_execute(0x280030u);
    for (component = 2u; component <= 3u; ++component) {
        uint32 sum = 0u;
        for (i = 0u; i < 4u; ++i) {
            sum += r_u8(p[i] + component);
            w_u8(edge[i] + component, (uint8)((r_u8(p[i] + component) + r_u8(p[(i + 1u) & 3u] + component)) >> 1u));
        }
        w_u8(mids + component, (uint8)(sum >> 2u));
        if (component == 2u) {
            polygon_store_data(12u, mids + 12u);
            polygon_store_data(13u, edge[0] + 12u);
            polygon_store_data(14u, edge[1] + 12u);
            polygon_load_vector(1u, edge[2] + 4u);
            polygon_load_vector(2u, edge[3] + 4u);
            xport_gte_execute(0x280030u);
        }
    }
    polygon_store_data(13u, edge[2] + 12u);
    polygon_store_data(14u, edge[3] + 12u);
    carrier_a = (r_u32(context + 164u) & 0xFFFFu) << 16u;
    carrier_b = r_u32(context + 164u);
    packet = r_u32(context + 180u);
    for (i = 0u; i < 4u; ++i) {
        uint32 area = polygon_area(p[i], p[(i + 1u) & 3u], edge[i]);
        w_u32(packet + 4u, r_u32(context + 160u));
        w_u32(packet + 12u, polygon_uv(p[i], carrier_a));
        w_u32(packet + 20u, polygon_uv(p[(i + 1u) & 3u], carrier_b));
        w_u32(packet + 28u, polygon_uv(edge[i], r_u32(context + 160u)));
        polygon_store_data(12u, packet + 8u);
        polygon_store_data(13u, packet + 16u);
        if (area != 0u) {
            polygon_store_data(14u, packet + 24u);
            polygon_chain_tag(packet, 32u);
            w_u32(context + 176u, packet);
            packet += 32u;
        }
    }
    w_u32(context + 180u, packet);
    for (child = 3; child >= 0; --child) {
        uint32 a, b, c, d, previous = ((uint32)child - 1u) & 3u;
        if (child & 1) { a = p[child]; b = edge[previous]; c = edge[child]; d = mids; }
        else { a = edge[child]; b = p[child]; c = mids; d = edge[previous]; }
        first = polygon_area(a, b, c);
        packet = r_u32(context + 180u);
        w_u32(packet + 4u, r_u32(context + 156u));
        w_u32(packet + 12u, polygon_uv(a, carrier_a));
        w_u32(packet + 20u, polygon_uv(b, carrier_b));
        w_u32(packet + 28u, polygon_uv(c, carrier_a));
        gte_lwc2(12u, r_u32(d + 12u));
        xport_gte_execute(0x1400006u);
        second = xport_gte_read_data(24u);
        w_u32(packet + 36u, polygon_uv(d, carrier_b));
        w_u32(packet + 8u, r_u32(a + 12u));
        polygon_chain_tag(packet, 40u);
        polygon_store_data(13u, packet + 16u);
        if ((sint32)polygon_abs(first - second) >= (sint32)r_u32(context + 184u) && depth < r_u16(context + 188u)) {
            w_u8(context + 196u + depth, (uint8)(child - 1));
            w_u32(points + 16u, b);
            w_u32(points + 20u, a);
            w_u32(points + 24u, c);
            w_u32(points + 28u, d);
            polygon_quad_split(context, points + 16u, mids + 80u, depth + 1u);
        } else {
            polygon_store_data(14u, packet + 24u);
            polygon_store_data(12u, packet + 32u);
            polygon_chain_accept(context, packet, 40u);
        }
    }
}

static uint32 polygon_subdivide(uint32 context, uint32 table, uint32 packet, int quad, uint32 uv0, uint32 uv1, uint32 uv2)
{
    uint32 i, old, last, first;
    w_u16(context + 188u, 5u);
    w_u32(context + 168u, table);
    w_u32(context + 172u, packet);
    w_u32(context + 184u, quad ? 2048u : 1152u);
    w_u16(context + 306u, (uint16)uv0);
    w_u16(context + 322u, (uint16)uv1);
    w_u16(context + 338u, (uint16)uv2);
    if (quad) w_u32(context + 352u, uv2);
    polygon_store_data(6u, context + 156u);
    w_u32(context + 164u, uv1);
    w_u16(context + 164u, (uint16)(uv0 >> 16u));
    if (quad) {
        polygon_store_data(12u, context + 348u);
        polygon_store_data(13u, context + 316u);
        polygon_store_data(14u, context + 364u);
    } else {
        polygon_store_data(12u, context + 348u);
        polygon_store_data(13u, context + 332u);
        polygon_store_data(14u, context + 316u);
    }
    for (i = 0u; i < (quad ? 4u : 3u); ++i) w_u32(context + 204u + 4u * i, context + 304u + 16u * i);
    w_u32(context + 176u, packet);
    w_u32(context + 180u, packet);
    w_u32(context + 160u, quad ? r_u32(context + 156u) & 0xF7FFFFFFu : r_u32(context + 156u) | 0x08000000u);
    if (quad) polygon_quad_split(context, context + 204u, context + 368u, 0u);
    else polygon_triangle_split(context, context + 204u, context + 352u, 0u);
    /* The original unwind reads the depth-minus-one continuation byte */
    (void)r_u8(context + 195u);
    first = r_u32(context + 172u);
    last = r_u32(context + 176u);
    old = r_u32(table);
    w_u16(table, (uint16)first); w_u8(table + 2u, (uint8)(first >> 16u));
    w_u16(last, (uint16)old); w_u8(last + 2u, (uint8)(old >> 16u));
    return r_u32(context + 180u);
}

static uint32 polygon_overlay(uint32 context, uint32 record, uint32 packet, uint32 table, int quad)
{
    uint32 normal_base = r_u32(context + 944u), word1 = r_u32(record + 4u), word2 = r_u32(record + 8u);
    uint32 packed = r_u32(context + 916u);
    if (quad) {
        polygon_store_data(12u, packet + 32u); polygon_store_data(13u, packet + 8u); polygon_store_data(14u, packet + 24u);
        w_u32(packet + 16u, r_u32(context + 332u));
    } else {
        polygon_store_data(12u, packet + 24u); polygon_store_data(13u, packet + 16u); polygon_store_data(14u, packet + 8u);
    }
    polygon_load_vector(0u, normal_base + ((word1 >> 6u) & 0x1FF8u));
    xport_gte_execute(0x4A2012u);
    polygon_load_vector(1u, normal_base + ((word1 >> 16u) & 0x1FF8u));
    w_u8(packet + 12u, (uint8)xport_gte_read_data(9u)); w_u8(packet + 13u, (uint8)xport_gte_read_data(10u));
    xport_gte_execute(0x4AA012u);
    polygon_load_vector(2u, normal_base + 8u * (word2 & 0x3FFu));
    w_u8(packet + 20u, (uint8)xport_gte_read_data(9u)); w_u8(packet + 21u, (uint8)xport_gte_read_data(10u));
    xport_gte_execute(0x4B2012u);
    if (quad) {
        polygon_load_vector(0u, normal_base + ((word2 >> 7u) & 0x1FF8u));
        w_u8(packet + 36u, (uint8)xport_gte_read_data(9u)); w_u8(packet + 37u, (uint8)xport_gte_read_data(10u));
        xport_gte_execute(0x4A2012u);
    }
    w_u32(packet, quad ? 0x09000000u : 0x07000000u);
    w_u16(packet + 14u, (uint16)packed);
    w_u32(packet + 4u, r_u32(context + (quad ? 972u : 968u)));
    w_u16(packet + 22u, (uint16)(packed >> 16u));
    w_u8(packet + 28u, (uint8)xport_gte_read_data(9u)); w_u8(packet + 29u, (uint8)xport_gte_read_data(10u));
    polygon_link(table, packet);
    return packet + (quad ? 40u : 32u);
}

uint32 gtos_model_polygons(uint32 model, uint32 context)
{
    static const uint8 record_bytes[8] = { 16u, 16u, 24u, 28u, 28u, 28u, 36u, 40u };
    static const uint8 packet_bytes[8] = { 20u, 24u, 28u, 36u, 32u, 40u, 40u, 52u };
    uint32 result = r_u32(model + 8u), group, record = result;
    w_u32(context + 992u, record);
    for (group = 0u; group < 8u; ++group) {
        uint32 count = r_u16(model + 44u + 2u * group), packet, flags, next_flags, vertices;
        int quad = (group & 1u) != 0u;
        if (count == 0u) continue;
        flags = r_u32(record);
        packet = r_u32(context + 932u);
        vertices = r_u32(context + 940u);
        result = (flags >> 15u) & 0xFF8u;
        while (count-- != 0u) {
            uint32 word1, word2, depth, area0, area1 = 0u, uv0 = 0u, uv1 = 0u, uv2 = 0u;
            uint32 address0 = vertices + 8u * (flags & 0x1FFu);
            uint32 address1 = vertices + ((flags >> 6u) & 0xFF8u);
            uint32 address2 = vertices + ((flags >> 15u) & 0xFF8u);
            uint32 table, overlay, i;
            sint32 bucket;
            if (quad) {
                polygon_load_vector(0u, address1); polygon_load_vector(1u, address2); polygon_load_vector(2u, address0);
                if (group == 5u) { polygon_store_data(0u, context + 324u); polygon_store_data(1u, context + 328u); }
            } else {
                polygon_load_vector(0u, address2); polygon_load_vector(1u, address1); polygon_load_vector(2u, address0);
            }
            xport_gte_execute(0x280030u);
            next_flags = r_u32(record + record_bytes[group]);
            word1 = r_u32(record + 4u);
            word2 = r_u32(record + 8u);
            xport_gte_execute(0x1400006u);
            area0 = xport_gte_read_data(24u);
            depth = xport_gte_read_data(17u);
            for (i = 18u; i <= (quad ? 19u : 18u); ++i) {
                uint32 value = xport_gte_read_data(i);
                if (((sint32)depth < (sint32)value) == (int)(word1 >> 31u)) depth = value;
            }
            if (quad) {
                polygon_load_vector(0u, vertices + 8u * (word1 & 0x1FFu));
                polygon_store_data(12u, context + 332u);
                xport_gte_execute(0x180001u);
                gte_lwc2(6u, r_u32(record + 12u));
                if (((sint32)depth < (sint32)xport_gte_read_data(19u)) == (int)(word1 >> 31u)) depth = xport_gte_read_data(19u);
                xport_gte_execute(0x1400006u);
                area1 = xport_gte_read_data(24u);
                area0 = 0u - area0;
                if ((sint16)r_u16(context + 908u) != 0) { area0 = 0u - area0; area1 = 0u - area1; }
                result = group == 5u ? area1 : area1 & area0 & flags;
            } else {
                gte_lwc2(6u, r_u32(record + 12u));
                if ((sint16)r_u16(context + 908u) != 0) area0 = 0u - area0;
                result = depth;
            }
            if (((quad ? area1 & area0 : area0) & flags & 0x80000000u) == 0u) {
                if (!quad) {
                    uint32 value = xport_gte_read_data(19u);
                    if (((sint32)depth < (sint32)value) == (int)(word1 >> 31u)) depth = value;
                }
                bucket = (sint32)(depth << (r_u16(context + 40u) & 31u)) >> 13;
                if (bucket >= 4096) bucket = 4095;
                table = r_u32(context + 928u) + ((uint32)bucket << 2u);
                overlay = flags & r_u32(context + 912u);
                if ((group == 4u || group == 5u) && (overlay != 0u ||
                    (sint32)(quad ? polygon_abs(area0) + polygon_abs(area1) : polygon_abs(area0)) >= (quad ? 1568 : 800))) {
                    if (quad) {
                        polygon_store_data(0u, context + 356u); polygon_store_data(1u, context + 360u);
                        polygon_store_data(2u, context + 340u); polygon_store_data(3u, context + 344u);
                    } else {
                        polygon_store_data(0u, context + 340u); polygon_store_data(1u, context + 344u);
                        polygon_store_data(2u, context + 324u); polygon_store_data(3u, context + 328u);
                    }
                    polygon_store_data(4u, context + 308u); polygon_store_data(5u, context + 312u);
                }
                if (overlay != 0u) packet = polygon_overlay(context, record, packet, table, quad);
                if (group >= 4u) { uv0 = r_u32(record + 16u) + r_u32(context + 904u); uv1 = r_u32(record + 20u); uv2 = r_u32(record + 24u); }
                if ((group == 4u || group == 5u) &&
                    (sint32)(quad ? polygon_abs(area0) + polygon_abs(area1) : polygon_abs(area0)) >= (quad ? 1568 : 800)) {
                    packet = polygon_subdivide(context, table, packet, quad, uv0, uv1, uv2);
                    result = r_u32(context + 172u) << 8u;
                } else {
                    if (group == 2u || group == 3u || group == 6u || group == 7u) {
                        uint32 base = group < 4u ? 16u : 28u;
                        gte_lwc2(20u, r_u32(record + base)); gte_lwc2(21u, r_u32(record + base + 4u));
                        if (quad) gte_lwc2(22u, r_u32(record + base + 8u));
                    }
                    w_u32(packet, (uint32)(packet_bytes[group] / 4u - 1u) << 24u);
                    polygon_link(table, packet);
                    switch (group) {
                    case 0u:
                        polygon_store_data(12u, packet + 16u); polygon_store_data(13u, packet + 12u); polygon_store_data(14u, packet + 8u);
                        result = packet << 8u; break;
                    case 1u:
                        polygon_store_data(12u, packet + 20u); polygon_store_data(13u, packet + 8u); polygon_store_data(14u, packet + 16u);
                        result = r_u32(context + 332u); w_u32(packet + 12u, result); break;
                    case 2u:
                        polygon_store_data(12u, packet + 24u); polygon_store_data(13u, packet + 16u); polygon_store_data(14u, packet + 8u);
                        polygon_store_data(20u, packet + 12u); polygon_store_data(21u, packet + 20u); result = packet << 8u; break;
                    case 3u:
                        polygon_store_data(12u, packet + 32u); polygon_store_data(13u, packet + 8u); polygon_store_data(14u, packet + 24u);
                        result = r_u32(context + 332u); w_u32(packet + 16u, result);
                        polygon_store_data(20u, packet + 12u); polygon_store_data(21u, packet + 28u); polygon_store_data(22u, packet + 20u); break;
                    case 4u:
                        w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1); w_u32(packet + 28u, uv2);
                        polygon_store_data(12u, packet + 24u); polygon_store_data(13u, packet + 16u); polygon_store_data(14u, packet + 8u);
                        result = packet << 8u; break;
                    case 5u:
                        w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1); w_u32(packet + 36u, uv2);
                        w_u16(packet + 28u, (uint16)(uv2 >> 16u));
                        polygon_store_data(12u, packet + 32u); polygon_store_data(13u, packet + 8u); polygon_store_data(14u, packet + 24u);
                        result = r_u32(context + 332u); w_u32(packet + 16u, result); break;
                    case 6u:
                        w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 36u, uv2);
                        polygon_store_data(12u, packet + 32u); polygon_store_data(13u, packet + 20u); polygon_store_data(14u, packet + 8u);
                        polygon_store_data(20u, packet + 16u); polygon_store_data(21u, packet + 28u); result = packet << 8u; break;
                    default:
                        w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 48u, uv2);
                        w_u16(packet + 36u, (uint16)(uv2 >> 16u));
                        polygon_store_data(12u, packet + 44u); polygon_store_data(13u, packet + 8u); polygon_store_data(14u, packet + 32u);
                        result = r_u32(context + 332u); w_u32(packet + 20u, result);
                        polygon_store_data(20u, packet + 16u); polygon_store_data(21u, packet + 40u); polygon_store_data(22u, packet + 28u); break;
                    }
                    polygon_store_data(6u, packet + 4u);
                    packet += packet_bytes[group];
                }
            }
            flags = next_flags;
            record += record_bytes[group];
        }
        if (group != 7u) model = r_u32(context + 936u);
        w_u32(context + 992u, record);
        w_u32(context + 932u, packet);
    }
    return result;
}
