#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include "psx_gte.h"

static sint32 gtmain_collision_sub_checked(sint32 first, sint32 second)
{
    sint64 result = (sint64)first - second;
    if (result < -2147483648LL || result > 2147483647LL)
        gt_draft_failure("Original collision SUB overflow", 0x80024398u);
    return (sint32)result;
}

uint32 gtmain_course_interval_find(sint32 lap, sint32 progress)
{
    uint32 list = r_u32(r_u32(0x800BC1B0u) + 24u);
    uint32 course_length = r_u32(r_u32(0x800BC1A8u));
    sint32 lap_count = r_u16(list + 2u);
    uint32 count = r_u16(list), index;
    if (lap_count && lap_count < lap)
        lap = (sint32)((sint32)((uint32)lap - 1u) % lap_count) + 1;
    for (index = 0; index < count; ++index)
    {
        uint32 record = r_u32(list + index * 4u + 4u);
        sint32 first_lap = r_u16(record + 2u), last_lap = r_u16(record + 4u);
        uint32 end, begin, current;
        if (lap < first_lap || last_lap < lap) continue;
        end = r_u32(record + 12u); begin = r_u32(record + 8u); current = (uint32)progress;
        if ((sint32)end < (sint32)begin) end += course_length;
        if (first_lap < lap) current += course_length;
        if ((sint32)current >= (sint32)begin && (sint32)current < (sint32)end) return record;
    }
    return 0;
}

uint32 gtmain_collision_probe_segment(uint8 *probes, uint32 segment)
{
    uint32 result = r_u32(segment + 152u);
    uint32 count = r_u32(result), edges = result + 4u;
    uint32 index, vertices;
    uint8 *current;
    uint32 packed_first, packed_second;
    if (!count) return result;
    {
        uint32 x_origin = r_u32(segment + 48u) & 0xFFF00000u;
        uint32 z_origin = r_u32(segment + 56u) & 0xFFF00000u;
        for (index = 0; index < 4u; ++index)
        {
            uint8 *probe = probes + index * 44u;
            uint32 a = xport_load_le32(probe), b = xport_load_le32(probe + 4u);
            uint32 c = xport_load_le32(probe + 12u), d = xport_load_le32(probe + 16u);
            uint32 x1 = (uint32)((sint32)(a - x_origin) >> 8);
            uint32 z1 = (uint32)((sint32)(b - z_origin) >> 8);
            uint32 x2 = (uint32)((sint32)(c - x_origin) >> 8);
            uint32 z2 = (uint32)((sint32)(d - z_origin) >> 8);
            uint32 dx = x2 - x1, dz = z2 - z1;
            xport_store_le16(probe + 32u, (uint16)x1); xport_store_le16(probe + 34u, (uint16)z1);
            xport_store_le16(probe + 36u, (uint16)z2); xport_store_le16(probe + 38u, (uint16)x2);
            if (x1 + 32767u > 65534u || x2 + 32767u > 65534u || z1 + 32767u > 65534u || z2 + 32767u > 65534u)
                dx = dz = 0;
            result = 0u - dz;
            xport_store_le16(probe + 40u, (uint16)result); xport_store_le16(probe + 42u, (uint16)dx);
        }
    }
    gte_lwc2(1u, 0); gte_lwc2(3u, 0); gte_ctc2(0, 4u); gte_ctc2(0, 12u);
    current = probes;
    vertices = r_u32(segment + 164u);
    packed_first = xport_load_le32(current + 32u); packed_second = xport_load_le32(current + 36u);
    while (count)
    {
        uint32 pair = r_u32(edges);
        uint32 first = r_u32(vertices + (pair & 65535u) * 8u);
        uint32 second = r_u32(vertices + (pair >> 16) * 8u);
        --count;
        gte_lwc2(0u, r_u32(edges + 4u)); gte_ctc2(first, 3u); gte_ctc2(first, 8u); gte_ctc2(second, 11u);
        for (index = 4u; index; )
        {
            uint8 *next;
            sint32 third, a, b;
            uint32 difference;
            gte_ctc2(packed_first, 0u); gte_ctc2(packed_second, 1u); gte_ctc2(packed_second, 2u);
            --index;
            xport_gte_execute(0x406012u);
            gte_ctc2(packed_second, 9u); gte_ctc2(packed_second, 10u);
            gte_lwc2(2u, xport_load_le32(current + 40u));
            next = index ? current + 44u : probes;
            packed_first = xport_load_le32(next + 32u); packed_second = xport_load_le32(next + 36u);
            third = (sint32)xport_gte_read_data(27u);
            a = gtmain_collision_sub_checked((sint32)xport_gte_read_data(25u), third);
            b = gtmain_collision_sub_checked((sint32)xport_gte_read_data(26u), third);
            xport_gte_execute(0x42E012u);
            result = (uint32)a ^ (uint32)b;
            difference = (uint32)a - (uint32)b;
            if ((sint32)result < 0)
            {
                uint32 middle = xport_gte_read_data(26u);
                uint32 begin = xport_gte_read_data(25u);
                uint32 end = xport_gte_read_data(27u);
                result = (begin - middle) ^ (end - middle);
                if ((sint32)result < 0)
                {
                    sint32 previous = (sint32)xport_load_le32(current + 24u);
                    result = (uint32)gtmain_signed_divide64((sint64)a * 4096,
                        (sint64)(sint32)difference, 0x800243F4u);
                    if ((sint32)result < previous)
                    {
                        uint32 normal;
                        xport_store_le32(current + 24u, result);
                        xport_store_le32(current + 28u, xport_gte_read_data(0u));
                        normal = (uint32)(sint32)(sint16)xport_load_le16(current + 28u);
                        result = 0u - normal;
                        xport_store_le16(current + 28u, xport_load_le16(current + 30u));
                        xport_store_le16(current + 30u, (uint16)result);
                    }
                }
            }
            current = next;
        }
        edges += 8u;
    }
    return result;
}

uint32 gtmain_collision_probes_update(uint32 segment_index, uint8 *probes)
{
    uint32 segment, result;
    xport_store_le32(probes + 24u, 4096); xport_store_le32(probes + 68u, 4096);
    xport_store_le32(probes + 112u, 4096); xport_store_le32(probes + 156u, 4096);
    segment = r_u32(r_u32(0x800BC1A8u) + segment_index * 4u + 12u);
    gtmain_collision_probe_segment(probes, segment);
    gtmain_collision_probe_segment(probes, r_u32(segment + 8u));
    result = gtmain_collision_probe_segment(probes, r_u32(segment + 4u));
    return result;
}

static sint32 gtmain_collision_clip(uint32 point, uint32 first, uint32 second)
{
    gte_lwc2(15u, point); gte_lwc2(15u, first); gte_lwc2(15u, second);
    xport_gte_execute(0x1400006u);
    return (sint32)xport_gte_read_data(24u);
}

uint32 gtmain_collision_polygon_find(uint32 segment, const uint8 *position)
{
    uint32 x_origin = r_u32(segment + 48u);
    uint32 x_position = xport_load_le32(position);
    uint32 grid = r_u32(segment + 156u);
    uint32 z_origin = r_u32(segment + 56u), z_position = xport_load_le32(position + 8u);
    uint32 x = (uint32)((sint32)(x_position - (x_origin & 0xFFF00000u)) >> 8);
    uint32 z = (uint32)((sint32)(z_position - (z_origin & 0xFFF00000u)) >> 8);
    uint32 x_cell = (uint32)((sint32)(x - (uint32)(sint32)(sint16)r_u16(grid)) >> (r_u16(grid + 4u) & 31u));
    uint32 z_cell = (uint32)((sint32)(z - (uint32)(sint32)(sint16)r_u16(grid + 2u)) >> (r_u16(grid + 6u) & 31u));
    uint32 cell, count, list, vertices, point;
    if (x_cell >= 4u || z_cell >= 4u) return 0;
    point = (x & 65535u) | (z << 16);
    cell = z_cell * 4u + x_cell;
    count = r_u16(grid + cell * 2u + 8u);
    list = r_u32(grid + cell * 4u + 40u);
    vertices = r_u32(segment + 164u);
    while (count)
    {
        uint32 polygon = r_u32(list);
        uint32 flags = r_u32(polygon), flags_second = r_u32(polygon + 4u);
        uint32 first = r_u32(vertices + (flags & 0x1FFu) * 8u);
        uint32 second = r_u32(vertices + ((flags >> 6) & 0xFF8u));
        uint32 third, fourth;
        sint32 clipped;
        --count;
        clipped = gtmain_collision_clip(point, first, second);
        third = r_u32(vertices + ((flags >> 15) & 0xFF8u));
        if (clipped >= 0)
        {
            clipped = gtmain_collision_clip(point, second, third);
            fourth = r_u32(vertices + (flags_second & 0x1FFu) * 8u);
            if (clipped >= 0)
            {
                if (!(flags & 0x40000000u)) clipped = gtmain_collision_clip(point, third, first);
                else
                {
                    clipped = gtmain_collision_clip(point, third, fourth);
                    if (clipped >= 0) clipped = gtmain_collision_clip(point, fourth, first);
                }
                if (clipped >= 0) return polygon;
            }
        }
        list += 4u;
    }
    return 0;
}

uint32 gtmain_collision_polygon_height(uint32 segment, uint32 polygon, const uint8 *position)
{
    uint32 x_origin = r_u32(segment + 48u), x_position = xport_load_le32(position);
    uint32 z_origin = r_u32(segment + 56u), z_position = xport_load_le32(position + 8u);
    sint32 x = (sint32)(x_position - (x_origin & 0xFFF00000u)) >> 8;
    sint32 z = (sint32)(z_position - (z_origin & 0xFFF00000u)) >> 8;
    uint32 y_origin = r_u32(segment + 52u), flags = r_u32(polygon);
    uint32 vertices = r_u32(segment + 164u), flags_second = r_u32(polygon + 4u);
    uint32 first_address = vertices + (flags & 0x1FFu) * 8u;
    uint32 second_address = vertices + ((flags >> 6) & 0xFF8u);
    uint32 third_address = vertices + ((flags >> 15) & 0xFF8u);
    const uint8 *first, *second, *third;
    uint8 center[6];
    uint32 quotient = 0;
    if (flags & 0x40000000u)
    {
        uint32 fourth_address = vertices + (flags_second & 0x1FFu) * 8u;
        uint32 addresses[4], mask = 0, index;
        sint32 center_x, center_z;
        addresses[0] = first_address; addresses[1] = second_address; addresses[2] = third_address; addresses[3] = fourth_address;
        for (index = 0; index < 3u; ++index)
        {
            sint32 sum = (sint16)r_u16(addresses[0] + index * 2u);
            sum += (sint16)r_u16(addresses[1] + index * 2u);
            sum += (sint16)r_u16(addresses[2] + index * 2u);
            sum += (sint16)r_u16(addresses[3] + index * 2u);
            xport_store_le16(center + index * 2u, (uint16)(sum / 4));
        }
        center_x = (sint16)xport_load_le16(center); center_z = (sint16)xport_load_le16(center + 2u);
        gte_ctc2((uint32)x - (uint32)center_x, 0u); gte_ctc2((uint32)z - (uint32)center_z, 2u);
        {
            uint32 packed = r_u32(addresses[0]);
            for (index = 0; index < 4u; ++index)
            {
                gte_lwc2(9u, (uint32)(sint32)(sint16)packed - (uint32)center_x);
                gte_lwc2(10u, (uint32)((sint32)packed >> 16) - (uint32)center_z);
                xport_gte_execute(0x170000Cu);
                if (index < 3u) packed = r_u32(addresses[index + 1u]);
                if ((sint32)xport_gte_read_data(27u) > 0) mask |= 1u << index;
            }
        }
        switch (mask)
        {
        case 1: case 3: case 7: first_address = fourth_address; second_address = addresses[0]; break;
        case 4: case 12: case 13: first_address = addresses[1]; second_address = addresses[2]; break;
        case 8: case 9: case 11: first_address = addresses[2]; second_address = addresses[3]; break;
        default: first_address = addresses[0]; second_address = addresses[1]; break;
        }
        third = center;
    }
    else third = (const uint8 *)psx_addr(third_address, 6u);
    first = (const uint8 *)psx_addr(first_address, 6u); second = (const uint8 *)psx_addr(second_address, 6u);
    {
        sint32 origin_z = (sint16)xport_load_le16(third + 2u), origin_x = (sint16)xport_load_le16(third);
        uint32 b_z = (uint32)(sint32)(sint16)xport_load_le16(second + 2u) - (uint32)origin_z;
        uint32 dx = (uint32)x - (uint32)origin_x;
        uint32 b_x = (uint32)(sint32)(sint16)xport_load_le16(second) - (uint32)origin_x;
        uint32 dz = (uint32)z - (uint32)origin_z;
        uint32 a_x = (uint32)(sint32)(sint16)xport_load_le16(first) - (uint32)origin_x;
        sint32 origin_y = (sint16)xport_load_le16(third + 4u);
        uint32 a_y = (uint32)(sint32)(sint16)xport_load_le16(first + 4u) - (uint32)origin_y;
        uint32 determinant_first = b_z * dx - b_x * dz;
        uint32 a_z = (uint32)(sint32)(sint16)xport_load_le16(first + 2u) - (uint32)origin_z;
        uint32 determinant_second = a_x * dz - a_z * dx;
        uint32 b_y = (uint32)(sint32)(sint16)xport_load_le16(second + 4u) - (uint32)origin_y;
        uint32 denominator = a_x * b_z - a_z * b_x;
        uint64 sum = (uint64)((sint64)(sint32)a_y * (sint32)determinant_first)
            + (uint64)((sint64)(sint32)b_y * (sint32)determinant_second);
        if (denominator) quotient = (uint32)gtmain_signed_divide64((sint64)(sum << 8),
            (sint64)(sint32)denominator, 0x80025A38u);
        return quotient + ((uint32)origin_y << 8) + (y_origin & 0xFFF00000u);
    }
}

uint32 gtmain_collision_query(uint8 *query)
{
    uint16 index = (uint16)gtmain_course_segment_find(xport_load_le16(query + 4u), query + 8u);
    uint32 owner, segment, polygon, result;
    xport_store_le16(query + 4u, index);
    owner = r_u32(r_u32(0x800BC1A8u) + (uint32)index * 4u + 12u);
    segment = owner; polygon = gtmain_collision_polygon_find(segment, query + 8u);
    if (!polygon) { segment = r_u32(segment + 8u); polygon = gtmain_collision_polygon_find(segment, query + 8u); }
    if (!polygon) { segment = r_u32(owner + 4u); polygon = gtmain_collision_polygon_find(segment, query + 8u); }
    if (polygon)
    {
        uint32 flags = r_u32(polygon + 4u);
        xport_store_le32(query, 0); query[0] = (uint8)(flags >> 28); query[1] = (uint8)((flags >> 23) & 3u); query[2] = (uint8)((flags >> 25) & 3u);
        result = gtmain_collision_polygon_height(segment, polygon, query + 8u);
    }
    else { result = 0x7FFFFFFFu; xport_store_le32(query, 0); query[0] = 7; query[1] = 4; }
    xport_store_le32(query + 20u, result);
    return result;
}

uint32 gtmain_collision_edges_end(uint32 edges)
{
    return edges + r_u32(edges) * 8u + 4u;
}

uint32 gtmain_collision_values_end(uint32 values)
{
    return values + ((r_u16(values) | 1u) * 2u) + 2u;
}

uint32 gtmain_collision_grid_relocate(uint32 grid, uint32 segment)
{
    uint32 destination = grid + 104u, cell;
    for (cell = 0; cell < 16u; ++cell)
    {
        uint32 count_address = grid + cell * 2u + 8u;
        uint32 index = 0;
        w_u32(grid + cell * 4u + 40u, destination);
        if (r_u16(count_address))
        {
            do
            {
                uint32 encoded = r_u32(destination);
                uint32 type = (encoded >> 16) & 7u;
                uint32 stride = r_u8(0x8008F720u + type);
                uint32 base = r_u32(segment + type * 4u + 168u);
                ++index;
                w_u32(destination, stride * (encoded & 65535u) + base);
                destination += 4u;
            } while (index < r_u16(count_address));
        }
    }
    return destination;
}

uint32 gtmain_course_segments_relocate(uint32 course)
{
    uint32 count = r_u16(course + 4u), index;
    uint32 next = course + count * 4u + 12u;
    uint32 end;
    for (index = 0; index < count; ++index)
    {
        uint32 segment = next;
        w_u32(course + index * 4u + 12u, segment);
        next = gt_draft_800245A0(segment + 164u, 0, 0, 0, 0, 0, 0, 0);
        w_u32(segment + 148u, next);
        next = gt_draft_800245A0(next, 0, 0, 0, 0, 0, 0, 0);
        w_u32(segment + 152u, next);
        next = gtmain_collision_edges_end(next);
        w_u32(segment + 156u, next);
        next = gtmain_collision_grid_relocate(next, segment);
        w_u32(segment + 160u, next);
        next = gtmain_collision_values_end(next);
    }
    if (count) count = r_u16(course + 4u);
    {
        uint32 objects = r_u16(course + 6u);
        w_u32(course + 8u, next);
        end = next + objects * 32u;
        end += r_u32(end) * 16u + 4u;
    }
    for (index = 0; index < count; ++index)
    {
        uint32 segment = r_u32(course + index * 4u + 12u);
        uint32 previous = r_u16(segment);
        uint32 following;
        /* Original zero-extended halfwords cannot equal the full-word minus-one sentinel */
        w_u32(segment + 4u, previous == 0xFFFFFFFFu ? 0 : r_u32(course + previous * 4u + 12u));
        following = r_u16(segment + 2u);
        w_u32(segment + 8u, following == 0xFFFFFFFFu ? 0 : r_u32(course + following * 4u + 12u));
    }
    return end;
}

uint32 gtmain_distance_groups_relocate(uint32 groups)
{
    uint32 count = r_u32(groups), index;
    uint32 next = groups + count * 4u + 4u;
    for (index = 0; index < count; ++index)
    {
        w_u32(groups + index * 4u + 4u, next);
        next += r_u16(next) * 8u + 4u;
    }
    return next;
}

void gtmain_distance_groups_bind(uint32 groups, uint32 records)
{
    uint32 count = r_u32(groups), index;
    for (index = 0; index < count; ++index)
    {
        uint32 group = r_u32(groups + index * 4u + 4u);
        uint32 entry_count, entry;
        gtmain_model_distances_square(group);
        entry_count = r_u16(group);
        for (entry = 0; entry < entry_count; ++entry)
        {
            uint32 field = group + entry * 8u + 8u;
            uint32 record_index = r_u32(field);
            if (record_index != 0xFFFFFFFFu) w_u32(field, r_u32(records + record_index * 4u + 4u));
        }
    }
}

uint32 gtmain_object_tail_end(uint32 tail)
{
    uint32 type = r_u16(tail);
    if (!type) return tail + r_u16(tail + 2u) * 16u + 4u;
    if (type == 1u) return tail + r_u16(tail + 2u) * 56u + 4u;
    return 0;
}

uint32 gtmain_object_records_relocate(uint32 records)
{
    uint32 count = r_u16(records), index;
    uint32 next = records + count * 4u + 4u;
    for (index = 0; index < count; ++index)
    {
        uint32 type;
        w_u32(records + index * 4u + 4u, next);
        type = r_u16(next);
        switch (type)
        {
        case 0: next = gtmain_object_tail_end(next + 24u); break;
        case 1: next += 40u; break;
        case 2: next += 20u; break;
        case 3: next = gtmain_object_tail_end(next + 28u); break;
        default: break;
        }
    }
    return next;
}

