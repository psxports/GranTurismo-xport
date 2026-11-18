#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static void remaining_math_gpf12(void)
{
    SVECTOR input;
    VECTOR output;
    input.vx = (sint16)xport_gte_read_data(9u);
    input.vy = (sint16)xport_gte_read_data(10u);
    input.vz = (sint16)xport_gte_read_data(11u);
    input.pad = 0;
    gte_gpf12(&input, (sint16)xport_gte_read_data(8u), &output);
}

static void remaining_math_square0(void)
{
    VECTOR input, output;
    input.vx = (sint16)xport_gte_read_data(9u);
    input.vy = (sint16)xport_gte_read_data(10u);
    input.vz = (sint16)xport_gte_read_data(11u);
    input.pad = 0;
    Square0(&input, &output);
}

/* Original: GTMAIN.EXE:sub_80070774. */ sint32 gtmain_matrix_scale_axes(uint8 *out, const uint8 *matrix, const uint8 *scales)
{
    uint32 scale = xport_load_le32(scales), a = xport_load_le32(matrix), b = xport_load_le32(matrix + 4u);
    uint32 high = (uint32)((sint32)a >> 16), c, d, next_scale, component, result;
    xport_gte_write_data(8u, scale); xport_gte_write_data(9u, a);
    xport_gte_write_data(10u, high); xport_gte_write_data(11u, b);
    b = (uint32)((sint32)b >> 16); scale = (uint32)((sint32)scale >> 16);
    remaining_math_gpf12();
    c = xport_load_le32(matrix + 8u); d = xport_load_le32(matrix + 12u);
    high = (uint32)((sint32)c >> 16);
    xport_store_le32(out, xport_gte_read_data(9u)); component = xport_gte_read_data(10u);
    xport_store_le32(out + 4u, xport_gte_read_data(11u));
    xport_gte_write_data(8u, scale); xport_gte_write_data(9u, b);
    xport_gte_write_data(10u, c); xport_gte_write_data(11u, high);
    xport_store_le16(out + 2u, (uint16)component); next_scale = xport_load_le32(scales + 4u);
    remaining_math_gpf12();
    high = (uint32)((sint32)d >> 16); a = xport_load_le32(matrix + 16u);
    component = xport_gte_read_data(9u); xport_store_le32(out + 8u, xport_gte_read_data(10u));
    result = xport_gte_read_data(11u);
    xport_gte_write_data(8u, next_scale); xport_gte_write_data(9u, d);
    xport_gte_write_data(10u, high); xport_gte_write_data(11u, a);
    xport_store_le16(out + 6u, (uint16)component); xport_store_le16(out + 10u, (uint16)result);
    remaining_math_gpf12();
    xport_store_le32(out + 12u, xport_gte_read_data(9u)); component = xport_gte_read_data(10u);
    xport_store_le32(out + 16u, xport_gte_read_data(11u)); xport_store_le16(out + 14u, (uint16)component);
    return (sint32)result;
}

/* Original: GTMAIN.EXE:sub_80070830. */ sint32 gtmain_model_range_index(uint32 ranges, const uint8 *position, sint32 divisor)
{
    sint32 denominator = divisor >> 4;
    sint32 quotient = denominator ? 0x01000000 / denominator : -1;
    uint32 a = xport_load_le32(position), b = xport_load_le32(position + 4u), c = xport_load_le32(position + 8u);
    uint32 count, sum, index = 0, record = ranges + 4u, value;
    sint64 product;
    xport_gte_write_data(9u, (uint32)((sint32)a >> 14));
    xport_gte_write_data(10u, (uint32)((sint32)b >> 14));
    xport_gte_write_data(11u, (uint32)((sint32)c >> 14));
    remaining_math_square0();
    count = r_u16(ranges); a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    sum = a + b + c; product = (sint64)(sint32)sum * (sint64)quotient;
    value = (uint32)((uint64)product >> 12);
    if (!count) return -1;
    do
    {
        uint32 threshold = r_u32(record);
        if ((sint32)value < (sint32)threshold) return (sint32)index;
        ++index; record += 8u;
    }
    while (index < count);
    return -1;
}

/* Original: GTMAIN.EXE:sub_8006B3E0. */ uint32 gtmain_view_projection_prepare(uint32 view)
{
    gtmain_projection_prepare(view);
    return gtmain_view_rectangle_prepare(view);
}

/* Original: GTMAIN.EXE:sub_80078904. */ sint32 gtmain_course_count(void)
{
    return (sint16)r_u16(0x800C0BFCu);
}

/* Original: GTMAIN.EXE:sub_8006E95C. */ sint32 gtmain_course_name_find(uint32 name)
{
    sint32 count = gtmain_course_count(), index = 0;
    if (count <= 0) return -1;
    do
    {
        uint32 candidate = gtmain_course_name((uint32)index);
        if (!gtmain_string_compare(name, candidate)) return index;
        ++index;
    }
    while (index < count);
    return -1;
}

/* Original: GTMAIN.EXE:sub_8006EAE8. */ uint32 gtmain_course_file_environment(void)
{
    return gtmain_environment_value(0x8008E194u);
}
