#include "game_first2_registry.h"

static sint32 forces_fixed(sint64 v) { return (sint32)(uint32)(v / 4096); }
static sint32 forces_low(sint32 a, sint32 b) { return (sint32)((uint32)a * (uint32)b) / 4096; }
static sint32 forces_time(sint32 v) { return (sint32)(uint32)(((sint64)v * r_s32(0x1F800000u)) / 65536); }

/* GTMAIN.EXE 8005BDEC */
/* Original: GTMAIN.EXE:sub_8005BDEC. */ void gtmain_race_slip_curve_update(uint32 car)
{
    sint32 first = gtmain_race_curve16(car + 1068u, r_s16(car + 180u));
    sint32 second_argument = r_s16(car + 316u);
    w_u16(car + 250u, (uint16)first); w_u16(car + 182u, (uint16)first);
    first = gtmain_race_curve16(car + 1104u, second_argument);
    w_u16(car + 386u, (uint16)first); w_u16(car + 318u, (uint16)first);
}

/* GTMAIN.EXE 8005C65C */
/* Original: GTMAIN.EXE:sub_8005C65C. */ void gtmain_race_lateral_force_update(uint32 car)
{
    sint32 wheel_values[4], axle, wheel, angle, steering, value, limit, speed;
    uint32 offset, sum;
    for (axle = 0; axle < 2; ++axle) {
        offset = 4u * (uint32)axle;
        if (!r_s16(car + 1776u + 2u * (uint32)axle)) {
            w_u32(car + 1744u + offset, 0); w_u16(car + 166u + 136u * (uint32)axle, 0); w_u16(car + 234u + 136u * (uint32)axle, 0); continue;
        }
        speed = r_s32(car + 1732u + offset); steering = r_s16(car + 1740u + 2u * (uint32)axle);
        angle = axle ? 0 : r_s16(car + 14u);
        if (speed <= 0) angle = (sint32)((uint32)angle + 2048u);
        angle = gtmain_race_angle_delta(angle, steering);
        value = gtmain_race_steering_curve(car + 852u + 44u * (uint32)axle, angle);
        value = forces_fixed((sint64)value * r_s16(car + 1776u + 2u * (uint32)axle));
        wheel_values[2 * axle] = value; wheel_values[2 * axle + 1] = value;
        for (wheel = 2 * axle; wheel < 2 * axle + 2; ++wheel) {
            limit = r_s16(car + 182u + 68u * (uint32)wheel);
            if (limit < wheel_values[wheel]) wheel_values[wheel] = limit;
            else if (wheel_values[wheel] < -limit) wheel_values[wheel] = -limit;
            value = wheel_values[wheel]; if (value < 0) value = (sint32)(0u - (uint32)value);
            w_u16(car + 166u + 68u * (uint32)wheel, (uint16)value);
            value = r_s32(car + 156u + 68u * (uint32)wheel);
            wheel_values[wheel] = forces_low(value, wheel_values[wheel]);
        }
        sum = (uint32)wheel_values[2 * axle] + (uint32)wheel_values[2 * axle + 1];
        if (speed <= 0) sum = 0u - sum;
        w_u32(car + 1744u + offset, sum);
    }
}

/* GTMAIN.EXE 8005CD5C */
/* Original: GTMAIN.EXE:sub_8005CD5C. */ void gtmain_race_lateral_drag_update(uint32 car, sint32 index)
{
    uint32 scratch = 0x1F800000u + 84u * (uint32)index, wheel, material, table;
    sint32 wheel_values[4], cosine, sine, speed, axis, difference, projected, first, second, value, coefficient;
    cosine = r_s16(scratch + 70u); first = r_s32(car + 1932u);
    first = forces_fixed((sint64)cosine * first);
    speed = r_s32(car + 1724u); axis = r_s32(car + 1732u); sine = r_s16(scratch + 68u);
    difference = (sint32)((uint32)speed - (uint32)axis);
    projected = (sint32)((uint32)first + (uint32)forces_fixed((sint64)sine * speed));
    value = (sint16)gtmain_race_curve16(car + 1140u, r_s16(car + 180u));
    value = (sint16)forces_fixed((sint64)cosine * value);
    coefficient = gtmain_race_lateral_drag_curve(car + 1220u, projected);
    wheel_values[0] = forces_low(value, coefficient); wheel_values[1] = wheel_values[0];
    if ((uint32)axis + 5689u < 11379u && (uint32)difference + 1137u < 2275u) {
        value = (sint16)gtmain_race_lateral_low_speed(axis, projected);
        wheel_values[0] = (sint32)((uint32)wheel_values[0] + (uint32)value); wheel_values[1] = wheel_values[0];
    }
    axis = r_s32(car + 1736u); speed = r_s32(car + 1724u); value = r_s16(car + 316u); projected = r_s32(car + 1936u);
    difference = (sint32)((uint32)speed - (uint32)axis);
    first = (sint16)gtmain_race_curve16(car + 1176u, value);
    coefficient = gtmain_race_lateral_drag_curve(car + 1264u, projected);
    wheel_values[2] = forces_low(first, coefficient); wheel_values[3] = wheel_values[2];
    if ((uint32)axis + 5689u < 11379u && (uint32)difference + 1137u < 2275u) {
        value = (sint16)gtmain_race_lateral_low_speed(axis, projected);
        wheel_values[2] = (sint32)((uint32)wheel_values[2] + (uint32)value); wheel_values[3] = wheel_values[2];
    }
    for (wheel = 0; wheel < 4; ++wheel) {
        material = r_u8(car + 176u + 68u * wheel); table = 0x80093E4Cu + 2u * material;
        coefficient = r_s16(table); wheel_values[wheel] = forces_low(coefficient, wheel_values[wheel]);
        coefficient = r_s32(car + 160u + 68u * wheel); wheel_values[wheel] = forces_fixed((sint64)coefficient * wheel_values[wheel]);
    }
    w_u32(car + 1752u, (uint32)wheel_values[0] + (uint32)wheel_values[1]); w_u32(car + 1756u, (uint32)wheel_values[2] + (uint32)wheel_values[3]);
    for (wheel = 0; wheel < 4; ++wheel) {
        first = wheel_values[wheel]; second = r_s16(car + 166u + 68u * wheel);
        if (first < 0) first = (sint32)(0u - (uint32)first);
        value = (sint32)((uint32)second + (uint32)first); if (value > 32767) value = 32767;
        w_u16(car + 166u + 68u * wheel, (uint16)value);
    }
}

/* GTMAIN.EXE 8005BB54 */
/* Original: GTMAIN.EXE:sub_8005BB54. */ void gtmain_race_longitudinal_integrate(uint32 car, sint32 index)
{
    uint32 scratch = 0x1F800000u + 84u * (uint32)index, value, first, second;
    sint32 cosine, sine, force, acceleration, speed, updated, coefficient;
    cosine = r_s16(scratch + 70u); force = r_s32(car + 1760u); sine = r_s16(scratch + 68u);
    value = (uint32)forces_low(cosine, force); first = r_u32(car + 1764u); second = r_u32(car + 1948u);
    value += first; value += second; w_u32(car + 1948u, value);
    first = r_u32(scratch + 84u); second = r_u32(car + 1940u); value += first; w_u32(car + 1948u, value);
    value += second; force = r_s16(car + 1896u); coefficient = r_s32(scratch + 72u); value += (uint32)force;
    acceleration = forces_fixed((sint64)(sint32)value * coefficient);
    acceleration = (sint32)(r_u32(car + 1924u) + (uint32)acceleration); w_u32(car + 1924u, (uint32)acceleration);
    acceleration = forces_time(acceleration); speed = r_s32(car + 1724u); updated = (sint32)((uint32)acceleration + (uint32)speed); w_u32(car + 1724u, (uint32)updated);
    if (!updated) return;
    force = forces_low(cosine, r_s32(car + 1768u)); first = r_u32(car + 1744u); second = r_u32(car + 1752u);
    coefficient = forces_low(sine, (sint32)(first + second)); value = r_u32(car + 1772u);
    force = (sint32)((uint32)force + value - (uint32)coefficient); coefficient = r_s32(scratch + 72u); acceleration = forces_low(force, coefficient);
    coefficient = forces_time(acceleration); speed = r_s32(car + 1724u); updated = (sint32)((uint32)coefficient + (uint32)speed);
    if ((speed > 0 && updated > 0) || (speed < 0 && updated < 0)) {
        first = r_u32(car + 1948u); second = r_u32(car + 1924u);
        w_u32(car + 1948u, (uint32)force + first); w_u32(car + 1924u, (uint32)acceleration + second);
    } else {
        coefficient = r_s32(car + 4u); force = (sint32)(0u - (uint32)speed);
        acceleration = forces_fixed((sint64)force * coefficient); updated = 0;
        w_u32(car + 1924u, r_u32(car + 1924u) + (uint32)acceleration);
    }
    w_u32(car + 1724u, (uint32)updated);
}

/* GTMAIN.EXE 8005B470 */
/* Original: GTMAIN.EXE:sub_8005B470. */ void gtmain_race_engine_velocity_update(uint32 car, sint32 index)
{
    uint32 params = car + 404u, scratch = 0x1F800000u + 84u * (uint32)index, kind, gear;
    sint32 value, coefficient, first, second, limit, selected;
    int commit = 0;
    if (!r_u8(car + 1691u)) {
        value = gtmain_race_engine_torque(car); coefficient = r_s32(car + 1572u);
        value = forces_time(forces_fixed((sint64)value * coefficient)); value = (sint32)(r_u32(car + 1888u) + (uint32)value); w_u32(car + 1888u, (uint32)value);
        limit = ((uint32)r_u16(params + 258u) << 12) / 60u; if (value < limit) w_u32(car + 1888u, (uint32)limit);
    } else {
        kind = r_u8(car + 1460u); value = 0;
        if (kind < 5u) {
            gear = r_u8(car + 32u); coefficient = r_s32(params + 1124u + 4u * gear);
            first = r_s32(car + ((kind == 1u || kind == 2u) ? 1732u : 1736u)); value = forces_fixed((sint64)coefficient * first);
        }
        if (!r_u8(car + 32u)) value = (sint32)(0u - (uint32)value);
        if (r_u8(car + 1691u) == 1u) commit = 1;
        else if ((uint32)r_u8(car + 1691u) - 2u < 2u) {
            first = r_s32(scratch + 64u); second = r_s32(scratch + 60u);
            if (first >= second && r_s32(car + 1888u) >= value) commit = 1;
            else {
                first = r_s32(scratch + 64u); second = r_s32(scratch + 60u);
                if (second >= first && value >= r_s32(car + 1888u)) commit = 1;
            }
            if (commit) w_u8(car + 1691u, 1);
        }
        if (commit) w_u32(car + 1888u, (uint32)value);
        if (r_s32(car + 1888u) < 0) w_u32(car + 1888u, 0);
        else { limit = ((r_u16(params + 256u) + 10u) << 12) / 60u; if (r_s32(car + 1888u) >= limit) w_u32(car + 1888u, (uint32)limit); }
    }
    selected = gtmain_race_gear_select(car, (const uint8 *)psx_addr(0x1F8001FCu + 4u * (uint32)index, 16u));
    if (selected != -1) { if (r_u8(car + 1691u)) w_u8(car + 1691u, 3); w_u8(car + 32u, (uint8)selected); }
}

/* GTMAIN.EXE 8005D114 */
/* Original: GTMAIN.EXE:sub_8005D114. */ void gtmain_race_yaw_integrate(uint32 car, sint32 index)
{
    uint32 scratch = 0x1F800000u + 84u * (uint32)index, first, second;
    sint32 forward, rear, value, coefficient, left, right;
    first = r_u32(car + 1752u); second = r_u32(car + 1744u); coefficient = r_s16(scratch + 70u);
    left = forces_fixed((sint64)coefficient * (sint32)(first + second));
    first = r_u32(car + 1756u); second = r_u32(car + 1748u); rear = (sint32)(first + second);
    coefficient = r_s16(scratch + 68u); second = r_u32(car + 1768u); first = r_u32(car + 1760u);
    right = forces_fixed((sint64)coefficient * (sint32)(first + second)); forward = (sint32)((uint32)left + (uint32)right);
    if (r_s8(car + 1690u)) { w_u32(car + 1928u, 0); return; }
    left = forces_fixed((sint64)forward * r_s16(car + 416u)); right = forces_fixed((sint64)rear * r_s16(car + 418u));
    value = (sint32)((uint32)left - (uint32)right); coefficient = r_s32(car + 500u); value = forces_fixed((sint64)value * coefficient);
    value = forces_time(value); value = (sint32)((uint32)value + r_u32(car + 1792u)); w_u32(car + 1792u, (uint32)value);
    if (value > 4096) w_u32(car + 1792u, 4096); else if (r_s32(car + 1792u) < -4096) w_u32(car + 1792u, (uint32)-4096);
    value = (sint32)(0u - (uint32)forward - (uint32)rear - r_u32(car + 1944u)); w_u32(car + 1928u, (uint32)value);
    coefficient = r_s32(car + 448u); w_u32(car + 1928u, (uint32)forces_fixed((sint64)value * coefficient));
}

/* GTMAIN.EXE 8005D478 */
/* Original: GTMAIN.EXE:sub_8005D478. */ void gtmain_race_world_velocity_integrate(uint32 car)
{
    sint32 forward, lateral, first, second, value, coefficient;
    uint32 i, address;
    if (r_u8(car + 1688u) || r_u16(car + 12u) || r_u8(car + 1696u) == 7u) {
        w_u32(car + 1948u, 0); w_u32(car + 1792u, 0); w_u32(car + 1712u, 0); w_u32(car + 1716u, 0); return;
    }
    if (!r_s8(car + 1690u)) {
        if (!r_u32(car + 1776u) && r_u32(car + 1724u) + 1137u < 2275u && (r_u32(car + 1784u) & 0x202u)) {
            w_u32(car + 1792u, 0); w_u32(car + 1712u, 0); w_u32(car + 1716u, 0); w_u32(car + 1948u, 0);
            w_u32(car + 1924u, 0); w_u32(car + 1928u, 0); w_u32(car + 1724u, 0); w_u32(car + 1728u, 0);
        } else {
            forward = forces_time(r_s32(car + 1924u)); lateral = forces_time(r_s32(car + 1928u));
            for (i = 0; i < 2; ++i) {
                coefficient = r_s16(car + 72u + 2u * i); first = forces_low(forward, coefficient);
                coefficient = r_s16(car + 80u + 2u * i); second = forces_low(lateral, coefficient);
                address = car + 1712u + 4u * i; w_u32(address, (uint32)first + (uint32)second + r_u32(address));
            }
        }
    }
    if (!r_u32(car + 1976u) && !r_u32(car + 1980u)) return;
    for (i = 0; i < 2; ++i) {
        value = r_s32(car + 1976u + 4u * i); coefficient = r_s32(car + 448u); value = forces_fixed((sint64)value * coefficient);
        value = forces_time(value); address = car + 1712u + 4u * i; w_u32(address, r_u32(address) + (uint32)value);
    }
}
