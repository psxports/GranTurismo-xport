#include "game_first2_registry.h"

static sint32 boost_fixed(sint64 value)
{
    return (sint32)(uint32)(value / 4096);
}

/* GTMAIN.EXE 8005E6CC */
void gtmain_race_engine_boost_initialize(uint32 specification, uint32 engine_curve)
{
    uint32 s = specification, d = engine_curve, first, second, third;
    sint32 value, center, amplitude, divisor;
    if (!r_u8(s + 45u)) w_u16(d + 172u, 0);
    else {
        w_u16(d + 172u, (uint16)(((uint32)r_u8(s + 45u) << 12) / 10u));
        if (!r_u8(s + 376u) || !r_u8(s + 377u) || !r_u8(s + 378u)) w_u16(d + 172u, 0);
    }
    if (!r_s16(d + 172u)) {
        w_u16(d + 152u, 0); w_u32(d + 156u, 0); w_u32(d + 164u, 0); w_u16(d + 148u, 0);
        w_u16(d + 154u, 0); w_u32(d + 160u, 0); w_u32(d + 168u, 0); w_u16(d + 150u, 0); return;
    }
    w_u16(d + 152u, (uint16)(((uint32)r_u8(s + 377u) << 12) / 10u));
    w_u32(d + 156u, ((uint32)r_u8(s + 378u) << 12) / 10u);
    first = 100u * r_u8(s + 376u); center = (sint16)first; amplitude = r_s16(d + 152u);
    w_u16(d + 148u, (uint16)first); divisor = boost_fixed((sint64)center * center);
    value = (sint32)(uint32)gtmain_signed_divide64((sint64)amplitude * 4096, divisor, 0x8005E868u);
    w_u32(d + 164u, (uint32)value); w_u16(d + 154u, 0); w_u32(d + 160u, 0); w_u16(d + 150u, 0); w_u32(d + 168u, 0);
    first = r_u8(s + 380u); second = r_u8(s + 381u); third = r_u8(s + 379u);
    if (!(first + second + third) || !first || !third || !second) return;
    w_u16(d + 154u, (uint16)((first << 12) / 10u));
    w_u32(d + 160u, ((uint32)r_u8(s + 381u) << 12) / 10u);
    first = 100u * r_u8(s + 379u); center = (sint16)first; amplitude = r_s16(d + 154u);
    w_u16(d + 150u, (uint16)first); divisor = boost_fixed((sint64)center * center);
    w_u32(d + 168u, (uint32)gtmain_signed_divide64((sint64)amplitude * 4096, divisor, 0x8005E99Cu));
}

/* GTMAIN.EXE 8005E5B8 */
sint32 gtmain_race_engine_boost_curve(uint32 engine_curve, sint16 first, sint16 second)
{
    sint16 arguments[2] = {first, second};
    uint32 sum = 0, i;
    sint32 amplitude, center, difference, square, coefficient;
    for (i = 0; i < 2u; ++i) {
        amplitude = r_s16(engine_curve + 152u + 2u * i);
        if (!amplitude) continue;
        center = r_s16(engine_curve + 148u + 2u * i);
        if (arguments[i] >= 2 * center) continue;
        difference = arguments[i] - center; square = (sint32)((uint32)difference * (uint32)difference) / 4096;
        coefficient = r_s32(engine_curve + 164u + 4u * i);
        sum += (uint32)amplitude - (uint32)boost_fixed((sint64)square * coefficient);
    }
    return (sint32)(sum + 4096u);
}

/* GTMAIN.EXE 8005E678 */
sint32 gtmain_race_engine_boost_limit(uint32 engine_curve, sint16 first, sint16 second)
{
    sint32 result = gtmain_race_engine_boost_curve(engine_curve, first, second);
    sint32 limit = r_s16(engine_curve + 172u) + 4096;
    return limit < result ? limit : result;
}

/* GTMAIN.EXE 8005BE34 */
sint32 gtmain_race_steering_curve(uint32 curve, sint32 angle)
{
    sint32 wrapped = gtmain_race_angle_wrap(angle), value, sign;
    if (wrapped < -1024) { value = (sint32)((uint32)wrapped + 2048u); sign = -1; }
    else if (wrapped < 0) { value = (sint32)(0u - (uint32)wrapped); sign = -1; }
    else { value = wrapped >= 1024 ? (sint32)(2048u - (uint32)wrapped) : wrapped; sign = 1; }
    return sign * (sint16)gtmain_race_curve16(curve, (sint16)value);
}

/* GTMAIN.EXE 8004E730 */
void gtmain_race_top_speed_initialize(uint32 parameters)
{
    uint32 d = parameters, gear = 1, i, speed, sum;
    sint32 bound = boost_fixed((sint64)r_s32(d + 444u) * 14745) >> 12;
    sint32 value, rpm, torque, limit, first, second, force, coefficient;
    for (i = 0; (sint32)i < bound; ++i) {
        speed = 1138u * i; value = r_s32(d + 1124u + 4u * gear);
        value = boost_fixed((sint64)(sint32)speed * value); rpm = (sint32)((uint32)value * 60u) / 4096;
        torque = gtmain_race_curve32(d + 116u, value);
        value = boost_fixed((sint64)r_s32(d + 1124u + 4u * gear) * 25736);
        limit = boost_fixed((sint64)torque * value);
        gtmain_race_aero_components(d, (sint32)speed, &first, &second);
        value = gtmain_race_curve32(0x800BF530u, (sint32)speed); coefficient = r_s32(0x80093E5Cu);
        coefficient = boost_fixed((sint64)value * coefficient);
        sum = r_u32(d + 1240u) + (uint32)first + (uint32)second;
        force = boost_fixed((sint64)(sint32)sum * coefficient);
        value = gtmain_race_aero_force(d, (sint32)speed); force = (sint32)((uint32)force - (uint32)value);
        if (force < limit) w_u32(d + 444u, speed);
        if (gear < r_u8(d + 1058u) && r_u16(d + 1066u + 2u * gear) < rpm) ++gear;
    }
}
