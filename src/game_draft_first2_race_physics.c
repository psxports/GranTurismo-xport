#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

/* TODO Bind remaining recorded physics dependencies */
static uint32 physics_call(uint32 owner, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5)
{
    return gt_draft_unresolved_call(owner, a0, a1, a2, a3, a4, a5, 0, 0);
}

static uint32 physics_mul(uint32 a, uint32 b)
{
    return a * b;
}

static uint32 physics_div_pow2(uint32 value, uint32 shift)
{
    return (uint32)((sint32)value / (sint32)(1u << shift));
}

static uint32 physics_fp(uint32 a, uint32 b)
{
    return physics_div_pow2(physics_mul(a, b), 12u);
}

static uint32 physics_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static void physics_controls_read(uint8 *controls, uint32 source, uint32 player, uint32 object)
{
    gtmain_race_controls_decode(controls, source, (sint32)player, object);
}

static void physics_controls_apply(uint32 owner, uint32 car, const uint8 *controls, sint32 index)
{
    if (owner == 0x80056D04u) gtmain_race_controls_ai(car, controls, index);
    else if (owner == 0x80065C6Cu) gtmain_race_controls_drive(car, controls, index);
    else if (owner == 0x80052094u) gtmain_race_controls_hud(car, controls);
    else gt_draft_failure("GTMAIN unknown native physics control consumer", owner);
}

/* GTMAIN.EXE 800521AC */
void gtmain_race_physics_prepare(uint32 cars, sint32 count)
{
    sint32 index;
    uint32 replay = r_s16(0x800B63DEu) != 0;
    uint8 controls[12];
    uint32 controls_valid = 0;
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        sint32 timer = r_s16(car + 74u);
        w_u8(car + 1728u, (uint8)replay);
        if (timer) w_u16(car + 74u, (uint16)(timer - 1));
        if (r_u16(car + 52u)) w_u16(car + 52u, (uint16)(r_u16(car + 52u) - 1u));
        {
            uint32 lifetime = r_u8(car + 2005u);
            if (lifetime && lifetime != 255u) w_u8(car + 2005u, (uint8)(r_u8(car + 2005u) - 1u));
        }
        gtmain_race_speed_apply(car + 40u);
    }
    if (count > 0) {
        uint32 blend = r_u32(0x80093D80u);
        for (index = 0; index < count; ++index) {
            uint32 car = cars + 3380u * (uint32)index, square, speed, coefficient, result;
            sint32 acceleration;
            w_u32(0x1F800000u, r_u32(car + 44u));
            speed = (uint32)((sint32)physics_abs(r_u32(car + 1764u)) >> 5);
            square = physics_div_pow2(physics_mul(speed, speed), 12u);
            acceleration = r_s16(car + 1860u);
            if (acceleration) {
                result = r_u16(car + 1862u) + physics_fp((uint32)acceleration, physics_div_pow2(r_u32(0x1F800000u), 4u));
                w_u16(car + 1862u, (uint16)result);
                if ((sint16)result >= 4097) w_u16(car + 1862u, 4096);
            } else {
                result = r_u16(car + 1862u) - physics_div_pow2(r_u32(0x1F800000u), 6u);
                w_u16(car + 1862u, (uint16)result);
                if ((sint16)result < 0) w_u16(car + 1862u, 0);
            }
            coefficient = physics_fp(physics_fp(4096u - blend, 4096u - (uint32)r_s16(car + 1862u)) + blend, square);
            result = physics_fp(coefficient, r_u32(car + 1696u));
            speed = r_u32(car + 1764u);
            w_u16(car + 1936u, (uint16)result);
            if ((sint32)speed >= 0) w_u16(car + 1936u, (uint16)(0u - result));
            w_u32(car + 1944u, physics_fp(coefficient, r_u32(car + 1700u)));
            w_u32(car + 1948u, physics_fp(coefficient, r_u32(car + 1704u)));
        }
    }
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        uint32 result = (uint32)gtmain_race_curve32(car + 560u, r_s32(car + 1928u));
        sint64 product = (sint64)r_s32(car + 704u) * r_s32(car + 1928u);
        w_u32(car + 1720u, result);
        w_u32(car + 1724u, (uint32)(product / 4096));
    }
    gtmain_race_suspension_prepare(cars, count);
    gtmain_race_suspension_update(cars, count);
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        sint32 type = r_s16(car + 24u), state;
        switch (type) {
        case 0: memset(controls, 0, 12); controls_valid = 1; break;
        case 2: physics_controls_read(controls, 0x800BC1C0u, 1, r_u32(car + 28u)); controls_valid = 1; break;
        case 3: physics_controls_read(controls, 0x800BC280u, 0, r_u32(car + 28u)); controls_valid = 1; break;
        case 6:
            if (gtmain_race_object_flag(r_u32(car + 28u))) { w_u32(car + 12u, r_u32(car + 12u) & ~1u); break; }
            /* Fall through */
        case 5: physics_controls_read(controls, 0x800BC220u, 2, r_u32(car + 28u)); controls_valid = 1; break;
        default: break;
        }
        w_u32(0x1F800000u, r_u32(car + 44u));
        state = r_s8(car + 1729u);
        if (state == 2) {
            physics_controls_apply(0x80056D04u, car + 40u, NULL, index);
            gtmain_race_controls_hud_ai(car + 40u);
        } else if (state == 4) physics_call(0x800521A4u, car + 40u, 0, 0, 0, 0, 0);
        else if (!state) {
            if (!controls_valid) {
                /* TODO Resolve the original first default route's uninitialized control bytes */
                gt_draft_failure("GTMAIN undefined initial physics controls", 0x800521ACu);
            }
            if (r_u8(car + 1736u) && !r_u16(car + 52u)) physics_controls_apply(0x80056D04u, car + 40u, controls, index);
            else physics_controls_apply(0x80065C6Cu, car + 40u, controls, index);
            physics_controls_apply(0x80052094u, car + 40u, controls, index);
        }
    }
    gtmain_race_simulation_step(cars, count);
    gtmain_race_velocity_publish(cars, count);
}

/* GTMAIN.EXE 80067368 */
void gtmain_race_collisions_tick(uint32 cars, sint32 count)
{
    sint32 index, other;
    for (index = 0; index < count; ++index) gtmain_race_controls_reset(cars + 3380u * (uint32)index + 40u);
    for (index = 0; index < count; ++index) gtmain_race_controls_apply_basic(cars + 3380u * (uint32)index + 40u, (uint8 *)psx_addr(0x1F8000B4u + 16u * (uint32)index, 16u));
    gtmain_race_pair_cache_update(cars, count);
    gtmain_race_pair_clip_publish(cars, count);
    gtmain_race_pair_response(cars, count);
    gtmain_race_pair_cache_refresh(cars, count);
    gtmain_race_pair_impulses_update(cars, count);
    for (index = 0; index < count; ++index)
        for (other = index + 1; other < count; ++other)
            gtmain_race_pair_proximity_update(cars + 3380u * (uint32)index + 40u, cars + 3380u * (uint32)other + 40u);
    if (count > 0) {
        sint32 limit = (sint32)(3u * r_u32(0x800BF518u)) / 5;
        for (index = 0; index < count; ++index) {
            uint32 car = cars + 3380u * (uint32)index;
            sint32 contact = r_s16(car + 82u);
            if (contact) {
                if (contact >= 4097) w_u16(car + 82u, 4096);
                if (r_s16(car + 74u) < limit) w_u16(car + 74u, r_u16(0x800BF518u));
            }
        }
    }
}

/* GTMAIN.EXE 8005F734 */
void gtmain_race_wheel_contacts_prepare(uint32 cars, sint32 count)
{
    sint32 index;
    uint32 wheel;
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        uint32 a = physics_fp((uint32)r_s16(car + 112u), (uint32)r_s16(car + 456u));
        uint32 b = physics_fp((uint32)r_s16(car + 114u), (uint32)r_s16(car + 456u));
        uint32 c = physics_fp((uint32)r_s16(car + 112u), (uint32)r_s16(car + 458u));
        uint32 d = physics_fp((uint32)r_s16(car + 114u), (uint32)r_s16(car + 458u));
        uint32 e = physics_fp((uint32)r_s16(car + 120u), (uint32)r_s16(car + 468u));
        uint32 f = physics_fp((uint32)r_s16(car + 122u), (uint32)r_s16(car + 468u));
        uint32 g = physics_fp((uint32)r_s16(car + 120u), (uint32)r_s16(car + 470u));
        uint32 h = physics_fp((uint32)r_s16(car + 122u), (uint32)r_s16(car + 470u));
        w_u16(car + 180u, (uint16)(a - e)); w_u16(car + 182u, (uint16)(b - f));
        w_u16(car + 248u, (uint16)(e + a)); w_u16(car + 250u, (uint16)(f + b));
        w_u16(car + 316u, (uint16)(0u - g - c)); w_u16(car + 318u, (uint16)(0u - h - d));
        w_u16(car + 384u, (uint16)(g - c)); w_u16(car + 386u, (uint16)(h - d));
    }
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        for (wheel = 0; wheel < 4u; ++wheel) {
            uint32 contact = car + 172u + 68u * wheel, scratch = 0x1F800000u + 96u * (uint32)index + 24u * wheel;
            uint32 longitudinal = physics_abs(r_u32(car + 1764u));
            uint32 lateral = physics_abs(r_u32(car + (wheel < 2u ? 1972u : 1976u)));
            uint32 smaller = (sint32)lateral < (sint32)longitudinal ? lateral : longitudinal;
            uint32 distance = longitudinal + lateral - (uint32)((sint32)smaller >> 1);
            uint32 strength = (uint32)(((sint64)(sint32)distance * 82) / 4096);
            if ((sint32)strength >= 4097) strength = 4096;
            w_u16(contact + 46u, (uint16)strength);
            w_u32(scratch + 12u, ((uint32)r_s16(contact + 8u) + r_u32(car + 100u)) << 4);
            w_u32(scratch + 16u, ((uint32)r_s16(contact + 12u) + r_u32(car + 108u)) << 4);
            w_u32(scratch + 20u, ((uint32)r_s16(contact + 10u) + r_u32(car + 104u)) << 4);
            w_u16(scratch + 8u, r_u16(car + 1932u));
        }
    }
}

/* GTMAIN.EXE 8005FA9C */
void gtmain_race_wheel_contacts_query(uint32 unused_cars, sint32 count)
{
    sint32 index;
    uint32 wheel;
    (void)unused_cars;
    for (index = 0; index < count; ++index)
        for (wheel = 0; wheel < 4u; ++wheel)
            gtmain_collision_query((uint8 *)psx_addr(0x1F800004u + 96u * (uint32)index + 24u * wheel, 24u));
}

/* GTMAIN.EXE 8005FB34 */
void gtmain_race_wheel_contacts_apply(uint32 cars, sint32 count)
{
    sint32 index;
    uint32 wheel;
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index, terrain_count = 0;
        w_u32(car + 1932u, r_u16(0x1F800008u + 96u * (uint32)index));
        for (wheel = 0; wheel < 4u; ++wheel) {
            uint32 contact = car + 172u + 68u * wheel, scratch = 0x1F800000u + 96u * (uint32)index + 24u * wheel;
            sint32 height = r_s32(scratch + 24u);
            uint32 response = 0;
            if (height != 0x7FFFFFFF) {
                uint32 old_height = r_u32(contact + 16u);
                uint32 interpolated = (uint32)(((sint64)(sint32)old_height * 1024) / 4096);
                interpolated += (uint32)(((sint64)(height >> 4) * 3072) / 4096);
                w_u32(contact + 16u, interpolated);
                w_u8(contact + 44u, r_u8(scratch + 4u)); w_u8(contact + 45u, r_u8(scratch + 5u));
                if (r_u8(scratch + 6u) & 2u) ++terrain_count;
                if (r_s16(0x80093E98u + 2u * r_u8(contact + 44u))) {
                    uint32 x = r_u32(scratch + 12u), y = r_u32(scratch + 20u);
                    sint32 slope = r_s16(0x8009423Cu + (x & 0x1FF0u));
                    slope += r_s16(0x80094A3Cu + (y & 0x1FF0u));
                    response = physics_fp((uint32)(slope / 8), (uint32)r_s16(contact + 46u));
                    response = physics_fp(response, (uint32)r_s16(0x80093E98u + 2u * r_u8(contact + 44u)));
                }
            }
            w_u16(contact + 52u, (uint16)response);
        }
        if (terrain_count) w_u8(car + 98u, (uint8)(r_u8(car + 98u) | 16u));
        else w_u8(car + 98u, (uint8)(r_u8(car + 98u) & 239u));
    }
}

/* GTMAIN.EXE 80052884 */
void gtmain_race_pose_update(uint32 cars, sint32 count)
{
    sint32 index;
    if (!r_s16(0x800B63DEu)) gtmain_race_suspension_step(cars, count);
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        w_u32(0x1F800000u, r_u32(car + 44u));
        gtmain_race_wheel_effects_update(car + 40u);
    }
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index, coefficient, result, drive, damp;
        uint32 target = (uint32)r_s16(car + 1830u), current = (uint32)r_s16(car + 1716u);
        coefficient = r_u8(0x80093C3Fu);
        result = (uint32)gtmain_race_angle_delta((sint32)target, (sint32)current);
        drive = physics_mul(result, r_u32(0x8008DF14u + 4u * coefficient));
        damp = physics_mul((uint32)r_s16(car + 1718u), r_u32(0x8008DF20u + 4u * coefficient));
        result = physics_mul(physics_div_pow2(drive, 12u) - physics_div_pow2(damp, 12u), r_u32(0x1F800000u));
        result = r_u16(car + 1718u) + physics_div_pow2(result, 16u);
        w_u16(car + 1718u, (uint16)result);
        result = physics_mul((uint32)(sint32)(sint16)result, r_u32(0x1F800000u));
        result = r_u16(car + 1716u) + physics_div_pow2(result, 16u);
        w_u16(car + 1716u, (uint16)result);
        result = (uint32)gtmain_race_angle_wrap((sint16)result);
        drive = (uint32)r_s16(car + 1830u);
        w_u16(car + 1716u, (uint16)result);
        result = (uint32)gtmain_race_angle_delta((sint32)drive, (sint16)result);
        w_u16(car + 80u, (uint16)result);
    }
    for (index = 0; index < count; ++index) gtmain_race_wheel_pose(cars + 3380u * (uint32)index + 40u);
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index;
        uint32 first = (uint32)r_s16(car + 1826u), second = (uint32)r_s16(car + 1828u), third = (uint32)r_s16(car + 1830u);
        gtmain_race_basis_set(car + 112u, car + 120u, car + 128u, (sint32)first, (sint32)second, (sint32)third);
    }
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index, first, second, third, fourth, x, y, left, right, sum, angle;
        first = r_u32(car + 324u); second = r_u32(car + 188u);
        x = physics_mul(first - second, (uint32)r_s16(car + 464u));
        first = r_u32(car + 392u); second = r_u32(car + 256u);
        y = physics_mul(first - second, (uint32)r_s16(car + 464u));
        left = physics_div_pow2(x, 12u) + r_u32(car + 188u);
        right = physics_div_pow2(y, 12u);
        first = r_u32(car + 100u); second = r_u32(car + 104u); third = r_u32(car + 188u); fourth = r_u32(car + 256u);
        w_u32(car + 136u, first);
        right += r_u32(car + 256u);
        w_u32(car + 140u, second);
        sum = left + right;
        w_u32(car + 144u, physics_div_pow2(sum, 1u));
        first = r_u32(car + 324u); second = r_u32(car + 392u);
        angle = (uint32)gtmain_angle_atan_wide((sint32)physics_div_pow2(third + fourth - (first + second), 1u), r_s16(car + 460u));
        w_u16(car + 1940u, (uint16)angle);
        angle = (uint32)gtmain_angle_atan_wide((sint32)(left - right), r_s16(car + 462u));
        w_u16(car + 1942u, (uint16)angle);
        first = (uint32)r_s16(car + 1940u); third = (uint32)r_s16(car + 1830u);
        gtmain_race_basis_set(car + 148u, car + 156u, car + 164u, (sint32)first, (sint16)angle, (sint32)third);
    }
    gtmain_race_wheel_rotation_update(cars, count);
}

/* GTMAIN.EXE 80057DB8 */
void gtmain_race_lap_groups_tick(uint32 cars)
{
    sint32 index, group;
    for (index = 0; index < r_s16(0x800BC1A6u); ++index) {
        sint32 rank = r_s8(0x800BFE40u + (uint32)index);
        gtmain_race_lap_car_tick(cars + 3380u * (uint32)rank + 40u, rank);
    }
    for (group = 0; group < (sint32)(r_u32(r_u32(0x800BC1B0u) + 28u) + 1u); ++group) {
        uint32 slot = 4u * (uint32)group;
        sint32 old_count = r_s32(0x800B54C0u + slot), new_count = r_s32(0x800B54A0u + slot);
        if (old_count == new_count) continue;
        if (old_count < new_count) {
            index = old_count;
            do {
                uint32 offset = 24u * (uint32)group, item = 4u * (uint32)index;
                uint32 resource = r_u32(0x800B54E0u + offset + item);
                if (index) {
                    uint32 current = r_u32(0x800B53C0u + offset + item);
                    uint32 first = r_u32(0x800B53C0u + offset);
                    gtmain_race_lap_difference_show((sint32)resource, (sint32)(current - first));
                }
                ++index;
            } while (index < r_s32(0x800B54A0u + slot));
        }
        w_u32(0x800B54C0u + slot, r_u32(0x800B54A0u + slot));
    }
}

/* GTMAIN.EXE 80057F7C */
void gtmain_race_clock_tick(void)
{
    if (!r_s16(0x800B63DEu)) {
        uint32 increment = r_u32(0x800B6318u) == 1u ? 50u : 100u;
        w_u32(0x80093D78u, r_u32(0x80093D78u) + increment);
    }
}

/* GTMAIN.EXE 80028BA8 */
sint32 gtmain_race_pad_axis_value(sint32 mapping, uint32 unused_state, uint32 packet, uint32 range, uint32 analog)
{
    sint32 value, minimum, maximum;
    (void)unused_state;
    if (mapping < 16) return 0;
    value = r_u8(packet + ((uint32)mapping & 15u) + 4u);
    minimum = r_u8(range); maximum = r_u8(range + 1u);
    if (value < minimum) value = minimum;
    else if (maximum < value) value = maximum;
    if (maximum == minimum) gt_draft_failure("GTMAIN original pad axis division by zero", 0x80028BA8u);
    value = 15 * (value - minimum) / (maximum - minimum);
    if (analog && maximum < 128) value = 15 - value;
    return value;
}
