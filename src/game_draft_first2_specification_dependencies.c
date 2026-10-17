#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 parameters_fp64(sint64 value)
{
    return (sint32)(uint32)(value / 4096);
}

static sint32 parameters_magic(uint32 input, uint32 multiplier, uint32 shift, uint32 add, uint32 correction)
{
    sint64 product = (sint64)(sint32)input * (sint32)multiplier;
    uint32 high = (uint32)((uint64)product >> 32);
    if (add) high += input;
    return (sint32)((uint32)((sint32)high >> shift) - (correction ? (uint32)((sint32)input >> 31) : 0u));
}

static sint32 parameters_div(uint32 numerator, sint32 denominator)
{
    if (!denominator) gt_draft_exception(7);
    if (denominator == -1 && numerator == 0x80000000u) gt_draft_exception(6);
    return (sint32)numerator / denominator;
}

static void parameters_header(uint32 header, uint32 count, uint32 x, uint32 y)
{
    w_u16(header, (uint16)count); w_u8(header + 2u, 0); w_u8(header + 3u, 0);
    w_u32(header + 4u, x); w_u32(header + 8u, y);
}

/* GTMAIN.EXE 8004EBE8 */
void gtmain_race_specification_curves_build(uint32 source_x, uint32 source_y, uint32 source_z, uint32 source_extra,
    sint32 count, uint32 source_a, uint32 source_b, uint32 source_c, uint32 source_d, sint32 secondary_count,
    uint32 curve_y, uint32 curve_z, uint32 curve_extra, uint32 output_x, uint32 output_y, uint32 output_z,
    uint32 output_extra, uint32 peak_first, uint32 peak_second)
{
    sint32 i, maximum = 0, position, value;
    uint32 address;
    w_u8(source_x, 0); w_u8(source_y, 0); w_u8(source_a, 0); w_u8(source_b, 0); w_u16(peak_first, 0);
    for (i = 0; i < count; ++i) {
        position = (sint32)((uint32)count - (uint32)i - 1u); address = output_x + 2u * (uint32)position;
        value = parameters_magic((0u - r_u8(source_x + (uint32)i)) << 12, 0x80808081u, 7, 1, 1);
        w_u16(address, (uint16)value);
        value = parameters_magic((uint32)r_u8(source_y + (uint32)i) << 12, 0x51EB851Fu, 6, 0, 1);
        w_u16(output_y + 2u * (uint32)position, (uint16)value); value = (sint16)value;
        if (maximum < value) { maximum = value; w_u16(peak_first, r_u16(address)); }
        value = parameters_magic((uint32)r_u8(source_z + (uint32)i) << 12, 0x51EB851Fu, 6, 0, 0);
        w_u16(output_z + 2u * (uint32)position, (uint16)value);
        value = parameters_magic((uint32)r_u8(source_extra + (uint32)i) << 12, 0x51EB851Fu, 6, 0, 0);
        w_u16(output_extra + 2u * (uint32)position, (uint16)value);
    }
    if (count > 0) maximum = 0;
    w_u16(peak_second, 0);
    for (i = 0; i < secondary_count; ++i) {
        position = (sint32)((uint32)count + (uint32)i - 1u); address = output_x + 2u * (uint32)position;
        value = parameters_magic((uint32)r_u8(source_a + (uint32)i) << 12, 0x80808081u, 7, 1, 0);
        w_u16(address, (uint16)value);
        value = parameters_magic((uint32)r_u8(source_b + (uint32)i) << 12, 0x51EB851Fu, 6, 0, 1);
        w_u16(output_y + 2u * (uint32)position, (uint16)value); value = (sint16)value;
        if (maximum < value) { maximum = value; w_u16(peak_second, r_u16(address)); }
        value = parameters_magic((uint32)r_u8(source_c + (uint32)i) << 12, 0x51EB851Fu, 6, 0, 0);
        w_u16(output_z + 2u * (uint32)position, (uint16)value);
        value = parameters_magic((uint32)r_u8(source_d + (uint32)i) << 12, 0x51EB851Fu, 6, 0, 0);
        w_u16(output_extra + 2u * (uint32)position, (uint16)value);
    }
    address = (uint32)count + (uint32)secondary_count - 1u;
    parameters_header(curve_y, address, output_x, output_y);
    parameters_header(curve_z, address, output_x, output_z);
    parameters_header(curve_extra, address, output_x, output_extra);
}

/* GTMAIN.EXE 8005BF54 */
sint32 gtmain_race_steering_peak(uint32 parameters)
{
    sint32 maximum = 0, best = 90, degree, angle, value;
    for (degree = -45; degree < 46; ++degree) {
        angle = (degree * 4096) / 360;
        value = gtmain_race_steering_curve(parameters + 448u, angle);
        value = parameters_fp64((sint64)r_s16(0x80094A3Cu + 2u * ((uint32)angle & 4095u)) * value);
        if (maximum < value) { maximum = value; best = degree; }
    }
    return (best * 4096) / 360;
}

/* GTMAIN.EXE 8005E9B4 */
void gtmain_race_engine_curve_initialize(uint32 specification, uint32 curve, uint32 enabled)
{
    uint32 s = specification, d = curve, address, old, i;
    sint32 value, numerator, denominator, argument;
    w_u16(d + 142u, (uint16)(10u * r_u8(s + 19u)));
    if (!r_u8(s + 50u)) w_u8(s + 50u, r_u8(s + r_u8(s + 86u) + 51u));
    w_u16(d + 140u, (uint16)(100u * r_u8(s + 50u)));
    address = s + 2u * r_u8(s + 86u);
    if (!r_u16(address + 316u)) w_u16(address + 316u, r_u16(address + 314u));
    for (i = 0; i < r_u8(s + 86u); ++i) {
        old = 409600u * r_u8(s + 52u + i);
        w_u32(d + 12u + 4u * i, (uint32)parameters_magic(old, 0x88888889u, 5, 1, 1));
        value = (sint32)(160564u * r_u16(s + 318u + 2u * i)) / 4096;
        old = (uint32)value * r_u8(s + 48u);
        w_u32(d + 76u + 4u * i, (uint32)parameters_magic(old, 0x68DB8BADu, 12, 0, 1));
    }
    old = r_u8(s + 86u);
    w_u32(d + 4u, d + 12u); w_u8(d + 2u, 0); w_u8(d + 3u, 0); w_u32(d + 8u, d + 76u); w_u16(d, (uint16)old);
    value = (sint32)(160564u * r_u8(s + 44u)) / 4096;
    denominator = r_s32(d + 4u * r_u8(s + 86u) + 8u);
    old = (uint32)gtmain_signed_divide64((sint64)value * 4096, denominator, 0x8005EBE8u);
    w_u32(d + 144u, old); gtmain_race_engine_boost_initialize(s, d);
    for (i = 0; (sint32)i < r_s16(d); ++i) {
        argument = (sint16)(100u * r_u8(s + 52u + i));
        numerator = gtmain_race_engine_boost_limit(d, (sint16)argument, (sint16)argument);
        denominator = gtmain_race_engine_boost_curve(d, (sint16)argument, (sint16)argument);
        value = parameters_div((uint32)numerator << 12, denominator);
        w_u32(d + 76u + 4u * i, (uint32)parameters_fp64((sint64)value * r_s32(d + 76u + 4u * i)));
    }
}

/* GTMAIN.EXE 8004E9D8 */
void gtmain_race_force_parameters_initialize(uint32 parameters, uint32 enabled)
{
    uint32 d = parameters, old;
    sint32 speed, value, first, second;
    speed = parameters_magic((uint32)r_u16(d + 256u) << 12, 0x88888889u, 5, 1, 1);
    value = r_s32(d + 1124u + 4u * r_u8(d + 1058u));
    old = (uint32)gtmain_signed_divide64((sint64)speed * 4096, value, 0x8004EA54u);
    speed = r_s32(d + 40u); value = r_u8(d + 1056u); w_u32(d + 444u, old);
    if (!value) speed = (sint32)((uint32)speed + (uint32)r_s16(d + 1212u));
    else if (value == 1) speed = (sint32)((uint32)speed + (uint32)r_s16(d + 1214u));
    value = r_s32(d + 1216u); first = parameters_fp64((sint64)value * r_s16(d + 1160u));
    value = r_s32(d + 1220u); second = parameters_fp64((sint64)value * r_s16(d + 1162u));
    value = (sint32)(2u * ((uint32)first + (uint32)second));
    w_u32(d + 1228u, (uint32)gtmain_signed_divide64((sint64)speed * 4096, value, 0x8004EBC4u));
    if (enabled) gtmain_race_top_speed_initialize(d);
}

/* GTMAIN.EXE 80057FD4 */
void gtmain_race_transmission_initialize(uint32 specification, uint32 transmission, uint32 engine_curve)
{
    uint32 s = specification, d = transmission, e = engine_curve, kind, mode, i, old, high, low;
    sint32 value, first, second, inverse_first, inverse_second, factor = 0, maximum, denominator, result;
    sint64 product;
    kind = r_u8(s + 138u);
    if (!kind || kind == 3u) { w_u8(d, 0); w_u8(d + 3u, 1); w_u32(d + 4u, 0); }
    else if (kind == 1u) { w_u8(d, 1); w_u8(d + 3u, 0); w_u32(d + 4u, 0); }
    else if (kind == 2u) {
        mode = r_u8(s + 81u);
        if (!mode) { w_u8(d, 2); w_u8(d + 3u, 0); }
        else if (mode < 3u) { w_u8(d, 3); w_u8(d + 3u, 1); }
        else if (mode == 3u) { w_u8(d, 4); w_u8(d + 3u, 1); }
        /* Other original modes retain the incoming destination bytes */
        w_u32(d + 4u, ((uint32)r_u8(s + 281u) << 12) / 100u);
    }
    w_u8(d + 2u, r_u8(s + 42u));
    for (i = 0; i <= r_u8(d + 2u); ++i) {
        value = r_s16(s + 24u + 2u * i); first = r_s16(s + 40u);
        product = (sint64)value * first * 4096;
        w_u32(d + 36u + 4u * i, (uint32)gtmain_signed_divide64(product, 1000000, 0x80058194u));
    }
    value = (40960 * r_u8(s + 104u) + 20480) / 1000; w_u16(d + 108u, (uint16)value);
    first = (1040384 * r_u8(s + 102u)) / 20000 + ((sint16)value * 5 * r_u8(s + 106u)) / 100;
    w_u16(d + 100u, (uint16)first); inverse_first = parameters_div(16777216u, r_s16(d + 100u));
    value = (40960 * r_u8(s + 105u) + 20480) / 1000; w_u16(d + 110u, (uint16)value);
    second = (1040384 * r_u8(s + 103u)) / 20000 + ((sint16)value * 5 * r_u8(s + 107u)) / 100;
    w_u16(d + 102u, (uint16)second); inverse_second = parameters_div(16777216u, r_s16(d + 102u));
    w_u16(d + 104u, (uint16)inverse_first); w_u16(d + 106u, (uint16)inverse_second);
    for (i = 0; i < 2u; ++i) { value = r_u8(s + 100u + i) - 12; if (value <= 0) value = 1; w_u16(d + 120u + 2u * i, (uint16)value); }
    kind = r_u8(d); w_u32(d + 112u, (uint32)parameters_div(267059200u, 4 * r_u8(s + 43u)));
    if (kind == 0 || kind == 3 || kind == 4) factor = r_s16(d + 122u);
    else if (kind == 1 || kind == 2) factor = r_s16(d + 120u);
    factor = (sint32)((uint32)factor << 10);
    for (i = 0; i <= r_u8(d + 2u); ++i) {
        old = (uint32)r_u8(s + 43u) << 12; value = r_s32(d + 36u + 4u * i);
        value = parameters_fp64((sint64)(sint32)old * value);
        denominator = (sint32)((uint32)factor + (uint32)(value / 100));
        w_u32(d + 124u + 4u * i, (uint32)gtmain_signed_divide64(16777216, denominator, 0x800584ACu));
    }
    for (i = 0; i < 2u; ++i) {
        value = r_s16(d + 104u + 2u * i); first = r_s16(d + 120u + 2u * i);
        value = parameters_fp64((sint64)value * value);
        w_u16(d + 156u + 2u * i, (uint16)parameters_fp64((sint64)first * value));
    }
    w_u16(d + 8u, (uint16)(10u * r_u8(s + 93u))); w_u16(d + 10u, (uint16)(100u * r_u8(s + 49u)));
    maximum = 0;
    for (i = 0; (sint32)i < r_s16(e); ++i) {
        value = r_s32(e + 12u + 4u * i); value = gtmain_race_curve32(e, value); if (maximum < value) maximum = value;
    }
    w_u16(d + 118u, (uint16)((sint32)((uint32)maximum * 15u) / 10));
    w_u16(d + 116u, (uint16)((160564 * r_u8(s + 139u)) / 4096));
    for (i = 0; i <= r_u8(d + 2u); ++i) {
        mode = r_u8(d);
        if (mode == 0 || mode == 3 || mode == 4) first = r_s16(d + 102u);
        else if (mode == 1 || mode == 2) first = r_s16(d + 100u);
        else continue;
        value = r_s32(d + 36u + 4u * i); denominator = (sint32)((uint32)first * 25736u) / 4096;
        w_u32(d + 68u + 4u * i, (uint32)gtmain_signed_divide64((sint64)value * 4096, denominator, 0x800587A4u));
    }
    high = r_u16(d + 8u); low = r_u16(e + 142u);
    if ((sint32)low >= (sint32)(high - 500u)) w_u16(d + 8u, (uint16)(r_u16(e + 142u) + 500u));
    high = r_u16(e + 140u); low = r_u16(d + 10u);
    if ((sint32)low >= (sint32)(high - 500u)) w_u16(d + 10u, (uint16)(r_u16(e + 140u) - 500u));
    for (i = 1; i < r_u8(d + 2u); ++i) {
        w_u16(d + 10u + 2u * i, r_u16(d + 10u));
        value = (sint32)((uint32)r_u16(d + 10u) << 12) / 60;
        first = r_s32(d + 40u + 4u * i); second = r_s32(d + 36u + 4u * i);
        result = (sint32)(uint32)gtmain_signed_divide64((sint64)first * 4096, second, 0x8005888Cu);
        value = parameters_fp64((sint64)value * result); value = (sint32)((uint32)value * 60u) / 4096;
        old = r_u16(d + 8u);
        if ((sint32)(old + 1000u) >= value) w_u16(d + 22u + 2u * i, (uint16)(r_u16(d + 8u) + 500u));
        else w_u16(d + 22u + 2u * i, (uint16)((uint32)value - 500u));
    }
}

