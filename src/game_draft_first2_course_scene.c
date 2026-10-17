#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include "psx_gte.h"

static void scene_ctc(uint32 index, uint32 value)
{
    gte_ctc2(value, index);
}

static uint32 scene_gte_flags(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return (uint32)state.flag;
}

static uint32 scene_lz_count(uint32 value)
{
    xport_gte_write_data(30u, value);
    return xport_gte_read_data(31u);
}

static void scene_matrix(uint32 address, uint32 first)
{
    uint32 index;
    for (index = 0; index < 5; ++index) scene_ctc(first + index, r_u32(address + 4u * index));
}

uint32 gtmain_car_collision_positions(uint32 car)
{
    uint32 first[3], second[3], shift[3], index, total;
    w_u32(car + 20u, r_u32(car + 1932u));
    for (index = 0; index < 3; ++index) first[index] = r_u32(car + 2228u + 4u * index);
    for (index = 0; index < 3; ++index) w_u32(car + 2308u + 4u * index, first[index]);
    gtmain_collision_layout(psx_addr(car + 2228u, 38u), psx_addr(car + 100u, 34u));
    gtmain_collision_layout(psx_addr(car + 2268u, 38u), psx_addr(car + 136u, 34u));
    gtmain_matrix_angles(psx_addr(car + 2240u, 6u), psx_addr(car + 2248u, 18u));
    scene_matrix(car + 2248u, 0);
    xport_gte_write_data(0, 0); xport_gte_write_data(1, 0u - r_u32(car + 1492u));
    xport_gte_execute(0x406012u);
    for (index = 0; index < 3; ++index) first[index] = r_u32(car + 2228u + 4u * index);
    for (index = 0; index < 3; ++index) shift[index] = (uint32)((sint32)xport_gte_read_data(25u + index) >> 8);
    for (index = 0; index < 3; ++index) second[index] = r_u32(car + 2268u + 4u * index);
    for (index = 0; index < 3; ++index) first[index] += shift[index];
    for (index = 0; index < 3; ++index) second[index] += shift[index];
    for (index = 0; index < 3; ++index) w_u32(car + 2228u + 4u * index, first[index]);
    for (index = 0; index < 3; ++index) w_u32(car + 2268u + 4u * index, second[index]);
    for (index = 0; index < 4; ++index)
    {
        uint32 source = car + 172u + 68u * index, out = car + 2024u + 24u * index, axis;
        w_u16(out + 14u, r_u16(source + 14u) - r_u16(source + 4u));
        xport_gte_write_data(0, r_u32(out + 12u)); xport_gte_write_data(1, r_u32(out + 16u));
        xport_gte_execute(0x406012u);
        for (axis = 0; axis < 3; ++axis) shift[axis] = (uint32)((sint32)xport_gte_read_data(25u + axis) >> 8);
        for (axis = 0; axis < 3; ++axis) w_u32(out + axis * 4u, first[axis] + shift[axis]);
    }
    gtmain_vector_subtract(psx_addr(car + 2320u, 12u), psx_addr(car + 2228u, 12u), psx_addr(car + 2308u, 12u));
    total = ((uint32)r_u8(car + 174u) + r_u8(car + 242u) + r_u8(car + 310u) + r_u8(car + 378u)) << 5;
    w_u16(car + 2222u, total);
    return total;
}

sint32 gtmain_collision_height_query(sint32 x, sint32 z, sint32 y, uint32 *segment)
{
    uint8 query[24];
    uint32 height;
    xport_store_le16(query + 4, (uint16)*segment);
    xport_store_le32(query + 8, (uint32)x << 4);
    xport_store_le32(query + 12, (uint32)y << 4);
    xport_store_le32(query + 16, (uint32)z << 4);
    gtmain_collision_query(query);
    *segment = xport_load_le16(query + 4);
    height = xport_load_le32(query + 20);
    return height == 0x7FFFFFFFu ? (sint32)height : (sint32)height >> 4;
}

void gtmain_car_rumble_update(uint32 car)
{
    uint32 slot; uint8 small, large;
    sint16 mode;
    if (r_u32(0x800B630Cu) == 1u || r_u8(0x800CB766u)) return;
    mode = (sint16)r_u16(car + 26u);
    if (mode == 1) slot = 0; else if (mode == 2) slot = 4; else return;
    large = r_u8(car + 1822u); small = r_u8(car + 1823u);
    if (r_u16(0x800B63D6u)) large = small = 0;
    gtmain_input_rumble(slot, small, large);
}

uint32 gtmain_course_segment_transform(uint32 context, uint32 segment)
{
    uint32 p[3], c[3], axis, shift = 0, packed, first[3], second[3], origin[3], coarse[3], final[3];
    uint64 square = 0;
    for (axis = 0; axis < 3; ++axis)
    {
        sint32 delta;
        p[axis] = r_u32(segment + 48u + 4u * axis); c[axis] = r_u32(context + 44u + 4u * axis);
        delta = (sint32)(p[axis] - c[axis]); square += (uint64)((sint64)delta * delta);
    }
    { sint32 radius = (sint32)r_u32(segment + 44u); square += (uint64)((sint64)radius * radius); }
    while (square > 0x400000000000ULL) { square >>= 2; ++shift; }
    w_u16(context + 40u, 5u - shift); w_u16(context + 42u, shift);
    for (axis = 0; axis < 3; ++axis)
    {
        uint32 off = axis * 6u;
        sint32 a = (sint16)r_u16(context + 864u + off), b = (sint16)r_u16(context + 866u + off), d;
        w_u16(context + 96u + off, a >> (shift & 31u));
        d = (sint16)r_u16(context + 868u + off);
        w_u16(context + 98u + off, b >> (shift & 31u)); w_u16(context + 100u + off, d >> (shift & 31u));
    }
    w_u16(context + 56u, (sint32)(c[0] - (p[0] & 0xFFF00000u)) >> ((shift + 8u) & 31u));
    w_u16(context + 58u, (sint32)(c[2] - (p[2] & 0xFFF00000u)) >> ((shift + 8u) & 31u));
    w_u16(context + 60u, (sint32)(c[1] - (p[1] & 0xFFF00000u)) >> ((shift + 8u) & 31u));
    scene_matrix(context + 96u, 0);
    xport_gte_write_data(0, ((p[0] & 0xFFFFFu) >> 8) | (((p[2] & 0xFFFFFu) >> 8) << 16));
    xport_gte_write_data(1, (p[1] & 0xFFFFFu) >> 8);
    xport_gte_execute(0x486012u);
    coarse[0] = (uint32)((sint32)(p[0] - r_u32(context + 884u)) >> 20);
    coarse[1] = (uint32)((sint32)(p[1] - r_u32(context + 888u)) >> 20);
    coarse[2] = (uint32)((sint32)(p[2] - r_u32(context + 892u)) >> 20);
    scene_matrix(context + 864u, 8);
    for (axis = 0; axis < 3; ++axis) first[axis] = xport_gte_read_data(25u + axis);
    origin[0] = r_u32(context + 896u); origin[1] = r_u32(context + 900u);
    xport_gte_execute(0x4A6012u);
    scene_matrix(context + 64u, 16); origin[2] = r_u32(context + 904u);
    for (axis = 0; axis < 3; ++axis) second[axis] = xport_gte_read_data(25u + axis);
    packed = (coarse[0] & 0xFFFFu) | (coarse[1] << 16);
    xport_gte_write_data(0, packed); xport_gte_write_data(1, coarse[2]); xport_gte_execute(0x446012u);
    for (axis = 0; axis < 3; ++axis) final[axis] = xport_gte_read_data(25u + axis);
    for (axis = 0; axis < 3; ++axis)
    {
        uint32 a = (uint32)((sint32)(final[axis] - origin[axis]) >> (shift & 31u));
        uint32 b = (uint32)((sint32)second[axis] >> (shift & 31u)) - first[axis];
        final[axis] = axis == 1u ? b - a : b + a;
    }
    for (axis = 0; axis < 3; ++axis) scene_ctc(5u + axis, final[axis]);
    for (axis = 0; axis < 3; ++axis) w_u32(context + 84u + 4u * axis, final[axis]);
    return packed;
}

uint32 gtmain_model_bounds_classify(uint32 context, const uint8 *points, uint32 count)
{
    uint32 either = 0, every = 0xFFFFFFFFu, screen_every = 0xFFFFFFFFu;
    uint32 threshold; sint32 width, height;
    xport_gte_write_data(0, xport_load_le32(points)); xport_gte_write_data(1, xport_load_le32(points + 4));
    xport_gte_execute(0x180001u);
    threshold = r_u16(context + 8u); width = (sint16)r_u16(context + 4u); height = (sint16)r_u16(context + 6u);
    while (count)
    {
        uint32 next0 = xport_load_le32(points + 8), next1 = xport_load_le32(points + 12);
        uint32 flags = scene_gte_flags(), ix = xport_gte_read_data(9), iy = xport_gte_read_data(10), iz = xport_gte_read_data(11), sxy = xport_gte_read_data(14), bits, screen = 0;
        sint32 sx = (sint16)sxy, sy = (sint16)(sxy >> 16);
        --count; points += 8;
        xport_gte_execute(0x4BE012u);
        xport_gte_write_data(2, (ix & 0xFFFFu) | (iy << 16)); xport_gte_write_data(3, iz);
        xport_gte_write_data(0, next0); xport_gte_write_data(1, next1);
        ix = xport_gte_read_data(9); iy = xport_gte_read_data(10);
        bits = (iz - threshold) >> 31;
        xport_gte_execute(0x4CE012u);
        bits |= ((uint32)((sint32)ix >> 31) & 2u) | ((uint32)((sint32)iy >> 31) & 4u);
        if ((sint32)flags < 0) bits |= 0x20u;
        ix = xport_gte_read_data(9); iy = xport_gte_read_data(10);
        bits |= (uint32)((sint32)ix >> 31) & 8u;
        xport_gte_execute(0x180001u);
        bits |= (uint32)((sint32)iy >> 31) & 16u;
        if (sx < 0) screen |= 2; if (sx >= width) screen |= 4;
        if (sy < 0) screen |= 8; if (sy >= height) screen |= 16;
        if ((sint32)flags < 0) screen = 0;
        either |= bits; every &= bits; screen_every &= screen;
    }
    return every | screen_every | (either << 16);
}

static sint32 scene_add_checked(sint32 first, sint64 second, uint32 address)
{
    sint64 value = (sint64)first + second;
    if (value < -2147483648LL || value > 2147483647LL) gt_draft_failure("Original billboard ADD/SUB overflow", address);
    return (sint32)value;
}

uint32 gtmain_model_billboards_draw(uint32 context, uint32 records, uint32 count)
{
    uint32 work = context + 184u, packet, attributes, links, shades, shift;
    if (!count) return records;
    scene_ctc(8, r_u16(context + 922u)); scene_ctc(9, (uint32)r_u16(context + 920u) << 16);
    scene_ctc(10, 0); scene_ctc(11, 0); scene_ctc(12, 0);
    packet = r_u32(context + 952u); attributes = r_u32(context + 944u);
    links = r_u32(context + 948u); shades = r_u32(context + 940u); shift = r_u16(context + 40u);
    do
    {
        uint32 packed = r_u32(records + 8u), bottom, top, attribute_index, flags, first_sxy, first_depth, axis;
        sint32 x, y, dx, dy, right, lower;
        uint32 left, upper;
        xport_gte_write_data(9, packed & 0xFFFFu);
        w_u32(work, r_u32(records)); xport_gte_execute(0x4BE012u);
        bottom = r_u32(records + 4u); top = bottom + (packed >> 16); attribute_index = (uint32)((sint32)bottom >> 16);
        x = (sint16)r_u16(work); y = (sint16)r_u16(work + 2u);
        dx = (sint32)xport_gte_read_data(9); dy = (sint32)xport_gte_read_data(10);
        left = (uint32)x - (uint32)dx; upper = (uint32)y + (uint32)dy;
        w_u16(work, left); w_u16(work + 2u, upper); w_u16(work + 4u, top);
        xport_gte_write_data(0, r_u32(work)); xport_gte_write_data(1, r_u32(work + 4u));
        right = scene_add_checked(x, dx, 0x80026670u);
        lower = scene_add_checked(y, -((sint64)dy), 0x80026674u);
        xport_gte_execute(0x180001u); xport_gte_write_data(6, r_u32(records + 12u));
        w_u16(work + 8u, right); w_u16(work + 10u, lower); w_u16(work + 12u, top);
        w_u16(work + 16u, left); w_u16(work + 18u, upper); w_u16(work + 20u, bottom);
        w_u16(work + 24u, right); w_u16(work + 26u, lower); w_u16(work + 28u, bottom);
        flags = scene_gte_flags();
        first_sxy = xport_gte_read_data(14); w_u32(packet + 8u, first_sxy); first_depth = xport_gte_read_data(19);
        for (axis = 0; axis < 6; ++axis) xport_gte_write_data(axis, r_u32(work + 8u + 4u * axis));
        xport_gte_execute(0x280030u);
        if ((sint32)flags >= 0)
        {
            uint32 attribute = attributes + (attribute_index << 5), a, b, c;
            a = r_u32(attribute + 4u); b = r_u32(attribute + 8u); c = r_u32(attribute + 12u);
            w_u32(packet + 12u, a); w_u32(packet + 20u, b); w_u32(packet + 28u, c);
            /* SWL at packet+0x25 writes the high two bytes */
            w_u8(packet + 36u, c >> 16); w_u8(packet + 37u, c >> 24);
            flags = scene_gte_flags();
            a = first_depth + xport_gte_read_data(17); b = xport_gte_read_data(18); c = xport_gte_read_data(19);
            if ((sint32)flags >= 0)
            {
                sint32 depth = (sint32)(a + b + c) >> 2;
                uint32 ordering, sxy[3], old_link, color;
                depth >>= shift & 31u; if (depth >= 4096) depth = 4095;
                color = r_u16(shades + ((uint32)depth << 1)); ordering = links + ((uint32)depth << 2);
                for (axis = 0; axis < 3; ++axis) { sxy[axis] = xport_gte_read_data(12u + axis); w_u32(packet + 16u + axis * 8u, sxy[axis]); }
                xport_gte_write_data(8, color); w_u32(packet, 0x09000000u); xport_gte_execute(0x780010u);
                if (!(first_sxy & sxy[0] & sxy[1] & sxy[2] & 0x80008000u))
                {
                    w_u32(packet + 4u, xport_gte_read_data(22));
                    old_link = (uint32)r_u8(ordering) << 8 | (uint32)r_u8(ordering + 1u) << 16 | (uint32)r_u8(ordering + 2u) << 24;
                    w_u8(ordering, packet); w_u8(ordering + 1u, packet >> 8); w_u8(ordering + 2u, packet >> 16);
                    w_u8(packet, old_link >> 8); w_u8(packet + 1u, old_link >> 16); w_u8(packet + 2u, old_link >> 24);
                    packet += 40u;
                }
            }
        }
        records += 16u;
    } while (--count);
    w_u32(context + 952u, packet);
    return records;
}

static void scene_secondary_render_missing(uint32 context, uint32 segment)
{
    /* TODO Bind unselected two-argument secondary renderer */
    gt_draft_failure("GTMAIN secondary course renderer", 0x8002723Cu);
}

static void scene_render_missing(uint32 owner, uint32 context, uint32 segment, uint32 mode)
{
    if (owner == 0x80011DA0u) {
        (void)gt_draft_80011DA0(context, segment, mode);
        return;
    }
    if (owner == 0x80010154u) {
        (void)gtmain_course_segment_draw(context, segment, mode);
        return;
    }
    scene_secondary_render_missing(context, segment);
}

uint32 gtmain_course_scene_draw(uint32 context, uint32 camera, uint32 mode)
{
    uint32 buckets[3][64], counts[3] = {0, 0, 0};
    uint32 course, segment, list, total, index, all_flags = 0, axis;
    uint32 packet_cursor, camera_state, attributes, matrix_first, matrix_second;
    sint32 angle;
    gtmain_model_translation(context, 0);
    gtmain_matrix_bind_packed_negation(psx_addr(context + 64u, 20u));
    gtmain_lighting_bind(context);
    packet_cursor = r_u32(0x800BFE4Cu);
    course = r_u32(0x800BC1A8u);
    camera_state = r_u32(0x800B6638u);
    w_u32(context + 948u, camera + 64u); w_u32(context + 952u, packet_cursor);
    attributes = r_u32(course + 8u);
    w_u32(context + 940u, 0x801A900Cu); w_u32(context + 944u, attributes);
    angle = (sint16)r_u16(0x8009423Cu + ((r_u16(camera_state + 134u) & 0xFFFu) << 1));
    w_u16(context + 920u, angle / 2);
    angle = (sint16)r_u16(0x80094A3Cu + ((r_u16(camera_state + 134u) & 0xFFFu) << 1));
    w_u32(context + 924u, mode);
    index = r_u32(0x8008F71Cu);
    w_u16(context + 922u, angle / 2);
    segment = r_u32(course + (index << 2) + 12u);
    list = r_u32(segment + 160u); total = r_u16(list); list += 2u;
    for (index = 0; index < total; ++index)
    {
        uint32 item = r_u16(list), group = item >> 14, bounds, near_y, far_y, point;
        sint32 classification; uint32 visibility;
        uint32 camera_x, camera_y, camera_z, near_height, far_height;
        segment = r_u32(r_u32(0x800BC1A8u) + ((item & 0x3FFFu) << 2) + 12u);
        list += 2u; all_flags |= r_u32(segment + 12u);
        bounds = segment + (mode ? 108u : 68u);
        camera_x = r_u32(context + 44u); camera_z = r_u32(context + 52u);
        near_height = r_u32(bounds + 32u); camera_y = r_u32(context + 48u);
        far_height = r_u32(bounds + 36u);
        near_y = near_height - camera_y; far_y = far_height - camera_y;
        for (point = 0; point < 8; ++point)
        {
            uint32 corner = bounds + ((point & 3u) << 3);
            uint32 x = r_u32(corner) - camera_x, z = r_u32(corner + 4u) - camera_z, y = point & 4u ? far_y : near_y;
            uint32 ax = (sint32)x < 0 ? 0u - x : x, ay = (sint32)y < 0 ? 0u - y : y, az = (sint32)z < 0 ? 0u - z : z;
            sint32 shift = (sint32)(19u - scene_lz_count(ax | ay | az));
            if (shift < 0) { uint32 amount = (0u - (uint32)shift) & 31u; x <<= amount; y <<= amount; z <<= amount; }
            else { x = (uint32)((sint32)x >> ((uint32)shift & 31u)); y = (uint32)((sint32)y >> ((uint32)shift & 31u)); z = (uint32)((sint32)z >> ((uint32)shift & 31u)); }
            w_u16(context + 184u + point * 8u, x); w_u16(context + 186u + point * 8u, y); w_u16(context + 188u + point * 8u, z);
        }
        visibility = gtmain_model_bounds_classify(context, psx_addr(context + 184u, 72u), 8);
        classification = visibility & 31u ? 2 : !!((visibility >> 16) & 63u);
        if (group < 3u)
        {
            if (classification < 2 && counts[classification] < 64u) buckets[classification][counts[classification]++] = segment;
        }
        else if (classification >= 0 && classification < 2 && counts[2] < 64u) buckets[2][counts[2]++] = segment;
    }
    /* Preserve the original interleaved transpose and axis negation stores */
    {
        uint32 a = r_u16(context + 64u), b = r_u16(context + 66u), c = r_u16(context + 68u);
        w_u16(context + 864u, a); a = r_u16(context + 70u);
        w_u16(context + 868u, b); w_u16(context + 866u, c); w_u16(context + 870u, 0u - a);
        a = r_u16(context + 72u); b = r_u16(context + 74u); c = r_u16(context + 76u);
        w_u16(context + 874u, 0u - a); w_u16(context + 872u, 0u - b);
        matrix_first = r_u32(context + 64u);
        a = r_u16(context + 78u); b = r_u16(context + 80u);
        w_u16(context + 876u, c); w_u16(context + 880u, a); w_u16(context + 878u, b);
    }
    matrix_second = r_u32(context + 68u); scene_ctc(8u, matrix_first);
    matrix_first = r_u32(context + 72u); scene_ctc(9u, matrix_second);
    matrix_second = r_u32(context + 76u); scene_ctc(10u, matrix_first);
    matrix_first = r_u32(context + 80u); scene_ctc(11u, matrix_second);
    scene_ctc(12u, matrix_first);
    {
        uint32 p[3], coarse[3];
        for (axis = 0; axis < 3; ++axis) { p[axis] = r_u32(context + 44u + 4u * axis); coarse[axis] = p[axis] & 0xFFF00000u; }
        xport_gte_write_data(0, (((p[0] - coarse[0]) >> 8) & 0xFFFFu) | ((p[1] - coarse[1]) >> 8 << 16));
        xport_gte_write_data(1, (p[2] - coarse[2]) >> 8);
        w_u32(context + 884u, coarse[0]); w_u32(context + 888u, coarse[1]);
        xport_gte_execute(0x4A6012u); w_u32(context + 892u, coarse[2]);
        for (axis = 0; axis < 3; ++axis) w_u32(context + 896u + axis * 4u, xport_gte_read_data(9u + axis));
    }
    for (index = 0; index < counts[1]; ++index) scene_render_missing(0x80011DA0u, context, buckets[1][index], mode);
    for (index = 0; index < counts[0]; ++index) scene_render_missing(0x80010154u, context, buckets[0][index], mode);
    if (!mode)
    {
        uint32 owner = r_u32(0x800B630Cu) == 1u && r_u16(context + 32u) >= 387u ? 0x80010154u : 0x8002723Cu;
        for (index = 0; index < counts[2]; ++index) scene_render_missing(owner, context, buckets[2][index], 0);
    }
    packet_cursor = r_u32(context + 952u);
    angle = r_s8(0x8008FBE4u);
    w_u32(0x800BFE4Cu, packet_cursor);
    if (angle)
    {
        gtmain_text_font(0);
        uint32 words[5] = {counts[0], counts[1], counts[2], total, all_flags};
        GTMainFormatArguments arguments = {words, 0, 5, 0};
        (void)gtmain_text_format(r_u32(0x800B6368u), 16, mode ? 16 : 8, 0x8009BB9Cu, &arguments);
    }
    return all_flags;
}



/* GTMAIN.EXE:800267E4 */
uint32 gtmain_model_radial_draw(uint32 context, uint32 records, uint32 count)
{
    uint32 packet, axis, item;
    PsxGteSnapshot initial;
    if (!count) return records;
    for (axis = 188u; axis <= 204u; axis += 4u) w_u32(context + axis, 0);
    psx_gte_snapshot(&initial);
    w_u32(context + 208u, (sint16)initial.h);
    w_u32(context + 212u, initial.ofx); w_u32(context + 216u, initial.ofy);
    for (axis = 0; axis < 3u; ++axis) w_u32(context + 220u + axis * 4u, initial.translation[axis]);
    for (axis = 0; axis < 4u; ++axis)
        w_u32(context + 232u + axis * 4u, xport_load_le32((const uint8 *)&initial.rotation + axis * 4u));
    w_u32(context + 248u, (sint32)initial.rotation.m[2][2]);
    scene_ctc(19u, r_u32(context + 56u)); scene_ctc(20u, r_u32(context + 60u));
    scene_ctc(27u, 2048u); scene_ctc(28u, 0);
    packet = r_u32(context + 952u);
    do
    {
        uint32 first = r_u32(records), second = r_u32(records + 4u), color, links, flags;
        uint32 shift = r_u16(context + 42u), first_packet, second_packet, old_link, ordering;
        sint32 depth, ir0, components[3], mac_x, mac_z, norm, thickness, scale, radius, diagonal, half;
        sint32 pixel_x, pixel_y, half_pixel, screen_x, screen_y, ratio;
        SVECTOR operand;
        VECTOR product;
        uint32 sxy[3], shaded;
        xport_gte_write_data(0u, first); xport_gte_write_data(1u, second);
        --count;
        xport_gte_execute(0x180001u);
        color = r_u32(records + 16u);
        xport_gte_write_data(2u, r_u32(records + 8u)); xport_gte_write_data(3u, r_u32(records + 12u));
        flags = scene_gte_flags(); w_u32(context + 184u, xport_gte_read_data(14u));
        ir0 = (sint32)xport_gte_read_data(8u);
        for (axis = 0; axis < 3u; ++axis) components[axis] = (sint32)xport_gte_read_data(9u + axis);
        scene_ctc(16u, first); scene_ctc(17u, second);
        links = r_u32(context + 948u);
        xport_gte_execute(0x4CE012u);
        depth = components[2] >> (r_u16(context + 40u) & 31u);
        if ((sint32)flags < 0) goto radial_next;
        for (axis = 0; axis < 3u; ++axis)
            if (components[axis] < 0) components[axis] = (sint32)(0u - (uint32)components[axis]);
        if (components[0] < components[1]) { sint32 swap = components[0]; components[0] = components[1]; components[1] = swap; }
        if (components[0] < components[2]) { sint32 swap = components[0]; components[0] = components[2]; components[2] = swap; }
        if (components[1] < components[2]) { sint32 swap = components[1]; components[1] = components[2]; components[2] = swap; }
        norm = (sint32)((uint32)components[0] + (uint32)(components[1] >> 1) + (uint32)(components[2] >> 2));
        mac_x = (sint32)xport_gte_read_data(25u); mac_z = (sint32)xport_gte_read_data(27u);
        xport_gte_write_data(8u, 2048u); xport_gte_write_data(6u, color | 0x33000000u);
        norm = scene_add_checked(norm, norm >> 2, 0x800269B0u);
        xport_gte_execute(0x780010u);
        thickness = (sint32)((uint32)(mac_z >> (shift & 31u)) - (uint32)mac_x);
        thickness = (sint32)((uint32)thickness + (uint32)(thickness >> 2));
        scale = ir0 >> (shift & 31u);
        if (thickness <= 0) goto radial_next;
        second >>= 16;
        second = (uint32)((sint32)second >> (shift & 31u));
        while (norm > 0x7FFF) { norm >>= 1; thickness >>= 1; }
        first_packet = packet + 8u; second_packet = packet + 224u;
        shaded = xport_gte_read_data(22u);
        for (item = 0; item < 6u; ++item) w_u32(second_packet + 4u + item * 28u, shaded);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 8u + item * 36u, xport_gte_read_data(14u));
        /* DIV consumes the wrapped 32-bit numerator */
        ratio = (sint32)((uint32)thickness << 12);
        if (!norm) ratio = ratio < 0 ? 1 : -1;
        else if (ratio == (-2147483647 - 1) && norm == -1) ratio = (-2147483647 - 1);
        else ratio /= norm;
        xport_gte_write_data(8u, scale); xport_gte_write_data(9u, second);
        operand.vx = (sint16)xport_gte_read_data(9u);
        operand.vy = (sint16)xport_gte_read_data(10u);
        operand.vz = (sint16)xport_gte_read_data(11u);
        operand.pad = 0;
        gte_gpf12(&operand, (sint32)xport_gte_read_data(8u), &product);
        w_u32(packet, (first_packet & 0xFFFFFFu) | 0x01000000u); w_u32(packet + 4u, 0xE1000220u);
        for (item = 0; item < 6u; ++item)
            w_u32(first_packet + item * 36u, ((first_packet & 0xFFFFFFu) | 0x08000000u) + (item + 1u) * 36u);
        for (item = 0; item < 6u; ++item)
            w_u32(second_packet + item * 28u, ((second_packet & 0xFFFFFFu) | 0x06000000u) + (item + 1u) * 28u);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 4u + item * 36u, color | 0x3B000000u);
        radius = (sint32)xport_gte_read_data(9u);
        half = (sint32)((uint32)ratio + 4096u) >> 1;
        radius = (sint32)((uint32)radius * (uint32)half) >> 12;
        diagonal = (sint32)((uint32)radius * 3547u);
        if (depth >= 4096) depth = 4095;
        ordering = links + ((uint32)depth << 2);
        half = (sint16)r_u16(0x80097488u + ((uint32)ratio << 1));
        w_u16(context + 190u, 0u - (uint32)half); w_u16(context + 188u, ratio);
        w_u16(context + 194u, half); w_u16(context + 196u, ratio);
        screen_x = (sint16)r_u16(context + 184u); screen_y = (sint16)r_u16(context + 186u);
        diagonal >>= 12; half = radius >> 1;
        w_u16(context + 254u, radius); pixel_x = radius >> 7;
        w_u16(context + 266u, 0u - (uint32)radius);
        w_u16(context + 252u, 0); w_u16(context + 256u, diagonal); w_u16(context + 258u, half);
        w_u16(context + 260u, diagonal); w_u16(context + 262u, 0u - (uint32)half);
        w_u16(context + 264u, 0); w_u16(context + 268u, 0u - (uint32)diagonal);
        w_u16(context + 270u, 0u - (uint32)half); w_u16(context + 272u, 0u - (uint32)diagonal);
        w_u16(context + 274u, half);
        if (!pixel_x) pixel_x = 1;
        pixel_y = diagonal >> 7; half_pixel = pixel_x >> 1;
        if (!pixel_y) pixel_y = 1;
        w_u16(context + 280u, half_pixel); w_u16(context + 288u, half_pixel);
        w_u16(context + 290u, 0u - (uint32)pixel_y); w_u16(context + 294u, 0u - (uint32)pixel_y);
        w_u16(context + 276u, 0u - (uint32)half_pixel); w_u16(context + 278u, pixel_y);
        w_u16(context + 282u, pixel_y); w_u16(context + 284u, pixel_x); w_u16(context + 286u, 0);
        w_u16(context + 292u, 0u - (uint32)half_pixel); w_u16(context + 296u, 0u - (uint32)pixel_x);
        w_u16(context + 298u, 0);
        xport_gte_write_data(8u, 1638u); xport_gte_execute(0x780010u);
        scene_ctc(5u, 0); scene_ctc(6u, 0); scene_ctc(7u, r_u32(context + 208u));
        scene_ctc(26u, r_u32(context + 208u));
        scene_ctc(24u, (uint32)screen_x << 16); scene_ctc(25u, (uint32)screen_y << 16);
        scene_ctc(0u, r_u32(context + 188u)); scene_ctc(1u, r_u32(context + 192u)); scene_ctc(2u, r_u32(context + 196u));
        xport_gte_write_data(0u, r_u32(context + 252u)); xport_gte_write_data(2u, r_u32(context + 256u));
        xport_gte_write_data(4u, r_u32(context + 260u));
        xport_gte_write_data(1u, 0); xport_gte_write_data(3u, 0); xport_gte_write_data(5u, 0);
        shaded = xport_gte_read_data(22u); xport_gte_execute(0x280030u);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 12u + item * 36u, shaded);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 20u + item * 36u, shaded);
        for (axis = 0; axis < 4u; ++axis)
        {
            static const uint16 destination1[4][6] = {{32,68,104,0,0,0},{140,176,212,0,0,0},{196,16,52,24,60,96},{88,124,160,132,168,204}};
            static const uint16 destination2[4][6] = {{16,44,72,164,24,52},{100,128,156,80,108,136},{148,8,36,0,0,0},{64,92,120,0,0,0}};
            uint32 limit1 = axis < 2u ? 3u : 6u, limit2 = axis < 2u ? 6u : 3u;
            for (item = 0; item < 3u; ++item) sxy[item] = xport_gte_read_data(12u + item);
            if (axis < 3u)
            {
                for (item = 0; item < 3u; ++item)
                    xport_gte_write_data(item * 2u, r_u32(context + 264u + axis * 12u + item * 4u));
                xport_gte_execute(0x280030u);
            }
            for (item = 0; item < limit1; ++item) w_u32(first_packet + destination1[axis][item], sxy[item % 3u]);
            for (item = 0; item < limit2; ++item) w_u32(second_packet + destination2[axis][item], sxy[item % 3u]);
            if (axis == 0u) for (item = 0; item < 6u; ++item) w_u32(first_packet + 28u + item * 36u, 0);
            if (axis == 1u) for (item = 0; item < 6u; ++item) w_u32(second_packet + 12u + item * 28u, 0);
            if (axis == 2u) for (item = 0; item < 6u; ++item) w_u32(second_packet + 20u + item * 28u, 0);
        }
        old_link = (uint32)r_u8(ordering) | (uint32)r_u8(ordering + 1u) << 8 | (uint32)r_u8(ordering + 2u) << 16;
        w_u8(ordering, packet); w_u8(ordering + 1u, packet >> 8); w_u8(ordering + 2u, packet >> 16);
        w_u8(second_packet + 140u, old_link); w_u8(second_packet + 141u, old_link >> 8); w_u8(second_packet + 142u, old_link >> 16);
        packet = second_packet + 168u;
        scene_ctc(27u, 2048u); scene_ctc(26u, r_u32(context + 208u));
        scene_ctc(24u, r_u32(context + 212u)); scene_ctc(25u, r_u32(context + 216u));
        for (axis = 0; axis < 3u; ++axis) scene_ctc(5u + axis, r_u32(context + 220u + axis * 4u));
        for (axis = 0; axis < 3u; ++axis) scene_ctc(axis, r_u32(context + 232u + axis * 4u));
radial_next:
        records += 20u;
    } while (count);
    w_u32(context + 952u, packet);
    return records;
}

uint32 gtmain_model_sprites_draw(uint32 context, uint32 records, uint32 count)
{
    uint32 work = context + 184u, packet, links, shades, shift;
    if (!count) return records;
    scene_ctc(8u, r_u16(context + 868u));
    scene_ctc(9u, (uint32)r_u16(context + 866u) << 16);
    scene_ctc(10u, 0u); scene_ctc(11u, 0u); scene_ctc(12u, 0u);
    packet = r_u32(context + 928u); links = r_u32(context + 924u);
    shades = r_u32(context + 920u); shift = r_u16(context + 40u);
    do
    {
        uint32 size = r_u32(records + 8u), packed_z, flags, first_sxy, first_depth, axis;
        uint32 left, upper, front;
        sint32 x, y, dx, dz, right, back;
        xport_gte_write_data(9u, size & 65535u);
        w_u32(work, r_u32(records));
        xport_gte_execute(0x4BE012u);
        packed_z = r_u32(records + 4u);
        x = (sint16)r_u16(work); y = (sint16)r_u16(work + 2u);
        dx = (sint32)xport_gte_read_data(9u); dz = (sint32)xport_gte_read_data(10u);
        left = (uint32)x - (uint32)dx;
        upper = (uint32)y + (size >> 16); front = packed_z + (uint32)dz;
        w_u16(work, left); w_u16(work + 2u, upper); w_u16(work + 4u, front);
        xport_gte_write_data(0u, r_u32(work)); xport_gte_write_data(1u, r_u32(work + 4u));
        right = scene_add_checked(x, dx, 0x8002EA28u);
        back = scene_add_checked((sint32)packed_z, -(sint64)dz, 0x8002EA2Cu);
        xport_gte_execute(0x180001u);
        xport_gte_write_data(6u, r_u32(records + 12u));
        w_u16(work + 8u, right); w_u16(work + 10u, upper); w_u16(work + 12u, back);
        w_u16(work + 16u, left); w_u16(work + 18u, y); w_u16(work + 20u, front);
        w_u16(work + 24u, right); w_u16(work + 26u, y); w_u16(work + 28u, back);
        flags = scene_gte_flags();
        first_sxy = xport_gte_read_data(14u); w_u32(packet + 8u, first_sxy);
        first_depth = xport_gte_read_data(19u);
        for (axis = 0u; axis < 6u; ++axis)
            xport_gte_write_data(axis, r_u32(work + 8u + 4u * axis));
        xport_gte_execute(0x280030u);
        if ((sint32)flags >= 0)
        {
            uint32 a = r_u32(records + 16u), b = r_u32(records + 20u), c = r_u32(records + 24u);
            uint32 depth_sum, sxy[3], ordering, shade, old_link;
            sint32 depth;
            w_u32(packet + 12u, a); w_u32(packet + 20u, b); w_u32(packet + 28u, c);
            w_u8(packet + 36u, c >> 16); w_u8(packet + 37u, c >> 24);
            flags = scene_gte_flags();
            depth_sum = first_depth + xport_gte_read_data(17u);
            depth_sum += xport_gte_read_data(18u); depth_sum += xport_gte_read_data(19u);
            if ((sint32)flags >= 0)
            {
                depth = (sint32)((uint32)((sint32)depth_sum >> 2) << (shift & 31u)) >> 13;
                if (depth >= 4096) depth = 4095;
                shade = ((packed_z >> 16) & 1u) ? 0u : r_u16(shades + ((uint32)depth << 1));
                ordering = links + ((uint32)depth << 2);
                for (axis = 0u; axis < 3u; ++axis)
                {
                    sxy[axis] = xport_gte_read_data(12u + axis);
                    w_u32(packet + 16u + 8u * axis, sxy[axis]);
                }
                xport_gte_write_data(8u, shade); w_u32(packet, 0x09000000u);
                xport_gte_execute(0x780010u);
                if (!(first_sxy & sxy[0] & sxy[1] & sxy[2] & 0x80008000u))
                {
                    old_link = (uint32)r_u8(ordering) | ((uint32)r_u8(ordering + 1u) << 8) | ((uint32)r_u8(ordering + 2u) << 16);
                    w_u8(ordering, packet); w_u8(ordering + 1u, packet >> 8); w_u8(ordering + 2u, packet >> 16);
                    w_u8(packet, old_link); w_u8(packet + 1u, old_link >> 8); w_u8(packet + 2u, old_link >> 16);
                    w_u32(packet + 4u, xport_gte_read_data(22u));
                    packet += 40u;
                }
            }
        }
        records += 28u;
    } while (--count);
    w_u32(context + 928u, packet);
    return records;
}

uint32 gtmain_model_radial_effect_draw(uint32 context, uint32 records, uint32 count)
{
    uint32 packet, axis, item;
    PsxGteSnapshot initial;
    if (!count) return records;
    for (axis = 188u; axis <= 204u; axis += 4u) w_u32(context + axis, 0);
    psx_gte_snapshot(&initial);
    w_u32(context + 208u, (sint16)initial.h);
    w_u32(context + 212u, initial.ofx); w_u32(context + 216u, initial.ofy);
    for (axis = 0; axis < 3u; ++axis) w_u32(context + 220u + axis * 4u, initial.translation[axis]);
    for (axis = 0; axis < 4u; ++axis)
        w_u32(context + 232u + axis * 4u, xport_load_le32((const uint8 *)&initial.rotation + axis * 4u));
    w_u32(context + 248u, (sint32)initial.rotation.m[2][2]);
    scene_ctc(19u, r_u32(context + 56u)); scene_ctc(20u, r_u32(context + 60u));
    scene_ctc(27u, 2048u); scene_ctc(28u, 0);
    packet = r_u32(context + 928u);
    do
    {
        uint32 first = r_u32(records), second = r_u32(records + 4u), color, links, flags;
        uint32 shift = r_u16(context + 42u), first_packet, second_packet, old_link, ordering;
        sint32 depth, ir0, components[3], mac_x, mac_z, norm, thickness, scale, radius, diagonal, half;
        sint32 pixel_x, pixel_y, half_pixel, screen_x, screen_y, ratio;
        SVECTOR operand;
        VECTOR product;
        uint32 sxy[3], shaded;
        xport_gte_write_data(0u, first); xport_gte_write_data(1u, second);
        count = (uint32)scene_add_checked((sint32)count, -1, 0x8002EC84u);
        xport_gte_execute(0x180001u);
        color = r_u32(records + 16u);
        xport_gte_write_data(2u, r_u32(records + 8u)); xport_gte_write_data(3u, r_u32(records + 12u));
        flags = scene_gte_flags(); w_u32(context + 184u, xport_gte_read_data(14u));
        ir0 = (sint32)xport_gte_read_data(8u);
        for (axis = 0; axis < 3u; ++axis) components[axis] = (sint32)xport_gte_read_data(9u + axis);
        scene_ctc(16u, first); scene_ctc(17u, second);
        links = r_u32(context + 924u);
        xport_gte_execute(0x4CE012u);
        depth = (sint32)((uint32)components[2] << (r_u16(context + 40u) & 31u)) >> 13;
        if ((sint32)flags < 0) goto effect_next;
        for (axis = 0; axis < 3u; ++axis)
            if (components[axis] < 0) components[axis] = (sint32)(0u - (uint32)components[axis]);
        if (components[0] < components[1]) { sint32 swap = components[0]; components[0] = components[1]; components[1] = swap; }
        if (components[0] < components[2]) { sint32 swap = components[0]; components[0] = components[2]; components[2] = swap; }
        if (components[1] < components[2]) { sint32 swap = components[1]; components[1] = components[2]; components[2] = swap; }
        norm = (sint32)((uint32)components[0] + (uint32)(components[1] >> 1) + (uint32)(components[2] >> 2));
        mac_x = (sint32)xport_gte_read_data(25u); mac_z = (sint32)xport_gte_read_data(27u);
        xport_gte_write_data(8u, 2048u); xport_gte_write_data(6u, color | 0x33000000u);
        norm = scene_add_checked(norm, norm >> 2, 0x8002ED74u);
        xport_gte_execute(0x780010u);
        thickness = (sint32)((uint32)(mac_z >> (shift & 31u)) - (uint32)mac_x);
        thickness = (sint32)((uint32)thickness + (uint32)(thickness >> 2));
        scale = (sint32)((uint32)ir0 << (r_u16(context + 864u) & 31u));
        if (thickness <= 0) goto effect_next;
        second >>= 16;
        second <<= r_u16(context + 864u) & 31u;
        while (norm > 0x7FFF) { norm >>= 1; thickness >>= 1; }
        first_packet = packet + 8u; second_packet = packet + 224u;
        shaded = xport_gte_read_data(22u);
        for (item = 0; item < 6u; ++item) w_u32(second_packet + 4u + item * 28u, shaded);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 8u + item * 36u, xport_gte_read_data(14u));
        /* DIV consumes the wrapped 32-bit numerator */
        ratio = (sint32)((uint32)thickness << 12);
        if (!norm) ratio = ratio < 0 ? 1 : -1;
        else if (ratio == (-2147483647 - 1) && norm == -1) ratio = (-2147483647 - 1);
        else ratio /= norm;
        xport_gte_write_data(8u, scale); xport_gte_write_data(9u, second);
        operand.vx = (sint16)xport_gte_read_data(9u);
        operand.vy = (sint16)xport_gte_read_data(10u);
        operand.vz = (sint16)xport_gte_read_data(11u);
        operand.pad = 0;
        gte_gpf12(&operand, (sint32)xport_gte_read_data(8u), &product);
        w_u32(packet, (first_packet & 0xFFFFFFu) | 0x01000000u); w_u32(packet + 4u, 0xE1000220u);
        for (item = 0; item < 6u; ++item)
            w_u32(first_packet + item * 36u, ((first_packet & 0xFFFFFFu) | 0x08000000u) + (item + 1u) * 36u);
        for (item = 0; item < 6u; ++item)
            w_u32(second_packet + item * 28u, ((second_packet & 0xFFFFFFu) | 0x06000000u) + (item + 1u) * 28u);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 4u + item * 36u, color | 0x3B000000u);
        radius = (sint32)xport_gte_read_data(9u);
        half = (sint32)((uint32)ratio + 4096u) >> 1;
        radius = (sint32)((uint32)radius * (uint32)half) >> 12;
        diagonal = (sint32)((uint32)radius * 3547u);
        if (depth >= 4096) depth = 4095;
        ordering = links + ((uint32)depth << 2);
        half = (sint16)r_u16(0x80097488u + ((uint32)ratio << 1));
        w_u16(context + 190u, 0u - (uint32)half); w_u16(context + 188u, ratio);
        w_u16(context + 194u, half); w_u16(context + 196u, ratio);
        screen_x = (sint16)r_u16(context + 184u); screen_y = (sint16)r_u16(context + 186u);
        diagonal >>= 12; half = radius >> 1;
        w_u16(context + 254u, radius); pixel_x = radius >> 7;
        w_u16(context + 266u, 0u - (uint32)radius);
        w_u16(context + 252u, 0); w_u16(context + 256u, diagonal); w_u16(context + 258u, half);
        w_u16(context + 260u, diagonal); w_u16(context + 262u, 0u - (uint32)half);
        w_u16(context + 264u, 0); w_u16(context + 268u, 0u - (uint32)diagonal);
        w_u16(context + 270u, 0u - (uint32)half); w_u16(context + 272u, 0u - (uint32)diagonal);
        w_u16(context + 274u, half);
        if (!pixel_x) pixel_x = 1;
        pixel_y = diagonal >> 7; half_pixel = pixel_x >> 1;
        if (!pixel_y) pixel_y = 1;
        w_u16(context + 280u, half_pixel); w_u16(context + 288u, half_pixel);
        w_u16(context + 290u, 0u - (uint32)pixel_y); w_u16(context + 294u, 0u - (uint32)pixel_y);
        w_u16(context + 276u, 0u - (uint32)half_pixel); w_u16(context + 278u, pixel_y);
        w_u16(context + 282u, pixel_y); w_u16(context + 284u, pixel_x); w_u16(context + 286u, 0);
        w_u16(context + 292u, 0u - (uint32)half_pixel); w_u16(context + 296u, 0u - (uint32)pixel_x);
        w_u16(context + 298u, 0);
        xport_gte_write_data(8u, 1638u); xport_gte_execute(0x780010u);
        scene_ctc(5u, 0); scene_ctc(6u, 0); scene_ctc(7u, r_u32(context + 208u));
        scene_ctc(26u, r_u32(context + 208u));
        scene_ctc(24u, (uint32)screen_x << 16); scene_ctc(25u, (uint32)screen_y << 16);
        scene_ctc(0u, r_u32(context + 188u)); scene_ctc(1u, r_u32(context + 192u)); scene_ctc(2u, r_u32(context + 196u));
        xport_gte_write_data(0u, r_u32(context + 252u)); xport_gte_write_data(2u, r_u32(context + 256u));
        xport_gte_write_data(4u, r_u32(context + 260u));
        xport_gte_write_data(1u, 0); xport_gte_write_data(3u, 0); xport_gte_write_data(5u, 0);
        shaded = xport_gte_read_data(22u); xport_gte_execute(0x280030u);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 12u + item * 36u, shaded);
        for (item = 0; item < 6u; ++item) w_u32(first_packet + 20u + item * 36u, shaded);
        for (axis = 0; axis < 4u; ++axis)
        {
            static const uint16 destination1[4][6] = {{32,68,104,0,0,0},{140,176,212,0,0,0},{196,16,52,24,60,96},{88,124,160,132,168,204}};
            static const uint16 destination2[4][6] = {{16,44,72,164,24,52},{100,128,156,80,108,136},{148,8,36,0,0,0},{64,92,120,0,0,0}};
            uint32 limit1 = axis < 2u ? 3u : 6u, limit2 = axis < 2u ? 6u : 3u;
            for (item = 0; item < 3u; ++item) sxy[item] = xport_gte_read_data(12u + item);
            if (axis < 3u)
            {
                for (item = 0; item < 3u; ++item)
                    xport_gte_write_data(item * 2u, r_u32(context + 264u + axis * 12u + item * 4u));
                xport_gte_execute(0x280030u);
            }
            for (item = 0; item < limit1; ++item) w_u32(first_packet + destination1[axis][item], sxy[item % 3u]);
            for (item = 0; item < limit2; ++item) w_u32(second_packet + destination2[axis][item], sxy[item % 3u]);
            if (axis == 0u) for (item = 0; item < 6u; ++item) w_u32(first_packet + 28u + item * 36u, 0);
            if (axis == 1u) for (item = 0; item < 6u; ++item) w_u32(second_packet + 12u + item * 28u, 0);
            if (axis == 2u) for (item = 0; item < 6u; ++item) w_u32(second_packet + 20u + item * 28u, 0);
        }
        old_link = (uint32)r_u8(ordering) | (uint32)r_u8(ordering + 1u) << 8 | (uint32)r_u8(ordering + 2u) << 16;
        w_u8(ordering, packet); w_u8(ordering + 1u, packet >> 8); w_u8(ordering + 2u, packet >> 16);
        w_u8(second_packet + 140u, old_link); w_u8(second_packet + 141u, old_link >> 8); w_u8(second_packet + 142u, old_link >> 16);
        packet = second_packet + 168u;
        scene_ctc(27u, 2048u); scene_ctc(26u, r_u32(context + 208u));
        scene_ctc(24u, r_u32(context + 212u)); scene_ctc(25u, r_u32(context + 216u));
        for (axis = 0; axis < 3u; ++axis) scene_ctc(5u + axis, r_u32(context + 220u + axis * 4u));
        for (axis = 0; axis < 3u; ++axis) scene_ctc(axis, r_u32(context + 232u + axis * 4u));
effect_next:
        records += 20u;
    } while (count);
    w_u32(context + 928u, packet);
    return records;
}
