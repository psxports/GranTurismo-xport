#include "game_first2_registry.h"

static sint32 response_neg(sint32 value) { return (sint32)(0u - (uint32)value); }
static sint32 response_add(sint32 first, sint32 second) { return (sint32)((uint32)first + (uint32)second); }
static sint32 response_sub(sint32 first, sint32 second) { return (sint32)((uint32)first - (uint32)second); }
static sint32 response_low(sint32 first, sint32 second) { return (sint32)((uint32)first * (uint32)second) / 4096; }
static sint32 response_wide(sint32 first, sint32 second) { return (sint32)(uint32)(((sint64)first * second) / 4096); }

/* GTMAIN.EXE 80062B84 */
/* Original: GTMAIN.EXE:sub_80062B84. */ void gtmain_race_pair_clip_publish(uint32 cars, sint32 count)
{
    sint32 index, other;
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index, slot = 0;
        if (r_u8(car + 2003u) || r_u8(car + 1952u) == 4u) continue;
        for (other = 0; other < count; ++other) {
            uint32 other_car;
            sint32 corner, side, fraction;
            if (other == index) continue;
            other_car = cars + 3380u * (uint32)other;
            if (!r_u8(other_car + 2003u) && r_u8(other_car + 1952u) != 4u) {
                fraction = gtmain_race_pair_clip((uint32)index, slot, car + 40u, &corner, &side);
                w_u16(0x800BFE00u + 10u * (uint32)index + 2u * slot, (uint16)fraction);
                w_u8(0x800BF550u + 5u * (uint32)index + slot, (uint8)corner);
                w_u8(0x800BF570u + 5u * (uint32)index + slot, (uint8)side);
            }
            ++slot;
        }
    }
}

/* GTMAIN.EXE 8005FDE8 */
/* Original: GTMAIN.EXE:sub_8005FDE8. */ sint32 gtmain_race_suspension_response(uint32 car, uint32 contact, sint32 height, uint32 parameters, uint32 *inactive_count)
{
    sint32 limit = r_s16(parameters + 2u), compression, velocity, force, damping, value, sum;
    if (limit < height) {
        w_u16(contact + 4u, (uint16)limit); w_u8(contact, 0);
        *inactive_count += 1u;
        w_u32(contact + 20u, 0);
        value = r_s32(parameters + 72u);
        w_u32(contact + 64u, 0);
        w_u32(contact + 60u, (uint32)response_neg(value));
        return response_neg(r_s32(parameters + 72u));
    }
    w_u8(contact, 1);
    limit = r_s16(parameters);
    compression = height < limit ? limit : height;
    value = r_s16(contact + 4u);
    limit = r_s32(car + 8u);
    velocity = response_wide(response_sub(height, value), limit);
    w_u32(contact + 56u, (uint32)velocity);
    force = response_neg(response_low(compression, r_s32(parameters + 8u)));
    if (compression < 0) {
        value = response_low(compression, compression);
        value = response_wide(value, r_s32(parameters + 64u));
        force = response_add(force, value);
    }
    limit = r_s32(parameters + 40u);
    if (velocity >= limit) damping = response_neg(r_s32(parameters + 48u));
    else {
        limit = r_s32(parameters + 16u);
        value = response_sub(velocity, limit);
        if (limit < velocity) {
            sint32 coefficient = r_s32(parameters + 56u);
            damping = response_neg(r_s32(parameters + 24u));
            damping = response_sub(damping, response_low(value, coefficient));
        } else if (velocity > 0) damping = response_neg(response_low(velocity, r_s32(parameters + 32u)));
        else {
            limit = response_neg(r_s32(parameters + 36u));
            if (velocity <= limit) damping = r_s32(parameters + 44u);
            else {
                limit = r_s32(parameters + 12u);
                value = response_add(velocity, limit);
                if (velocity < response_neg(limit)) {
                    value = response_low(value, r_s32(parameters + 52u));
                    damping = response_sub(r_s32(parameters + 20u), value);
                } else if (velocity < 0) damping = response_neg(response_low(velocity, r_s32(parameters + 28u)));
                else damping = 0;
            }
        }
    }
    w_u32(contact + 60u, (uint32)force);
    force = r_s32(contact + 60u);
    w_u16(contact + 4u, (uint16)compression);
    w_u32(contact + 64u, (uint32)damping);
    value = r_s32(parameters + 72u);
    sum = response_add(force, damping);
    value = response_add(sum, value);
    w_u32(contact + 20u, (uint32)value);
    if (value < 0) w_u32(contact + 20u, 0);
    return sum;
}
