#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified native draft from complete pseudocode and MIPS */
static uint32 pc_mul(sint32 a, sint32 b) { return (uint32)((sint64)a * b / 4096); }
static uint32 pc_abs(uint32 a) { return (sint32)a < 0 ? 0u - a : a; }
static sint32 pc_div(uint32 a, uint32 b)
{
    if (!b) gt_draft_exception(7u);
    if (a == 0x80000000u && b == 0xFFFFFFFFu) gt_draft_exception(6u);
    return (sint32)a / (sint32)b;
}
static sint32 pc_range(uint32 a, uint32 radius)
{
    return (sint32)((0u - radius - a) ^ (radius - (a + 1u)));
}
static void pc_cache_car(uint32 cars, sint32 count, uint32 owner, uint32 toggle)
{
    uint32 car = cars + owner * 3380u + 40u, slot = 0;
    uint32 angle = r_u16(car + 1790u) & 4095u;
    sint32 sine = r_s16(0x8009423Cu + angle * 2u), cosine = r_s16(0x80094A3Cu + angle * 2u);
    sint32 other;
    for (other = 0; other < count; ++other) {
        if ((uint32)other == owner) continue;
        uint32 peer = cars + (uint32)other * 3380u + 40u;
        uint32 flags = 0x800BFD10u + toggle * 120u + owner * 20u + slot * 4u;
        if (!r_u8(peer + 1963u) && r_u8(peer + 1912u) != 4u) {
            uint32 position[3], axis;
            for (axis = 0; axis < 3u; ++axis) position[axis] = r_u32(car + 60u + axis * 4u);
            uint32 vertical = r_u32(peer + 68u) - position[2];
            if ((sint32)((0xFFFFE000u - vertical) ^ (8192u - vertical)) > 0) {
                for (axis = 0; axis < 4u; ++axis) w_u8(flags + axis, 0);
            } else for (axis = 0; axis < 4u; ++axis) {
                uint32 x = r_u32(peer + 1824u + r_u8(peer + 1961u) * 32u + axis * 8u) - position[0];
                uint32 z = r_u32(peer + 1828u + r_u8(peer + 1961u) * 32u + axis * 8u) - position[1];
                uint32 radius = ((sint8)r_u8(car + 1689u) == 4 || (sint8)r_u8(peer + 1689u) == 4) ? 147456u : 65536u;
                if (pc_range(x, radius) <= 0 && pc_range(z, radius) <= 0) {
                    w_u8(flags + axis, 1);
                    uint32 record = 0x800BF590u + toggle * 960u + owner * 160u + slot * 32u + axis * 8u;
                    w_u32(record, pc_mul(cosine, (sint32)x) + pc_mul(sine, (sint32)z));
                    w_u32(record + 4u, pc_mul(-sine, (sint32)x) + pc_mul(cosine, (sint32)z));
                } else w_u8(flags + axis, 0);
            }
        }
        ++slot;
    }
}
/* Original: GTMAIN.EXE:sub_80062234. */ uint32 gtmain_race_pair_cache_update(uint32 cars, sint32 count)
{
    uint32 toggle = 1u - r_u32(0x800BF540u);
    w_u32(0x800BF540u, toggle);
    if (count <= 0) return toggle;
    sint32 i;
    for (i = 0; i < count; ++i) {
        uint32 car = cars + (uint32)i * 3380u + 40u;
        toggle = r_u32(0x800BF540u);
        if (!r_u8(car + 1963u) && r_u8(car + 1912u) != 4u) pc_cache_car(cars, count, (uint32)i, toggle);
    }
    return (uint32)count * 2u;
}
/* Original: GTMAIN.EXE:sub_80064524. */ void gtmain_race_pair_cache_refresh(uint32 cars, sint32 count)
{
    sint32 i;
    for (i = 0; i < count; ++i) {
        uint32 car = cars + (uint32)i * 3380u + 40u;
        if (!(r_u8(car + 1698u) & 2u)) continue;
        gtmain_race_vehicle_bounds_update(car);
        uint32 toggle = r_u32(0x800BF540u);
        if (!r_u8(car + 1963u) && r_u8(car + 1912u) != 4u) pc_cache_car(cars, count, (uint32)i, toggle);
    }
}
static uint32 pc_code(sint32 x, sint32 z, sint32 half, sint32 front, sint32 back)
{
    uint32 code = z >= front ? 8u : 0u;
    if (back >= z) code |= 4u;
    if (-half >= x) code |= 2u;
    if (x >= half) code |= 1u;
    return code;
}
/* Original: GTMAIN.EXE:sub_800626A0. */ sint32 gtmain_race_pair_clip(uint32 car_index, uint32 other_slot, uint32 car, sint32 *corner, sint32 *side)
{
    sint32 minimum = 4096, chosen_corner = -1, chosen_side = -1;
    sint32 front = r_s32(car + 404u);
    uint32 toggle = r_u32(0x800BF540u);
    sint32 back = (sint32)(0u - r_u32(car + 408u)), half = r_s16(car + 412u) / 2;
    uint32 i;
    for (i = 0; i < 4u; ++i) {
        uint32 flag = 0x800BFD10u + car_index * 20u + other_slot * 4u + i;
        if (!r_u8(flag + (1u - toggle) * 120u) || !r_u8(flag + toggle * 120u)) continue;
        uint32 record = 0x800BF590u + car_index * 160u + other_slot * 32u + i * 8u;
        uint32 previous = record + (1u - toggle) * 960u;
        sint32 z0 = r_s32(previous + 4u), x0 = r_s32(previous);
        uint32 a = pc_code(x0, z0, half, front, back);
        if (!a) continue;
        uint32 current = record + toggle * 960u;
        sint32 z1 = r_s32(current + 4u), x1 = r_s32(current);
        uint32 b = pc_code(x1, z1, half, front, back);
        if (a & b) continue;
        uint32 different = a ^ b, edge;
        for (edge = 0; edge < 4u; ++edge) {
            uint32 mask = 8u >> edge;
            if (!(different & mask)) continue;
            uint32 numerator, denominator, origin, delta, lower, upper, link;
            if (edge < 2u) {
                numerator = (uint32)(edge == 0u ? front : back) - (uint32)z0;
                denominator = (uint32)z1 - (uint32)z0; origin = (uint32)x0; delta = (uint32)x1 - (uint32)x0;
                lower = (uint32)-half; upper = (uint32)half; link = edge == 0u ? 0x80062930u : 0x800629BCu;
            } else {
                numerator = (uint32)(edge == 2u ? -half : half) - (uint32)x0;
                denominator = (uint32)x1 - (uint32)x0; origin = (uint32)z0; delta = (uint32)z1 - (uint32)z0;
                lower = (uint32)back; upper = (uint32)front; link = edge == 2u ? 0x80062A40u : 0x80062ACCu;
            }
            sint32 fraction = (sint32)(uint32)gtmain_signed_divide64((sint64)(sint32)numerator * 4096, (sint32)denominator, link);
            if (fraction >= minimum) continue;
            uint32 position = origin + (uint32)((sint32)((uint32)fraction * delta) / 4096);
            if ((sint32)((lower - position) ^ (upper - position)) < 0) {
                minimum = fraction; chosen_corner = (sint32)i; chosen_side = (sint32)edge;
            }
        }
    }
    *corner = chosen_corner; *side = chosen_side;
    return minimum;
}
/* Original: GTMAIN.EXE:sub_800649A4. */ void gtmain_race_pair_impulses_update(uint32 cars, sint32 count)
{
    if (count <= 0) return;
    uint32 toggle = r_u32(0x800BF540u);
    sint32 i, j;
    for (i = 0; i < count; ++i) {
        uint32 car = cars + (uint32)i * 3380u + 40u;
        sint32 front = (sint32)(r_u32(car + 404u) * 15u) / 16;
        sint32 back = (sint32)(r_u32(car + 408u) * 15u) / 16;
        sint32 half = r_s16(car + 412u) * 15 / 32;
        w_u32(car + 1976u, 0); w_u32(car + 1980u, 0);
        if (r_u8(car + 1963u) || r_u8(car + 1912u) == 4u) continue;
        uint32 slot = 0;
        for (j = 0; j < count; ++j) {
            if (j == i) continue;
            uint32 peer = cars + (uint32)j * 3380u + 40u;
            if (r_u8(peer + 1963u) || r_u8(peer + 1912u) == 4u) { ++slot; continue; }
            uint32 x[4], z[4], valid[4], any = 0, k;
            for (k = 0; k < 4u; ++k) {
                valid[k] = 0;
                uint32 flags = 0x800BFD10u + toggle * 120u + (uint32)i * 20u + slot * 4u + k;
                if (!r_u8(flags)) continue;
                uint32 record = 0x800BF590u + toggle * 960u + (uint32)i * 160u + slot * 32u + k * 8u;
                uint32 px = r_u32(record);
                if ((sint32)px >= half || -half >= (sint32)px) continue;
                uint32 pz = r_u32(record + 4u);
                if ((sint32)pz >= front || (sint32)(0u - (uint32)back) >= (sint32)pz) continue;
                any = 1; x[k] = px; z[k] = pz; valid[k] = 1;
            }
            if (!any) { ++slot; continue; }
            uint32 dx = r_u32(peer + 60u) - r_u32(car + 60u);
            uint32 angle = r_u16(car + 1790u) & 4095u;
            sint32 cosine = r_s16(0x80094A3Cu + angle * 2u), sine = r_s16(0x8009423Cu + angle * 2u);
            uint32 dz = r_u32(peer + 64u) - r_u32(car + 64u);
            uint32 longitudinal = pc_mul(cosine, (sint32)dx) + pc_mul(sine, (sint32)dz);
            dx = r_u32(peer + 60u) - r_u32(car + 60u);
            uint32 lateral = 0u - pc_mul(sine, (sint32)dx);
            dz = r_u32(peer + 64u) - r_u32(car + 64u);
            lateral += pc_mul(cosine, (sint32)dz);
            uint32 penetration = 0;
            sint32 side = -1;
            if ((sint32)pc_abs(lateral) < (sint32)pc_abs(longitudinal)) {
                uint32 denominator = (uint32)half * 2u;
                for (k = 0; k < 4u; ++k) if (valid[k]) {
                    uint32 distance = (sint32)longitudinal > 0 ? (uint32)half - x[k] : (uint32)half + x[k];
                    sint32 fraction = pc_div(distance << 12, denominator);
                    if ((sint32)penetration < fraction) { penetration = (uint32)fraction; side = (sint32)longitudinal > 0 ? 3 : 2; }
                }
            } else {
                uint32 denominator = (uint32)front + (uint32)back;
                for (k = 0; k < 4u; ++k) if (valid[k]) {
                    uint32 distance = (sint32)lateral > 0 ? (uint32)front - z[k] : (uint32)back + z[k];
                    sint32 fraction = pc_div(distance << 12, denominator);
                    if ((sint32)penetration < fraction) { penetration = (uint32)fraction; side = (sint32)lateral < 1; }
                }
            }
            if (!penetration) { ++slot; continue; }
            sint32 normal[2];
            /* Nonzero penetration selects exactly one of sides 0 through 3 */
            if (side == 0) { normal[0] = -sine; normal[1] = cosine; }
            else if (side == 1) { normal[0] = sine; normal[1] = -cosine; }
            else if (side == 2) { normal[0] = -cosine; normal[1] = -sine; }
            else { normal[0] = cosine; normal[1] = sine; }
            uint32 magnitude = penetration + 2048u;
            if ((sint32)magnitude >= 4097) magnitude = 4096;
            magnitude = pc_mul(313601, (sint32)magnitude);
            for (k = 0; k < 2u; ++k) {
                uint32 impulse = pc_mul((sint32)magnitude, normal[k]);
                if ((sint8)r_u8(peer + 1689u) == 4) w_u32(car + 1976u + k * 4u, r_u32(car + 1976u + k * 4u) - impulse * 2u);
                else if ((sint8)r_u8(car + 1689u) == 4) w_u32(peer + 1976u + k * 4u, r_u32(peer + 1976u + k * 4u) + impulse * 2u);
                else {
                    w_u32(peer + 1976u + k * 4u, r_u32(peer + 1976u + k * 4u) + impulse);
                    w_u32(car + 1976u + k * 4u, r_u32(car + 1976u + k * 4u) - impulse);
                }
            }
            ++slot;
        }
    }
}
