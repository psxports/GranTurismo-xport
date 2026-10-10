#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 direction_scale(uint32 value, sint32 shift)
{
    if (shift < 0)
        return value << ((0u - (uint32)shift) & 31u);
    return (uint32)((sint32)value >> ((uint32)shift & 31u));
}

static uint32 direction_divide(uint32 numerator, uint32 denominator)
{
    if (!denominator)
        gt_draft_exception(7u);
    if (denominator == 0xFFFFFFFFu && numerator == 0x80000000u)
        gt_draft_exception(6u);
    return (uint32)((sint32)numerator / (sint32)denominator);
}

/* Unverified translation from complete pseudocode and original MIPS */
uint32 gtmain_camera_direction_matrix(uint8 *matrix, const uint8 *vector)
{
    uint32 x = xport_load_le32(vector);
    uint32 y = xport_load_le32(vector + 4u);
    uint32 z = xport_load_le32(vector + 8u);
    uint32 ax = (sint32)x < 0 ? 0u - x : x;
    uint32 ay = (sint32)y < 0 ? 0u - y : y;
    uint32 az = (sint32)z < 0 ? 0u - z : z;
    uint32 sx, sy, sz, square_x, square_y, square_z, root, horizontal;
    uint32 pitch_sine = 0u, pitch_cosine = 4096u;
    uint32 yaw_sine = 0u, yaw_cosine = 4096u, result;
    sint32 shift;

    xport_gte_write_data(30u, ax | ay | az);
    shift = (sint32)(17u - xport_gte_read_data(31u));
    sx = direction_scale(x, shift);
    sy = direction_scale(y, shift);
    sz = direction_scale(z, shift);
    gte_lwc2(9u, sx);
    gte_lwc2(10u, sy);
    gte_lwc2(11u, sz);
    xport_gte_execute(0xA00428u);
    square_x = xport_gte_read_data(25u);
    square_y = xport_gte_read_data(26u);
    square_z = xport_gte_read_data(27u);
    root = gtmain_fixed_sqrt(square_x + square_y + square_z, 0u);
    if (root)
    {
        horizontal = gtmain_fixed_sqrt(square_x + square_z, 0u);
        pitch_sine = direction_divide(sy << 12, root);
        pitch_cosine = direction_divide(horizontal << 12, root);
    }
    xport_gte_write_data(30u, ax | az);
    shift = (sint32)(17u - xport_gte_read_data(31u));
    sz = direction_scale(z, shift);
    sx = direction_scale(x, shift);
    gte_lwc2(9u, sx);
    gte_lwc2(10u, sz);
    /* The second SQ leaves IR3 from the first operation unchanged */
    xport_gte_execute(0xA00428u);
    square_x = xport_gte_read_data(25u);
    square_z = xport_gte_read_data(26u);
    root = gtmain_fixed_sqrt(square_x + square_z, 0u);
    if (root)
    {
        yaw_sine = direction_divide(sx << 12, root);
        yaw_cosine = direction_divide(sz << 12, root);
    }
    xport_store_le16(matrix, (uint16)yaw_cosine);
    xport_store_le16(matrix + 6u, 0u);
    xport_store_le16(matrix + 8u, (uint16)pitch_cosine);
    xport_store_le16(matrix + 10u, (uint16)pitch_sine);
    xport_store_le16(matrix + 12u, (uint16)(0u - yaw_sine));
    xport_store_le16(matrix + 2u, (uint16)(0u - (uint32)((sint32)(yaw_sine * pitch_sine) >> 12)));
    xport_store_le16(matrix + 4u, (uint16)((sint32)(yaw_sine * pitch_cosine) >> 12));
    xport_store_le16(matrix + 14u, (uint16)(0u - (uint32)((sint32)(yaw_cosine * pitch_sine) >> 12)));
    result = (uint32)((sint32)(yaw_cosine * pitch_cosine) >> 12);
    xport_store_le16(matrix + 16u, (uint16)result);
    return result;
}
