#include "game_first2_registry.h"
#include "psx_gte.h"

static void gtmain_matrix_load(const uint8 *matrix)
{
    uint32 index;
    for (index = 0u; index < 5u; ++index)
        gte_ctc2(xport_load_le32(matrix + index * 4u), index);
}

static void gtmain_matrix_read_column(uint32 column[3])
{
    column[0] = xport_gte_read_data(9u);
    column[1] = xport_gte_read_data(10u);
    column[2] = xport_gte_read_data(11u);
}

static void gtmain_matrix_store_column(uint8 *out, uint32 column_index, const uint32 column[3])
{
    xport_store_le16(out + column_index * 2u, (uint16)column[0]);
    xport_store_le16(out + column_index * 2u + 6u, (uint16)column[1]);
    xport_store_le16(out + column_index * 2u + 12u, (uint16)column[2]);
}

/* Original: GTMAIN.EXE:sub_80070320. */ sint32 gtmain_matrix_multiply(uint8 *out, const uint8 *left, const uint8 *right)
{
    uint32 first[3], second[3], third[3];
    uint32 r0, r1, r2, r3, r4;
    gtmain_matrix_load(left);
    r0 = xport_load_le32(right); r3 = xport_load_le32(right + 12u); r1 = xport_load_le32(right + 4u);
    gte_lwc2(0u, (r0 & 65535u) | (r1 & 0xFFFF0000u)); gte_lwc2(1u, r3);
    xport_gte_execute(0x486012u);
    r2 = xport_load_le32(right + 8u);
    gtmain_matrix_read_column(first);
    gte_lwc2(0u, (r0 >> 16) | (r2 << 16)); gte_lwc2(1u, r3 >> 16);
    xport_gte_execute(0x486012u);
    r4 = xport_load_le32(right + 16u);
    gtmain_matrix_read_column(second);
    gte_lwc2(0u, (r1 & 65535u) | (r2 & 0xFFFF0000u)); gte_lwc2(1u, r4);
    xport_gte_execute(0x486012u);
    gtmain_matrix_store_column(out, 0u, first);
    gtmain_matrix_store_column(out, 1u, second);
    gtmain_matrix_read_column(third);
    gtmain_matrix_store_column(out, 2u, third);
    return (sint32)third[0];
}

/* Original: GTMAIN.EXE:sub_80070410. */ uint8 *gtmain_vector_add(uint8 *out, const uint8 *first, const uint8 *second)
{
    uint32 a[3], b[3], index;
    for (index = 0u; index < 3u; ++index) a[index] = xport_load_le32(first + index * 4u);
    for (index = 0u; index < 3u; ++index) b[index] = xport_load_le32(second + index * 4u);
    for (index = 0u; index < 3u; ++index) xport_store_le32(out + index * 4u, a[index] + b[index]);
    return out;
}

/* Original: GTMAIN.EXE:sub_8007070C. */ uint32 gtmain_matrix_bind_negated_axes(const uint8 *matrix)
{
    uint32 word1 = xport_load_le32(matrix + 4u), word0 = xport_load_le32(matrix);
    uint32 word2, result, word4, word3;
    gte_ctc2(word0, 0u);
    gte_ctc2((word1 ^ 0xFFFF0000u) + 65536u, 1u);
    word2 = xport_load_le32(matrix + 8u);
    result = ((0u - word2) & 65535u) | ((0u - (word2 >> 16)) << 16);
    gte_ctc2(result, 2u);
    word4 = xport_load_le32(matrix + 16u); word3 = xport_load_le32(matrix + 12u);
    gte_ctc2(word3, 3u); gte_ctc2(word4, 4u);
    return result;
}

/* Original: GTMAIN.EXE:sub_800708E8. */ sint32 gtmain_matrix_scale_gte(uint8 *out, const uint8 *matrix, uint16 scale)
{
    uint32 first[3], second[3], third[3];
    gtmain_matrix_load(matrix);
    gte_lwc2(0u, scale); gte_lwc2(1u, 0u);
    xport_gte_execute(0x486012u);
    gte_lwc2(4u, 0u); gte_lwc2(5u, scale);
    gte_lwc2(2u, (uint32)scale << 16); gte_lwc2(3u, 0u);
    gtmain_matrix_read_column(first);
    xport_gte_execute(0x48E012u);
    gtmain_matrix_store_column(out, 0u, first);
    gtmain_matrix_read_column(second);
    xport_gte_execute(0x496012u);
    gtmain_matrix_store_column(out, 1u, second);
    gtmain_matrix_read_column(third);
    gtmain_matrix_store_column(out, 2u, third);
    return (sint32)third[0];
}

/* Original: GTMAIN.EXE:sub_800709A8. */ void gtmain_matrix_shift(uint8 *out, const uint8 *matrix, sint32 shift)
{
    uint32 values[9], index, count;
    if (!shift && out == matrix) return;
    for (index = 0u; index < 9u; ++index) values[index] = (uint32)(sint32)(sint16)xport_load_le16(matrix + index * 2u);
    count = (shift < 0 ? 0u - (uint32)shift : (uint32)shift) & 31u;
    for (index = 0u; index < 9u; ++index)
    {
        uint32 value = values[index];
        if (shift < 0) value <<= count;
        else if (count) value = (value >> count) | ((value & 0x80000000u) ? (~0u << (32u - count)) : 0u);
        xport_store_le16(out + index * 2u, (uint16)value);
    }
}

/* Original: GTMAIN.EXE:sub_80070A60. */ uint32 gtmain_matrix_transpose(uint8 *out, const uint8 *matrix)
{
    uint32 value, other;
    if (out != matrix)
    {
        value = xport_load_le16(matrix); xport_store_le16(out, (uint16)value);
        value = xport_load_le16(matrix + 6u); xport_store_le16(out + 2u, (uint16)value);
        value = xport_load_le16(matrix + 12u); xport_store_le16(out + 4u, (uint16)value);
        value = xport_load_le16(matrix + 2u); xport_store_le16(out + 6u, (uint16)value);
        value = xport_load_le16(matrix + 8u); xport_store_le16(out + 8u, (uint16)value);
        value = xport_load_le16(matrix + 14u); xport_store_le16(out + 10u, (uint16)value);
        value = xport_load_le16(matrix + 4u); xport_store_le16(out + 12u, (uint16)value);
        value = xport_load_le16(matrix + 10u); xport_store_le16(out + 14u, (uint16)value);
        value = xport_load_le16(matrix + 16u); xport_store_le16(out + 16u, (uint16)value);
    }
    else
    {
        /* The original alias branch retains each element in place */
        other = xport_load_le16(matrix + 2u); value = xport_load_le16(matrix + 6u);
        xport_store_le16(out + 2u, (uint16)other); xport_store_le16(out + 6u, (uint16)value);
        other = xport_load_le16(matrix + 4u); value = xport_load_le16(matrix + 12u);
        xport_store_le16(out + 4u, (uint16)other); xport_store_le16(out + 12u, (uint16)value);
        other = xport_load_le16(matrix + 10u); value = xport_load_le16(matrix + 14u);
        xport_store_le16(out + 10u, (uint16)other); xport_store_le16(out + 14u, (uint16)value);
    }
    return value;
}

/* Original: GTMAIN.EXE:sub_80070B0C. */ sint32 gtmain_matrix_rotate_short(uint8 *out, const uint8 *matrix, const uint8 *vector)
{
    uint32 result;
    gtmain_matrix_load(matrix);
    gte_lwc2(0u, xport_load_le32(vector)); gte_lwc2(1u, xport_load_le32(vector + 4u));
    xport_gte_execute(0x486012u);
    xport_store_le32(out, xport_gte_read_data(9u));
    result = xport_gte_read_data(10u);
    xport_store_le32(out + 4u, xport_gte_read_data(11u));
    xport_store_le16(out + 2u, (uint16)result);
    return (sint32)result;
}
