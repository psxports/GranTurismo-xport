#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 specification_fp64(sint64 product)
{
    return (sint32)(uint32)(product / 4096);
}

static sint32 specification_fp32(uint32 first, uint32 second)
{
    return (sint32)(first * second) / 4096;
}

static sint32 specification_magic(uint32 input, uint32 multiplier, uint32 shift, uint32 add_input, uint32 correct_sign)
{
    sint64 product = (sint64)(sint32)input * (sint32)multiplier;
    uint32 high = (uint32)((uint64)product >> 32);
    if (add_input) high += input;
    return (sint32)((uint32)((sint32)high >> shift) - (correct_sign ? (uint32)((sint32)input >> 31) : 0u));
}

static sint32 specification_div32(uint32 numerator, sint32 denominator)
{
    if (!denominator) gt_draft_exception(7);
    if (denominator == -1 && numerator == 0x80000000u) gt_draft_exception(6);
    return (sint32)numerator / denominator;
}

static void specification_curve_header(uint32 destination, uint32 count, uint32 first, uint32 second)
{
    w_u32(destination + 4u, first); w_u8(destination + 2u, 0); w_u8(destination + 3u, 0);
    w_u32(destination + 8u, second); w_u16(destination, (uint16)count);
}

static void specification_suspension_curve(uint32 destination, uint32 source, uint32 base, uint32 original_link)
{
    uint32 a, b, delta, denominator;
    sint64 product;
    w_u32(destination, (uint32)specification_fp64((sint64)r_u8(source) * 16056400));
    w_u32(destination + 4u, ((uint32)r_u8(source + 4u) << 12) / 100u);
    w_u32(destination + 8u, ((uint32)r_u8(source + 8u) << 12) / 100u);
    w_u32(destination + 28u, ((uint32)r_u8(source + 6u) << 12) / 100u);
    w_u32(destination + 32u, ((uint32)r_u8(source + 10u) << 12) / 100u);
    w_u32(destination + 12u, (uint32)specification_fp32(802820u, r_u8(source + 5u)));
    w_u32(destination + 16u, (uint32)specification_fp32(802820u, r_u8(source + 9u)));
    w_u32(destination + 36u, (uint32)specification_fp32(802820u, r_u8(source + 7u)));
    a = r_u32(destination + 12u);
    w_u32(destination + 40u, (uint32)specification_fp32(802820u, r_u8(source + 11u)));
    b = (uint32)gtmain_signed_divide64((sint64)(sint32)a * 4096, r_s32(destination + 4u), original_link);
    a = r_u32(destination + 16u); w_u32(destination + 20u, b);
    b = (uint32)gtmain_signed_divide64((sint64)(sint32)a * 4096, r_s32(destination + 8u), original_link + 48u);
    product = (sint64)r_s32(destination + 20u) * r_s32(destination + 4u); w_u32(destination + 24u, b);
    a = (uint32)specification_fp64(product);
    product = (sint64)r_s32(destination + 24u) * r_s32(destination + 8u); w_u32(destination + 12u, a);
    a = r_u32(destination + 36u); b = r_u32(destination + 12u);
    w_u32(destination + 16u, (uint32)specification_fp64(product));
    delta = a - b; denominator = r_u32(destination + 28u) - r_u32(destination + 4u);
    b = (uint32)gtmain_signed_divide64((sint64)(sint32)delta * 4096, (sint32)denominator, original_link + 232u);
    a = r_u32(destination + 40u); delta = a - r_u32(destination + 16u); w_u32(destination + 44u, b);
    denominator = r_u32(destination + 32u) - r_u32(destination + 8u);
    b = (uint32)gtmain_signed_divide64((sint64)(sint32)delta * 4096, (sint32)denominator, original_link + 296u);
    product = (sint64)(sint32)(r_u32(destination + 28u) - r_u32(destination + 4u)) * r_s32(destination + 44u);
    w_u32(destination + 48u, b);
    a = (uint32)specification_fp64(product);
    product = (sint64)(sint32)(r_u32(destination + 32u) - r_u32(destination + 8u)) * r_s32(destination + 48u);
    w_u32(destination + 36u, r_u32(destination + 12u) + a);
    w_u32(destination + 40u, r_u32(destination + 16u) + (uint32)specification_fp64(product));
    w_u32(destination + 52u, (uint32)specification_fp32(1605640u, r_u8(source + 1u)));
    w_u32(destination + 56u, (uint32)specification_fp32(16056400u, r_u8(source + 2u)));
    w_u32(destination + 60u, r_u8(source + 3u));
    w_u32(destination + 64u, (uint32)((sint32)(160564u * r_u8(source + 3u)) / 16384));
}

/* GTMAIN.EXE 8004F030 */
/* Original: GTMAIN.EXE:sub_8004F030. */ void gtmain_race_specification_initialize(uint32 destination, uint32 specification, sint32 body, uint32 enabled, uint8 transmission)
{
    uint32 d = destination, s = specification, i, j, count, old, source, output, first_weight, second_weight, sum;
    sint32 value, other, half, weight, strength, slope, axle, numerator, denominator;
    sint64 product;
    gtmain_race_car_kind_set(d, transmission);
    if (body != 4) {
        w_u32(d, (uint32)((r_s16(s + 8u) * 4096) / 1000));
        w_u32(d + 4u, (uint32)((r_s16(s + 10u) * 4096) / 1000));
        value = (r_s16(s + 16u) * 4096) / 1000; w_u16(d + 16u, (uint16)value);
        other = (sint16)value * r_u8(s + 18u); half = other / 100;
        old = r_u16(d + 16u); w_u16(d + 14u, (uint16)half); w_u16(d + 12u, (uint16)(old - (uint32)half));
        weight = (sint32)((uint32)r_u8(s + 18u) << 12);
        value = r_s16(d + 14u); other = r_s16(d + 12u); first_weight = r_u32(d + 4u);
        half = (value - other) / 2; old = r_u32(d);
        w_u32(d + 1048u, (uint32)half); second_weight = r_u32(d + 1048u);
        w_u32(d, old - (uint32)half); w_u32(d + 4u, first_weight + second_weight);
        w_u16(d + 22u, (uint16)(weight / 100)); w_u16(d + 20u, (uint16)(4096 - weight / 100));
        w_u16(d + 8u, (uint16)((r_s16(s + 12u) * 4096) / 1000));
        w_u16(d + 24u, (uint16)((r_s16(s + 20u) * 4096) / 2000));
        value = (r_s16(s + 22u) * 4096) / 2000; old = r_u16(d + 24u);
        w_u16(d + 26u, (uint16)value); w_u16(d + 18u, (uint16)(old + (uint32)value));
        w_u16(d + 10u, (uint16)((r_s16(s + 14u) * 4096) / 1000));
    } else {
        w_u32(d, 40960); w_u32(d + 4u, 40960); w_u16(d + 16u, 16384); old = r_u16(d + 16u);
        w_u16(d + 14u, 8192); w_u16(d + 22u, 2048); w_u16(d + 20u, 2048); w_u16(d + 8u, 14745);
        w_u16(d + 24u, 7372); w_u16(d + 26u, 7372); w_u32(d + 1048u, 0); w_u16(d + 10u, 8192);
        value = (sint16)old / 2; old = r_u16(d + 24u); w_u16(d + 12u, (uint16)value); w_u16(d + 18u, (uint16)(old + 7372u));
    }
    gtmain_race_engine_curve_initialize(s, d + 116u, enabled);
    gtmain_race_transmission_initialize(s, d + 1056u, d + 116u);
    for (j = 0; j < 2u; ++j) {
        source = s + 142u + 17u * j; output = d + 448u + 44u * j;
        count = r_u8(source); w_u8(source + 1u, 0); w_u8(source + 9u, 0);
        for (i = 0; i < count; ++i) {
            value = specification_magic((uint32)r_u8(source + 1u + i) << 10, 0x80808081u, 7, 1, 0);
            w_u16(output + 12u + 2u * i, (uint16)value);
            value = specification_magic((uint32)r_u8(source + 9u + i) << 12, 0x51EB851Fu, 6, 0, 0);
            w_u16(output + 28u + 2u * i, (uint16)value); count = r_u8(source);
        }
        specification_curve_header(output, r_u8(source), output + 12u, output + 28u);
    }
    value = (sint16)gtmain_race_steering_peak(d); w_u16(d + 92u, (uint16)value);
    if (value >= 342) w_u16(d + 92u, 341);
    gtmain_race_specification_curves_build(s + 177u, s + 183u, s + 189u, s + 350u, r_u8(s + 176u),
        s + 196u, s + 202u, s + 208u, s + 356u, r_u8(s + 195u), d + 536u, d + 664u,
        d + 736u, d + 548u, d + 572u, d + 676u, d + 748u, d + 596u, d + 598u);
    gtmain_race_specification_curves_build(s + 215u, s + 221u, s + 227u, s + 362u, r_u8(s + 214u),
        s + 234u, s + 240u, s + 246u, s + 368u, r_u8(s + 233u), d + 600u, d + 700u,
        d + 772u, d + 612u, d + 636u, d + 712u, d + 784u, d + 660u, d + 662u);
    for (j = 0; j < 2u; ++j) {
        source = s + 252u + 13u * j; output = d + 816u + 44u * j;
        count = r_u8(source); w_u8(source + 1u, 0); w_u8(source + 7u, 0);
        for (i = 0; i < count; ++i) {
            value = (1138 * r_u8(source + 1u + i)) / 8; if (value > 32767) value = 32767;
            w_u16(output + 12u + 2u * i, (uint16)value);
            old = r_u8(s + 140u + j); old *= r_u8(source + 7u + i);
            value = specification_magic(old << 12, 0xDB4F5D05u, 16, 1, 0);
            w_u16(output + 28u + 2u * i, (uint16)value); count = r_u8(source);
        }
        specification_curve_header(output, r_u8(source), output + 12u, output + 28u);
    }
    first_weight = r_u8(s + 134u); second_weight = r_u8(s + 135u);
    value = (sint32)(first_weight - 175u);
    strength = specification_magic((uint32)value << 12, 0x51EB851Fu, first_weight >= 175u ? 4u : 5u, 0, 1) + 4096;
    if (strength < 512) strength = 512;
    w_u16(d + 1264u, (uint16)((strength * 8) / 100)); w_u16(d + 1268u, (uint16)((strength * 35) / 100));
    old = r_u8(s + 135u); value = (sint32)(old - 175u);
    strength = specification_magic((uint32)value << 12, 0x51EB851Fu, old >= 175u ? 4u : 5u, 0, 1) + 4096;
    if (strength < 512) strength = 512;
    w_u16(d + 1266u, (uint16)((strength * 8) / 100)); w_u16(d + 1270u, (uint16)((strength * 35) / 100));
    old = r_u8(s + 382u); count = r_u8(s + 383u); if (!old) old = 100; if (!count) count = 100;
    first_weight = (uint32)((sint32)(((first_weight << 12) / 100u) * old) / 100);
    second_weight = (uint32)((sint32)(((second_weight << 12) / 100u) * count) / 100);
    sum = first_weight + second_weight;
    if (body == 2) {
        old = r_u8(s + 138u); first_weight = (uint32)((sint32)(first_weight * r_u8(0x80093C50u + old)) / 100);
        second_weight = (uint32)((sint32)(second_weight * r_u8(0x80093C54u + old)) / 100); sum = first_weight + second_weight;
    }
    w_u32(d + 808u, (uint32)specification_fp64((sint64)((sint32)sum / 2) * 40141));
    old = ((uint32)r_u8(s + 140u) + r_u8(s + 141u)) << 12;
    value = specification_magic(old, 0x1B4E81B5u, 6, 0, 0);
    w_u32(d + 812u, (uint32)specification_fp64((sint64)value * 40141));
    for (j = 0; j < 2u; ++j) {
        source = s + 282u + 9u * j; output = d + 904u + 44u * j; count = r_u8(source); w_u8(source + 1u, 0);
        for (i = 0; i < count; ++i) {
            value = specification_fp32(1605640u, r_u8(source + 1u + i));
            product = (sint64)(sint32)(j ? second_weight : first_weight) * value;
            w_u32(output + 12u + 4u * i, (uint32)value);
            value = specification_magic((uint32)r_u8(source + 5u + i) << 12, 0x51EB851Fu, 6, 0, 0);
            w_u32(output + 28u + 4u * i, (uint32)specification_fp64((sint64)value * specification_fp64(product)));
            count = r_u8(source);
        }
        specification_curve_header(output, r_u8(source), output + 12u, output + 28u);
    }
    for (j = 0; j < 2u; ++j) {
        source = s + 300u + 9u * j; output = d + 992u + 28u * j; count = r_u8(source); w_u8(source + 1u, 0);
        for (i = 0; i < count; ++i) {
            old = 227u * r_u8(source + 1u + i);
            value = specification_magic(old, 0x80808081u, 7, 1, 1); w_u16(output + 12u + 2u * i, (uint16)value);
            value = specification_magic((uint32)r_u8(source + 5u + i) << 12, 0x51EB851Fu, 6, 0, 0);
            w_u16(output + 20u + 2u * i, (uint16)value); count = r_u8(source);
        }
        if (!j) specification_curve_header(output, r_u8(source), output + 12u, output + 20u);
    }
    product = (sint64)r_s16(d + 8u) * r_s16(d + 10u); count = r_u8(s + 309u);
    specification_curve_header(d + 1020u, count, d + 1032u, d + 1040u); axle = specification_fp64(product);
    for (i = 0; i < 3u; ++i) {
        static const uint32 source_offsets[3] = {98, 136, 137};
        value = specification_magic((uint32)r_u8(s + source_offsets[i]) << 12, 0x51EB851Fu, 5, 0, 0);
        half = r_s32(0x80093D7Cu) / 2; value = specification_fp64((sint64)half * value);
        w_u32(d + 1252u + 4u * i, (uint32)specification_fp64((sint64)axle * value));
    }
    old = r_u32(d + 1260u) + r_u32(d + 1256u);
    w_u32(d + 1252u, r_u32(d + 1252u) + (uint32)((sint32)old / 10));
    w_u32(d + 1216u, (uint32)specification_fp32(1605640u, r_u8(s + 96u)));
    w_u32(d + 1220u, (uint32)specification_fp32(1605640u, r_u8(s + 97u)));
    w_u32(d + 1224u, (uint32)specification_fp32(1605640u, r_u8(s + 99u)));
    old = r_u8(s + 46u); value = old ? (sint32)(160u - old) : 70; w_u32(d + 1232u, (uint32)((value * 4096) / 10));
    old = r_u8(s + 47u); value = old ? (sint32)(160u - old) : 70; w_u32(d + 1236u, (uint32)((value * 4096) / 10));
    w_u16(d + 52u, (uint16)specification_magic((uint32)r_u8(s + 94u) << 12, 0xB60B60B7u, 8, 1, 0));
    w_u16(d + 54u, (uint16)specification_magic((uint32)r_u8(s + 95u) << 12, 0xB60B60B7u, 8, 1, 0));
    w_u16(d + 94u, (uint16)specification_magic(40960u * r_u8(s + 280u), 0xB60B60B7u, 8, 1, 0));
    for (i = 0; i < r_u8(s + 68u); ++i) {
        w_u16(d + 68u + 2u * i, (uint16)((uint32)r_u8(s + 69u + i) << 7));
        w_u16(d + 80u + 2u * i, (uint16)specification_magic((uint32)r_u8(s + 75u + i) << 12, 0x91A2B3C5u, 10, 1, 0));
    }
    specification_curve_header(d + 56u, r_u8(s + 68u), d + 68u, d + 80u);
    value = specification_magic((uint32)r_u8(s + 108u) << 12, 0xB60B60B7u, 8, 1, 1);
    w_u16(d + 1052u, (uint16)(-value / 10));
    value = specification_magic((uint32)r_u8(s + 109u) << 12, 0xB60B60B7u, 8, 1, 1);
    w_u16(d + 1054u, (uint16)(-value / 10));
    old = (uint32)gtmain_signed_divide64(2670592, 400u * r_u8(s + 87u), 0x800502E0u);
    half = r_s16(d + 12u); w_u32(d + 96u, old);
    old = (uint32)gtmain_signed_divide64((sint64)half * 4096, 400u * r_u8(s + 88u), 0x80050328u);
    half = r_s16(d + 14u); w_u32(d + 100u, old);
    old = (uint32)gtmain_signed_divide64((sint64)half * 4096, 400u * r_u8(s + 88u), 0x80050370u);
    half = r_s16(d + 24u); w_u32(d + 104u, old);
    old = (uint32)gtmain_signed_divide64((sint64)half * 4096, 400u * r_u8(s + 89u), 0x800503B8u);
    half = r_s16(d + 26u); w_u32(d + 108u, old);
    w_u32(d + 112u, (uint32)gtmain_signed_divide64((sint64)half * 4096, 400u * r_u8(s + 89u), 0x80050400u));
    specification_suspension_curve(d + 300u, s + 110u, d, 0x800505F4u);
    specification_suspension_curve(d + 376u, s + 122u, d, 0x80050A6Cu);
    value = specification_magic((uint32)r_u8(s + 278u) << 12, 0x10624DD3u, 6, 0, 0); w_u16(d + 36u, (uint16)value);
    value = r_s16(d + 36u) * r_s16(d + 22u);
    w_u16(d + 38u, (uint16)specification_magic((uint32)r_u8(s + 279u) << 12, 0x10624DD3u, 6, 0, 0));
    w_u16(d + 34u, (uint16)(value / 4096 + r_s16(d + 38u) * r_s16(d + 20u) / 4096));
    value = ((5 * r_u8(s + 92u) - 150) * 4096) / 1000; if ((uint32)value & 32768u) value = 0;
    denominator = r_s16(d + 18u); numerator = (sint32)(((uint32)value + r_u16(d + 34u)) << 16) >> 4;
    half = specification_div32((uint32)numerator, denominator); denominator = r_s16(d + 16u);
    other = specification_div32((uint32)numerator, denominator); w_u16(d + 30u, (uint16)(half / 2)); w_u16(d + 32u, (uint16)(other / 2));
    value = r_s16(s + 90u) * 4; w_u32(d + 40u, (uint32)value);
    old = (uint32)gtmain_signed_divide64(16777216, value, 0x80050E6Cu);
    product = (sint64)r_s32(d + 40u) * 40141; w_u32(d + 44u, old);
    first_weight = r_u32(d + 360u); second_weight = r_u32(d + 436u); w_u32(d + 1240u, (uint32)specification_fp64(product));
    axle = (sint32)(r_u32(d + 40u) - 2u * (first_weight + second_weight));
    old = (uint32)gtmain_signed_divide64(16777216, axle, 0x80050ED4u); product = (sint64)axle * 40141; w_u32(d + 48u, old);
    value = specification_fp64(product); half = r_s16(d + 22u); product = (sint64)value * half;
    w_u32(d + 1244u, (uint32)specification_fp64(product));
    value = specification_fp64((sint64)axle * 40141); half = r_s16(d + 20u); product = (sint64)value * half;
    denominator = r_s32(d + 40u); weight = specification_div32((uint32)axle << 12, denominator);
    value = specification_fp32((uint32)weight, (uint32)r_s16(d + 30u));
    w_u32(d + 1248u, (uint32)specification_fp64(product));
    other = specification_fp32((uint32)weight, (uint32)r_s16(d + 32u)); w_u16(d + 30u, (uint16)value);
    value = r_s32(d + 1244u); w_u16(d + 32u, (uint16)other);
    old = (uint32)gtmain_signed_divide64((sint64)(value / 2) * 4096, r_s32(d + 300u), 0x800510E4u);
    value = r_s32(d + 1248u); w_u16(d + 298u, (uint16)(0u - old));
    old = (uint32)gtmain_signed_divide64((sint64)(value / 2) * 4096, r_s32(d + 376u), 0x80051124u);
    slope = r_s32(d + 356u); w_u16(d + 374u, (uint16)(0u - old));
    for (j = 0; j < 2u; ++j) {
        output = d + 300u + 76u * j; if (j) slope = r_s32(output + 56u);
        if (slope) {
            strength = r_s32(output); product = (sint64)strength * strength;
            old = r_u32(d + 1244u + 4u * j); value = (sint32)(0u - old); value /= 2;
            numerator = specification_fp64(product); product = (sint64)slope * value;
            old = (uint32)numerator - 4u * (uint32)specification_fp64(product);
            /* Use the canonical square root with the existing shared LZ binding */
            old = gtmain_fixed_sqrt(old, 6); old -= (uint32)strength;
            old = (uint32)gtmain_signed_divide64((sint64)(uint64)old * 4096, (sint32)((uint32)slope * 2u),
                j ? 0x800512B4u : 0x800511E8u);
            w_u16(output - 2u, (uint16)(0u - old));
        }
    }
    value = r_s32(d + 364u); half = r_s32(d + 300u); old = r_u16(d + 36u);
    first_weight = r_u16(d + 298u); other = r_u16(d + 1158u);
    w_u16(d + 296u, (uint16)(r_u16(d + 1156u) - old + first_weight));
    w_u16(d + 372u, (uint16)(r_u16(d + 1158u) - r_u16(d + 38u) + r_u16(d + 374u)));
    second_weight = r_u16(d + 372u) - (uint32)other;
    w_u16(d + 292u, (uint16)(r_u16(d + 296u) - r_u16(d + 1156u))); w_u16(d + 368u, (uint16)second_weight);
    old = (uint32)gtmain_signed_divide64((sint64)value * 4096, half, 0x80051338u);
    value = r_s32(d + 440u); w_u16(d + 294u, (uint16)old);
    w_u16(d + 370u, (uint16)gtmain_signed_divide64((sint64)value * 4096, r_s32(d + 376u), 0x80051368u));
    gtmain_race_force_parameters_initialize(d, enabled);
}

