#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified native draft from complete GTMAIN pseudocode and MIPS */
static sint32 route_divide(sint32 numerator, sint32 denominator, uint32 owner)
{
    if (!denominator) gt_draft_failure("GTMAIN AI route BREAK7", owner);
    if ((uint32)numerator == 0x80000000u && denominator == -1)
        gt_draft_failure("GTMAIN AI route BREAK6", owner);
    return numerator / denominator;
}

static uint32 product_shift12(sint32 first, sint32 second)
{
    return (uint32)((sint64)first * second / 4096);
}

static sint32 route_position(sint32 segment, uint32 x, uint32 z)
{
    uint8 position[12];
    xport_store_le32(position, x << 4); xport_store_le32(position + 4u, 0); xport_store_le32(position + 8u, z << 4);
    return gtmain_course_segment_find((uint32)segment, position);
}

/* Original: GTMAIN.EXE:sub_80053148. */ sint32 gtmain_race_ai_route_segment(uint32 records, uint32 index, sint32 segment)
{
    uint32 previous = records + (index - 1u) * 40u, current = records + index * 40u;
    if (!r_u32(previous)) {
        uint32 x = r_u32(previous + 4u), z = r_u32(previous + 8u);
        uint32 target_x = r_u32(current + 4u), target_z = r_u32(current + 8u);
        uint32 ax = x - target_x, az = z - target_z;
        if ((sint32)ax < 0) ax = 0u - ax;
        if ((sint32)az < 0) az = 0u - az;
        uint32 minimum = ax;
        if ((sint32)az < (sint32)minimum) minimum = az;
        uint32 estimate = ax + az - (uint32)((sint32)minimum >> 1);
        sint32 steps = (sint32)estimate / 81920;
        if (!steps) steps = 1;
        if (steps <= 0) return segment;
        sint32 dx = (sint32)(target_x - x), dz = (sint32)(target_z - z);
        uint32 sample = 1;
        do {
            sint32 fraction = route_divide((sint32)(sample << 12), steps, 0x8005323Cu);
            segment = route_position(segment, x + product_shift12(fraction, dx), z + product_shift12(fraction, dz));
            ++sample;
        } while ((sint32)sample <= steps);
        return segment;
    }
    uint32 angle = (r_u32(previous + 36u) & 4095u) * 2u;
    uint32 radius_word = r_u32(previous + 24u);
    sint32 cosine = (sint16)r_u16(0x80094A3Cu + angle);
    uint32 previous_z = r_u32(previous + 8u);
    sint32 sine = (sint16)r_u16(0x8009423Cu + angle);
    uint32 previous_x = r_u32(previous + 4u);
    uint32 target_x = r_u32(current + 4u), target_z = r_u32(current + 8u);
    uint32 dx = product_shift12(cosine, (sint32)radius_word);
    uint32 dz = product_shift12(-sine, (sint32)radius_word);
    uint32 center_x = previous_x - dx, center_z = previous_z + dz;
    sint32 first_angle = gtmain_angle_atan_wide((sint32)(center_x - previous_x), (sint32)(previous_z - center_z));
    sint32 last_angle = gtmain_angle_atan_wide((sint32)(center_x - target_x), (sint32)(target_z - center_z));
    uint32 span;
    if ((sint32)radius_word <= 0) {
        radius_word = 0u - radius_word;
        if (first_angle < last_angle) first_angle = (sint32)((uint32)first_angle + 4096u);
        span = (uint32)first_angle - (uint32)last_angle;
    } else {
        span = (uint32)last_angle - (uint32)first_angle;
        if (last_angle < first_angle) {
            last_angle = (sint32)((uint32)last_angle + 4096u);
            span = (uint32)last_angle - (uint32)first_angle;
        }
    }
    sint32 steps = (sint32)span / 170;
    if (!steps) steps = 1;
    if (steps <= 0) return segment;
    uint32 sample = 1;
    do {
        sint32 fraction = route_divide((sint32)(sample << 12), steps, 0x8005345Cu);
        sint32 first = (sint32)((4096u - (uint32)fraction) * (uint32)first_angle) / 4096;
        sint32 second = (sint32)((uint32)fraction * (uint32)last_angle) / 4096;
        uint32 table = (((uint32)first + (uint32)second) & 4095u) * 2u;
        sint32 sample_sine = (sint16)r_u16(0x8009423Cu + table);
        uint32 offset_x = product_shift12(sample_sine, (sint32)radius_word);
        sint32 sample_cosine = (sint16)r_u16(0x80094A3Cu + table);
        uint32 offset_z = product_shift12(sample_cosine, (sint32)radius_word);
        segment = route_position(segment, center_x - offset_x, center_z + offset_z);
        ++sample;
    } while ((sint32)sample <= steps);
    return segment;
}

/* Original: GTMAIN.EXE:sub_80053620. */ uint32 gtmain_race_ai_route_ground(sint32 *height, sint32 *pitch, sint32 *roll, uint32 *flags,
    sint32 x, sint32 z, sint32 angle, uint32 segment)
{
    uint32 table = ((uint32)angle & 4095u) * 2u;
    sint32 sine = (sint32)((uint32)(sint32)(sint16)r_u16(0x8009423Cu + table) << 11) / 4096;
    sint32 cosine = (sint32)((uint32)(sint32)(sint16)r_u16(0x80094A3Cu + table) << 11) / 4096;
    uint8 query[24];
    xport_store_le32(query + 8u, (uint32)x << 4); xport_store_le32(query + 12u, 6553600);
    uint32 mutable_segment = segment;
    xport_store_le32(query + 16u, (uint32)z << 4); xport_store_le16(query + 4u, (uint16)mutable_segment);
    gtmain_collision_query(query);
    uint32 ground_word = xport_load_le32(query + 20u);
    sint32 ground = (sint32)ground_word >> 4;
    if (ground_word == 0x7fffffffu) { ground = 0x7fffffff; *flags = 0; }
    else *flags = query[2] & 2u;
    sint32 first = gtmain_collision_height_query((sint32)((uint32)x - (uint32)sine), (sint32)((uint32)z + (uint32)cosine), 409600, &mutable_segment);
    sint32 second = gtmain_collision_height_query((sint32)((uint32)x + (uint32)sine), (sint32)((uint32)z - (uint32)cosine), 409600, &mutable_segment);
    sint32 third = gtmain_collision_height_query((sint32)((uint32)x - (uint32)cosine), (sint32)((uint32)z - (uint32)sine), 409600, &mutable_segment);
    sint32 fourth = gtmain_collision_height_query((sint32)((uint32)x + (uint32)cosine), (sint32)((uint32)z + (uint32)sine), 409600, &mutable_segment);
    if (ground == 0x7fffffff || (first == 0x7fffffff && second == 0x7fffffff)
        || (third == 0x7fffffff && fourth == 0x7fffffff)) return 1;
    *height = ground;
    sint32 difference, distance;
    if (first == 0x7fffffff) { difference = (sint32)((uint32)ground - (uint32)second); distance = 2048; }
    else if (second == 0x7fffffff) { difference = (sint32)((uint32)first - (uint32)ground); distance = 2048; }
    else { difference = (sint32)((uint32)first - (uint32)second); distance = 4096; }
    *pitch = gtmain_angle_atan_wide(difference, distance);
    if (third == 0x7fffffff) { difference = (sint32)((uint32)ground - (uint32)fourth); distance = 2048; }
    else if (fourth == 0x7fffffff) { difference = (sint32)((uint32)third - (uint32)ground); distance = 2048; }
    else { difference = (sint32)((uint32)third - (uint32)fourth); distance = 4096; }
    *roll = gtmain_angle_atan_wide(difference, distance);
    return 0;
}

/* Original: GTMAIN.EXE:sub_80053898. */ sint32 gtmain_race_ai_routes_relocate(uint32 routes)
{
    sint32 course_length = (sint32)r_u32(r_u32(0x800BC1A8u));
    w_u32(0x800BF51Cu, routes);
    sint32 half_length = course_length / 2;
    for (uint32 table = 0; table < 7u; ++table) {
        uint32 base = r_u32(0x800BF51Cu), entry = base + table * 4u + 8u;
        uint32 records = r_u32(entry);
        if (!records) continue;
        if ((sint32)r_u32(base) > 0) w_u32(entry, records + base);
        base = r_u32(0x800BF51Cu);
        records = r_u32(base + table * 4u + 8u);
        sint32 count = (sint32)r_u32(records);
        records += 4u;
        sint32 segment = gtmain_course_start_segment();
        for (sint32 i = 0; i < count; ++i) {
            uint32 row = records + (uint32)i * 40u;
            uint8 position[12];
            xport_store_le32(position + 4u, 0);
            xport_store_le32(position, r_u32(row + 4u) << 4);
            xport_store_le32(position + 8u, r_u32(row + 8u) << 4);
            if (i) segment = gtmain_race_ai_route_segment(records, (uint32)i, segment);
            else segment = gtmain_course_segment_find((uint32)segment, position);
            sint32 progress = gtmain_course_progress((uint32)segment, position);
            uint32 x = r_u32(row + 4u);
            w_u32(row + 20u, (uint32)progress);
            uint32 z = r_u32(row + 8u), angle = r_u32(row + 36u);
            sint32 height, pitch, roll;
            uint32 flags;
            if (gtmain_race_ai_route_ground(&height, &pitch, &roll, &flags, (sint32)x, (sint32)z, (sint32)angle, (uint32)segment)) {
                w_u32(row + 12u, 0); w_u32(row + 28u, 0); w_u32(row + 32u, 0);
            } else {
                w_u32(row + 12u, (uint32)height); w_u32(row + 28u, (uint32)pitch); w_u32(row + 32u, (uint32)roll);
            }
            if (table == 4u) {
                sint32 current = (sint32)r_u32(row + 20u);
                if (current < half_length) w_u32(row + 20u, (uint32)course_length + (uint32)current);
            }
        }
    }
    uint32 base = r_u32(0x800BF51Cu), marker = r_u32(base);
    uint32 result = 0u - marker;
    if ((sint32)marker > 0) w_u32(base, result);
    return (sint32)result;
}

/* Original: GTMAIN.EXE:sub_80057460. */ uint32 gtmain_race_event_tables_reset(void)
{
    w_u32(0x80093D78u, 0);
    for (uint32 i = 0; i < 8u; ++i) {
        for (uint32 j = 0; j < 6u; ++j) w_u32(0x800B53C0u + i * 24u + j * 4u, 0x405f7dfdu);
        w_u32(0x800B5480u + i * 4u, 0xffffffffu);
        w_u32(0x800B54A0u + i * 4u, 0); w_u32(0x800B54C0u + i * 4u, 0);
    }
    return 0;
}

/* Original: GTMAIN.EXE:sub_8006200C. */ uint32 gtmain_race_speed_tables_reset(void)
{
    for (uint32 bank = 0; bank < 2u; ++bank)
        for (uint32 car = 0; car < 6u; ++car)
            for (uint32 wheel = 0; wheel < 5u; ++wheel)
                for (uint32 slot = 0; slot < 4u; ++slot) {
                    w_u8(0x800BFD10u + bank * 120u + car * 20u + wheel * 4u + slot, 0);
                    for (uint32 word = 0; word < 2u; ++word)
                        w_u32(0x800BF590u + bank * 960u + car * 160u + wheel * 32u + slot * 8u + word * 4u, 0);
                    w_u16(0x800BFE00u + car * 10u + wheel * 2u, 4096);
                    w_u8(0x800BF550u + car * 5u + wheel, 255);
                    w_u8(0x800BF570u + car * 5u + wheel, 255);
                }
    w_u32(0x800BF540u, 1);
    return 1;
}
