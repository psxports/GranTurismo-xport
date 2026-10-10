#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 spline_fixed64(sint64 value)
{
    return (sint32)(uint32)(value / 4096);
}

static sint32 spline_fixed32(uint32 a, uint32 b)
{
    return (sint32)(a * b) / 4096;
}

static sint32 spline_weight(uint32 numerator, uint32 denominator)
{
    return (sint32)(uint32)gtmain_signed_divide64((sint64)(sint32)numerator * 4096,
        (sint32)denominator, 0x800542C4u);
}

/* GTMAIN.EXE 80054164 */
void gtmain_race_ai_spline_position(sint32 mode, sint32 segment, sint32 progress, sint32 *first, sint32 *second)
{
    uint32 table = r_u32(r_u32(0x800BF51Cu) + 8u + 4u * (uint32)mode), rows = table + 4u;
    sint32 length = r_s32(r_u32(0x800BC1A8u)), count = r_s32(table), previous = 0, next = 0, weight = 0;
    sint32 start, end, step, lower, upper;
    uint32 previous_row, next_row;
    if (progress < 0) progress = (sint32)((uint32)progress + (uint32)length);
    else if (progress >= length) progress = (sint32)((uint32)progress - (uint32)length);
    start = r_s32(rows + 20u);
    if (progress < start) {
        previous = (sint32)((uint32)count - 1u); end = r_s32(rows + 40u * (uint32)previous + 20u);
        weight = spline_weight((uint32)progress + (uint32)length - (uint32)end,
            (uint32)length + (uint32)start - (uint32)end);
    } else if (progress >= r_s32(rows + 40u * (uint32)count - 20u)) {
        previous = (sint32)((uint32)count - 1u); end = r_s32(rows + 40u * (uint32)previous + 20u);
        start = r_s32(rows + 20u);
        weight = spline_weight((uint32)progress - (uint32)end, (uint32)length + (uint32)start - (uint32)end);
    } else {
        for (step = 0; step <= count; step = (sint32)((uint32)step + 1u)) {
            segment = (sint32)((step & 1) ? (uint32)segment + (uint32)step : (uint32)segment - (uint32)step);
            next = segment;
            if (next < 0) next = (sint32)((uint32)next + (uint32)count);
            else if (next >= count) next = (sint32)((uint32)next - (uint32)count);
            if (!next) continue;
            previous = (sint32)((uint32)next - 1u);
            if (previous < 0) previous = (sint32)((uint32)previous + (uint32)count);
            lower = r_s32(rows + 40u * (uint32)previous + 20u);
            if (progress < lower) continue;
            upper = r_s32(rows + 40u * (uint32)next + 20u);
            if (progress < upper) {
                weight = spline_weight((uint32)progress - (uint32)lower, (uint32)upper - (uint32)lower); break;
            }
        }
    }
    previous_row = rows + 40u * (uint32)previous; next_row = rows + 40u * (uint32)next;
    if (!r_u32(previous_row)) {
        uint32 x = r_u32(previous_row + 4u), next_x = r_u32(next_row + 4u);
        sint64 x_product = (sint64)weight * (sint32)(next_x - x);
        uint32 z = r_u32(previous_row + 8u), next_z = r_u32(next_row + 8u);
        sint64 z_product = (sint64)weight * (sint32)(next_z - z);
        *first = (sint32)(x + (uint32)spline_fixed64(x_product));
        *second = (sint32)(z + (uint32)spline_fixed64(z_product));
    } else {
        sint32 radius = r_s32(previous_row + 24u), angle = (sint32)(r_u32(previous_row + 36u) & 4095u);
        sint64 cosine_product = (sint64)r_s16(0x80094A3Cu + 2u * (uint32)angle) * radius;
        sint32 sine = -r_s16(0x8009423Cu + 2u * (uint32)angle);
        uint32 next_x = r_u32(next_row + 4u), next_z = r_u32(next_row + 8u);
        sint32 fx = spline_fixed64(cosine_product), fz = spline_fixed64((sint64)sine * radius);
        uint32 center_x = r_u32(previous_row + 4u) - (uint32)fx, center_z = r_u32(previous_row + 8u) + (uint32)fz;
        /* Use the canonical angle service with the existing shared LZ binding */
        sint32 first_angle = gtmain_angle_atan_wide((sint32)(0u - (uint32)fx), (sint32)(0u - (uint32)fz));
        sint32 second_angle = gtmain_angle_atan_wide((sint32)(center_x - next_x), (sint32)(next_z - center_z));
        if (radius > 0) { if (second_angle < first_angle) second_angle = (sint32)((uint32)second_angle + 4096u); }
        else { radius = (sint32)(0u - (uint32)radius); if (first_angle < second_angle) first_angle = (sint32)((uint32)first_angle + 4096u); }
        angle = (sint32)((uint32)spline_fixed32(4096u - (uint32)weight, (uint32)first_angle)
            + (uint32)spline_fixed32((uint32)weight, (uint32)second_angle));
        sine = (sint16)r_u16(0x8009423Cu + 2u * ((uint32)angle & 4095u));
        *first = (sint32)(center_x - (uint32)spline_fixed64((sint64)sine * radius));
        sine = (sint16)r_u16(0x80094A3Cu + 2u * ((uint32)angle & 4095u));
        *second = (sint32)(center_z + (uint32)spline_fixed64((sint64)sine * radius));
    }
}

/* GTMAIN.EXE 80053CD8 */
void gtmain_race_ai_segment_speed(uint32 car)
{
    uint32 table = r_u32(r_u32(0x800BF51Cu) + 8u + 4u * r_u8(car + 1912u)), rows = table + 4u;
    uint32 old_speed = r_u32(car + 1920u), segment, sum, wheel, sqrt_value;
    sint32 count = r_s32(table), radius, strength, friction, coefficient, value, angle, speed, maximum;
    w_u8(car + 1914u, r_u8(car + 1915u)); segment = r_u8(car + 1913u); w_u32(car + 1916u, old_speed);
    w_u8(car + 1915u, (sint32)segment == count ? 0 : r_u8(rows + 40u * segment));
    if (!r_u8(car + 1915u)) { w_u32(car + 1920u, r_u32(car + 848u)); return; }
    if (r_u8(car + 1915u) != 1u) return;
    segment = r_u8(car + 1913u);
    while ((sint32)segment >= count) segment -= (uint32)count;
    radius = r_s32(rows + 40u * segment + 24u); strength = spline_fixed64((sint64)radius * 32);
    if (strength < 0) strength = (sint32)(0u - (uint32)strength);
    if (strength > 4096) strength = 4096;
    friction = (sint32)((uint32)spline_fixed64((sint64)strength * r_s32(car + 1216u)) + r_u32(car + 1212u));
    if (r_u32(0x80093E30u) && !(r_u8(car + 58u) & 16u)) {
        sum = 0; for (wheel = 0; wheel < 4u; ++wheel) sum += r_u16(car + 172u + 68u * wheel);
        friction = spline_fixed32((uint32)((sint32)(sum << 16) >> 18), (uint32)friction);
    }
    coefficient = spline_fixed32((uint32)r_s16(car + 1700u), (uint32)friction);
    value = spline_fixed64((sint64)radius * coefficient); angle = r_s32(rows + 40u * segment + 32u);
    if (radius > 0) {
        if (angle < 0) value = (sint32)(uint32)gtmain_signed_divide64((sint64)value * 4096,
            r_s16(0x8009423Cu + 2u * ((uint32)angle & 4095u)) + 4096, 0x80053FC0u);
    } else {
        value = (sint32)(0u - (uint32)value);
        if (angle > 0) value = (sint32)(uint32)gtmain_signed_divide64((sint64)value * 4096,
            4096 - r_s16(0x8009423Cu + 2u * ((uint32)angle & 4095u)), 0x80053FC0u);
    }
    /* Use the canonical square root with the existing shared LZ binding */
    sqrt_value = gtmain_fixed_sqrt((uint32)value, 6);
    speed = (sint32)((uint32)spline_fixed64((sint64)r_s16(r_u32(car + 1708u) + 2u) * (sint64)(uint64)sqrt_value)
        + r_u32(r_u32(car + 1708u) + 4u));
    w_u32(car + 1920u, (uint32)speed);
    if (r_u8(0x80093C62u) == 1u) w_u32(car + 1920u, (uint32)spline_fixed64((sint64)speed * r_s16(car + 1704u)));
    else if (r_u8(0x80093C62u) == 2u) {
        sint32 offset = r_s16(car + 1704u); w_u32(car + 1920u, (uint32)offset + r_u32(car + 1920u));
    }
    speed = r_s32(car + 1920u); maximum = r_s32(car + 848u);
    if (speed > maximum) w_u32(car + 1920u, (uint32)maximum);
    else if (r_s32(car + 1920u) < 22760) w_u32(car + 1920u, 22760);
}

/* GTMAIN.EXE 800548F0 */
void gtmain_race_ai_segment_bind(uint32 car)
{
    uint32 table = r_u32(r_u32(0x800BF51Cu) + 8u + 4u * r_u8(car + 1912u)), rows = table + 4u;
    sint32 progress = r_s32(car + 1804u), count = r_s32(table), index;
    w_u8(car + 1913u, (uint8)count);
    for (index = 0; index < count; ++index) {
        if (progress < r_s32(rows + 40u * (uint32)index + 20u)) { w_u8(car + 1913u, (uint8)index); break; }
    }
    if (!r_u8(car + 1913u)) w_u8(car + 1915u, 0);
    else {
        index = (sint32)((uint32)r_u8(car + 1913u) + (uint32)count - 1u);
        while (index >= count) index = (sint32)((uint32)index - (uint32)count);
        w_u8(car + 1915u, r_u8(rows + 40u * (uint32)index));
    }
    gtmain_race_ai_segment_speed(car);
}

/* GTMAIN.EXE 80053AF0 */
sint32 gtmain_race_ai_mode_select(uint32 mode)
{
    uint32 header, table, i;
    sint32 candidate;
    if (mode >= 7u) mode = 0;
    table = 0x80093CD0u + 24u * mode; header = r_u32(0x800BF51Cu);
    for (i = 0; i < 6u; ++i) {
        candidate = r_s32(table + 4u * i);
        if (r_u32(header + 8u + 4u * (uint32)candidate)) return candidate;
    }
    return -1;
}

/* GTMAIN.EXE 8002DB8C */
void gtmain_race_best_time_set(uint32 time)
{
    uint32 old = r_u32(0x800BDB4Cu);
    if (!old || time < old) { w_u32(0x800BDB4Cu, time); gtmain_time_string(0x801A8FECu, time); w_u8(0x800BDD01u, 1); }
}

/* GTMAIN.EXE 8002DCA0 */
void gtmain_race_best_lap_set(uint32 elapsed)
{
    uint32 old = r_u32(0x800BDB48u);
    if (!old || elapsed < old) { w_u32(0x800BDB48u, elapsed); gtmain_time_string(0x801A8FFCu, elapsed); w_u8(0x800BDD00u, 1); }
}
