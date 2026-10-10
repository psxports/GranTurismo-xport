#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* TODO Bind the remaining original AI dependencies */
static uint32 ai_call(uint32 owner, uint32 a, uint32 b, uint32 c, uint32 d)
{
    return gt_draft_unresolved_call(owner, a, b, c, d, 0, 0, 0, 0);
}

static uint32 ai_fp(uint32 a, uint32 b)
{
    return (uint32)((sint32)(a * b) / 4096);
}

static sint32 ai_div(uint32 numerator, sint32 denominator)
{
    if (!denominator) gt_draft_exception(7);
    if (denominator == -1 && numerator == 0x80000000u) gt_draft_exception(6);
    return (sint32)numerator / denominator;
}

/* GTMAIN.EXE 8005C418 */
sint32 gtmain_race_suspension_angle(uint32 guest_output, sint32 first, sint32 second)
{
    sint32 magnitude, scale = 4096;
    if ((uint32)first + 11380u < 0x58E9u && (uint32)second + 11380u < 0x58E9u) {
        magnitude = (sint32)gtmain_fixed_sqrt(ai_fp((uint32)second, (uint32)second)
            + ai_fp((uint32)first, (uint32)first), 6);
        if (magnitude < 11380) scale = magnitude < 1139 ? 0 : (sint32)ai_fp(1638u, (uint32)magnitude - 1138u);
    }
    w_u16(guest_output, (uint16)scale);
    return scale ? (sint16)gtmain_angle_atan_wide((sint32)(0u - (uint32)second), first) : 0;
}

/* GTMAIN.EXE 800574F8 */
void gtmain_race_lap_report(uint32 unused_car, sint32 lap, uint32 checkpoint, uint32 time, uint32 unused_elapsed, sint32 rank)
{
    uint32 record = 0x800B5480u + 4u * checkpoint, slot, count, lower;
    sint32 position, start;
    sint32 recorded = r_s32(record);
    (void)unused_car; (void)unused_elapsed;
    if (recorded < lap) {
        w_u32(record, (uint32)lap); w_u32(0x800B54A0u + 4u * checkpoint, 1);
        w_u32(0x800B54C0u + 4u * checkpoint, 0); w_u32(0x800B53C0u + 24u * checkpoint, time);
        w_u32(0x800B54E0u + 24u * checkpoint, (uint32)rank); return;
    }
    if (lap != r_s32(record)) return;
    position = r_s32(0x800B54A0u + 4u * checkpoint);
    lower = r_u32(0x800B54C0u + 4u * checkpoint);
    if ((uint32)position != lower) {
        start = r_s32(0x800B54A0u + 4u * checkpoint);
        lower = r_u32(0x800B54C0u + 4u * checkpoint); position = start;
        if ((sint32)lower < start) {
            do {
                if (time >= r_u32(0x800B53C0u + 24u * checkpoint + 4u * (uint32)start - 4u)) break;
                position = (sint32)((uint32)start - 1u); start = position;
            } while ((sint32)lower < start);
        }
        count = r_u32(0x800B54A0u + 4u * checkpoint);
        while (position < (sint32)count) {
            slot = 0x800B53C0u + 24u * checkpoint + 4u * count;
            w_u32(slot, r_u32(slot - 4u)); --count;
        }
    }
    w_u32(0x800B53C0u + 24u * checkpoint + 4u * (uint32)position, time);
    w_u32(0x800B54E0u + 24u * checkpoint + 4u * (uint32)position, (uint32)rank);
    record = 0x800B54A0u + 4u * checkpoint; w_u32(record, r_u32(record) + 1u);
}

/* GTMAIN.EXE 8002D5A8 */
void gtmain_race_input_record_flush(uint32 object)
{
    uint32 offset = r_u16(object + 14u), duration = r_u32(object + 8u), header = r_u8(object + 18u);
    uint32 flags = r_u8(object + 19u), buttons = r_u8(object + 20u), steer = r_u8(object + 21u), axes = r_u8(object + 22u);
    uint32 cursor = object + 23u + offset, prefix_middle = 0, prefix_low = 0, size;
    sint32 encoded;
    header = (header & 248u) | (duration & 7u);
    if ((sint32)duration >= 8) header |= 128u;
    w_u8(cursor++, (uint8)header);
    if (header & 8u) w_u8(cursor++, (uint8)flags);
    if (header & 16u) w_u8(cursor++, (uint8)buttons);
    if (header & 32u) w_u8(cursor++, (uint8)steer);
    if (header & 64u) w_u8(cursor++, (uint8)axes);
    encoded = (sint32)duration >> 3;
    if (header & 128u) {
        if (encoded > 0x1FFFFF) {
            w_u8(cursor++, (uint8)((encoded >> 24) | 224));
            w_u8(cursor++, (uint8)(encoded >> 16));
            w_u8(cursor++, (uint8)(encoded >> 8));
        } else if (encoded >= 0x4000) {
            prefix_middle = 192u; w_u8(cursor++, (uint8)((uint32)(encoded >> 16) | prefix_middle));
            w_u8(cursor++, (uint8)(encoded >> 8));
        } else if (encoded >= 128) {
            prefix_low = 128u; w_u8(cursor++, (uint8)((uint32)(encoded >> 8) | prefix_low));
        }
        w_u8(cursor++, (uint8)encoded);
    }
    size = cursor - (object + 23u); w_u16(object + 14u, (uint16)size); w_u16(object + 16u, (uint16)size);
    if ((sint32)size >= 8184) w_u16(object + 12u, 1);
}

/* GTMAIN.EXE 80055068 */
void gtmain_race_course_events(uint32 car, sint32 current, sint32 previous)
{
    uint32 table = r_u32(r_u32(0x800BF51Cu) + 24u), rows, count, flags, type, wheel, contacts, surface;
    sint32 length, current_normal = current, previous_normal = previous, lap_limit, target, first, second;
    uint32 finished = 0;
    if (!table) return;
    length = r_s32(r_u32(0x800BC1A8u)); count = r_u32(table); rows = table + 4u;
    if (current < length / 2) current_normal = (sint32)((uint32)current + (uint32)length);
    if (previous < length / 2) previous_normal = (sint32)((uint32)previous + (uint32)length);
    if (r_u16(car + 12u)) gtmain_race_control_latch(car, 11, 0);
    flags = r_u8(car + 58u);
    if (flags & 1u) {
        if (r_s8(car + 1689u) == 2 || r_u8(car + 1696u) || !(flags & 16u)) return;
        if (r_s32(rows + 140u) >= current_normal || current_normal >= r_s32(rows + 40u * count - 60u)) return;
        target = r_s32(rows + 40u * count - 100u);
        if (target < previous_normal && target >= current_normal) {
            gtmain_race_behavior_set(car, 4); gtmain_race_control_latch(car, 12, 2u * r_u32(0x800BF518u)); return;
        }
        if ((r_u32(car + 1724u) + 5690u >= 0x2C75u || r_u32(car + 1728u) + 5690u >= 0x2C75u) && !r_u8(car + 1698u))
            gtmain_race_control_latch(car, 2, 0);
        return;
    }
    if (r_s8(car + 1689u) && !(r_u8(car + 58u) & 8u)) return;
    lap_limit = r_s16(0x800B634Cu);
    if (lap_limit) {
        sint32 lap = r_s16(car + 56u);
        uint32 half = r_u32(r_u32(0x800BC1A8u)) >> 1;
        if (r_u32(car + 1804u) >= half ? lap >= lap_limit : lap >= lap_limit + 1)
            if (!(r_u8(car + 58u) & 2u)) finished = 1;
    }
    type = r_u8(car + 1696u);
    if (type == 3u) {
        if (r_u16(car + 12u)) {
            if (r_u16(car + 12u) == r_u32(0x800BF518u)) gtmain_race_wheel_surface_reset(car);
            return;
        }
        contacts = 0; surface = r_u32(0x80093E3Cu);
        for (wheel = 0; wheel < 4u; ++wheel) if (r_u32(car + 168u + 68u * wheel) != surface) ++contacts;
        target = r_s32(rows + 40u * (uint32)r_s8(car + 1695u) + 20u);
        if (contacts && previous_normal < target && current_normal >= target) {
            gtmain_race_vehicle_reset(car);
            if (r_s8(0x800B6310u)) w_u16(car + 12u, 1);
            else { w_u16(car + 12u, (uint16)(5u * r_u32(0x800BF518u))); gtmain_race_control_latch(car, 11, 0); }
        } else if (current_normal >= (sint32)((uint32)target + 0x10000u)) {
            gtmain_race_behavior_set(car, 4); w_u8(car + 58u, (uint8)(r_u8(car + 58u) & 251u));
        }
        return;
    }
    if (type == 2u) {
        if (finished || !r_u32(0x80093E30u)) {
            gtmain_race_behavior_set(car, 4); gtmain_race_control_latch(car, finished ? 8 : 9, 2u * r_u32(0x800BF518u)); return;
        }
        target = (sint32)(r_u32(rows + 40u * (uint32)r_s8(car + 1695u) + 20u) - 1310720u);
        if (previous_normal < target && current_normal >= target) {
            if (r_s32(car + 1724u) > 125179) {
                gtmain_race_behavior_set(car, 4);
                if (!finished) gtmain_race_control_latch(car, 7, 2u * r_u32(0x800BF518u));
            } else gtmain_race_behavior_set(car, 3);
            return;
        }
        if (!finished && r_u32(0x80093E30u)) gtmain_race_control_latch(car, 10, 0);
        if (!r_s8(car + 1689u) && !(r_u8(car + 58u) & 16u)) {
            gtmain_race_behavior_set(car, 0); gtmain_race_control_latch(car, 0, 0);
        }
        return;
    }
    if (type == 4u) {
        if (!r_s8(car + 1689u) && !(r_u8(car + 58u) & 16u) && current_normal < length) {
            gtmain_race_behavior_set(car, 0); gtmain_race_control_latch(car, 0, 0);
        }
        if (r_s8(car + 1689u) || r_u8(car + 1960u)) {
            first = r_s32(rows + 40u * count - 20u); second = r_s32(rows + 40u * count - 60u);
        } else { first = r_s32(rows + 40u * count - 60u); second = r_s32(rows + 40u * count - 100u); }
        if (previous_normal < first && current_normal >= first) {
            if (r_u8(car + 1960u)) gtmain_race_finish_mark(car);
            else { gtmain_race_behavior_set(car, 0); if (!finished) gtmain_race_control_latch(car, 6, 2u * r_u32(0x800BF518u)); }
            w_u8(car + 58u, (uint8)(r_u8(car + 58u) & 247u)); return;
        }
        if (current_normal >= second) {
            if (!finished) gtmain_race_control_latch(car, (r_u8(car + 1966u) & 15u) == 15u ? 5 : 4, 0);
        } else if (!finished && r_u32(0x80093E30u)) gtmain_race_control_latch(car, 10, 0);
        return;
    }
    if (r_s8(car + 1689u)) {
        if (finished) return;
        if (r_s8(car + 1689u)) target = r_s32(rows + 20u);
        else target = r_s32(rows + 60u);
    } else {
        if (!(r_u8(car + 58u) & 16u)) return;
        target = r_s32(rows + 60u);
    }
    if (previous_normal < target && current_normal >= target) { gtmain_race_behavior_set(car, 2); return; }
    if (!(r_u8(car + 58u) & 16u) || !r_s8(car + 1689u)) return;
    target = r_s32(rows + 60u);
    if (previous_normal < target && current_normal >= target) gtmain_race_behavior_set(car, 2);
}

/* GTMAIN.EXE 80055D50 */
void gtmain_race_controls_ai_update(uint32 car, sint32 index)
{
    sint32 drive = 0, braking = 0, oversteer = 0, sign = 1, steering = 0;
    sint32 direction_x, direction_z, sine, cosine, angle, absolute_angle, mode, force, limit;
    sint32 minimum, absolute_x, absolute_z, speed_estimate, coefficient, delta, next;
    uint32 distance, table_index, numerator, row, rows, count, type, allow_changes, ai_mode, flags;
    sint32 first_speed, second_speed, control_mode, target, change;
    if (r_u8(car + 1696u) - 6u < 2u) goto steering_publish;
    if (r_u8(car + 1912u) != 4u) gtmain_race_ai_segment_update(car);
    if (r_s8(car + 1690u) || r_u32(0x80093D78u) < 6000u) { drive = 1; goto steering_publish; }
    table_index = r_u16(car + 1790u) & 4095u;
    sine = -r_s16(0x8009423Cu + 2u * table_index); cosine = r_s16(0x80094A3Cu + 2u * table_index);
    if (r_u8(car + 1696u) == 3u) distance = 0x80000u;
    else if (r_u8(car + 1696u) == 4u && r_u8(car + 1967u)) distance = 0xA0000u;
    else distance = (uint32)gtmain_race_ai_distance(r_u32(car + 1708u), r_s32(car + 1724u));
    gtmain_race_ai_direction(car, (sint32)distance, &direction_x, &direction_z);
    angle = gtmain_race_angle_delta(gtmain_angle_atan_wide((sint32)(0u - (uint32)direction_x), direction_z), r_s16(car + 1790u));
    absolute_angle = angle < 0 ? -angle : angle; mode = r_u8(car + 1780u);
    if (mode == 1) {
        if (absolute_angle >= 341) { if (r_u8(car + 33u) & 12u) w_u8(car + 1780u, 2); }
        else if (r_s32(car + 1724u) < 5691) w_u8(car + 1780u, 2);
        else w_u8(car + 1780u, 0);
    } else if (mode == 0) { if (absolute_angle >= 683) w_u8(car + 1780u, 1); }
    else if (mode == 2) {
        if (absolute_angle < 341 && r_s32(car + 1724u) >= 5691) w_u8(car + 1780u, 0);
        else if (r_u8(car + 33u) & 3u) w_u8(car + 1780u, 1);
    }
    mode = r_u8(car + 1780u);
    if (mode && mode != 2 && absolute_angle >= 341) {
        sign = -1;
        if (r_s32(car + 1724u) < 2277) drive = -1;
        else { sign = 1; drive = 0; braking = 1; }
    } else {
        sign = -1;
        if (r_s32(car + 1724u) < -2275) { drive = 0; braking = 1; }
        else { sign = 1; drive = 1; }
    }
    if (drive == 1) {
        sint64 product_x = (sint64)direction_x * direction_x, product_z = (sint64)direction_z * direction_z;
        uint32 squared = (uint32)(product_x / 4096) + (uint32)(product_z / 4096);
        sint32 raw_x = r_s32(car + 1712u), raw_z = r_s32(car + 1716u);
        absolute_x = (sint32)(0u - (uint32)raw_x); if (absolute_x < 0) absolute_x = raw_x;
        absolute_z = (sint32)(0u - (uint32)raw_z); if (absolute_z < 0) absolute_z = (sint32)(0u - (uint32)absolute_z);
        minimum = absolute_x < absolute_z ? absolute_x : absolute_z;
        speed_estimate = (sint32)((uint32)absolute_x + (uint32)absolute_z - (uint32)(minimum >> 1));
        if (speed_estimate >= 5690) {
            table_index = (uint32)gtmain_angle_atan_wide((sint32)(0u - r_u32(car + 1712u)), r_s32(car + 1716u)) & 4095u;
            sine = -r_s16(0x8009423Cu + 2u * table_index); cosine = r_s16(0x80094A3Cu + 2u * table_index);
        }
        delta = (sint32)(ai_fp((uint32)cosine, (uint32)direction_x) - ai_fp((uint32)sine, (uint32)direction_z));
        product_x = (sint64)delta * speed_estimate;
        delta = (sint32)(uint32)(product_x / 4096);
        product_x = gtmain_signed_divide64((sint64)delta * 4096, (sint32)squared, 0x80056180u);
        product_x = (sint64)(sint32)(uint32)product_x * 1304;
        force = (sint32)(uint32)(product_x / 4096);
        if (r_s32(car + 1724u) >= 0) force = (sint32)(0u - (uint32)force);
        coefficient = 0;
        if ((force > 0 && force >= r_s32(car + 1792u)) || (force < 0 && r_s32(car + 1792u) >= force)) {
            coefficient = (sint32)ai_fp((uint32)force - r_u32(car + 1792u), (uint32)r_s16(r_u32(car + 1708u) + 8u));
            limit = r_s16(car + 496u);
            if (force > 0 && coefficient > limit) {
                oversteer = ai_div((uint32)coefficient << 12, limit); coefficient = r_s16(car + 496u);
            } else if (force < 0 && coefficient < -limit) {
                oversteer = ai_div((0u - (uint32)coefficient) << 12, limit); coefficient = -limit;
            }
        }
        steering = (sint32)((uint32)coefficient + (uint32)r_s16(car + 1740u));
        if (steering > 512) steering = 512; else if (steering < -512) steering = -512;
    } else if (angle < -227) steering = -512;
    else if (angle >= 228) steering = 512;
    else steering = (sint32)ai_fp(9238u, (uint32)angle);
    if (sign == -1) steering = -steering;
steering_publish:
    if (r_s8(car + 1689u) == 2 || r_u8(car + 1696u)) {
        next = r_s16(car + 14u);
        if (next < steering) {
            numerator = (uint32)r_s16(car + 498u) * r_u32(0x1F800000u);
            if ((sint32)numerator < 0) numerator += 65535u;
            next = (sint16)(r_u16(car + 14u) + (uint32)((sint32)numerator >> 16)); w_u16(car + 14u, (uint16)next);
            if (steering < next) w_u16(car + 14u, (uint16)steering);
        } else if (steering < r_s16(car + 14u)) {
            numerator = (uint32)r_s16(car + 498u) * r_u32(0x1F800000u);
            if ((sint32)numerator < 0) numerator += 65535u;
            next = (sint16)(r_u16(car + 14u) - (uint32)((sint32)numerator >> 16)); w_u16(car + 14u, (uint16)next);
            if (next < steering) w_u16(car + 14u, (uint16)steering);
        }
    }
    if (r_u8(car + 1696u) == 7u) {
        w_u16(car + 16u, 0); w_u16(car + 20u, 0); w_u16(car + 18u, 0); w_u16(car + 22u, 0); w_u16(car + 24u, 0); return;
    }
    if (r_u8(car + 1696u) - 5u < 2u) {
        flags = r_u32(car + 1724u); w_u16(car + 16u, 0); w_u16(car + 18u, 0);
        if (flags + 1137u >= 0x8E3u || r_u32(car + 1728u) + 1137u >= 0x8E3u) {
            w_u16(car + 20u, 4096); w_u16(car + 22u, 4096); w_u16(car + 24u, 4096); gtmain_race_brake_assist(car);
        } else {
            w_u16(car + 20u, 0); w_u16(car + 22u, 0); w_u16(car + 24u, 0); gtmain_race_behavior_set(car, 7);
        }
        return;
    }
    w_u16(car + 18u, 0); w_u16(car + 22u, 0); w_u16(car + 24u, 0); w_u16(car + 16u, 0); w_u16(car + 20u, 0);
    if (drive != 1) {
        w_u8(0x1F8001FCu + 4u * (uint32)index, drive == -1 ? 1 : 0); w_u16(car + 18u, drive == -1 ? 4096 : 0);
        w_u16(car + 24u, braking ? 4096 : 0); w_u16(car + 22u, braking ? 4096 : 0);
        flags = r_u16(car + 18u); type = r_u16(car + 22u); w_u16(car + 16u, (uint16)flags); w_u16(car + 20u, (uint16)type);
        gtmain_race_brake_assist(car); goto oversteer_apply;
    }
    allow_changes = 0;
    if (r_u32(car + 1724u) + 1137u >= 0x8E3u)
        gtmain_race_angle_delta(gtmain_angle_atan_wide((sint32)(0u - r_u32(car + 1712u)), r_s32(car + 1716u)), r_s16(car + 1790u));
    ai_mode = r_u8(car + 1912u); w_u8(car + 1967u, 0);
    if (ai_mode == 4u) { first_speed = second_speed = 22760; control_mode = 3; w_u8(car + 1967u, 1); }
    else if (r_u8(car + 1912u) != 3u) {
        allow_changes = 1; first_speed = r_s32(car + 1916u); second_speed = r_s32(car + 1920u); control_mode = r_u8(car + 1914u);
    } else {
        sint32 progress = r_s32(car + 1804u), course_length;
        row = r_u32(r_u32(0x800BF51Cu) + 24u); rows = row + 4u; course_length = r_s32(r_u32(0x800BC1A8u)); count = r_u32(row);
        if (progress < course_length / 2) progress = (sint32)((uint32)progress + (uint32)course_length);
        target = r_s16(0x800B634Cu);
        if (target) {
            uint32 progress_raw = r_u32(car + 1804u), half = r_u32(r_u32(0x800BC1A8u)) >> 1;
            sint32 lap = r_s16(car + 56u);
            if ((progress_raw >= half ? lap >= target : lap >= target + 1) && !(r_u8(car + 58u) & 2u)) goto ordinary_speed;
        }
        if (r_u32(0x80093E30u)) {
            if (progress < r_s32(rows + 60u)) {
                first_speed = 138836; if (r_s32(car + 1916u) < first_speed) first_speed = r_s32(car + 1916u);
                second_speed = 138836; if (r_s32(car + 1920u) < second_speed) second_speed = r_s32(car + 1920u);
                control_mode = 4; goto speed_ready;
            }
            if (progress < r_s32(rows + 100u) || progress < r_s32(rows + 40u * count - 100u)) {
                first_speed = second_speed = 116076; control_mode = 3; w_u8(car + 1967u, 1); goto speed_ready;
            }
        }
ordinary_speed:
        first_speed = r_s32(car + 1916u); second_speed = r_s32(car + 1920u); control_mode = r_u8(car + 1914u);
    }
speed_ready:
    if (control_mode == 0) w_u16(car + 18u, 4096);
    else if (control_mode == 1 || control_mode == 3 || control_mode == 4) {
        sint32 speed = r_s32(car + 1724u), reference = control_mode == 1 ? first_speed : second_speed;
        change = control_mode == 1 ? -2048 : control_mode == 3 ? -8192 : -4096;
        next = (sint16)(uint32)gtmain_race_speed_control(speed, (sint32)((uint32)reference - 2276u), 3072, change);
        w_u16(car + 18u, (uint16)next); w_u8(car + 1699u, next != 4096 && next != 0);
        speed = r_s32(car + 1724u);
        if (reference < speed && !r_s16(car + 18u)) {
            change = control_mode == 1 ? -8192 : control_mode == 3 ? 0 : -2048;
            numerator = (uint32)gtmain_race_speed_control(speed, (sint32)((uint32)second_speed - 2276u), change, 4096) * r_u16(car + 1898u);
            if ((sint32)numerator < 0) numerator += 4095u;
            numerator >>= 12; w_u16(car + 24u, (uint16)numerator); w_u16(car + 22u, (uint16)numerator);
        }
    }
    ai_mode = r_u8(car + 1912u);
    if (ai_mode == 3u) {
        if (r_s8(car + 1815u) && r_s8(car + 1813u)) w_u16(car + 18u, (uint16)(r_s16(car + 18u) / 2));
        goto controls_publish;
    }
    if (r_u32(car + 1808u)) {
        if (!allow_changes) goto controls_publish;
        ai_mode = r_u8(car + 1912u);
        if (!r_s8(car + 1813u)) {
            change = (sint32)(uint32)gtmain_race_ai_mode_right((sint32)ai_mode); if (change != -1) goto change_mode;
        }
        if (!r_s8(car + 1814u)) {
            change = (sint32)(uint32)gtmain_race_ai_mode_left((sint32)ai_mode); if (change != -1) goto change_mode;
        }
    }
    if (!allow_changes) goto controls_publish;
    if (!(r_s8(car + 1812u) + r_s8(car + 1813u) + r_s8(car + 1814u) + r_s8(car + 1815u) + r_s8(car + 1816u)))
        change = (sint32)(uint32)gtmain_race_ai_mode_neutral(r_u8(car + 1912u));
    else {
        sint32 right = r_s8(car + 1815u), left = r_s8(car + 1816u);
        if ((!right && !left) || (right && left)) goto controls_publish;
        ai_mode = r_u8(car + 1912u);
        if (right) {
            change = (sint32)(uint32)gtmain_race_ai_mode_left((sint32)ai_mode); if (change != -1) goto change_mode;
        }
        if (!left) goto controls_publish;
        change = (sint32)(uint32)gtmain_race_ai_mode_right((sint32)ai_mode);
    }
    if (change == -1) goto controls_publish;
change_mode:
    gtmain_race_ai_mode_set(car, (uint8)change);
controls_publish:
    flags = r_u16(car + 18u); type = r_u16(car + 22u); w_u16(car + 16u, (uint16)flags); w_u16(car + 20u, (uint16)type);
    gtmain_race_throttle_assist(car); gtmain_race_brake_assist(car);
    if (r_s8(car + 1689u) == 2 || r_u8(car + 1696u)) w_u8(0x1F8001FCu + 4u * (uint32)index, 0);
oversteer_apply:
    if (oversteer && r_u8(car + 32u) >= 2u) {
        delta = (sint32)((uint32)oversteer - 4096u);
        coefficient = (sint32)(4096u - ai_fp((uint32)r_s16(r_u32(car + 1708u) + 10u), (uint32)delta));
        if (coefficient < 0) coefficient = 0;
        next = (sint32)ai_fp((uint32)coefficient, (uint32)r_s16(car + 18u)); row = r_u32(car + 1708u);
        w_u16(car + 18u, (uint16)next); w_u16(car + 16u, (uint16)next);
        next = (sint32)(r_u16(car + 22u) + ai_fp((uint32)r_s16(row + 12u), (uint32)delta));
        w_u16(car + 22u, (uint16)next); if ((sint16)next >= 4097) w_u16(car + 22u, 4096);
        next = r_u16(car + 22u); w_u16(car + 20u, (uint16)next); w_u16(car + 24u, (uint16)next); gtmain_race_brake_assist(car);
    }
    if (!r_u8(car + 1460u) && ((r_s16(car + 14u) >= 23 && r_s32(car + 1792u) < -227)
        || (r_s16(car + 14u) < -22 && r_s32(car + 1792u) >= 228))) {
        next = r_s16(car + 14u); if (next < 0) next = -next;
        coefficient = next < 228 ? (sint32)((uint32)next << 12) / 227 : 4096;
        next = (sint32)ai_fp((uint32)(4096 - coefficient / 2), (uint32)r_s16(car + 18u));
        w_u16(car + 18u, (uint16)next); w_u16(car + 16u, (uint16)next);
    }
}

