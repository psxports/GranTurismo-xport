#include "game_first2_registry.h"

sint32 gtmain_angle_asin(sint16 value)
{
    uint32 index;
    if (value < -4095) return -1024;
    if (value >= 4096) return 1024;
    if (value >= 0) return (sint16)r_u16(0x800AD9B4u + (uint32)value * 2u);
    index = (uint32)(-(sint32)value);
    return (sint16)(0u - r_u16(0x800AD9B4u + index * 2u));
}

sint32 gtmain_angle_atan(sint16 x, sint16 y)
{
    uint32 magnitude_y, magnitude_x, ratio, quadrant = 0u;
    sint32 sign, offset;
    uint32 angle;
    if (!((sint32)x | (sint32)y)) return 0;
    magnitude_y = y < 0 ? (uint32)(-(sint32)y) : (uint32)y;
    magnitude_x = x < 0 ? (uint32)(-(sint32)x) : (uint32)x;
    if (magnitude_y < magnitude_x) ratio = (magnitude_y << 12) / magnitude_x;
    else ratio = (magnitude_x << 12) / magnitude_y;
    if (x < 0) quadrant = 4u;
    if (y < 0) quadrant |= 2u;
    if (magnitude_y < magnitude_x) quadrant |= 1u;
    offset = (sint16)r_u16(0x80099988u + quadrant * 2u);
    sign = (sint8)r_u8(0x80099980u + quadrant);
    angle = (uint32)(sint32)(sint16)r_u16(0x800AF9B8u + ratio * 2u);
    return (sint16)((angle ^ (uint32)sign) + (uint32)offset);
}

sint32 gtmain_matrix_angles(uint8 *out, const uint8 *matrix)
{
    sint32 absolute, branch, first, second, third = 0, result;
    result = gtmain_angle_asin((sint16)xport_load_le16(matrix + 10u));
    xport_store_le16(out, (uint16)result);
    absolute = (sint16)xport_load_le16(matrix + 14u);
    if (absolute < 0) absolute = -absolute;
    branch = absolute < 4096 ? (sint16)r_u16(0x80097488u + (uint32)absolute * 2u) : 0;
    if (!branch)
    {
        first = (sint16)(0u - xport_load_le16(matrix + 2u));
        second = (sint16)(0u - xport_load_le16(matrix + 14u));
    }
    else
    {
        first = (sint16)xport_load_le16(matrix + 6u);
        second = (sint16)xport_load_le16(matrix + 8u);
        third = gtmain_angle_atan((sint16)first, (sint16)second);
        first = (sint16)xport_load_le16(matrix + 4u);
        second = (sint16)xport_load_le16(matrix + 16u);
    }
    result = gtmain_angle_atan((sint16)first, (sint16)second);
    xport_store_le16(out + 2u, (uint16)result);
    xport_store_le16(out + 4u, (uint16)third);
    return result;
}

sint32 gtmain_model_orientation(uint32 context, uint32 reference_nullable)
{
    uint8 *angles = (uint8 *)psx_addr(context + 132u, 6u);
    const uint8 *rotation = (const uint8 *)psx_addr(context + 140u, 18u);
    uint8 *inverse = (uint8 *)psx_addr(context + 160u, 20u);
    sint32 result;
    gtmain_matrix_angles(angles, rotation);
    result = (sint32)gtmain_matrix_transpose(inverse, rotation);
    if (reference_nullable)
        result = gtmain_matrix_multiply(inverse, (const uint8 *)psx_addr(reference_nullable, 20u), inverse);
    return result;
}
