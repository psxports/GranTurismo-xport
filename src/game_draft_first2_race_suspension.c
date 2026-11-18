#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* TODO Bind remaining original suspension and lap dependencies */
static uint32 suspension_call(uint32 owner, uint32 a, uint32 b, uint32 c, uint32 d)
{
    if (owner == 0x80056EBCu) return gtmain_race_objective_mode2_time(a);
    if (owner == 0x80056EF8u) return gtmain_race_objective_mode3_time(a);
    if (owner == 0x80056F44u) return gtmain_race_objective_mode4_time(a, b);
    return gt_draft_unresolved_call(owner, a, b, c, d, 0, 0, 0, 0);
}

static sint32 suspension_div(uint32 numerator, sint32 denominator)
{
    if (!denominator) gt_draft_exception(7);
    if (denominator == -1 && numerator == 0x80000000u) gt_draft_exception(6);
    return (sint32)numerator / denominator;
}

static uint32 suspension_fp(uint32 a, uint32 b)
{
    return (uint32)((sint32)(a * b) / 4096);
}

/* GTMAIN.EXE 80067D48 */
/* Original: GTMAIN.EXE:sub_80067D48. */ sint32 gtmain_race_curve16(uint32 curve, sint32 value)
{
    uint16 count = r_u16(curve), low = 0, high = (uint16)(count - 1u), index;
    uint32 xs = r_u32(curve + 4u), ys;
    sint32 last = (sint16)r_u16(xs + 2u * high), first = r_s16(xs);
    sint32 first_again = (sint16)r_u16(xs), x0, x1, y0, y1;
    uint32 numerator;
    value = (sint16)value;
    if (first >= value) {
        ys = r_u32(curve + 8u); x1 = r_s16(xs + 2u);
        y0 = r_u16(ys); y1 = r_s16(ys + 2u);
        if (value == first || !r_s8(curve + 2u)) return (sint16)y0;
        numerator = suspension_fp((uint32)((sint16)y0 - y1), (uint32)(value - x1)) << 12;
        return (sint16)(y1 + suspension_div(numerator, first_again - x1));
    }
    if (value >= last) {
        uint32 offset = 2u * high;
        xs = r_u32(curve + 4u); ys = r_u32(curve + 8u);
        x0 = r_s16(xs + offset - 2u); y0 = r_s16(ys + offset - 2u); y1 = r_u16(ys + offset);
        if (value == last || !r_s8(curve + 3u)) return (sint16)y1;
        numerator = suspension_fp((uint32)((sint16)y1 - y0), (uint32)(value - x0)) << 12;
        return (sint16)(y0 + suspension_div(numerator, last - x0));
    }
    while ((sint32)high - low >= 4) {
        uint32 mid = ((uint32)high + low) >> 1;
        sint32 at = r_s16(r_u32(curve + 4u) + 2u * mid);
        if (value >= at) {
            if (value == r_s16(r_u32(curve + 4u) + 2u * (uint16)mid))
                return r_s16(r_u32(curve + 8u) + 2u * (uint16)mid);
            low = (uint16)mid;
        } else high = (uint16)mid;
    }
    if (low == high) return r_s16(r_u32(curve + 8u) + 2u * low);
    index = low;
    while (index < high && value >= r_s16(r_u32(curve + 4u) + 2u * index)) ++index;
    xs = r_u32(curve + 4u) + 2u * index; ys = r_u32(curve + 8u) + 2u * index;
    y0 = r_u16(ys - 2u); x1 = r_s16(xs); x0 = r_s16(xs - 2u);
    if (value == x0) return (sint16)y0;
    numerator = (suspension_fp((uint32)(value - x0), (uint32)r_s16(ys))
        + suspension_fp((uint32)(x1 - value), (uint32)(sint16)y0)) << 12;
    return (sint16)suspension_div(numerator, x1 - x0);
}

/* GTMAIN.EXE 8005C0C0 */
/* Original: GTMAIN.EXE:sub_8005C0C0. */ uint32 gtmain_race_suspension_car_prepare(uint32 car)
{
    uint32 force[4], rate[4], enabled, i;
    sint32 delta = r_s16(car + 1788u) - r_s16(car + 1902u), value;
    value = r_s16(car + 1456u) - delta; rate[0] = (uint32)(value < 0 ? -value : value);
    value = r_s16(car + 1456u) + delta; rate[1] = (uint32)(value < 0 ? -value : value);
    value = r_s16(car + 1458u) - delta; rate[2] = (uint32)(value < 0 ? -value : value);
    value = r_s16(car + 1458u) + delta; rate[3] = (uint32)(value < 0 ? -value : value);
    for (i = 0; i < 4u; ++i) rate[i] = (uint32)(sint16)gtmain_race_curve16(car + (i < 2u ? 1396u : 1424u), (sint16)rate[i]);
    force[0] = r_u32(car + 152u); enabled = r_u32(0x80093E30u);
    force[1] = r_u32(car + 220u); force[2] = r_u32(car + 288u); force[3] = r_u32(car + 356u);
    if (enabled && !(r_u8(car + 58u) & 16u))
        for (i = 0; i < 4u; ++i) force[i] = suspension_fp((uint32)r_s16(car + 172u + 68u * i), force[i]);
    for (i = 0; i < 4u; ++i) w_u32(car + 160u + 68u * i, force[i]);
    for (i = 0; i < 4u; ++i) force[i] = (uint32)gtmain_race_curve32(car + (i < 2u ? 1308u : 1352u), (sint32)force[i]);
    for (i = 0; i < 4u; ++i) force[i] = suspension_fp(force[i], rate[i]);
    for (i = 0; i < 4u; ++i)
        w_u32(car + 156u + 68u * i, suspension_fp((uint32)r_s16(0x80093E4Cu + 2u * r_u8(car + 176u + 68u * i)), force[i]));
    return 64u;
}

/* GTMAIN.EXE 8005C514 */
/* Original: GTMAIN.EXE:sub_8005C514. */ void gtmain_race_suspension_car_update(uint32 car)
{
    uint32 coefficient = suspension_fp(r_u32(car + 1792u), 25736u), result, first, second;
    w_u32(car + 1932u, r_u32(car + 1728u) - suspension_fp(coefficient, (uint32)r_s16(car + 416u)));
    result = suspension_fp(coefficient, (uint32)r_s16(car + 418u));
    first = r_u32(car + 1732u); second = r_u32(car + 1932u);
    w_u32(car + 1936u, result + r_u32(car + 1728u));
    result = (uint32)gtmain_race_suspension_angle(car + 1776u, (sint32)first, (sint32)second);
    first = r_u32(car + 1736u); second = r_u32(car + 1936u);
    w_u16(car + 1740u, (uint16)result);
    result = (uint32)gtmain_race_suspension_angle(car + 1778u, (sint32)first, (sint32)second);
    w_u16(car + 1742u, (uint16)result);
}

/* GTMAIN.EXE 8002D424 */
/* Original: GTMAIN.EXE:sub_8002D424. */ void gtmain_race_input_playback(uint32 object, uint32 state)
{
    uint32 count, cursor, duration, axes, buttons, flags, steer, header, old_buttons;
    sint32 previous;
    if (!object) return;
    count = r_u32(object + 4u) - 1u;
    if (r_u16(object + 12u)) return;
    w_u32(object + 4u, count);
    if (!count) { w_u16(object + 12u, 1); w_u32(object + 8u, 0); return; }
    flags = r_u8(object + 19u); buttons = r_u8(object + 20u);
    steer = r_u8(object + 21u); axes = r_u8(object + 22u);
    previous = r_s32(object + 8u); duration = (uint32)previous - 1u;
    if (previous <= 0) {
        cursor = object + 23u + r_u16(object + 14u); header = r_u8(cursor++); w_u8(object + 18u, (uint8)header);
        if (header & 8u) { flags = r_u8(cursor++); w_u8(object + 19u, (uint8)flags); }
        if (header & 16u) { buttons = r_u8(cursor++); w_u8(object + 20u, (uint8)buttons); }
        if (header & 32u) { steer = r_u8(cursor++); w_u8(object + 21u, (uint8)steer); }
        if (header & 64u) { axes = r_u8(cursor++); w_u8(object + 22u, (uint8)axes); }
        duration = 0;
        if (header & 128u) {
            duration = r_u8(cursor++);
            if (duration & 128u) {
                duration = ((duration & 127u) << 8) | r_u8(cursor++);
                if (duration & 0x4000u) {
                    duration = ((duration & 0x3FFFu) << 8) | r_u8(cursor++);
                    if (duration & 0x200000u) duration = ((duration & 0x1FFFFFu) << 8) | r_u8(cursor++);
                }
            }
            duration <<= 3;
        }
        duration |= header & 7u; w_u16(object + 14u, (uint16)(cursor - (object + 23u)));
    }
    w_u32(object + 8u, duration); w_u8(state + 84u, (uint8)(axes & 15u));
    old_buttons = r_u16(state + 80u); w_u8(state + 82u, (uint8)flags); w_u8(state + 83u, (uint8)steer);
    w_u8(state + 85u, (uint8)(axes >> 4)); w_u16(state + 80u, (uint16)((old_buttons & 0xFF00u) | buttons));
}

/* GTMAIN.EXE 80055780 */
/* Original: GTMAIN.EXE:sub_80055780. */ sint32 gtmain_race_brake_assist(uint32 car)
{
    uint32 axle, wheel, half;
    sint32 maximum, coefficient, value = 0;
    for (axle = 0; axle < 2u; ++axle) {
        half = 22u + 2u * axle;
        if (!r_u16(car + half)) continue;
        maximum = -4096;
        for (wheel = 2u * axle; wheel < 2u * axle + 2u; ++wheel)
            if (r_u8(car + 132u + 68u * wheel)) {
                value = r_s16(car + 180u + 68u * wheel); if (value > maximum) maximum = value;
            }
        coefficient = (sint32)(r_u32(0x80093DD0u + 4u * axle)
            - suspension_fp((uint32)(maximum - r_s16(car + 1002u + 64u * axle)), r_u32(car + 1636u + 4u * axle)));
        if (coefficient < 0) coefficient = 0;
        if (coefficient > 4096) coefficient = 4096;
        value = (sint32)suspension_fp((uint32)coefficient, (uint32)r_s16(car + half));
        w_u16(car + half, (uint16)value);
        if (!axle) value = 0;
    }
    return value;
}

/* GTMAIN.EXE 80055A64 */
/* Original: GTMAIN.EXE:sub_80055A64. */ uint32 gtmain_race_controls_launch(uint32 car, sint32 index, sint32 time)
{
    uint32 value;
    w_u8(0x1F8001FDu + 4u * (uint32)index, 0);
    if (time > r_s32(0x800BF518u)) {
        if (r_u8(car + 1707u)) {
            if (r_s16(car + 38u) >= (sint32)((r_u16(car + 660u) + r_u16(car + 1470u)) >> 1)) w_u8(car + 1707u, 0);
        } else if (r_s16(car + 38u) < (sint32)(2u * r_u16(car + 1470u) - r_u16(car + 660u))) w_u8(car + 1707u, 1);
        w_u16(car + 16u, r_u8(car + 1707u) ? 4096 : 0);
    } else {
        value = (uint32)(r_s16(car + 38u) - r_u16(car + 1470u)) << 13;
        w_u16(car + 16u, (uint16)(2560u - (uint32)suspension_div(value, r_u16(car + 660u))));
        if (r_s16(car + 16u) < 0) w_u16(car + 16u, 0);
        else if (r_s16(car + 16u) > 4096) w_u16(car + 16u, 4096);
    }
    value = r_u16(car + 16u); w_u16(car + 20u, 0); w_u16(car + 24u, 0); w_u16(car + 22u, 0);
    w_u16(car + 14u, 0); w_u16(car + 18u, (uint16)value);
    return value;
}

/* GTMAIN.EXE 80055928 */
/* Original: GTMAIN.EXE:sub_80055928. */ sint32 gtmain_race_throttle_assist(uint32 car)
{
    uint32 type, wheel;
    sint32 minimum = 4096, value, front, rear, coefficient;
    if (r_u8(car + 1691u) != 1u) return 4;
    type = r_u8(car + 1460u);
    if (type == 4u) return 2;
    if (type == 2u || type == 3u) return 3;
    value = r_u16(car + 1470u) >> 1;
    if (r_s16(car + 38u) < value) return value;
    if (!r_s16(car + 18u)) return 0;
    for (wheel = 0; wheel < 4u; ++wheel) if (r_u8(car + 132u + 68u * wheel)) {
        value = r_s16(car + 180u + 68u * wheel); if (value < minimum) minimum = value;
    }
    front = r_s16(car + 1000u); rear = r_s16(car + 1064u); if (rear < front) front = rear;
    coefficient = (sint32)(suspension_fp((uint32)(minimum - front), r_u32(0x80093DCCu)) + r_u32(0x80093DC8u));
    if (coefficient < 0) coefficient = 0;
    if (coefficient > 4096) coefficient = 4096;
    value = (sint32)suspension_fp((uint32)coefficient, (uint32)r_s16(car + 18u));
    w_u16(car + 18u, (uint16)value);
    return value;
}

static uint32 suspension_lap_ratio(uint32 difference, uint32 span)
{
    return (uint32)suspension_div(difference * (r_u32(0x800B6318u) == 1u ? 50u : 100u), (sint32)span);
}

static void suspension_lap_report(uint32 car, sint32 lap, sint32 time, uint32 elapsed, sint32 rank)
{
    gtmain_race_lap_report(car, lap, r_u8(car + 59u), (uint32)time, elapsed, rank);
}

/* GTMAIN.EXE 800576D4 */
/* Original: GTMAIN.EXE:sub_800576D4. */ sint32 gtmain_race_lap_crossing(uint32 car, sint32 previous, sint32 rank)
{
    sint32 length, lap = 0, current, target, time;
    uint32 index, ratio, tick, elapsed, old, flags;
    if (r_u8(0x800CB7F5u) == 4u && r_u16(0x800BF520u) == 4u) length = 0x7C0000;
    else length = r_s32(r_u32(0x800BC1A8u));
    if (r_s16(car + 56u)) lap = r_s16(car + 56u) - 1;
    current = r_s32(car + 1804u);
    if (current < previous) {
        if ((sint32)((uint32)(length / 2) + r_u32(car + 1804u)) >= previous) return 1;
        flags = r_u8(car + 58u);
        if (flags & 2u) {
            sint32 actual_lap = r_s16(car + 56u); w_u8(car + 58u, (uint8)(flags & ~2u));
            if (!actual_lap) w_u16(car + 56u, 1); return 0;
        }
        if (!r_u16(car + 56u)) { w_u16(car + 56u, 1); return 0; }
        ratio = suspension_lap_ratio((uint32)length - (uint32)previous, (uint32)length - (uint32)previous + r_u32(car + 1804u));
        if (!r_u8(car + 1960u)) {
            tick = ratio + r_u32(0x80093D78u);
            old = r_u32(car + 44u); time = (sint32)tick / 3;
            w_u32(car + 44u, (uint32)time); elapsed = (uint32)time - old;
            if (r_u8(0x80093C0Cu)) suspension_lap_report(car, lap, time, elapsed, rank);
            if (r_u8(0x80093C0Bu)) gtmain_race_lap_record(rank, r_s16(car + 56u), (uint32)time, elapsed);
            if (r_u8(0x800CB7F5u) == 4u) {
                suspension_call(0x80056EF8u, elapsed, 0, 0, 0);
                suspension_call(0x80056F44u, (uint32)r_s16(car + 56u), (uint32)time, 0, 0);
            }
        }
        old = r_u16(car + 56u); w_u8(car + 59u, 0); w_u16(car + 56u, (uint16)(old + 1u));
        if ((sint16)(old + 1u) >= 10001) w_u16(car + 56u, 10000);
        return 0;
    }
    if ((sint32)((uint32)previous + (uint32)(length / 2)) < current) {
        w_u8(car + 58u, (uint8)(r_u8(car + 58u) | 2u)); return 1;
    }
    if (r_u8(0x80093C0Du)) {
        index = r_u8(car + 1781u);
        if ((sint32)index < r_s16(0x8009071Eu)) {
            target = (sint32)((uint32)r_s16(0x80090720u + 2u * index) << 16);
            if (!(r_u8(car + 58u) & 2u) && previous < target && (current = r_s32(car + 1804u)) >= target) {
                ratio = suspension_lap_ratio((uint32)target - (uint32)previous, (uint32)current - (uint32)previous);
                tick = ratio + r_u32(0x80093D78u);
                index = r_u8(car + 1781u);
                old = r_u32(car + 44u);
                elapsed = tick / 3u - old;
                gtmain_race_split_record(rank, index, elapsed);
                w_u8(car + 1781u, (uint8)(r_u8(car + 1781u) + 1u));
            }
        }
    }
    index = r_u8(car + 59u);
    uint32 checkpoints = r_u32(0x800BC1B0u);
    flags = r_u8(car + 58u);
    target = r_s32(checkpoints + 32u + 4u * index);
    if (!(flags & 2u) && previous < target && (current = r_s32(car + 1804u)) >= target) {
        if (!r_u8(car + 1960u)) {
            ratio = suspension_lap_ratio((uint32)target - (uint32)previous, (uint32)current - (uint32)previous);
            tick = ratio + r_u32(0x80093D78u);
            old = r_u32(car + 44u); elapsed = tick / 3u - old;
            if (r_u8(0x80093C0Cu)) suspension_lap_report(car, lap, (sint32)tick / 3, elapsed, rank);
            if (r_u8(0x800CB7F5u) == 4u) suspension_call(0x80056EBCu, elapsed, 0, 0, 0);
        }
        w_u8(car + 59u, (uint8)(r_u8(car + 59u) + 1u));
    }
    return 0;
}

/* GTMAIN.EXE 80065C44 */
/* Original: GTMAIN.EXE:sub_80065C44. */ void gtmain_race_control_latch(uint32 car, uint8 value, uint32 hold)
{
    if (hold || !r_u8(car + 1965u)) { w_u8(car + 1964u, value); w_u8(car + 1965u, (uint8)hold); }
}

/* GTMAIN.EXE 8002D6D4 */
/* Original: GTMAIN.EXE:sub_8002D6D4. */ void gtmain_race_input_record(uint32 object, uint32 state)
{
    uint32 count, flags, buttons, steer, axes, low, high, changed = 120u;
    sint32 duration;
    if (!object) return;
    count = r_u32(object);
    if (r_u16(object + 12u)) return;
    w_u32(object, count + 1u);
    if (r_u8(state + 2u)) {
        flags = r_u8(state + 82u); buttons = r_u8(state + 80u); steer = r_u8(state + 83u);
        low = r_u8(state + 84u) & 15u; high = r_u8(state + 85u) & 15u;
    } else flags = buttons = steer = low = high = 0;
    duration = r_s32(object + 8u); axes = low | (high << 4);
    if (duration >= 0) {
        changed = flags != r_u8(object + 19u) ? 8u : 0u;
        if (buttons != r_u8(object + 20u)) changed |= 16u;
        if (steer != r_u8(object + 21u)) changed |= 32u;
        if (axes != r_u8(object + 22u)) changed |= 64u;
        if (!(changed & 120u)) { w_u32(object + 8u, (uint32)duration + 1u); return; }
        gtmain_race_input_record_flush(object);
    }
    w_u8(object + 18u, (uint8)changed); w_u8(object + 19u, (uint8)flags); w_u8(object + 20u, (uint8)buttons);
    w_u8(object + 21u, (uint8)steer); w_u8(object + 22u, (uint8)axes); w_u32(object + 8u, 0);
}
