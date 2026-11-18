#include "game_first2_registry.h"

static sint32 pair_add(sint32 a, sint32 b) { return (sint32)((uint32)a + (uint32)b); }
static sint32 pair_sub(sint32 a, sint32 b) { return (sint32)((uint32)a - (uint32)b); }
static sint32 pair_neg(sint32 a) { return (sint32)(0u - (uint32)a); }
static sint32 pair_low(sint32 a, sint32 b) { return (sint32)((uint32)a * (uint32)b) / 4096; }
static sint32 pair_fixed(sint64 value) { return (sint32)(uint32)(value / 4096); }

static sint32 pair_projection(uint32 car, uint32 offset, sint32 x, sint32 z, sint32 y, int wide)
{
    sint32 first, second, third, coefficient;
    coefficient = r_s16(car + offset);
    first = wide ? pair_fixed((sint64)coefficient * x) : pair_low(coefficient, x);
    coefficient = r_s16(car + offset + 2u);
    second = wide ? pair_fixed((sint64)coefficient * z) : pair_low(coefficient, z);
    coefficient = r_s16(car + offset + 4u);
    third = wide ? pair_fixed((sint64)coefficient * y) : pair_low(coefficient, y);
    return pair_add(pair_add(first, second), third);
}

static void pair_regions(uint32 car, sint32 forward, sint32 lateral, sint32 front, sint32 back, sint32 width)
{
    sint32 negative_half = pair_neg(width) / 2, positive_half = width / 2;
    if (front < forward) {
        if (lateral < negative_half) { w_u8(car + 1813u, 2); return; }
        if (positive_half < lateral) { w_u8(car + 1814u, 2); return; }
        w_u8(car + 1812u, 2);
        w_u8(car + (lateral <= 0 ? 1813u : 1814u), 1); return;
    }
    if (forward < pair_neg(back)) {
        if (lateral < negative_half) { w_u8(car + 1817u, 2); return; }
        if (positive_half < lateral) { w_u8(car + 1818u, 2); return; }
        w_u8(car + 1819u, 2);
        w_u8(car + (lateral <= 0 ? 1817u : 1818u), 1); return;
    }
    w_u8(car + (lateral < 0 ? 1815u : 1816u), 2);
    if (lateral < 0) w_u8(car + (forward > 0 ? 1813u : 1817u), 1);
    else w_u8(car + (forward > 0 ? 1814u : 1818u), 1);
}

static sint32 pair_divide(sint32 numerator, sint32 denominator)
{
    if (!denominator) { gt_draft_exception(7); return 0; }
    if (denominator == -1 && (uint32)numerator == 0x80000000u) { gt_draft_exception(6); return 0; }
    return numerator / denominator;
}

static void pair_braking(uint32 car, sint32 gap, sint32 lateral, sint32 width, sint32 dx, sint32 dz, sint32 dy, int reverse)
{
    sint32 velocity, quotient, numerator;
    if (!r_s8(car + 1689u) && !r_u8(car + 1696u)) return;
    if (gap <= 0 || !(pair_neg(width) < lateral && lateral < width)) return;
    velocity = pair_projection(car, 72u, dx, dz, dy, 1);
    if (reverse) velocity = pair_neg(velocity);
    if (velocity < -40968) w_u32(car + 1808u, 4096);
    else if (velocity < -1138) {
        numerator = (sint32)((uint32)gap << 12);
        quotient = reverse ? pair_neg(pair_divide(numerator, velocity)) : pair_divide(numerator, pair_neg(velocity));
        if (quotient < 12288) w_u32(car + 1808u, 2048);
    }
    if (!r_u32(car + 1808u) && gap < 4096) w_u32(car + 1808u, 1024);
}

static void pair_wake(uint32 car, uint32 other, sint32 forward, sint32 lateral, sint32 width, uint32 caller)
{
    sint32 attenuation = r_s16(car + 1820u), speed, value;
    if (attenuation == 4096) return;
    speed = r_s32(other + 1724u);
    if (speed <= 113800 || forward >= 0 || !(pair_neg(speed) < forward) || !(pair_neg(width) < lateral && lateral < width)) return;
    value = pair_add(forward, speed);
    value = (sint32)(uint32)gtmain_signed_divide64((sint64)value * 4096, speed, caller);
    value = pair_add(value, attenuation); w_u16(car + 1820u, (uint16)value);
    if ((sint16)value > 4096) w_u16(car + 1820u, 4096);
}

/* GTMAIN.EXE 80068150 */
/* Original: GTMAIN.EXE:sub_80068150. */ void gtmain_race_pair_proximity_update(uint32 first_car, uint32 second_car)
{
    uint32 a = first_car, b = second_car, flags_a, flags_b;
    sint32 ax, az, ay, bx, bz, by, dx, dz, dy, distance, temporary, vertical;
    sint32 forward_a, side_a, forward_b, side_b, front_a, back_a, width_a, front_b, back_b, width_b;
    sint32 avx, avz, avy, bvx, bvz, bvy, vx, vz, vy, width;
    if (r_u8(a + 1963u)) return;
    if (r_u8(b + 1963u)) return;
    if (r_u8(a + 1912u) == 4u) return;
    if (r_u8(b + 1912u) == 4u) return;
    flags_a = r_u8(a + 58u); flags_b = r_u8(b + 58u);
    if ((flags_a & 16u) != (flags_b & 16u)) return;
    ax = r_s32(a + 60u); bx = r_s32(b + 60u); az = r_s32(a + 64u); ay = r_s32(a + 68u);
    dx = pair_sub(ax, bx); bz = r_s32(b + 64u); by = r_s32(b + 68u); dz = pair_sub(az, bz); dy = pair_sub(ay, by);
    if (dx < 0) dx = pair_neg(dx); if (dz < 0) dz = pair_neg(dz); if (dy < 0) dy = pair_neg(dy);
    if (dx < dz) { temporary = dx; dx = dz; dz = temporary; }
    if (dx < dy) { temporary = dx; dx = dy; dy = temporary; }
    distance = pair_add(dx, pair_add(dz, dy) >> 2); if (distance > 409600) return;
    by = r_s32(b + 68u); ay = r_s32(a + 68u); temporary = r_s16(b + 414u); vertical = pair_sub(by, ay);
    width_a = r_s16(a + 414u);
    if (vertical < pair_neg(pair_add(temporary, 20480))) return;
    if (pair_add(width_a, 20480) < vertical) return;
    bx = r_s32(b + 60u); ax = r_s32(a + 60u); dx = pair_sub(bx, ax);
    bz = r_s32(b + 64u); az = r_s32(a + 64u); dz = pair_sub(bz, az); dy = vertical;
    forward_a = pair_projection(a, 72u, dx, dz, dy, 0); side_a = pair_projection(a, 80u, dx, dz, dy, 0);
    forward_b = pair_neg(pair_projection(b, 72u, dx, dz, dy, 0)); side_b = pair_neg(pair_projection(b, 80u, dx, dz, dy, 0));
    avx = r_s32(a + 1712u);
    front_a = r_s32(a + 404u); back_a = r_s32(a + 408u); width_a = r_s16(a + 412u);
    front_b = r_s32(b + 404u); back_b = r_s32(b + 408u); width_b = r_s16(b + 412u);
    avz = r_s32(a + 1716u); avy = r_s32(a + 1720u); bvx = r_s32(b + 1712u); bvz = r_s32(b + 1716u); bvy = r_s32(b + 1720u);
    if (distance <= 81920) {
        vx = pair_sub(bvx, avx); vz = pair_sub(bvz, avz); vy = pair_sub(bvy, avy); width = pair_add(width_a, width_b);
        pair_regions(a, forward_a, side_a, pair_add(back_b, front_a), pair_add(front_b, back_a), width);
        pair_regions(b, forward_b, side_b, pair_add(back_a, front_b), pair_add(front_a, back_b), width);
        pair_braking(a, pair_sub(pair_sub(forward_a, back_b), front_a), side_a, width, vx, vz, vy, 0);
        pair_braking(b, pair_sub(pair_sub(forward_b, back_a), front_b), side_b, width, vx, vz, vy, 1);
    }
    pair_wake(a, b, forward_b, side_b, width_b, 0x80068BC8u);
    pair_wake(b, a, forward_a, side_a, width_a, 0x80068C60u);
}
