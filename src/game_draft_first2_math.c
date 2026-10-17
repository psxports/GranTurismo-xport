#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 gtmain_math_asr(uint32 value, uint32 shift)
{
    shift &= 31u;
    if (!shift) return value;
    return (value >> shift) | ((value & 0x80000000u) ? (~0u << (32u - shift)) : 0u);
}

uint32 gtmain_model_translation(uint32 context, uint32 vector_nullable)
{
    uint32 first = 0, second = 0, third = 0;
    if (vector_nullable) {
        first = r_u32(vector_nullable); second = r_u32(vector_nullable + 4u);
        gte_ctc2(first, 5u);
        third = r_u32(vector_nullable + 8u);
        gte_ctc2(second, 6u); gte_ctc2(third, 7u);
    } else {
        gte_ctc2(0, 5u); gte_ctc2(0, 6u); gte_ctc2(0, 7u);
    }
    /* Readbacks retain the words just written without intervening GTE operations */
    w_u32(context + 84u, first); w_u32(context + 88u, second); w_u32(context + 92u, third);
    return second;
}

uint32 gtmain_fixed_sqrt(uint32 value, uint32 fractional_bits)
{
    uint32 count, root = 0u, trial = 0u, remainder = 0u, index;
    if (!value) return 0u;
    if (value & 0x80000000u) count = 16u;
    else
    {
        /* Preserve LZCS and LZCR through the existing shared runtime */
        gt_draft_gte_write(30u, 0u, value);
        count = (((31u - gt_draft_gte_read(31u, 0u)) & ~1u) + 2u) >> 1;
    }
    value <<= (32u - count - count) & 31u;
    for (index = 0u; index < count; ++index)
    {
        root *= 2u;
        remainder = (remainder * 4u | (value >> 30)) - (trial * 2u + 1u);
        trial *= 2u;
        value *= 4u;
        if (!(remainder & 0x80000000u)) { ++root; trial += 2u; }
        else remainder += trial + 1u;
    }
    for (index = 0u; index < fractional_bits; ++index)
    {
        trial *= 2u;
        remainder = remainder * 4u - (trial + 1u);
        root *= 2u;
        if (!(remainder & 0x80000000u)) { ++root; trial += 2u; }
        else remainder += trial + 1u;
    }
    return root;
}

uint8 *gtmain_vector_lerp(uint8 *out, const uint8 *first, const uint8 *second, sint32 fraction)
{
    uint32 a[3], result[3], index;
    for (index = 0u; index < 3u; ++index)
    {
        uint32 difference;
        sint64 product;
        a[index] = xport_load_le32(first + index * 4u);
        difference = xport_load_le32(second + index * 4u) - a[index];
        product = (sint64)fraction * (sint32)difference;
        result[index] = a[index] + (uint32)(product >> 12);
    }
    for (index = 0u; index < 3u; ++index) xport_store_le32(out + index * 4u, result[index]);
    return out;
}

sint32 gtmain_model_rotation(uint8 *matrix, sint16 x, sint16 y, sint16 z)
{
    uint32 ax = 0x8009423Cu + (((uint32)x & 4095u) << 1);
    uint32 ay = 0x8009423Cu + (((uint32)y & 4095u) << 1);
    uint32 az = 0x8009423Cu + (((uint32)z & 4095u) << 1);
    sint32 sy = (sint16)r_u16(ay), sx = (sint16)r_u16(ax);
    sint32 cx = (sint16)r_u16(ax + 2048u), cy = (sint16)r_u16(ay + 2048u);
    sint32 sz = (sint16)r_u16(az), cz = (sint16)r_u16(az + 2048u);
    uint32 a = gtmain_math_asr((uint32)(sx * -sy), 12u);
    uint32 b = gtmain_math_asr((uint32)(cx * -sy), 12u);
    uint32 c = gtmain_math_asr((uint32)(sx * cy), 12u);
    uint32 d = gtmain_math_asr((uint32)(cx * cy), 12u);
    uint32 first, second, third;
    gte_ctc2((uint32)cx, 0u); gte_ctc2((uint16)a, 1u);
    gte_ctc2((uint32)cy << 16, 2u); gte_ctc2(0u - (uint32)sx, 3u); gte_ctc2(b, 4u);
    gte_lwc2(0u, (uint16)cz); gte_lwc2(1u, (uint32)sz);
    xport_gte_execute(0x486012u);
    gte_lwc2(2u, (uint16)-sz); gte_lwc2(3u, (uint32)cz);
    xport_store_le16(matrix + 4u, (uint16)c);
    first = xport_gte_read_data(9u); second = xport_gte_read_data(10u); third = xport_gte_read_data(11u);
    xport_store_le16(matrix, (uint16)first); xport_store_le16(matrix + 6u, (uint16)second);
    xport_gte_execute(0x48E012u);
    xport_store_le16(matrix + 12u, (uint16)third); xport_store_le16(matrix + 10u, (uint16)sy);
    xport_store_le16(matrix + 16u, (uint16)d);
    first = xport_gte_read_data(9u); second = xport_gte_read_data(10u); third = xport_gte_read_data(11u);
    xport_store_le16(matrix + 2u, (uint16)first); xport_store_le16(matrix + 8u, (uint16)second);
    xport_store_le16(matrix + 14u, (uint16)third);
    return (sint32)first;
}

sint32 gtmain_angles_lerp(uint8 *out, const uint8 *first, const uint8 *second, sint32 fraction)
{
    uint32 result[3], index;
    const uint32 order[3] = {1u, 0u, 2u};
    for (index = 0u; index < 3u; ++index)
    {
        uint32 component = order[index];
        uint32 a = (uint32)(sint32)(sint16)xport_load_le16(first + component * 2u);
        uint32 b = (uint32)(sint32)(sint16)xport_load_le16(second + component * 2u);
        uint32 difference = gtmain_math_asr((b - a) << 20, 20u);
        result[component] = a + gtmain_math_asr(difference * (uint32)fraction, 12u);
    }
    xport_store_le16(out + 2u, (uint16)result[1]); xport_store_le16(out, (uint16)result[0]);
    xport_store_le16(out + 4u, (uint16)result[2]);
    return (sint32)result[2];
}

uint32 gtmain_model_rotate_vector(uint8 *out, const uint8 *matrix, const uint8 *vector)
{
    uint32 value[3], sum[3], index, result;
    for (index = 0u; index < 5u; ++index) gte_ctc2(xport_load_le32(matrix + index * 4u), index);
    for (index = 0u; index < 3u; ++index) value[index] = xport_load_le32(vector + index * 4u);
    gte_lwc2(0u, (value[0] & 4095u) | ((value[1] & 4095u) << 16)); gte_lwc2(1u, value[2] & 4095u);
    value[0] = gtmain_math_asr(value[0], 12u); value[1] = gtmain_math_asr(value[1], 12u);
    xport_gte_execute(0x486012u); value[2] = gtmain_math_asr(value[2], 12u);
    for (index = 0u; index < 3u; ++index) sum[index] = xport_gte_read_data(9u + index);
    gte_lwc2(0u, (value[0] & 4095u) | ((value[1] & 4095u) << 16)); gte_lwc2(1u, value[2] & 4095u);
    value[0] = gtmain_math_asr(value[0], 12u); value[1] = gtmain_math_asr(value[1], 12u);
    xport_gte_execute(0x406012u); value[2] = gtmain_math_asr(value[2], 12u);
    for (index = 0u; index < 3u; ++index) sum[index] += xport_gte_read_data(25u + index);
    gte_lwc2(0u, (value[0] & 65535u) | (value[1] << 16)); gte_lwc2(1u, value[2]);
    xport_gte_execute(0x406012u);
    for (index = 0u; index < 3u; ++index) value[index] = xport_gte_read_data(25u + index) << 12;
    result = value[1];
    for (index = 0u; index < 3u; ++index) xport_store_le32(out + index * 4u, sum[index] + value[index]);
    return result;
}

uint8 *gtmain_vector_subtract(uint8 *out, const uint8 *first, const uint8 *second)
{
    uint32 result[3], index;
    for (index = 0u; index < 3u; ++index) result[index] = xport_load_le32(first + index * 4u) - xport_load_le32(second + index * 4u);
    for (index = 0u; index < 3u; ++index) xport_store_le32(out + index * 4u, result[index]);
    return out;
}

sint32 gtmain_vector_narrow(uint8 *out, const uint8 *vector)
{
    uint32 value[3], magnitude = 0u, index;
    sint32 shift;
    for (index = 0u; index < 3u; ++index)
    {
        value[index] = xport_load_le32(vector + index * 4u);
        magnitude |= (value[index] & 0x80000000u) ? 0u - value[index] : value[index];
    }
    /* Preserve LZCS and LZCR through the existing shared runtime */
    gt_draft_gte_write(30u, 0u, magnitude);
    shift = (sint32)(19u - gt_draft_gte_read(31u, 0u));
    for (index = 0u; index < 3u; ++index)
        xport_store_le16(out + index * 2u, (uint16)(shift >= 0 ? gtmain_math_asr(value[index], (uint32)shift) : value[index] << ((0u - (uint32)shift) & 31u)));
    return shift;
}

