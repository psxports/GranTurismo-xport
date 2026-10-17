#include "game_first2_registry.h"

static sint32 suspension_add(sint32 a, sint32 b) { return (sint32)((uint32)a + (uint32)b); }
static sint32 suspension_sub(sint32 a, sint32 b) { return (sint32)((uint32)a - (uint32)b); }
static sint32 suspension_neg(sint32 a) { return (sint32)(0u - (uint32)a); }
static sint32 suspension_low(sint32 a, sint32 b) { return (sint32)((uint32)a * (uint32)b) / 4096; }
static sint32 suspension_wide(sint32 a, sint32 b) { return (sint32)(uint32)(((sint64)a * b) / 4096); }
static sint32 suspension_time(sint32 a) { return (sint32)((uint32)a * r_u32(0x1F800000u)) / 65536; }
static uint32 suspension_half(sint32 a) { return ((uint32)a + ((uint32)a >> 31)) >> 1; }

static uint32 suspension_route(uint32 key, sint32 *mode)
{
    /* TODO Bind the unrecorded route lookup with its actual native mode output */
    gt_draft_failure("Native suspension route lookup and mode output", 0x80052F60u);
}
static uint32 suspension_route_apply(uint32 car, uint32 route)
{
    /* TODO Bind the unrecorded pose override and its actual result */
    gt_draft_failure("Native suspension route pose result", 0x80060050u);
}

static void suspension_heights(uint32 car, uint32 scratch, sint32 fraction, sint32 thresholds[4])
{
    uint32 wheel;
    sint32 first, second, height = r_s32(car + 68u);
    for (wheel = 0; wheel < 4u; ++wheel) {
        uint32 contact = car + 132u + 68u * wheel;
        first = r_s16(contact + 12u);
        second = r_s32(contact + 16u);
        first = suspension_sub(suspension_add(height, first), second);
        second = r_s16(contact + 52u);
        first = suspension_sub(first, second);
        first = suspension_add(suspension_low(first, fraction), thresholds[wheel]);
        w_u32(scratch + 12u + 4u * wheel, (uint32)first);
    }
}
static uint32 suspension_mask(uint32 scratch, sint32 thresholds[4], uint32 mask)
{
    uint32 wheel, bit = 1;
    for (wheel = 0; wheel < 4u; ++wheel) {
        if (r_s32(scratch + 12u + 4u * wheel) < thresholds[wheel]) mask |= bit;
        bit <<= 1;
    }
    return mask;
}
static void suspension_basis(uint32 car)
{
    sint32 x = r_s16(car + 1786u), y = r_s16(car + 1788u), z = r_s16(car + 1790u);
    gtmain_race_basis_set(car + 72u, car + 80u, car + 88u, x, y, z);
}
static void suspension_axis(uint32 car, uint32 scratch, uint32 parameters, uint32 velocity_offset,
    uint32 angle_offset, uint32 front_coefficient, uint32 rear_coefficient, int lateral)
{
    sint32 first, second, coefficient, delta, old, next, angle;
    first = r_s32(scratch + 28u); second = r_s32(scratch + 32u);
    coefficient = r_s32(parameters + front_coefficient);
    first = suspension_low(lateral ? suspension_sub(first, second) : suspension_add(first, second), coefficient);
    second = r_s32(scratch + 36u); coefficient = r_s32(scratch + 40u);
    second = lateral ? suspension_sub(second, coefficient) : suspension_add(second, coefficient);
    coefficient = r_s32(parameters + rear_coefficient);
    second = suspension_low(second, coefficient);
    delta = suspension_time(lateral ? suspension_add(first, second) : suspension_sub(first, second));
    old = r_s32(car + velocity_offset); w_u32(car + velocity_offset, (uint32)suspension_add(delta, old));
    first = r_s32(scratch + 44u); second = r_s32(scratch + 48u);
    coefficient = r_s32(parameters + 100u);
    first = suspension_low(lateral ? suspension_sub(first, second) : suspension_add(first, second), coefficient);
    second = r_s32(scratch + 52u); coefficient = r_s32(scratch + 56u);
    second = lateral ? suspension_sub(second, coefficient) : suspension_add(second, coefficient);
    coefficient = r_s32(parameters + 104u);
    second = suspension_low(second, coefficient);
    delta = suspension_time(lateral ? suspension_add(first, second) : suspension_sub(first, second));
    old = r_s32(car + velocity_offset); next = suspension_add(delta, old);
    if ((old > 0 && next < 0) || (r_s32(car + velocity_offset) < 0 && next > 0))
        w_u32(car + velocity_offset, 0);
    else w_u32(car + velocity_offset, (uint32)next);
    delta = suspension_time(r_s32(car + velocity_offset));
    angle = (sint16)(r_u16(car + angle_offset) + (uint32)delta);
    w_u16(car + angle_offset, (uint16)angle);
    if (angle < -512) { w_u16(car + angle_offset, (uint16)-512); w_u32(car + velocity_offset, 0); }
    else if (r_s16(car + angle_offset) >= 513) { w_u16(car + angle_offset, 512); w_u32(car + velocity_offset, 0); }
}
static void suspension_velocity(uint32 car, sint32 impulse)
{
    uint32 axis;
    sint32 coefficient, delta, old;
    for (axis = 0; axis < 3u; ++axis) {
        coefficient = r_s16(car + 88u + 2u * axis);
        delta = suspension_low(coefficient, impulse);
        old = r_s32(car + 1712u + 4u * axis);
        w_u32(car + 1712u + 4u * axis, (uint32)suspension_add(delta, old));
    }
}
static sint32 suspension_impulse(uint32 scratch, uint32 offset, uint32 parameters)
{
    sint32 sum = r_s32(scratch + offset), next;
    next = r_s32(scratch + offset + 4u); sum = suspension_add(sum, next);
    next = r_s32(scratch + offset + 8u); sum = suspension_add(sum, next);
    next = r_s32(scratch + offset + 12u); sum = suspension_add(sum, next);
    next = r_s32(parameters + 48u);
    return suspension_time(suspension_wide(sum, next));
}
static uint32 suspension_ground_pair(uint32 car, uint32 parameters, uint32 angle_offset,
    uint32 coefficient_offset, sint32 first, sint32 second)
{
    sint32 a = (sint16)first, b = (sint16)second, selected = b < a ? b : a;
    sint32 delta, angle;
    if (!a && !b) return 0;
    if (a != b) {
        delta = suspension_sub(a, b);
        angle = gtmain_angle_atan_wide(delta, r_s16(parameters + coefficient_offset));
        w_u16(car + angle_offset, (uint16)(r_u16(car + angle_offset) + (uint32)angle));
    }
    return suspension_half(selected);
}

/* GTMAIN.EXE 8006008C */
void gtmain_race_suspension_step(uint32 cars, sint32 count)
{
    sint32 index;
    uint32 wheel;
    /* Keep complete ordered passes across all cars */
    for (index = 0; index < count; ++index) {
        uint32 outer = cars + 3380u * (uint32)index, car = outer + 40u;
        uint32 scratch = 0x1F800000u + 56u * (uint32)index;
        sint32 thresholds[4], numerator, divisor, fraction, first, second;
        if (r_s8(outer + 1729u) == 4) continue;
        numerator = (sint32)((uint32)r_s16(outer + 168u) << 12);
        divisor = r_s16(outer + 132u);
        if (!divisor) { gt_draft_exception(7u); return; }
        if (divisor == -1 && (uint32)numerator == 0x80000000u) { gt_draft_exception(6u); return; }
        fraction = numerator / divisor; w_u32(scratch + 4u, (uint32)fraction);
        first = r_s16(outer + 1600u); second = r_s16(outer + 740u);
        thresholds[0] = thresholds[1] = suspension_sub(second, first);
        first = r_s16(outer + 1602u); second = r_s16(outer + 816u);
        thresholds[2] = thresholds[3] = suspension_sub(second, first);
        suspension_heights(car, scratch, fraction, thresholds);
        first = r_s16(outer + 736u); thresholds[0] = thresholds[1] = first;
        first = r_s16(outer + 812u); thresholds[2] = thresholds[3] = first;
        w_u8(car + 1694u, suspension_mask(scratch, thresholds, 0));
    }
    for (index = 0; index < count; ++index) {
        uint32 outer = cars + 3380u * (uint32)index, car = outer + 40u;
        uint32 scratch = 0x1F800000u + 56u * (uint32)index, parameters[4];
        if (r_s8(outer + 1729u) == 4) continue;
        parameters[0] = parameters[1] = outer + 736u; parameters[2] = parameters[3] = outer + 812u;
        w_u32(scratch + 8u, 0);
        for (wheel = 0; wheel < 4u; ++wheel)
            gtmain_race_suspension_response(car, car + 132u + 68u * wheel,
                r_s32(scratch + 12u + 4u * wheel), parameters[wheel], (uint32 *)psx_addr(scratch + 8u, 4u));
    }
    for (index = 0; index < count; ++index) {
        uint32 outer = cars + 3380u * (uint32)index, car = outer + 40u;
        uint32 parameters = outer + 444u, scratch = 0x1F800000u + 56u * (uint32)index;
        sint32 difference, front, rear, square, first, second, pitch, roll, value;
        if (r_s8(outer + 1729u) == 4) continue;
        first = r_s16(outer + 176u); second = r_s16(outer + 244u); difference = suspension_sub(first, second);
        square = suspension_low(difference, difference); if (difference < 0) square = suspension_neg(square);
        front = suspension_wide(square, r_s32(outer + 796u));
        first = r_s16(car + 272u); second = r_s16(car + 340u); difference = suspension_sub(first, second);
        square = suspension_low(difference, difference); if (difference < 0) square = suspension_neg(square);
        rear = suspension_wide(square, r_s32(parameters + 428u));
        first = r_s32(car + 1928u); second = r_s16(parameters + 30u); roll = suspension_low(second, suspension_neg(first));
        first = r_s32(car + 1940u); second = r_s32(car + 1948u);
        pitch = suspension_low(r_s16(parameters + 32u), suspension_sub(second, first));
        first = r_s32(car + 192u); second = r_s32(car + 1904u);
        value = suspension_sub(suspension_sub(first, front), second / 2);
        w_u32(scratch + 28u, (uint32)suspension_add(suspension_add(value, pitch), roll));
        first = r_s32(car + 260u); second = r_s32(car + 1904u);
        value = suspension_sub(suspension_add(front, first), second / 2);
        w_u32(scratch + 32u, (uint32)suspension_sub(suspension_add(value, pitch), roll));
        first = r_s32(car + 328u); second = r_s32(car + 1908u);
        value = suspension_sub(suspension_sub(first, rear), second / 2);
        w_u32(scratch + 36u, (uint32)suspension_add(suspension_sub(value, pitch), roll));
        first = r_s32(car + 396u); second = r_s32(car + 1908u);
        value = suspension_sub(suspension_add(rear, first), second / 2);
        w_u32(scratch + 40u, (uint32)suspension_sub(suspension_sub(value, pitch), roll));
        for (wheel = 0; wheel < 4u; ++wheel) w_u32(scratch + 44u + 4u * wheel, r_u32(car + 196u + 68u * wheel));
    }
    for (index = 0; index < count; ++index) {
        uint32 outer = cars + 3380u * (uint32)index, car = outer + 40u;
        uint32 parameters = outer + 444u, scratch = 0x1F800000u + 56u * (uint32)index, key, route;
        sint32 mode, heading, difference, step, first, old_gravity, final_velocity;
        if (r_s8(outer + 1729u) == 4) continue;
        key = r_u16(outer + 52u);
        if (key >= 2u) {
            route = suspension_route(key, &mode);
            if (suspension_route_apply(car, route)) {
                if (mode == 2) {
                    sint32 route_index = r_s8(outer + 1735u);
                    route = r_u32(0x800BF51Cu); route = r_u32(route + 24u);
                    heading = r_s32(route + 40u * (uint32)route_index + 40u);
                    difference = gtmain_race_angle_delta(heading, r_s16(outer + 1830u));
                    if (difference) {
                        step = suspension_time(341);
                        if (step < difference) w_u16(car + 1790u, (uint16)(r_u16(car + 1790u) + (uint32)step));
                        else if (difference < suspension_neg(step)) w_u16(car + 1790u, (uint16)(r_u16(car + 1790u) - (uint32)step));
                        else w_u16(car + 1790u, (uint16)heading);
                    }
                }
                suspension_basis(car); continue;
            }
        }
        w_u32(0x1F800000u, r_u32(car + 4u));
        suspension_axis(car, scratch, parameters, 1796u, 1786u, 100u, 104u, 0);
        suspension_axis(car, scratch, parameters, 1800u, 1788u, 108u, 112u, 1);
        if (r_s8(car + 1690u)) { if (r_s32(scratch + 8u) < 4) w_u8(car + 1690u, 0); }
        else if (r_u32(scratch + 8u) == 4u) w_u8(car + 1690u, 1);
        suspension_basis(car);
        first = suspension_impulse(scratch, 28u, parameters); suspension_velocity(car, first);
        first = suspension_time(40141); old_gravity = suspension_sub(r_s32(car + 1720u), first);
        w_u32(car + 1720u, (uint32)old_gravity);
        first = suspension_impulse(scratch, 44u, parameters); suspension_velocity(car, first);
        final_velocity = r_s32(car + 1720u);
        if ((final_velocity > 0 && old_gravity < 0) || (final_velocity < 0 && old_gravity > 0)) w_u32(car + 1720u, 0);
    }
    for (index = 0; index < count; ++index) {
        uint32 outer = cars + 3380u * (uint32)index, car = outer + 40u;
        uint32 parameters = outer + 444u, scratch = 0x1F800000u + 56u * (uint32)index, flags;
        sint32 first, second, third, fourth, fraction, thresholds[4], d[4], average_front, average_rear;
        uint32 correction;
        if (r_s8(outer + 1729u) == 4 || !r_u8(outer + 1734u)) continue;
        first = suspension_low(r_s16(outer + 116u), r_s16(outer + 456u));
        second = suspension_low(r_s16(car + 76u), r_s16(parameters + 14u));
        third = suspension_low(r_s16(car + 84u), r_s16(parameters + 24u));
        fourth = suspension_low(r_s16(car + 84u), r_s16(parameters + 26u));
        w_u16(car + 144u, (uint16)suspension_sub(first, third));
        w_u16(car + 212u, (uint16)suspension_add(third, first));
        w_u16(car + 348u, (uint16)suspension_sub(fourth, second));
        w_u16(car + 280u, (uint16)suspension_sub(suspension_neg(fourth), second));
        fraction = r_s32(scratch + 4u);
        first = r_s16(parameters + 1156u); second = r_s16(parameters + 296u);
        thresholds[0] = thresholds[1] = suspension_sub(second, first);
        first = r_s16(parameters + 1158u); second = r_s16(parameters + 372u);
        thresholds[2] = thresholds[3] = suspension_sub(second, first);
        suspension_heights(car, scratch, fraction, thresholds);
        flags = r_u8(car + 1694u);
        first = r_s16(parameters + 292u); thresholds[0] = thresholds[1] = first;
        first = r_s16(parameters + 368u); thresholds[2] = thresholds[3] = first;
        flags = suspension_mask(scratch, thresholds, flags << 4);
        w_u8(car + 1694u, flags);
        if (flags & 15u) {
            d[0] = suspension_sub((sint32)r_u16(parameters + 292u), (sint32)r_u16(scratch + 12u)); if ((sint16)d[0] < 0) d[0] = 0;
            d[1] = suspension_sub((sint32)r_u16(parameters + 292u), (sint32)r_u16(scratch + 16u)); if ((sint16)d[1] < 0) d[1] = 0;
            d[2] = suspension_sub((sint32)r_u16(parameters + 368u), (sint32)r_u16(scratch + 20u)); if ((sint16)d[2] < 0) d[2] = 0;
            d[3] = suspension_sub((sint32)r_u16(parameters + 368u), (sint32)r_u16(scratch + 24u)); if ((sint16)d[3] < 0) d[3] = 0;
            first = suspension_sub((sint16)d[2], (sint16)d[0]); second = r_s16(parameters + 20u);
            average_front = (sint32)suspension_half(suspension_add((sint16)d[0], (sint16)d[1]));
            average_rear = (sint32)suspension_half(suspension_add((sint16)d[2], (sint16)d[3]));
            first = suspension_add(d[0], suspension_low(first, second));
            second = suspension_sub((sint16)d[3], (sint16)d[1]); third = r_s16(parameters + 20u);
            second = suspension_add(d[1], suspension_low(second, third));
            correction = suspension_ground_pair(car, parameters, 1788u, 18u, first, second);
            correction += suspension_ground_pair(car, parameters, 1786u, 16u, average_front, average_rear);
            w_u32(car + 68u, (uint32)suspension_add((sint16)correction, r_s32(car + 68u)));
        }
        first = gtmain_vector_dot_fixed((const uint8 *)psx_addr(car + 1712u, 12u), (const uint8 *)psx_addr(car + 108u, 6u));
        second = gtmain_vector_dot_fixed((const uint8 *)psx_addr(car + 1712u, 12u), (const uint8 *)psx_addr(car + 116u, 6u));
        first = suspension_low(first, r_s16(car + 112u)); second = suspension_low(second, r_s16(car + 120u));
        first = suspension_add(first, second); if (first > 0) first = 0;
        if (r_s32(car + 1720u) < first) w_u32(car + 1720u, (uint32)first);
    }
}
