#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* TODO Bind remaining original vehicle dependencies */
static uint32 controls_call(uint32 owner, uint32 a0, uint32 a1, uint32 a2, uint32 a3)
{
    return gt_draft_unresolved_call(owner, a0, a1, a2, a3, 0, 0, 0, 0);
}

static uint32 controls_div(uint32 value, uint32 shift)
{
    return (uint32)((sint32)value / (sint32)(1u << shift));
}

static uint32 controls_fp(uint32 first, uint32 second)
{
    return controls_div(first * second, 12u);
}

static uint16 controls_half(const uint8 *controls, uint32 offset)
{
    return xport_load_le16(controls + offset);
}

/* GTMAIN.EXE 80020BDC */
void gtmain_race_controls_decode(uint8 *controls, uint32 state, sint32 player, uint32 object)
{
    uint32 buttons, flags, result;
    if (r_s8(0x800B63C2u)) {
        buttons = r_u16(state + 80u);
        w_u8(state + 83u, 0); w_u8(state + 84u, 0); w_u8(state + 85u, 0);
        w_u16(state + 80u, (uint16)(buttons & 0xFF00u));
    }
    if (player >= 0 && player < 2) gtmain_race_input_record(object, state);
    else if (player == 2) gtmain_race_input_playback(object, state);
    buttons = r_u16(state + 80u); flags = r_u8(state + 82u);
    controls[9] = (uint8)((buttons >> 4) & 1u); controls[10] = (uint8)((buttons >> 7) & 1u);
    controls[8] = (uint8)(((buttons >> 5) & 1u) - ((buttons >> 6) & 1u));
    xport_store_le16(controls, (uint16)flags);
    if (flags & 1u) result = 0u - ((uint32)r_s8(state + 83u) << 6);
    else result = ((buttons & 1u) ? 2u : 0u) - ((buttons & 2u) ? 2u : 0u);
    xport_store_le16(controls + 2u, (uint16)result);
    if (flags & 2u) result = (uint32)r_s16(0x8008F3B0u + 2u * (r_u8(state + 84u) & 15u));
    else result = (buttons >> 2) & 1u;
    xport_store_le16(controls + 6u, (uint16)result);
    if (flags & 4u) result = (uint32)r_s16(0x8008F3B0u + 2u * (r_u8(state + 85u) & 15u));
    else result = (buttons >> 3) & 1u;
    xport_store_le16(controls + 4u, (uint16)result);
}

/* GTMAIN.EXE 800654C0 */
void gtmain_race_controls_apply(uint32 car, const uint8 *controls, uint32 guest_output, sint32 index)
{
    uint32 speed = r_u32(car + 1724u), rotation = r_u16(car + 1740u), curve, limit, first, result, target, damping;
    uint32 table_index = 2u * (uint32)index;
    sint32 steering, direction, current, next;
    first = 0;
    if ((sint32)speed > 0) {
        first = controls_fp(speed, 461u);
        if ((sint32)first > 32767) first = 32767;
    }
    curve = (uint32)gtmain_race_curve16(car + 460u, (sint16)first);
    limit = (uint32)r_s16(car + 496u);
    if ((sint16)curve < (sint32)limit) limit = curve;
    speed = r_u32(car + 1724u);
    if ((sint32)speed <= 34139 || ((sint16)rotation >= -(sint32)(sint16)limit && (sint16)rotation <= (sint16)limit)) {
        w_u16(car + 54u, (uint16)curve); w_u16(car + 52u, (uint16)(0u - curve));
    } else if ((sint16)rotation > 0) {
        w_u16(car + 54u, (uint16)rotation);
        first = (uint32)r_s16(car + 458u);
        if ((sint32)first < (sint16)rotation) w_u16(car + 54u, (uint16)first);
        w_u16(car + 52u, (uint16)(0u - curve));
    } else if ((sint16)rotation < 0) {
        w_u16(car + 54u, (uint16)curve); w_u16(car + 52u, (uint16)rotation);
        first = 0u - (uint32)r_s16(car + 458u);
        if ((sint16)rotation < (sint32)first) w_u16(car + 52u, (uint16)first);
    }
    if (controls_half(controls, 0) & 1u) {
        if (controls_half(controls, 0) & 8u) {
            steering = (sint16)controls_half(controls, 2u);
            if (steering < 0) steering = -steering;
            result = (uint32)gtmain_race_curve16(0x80093EFCu, (sint16)steering);
            steering = (sint16)result;
            if ((sint16)controls_half(controls, 2u) < 0) steering = -steering;
        } else {
            steering = (sint16)controls_half(controls, 2u);
            result = controls_fp((uint32)steering, (uint32)steering);
            steering = (sint16)controls_half(controls, 2u) < 0 ? (sint32)(0u - result) : (sint32)result;
        }
        if (steering < 0) result = controls_fp((uint32)r_s16(car + 52u), 0u - (uint32)steering);
        else result = controls_fp((uint32)r_s16(car + 54u), (uint32)steering);
        w_u16(car + 14u, (uint16)result);
    } else {
        steering = (sint16)controls_half(controls, 2u);
        target = steering > 0 ? r_u16(car + 54u) : (steering < 0 ? r_u16(car + 52u) : 0u);
        current = r_s16(car + 14u);
        first = controls_fp((uint32)(sint32)(sint16)target - (uint32)current, r_u32(0x80093DD8u));
        damping = controls_fp((uint32)r_s16(car + 26u), r_u32(0x80093DDCu));
        result = first - damping;
        if (!target) result -= controls_fp((uint32)r_s16(car + 14u), r_u32(0x80093DE0u));
        result = result * r_u32(0x1F800000u);
        result = r_u16(car + 26u) + controls_div(result, 16u);
        w_u16(car + 26u, (uint16)result);
        first = (uint32)(sint32)(sint16)result * r_u32(0x1F800000u);
        current = r_s16(car + 14u);
        next = (sint32)((uint32)current + controls_div(first, 16u));
        direction = (sint16)target;
        if ((direction > 0 && next > direction) || (direction < 0 && next < direction) ||
            (current < direction && next >= direction) || (direction < current && next <= direction)) {
            w_u16(car + 14u, (uint16)target); w_u16(car + 26u, 0);
        } else w_u16(car + 14u, (uint16)next);
    }
    if (controls_half(controls, 0) & 2u) {
        if ((sint8)controls[9]) { w_u8(guest_output, 1); result = 4096; }
        else { w_u8(guest_output, 0); result = controls_half(controls, 6u); }
        w_u16(car + 16u, (uint16)result);
        first = (uint32)r_s16(car + 16u);
        if (first) w_u16(car + 16u, (uint16)(controls_fp(first, 3584u) + 512u));
    } else {
        if ((sint8)controls[9] && !r_u16(car + 12u)) {
            w_u8(guest_output, 1);
            result = r_u16(car + 16u) + r_u16(0x80093F08u + table_index);
        } else {
            w_u8(guest_output, 0);
            if ((sint16)controls_half(controls, 6u)) result = r_u16(car + 16u) + r_u16(0x80093F08u + table_index);
            else {
                result = r_u16(car + 16u) - r_u16(0x80093F0Cu + table_index);
                w_u16(car + 16u, (uint16)result);
                if ((sint16)result < 0) w_u16(car + 16u, 0);
                goto throttle_done;
            }
        }
        w_u16(car + 16u, (uint16)result);
        if ((sint16)result >= 4097) w_u16(car + 16u, 4096);
    }
throttle_done:
    if (controls_half(controls, 0) & 4u) {
        result = controls_half(controls, 4u); w_u16(car + 20u, (uint16)result);
        if ((sint16)result) w_u16(car + 20u, (uint16)(controls_div(((uint32)(sint32)(sint16)result * 7u) << 9, 12u) + 512u));
    } else if ((sint16)controls_half(controls, 4u)) {
        result = r_u16(car + 20u) + r_u16(0x80093F10u + table_index);
        w_u16(car + 20u, (uint16)result);
        if ((sint16)result >= 4097) w_u16(car + 20u, 4096);
    } else {
        result = r_u16(car + 20u) - r_u16(0x80093F14u + table_index);
        w_u16(car + 20u, (uint16)result);
        if ((sint16)result < 0) w_u16(car + 20u, 0);
    }
    if (r_u16(car + 12u)) w_u8(guest_output + 2u, 0);
    else w_u8(guest_output + 2u, controls[8]);
    if ((sint8)controls[10]) {
        result = r_u16(car + 1692u) + r_u16(0x80093F18u + table_index);
        w_u16(car + 1692u, (uint16)result);
        if ((sint16)result >= 4097) w_u16(car + 1692u, 4096);
    } else {
        result = r_u16(car + 1692u) - r_u16(0x80093F1Cu + table_index);
        w_u16(car + 1692u, (uint16)result);
        if ((sint16)result < 0) w_u16(car + 1692u, 0);
    }
    if (r_u8(car + 1688u) || r_u16(car + 12u) || r_u8(car + 1696u) == 7u ||
        (r_u8(car + 1463u) == 1u && r_s16(car + 1692u) == 4096)) w_u8(guest_output + 1u, 0);
    else if (r_u8(car + 1691u)) {
        w_u8(guest_output + 1u, 1);
        current = r_s16(car + 38u); result = r_u16(car + 662u);
        if (current < (sint32)result) w_u8(guest_output + 1u, 0);
    } else {
        w_u8(guest_output + 1u, 0);
        if (r_s16(car + 20u) != 4096) {
            current = r_s16(car + 38u); result = r_u16(car + 1468u);
            if ((sint32)result < current || r_u32(car + 1724u) + 11380u >= 0x58E9u) w_u8(guest_output + 1u, 1);
        }
    }
}

/* GTMAIN.EXE 80065C6C */
void gtmain_race_controls_drive(uint32 car, const uint8 *controls, sint32 index)
{
    uint32 throttle, brake, second_brake, table_index = 11u * (uint32)index;
    gtmain_race_controls_apply(car, controls, 0x1F8001FCu + 4u * (uint32)index, index);
    throttle = r_u16(car + 16u); brake = r_u16(car + 20u); second_brake = r_u16(car + 20u);
    w_u16(car + 18u, (uint16)throttle); w_u16(car + 22u, (uint16)brake); w_u16(car + 24u, (uint16)second_brake);
    if (r_u8(0x80093C3Eu + table_index)) gtmain_race_brake_assist(car);
    if (!r_u8(car + 32u) || r_u8(0x80093C3Du + table_index)) gtmain_race_throttle_assist(car);
}

/* GTMAIN.EXE 80056D04 */
void gtmain_race_controls_ai(uint32 car, const uint8 *unused_controls, sint32 index)
{
    uint32 output = 0x1F8001FCu + 4u * (uint32)index;
    uint32 timer;
    (void)unused_controls;
    w_u8(car + 1699u, 0); w_u8(output, 0); w_u8(output + 2u, r_u8(car + 32u));
    if (r_u8(car + 1688u)) {
        w_u8(output + 1u, 0);
        timer = (uint32)r_s16(0x800B6312u) - r_u32(0x800BF518u);
        gtmain_race_controls_launch(car, index, (sint32)timer);
        w_u16(car + 1692u, 0); return;
    }
    if (r_u16(car + 12u)) {
        w_u8(output + 1u, 0); timer = r_u16(car + 12u);
        gtmain_race_controls_launch(car, index, (sint32)timer);
        w_u16(car + 1692u, 0); return;
    }
    if (r_u8(car + 1696u) == 7u) w_u8(output + 1u, 0);
    else {
        if (r_u8(car + 1691u)) {
            w_u8(output + 1u, 1);
            if (r_s16(car + 38u) < (sint32)r_u16(car + 662u)) w_u8(output + 1u, 0);
        } else {
            uint32 limit;
            w_u8(output + 1u, 0);
            limit = controls_div(3u * r_u16(car + 1470u), 2u);
            if ((sint32)limit < r_s16(car + 38u)) w_u8(output + 1u, 1);
        }
        gtmain_race_controls_ai_update(car, index);
    }
    w_u16(car + 1692u, 0);
}

/* GTMAIN.EXE 80052094 */
void gtmain_race_controls_hud(uint32 car, const uint8 *controls)
{
    sint32 level;
    uint32 value;
    w_u8(car + 1966u, 0);
    if (controls_half(controls, 0) & 2u) {
        value = (uint32)controls_half(controls, 6u) << 16;
        level = (sint32)value >> 24;
        if (value) { if (level >= 16) level = 15; w_u8(car + 1966u, (uint8)(r_u8(car + 1966u) | (uint32)level)); }
    } else if ((sint16)controls_half(controls, 6u)) w_u8(car + 1966u, 15);
    if (controls_half(controls, 0) & 4u) {
        value = (uint32)controls_half(controls, 4u) << 16;
        level = (sint32)value >> 24;
        if (value) { if (level >= 16) level = 15; w_u8(car + 1966u, (uint8)(r_u8(car + 1966u) | ((uint32)level << 4))); }
    } else if ((sint16)controls_half(controls, 4u)) w_u8(car + 1966u, (uint8)(r_u8(car + 1966u) | 0xF0u));
}

/* GTMAIN.EXE 80052164 */
void gtmain_race_controls_hud_ai(uint32 car)
{
    sint32 throttle = r_s16(car + 16u);
    w_u8(car + 1966u, 0);
    if (throttle) w_u8(car + 1966u, 15);
    if (r_s16(car + 20u)) w_u8(car + 1966u, (uint8)(r_u8(car + 1966u) | 0xF0u));
}

static sint32 controls_curve_divide(uint32 scaled, uint32 divisor, uint32 link)
{
    sint64 numerator = (sint64)(sint32)scaled * 4096;
    return (sint32)gtmain_signed_divide64(numerator, (sint64)(sint32)divisor, link);
}

/* GTMAIN.EXE 80058C34 */
sint32 gtmain_race_curve32(uint32 curve, sint32 value)
{
    uint32 last = (r_u16(curve) - 1u) & 0xFFFFu, xs = r_u32(curve + 4u);
    uint32 first_x = r_u32(xs), last_x = r_u32(xs + 4u * last), lower = 0, upper = last;
    uint32 ys, x0, x1, y0, y1, scaled;
    sint64 product;
    if ((sint32)first_x >= value) {
        ys = r_u32(curve + 8u); x1 = r_u32(xs + 4u); y0 = r_u32(ys); y1 = r_u32(ys + 4u);
        if ((uint32)value == first_x || !r_s8(curve + 2u)) return (sint32)y0;
        product = (sint64)(sint32)(y0 - y1) * (sint32)((uint32)value - x1);
        scaled = (uint32)(product / 4096);
        return (sint32)(y1 + (uint32)controls_curve_divide(scaled, first_x - x1, 0x80058CF8u));
    }
    if (value >= (sint32)last_x) {
        xs = r_u32(curve + 4u); ys = r_u32(curve + 8u);
        x0 = r_u32(xs + 4u * last - 4u); y0 = r_u32(ys + 4u * last - 4u); y1 = r_u32(ys + 4u * last);
        if ((uint32)value == last_x || !r_s8(curve + 3u)) return (sint32)y1;
        product = (sint64)(sint32)(y1 - y0) * (sint32)((uint32)value - x0);
        scaled = (uint32)(product / 4096);
        return (sint32)(y0 + (uint32)controls_curve_divide(scaled, last_x - x0, 0x80058DA0u));
    }
    if (last >= 4u) {
        xs = r_u32(curve + 4u);
        do {
            uint32 middle = (upper + lower) >> 1;
            if (value < r_s32(xs + 4u * middle)) upper = middle & 0xFFFFu;
            else {
                uint32 coordinate = r_u32(r_u32(curve + 4u) + 4u * (middle & 0xFFFFu));
                if ((uint32)value == coordinate) return r_s32(r_u32(curve + 8u) + 4u * (middle & 0xFFFFu));
                lower = middle & 0xFFFFu;
            }
        } while ((sint32)(upper - lower) >= 4);
    }
    if (lower == upper) return r_s32(r_u32(curve + 8u) + 4u * lower);
    while (lower < upper) {
        if (value < r_s32(r_u32(curve + 4u) + 4u * lower)) break;
        lower = (lower + 1u) & 0xFFFFu;
    }
    xs = r_u32(curve + 4u); ys = r_u32(curve + 8u);
    x0 = r_u32(xs + 4u * lower - 4u); x1 = r_u32(xs + 4u * lower);
    y0 = r_u32(ys + 4u * lower - 4u); y1 = r_u32(ys + 4u * lower);
    if ((uint32)value == x0) return (sint32)y0;
    product = (sint64)(sint32)((uint32)value - x0) * (sint32)y1;
    scaled = (uint32)(product / 4096);
    product = (sint64)(sint32)(x1 - (uint32)value) * (sint32)y0;
    scaled += (uint32)(product / 4096);
    return controls_curve_divide(scaled, x1 - x0, 0x80058F60u);
}

/* GTMAIN.EXE 8005F000 */
void gtmain_race_basis_set(uint32 first, uint32 second, uint32 third, sint32 first_angle, sint32 second_angle, sint32 third_angle)
{
    uint32 b = ((uint32)second_angle & 4095u) * 2u, c = ((uint32)third_angle & 4095u) * 2u, a = ((uint32)first_angle & 4095u) * 2u;
    uint32 sb = (uint32)r_s16(0x8009423Cu + b), cc = (uint32)r_s16(0x80094A3Cu + c);
    uint32 sa = (uint32)r_s16(0x8009423Cu + a), ca = (uint32)r_s16(0x80094A3Cu + a);
    uint32 cb = (uint32)r_s16(0x80094A3Cu + b), sc = (uint32)r_s16(0x8009423Cu + c);
    uint32 mixed_c = controls_fp(cc, sb), mixed_s = controls_fp(sc, sb), result;
    w_u16(second, (uint16)controls_fp(cc, cb)); w_u16(second + 2u, (uint16)controls_fp(sc, cb)); w_u16(second + 4u, (uint16)(0u - sb));
    result = controls_fp(mixed_c, sa) - controls_fp(sc, ca); w_u16(first, (uint16)result);
    result = controls_fp(mixed_s, sa) + controls_fp(cc, ca); w_u16(first + 2u, (uint16)result);
    w_u16(first + 4u, (uint16)controls_fp(cb, sa));
    result = controls_fp(mixed_c, ca) + controls_fp(sc, sa); w_u16(third, (uint16)result);
    result = controls_fp(mixed_s, ca) - controls_fp(cc, sa); w_u16(third + 2u, (uint16)result);
    w_u16(third + 4u, (uint16)controls_fp(cb, ca));
}

/* GTMAIN.EXE 8005F1CC */
sint32 gtmain_race_angle_wrap(sint32 angle)
{
    while (angle < -2048) angle = (sint32)((uint32)angle + 4096u);
    while (angle >= 2048) angle = (sint32)((uint32)angle - 4096u);
    return angle;
}

/* GTMAIN.EXE 8005F208 */
sint32 gtmain_race_angle_delta(sint32 first, sint32 second)
{
    uint32 a = (uint32)gtmain_race_angle_wrap(first), b = (uint32)gtmain_race_angle_wrap(second);
    sint32 difference = (sint32)(a - b);
    if (difference >= 2048) return (sint32)((uint32)difference - 4096u);
    if (difference < -2048) return (sint32)((uint32)difference + 4096u);
    return difference;
}

/* GTMAIN.EXE 8005F53C */
void gtmain_race_wheel_pose(uint32 car)
{
    uint32 a = controls_fp((uint32)r_s16(car + 76u), (uint32)r_s16(car + 416u));
    uint32 b = controls_fp((uint32)r_s16(car + 76u), (uint32)r_s16(car + 418u));
    uint32 c = controls_fp((uint32)r_s16(car + 84u), (uint32)r_s16(car + 428u));
    uint32 d = controls_fp((uint32)r_s16(car + 84u), (uint32)r_s16(car + 430u));
    uint32 e = (uint32)r_s16(car + 72u) * (uint32)r_s16(car + 416u), f, g, h, i, j, k, l;
    w_u16(car + 144u, (uint16)(a - c)); w_u16(car + 212u, (uint16)(c + a));
    w_u16(car + 280u, (uint16)(0u - d - b)); w_u16(car + 348u, (uint16)(d - b));
    e = controls_div(e, 12u);
    f = controls_fp((uint32)r_s16(car + 74u), (uint32)r_s16(car + 416u));
    g = controls_fp((uint32)r_s16(car + 72u), (uint32)r_s16(car + 418u));
    h = controls_fp((uint32)r_s16(car + 74u), (uint32)r_s16(car + 418u));
    i = controls_fp((uint32)r_s16(car + 80u), (uint32)r_s16(car + 428u));
    j = controls_fp((uint32)r_s16(car + 82u), (uint32)r_s16(car + 428u));
    k = controls_fp((uint32)r_s16(car + 80u), (uint32)r_s16(car + 430u));
    l = controls_fp((uint32)r_s16(car + 82u), (uint32)r_s16(car + 430u));
    w_u16(car + 140u, (uint16)(e - i)); w_u16(car + 142u, (uint16)(f - j));
    w_u16(car + 208u, (uint16)(i + e)); w_u16(car + 210u, (uint16)(j + f));
    w_u16(car + 276u, (uint16)(0u - k - g)); w_u16(car + 278u, (uint16)(0u - l - h));
    w_u16(car + 344u, (uint16)(k - g)); w_u16(car + 346u, (uint16)(l - h));
}

/* GTMAIN.EXE 8005C3A4 */
void gtmain_race_suspension_prepare(uint32 cars, sint32 count)
{
    sint32 index;
    for (index = 0; index < count; ++index) gtmain_race_suspension_car_prepare(cars + 3380u * (uint32)index + 40u);
}

/* GTMAIN.EXE 8005C5E8 */
void gtmain_race_suspension_update(uint32 cars, sint32 count)
{
    sint32 index;
    for (index = 0; index < count; ++index) gtmain_race_suspension_car_update(cars + 3380u * (uint32)index + 40u);
}

/* GTMAIN.EXE 80065D34 */
void gtmain_race_velocity_publish(uint32 cars, sint32 count)
{
    sint32 index;
    for (index = 0; index < count; ++index) {
        uint32 car = cars + 3380u * (uint32)index, scratch = 0x1F8000B4u + 16u * (uint32)index, step = r_u32(car + 44u), value;
        w_u32(0x1F800000u, step);
        value = r_u32(car + 1752u) * step; w_u32(scratch, controls_div(value, 16u));
        value = r_u32(car + 1756u) * r_u32(0x1F800000u); w_u32(scratch + 4u, controls_div(value, 16u));
        value = r_u32(car + 1760u) * r_u32(0x1F800000u); w_u32(scratch + 8u, controls_div(value, 16u));
        value = r_u32(car + 1832u) * r_u32(0x1F800000u); w_u32(scratch + 12u, controls_div(value, 16u));
    }
}

/* GTMAIN.EXE 80057BF8 */
void gtmain_race_lap_car_tick(uint32 car, sint32 rank)
{
    uint32 mode = r_u8(0x800CB7F5u), previous = r_u32(car + 1804u), current, hit, sum;
    uint32 storage[3];
    uint8 *position = (uint8 *)storage;
    if (mode == 4u && r_u16(0x800BF520u) == 4u) current = controls_call(0x8005F2D0u, car + 60u, 0, 0, 0);
    else {
        xport_store_le32(position, r_u32(car + 60u) << 4); xport_store_le32(position + 4u, r_u32(car + 68u) << 4);
        xport_store_le32(position + 8u, r_u32(car + 64u) << 4);
        current = (uint32)gtmain_course_progress(r_u32(car + 1892u), position);
    }
    w_u32(car + 1804u, current);
    if (r_s16(0x800B63DEu)) return;
    hit = (uint32)gtmain_race_lap_crossing(car, (sint32)previous, rank);
    if (hit == 1u) {
        sint32 component = r_s32(car + 1724u);
        sint64 product = (sint64)component * component;
        w_u8(car + 58u, (uint8)(r_u8(car + 58u) | 1u));
        sum = (uint32)(product / 4096);
        component = r_s32(car + 1728u);
        product = (sint64)component * component;
        sum += (uint32)(product / 4096);
        if ((sint32)sum > 7904) {
            if (!r_u8(car + 1698u)) gtmain_race_control_latch(car, 1, 0);
        } else if ((sint32)sum < 2845) gtmain_race_control_latch(car, 0, 0);
    } else {
        w_u8(car + 58u, (uint8)(r_u8(car + 58u) & 254u));
        gtmain_race_control_latch(car, 0, 0);
    }
    gtmain_race_course_events(car, r_s32(car + 1804u), (sint32)previous);
}

/* GTMAIN.EXE 8002E0B8 */
void gtmain_race_lap_difference_show(sint32 car_index, sint32 difference)
{
    sint32 type = r_s16(0x800B686Au + 3380u * (uint32)car_index);
    uint32 prefix = 43, value = (uint32)difference, first = 1, tick;
    if (type >= 4 || type <= 0) return;
    if (!value) prefix = 32;
    else if ((sint32)value < 0) { prefix = 45; value = 0u - value; }
    if (car_index && prefix == 43u) prefix = 45;
    if (value) gtmain_time_string(0x800B39B5u, value);
    else gtmain_string_copy(0x800B39B5u, 0x8008C068u);
    if (r_s8(0x800B39B5u) == 32) {
        first = 2;
        while (r_s8(0x800B39B4u + first) == 32) ++first;
    }
    gtmain_memory_move(0x800B39B5u, 0x800B39B4u + first, 15u - first);
    tick = r_u32(0x800BF518u);
    w_u8(0x800B39B4u, (uint8)prefix);
    w_u16(0x8009071Cu, (uint16)(3u * tick));
}

