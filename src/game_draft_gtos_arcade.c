#include "game_draft_registry.h"
#include "psx_spu.h"
#include "psx_gte.h"
#include <string.h>
#include <stdio.h>

static const uint32 gtos_font_glyphs[4] = { 0x800532CCu, 0x8005574Cu, 0x8005B64Cu, 0x80060F4Cu };
static const uint32 gtos_font_kerning[4] = { 0x8005534Cu, 0x8005B24Cu, 0x80060B4Cu, 0x80066DCCu };

static uint32 gtos_leading_sign_bits(uint32 value)
{
    uint32 count = 0u;
    if (value & 0x80000000u) value = ~value;
    while (count < 32u && !(value & 0x80000000u)) { ++count; value <<= 1u; }
    return count;
}
void gtos_model_transform_identity(uint8 transform[40])
{
    memset(transform, 0, 40u);
    xport_store_le16(transform + 20u, 4096u);
    xport_store_le16(transform + 28u, 4096u);
    xport_store_le16(transform + 36u, 4096u);
}

void gtos_model_rotate_vector(uint32 output[3], const uint8 matrix[20], const uint32 input[3])
{
    uint32 low[3], middle[3], high[3], result[3], i;
    /* Split fixed-point coordinates before GTE rotation to retain their full range */
    for (i = 0u; i < 3u; ++i) {
        low[i] = input[i] & 4095u;
        middle[i] = ((uint32)((sint32)input[i] >> 12)) & 4095u;
        high[i] = (uint32)((sint32)input[i] >> 24);
    }
    gte_SetRotMatrix(matrix);
    gte_lwc2(0u, low[0] | (low[1] << 16u));
    gte_lwc2(1u, low[2]);
    xport_gte_execute(0x486012u);
    for (i = 0u; i < 3u; ++i) result[i] = xport_gte_read_data(9u + i);
    gte_lwc2(0u, middle[0] | (middle[1] << 16u));
    gte_lwc2(1u, middle[2]);
    xport_gte_execute(0x406012u);
    for (i = 0u; i < 3u; ++i) result[i] += xport_gte_read_data(25u + i);
    gte_lwc2(0u, (high[0] & 65535u) | (high[1] << 16u));
    gte_lwc2(1u, high[2]);
    xport_gte_execute(0x406012u);
    for (i = 0u; i < 3u; ++i) output[i] = result[i] + (xport_gte_read_data(25u + i) << 12u);
}

void gtos_model_rotation(uint8 matrix[20], sint32 angle_x, sint32 angle_y, sint32 angle_z)
{
    uint32 table_x = 0x8004FB58u + 2u * ((uint32)angle_x & 4095u);
    uint32 table_y = 0x8004FB58u + 2u * ((uint32)angle_y & 4095u);
    uint32 table_z = 0x8004FB58u + 2u * ((uint32)angle_z & 4095u);
    sint32 sx = (sint16)r_u16(table_x), cx = (sint16)r_u16(table_x + 2048u);
    sint32 sy = (sint16)r_u16(table_y), cy = (sint16)r_u16(table_y + 2048u);
    sint32 sz = (sint16)r_u16(table_z), cz = (sint16)r_u16(table_z + 2048u);
    uint32 first[3], second[3], i;
    /* Use the game's lookup table and original intermediate rounding */
    gte_ctc2((uint32)cx, 0u);
    gte_ctc2((uint16)((sx * -sy) >> 12), 1u);
    gte_ctc2((uint32)cy << 16u, 2u);
    gte_ctc2((uint32)-sx, 3u);
    gte_ctc2((uint32)((cx * -sy) >> 12), 4u);
    gte_lwc2(0u, (uint16)cz);
    gte_lwc2(1u, (uint32)sz);
    xport_gte_execute(0x486012u);
    gte_lwc2(2u, (uint16)-sz);
    gte_lwc2(3u, (uint32)cz);
    for (i = 0u; i < 3u; ++i) first[i] = xport_gte_read_data(9u + i);
    xport_store_le16(matrix + 4u, (uint16)((sx * cy) >> 12));
    xport_store_le16(matrix, (uint16)first[0]);
    xport_store_le16(matrix + 6u, (uint16)first[1]);
    xport_gte_execute(0x48E012u);
    xport_store_le16(matrix + 12u, (uint16)first[2]);
    xport_store_le16(matrix + 10u, (uint16)sy);
    xport_store_le16(matrix + 16u, (uint16)((cx * cy) >> 12));
    for (i = 0u; i < 3u; ++i) second[i] = xport_gte_read_data(9u + i);
    xport_store_le16(matrix + 2u, (uint16)second[0]);
    xport_store_le16(matrix + 8u, (uint16)second[1]);
    xport_store_le16(matrix + 14u, (uint16)second[2]);
}

void gtos_model_transpose(uint8 output[20], const uint8 input[20])
{
    uint32 row, column;
    /* The original same-address branch writes each off-diagonal value back unchanged */
    if (output == input) return;
    for (row = 0u; row < 3u; ++row)
        for (column = 0u; column < 3u; ++column)
            xport_store_le16(output + 6u * row + 2u * column,
                xport_load_le16(input + 6u * column + 2u * row));
}

void gtos_model_multiply(uint8 output[20], const uint8 left[20], const uint8 right[20])
{
    uint32 column, row, results[3][3];
    uint32 packed[5];
    for (column = 0u; column < 5u; ++column) packed[column] = xport_load_le32(right + 4u * column);
    gte_SetRotMatrix(left);
    gte_lwc2(0u, (packed[0] & 65535u) | (packed[1] & 0xFFFF0000u));
    gte_lwc2(1u, packed[3]);
    xport_gte_execute(0x486012u);
    for (row = 0u; row < 3u; ++row) results[0][row] = xport_gte_read_data(9u + row);
    gte_lwc2(0u, (packed[0] >> 16u) | (packed[2] << 16u));
    gte_lwc2(1u, packed[3] >> 16u);
    xport_gte_execute(0x486012u);
    for (row = 0u; row < 3u; ++row) results[1][row] = xport_gte_read_data(9u + row);
    gte_lwc2(0u, (packed[1] & 65535u) | (packed[2] & 0xFFFF0000u));
    gte_lwc2(1u, packed[4]);
    xport_gte_execute(0x486012u);
    for (row = 0u; row < 3u; ++row) results[2][row] = xport_gte_read_data(9u + row);
    for (column = 0u; column < 3u; ++column)
        for (row = 0u; row < 3u; ++row)
            xport_store_le16(output + 6u * row + 2u * column, (uint16)results[column][row]);
}

uint32 gtos_model_wheels(uint32 context, uint32 model, sint32 roll, sint32 steering, sint32 yaw)
{
    uint32 index;
    for (index = 0u; index < 4u; ++index) {
        uint32 dimensions = model + ((index & 2u) ? 52u : 48u);
        uint32 position = model + 16u + 8u * index;
        sint32 width = (sint16)r_u16(dimensions), radius = (sint16)r_u16(dimensions + 2u);
        sint32 scale, angle = (index & 2u) ? 0 : steering;
        sint32 offset;
        if (!radius) {
            gt_draft_unresolved_call(0x8002D798u, context, model, index, (uint32)width, 0u, 0u, 0u, 0u);
            return 0u;
        }
        scale = (sint32)((uint32)width << 12u) / radius;
        if (index & 1u) { angle -= 1024; roll = (sint32)(0u - (uint32)roll); }
        else angle += 1024;
        gtos_model_rotation(psx_addr(context + 960u, 20u), (sint16)angle, (sint16)yaw, (sint16)roll);
        if (index & 1u) roll = (sint32)(0u - (uint32)roll);
        gtos_model_wheel_scale(context, scale);
        offset = (index & 1u) ? -width >> 1 : width >> 1;
        gtos_model_wheel_position(context, (sint16)r_u16(position) + offset,
            (sint16)r_u16(position + 2u), (sint16)r_u16(position + 4u));
        gtos_model_wheel_draw(context, (sint32)((uint32)radius << 4u), 0u);
    }
    return 0u;
}

uint32 gtos_model_wheel_draw(uint32 context, sint32 radius, uint32 kind)
{
    uint32 displacement[3], combined = 0u, i, table, cursor, packet, count, words[6], result;
    sint32 radius_bits, highest, shift, model_shift, scale_shift, reflection;
    for (i = 0u; i < 3u; ++i) {
        displacement[i] = r_u32(context + 940u + 4u * i) - r_u32(context + 44u + 4u * i);
        w_u32(context + 268u + 4u * i, displacement[i]);
        combined |= (sint32)displacement[i] < 0 ? 0u - displacement[i] : displacement[i];
    }
    radius_bits = 31 - (sint32)gtos_leading_sign_bits((uint32)radius);
    highest = 31 - (sint32)gtos_leading_sign_bits(combined);
    if (highest < radius_bits + 1) highest = radius_bits + 1;
    model_shift = highest - radius_bits - 1; shift = highest - 13;
    if (shift < 0) shift = 0;
    w_u16(context + 40u, shift); w_u16(context + 42u, model_shift);
    for (i = 0u; i < 3u; ++i) w_u16(context + 280u + 2u * i, (sint32)displacement[i] >> ((uint32)shift & 31u));
    scale_shift = radius_bits - 12;
    if (scale_shift > 0) radius >>= (uint32)scale_shift & 31u;
    else if (scale_shift < 0) radius = (sint32)((uint32)radius << ((0u - (uint32)scale_shift) & 31u));
    gtos_model_transpose(psx_addr(context + 248u, 20u), psx_addr(context + 960u, 20u));
    gtos_model_scale(psx_addr(context + 248u, 20u), psx_addr(context + 248u, 20u), (sint16)(0u - (uint32)radius));
    gtos_model_rotate_short(psx_addr(context + 56u, 8u), psx_addr(context + 248u, 20u), psx_addr(context + 280u, 8u));
    gtos_model_geometry_rotation(psx_addr(context + 64u, 20u));
    gte_lwc2(0u, r_u32(context + 280u)); gte_lwc2(1u, r_u32(context + 284u));
    xport_gte_execute(0x486012u);
    for (i = 0u; i < 3u; ++i) {
        uint32 value = xport_gte_read_data(9u + i);
        w_u32(context + 84u + 4u * i, value); gte_ctc2(value, 5u + i);
    }
    gtos_model_multiply(psx_addr(context + 96u, 20u), psx_addr(context + 64u, 20u), psx_addr(context + 960u, 20u));
    gtos_model_scale(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), (sint16)radius);
    gtos_model_shift(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), model_shift);
    gtos_model_bind_geometry_rotation(context, psx_addr(context + 96u, 20u));
    gtos_model_clip_planes(context);
    table = r_u32(0x8006D668u + 4u * kind); result = table;
    count = (uint32)(sint32)(sint16)r_u16(table);
    if (!count) return result;
    cursor = table + 4u; packet = r_u32(0x8007A024u); reflection = (sint16)r_u16(context + 908u);
    for (i = 0u; i < 6u; ++i) words[i] = r_u32(cursor + 4u * i);
    do {
        PsxGteSnapshot first, fourth;
        uint32 next[4], fourth_z, depths, winding, link, old;
        sint32 depth;
        gte_lwc2(0u, words[2]); gte_lwc2(1u, words[3]);
        gte_lwc2(2u, words[4]); gte_lwc2(3u, words[5]);
        gte_lwc2(4u, words[0]); gte_lwc2(5u, words[1]);
        /* Preserve SWL writes that also touch the preceding texture byte */
        for (i = 0u; i < 3u; ++i) w_u8(packet + 11u + i, words[1] >> (8u + 8u * i));
        for (i = 0u; i < 3u; ++i) w_u8(packet + 19u + i, words[3] >> (8u + 8u * i));
        xport_gte_execute(0x280030u);
        for (i = 0u; i < 4u; ++i) next[i] = r_u32(cursor + 32u + 4u * i);
        fourth_z = r_u32(cursor + 28u); psx_gte_snapshot(&first);
        w_u32(packet + 16u, xport_gte_read_data(12u));
        depths = xport_gte_read_data(17u) + xport_gte_read_data(18u) + xport_gte_read_data(19u);
        for (i = 0u; i < 3u; ++i) w_u8(packet + 35u + i, words[5] >> (8u + 8u * i));
        for (i = 0u; i < 3u; ++i) w_u8(packet + 27u + i, fourth_z >> (8u + 8u * i));
        xport_gte_execute(0x1400006u);
        gte_lwc2(0u, r_u32(cursor + 24u)); gte_lwc2(1u, fourth_z);
        winding = xport_gte_read_data(24u);
        w_u32(packet + 4u, 0x2D000000u); w_u32(packet, 0x09000000u);
        w_u16(packet + 14u, r_u16(context + 936u)); w_u16(packet + 22u, r_u16(context + 938u));
        if (!reflection) winding = 0u - winding;
        xport_gte_execute(0x180001u);
        words[4] = r_u32(cursor + 48u); words[5] = r_u32(cursor + 52u);
        psx_gte_snapshot(&fourth);
        result = (uint32)first.flag | (uint32)fourth.flag;
        depths += xport_gte_read_data(19u);
        xport_gte_execute(0x1400006u);
        if ((sint32)result >= 0) {
            depth = (sint32)(depths << (r_u16(context + 40u) & 31u)) >> 15;
            link = r_u32(context + 928u) + (depth >= 4096 ? 16380u : (uint32)depth << 2u);
            result = xport_gte_read_data(24u);
            w_u32(packet + 32u, xport_gte_read_data(12u));
            if (reflection) result = 0u - result;
            result &= winding;
            w_u32(packet + 8u, xport_gte_read_data(13u));
            if ((sint32)result >= 0) {
                w_u32(packet + 24u, xport_gte_read_data(14u));
                old = r_u32(link) & 0xFFFFFFu;
                w_u32(link, (r_u32(link) & 0xFF000000u) | (packet & 0xFFFFFFu));
                w_u32(packet, 0x09000000u | old); result = packet << 8u; packet += 40u;
            }
        }
        for (i = 0u; i < 4u; ++i) words[i] = next[i];
        cursor += 32u;
    } while (--count);
    w_u32(0x8007A024u, packet);
    return result;
}

void gtos_model_wheel_scale(uint32 context, sint32 scale)
{
    uint32 i;
    /* GPF scales signed IR components by unsigned IR0 with a twelve-bit shift */
    for (i = 0u; i < 3u; ++i) {
        sint32 value = (sint32)(((sint64)(uint16)scale *
            (sint16)r_u16(context + 964u + 6u * i)) >> 12);
        if (value < -32768) value = -32768;
        if (value > 32767) value = 32767;
        w_u16(context + 964u + 6u * i, (uint16)value);
    }
    gtos_model_multiply(psx_addr(context + 960u, 20u), psx_addr(context + 884u, 20u), psx_addr(context + 960u, 20u));
}

uint32 gtos_model_wheel_position(uint32 context, sint32 x, sint32 y, sint32 z)
{
    uint32 vector[3] = { (uint32)x << 4u, (uint32)y << 4u, (uint32)z << 4u }, i;
    for (i = 0u; i < 3u; ++i) w_u32(context + 940u + 4u * i, vector[i]);
    gtos_model_rotate_vector(vector, psx_addr(context + 884u, 20u), vector);
    for (i = 0u; i < 3u; ++i) {
        w_u32(context + 940u + 4u * i, vector[i]);
    }
    for (i = 0u; i < 3u; ++i) w_u32(context + 940u + 4u * i, vector[i] + r_u32(context + 864u + 4u * i));
    return r_u32(context + 944u);
}

sint32 gtos_model_prepare(uint32 context, uint32 levels, uint32 mode, const uint8 *camera)
{
    uint32 displacement[3], magnitude[3], combined, metric, range, model, clipping, i, temporary;
    sint32 detail, highest, precision, model_shift, coordinate_shift, scale = 4096;
    for (i = 0u; i < 3u; ++i) {
        displacement[i] = r_u32(context + 864u + 4u * i) - r_u32(context + 44u + 4u * i);
        w_u32(context + 268u + 4u * i, displacement[i]);
        magnitude[i] = (sint32)displacement[i] < 0 ? 0u - displacement[i] : displacement[i];
    }
    combined = magnitude[0] | magnitude[1] | magnitude[2];
    if ((sint32)magnitude[0] < (sint32)magnitude[1]) { temporary = magnitude[0]; magnitude[0] = magnitude[1]; magnitude[1] = temporary; }
    if ((sint32)magnitude[0] < (sint32)magnitude[2]) { temporary = magnitude[0]; magnitude[0] = magnitude[2]; magnitude[2] = temporary; }
    if ((sint32)magnitude[1] < (sint32)magnitude[2]) { temporary = magnitude[1]; magnitude[1] = magnitude[2]; magnitude[2] = temporary; }
    metric = magnitude[0] + (uint32)((sint32)magnitude[1] >> 1) + (uint32)((sint32)magnitude[2] >> 2);
    range = xport_load_le32(camera + 28u) << 4u;
    detail = gtos_model_select_detail(levels, displacement, (sint32)range);
    if (detail == -1) return -1;
    if (mode) detail = mode == 1u ? 0 : 2;
    model = r_u32(levels + 8u + 8u * (uint32)detail);
    highest = 31 - (sint32)gtos_leading_sign_bits(combined);
    precision = (sint16)r_u16(model + 76u) + 1;
    if (highest < precision) highest = precision;
    model_shift = highest - precision;
    coordinate_shift = highest - 13;
    if (coordinate_shift < 0) coordinate_shift = 0;
    w_u16(context + 40u, coordinate_shift); w_u16(context + 42u, model_shift);
    for (i = 0u; i < 3u; ++i)
        w_u16(context + 280u + 2u * i, (sint32)displacement[i] >> ((uint32)coordinate_shift & 31u));
    gtos_model_geometry_rotation(psx_addr(context + 64u, 20u));
    gte_lwc2(0u, r_u32(context + 280u)); gte_lwc2(1u, r_u32(context + 284u));
    xport_gte_execute(0x486012u);
    for (i = 0u; i < 3u; ++i) {
        uint32 value = xport_gte_read_data(9u + i);
        w_u32(context + 84u + 4u * i, value); gte_ctc2(value, 5u + i);
    }
    gtos_model_multiply(psx_addr(context + 948u, 20u), psx_addr(context + 64u, 20u), psx_addr(context + 884u, 20u));
    gtos_model_scale(psx_addr(context + 96u, 20u), psx_addr(context + 948u, 20u), scale);
    gtos_model_shift(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), model_shift);
    gtos_model_bind_geometry_rotation(context, psx_addr(context + 96u, 20u));
    gtos_model_clip_planes(context);
    for (i = 0u; i < 8u; ++i) {
        uint32 point = context + 184u + 8u * i;
        w_u16(point, r_u16(model + ((i & 1u) ? 68u : 60u)));
        w_u16(point + 2u, r_u16(model + ((i & 2u) ? 70u : 62u)));
        w_u16(point + 4u, r_u16(model + ((i & 4u) ? 72u : 64u)));
    }
    clipping = gtos_model_clip_points(context, context + 184u, 8u);
    if (clipping & 31u) return -1;
    gtos_model_transpose(psx_addr(context + 248u, 20u), psx_addr(context + 884u, 20u));
    gtos_model_scale(psx_addr(context + 248u, 20u), psx_addr(context + 248u, 20u), -scale);
    gtos_model_rotate_short(psx_addr(context + 56u, 8u), psx_addr(context + 248u, 20u), psx_addr(context + 280u, 8u));
    if (r_u32(context + 912u)) {
        sint32 denominator = (sint32)(((sint64)((sint32)range >> 8) * 2560) >> 8);
        sint32 numerator = (sint32)((uint32)((sint32)metric >> 8) << 12u), attenuation, brightness, quotient;
        if (!denominator || (denominator == -1 && numerator == (sint32)0x80000000u)) {
            gt_draft_unresolved_call(0x8002D1E8u, context, levels, mode, (uint32)numerator, (uint32)denominator, 0u, 0u, 0u);
            return -1;
        }
        quotient = numerator / denominator;
        attenuation = 4096 - (sint16)r_u16(context + 924u);
        if (attenuation < 0) attenuation = 0;
        brightness = (sint32)((uint32)(80 - ((sint32)((uint32)quotient * 80u) >> 12)) * (uint32)attenuation) >> 12;
        if (brightness < 0) brightness = 0;
        if (brightness >= 64) brightness = 64;
        temporary = (uint32)brightness * 0x010101u;
        w_u32(context + 968u, temporary | 0x26000000u); w_u32(context + 972u, temporary | 0x2E000000u);
        gtos_model_shift(psx_addr(context + 948u, 20u), psx_addr(context + 948u, 20u), (sint16)r_u16(context + 910u));
        if (!r_u16(context + 908u)) gtos_model_reflect(psx_addr(context + 948u, 20u), psx_addr(context + 948u, 20u));
        gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 920u), 13u);
        gte_ctc2((uint32)(sint32)(sint16)r_u16(context + 922u), 14u);
        for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 948u + 4u * i), 8u + i);
    }
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 96u + 4u * i), i);
    w_u32(context + 936u, model); w_u32(context + 940u, r_u32(model)); w_u32(context + 944u, r_u32(model + 4u));
    w_u32(context + 932u, r_u32(0x8007A024u));
    if ((clipping >> 16u) & 63u) gtos_model_polygons_clipped(model, context);
    else gtos_model_polygons(model, context);
    w_u32(0x8007A024u, r_u32(context + 932u));
    return detail;
}

uint32 gtos_model_bind_geometry_rotation(uint32 context, const uint8 matrix[20])
{
    PsxGteSnapshot state;
    uint32 cells[9], i, result;
    gtos_model_geometry_rotation(matrix);
    psx_gte_snapshot(&state);
    for (i = 0u; i < 9u; ++i) cells[i] = (uint16)state.rotation.m[i / 3u][i % 3u];
    for (i = 0u; i < 4u; ++i)
        w_u32(context + 96u + 4u * i, cells[2u * i] | (cells[2u * i + 1u] << 16u));
    result = (uint32)(sint32)(sint16)cells[8];
    w_u32(context + 112u, result);
    return result;
}

uint32 gtos_model_clip_points(uint32 context, uint32 points, uint32 count)
{
    uint32 any = 0u, all = 0xFFFFFFFFu, screen_all = 0xFFFFFFFFu;
    uint32 near = r_u16(context + 8u), i;
    sint32 width = (sint16)r_u16(context + 4u), height = (sint16)r_u16(context + 6u);
    gte_lwc2(0u, r_u32(points)); gte_lwc2(1u, r_u32(points + 4u));
    xport_gte_execute(0x180001u);
    for (i = 0u; i < count; ++i) {
        PsxGteSnapshot state;
        uint32 next_xy = r_u32(points + 8u), next_z = r_u32(points + 12u);
        uint32 x = xport_gte_read_data(9u), y = xport_gte_read_data(10u);
        uint32 z = xport_gte_read_data(11u), screen = xport_gte_read_data(14u), bits, screen_bits = 0u;
        sint32 sx = (sint16)screen, sy = (sint16)(screen >> 16u);
        psx_gte_snapshot(&state);
        points += 8u;
        xport_gte_execute(0x4BE012u);
        gte_lwc2(2u, (x & 65535u) | (y << 16u)); gte_lwc2(3u, z);
        gte_lwc2(0u, next_xy); gte_lwc2(1u, next_z);
        bits = (z - near) >> 31u;
        if ((sint32)xport_gte_read_data(9u) < 0) bits |= 2u;
        if ((sint32)xport_gte_read_data(10u) < 0) bits |= 4u;
        xport_gte_execute(0x4CE012u);
        if ((sint32)state.flag < 0) bits |= 32u;
        if ((sint32)xport_gte_read_data(9u) < 0) bits |= 8u;
        if ((sint32)xport_gte_read_data(10u) < 0) bits |= 16u;
        xport_gte_execute(0x180001u);
        if (sx < 0) screen_bits |= 2u;
        if (sx >= width) screen_bits |= 4u;
        if (sy < 0) screen_bits |= 8u;
        if (sy >= height) screen_bits |= 16u;
        if ((sint32)state.flag < 0) screen_bits = 0u;
        any |= bits; all &= bits; screen_all &= screen_bits;
    }
    return all | screen_all | (any << 16u);
}

void gtos_model_clip_planes(uint32 context)
{
    uint32 i;
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 116u + 4u * i), 8u + i);
    for (i = 0u; i < 5u; ++i) gte_ctc2(r_u32(context + 136u + 4u * i), 16u + i);
}

uint32 gtos_model_rotate_short(uint8 output[8], const uint8 matrix[20], const uint8 vector[8])
{
    uint32 y;
    gte_SetRotMatrix(matrix);
    gte_lwc2(0u, xport_load_le32(vector));
    gte_lwc2(1u, xport_load_le32(vector + 4u));
    xport_gte_execute(0x486012u);
    xport_store_le32(output, xport_gte_read_data(9u));
    y = xport_gte_read_data(10u);
    xport_store_le32(output + 4u, xport_gte_read_data(11u));
    xport_store_le16(output + 2u, (uint16)y);
    return y;
}

sint32 gtos_model_select_detail(uint32 levels, const uint32 displacement[3], sint32 range)
{
    sint32 divisor = range >> 4, reciprocal, distance;
    uint32 sum, index, count;
    if (!divisor) {
        gt_draft_unresolved_call(0x80031A94u, levels, (uint32)range, 0u, 0u, 0u, 0u, 0u, 0u);
        return -1;
    }
    reciprocal = 16777216 / divisor;
    sum = 0u;
    /* SQR uses the signed low halfword of each shifted input without scaling */
    for (index = 0u; index < 3u; ++index) {
        sint32 coordinate = (sint16)((sint32)displacement[index] >> 14);
        sum += (uint32)(coordinate * coordinate);
    }
    distance = (sint32)(((sint64)(sint32)sum * reciprocal) >> 12);
    count = r_u16(levels);
    for (index = 0u; index < count; ++index)
        if (distance < (sint32)r_u32(levels + 4u + 8u * index)) return (sint32)index;
    return -1;
}

void gtos_model_geometry_rotation(const uint8 matrix[20])
{
    uint32 words[5], i;
    for (i = 0u; i < 5u; ++i) words[i] = xport_load_le32(matrix + 4u * i);
    words[1] = (words[1] ^ 0xFFFF0000u) + 65536u;
    words[2] = 0u - words[2];
    if (words[2] & 65535u) words[2] += 65536u;
    for (i = 0u; i < 5u; ++i) gte_ctc2(words[i], i);
}

uint32 gtos_model_scale(uint8 output[20], const uint8 input[20], sint32 scale)
{
    uint32 column, row, result = 0u;
    uint32 commands[3] = { 0x486012u, 0x48E012u, 0x496012u };
    gte_SetRotMatrix(input);
    gte_lwc2(0u, (uint16)scale); gte_lwc2(1u, 0u);
    gte_lwc2(2u, (uint32)(uint16)scale << 16u); gte_lwc2(3u, 0u);
    gte_lwc2(4u, 0u); gte_lwc2(5u, (uint16)scale);
    for (column = 0u; column < 3u; ++column) {
        xport_gte_execute(commands[column]);
        for (row = 0u; row < 3u; ++row) {
            uint32 value = xport_gte_read_data(9u + row);
            xport_store_le16(output + 6u * row + 2u * column, (uint16)value);
            if (column == 2u && row == 0u) result = value;
        }
    }
    return result;
}

void gtos_model_shift(uint8 output[20], const uint8 input[20], sint32 shift)
{
    sint32 values[9];
    uint32 i, count = (shift < 0 ? 0u - (uint32)shift : (uint32)shift) & 31u;
    if (!shift && output == input) return;
    for (i = 0u; i < 9u; ++i) values[i] = (sint16)xport_load_le16(input + 2u * i);
    for (i = 0u; i < 9u; ++i) {
        uint32 value = shift < 0 ? (uint32)values[i] << count : (uint32)(values[i] >> count);
        xport_store_le16(output + 2u * i, (uint16)value);
    }
}

uint32 gtos_model_reflect(uint8 output[20], const uint8 input[20])
{
    uint32 middle, reflected, result;
    if (output != input) {
        xport_store_le32(output, xport_load_le32(input));
        xport_store_le32(output + 12u, xport_load_le32(input + 12u));
        xport_store_le32(output + 16u, xport_load_le32(input + 16u));
    }
    middle = xport_load_le32(input + 8u);
    result = (xport_load_le32(input + 4u) ^ 0xFFFF0000u) + 65536u;
    reflected = 0u - middle;
    if (reflected & 65535u) reflected += 65536u;
    xport_store_le32(output + 8u, reflected);
    xport_store_le32(output + 4u, result);
    return result;
}

uint32 gtos_model_reflected_rotation(const uint8 matrix[20])
{
    uint32 middle, result;
    gte_ctc2(xport_load_le32(matrix), 0u);
    gte_ctc2((xport_load_le32(matrix + 4u) ^ 0xFFFF0000u) + 65536u, 1u);
    middle = xport_load_le32(matrix + 8u);
    result = ((0u - middle) & 65535u) | ((0u - (middle >> 16u)) << 16u);
    gte_ctc2(result, 2u);
    gte_ctc2(xport_load_le32(matrix + 12u), 3u);
    gte_ctc2(xport_load_le32(matrix + 16u), 4u);
    return result;
}

void gtos_model_depth_cue(uint32 context, uint32 color, sint32 coefficient, uint32 offset)
{
    uint32 red = (color << 4u) & 0xFF0u, green = (color >> 4u) & 0xFF0u;
    uint32 blue = (color >> 12u) & 0xFF0u;
    /* Preserve the three writes to control register 21 from the original function */
    gte_ctc2(red, 21u); gte_ctc2(green, 21u); gte_ctc2(blue, 21u);
    gte_ctc2((uint32)coefficient, 27u); gte_ctc2(offset, 28u);
    w_u32(context + 12u, red); w_u32(context + 16u, green); w_u32(context + 20u, blue);
    w_u16(context + 34u, coefficient); w_u32(context + 36u, offset);
}

void gtos_model_camera_bind(uint32 context, uint32 camera)
{
    uint32 i;
    w_u32(context, camera);
    for (i = 0u; i < 10u; ++i) w_u32(context + 116u + 4u * i, r_u32(camera + 76u + 4u * i));
    w_u16(context + 8u, r_u16(camera + 116u));
    w_u16(context + 10u, r_u16(camera + 118u));
    w_u32(context + 24u, (uint32)(sint32)(sint16)r_u16(camera + 8u) << 16u);
    w_u32(context + 28u, (uint32)(sint32)(sint16)r_u16(camera + 10u) << 16u);
    w_u16(context + 32u, r_u16(camera + 12u));
    w_u16(context + 4u, r_u16(camera + 18u));
    w_u16(context + 6u, r_u16(camera + 20u));
}

void gtos_model_camera_context(uint32 context, uint32 camera)
{
    uint32 i;
    gtos_model_camera_bind(context, camera);
    for (i = 0u; i < 3u; ++i) w_u32(context + 44u + 4u * i, r_u32(camera + 120u + 4u * i));
    for (i = 0u; i < 5u; ++i) w_u32(context + 64u + 4u * i, r_u32(camera + 160u + 4u * i));
}

void gtos_model_camera_context_native(uint32 context, const uint8 camera[180])
{
    uint32 i;
    /* Native callers carry the camera pointer explicitly instead of storing it in guest memory */
    for (i = 0u; i < 10u; ++i)
        w_u32(context + 116u + 4u * i, xport_load_le32(camera + 76u + 4u * i));
    w_u16(context + 8u, xport_load_le16(camera + 116u));
    w_u16(context + 10u, xport_load_le16(camera + 118u));
    w_u32(context + 24u, (uint32)(sint32)(sint16)xport_load_le16(camera + 8u) << 16u);
    w_u32(context + 28u, (uint32)(sint32)(sint16)xport_load_le16(camera + 10u) << 16u);
    w_u16(context + 32u, xport_load_le16(camera + 12u));
    w_u16(context + 4u, xport_load_le16(camera + 18u));
    w_u16(context + 6u, xport_load_le16(camera + 20u));
    for (i = 0u; i < 3u; ++i)
        w_u32(context + 44u + 4u * i, xport_load_le32(camera + 120u + 4u * i));
    for (i = 0u; i < 5u; ++i)
        w_u32(context + 64u + 4u * i, xport_load_le32(camera + 160u + 4u * i));
}

void gtos_model_screen_bounds(uint8 *camera)
{
    sint32 x0 = (sint16)xport_load_le16(camera), y0 = (sint16)xport_load_le16(camera + 2u);
    sint32 x1 = (sint16)xport_load_le16(camera + 4u), y1 = (sint16)xport_load_le16(camera + 6u);
    sint32 center_x = (sint16)xport_load_le16(camera + 8u), center_y = (sint16)xport_load_le16(camera + 10u);
    sint32 distance = (sint16)xport_load_le16(camera + 12u);
    sint32 left, right, top, bottom;
    if (!distance) {
        gt_draft_unresolved_call(0x8003160Cu, (uint32)x0, (uint32)y0, (uint32)x1,
            (uint32)y1, (uint32)center_x, (uint32)center_y, 0u, 0u);
        return;
    }
    left = (sint32)((uint32)(x0 - center_x) << 12u) / distance;
    right = (sint32)((uint32)(x1 - center_x) << 12u) / distance;
    top = (sint32)((uint32)(center_y - y0) << 12u) / distance;
    bottom = (sint32)((uint32)(center_y - y1) << 12u) / distance;
    xport_store_le16(camera + 46u, (uint16)y0);
    xport_store_le16(camera + 48u, (uint16)x1);
    xport_store_le16(camera + 50u, (uint16)y0);
    xport_store_le16(camera + 52u, (uint16)x0);
    xport_store_le16(camera + 54u, (uint16)y1);
    xport_store_le16(camera + 56u, (uint16)x1);
    xport_store_le16(camera + 58u, (uint16)y1);
    xport_store_le16(camera + 44u, (uint16)x0);
    xport_store_le16(camera + 36u, (uint16)left);
    xport_store_le16(camera + 40u, (uint16)right);
    xport_store_le16(camera + 38u, (uint16)top);
    xport_store_le16(camera + 42u, (uint16)bottom);
    xport_store_le16(camera + 60u, (uint16)x0);
    xport_store_le16(camera + 66u, (uint16)y0);
    xport_store_le16(camera + 68u, (uint16)x1);
    xport_store_le16(camera + 74u, (uint16)y1);
}

uint32 gtos_model_square_root(uint32 value, uint32 fractional_bits)
{
    uint32 digits = 16u, root = 0u, trial = 0u, remainder = 0u, i;
    if (!value) return 0u;
    if ((sint32)value >= 0) {
        digits = (((31u - gtos_leading_sign_bits(value)) & ~1u) + 2u) >> 1u;
    }
    value <<= (32u - 2u * digits) & 31u;
    for (i = 0u; i < digits; ++i) {
        uint32 candidate;
        root <<= 1u;
        remainder = (remainder << 2u) | (value >> 30u);
        trial <<= 1u;
        candidate = trial + 1u;
        remainder -= candidate;
        value <<= 2u;
        if ((sint32)remainder < 0) remainder += candidate;
        else { ++root; trial = candidate + 1u; }
    }
    for (i = 0u; i < fractional_bits; ++i) {
        uint32 candidate;
        remainder <<= 2u;
        trial <<= 1u;
        candidate = trial + 1u;
        remainder -= candidate;
        root <<= 1u;
        if ((sint32)remainder < 0) remainder += candidate;
        else { ++root; trial = candidate + 1u; }
    }
    return root;
}

uint32 gtos_model_normalize_pair(sint16 output[2], const sint16 input[2])
{
    sint32 x = input[0], y = input[1];
    uint32 length = 0u;
    if (x || y) {
        sint32 ax = x < 0 ? -x : x, ay = y < 0 ? -y : y;
        uint32 maximum = (uint32)(ax >= ay ? ax : ay);
        while (maximum < 4096u) { maximum <<= 1u; x *= 2; y *= 2; }
        length = gtos_model_square_root((uint32)x * (uint32)x + (uint32)y * (uint32)y, 0u);
        if (!length) {
            gt_draft_unresolved_call(0x8003A49Cu, (uint32)x, (uint32)y, 0u, 0u, 0u, 0u, 0u, 0u);
            return 0u;
        }
        x = (sint32)((uint32)x << 12u) / (sint32)length;
        y = (sint32)((uint32)y << 12u) / (sint32)length;
    }
    output[0] = (sint16)x; output[1] = (sint16)y;
    return length;
}

static void gtos_model_camera_planes(uint8 *camera, uint32 vertical)
{
    uint32 axis = vertical ? 2u : 0u, base = vertical ? 96u : 76u;
    sint32 first = (sint16)xport_load_le16(camera + axis);
    sint32 last = (sint16)xport_load_le16(camera + axis + 4u);
    sint32 center = (sint16)xport_load_le16(camera + axis + 8u);
    sint32 margin = (sint16)xport_load_le16(camera + (vertical ? 16u : 14u));
    sint16 vector[2];
    sint16 distance = (sint16)xport_load_le16(camera + 12u);
    vector[0] = (sint16)(first - center - margin); vector[1] = distance;
    gtos_model_normalize_pair(vector, vector);
    xport_store_le16(camera + base, vertical ? 0u : (uint16)vector[1]);
    xport_store_le16(camera + base + 2u, vertical ? (uint16)vector[1] : 0u);
    xport_store_le16(camera + base + 4u, (uint16)-vector[0]);
    vector[0] = (sint16)(last - center + margin); vector[1] = distance;
    gtos_model_normalize_pair(vector, vector);
    xport_store_le16(camera + base + 6u, vertical ? 0u : (uint16)-vector[1]);
    xport_store_le16(camera + base + 8u, vertical ? (uint16)-vector[1] : 0u);
    xport_store_le16(camera + base + 10u, (uint16)vector[0]);
    xport_store_le16(camera + base + 12u, 0u);
    xport_store_le16(camera + base + 14u, 0u);
    xport_store_le16(camera + base + 16u, vertical ? (uint16)-4096 : 4096u);
}

void gtos_model_camera_initialize(uint8 *camera)
{
    sint32 width, distance, coefficient;
    xport_store_le16(camera + 18u, (uint16)(xport_load_le16(camera + 4u) - xport_load_le16(camera)));
    xport_store_le16(camera + 20u, (uint16)(xport_load_le16(camera + 6u) - xport_load_le16(camera + 2u)));
    gtos_model_camera_planes(camera, 0u);
    gtos_model_camera_planes(camera, 1u);
    width = (sint16)xport_load_le16(camera + 18u);
    distance = (sint16)xport_load_le16(camera + 12u);
    if (!width) {
        gt_draft_unresolved_call(0x80031524u, (uint32)distance, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    coefficient = (sint32)((uint32)distance << 13u) / width;
    xport_store_le16(camera + 118u, 32767u);
    xport_store_le16(camera + 116u, (uint16)((distance >> 1) + 1));
    xport_store_le32(camera + 24u, (uint32)coefficient);
    xport_store_le32(camera + 28u, (uint32)(((long long)coefficient * coefficient) >> 12));
    xport_store_le32(camera + 32u, 31u - gtos_leading_sign_bits((uint32)coefficient));
    gtos_model_screen_bounds(camera);
}

static sint32 gtos_model_asin(sint32 value)
{
    value = (sint16)value;
    if (value < -4095) return -1024;
    if (value >= 4096) return 1024;
    if (value < 0) return (sint16)-(sint16)r_u16(0x8006F698u + 2u * (uint32)-value);
    return (sint16)r_u16(0x8006F698u + 2u * (uint32)value);
}

static sint32 gtos_model_atan(sint32 y, sint32 x)
{
    sint32 ax, ay, ratio;
    uint32 quadrant;
    x = (sint16)x; y = (sint16)y;
    if (!(x | y)) return 0;
    ax = x < 0 ? -x : x; ay = y < 0 ? -y : y;
    ratio = ax < ay ? (ax << 12) / ay : (ay << 12) / ax;
    quadrant = (y < 0 ? 4u : 0u) | (x < 0 ? 2u : 0u) | (ax < ay ? 1u : 0u);
    return (sint16)(((sint32)(sint16)r_u16(0x8007169Cu + 2u * (uint32)ratio) ^
        (sint32)(sint8)r_u8(0x8006F680u + quadrant)) +
        (sint32)(sint16)r_u16(0x8006F688u + 2u * quadrant));
}

void gtos_model_camera_orientation(uint8 *camera, const uint8 *adjustment)
{
    uint8 *matrix = camera + 140u;
    sint32 component = (sint16)xport_load_le16(matrix + 14u), roll = 0, yaw;
    sint32 magnitude = component < 0 ? -component : component;
    sint32 cosine = magnitude < 4096 ? (sint16)r_u16(0x8006D67Cu + 2u * (uint32)magnitude) : 0;
    xport_store_le16(camera + 132u, (uint16)gtos_model_asin((sint16)xport_load_le16(matrix + 10u)));
    if (cosine) {
        roll = gtos_model_atan((sint16)xport_load_le16(matrix + 6u), (sint16)xport_load_le16(matrix + 8u));
        yaw = gtos_model_atan((sint16)xport_load_le16(matrix + 4u), (sint16)xport_load_le16(matrix + 16u));
    } else {
        yaw = gtos_model_atan((sint16)-xport_load_le16(matrix + 2u), (sint16)-xport_load_le16(matrix + 14u));
    }
    xport_store_le16(camera + 134u, (uint16)yaw);
    xport_store_le16(camera + 136u, (uint16)roll);
    gtos_model_transpose(camera + 160u, matrix);
    if (adjustment) gtos_model_multiply(camera + 160u, adjustment, camera + 160u);
}

static void gtos_packet_link(uint32 tag, uint32 packet)
{
    uint32 previous = r_u32(tag) & 0xFFFFFFu;
    w_u32(tag, (r_u32(tag) & 0xFF000000u) | (packet & 0xFFFFFFu));
    w_u32(packet, (r_u32(packet) & 0xFF000000u) | previous);
}

void gtos_menu_font(uint32 font, uint32 flags, uint32 color)
{
    static const uint16 heights[4] = { 6u, 8u, 12u, 24u };
    if (font < 4u) {
        w_u32(0x80079FF0u, gtos_font_glyphs[font]);
        w_u32(0x80079FF4u, gtos_font_kerning[font]);
        w_u16(0x8007A008u, r_u16(0x80079FF8u) + 64u * font);
        w_u16(0x8007A00Au, font);
        w_u16(0x8007A00Eu, heights[font]);
    }
    w_u8(0x80079FFFu, flags | 0x64u);
    w_u8(0x80079FFCu, color);
    w_u8(0x80079FFDu, color >> 8u);
    w_u8(0x80079FFEu, color >> 16u);
}

static uint32 gtos_font_advance(uint32 metrics, uint32 kerning, uint32 character, uint32 next)
{
    uint32 table = r_u32(kerning + 4u * character);
    uint32 packed = r_u32(table + ((next >> 1u) & 0x7Cu));
    return 1u + r_u8(metrics + 8u * character + 6u) - ((packed >> (4u * (next & 7u))) & 15u);
}

uint32 gtos_menu_text_width(uint32 font, uint32 text)
{
    uint32 width = 0u, character;
    if (font >= 4u) return 0u;
    while ((character = r_u8(text)) != 0u) {
        ++text;
        width += gtos_font_advance(gtos_font_glyphs[font], gtos_font_kerning[font], character,
            (uint32)(sint32)(sint8)r_u8(text));
    }
    return width;
}

static void gtos_menu_text_spacing(uint32 tag, sint32 x, sint32 y, uint32 text, uint32 spacing)
{
    uint32 packet = r_u32(0x8007A024u), mode_packet = packet;
    uint32 metrics = r_u32(0x80079FF0u), kerning = r_u32(0x80079FF4u), character;
    w_u32(packet, 0x01000000u);
    w_u32(packet + 4u, 0xE1000000u | r_u16(0x80079FFAu) | (r_u32(0x1F801814u) & 0x600u));
    gtos_packet_link(tag, packet);
    packet += 8u;
    while ((character = r_u8(text)) != 0u) {
        uint32 glyph = metrics + 8u * character, height = r_u8(glyph + 3u);
        if (height) {
            w_u32(packet, 0x04000000u);
            w_u32(packet + 4u, r_u32(0x80079FFCu));
            w_u16(packet + 8u, (uint32)x);
            w_u16(packet + 10u, (uint32)y + r_u8(glyph + 5u) - 4u);
            w_u16(packet + 12u, r_u16(glyph));
            w_u16(packet + 14u, r_u16(0x8007A008u));
            w_u16(packet + 16u, r_u8(glyph + 2u));
            w_u16(packet + 18u, height);
            gtos_packet_link(mode_packet, packet);
            packet += 20u;
        }
        ++text;
        x = (sint32)((uint32)x + gtos_font_advance(metrics, kerning, character,
            (uint32)(sint32)(sint8)r_u8(text)) - 1u + spacing);
    }
    w_u32(0x8007A024u, packet);
}

void gtos_menu_text(uint32 tag, sint32 x, sint32 y, uint32 text)
{
    gtos_menu_text_spacing(tag, x, y, text, 1u);
}

void gtos_menu_rectangle(uint32 tag, sint32 x0, sint32 y0, sint32 x1, sint32 y1, uint32 color)
{
    uint32 packet = r_u32(0x8007A024u), reverse_x = (sint16)x1 < (sint16)x0;
    uint32 reverse_y = (sint16)y1 < (sint16)y0;
    x0 = (sint16)x0 + reverse_x; x1 = (sint16)x1 + !reverse_x;
    y0 = (sint16)y0 + reverse_y; y1 = (sint16)y1 + !reverse_y;
    w_u32(packet, 0x05000000u);
    w_u32(packet + 4u, color ^ 0x29000000u);
    w_u16(packet + 8u, x0); w_u16(packet + 10u, y0);
    w_u16(packet + 12u, x1); w_u16(packet + 14u, y0);
    w_u16(packet + 16u, x0); w_u16(packet + 18u, y1);
    w_u16(packet + 20u, x1); w_u16(packet + 22u, y1);
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 24u);
}

void gtos_menu_outline(uint32 tag, sint32 x0, sint32 y0, sint32 x1, sint32 y1, uint32 color)
{
    uint32 first = r_u32(0x8007A024u), packet = first, count, i;
    sint32 coordinates[4][4], temporary;
    x0 = (sint16)x0; x1 = (sint16)x1;
    y0 = (sint16)y0; y1 = (sint16)y1;
    if (x1 < x0) { temporary = x0; x0 = x1; x1 = temporary; }
    if (y1 < y0) { temporary = y0; y0 = y1; y1 = temporary; }
    coordinates[0][0] = x0; coordinates[0][1] = y0;
    coordinates[0][2] = x1; coordinates[0][3] = y1;
    count = 1u;
    if (x0 != x1 && y0 != y1) {
        coordinates[0][3] = y0;
        coordinates[1][0] = x0; coordinates[1][1] = y1;
        coordinates[1][2] = x1; coordinates[1][3] = y1;
        count = 2u;
        if (y1 - y0 >= 2) {
            coordinates[2][0] = x0; coordinates[2][1] = y0 + 1;
            coordinates[2][2] = x0; coordinates[2][3] = y1 - 1;
            coordinates[3][0] = x1; coordinates[3][1] = y0 + 1;
            coordinates[3][2] = x1; coordinates[3][3] = y1 - 1;
            count = 4u;
        }
    }
    /* Keep the original top, bottom, left and right packet chain */
    for (i = 0u; i < count; ++i, packet += 16u) {
        uint32 next = i + 1u < count ? packet + 16u : r_u32(tag);
        w_u32(packet, 0x03000000u | (next & 0xFFFFFFu));
        w_u32(packet + 4u, color ^ 0x41000000u);
        w_u16(packet + 8u, coordinates[i][0]);
        w_u16(packet + 10u, coordinates[i][1]);
        w_u16(packet + 12u, coordinates[i][2]);
        w_u16(packet + 14u, coordinates[i][3]);
    }
    w_u32(tag, (r_u32(tag) & 0xFF000000u) | (first & 0xFFFFFFu));
    w_u32(0x8007A024u, packet);
}

void gtos_menu_clip(uint32 tag, uint32 offset, uint32 top_left, uint32 bottom_right)
{
    uint32 packet = r_u32(0x8007A024u);
    w_u32(packet, 0x03000000u);
    w_u32(packet + 4u, top_left | 0xE3000000u);
    w_u32(packet + 8u, bottom_right | 0xE4000000u);
    w_u32(packet + 12u, offset | 0xE5000000u);
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 16u);
}

static uint32 gtos_menu_color(uint32 index, uint32 intensity)
{
    uint32 table = 0x8004DBB0u + 3u * index;
    return ((intensity * r_u8(table) / 255u) & 255u) |
        (((intensity * r_u8(table + 1u) / 255u) & 255u) << 8u) |
        (((intensity * r_u8(table + 2u) / 255u) & 255u) << 16u);
}

static void gtos_menu_sprite(uint32 tag, uint32 xy, uint32 uv_clut, uint32 size, uint32 color)
{
    uint32 packet = r_u32(0x8007A024u);
    w_u32(packet, 0x04000000u);
    w_u32(packet + 4u, color ^ 0x65000000u);
    w_u32(packet + 8u, xy);
    w_u32(packet + 12u, uv_clut);
    w_u32(packet + 16u, size);
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 20u);
}

static void gtos_menu_draw_mode(uint32 tag, uint32 mode)
{
    uint32 packet = r_u32(0x8007A024u);
    w_u32(packet, 0x01000000u);
    w_u32(packet + 4u, 0xE1000000u | (mode & 65535u));
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 8u);
}

typedef struct GTOSListDrawOutput {
    sint16 offset;
    sint8 items[24];
    uint8 intensity[24];
    sint16 x[24], y[24];
} GTOSListDrawOutput;

void gtos_menu_binary_draw(uint32 tag, uint32 list, sint32 x, sint32 y,
    sint32 intensity, uint32 color_flag, uint32 left_motion, uint32 title_below)
{
    sint32 delay = (sint8)r_u8(list + 13u), gray, center, border;
    uint32 text, pulse, table;
    if (delay > 0) w_u8(list + 13u, --delay);
    x = (sint32)((uint32)x + (uint32)(left_motion ? -(160 * delay / 12) : 160 * delay / 12));
    center = (sint16)x;
    y = (sint16)y;
    gray = (sint32)((uint32)(intensity >> 1) * (uint32)(12 - delay)) / 12;
    gtos_menu_font(1u, 0u, ((uint32)gray & 255u) * 0x010101u);
    text = r_u32(list);
    gtos_menu_text(tag, center - ((sint32)gtos_menu_text_width(1u, text) >> 1),
        y + (title_below ? 20 : -12), text);
    text = r_u32(list + 4u);
    gtos_menu_text(tag, center - (((sint32)gtos_menu_text_width(1u, text) + 96) >> 1), y + 3, text);
    text = r_u32(list + 8u);
    gtos_menu_text(tag, center + ((96 - (sint32)gtos_menu_text_width(1u, text)) >> 1), y + 3, text);
    border = (sint32)((uint32)gray * 51u) >> 7;
    gtos_menu_outline(tag, x - 96, y, x - 1, y + 15, ((uint32)border & 255u) * 0x010101u);
    gtos_menu_outline(tag, x, y, x + 95, y + 15, ((uint32)border & 255u) * 0x010101u);
    if (!delay) {
        table = 0x8004DC32u + 3u * (uint32)(sint32)(sint8)r_u8(0x8004DC70u);
        pulse = (uint32)((sint32)((uint32)intensity * r_u8(table)) / 255) & 255u;
        if (color_flag == 1u) pulse <<= 16u;
        if (r_u8(list + 12u)) gtos_menu_rectangle(tag, x, y, x + 95, y + 15, pulse);
        else gtos_menu_rectangle(tag, x - 96, y, x - 1, y + 15, pulse);
    }
    gtos_menu_rectangle(tag, x - 96, y, x + 95, y + 15, 0x02000000u);
    gtos_menu_draw_mode(tag, 512u);
}

static uint32 gtos_menu_selection_color(uint32 intensity)
{
    uint32 table = 0x8004DC30u + 3u * (uint32)(sint32)(sint8)r_u8(0x8004DC70u);
    return ((intensity * r_u8(table) / 255u) & 255u) |
        (((intensity * r_u8(table + 1u) / 255u) & 255u) << 8u) |
        (((intensity * r_u8(table + 2u) / 255u) & 255u) << 16u);
}

static void gtos_menu_arrow(uint32 tag, sint32 x, sint32 y, uint32 color, uint32 down)
{
    uint32 packet = r_u32(0x8007A024u);
    w_u32(packet, 0x04000000u);
    w_u32(packet + 4u, (color & 0xFFFFFFu) | 0x20000000u);
    if (down) {
        w_u16(packet + 8u, x - 6); w_u16(packet + 10u, y - 6);
        w_u16(packet + 12u, x + 6); w_u16(packet + 14u, y - 6);
        w_u16(packet + 16u, x); w_u16(packet + 18u, y);
    } else {
        w_u16(packet + 8u, x); w_u16(packet + 10u, y);
        w_u16(packet + 12u, x + 6); w_u16(packet + 14u, y + 6);
        w_u16(packet + 16u, x - 6); w_u16(packet + 18u, y + 6);
    }
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 20u);
}

static void gtos_menu_image(uint32 data, uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    uint32 color = (intensity & 255u) * 0x010101u | 0x01000000u;
    gtos_menu_sprite(tag, ((uint32)y << 16u) | (uint16)x,
        r_u32(data + 4u), r_u32(data + 12u), color);
    gtos_menu_draw_mode(tag, r_u16(data) | 0x220u);
}

static void gtos_menu_scaled_image(uint32 data, uint32 tag, sint32 x, sint32 y,
    uint32 intensity, uint32 scale)
{
    uint32 packet = r_u32(0x8007A024u), uv = r_u32(data + 4u);
    uint32 width = r_u16(data + 8u), height = r_u16(data + 10u);
    uint32 u = uv & 255u, v = (uv >> 8u) & 255u;
    uint32 right_u = (u + width - 1u) & 255u, bottom_v = (v + height - 1u) & 255u;
    sint32 right = x + ((sint32)(scale * width) >> 7u);
    sint32 bottom = y + ((sint32)(scale * height) >> 7u);
    w_u32(packet, 0x09000000u);
    w_u32(packet + 4u, (intensity & 255u) * 0x010101u | 0x2C000000u);
    w_u16(packet + 8u, (uint16)x); w_u16(packet + 10u, (uint16)y);
    w_u32(packet + 12u, uv);
    w_u16(packet + 16u, (uint16)right); w_u16(packet + 18u, (uint16)y);
    w_u32(packet + 20u, right_u | (v << 8u) | ((uint32)r_u16(data) << 16u));
    w_u16(packet + 24u, (uint16)x); w_u16(packet + 26u, (uint16)bottom);
    w_u32(packet + 28u, u | (bottom_v << 8u));
    w_u16(packet + 32u, (uint16)right); w_u16(packet + 34u, (uint16)bottom);
    w_u32(packet + 36u, right_u | (bottom_v << 8u));
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 40u);
}

static void gtos_menu_horizontal_arrow(uint32 tag, sint32 x, sint32 y, uint32 color, uint32 right)
{
    uint32 packet = r_u32(0x8007A024u);
    w_u32(packet, 0x04000000u);
    w_u32(packet + 4u, (color & 0xFFFFFFu) | 0x20000000u);
    w_u16(packet + 8u, right ? x - 6 : x);
    w_u16(packet + 10u, right ? y - 6 : y);
    w_u16(packet + 12u, right ? x : x + 6);
    w_u16(packet + 14u, right ? y : y - 6);
    w_u16(packet + 16u, right ? x - 6 : x + 6);
    w_u16(packet + 18u, y + 6);
    gtos_packet_link(tag, packet);
    w_u32(0x8007A024u, packet + 20u);
}

void gtos_menu_category_draw(GTOSCategoryDrawOutput *output, uint32 tag, uint32 list,
    sint32 x, sint32 y, sint32 intensity, uint32 flags)
{
    sint32 old_x = (sint16)x, old_y = (sint16)y;
    sint32 current_x = old_x, current_y = old_y, old_intensity = 0, current_intensity = intensity;
    sint32 motion = (sint8)r_u8(list + 14u), timer = (sint8)r_u8(list + 15u);
    sint32 width = (sint16)r_u16(list + 2u), height = (sint16)r_u16(list + 4u);
    uint32 items = r_u32(list + 8u), color = gtos_menu_selection_color((uint32)intensity);
    output->previous = -1;
    if (motion) {
        output->previous = (sint8)r_u8(list + 13u);
        if (motion >= 1 && motion <= 4) {
            old_intensity = (sint32)((uint32)intensity * (uint32)(12 - timer)) / 12;
            if (motion == 1) { old_x = (sint16)(x + width * timer / 12); current_x = (sint16)(old_x - width); }
            if (motion == 2) { old_x = (sint16)(x - width * timer / 12); current_x = (sint16)(old_x + width); }
            if (motion == 3) { old_y = (sint16)(y + height * timer / 12); current_y = (sint16)(old_y - height); }
            if (motion == 4) { old_y = (sint16)(y - height * timer / 12); current_y = (sint16)(old_y + height); }
        }
        gtos_menu_category_entry(tag, items + 12u * (uint32)(sint32)output->previous,
            old_x, old_y, (uint32)(old_intensity >> 1));
        current_intensity = 255 * timer / 12;
    } else if (!(r_u8(list + 12u) & 32u) && r_u8(0x8004DC79u) && (flags & 1u)) {
        uint32 orientation = r_u8(list + 12u) & 31u;
        if (orientation == 0u) {
            gtos_menu_horizontal_arrow(tag, (sint16)x - (width >> 1) - 12, (sint16)y, color, 0u);
            gtos_menu_horizontal_arrow(tag, (sint16)x + (width >> 1) + 12, (sint16)y, color, 1u);
        } else if (orientation == 1u) {
            gtos_menu_arrow(tag, (sint16)x, (sint16)y - (height >> 1) - 12, color, 0u);
            gtos_menu_arrow(tag, (sint16)x, (sint16)y + (height >> 1) + 12, color, 1u);
        }
        gtos_menu_draw_mode(tag, 544u);
    }
    output->current = (sint8)r_u8(list + 1u);
    gtos_menu_category_entry(tag, items + 12u * (uint32)(sint32)output->current,
        current_x, current_y, (uint32)(current_intensity >> 1));
    output->x = current_x; output->y = current_y;
    output->previous_x = old_x; output->previous_y = old_y;
    output->intensity = (uint8)current_intensity;
    output->previous_intensity = (uint8)old_intensity;
}

void gtos_menu_category_entry(uint32 tag, uint32 entry, sint32 x, sint32 y, uint32 intensity)
{
    uint32 kind = r_u8(entry) & 63u, image = r_u32(entry + 4u);
    sint32 width, height;
    if (kind > 2u) return;
    width = r_u16(image + 8u);
    height = r_u16(image + 10u);
    if (kind == 0u) {
        uint32 scale = r_u32(entry + 8u);
        sint32 dx = (sint32)(scale * (uint32)(width >> 1)) >> 7;
        sint32 dy = (sint32)(scale * (uint32)(height >> 1)) >> 7;
        gtos_menu_scaled_image(image, tag, (sint32)((uint32)x - (uint32)dx),
            (sint32)((uint32)y - (uint32)dy), intensity, scale);
    } else if (kind == 1u) {
        gtos_menu_image(image, tag, x - (width >> 1), y - (height >> 1), intensity);
    } else {
        uint32 text = r_u32(entry + 8u);
        sint32 left = x - (((sint32)gtos_menu_text_width(2u, text) + width) >> 1);
        sint32 top = y - (height >> 1);
        gtos_menu_font(2u, 2u, (intensity & 255u) * 0x010101u);
        gtos_menu_text(tag, left + width, top, text);
        gtos_menu_image(image, tag, left, top, intensity);
    }
}

void gtos_menu_drivetrain(uint32 kind, uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    uint32 image;
    switch (kind) {
    case 4u:
        gtos_menu_image(0x8004DEC0u, tag,
            x - r_u16(0x8004DEC8u) - r_u16(0x8004DEB8u), y, intensity);
        image = 0x8004DEB0u;
        break;
    case 1u: image = 0x8004DEB0u; break;
    case 2u: image = 0x8004DEC0u; break;
    case 3u: image = 0x8004DED0u; break;
    default: return;
    }
    gtos_menu_image(image, tag, x - r_u16(image + 8u), y, intensity);
}

void gtos_menu_stat_bar(uint32 tag, sint32 x, sint32 y, sint32 intensity,
    sint32 value, sint32 limit)
{
    sint32 amount = value < limit ? value : limit;
    sint32 shade = (sint32)((uint32)intensity * 40u) / 255;
    uint32 gray = ((uint32)shade & 255u) * 0x010101u, i;
    for (i = 1u; i <= 4u; ++i) {
        sint32 marker = (sint32)((uint32)x + 8u * i);
        gtos_menu_rectangle(tag, marker, y, marker, (sint32)((uint32)y + 6u), gray);
    }
    if (amount > 0) {
        sint32 width = (sint32)((uint32)amount * 40u) / 255;
        uint32 red = (uint32)((sint32)((uint32)amount * (uint32)intensity) / 255) & 255u;
        uint32 blue = (uint32)((sint32)((255u - (uint32)amount) * (uint32)intensity) / 255) & 255u;
        gtos_menu_rectangle(tag, x, y, (sint32)((uint32)x + (uint32)width - 1u),
            (sint32)((uint32)y + 6u), red | (blue << 16u));
    }
    gtos_menu_rectangle(tag, x, y, (sint32)((uint32)x + 39u),
        (sint32)((uint32)y + 6u), gray);
}

void gtos_menu_stats(uint32 tag, uint32 rows, sint32 x, sint32 y, sint32 intensity, sint32 selected)
{
    uint32 row = rows + 4u * (uint32)selected, counter = r_u8(row), i;
    sint32 limit;
    if (counter < 27u) w_u8(row, ++counter);
    limit = (sint32)counter - 12;
    if (limit < 0) limit = 0;
    limit *= 17;
    for (i = 0u; i < 3u; ++i)
        gtos_menu_stat_bar(tag, (sint32)((uint32)x + 75u),
            (sint32)((uint32)y + 9u * i), intensity, r_u8(row + i + 1u), limit);
    gtos_menu_font(1u, 0u, ((uint32)(intensity >> 2) & 255u) * 0x010101u);
    gtos_menu_text_spacing(tag, x - 2, y - 1, 0x80078AABu, 0u);
    gtos_menu_text_spacing(tag, x - 2, y + 8, 0x80078AB9u, 0u);
    gtos_menu_text_spacing(tag, x - 2, y + 17, 0x80078AC6u, 0u);
}

void gtos_menu_number(char *output, sint32 value, uint32 suffix)
{
    sint32 digit, written = 0;
    uint32 character;
    /* Preserve the original conversion before extracting decimal digits */
    value = (sint32)((uint32)value * 1000u) / 1014;
    digit = value / 1000;
    if (digit > 0) { *output++ = (char)(digit + 48); value -= 1000 * digit; written = 1; }
    digit = value / 100;
    if (digit >= 1 || written) { *output++ = (char)(digit + 48); value %= 100; written = 1; }
    digit = value / 10;
    if (digit >= 1 || written) { *output++ = (char)(digit + 48); value %= 10; }
    *output++ = (char)(value + 48);
    do { character = r_u8(suffix++); *output++ = (char)character; } while (character);
}

void gtos_menu_text_cells(uint32 tag, sint32 x, sint32 y, const char *text, sint32 cell_width, uint32 font)
{
    while (*text) {
        sint32 width;
        w_u8(0x8004C210u, (uint8)*text++);
        width = (sint32)gtos_menu_text_width(font, 0x8004C210u);
        gtos_menu_text(tag, (sint32)((uint32)x + (uint32)((cell_width - width) >> 1)), y, 0x8004C210u);
        x = (sint32)((uint32)x + (uint32)cell_width);
    }
}

static void gtos_menu_character(uint32 tag, sint32 x, sint32 y, uint8 character)
{
    uint32 mode = r_u32(0x8007A024u), packet, glyph = r_u32(0x80079FF0u) + 8u * character;
    gtos_menu_draw_mode(tag, r_u16(0x80079FFAu) | (r_u32(0x1F801814u) & 0x600u));
    if (!r_u8(glyph + 3u)) return;
    packet = r_u32(0x8007A024u);
    w_u32(packet, 0x04000000u);
    w_u32(packet + 4u, r_u32(0x80079FFCu));
    w_u16(packet + 8u, x); w_u16(packet + 10u, y + r_u8(glyph + 5u) - 4);
    w_u16(packet + 12u, r_u16(glyph)); w_u16(packet + 14u, r_u16(0x8007A008u));
    w_u16(packet + 16u, r_u8(glyph + 2u)); w_u16(packet + 18u, r_u8(glyph + 3u));
    gtos_packet_link(mode, packet);
    w_u32(0x8007A024u, packet + 20u);
}

void gtos_menu_car_facts(uint32 tag, sint32 category, sint32 selected,
    sint32 number_x, sint32 number_y, sint32 class_x, sint32 class_y, sint32 intensity, uint32 small)
{
    uint32 data = 0x8004DF2Cu + 40u * (uint32)category + 10u * (uint32)selected;
    uint32 flags = r_u8(data + 1u), badge = 0x8004DEE0u + (flags & 48u);
    uint32 label_font = small ? 1u : 2u, class_font = small ? 2u : 3u;
    uint32 cursor = 0x80078AA4u, length = 0u;
    sint32 label_width;
    char number[16];
    gtos_menu_font(2u, 0u, ((uint32)intensity & 255u) * 0x010101u);
    gtos_menu_number(number, (sint16)r_u16(data + 8u), 0x80077AEEu);
    gtos_menu_text_cells(tag, number_x, number_y, number, 10, 2u);
    gtos_menu_image(badge, tag, number_x - r_u16(badge + 8u), number_y, (uint32)intensity);
    gtos_menu_font(label_font, 0u, ((uint32)(intensity >> 1) & 255u) * 0x010101u);
    gtos_menu_text_spacing(tag, class_x, class_y, cursor, 0u);
    while (r_u8(cursor++)) ++length;
    label_width = (sint32)(gtos_menu_text_width(label_font, 0x80078AA4u) - length);
    gtos_menu_font(class_font, 0u, ((uint32)intensity & 255u) * 0x010101u);
    gtos_menu_character(tag, class_x + label_width + 2, class_y - 7, (uint8)((flags >> 6u) + 65u));
}

void gtos_menu_car_details(uint32 tag, const GTOSCategoryDrawOutput *output, uint32 rows,
    uint32 player, uint32 badges, sint32 badge_x, sint32 badge_y, sint32 number_x,
    sint32 number_y, uint32 small, sint32 class_x, sint32 class_y, sint32 stats_x, sint32 stats_y)
{
    sint32 category = (sint8)r_u8(0x800787D3u + player), pass;
    for (pass = 0; pass < 2; ++pass) {
        sint32 selected = pass ? output->current : output->previous;
        sint32 x = pass ? output->x : output->previous_x;
        uint32 intensity = pass ? output->intensity : output->previous_intensity;
        if (!pass && selected < 0) continue;
        if (badges) {
            uint32 data = 0x8004DF2Du + 40u * (uint32)category + 10u * (uint32)selected;
            gtos_menu_drivetrain(r_u8(data) & 15u, tag, x + badge_x, badge_y, intensity >> 1u);
        }
        if ((sint8)r_u8(0x800792B0u + player) == 1) {
            gtos_menu_car_facts(tag, category, selected, x + number_x, number_y,
                x + class_x, class_y, (sint32)(intensity >> 1u), small);
            gtos_menu_stats(tag, rows, x + stats_x, stats_y, (sint32)intensity, selected);
        }
    }
}

static void gtos_menu_list_draw(uint32 tag, uint32 list, sint32 x, sint32 y,
    uint32 intensity, GTOSListDrawOutput *output)
{
    sint32 visible = (sint8)r_u8(list + 2u), selected = (sint8)r_u8(list + 1u);
    sint32 count = (sint8)r_u8(list), first = selected - visible / 2, old_first;
    sint32 last, delta = (sint8)r_u8(list + 9u) + (sint8)r_u8(list + 10u);
    sint32 extra = (sint8)r_u8(list + 3u), fade_out = 0, fade_in = 0, scroll_offset = 0;
    uint32 alignment = r_u8(list + 8u) & 31u, row = 0u;
    sint32 cursor_y = (uint16)y, index;
    w_u8(0x80079FFFu, 0x64u);
    if (first < 0) first = 0;
    if (count < first + visible) first = count - visible;
    last = first + visible;
    old_first = (sint8)r_u8(list + 12u) - visible / 2;
    if (old_first < 0) old_first = 0;
    if (count < old_first + visible) old_first = count - visible;
    if (first != old_first) {
        w_u8(list + 13u, old_first < first ? 3u : 4u);
        w_u8(list + 14u, 8u);
    }
    if (r_u8(list + 13u)) {
        sint32 remaining = (sint8)(uint8)(r_u8(list + 14u) - 1u);
        w_u8(list + 14u, (uint8)remaining);
        if (remaining <= 0) {
            w_u8(list + 13u, 0u); w_u8(list + 14u, 0u);
        } else {
            if (r_u8(list + 13u) == 3u) {
                --first;
                scroll_offset = delta * (8 - remaining) / 8;
                cursor_y -= scroll_offset;
                fade_out = first; fade_in = last - 1;
            } else if (r_u8(list + 13u) == 4u) {
                ++last;
                scroll_offset = delta * remaining / 8;
                cursor_y -= scroll_offset;
                fade_in = first; fade_out = last - 1;
            }
            scroll_offset -= delta;
        }
    }
    for (index = first; index < last; ++index, ++row, cursor_y += delta) {
        uint32 entry = r_u32(list + 4u) + 8u * (uint32)index;
        uint32 flags = r_u8(entry), kind = flags & 63u, data = r_u32(entry + 4u);
        uint32 level = flags & 64u ? intensity >> 1u : intensity, color, font = 3u;
        sint32 draw_x = x, width;
        if (row >= 24u) {
            /* TODO Handle list layouts exceeding the original local output capacity */
            gt_draft_unresolved_call(0x80022BC0u, list, row, 0u, 0u, 0u, 0u, 0u, 0u);
        }
        if (r_u8(list + 13u)) {
            uint32 remaining = r_u8(list + 14u);
            if (index == fade_out) level = level * remaining / 8u;
            else if (index == fade_in) level -= level * remaining / 8u;
        }
        color = (level >> 1u) * 0x010101u;
        if (kind < 2u || (kind >= 4u && (kind >= 12u || kind < 10u))) {
            if (index == selected && !(r_u8(list + 8u) & 32u)) color = gtos_menu_selection_color(level >> 1u);
            w_u8(0x80079FFCu, color); w_u8(0x80079FFDu, color >> 8u); w_u8(0x80079FFEu, color >> 16u);
        } else if (index == selected && !(r_u8(list + 8u) & 32u)) {
            uint32 pulse = 0x8004DC30u + 3u * (uint32)(sint32)(sint8)r_u8(0x8004DC70u);
            level = level * r_u8(pulse) / 255u;
        }
        if (kind == 4u || kind == 5u || kind == 6u || kind == 7u || kind == 9u || kind == 11u || kind == 13u)
            cursor_y += extra;
        if (kind == 2u || kind == 6u || kind == 3u || kind == 7u) {
            width = r_u16(data + 8u);
            if (kind == 3u || kind == 7u) width = (width * r_u8(entry + 1u)) >> (alignment == 2u ? 7u : 8u);
            else if (!alignment) width >>= 1;
            if (!alignment || alignment == 2u) draw_x -= width;
            if (kind == 2u || kind == 6u)
                gtos_menu_image(data, tag + 4u, (sint16)draw_x, (sint16)cursor_y, level >> 1u);
            else gtos_menu_scaled_image(data, tag + 4u, (sint16)draw_x, (sint16)cursor_y,
                level >> 1u, r_u8(entry + 1u));
        } else if (kind != 8u && kind != 9u && kind != 10u && kind != 11u) {
            if (kind == 1u || kind == 5u || (kind >= 12u && kind <= 14u)) font = 2u;
            if (kind >= 12u && kind <= 14u) {
                uint32 line_color = gtos_menu_color(r_u8(entry + 1u), level);
                width = kind == 14u ? (sint32)gtos_menu_text_width(2u, data) : 320;
                gtos_menu_rectangle(tag + 4u, x, cursor_y + 17, x + width - 1, cursor_y + 17, line_color | 0x02000000u);
                gtos_menu_rectangle(tag + 4u, x - 20, cursor_y + 3, x - 10, cursor_y + 13, line_color);
            }
            /* Font selection preserves the color and packet flags */
            gtos_menu_font(font, r_u8(0x80079FFFu) & 3u, r_u32(0x80079FFCu) & 0xFFFFFFu);
            width = (sint32)gtos_menu_text_width(font, data);
            if (!alignment) draw_x -= width >> 1;
            else if (alignment == 2u) draw_x -= width;
            gtos_menu_text(tag + 4u, (sint16)draw_x, (sint16)cursor_y, data);
        }
        if (output) {
            output->items[row] = (sint8)index;
            output->intensity[row] = (uint8)(level >> 1u);
            output->x[row] = (sint16)draw_x; output->y[row] = (sint16)cursor_y;
        }
    }
    if (r_u8(0x8004DC79u)) {
        uint32 color = gtos_menu_color(0u, intensity);
        if (first) gtos_menu_arrow(tag, (sint16)x, (sint16)(y - 8), color, 0u);
        if (last != count) gtos_menu_arrow(tag, (sint16)x,
            (sint16)cursor_y + scroll_offset + 9, color, 1u);
        gtos_menu_draw_mode(tag, 544u);
    }
    if (output) {
        output->offset = (sint16)scroll_offset;
        if (row < 24u) output->items[row] = -1;
        else {
            /* The original terminator aliases the first intensity byte */
            output->intensity[0] = 255u;
        }
    }
}

static void gtos_menu_background(uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    uint32 color = (intensity & 255u) * 0x010101u | 0x01000000u;
    gtos_menu_sprite(tag, ((uint32)y << 16u) | (uint16)x, 0x3FF00000u, 0x00F00100u, color);
    gtos_menu_draw_mode(tag, 684u);
    gtos_menu_sprite(tag, ((uint32)y << 16u) | (uint16)(x + 256), 0x3FF00000u, 0x00F00040u, color);
    gtos_menu_draw_mode(tag, 686u);
}

static void gtos_arcade_level_draw(uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    GTOSListDrawOutput output;
    gtos_menu_list_draw(tag + 4u, 0x8004E174u, x + 160, y + 88, intensity, &output);
    gtos_menu_background(tag + 8u, x, y, intensity >> 1u);
}

static void gtos_arcade_root_draw(uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    GTOSListDrawOutput output;
    uint32 i = 0u;
    gtos_menu_list_draw(tag + 4u, 0x8004E0ECu, x + 160, y + 56, intensity, &output);
    while (i < 24u && output.items[i] >= 0) {
        sint32 icon_x = (sint16)(x + 48), icon_y = (sint16)(output.y[i] + 5);
        gtos_menu_rectangle(tag, icon_x + 1, icon_y + 1, icon_x + 12, icon_y + 12,
            gtos_menu_color(r_u8(0x8004E334u + i), intensity));
        gtos_menu_rectangle(tag, icon_x, icon_y, icon_x + 13, icon_y + 13, 0u);
        ++i;
    }
    gtos_menu_background(tag + 8u, x, y, intensity >> 1u);
}

static void gtos_arcade_car_type_draw(uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    GTOSListDrawOutput output;
    uint32 i;
    gtos_menu_list_draw(tag, 0x8004E14Cu, x + 160, y + 60, intensity, &output);
    if (r_u8(0x8007ABDDu)) return;
    for (i = 0u; i < 24u && output.items[i] >= 0; ++i) {
        if (output.items[i] == 1) {
            uint32 level = output.intensity[i];
            sint32 first_width = r_u16(0x8004DD78u), second_width = r_u16(0x8004DE28u);
            if ((sint8)r_u8(0x8004E14Du) == 1) {
                uint32 pulse = 0x8004DC30u + 3u * (uint32)(sint32)(sint8)r_u8(0x8004DC70u);
                level = level * r_u8(pulse) / 255u;
            }
            gtos_menu_image(0x8004DD70u, tag + 4u,
                output.x[i] - ((first_width + second_width) >> 1), output.y[i], level);
            gtos_menu_image(0x8004DE20u, tag + 4u,
                output.x[i] - ((second_width - first_width) >> 1), output.y[i], level);
        }
    }
}

static uint32 gtos_car_renderer(uint32 tag, sint32 x, sint32 y, sint32 rotation,
    uint32 model, uint32 texture, uint32 palette, uint32 depth, uint32 color,
    sint32 distance, uint32 flags)
{
    uint8 camera[180];
    uint32 vector[3], i, context = 0x1F800000u;
    sint32 height;
    /* Keep the camera vector and matrices in one native object */
    xport_store_le16(camera, 0u); xport_store_le16(camera + 2u, 0u);
    xport_store_le16(camera + 4u, 320u); xport_store_le16(camera + 6u, 240u);
    xport_store_le16(camera + 8u, (uint16)x); xport_store_le16(camera + 10u, (uint16)y);
    xport_store_le16(camera + 12u, (uint16)distance);
    xport_store_le16(camera + 14u, 64u); xport_store_le16(camera + 16u, 64u);
    gtos_model_camera_initialize(camera);
    gtos_model_camera_initialize(camera);
    gtos_model_transform_identity(camera + 120u);
    gtos_model_rotation(camera + 140u, (sint16)(rotation + 2048), (sint16)r_u16(0x8004E524u), 0);
    vector[0] = 0u; vector[1] = 52428u; vector[2] = (uint32)-786432;
    gtos_model_rotate_vector(vector, camera + 140u, vector);
    for (i = 0u; i < 3u; ++i) xport_store_le32(camera + 120u + 4u * i, vector[i]);
    gtos_model_camera_orientation(camera, psx_addr(0x8004FB44u, 20u));
    gtos_model_camera_context_native(context, camera);
    gtos_model_depth_cue(context, 0u, 256, 0u);
    gte_ctc2(r_u16(context + 32u), 26u);
    gte_ctc2(r_u32(context + 24u), 24u); gte_ctc2(r_u32(context + 28u), 25u);
    w_u32(context + 916u, (color & 65535u) | ((texture | 32u) << 16u));
    w_u16(context + 920u, 64u); w_u16(context + 922u, 64u); w_u16(context + 924u, 0u);
    w_u32(context + 904u, (palette & 65535u) << 16u);
    w_u32(context + 912u, 0x10000000u); w_u16(context + 910u, 6u);
    gtos_model_transform_identity(psx_addr(context + 864u, 40u));
    height = 16 * ((sint16)r_u16(model + 50u) - (sint16)r_u16(model + 18u));
    w_u16(context + 908u, 0u); w_u32(context + 928u, tag); w_u32(context + 868u, (uint32)height);
    gtos_model_prepare(context, model + 60u, 1u, camera);
    w_u16(context + 936u, palette); w_u16(context + 938u, depth);
    gtos_model_wheels(context, model, (sint16)r_u16(0x8004E526u), 0, 0);
    w_u32(context + 868u, 0u);
    gtos_model_secondary(context, r_u32(model + 56u));
    if (flags & 1u) {
        w_u32(context + 868u, 0u - (uint32)height);
        gtos_model_reflect(psx_addr(context + 884u, 20u), psx_addr(context + 884u, 20u));
        w_u16(context + 908u, 65535u); w_u32(context + 928u, tag + 8192u);
        gtos_model_prepare(context, model + 60u, 1u, camera);
        w_u16(context + 936u, palette); w_u16(context + 938u, depth);
        gtos_model_wheels(context, model, (sint16)r_u16(0x8004E526u), 0, 0);
    }
    if (flags & 2u) {
        uint32 magnitude = 0u, center, previous, next, packet, shadow_tag = tag + 4096u;
        sint32 highest, coordinate_shift, table_shift;
        gtos_model_rotation(camera + 140u, 0, (sint16)r_u16(0x8004E524u), 0);
        vector[0] = 0u; vector[1] = 52428u; vector[2] = (uint32)-786432;
        gtos_model_rotate_vector(vector, camera + 140u, vector);
        for (i = 0u; i < 3u; ++i) xport_store_le32(camera + 120u + 4u * i, vector[i]);
        gtos_model_camera_orientation(camera, psx_addr(0x8004FB44u, 20u));
        for (i = 0u; i < 3u; ++i) {
            vector[i] = 0u - vector[i];
            magnitude |= (sint32)vector[i] < 0 ? 0u - vector[i] : vector[i];
        }
        highest = 31 - (sint32)gtos_leading_sign_bits(magnitude);
        if (highest < 18) highest = 18;
        table_shift = (highest - 18) & 31; coordinate_shift = (highest - 12) & 31;
        gte_lwc2(0u, ((uint32)((sint32)vector[0] >> coordinate_shift) & 65535u) |
            ((uint32)((sint32)vector[1] >> coordinate_shift) << 16u));
        gte_lwc2(1u, (uint32)((sint32)vector[2] >> coordinate_shift));
        gtos_model_reflected_rotation(camera + 160u);
        xport_gte_execute(0x486012u);
        for (i = 0u; i < 3u; ++i) gte_ctc2(xport_gte_read_data(25u + i), 5u + i);
        gte_lwc2(0u, 0u); gte_lwc2(1u, 0u); xport_gte_execute(0x180001u);
        center = xport_gte_read_data(14u);
        gte_lwc2(0u, (uint16)((sint16)r_u16(0x8004FB58u) >> table_shift));
        gte_lwc2(1u, (uint32)((sint16)r_u16(0x80050358u) >> table_shift));
        xport_gte_execute(0x180001u); previous = xport_gte_read_data(14u);
        packet = r_u32(0x8007A024u);
        for (i = 64u; i <= 4096u; i += 64u) {
            uint32 angle = i & 4095u;
            gte_lwc2(0u, (uint16)((sint16)r_u16(0x8004FB58u + 2u * angle) >> table_shift));
            gte_lwc2(1u, (uint32)((sint16)r_u16(0x80050358u + 2u * angle) >> table_shift));
            xport_gte_execute(0x180001u); next = xport_gte_read_data(14u);
            w_u32(packet, 0x06000000u); w_u32(packet + 4u, 0x32404040u);
            w_u32(packet + 8u, center); w_u32(packet + 12u, 0u);
            w_u32(packet + 16u, previous); w_u32(packet + 20u, 0u); w_u32(packet + 24u, next);
            gtos_packet_link(shadow_tag, packet);
            previous = next; packet += 28u;
        }
        w_u32(0x8007A024u, packet); gtos_menu_draw_mode(shadow_tag, 544u);
        if (!(flags & 4u)) {
            gtos_menu_rectangle(shadow_tag, (sint16)r_u16(0x8004E528u), (sint16)r_u16(0x8004E52Au),
                (sint16)r_u16(0x8004E52Cu), (sint16)r_u16(0x8004E52Eu), 0x02000000u);
            gtos_menu_draw_mode(shadow_tag, 512u);
        }
        packet = r_u32(0x8007A024u); gtos_menu_draw_mode(shadow_tag, 576u);
        return packet;
    }
    return flags & 2u;
}

static void gtos_arcade_car_model(uint32 tag, sint32 x, sint32 y, uint32 player,
    sint32 selected, sint32 rotation, sint32 distance, uint32 flags)
{
    if (!(sint8)r_u8(0x800762D4u + player)) {
        gtos_menu_scaled_image(0x8004DB18u, tag, x - (r_u16(0x8004DB20u) >> 1u), y, 128u, 128u);
    } else if ((sint8)r_u8(0x800762D6u + player) == 8) {
        uint32 model = player == 1u ? 0x80140F7Cu : 0x8013AF7Cu;
        uint32 palette = (((uint32)selected << 8u) + 0x4000u) | (player == 1u ? 32u : 24u);
        gtos_car_renderer(tag, x, y, rotation, model, player == 1u ? 23u : 21u,
            palette & 65535u, 29u, 0x7C38u, distance, flags);
    }
}

static void gtos_arcade_car_draw(uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    GTOSCategoryDrawOutput output;
    sint32 mode = (sint8)r_u8(0x800792B0u), category = (sint8)r_u8(0x800787D3u), pass;
    gtos_menu_category_draw(&output, tag, 0x8004E414u, x + 160, y + 94, (sint32)intensity, mode == 1);
    if (mode != 1)
        gtos_menu_binary_draw(tag, mode == 2 ? 0x8004E464u : 0x8004E474u,
            x + 160, y + 200, (sint32)intensity, mode == 2, 0u, 0u);
    gtos_menu_car_details(tag, &output, 0x800792A0u, 0u, 1u, 128, y + 118,
        -32, y + 208, 0u, -144, y + 208, 27, y + 198);
    for (pass = 0; pass < 2; ++pass) {
        sint32 selected = pass ? output.current : output.previous;
        sint32 position = pass ? output.x : output.previous_x;
        uint32 fade = pass ? output.intensity : output.previous_intensity;
        uint32 data, image;
        if (!pass && selected < 0) continue;
        data = 0x8004DF2Cu + 40u * (uint32)category + 10u * (uint32)selected;
        image = 0x8004DD60u + 16u * (uint32)(sint32)(sint8)r_u8(data);
        gtos_menu_image(image, tag, position - (r_u16(image + 8u) >> 1u), y + 46, fade >> 1u);
    }
    if (mode == 1 && (sint8)r_u8(0x80079278u) >= 2) {
        uint32 color = gtos_menu_selection_color(intensity);
        if (r_u8(0x8004DC79u)) {
            gtos_menu_arrow(tag, output.x - 136, y + 180, color, 0u);
            gtos_menu_arrow(tag, output.x - 136, y + 204, color, 1u);
        }
        gtos_menu_font(1u, 0u, ((intensity >> 2u) & 255u) * 0x010101u);
        gtos_menu_text_spacing(tag, output.x - 144, y + 188, 0x80078B1Bu, 0u);
    }
    gtos_arcade_car_model(tag, output.x, y + 148, 0u,
        (sint8)r_u8(0x80079270u), (sint16)r_u16(0x80079274u), 570, 3u);
    w_u16(0x80079274u, r_u16(0x80079274u) + 10u);
    if ((sint16)r_u16(0x80079274u) >= 4096) w_u16(0x80079274u, 0u);
}


static sint32 gtos_track_text_width(uint32 text, uint32 spacing)
{
    uint32 count = 0u, cursor = text;
    while (r_u8(cursor++)) ++count;
    return (sint32)(gtos_menu_text_width(1u, text) + count * (spacing - 1u));
}

static void gtos_track_record_draw(uint32 tag, sint32 x, sint32 y,
    uint32 intensity, uint32 record, uint32 line_color)
{
    uint32 milliseconds = r_u32(record + 4u), name, index, i;
    sint32 name_width, car_width, gap;
    char clock[16];
    gtos_menu_font(1u, 0u, gtos_menu_color(0u, intensity));
    if (!milliseconds) {
        gtos_menu_text_spacing(tag, x - 40, y, 0x80078C46u, 0u);
        return;
    }
    /* Preserve the original time cap and both display formats */
    if (milliseconds == 0xFFFFFFFFu) strcpy(clock, "--:--:---");
    else {
        uint32 seconds, minutes, hours;
        if (milliseconds > 35999999u) milliseconds = 35999999u;
        seconds = milliseconds / 1000u;
        minutes = seconds / 60u;
        hours = minutes / 60u;
        if (hours) sprintf(clock, "%u-%02u'%02u\"%u", hours, minutes % 60u,
            seconds % 60u, milliseconds % 1000u / 100u);
        else sprintf(clock, "%2u:%02u:%03u", minutes % 60u, seconds % 60u, milliseconds % 1000u);
    }
    {
        uint32 advances = (uint8)clock[0] - 48u < 10u && clock[1] == '-' ? 0x8004E804u : 0x8004E7F8u;
        sint32 position = x - 40;
        for (i = 0u; i < 9u && clock[i]; ++i) {
            w_u8(0x8004C210u, (uint8)clock[i]);
            gtos_menu_text(tag, position + (clock[i] == '1' ? 2 : 0), y, 0x8004C210u);
            position = (sint32)((uint32)position + (uint32)(sint32)(sint8)r_u8(advances + i));
        }
    }
    gtos_menu_font(1u, 0u, (intensity & 255u) * 0x010101u);
    name = r_u32(0x801B16ECu);
    index = r_u16(record + 8u);
    while (index--) name += r_u8(name) + 1u;
    ++name;
    car_width = gtos_track_text_width(name, 0u);
    name_width = gtos_track_text_width(record, 1u);
    gtos_menu_text_spacing(tag, x - (car_width - 128), y + 10, name, 0u);
    gtos_menu_text_spacing(tag, x - 120, y + 10, record, 1u);
    gap = 246 - name_width - car_width;
    if (gap - 2 > 0)
        gtos_menu_outline(tag, x + name_width - 118, y + 17,
            x + name_width - 120 + gap - 1, y + 17, line_color);
}

static void gtos_track_details_draw(uint32 tag, sint32 x, sint32 y, uint32 intensity,
    sint32 selected, uint32 preview, sint32 vertical, uint32 parity_offset)
{
    sint32 mode = (sint8)r_u8(0x800787D1u);
    uint32 record = mode == 1 ? 0x80080F1Cu + 180u * (uint32)selected :
        mode == 6 ? 0x800814BCu + 180u * (uint32)selected : 0x80080DDCu + 40u * (uint32)selected;
    uint32 color = gtos_menu_color(r_u8(0x8004E3C4u), intensity), text;
    gtos_menu_font(1u, 0u, gtos_menu_color(7u, intensity));
    if (mode != 2) {
        gtos_menu_text_spacing(tag, x - 144, y + 176, 0x80078C0Au, 0u);
        gtos_menu_text_spacing(tag, x - 144, y + 200, 0x80078C18u, 0u);
        gtos_track_record_draw(tag, x, y + 176, intensity, record + 20u, color);
        gtos_track_record_draw(tag, x, y + 200, intensity, record, color);
    }
    gtos_menu_font(2u, 0u, (intensity & 255u) * 0x010101u);
    text = r_u32((mode == 6 ? 0x8004E220u : 0x8004E18Cu) + 4u * (uint32)selected);
    gtos_menu_text(tag, x - ((sint32)gtos_menu_text_width(2u, text) >> 1), y + 68, text);
    if (preview) {
        uint32 parity = (parity_offset + (uint32)selected) & 1u;
        uint32 xy = ((uint32)(y + vertical + 88) << 16u) | (uint16)(x - 16);
        uint32 uv = 20480u * parity | ((0x7DC0u | ((896u + 16u * parity) >> 4u)) << 16u);
        gtos_menu_sprite(tag + 8u, xy, uv, 0x005000A0u,
            0x01000000u | (intensity & 255u) * 0x010101u);
        gtos_menu_draw_mode(tag + 8u, 570u);
    }
}

void gtos_arcade_track_draw(uint32 tag, sint32 x, sint32 y, uint32 intensity)
{
    GTOSCategoryDrawOutput output;
    sint32 mode = (sint8)r_u8(0x800787D1u), vertical = mode == 2 ? 16 : 0;
    uint32 list = mode == 6 ? 0x8004E250u : 0x8004E20Cu;
    uint32 labels = mode == 6 ? 0x800786F0u : 0x80078750u;
    uint32 parity_offset = mode == 6 ? 44u : 36u, level;
    sint32 visible;
    gtos_menu_category_draw(&output, tag, list, x + 160, y + 62, (sint32)intensity, 1u);
    if (output.previous >= 0) {
        level = output.previous_intensity >> 1u;
        gtos_track_details_draw(tag, output.previous_x, y, level, output.previous, 0u, vertical, parity_offset);
        gtos_menu_image(labels + 16u * (uint32)(sint32)output.previous, tag,
            output.previous_x - 144, y + vertical + 84, level);
    }
    level = output.intensity >> 1u;
    visible = gtos_arcade_track_preview_visible();
    gtos_track_details_draw(tag + 4u, output.x, y, level, output.current,
        (uint32)visible >> 31u, vertical, parity_offset);
    gtos_menu_image(labels + 16u * (uint32)(sint32)output.current, tag + 4u,
        output.x - 144, y + vertical + 84, level);
    if (visible >= 0) {
        uint32 xy = ((uint32)(y + vertical + 88) << 16u) | (uint16)(output.x - 16);
        gtos_menu_sprite(tag + 4u, xy, 0x3FF00000u | (((uint32)visible << 15u) & 0x8000u),
            0x005000A0u, 0u);
        gtos_menu_draw_mode(tag + 4u, 807u);
    }
}

void gtos_menu_page_draw(uint32 state, sint32 x, sint32 y, uint32 previous)
{
    uint32 tag = r_u32(state + 184u) + 4u, framebuffer = r_u32(0x8007A13Cu) ? 0u : 240u;
    uint32 descriptor = previous == 1u ? r_u32(state + 188u) :
        r_u32(state + 192u + 4u * (uint32)(sint32)(sint8)r_u8(state + 256u));
    uint32 text = r_u32(descriptor + 4u), font_style = r_u8(state + 274u);
    uint32 distance = y < 0 ? 0u - (uint32)y : (uint32)y, intensity, centered_line = 0u;
    sint32 width, title_x, title_y, alignment = (sint8)r_u8(descriptor + 1u);
    if (distance >= 181u) distance = 180u;
    intensity = 255u * (180u - distance) / 180u;
    if (text) {
        gtos_menu_font(3u, 2u, (intensity >> 1u) * 0x010101u);
        width = (sint32)gtos_menu_text_width(3u, text);
        if (font_style) {
            title_x = x + ((320 - width) >> 1);
            title_y = y + 17;
            if (!alignment) title_x = x + 16;
            else if (alignment == 3) { title_y = y + 109; centered_line = 1u; }
            gtos_menu_text(tag + 16u, title_x, title_y, text);
            gtos_menu_rectangle(tag + 16u, title_x, title_y + 24, title_x + (sint16)width - 1,
                title_y + 24, intensity & 255u);
        } else {
            uint32 cursor = text;
            while (r_u8(cursor)) { ++cursor; --width; }
            title_x = x + 16;
            if (alignment == 2) title_x = x - ((width >> 1) - 160);
            else if (alignment) title_x = x - (width - 304);
            title_y = y + 19;
            gtos_menu_rectangle(tag + 16u, title_x, y + 38, title_x + (sint16)width - 1,
                y + 38, intensity & 255u);
            gtos_menu_text_spacing(tag + 16u, title_x, y + 14, text, 0u);
        }
        {
            uint32 color = gtos_menu_color(r_u8(descriptor + 24u), intensity);
            if (centered_line) {
                gtos_menu_rectangle(tag + 16u, x, title_y + 13, title_x - 3, title_y + 14, color);
                gtos_menu_rectangle(tag + 16u, x - (title_x - x - 322), title_y + 13,
                    x + 319, title_y + 14, color);
            } else gtos_menu_rectangle(tag + 16u, x, y, x + 319, y + 27, color);
        }
        if (r_u32(descriptor + 12u) == 0x80019D74u) gtos_arcade_root_draw(tag + 32u, x, y, intensity);
        else if (r_u32(descriptor + 12u) == 0x80019F7Cu) gtos_arcade_level_draw(tag + 32u, x, y, intensity);
        else if (r_u32(descriptor + 12u) == 0x8001A130u) gtos_arcade_car_type_draw(tag + 32u, x, y, intensity);
        else if (r_u32(descriptor + 12u) == 0x8001A694u) gtos_arcade_car_draw(tag + 32u, x, y, intensity);
        else if (r_u32(descriptor + 12u) == 0x8001B14Cu) gtos_arcade_track_draw(tag + 32u, x, y, intensity);
        else {
            /* TODO Translate the remaining page drawing callbacks */
            gt_draft_unresolved_call(r_u32(descriptor + 12u), tag + 32u, (uint32)x, (uint32)y,
                framebuffer, intensity, 0u, 0u, 0u);
        }
        if (!font_style) {
            gtos_menu_clip(tag + 64u, framebuffer << 11u, framebuffer << 10u, ((framebuffer + 207u) << 10u) | 319u);
            if (!previous) {
                uint32 flags = r_u8(state + 258u), kind = flags & 15u, row, position = 192u;
                if (kind == 2u || kind == 3u)
                    gtos_menu_sprite(tag, 0xD00000u | (kind == 2u ? 112u : 120u), r_u32(0x8004EDFCu), r_u32(0x8004EDF8u), 0x808080u);
                row = (r_u8(r_u32(state + 152u) + 1u) & 0xF0u) == 0x20u;
                if (flags & 16u) { position = 160u; row += 2u; }
                gtos_menu_sprite(tag, 0xD00000u | position, r_u32(0x8004EDBCu + 16u * row),
                    r_u32(0x8004EDB8u + 16u * row), 0x808080u);
            }
            gtos_menu_rectangle(tag, 0, 208, 319, 208, 0x333333u);
            gtos_menu_draw_mode(tag, (uint32)(sint32)(sint8)(r_u8(0x8004EDB1u) | 32u) | 512u);
        }
        gtos_menu_clip(tag + 16u, framebuffer << 11u, framebuffer << 10u, ((framebuffer + 239u) << 10u) | 319u);
    }
    gtos_menu_clip(tag + 16360u, framebuffer << 11u, framebuffer << 10u, ((framebuffer + 239u) << 10u) | 319u);
}

static uint32 gtos_arcade_path(void)
{
    uint32 entry = r_u32(0x801B1704u);
    while (r_u8(entry)) {
        uint32 length = 0u;
        while (r_u8(entry + length)) ++length;
        if (length >= 12u && !memcmp(psx_addr(entry, 12u), "arcade.file=", 12u)) return entry + 12u;
        entry += length + 1u;
    }
    return gt_draft_unresolved_call(0x80041010u, 0x8004B710u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

void gtos_data_unpack(uint32 output, uint32 input, uint32 bytes)
{
    uint32 written = 0u;
    uint32 flags = 0u, remaining = 0u;
    while (written < bytes) {
        if (!remaining) { flags = r_u8(input++); remaining = 8u; }
        if (flags & 1u) {
            uint32 length = r_u8(input++) + 3u, part = r_u8(input++), distance = part & 127u;
            if (part & 128u) distance = (distance << 8u) | r_u8(input++);
            ++distance;
            if (distance > written)
                gt_draft_unresolved_call(0x8002BFE8u, input, distance, written, 0u, 0u, 0u, 0u, 0u);
            while (length-- && written < bytes) {
                w_u8(output + written, r_u8(output + written - distance));
                ++written;
            }
        } else w_u8(output + written++, r_u8(input++));
        flags >>= 1u; --remaining;
    }
}

static void gtos_arcade_unpack(uint32 archive, uint32 index, uint32 output)
{
    gtos_data_unpack(output, archive + r_u32(archive + 16u + 12u * index),
        r_u32(archive + 24u + 12u * index));
}

static void gtos_tim_block(uint32 block, uint32 position, uint32 size)
{
    PSX_RECT rect;
    if (position == 0xFFFFFFFFu) position = r_u32(block + 4u);
    if (size == 0xFFFFFFFFu) size = r_u32(block + 8u);
    rect.x = (sint16)position; rect.y = (sint16)(position >> 16u);
    rect.w = (sint16)size; rect.h = (sint16)(size >> 16u);
    DrawSync(0);
    LoadImagePSX(&rect, (uint32 *)psx_addr(block + 12u, (uint32)(uint16)rect.w * (uint16)rect.h * 2u));
    DrawSync(0);
}

void gtos_arcade_track_label(uint32 index)
{
    uint32 image = 0x8008CF7Cu, parity = index & 1u;
    uint32 pixels = ((256u + 80u * parity) << 16u) | 640u;
    uint32 palette = (896u + 16u * parity) | 0x01F70000u;
    gtos_arcade_unpack(r_u32(0x800787E0u), index, image);
    if (r_u32(image + 4u) & 8u) {
        uint32 palette_block = image + 8u;
        gtos_tim_block(palette_block + r_u32(palette_block), pixels, 0xFFFFFFFFu);
        gtos_tim_block(palette_block, palette, 0xFFFFFFFFu);
    } else gtos_tim_block(image + 8u, pixels, 0xFFFFFFFFu);
}

static void gtos_arcade_texture(uint32 archive, uint32 index, uint32 palette, uint32 pixels)
{
    uint32 image = 0x8013AF7Cu;
    gtos_arcade_unpack(archive, index, image);
    if (r_u32(image + 4u) & 8u) {
        uint32 palette_block = image + 8u;
        gtos_tim_block(palette_block, palette, 0xFFFFFFFFu);
        gtos_tim_block(palette_block + r_u32(palette_block), pixels, 0xFFFFFFFFu);
    } else gtos_tim_block(image + 8u, pixels, 0xFFFFFFFFu);
}

void gtos_arcade_category_image(sint32 category, sint32 selected, uint32 player)
{
    uint32 image = 0x8008CF7Cu;
    uint32 entry = 0x8004DF32u + 40u * (uint32)category + 10u * (uint32)selected;
    uint32 index = (uint32)(sint32)(sint16)r_u16(entry);
    uint32 palette = (896u + 16u * player) | 0x01FF0000u;
    uint32 pixels = ((256u + 48u * player) << 16u) | 896u;
    gtos_arcade_unpack(r_u32(0x800787E0u), index, image);
    if (r_u32(image + 4u) & 8u) {
        uint32 block = image + 8u;
        gtos_tim_block(block + r_u32(block), pixels, 0xFFFFFFFFu);
        gtos_tim_block(block, palette, 0xFFFFFFFFu);
    } else gtos_tim_block(image + 8u, pixels, 0xFFFFFFFFu);
}

static uint32 gtos_arcade_sequence_relocate(uint32 sequence)
{
    uint32 count, delta, i, j;
    if (r_u32(sequence) != 0x47514553u) return 0u;
    delta = sequence - r_u32(sequence + 4u);
    count = r_u32(sequence + 8u);
    w_u32(sequence + 4u, sequence);
    for (i = 0u; i < count; ++i) {
        for (j = 0u; j < 16u; ++j) {
            uint32 field = sequence + 72u * i + 20u + 4u * j;
            uint32 pointer = r_u32(field);
            if (pointer) w_u32(field, pointer + delta);
        }
    }
    return sequence;
}

static sint32 gtos_arcade_resource_index(uint32 name)
{
    sint32 count = (sint16)r_u16(0x801B16CEu), i;
    uint32 table = r_u32(0x801B16E4u) + 32u;
    for (i = 0; i < count; ++i)
        if (!strcmp((const char *)psx_addr(name, 1u), (const char *)psx_addr(table + 6u * i, 1u))) return i;
    return -1;
}

void gtos_arcade_graphics_load(void)
{
    uint32 path, size = 0u, archive = 0x8009E77Cu;
    size_t received = 0u;
    FILE *directory = NULL;
    gtos_cd_movie_mode();
    /* Read the original first two CARCADE sectors used as its archive directory */
    if (fopen_s(&directory, "DATA/CARCADE.DAT", "rb") || !directory)
        gt_draft_unresolved_call(0x80018594u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    received = fread(psx_addr(0x8009677Cu, 4096u), 1u, 4096u, directory);
    fclose(directory);
    if (received != 4096u)
        gt_draft_unresolved_call(0x80018594u, (uint32)received, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x800762A8u, 0x8009677Cu);
    printf("arcs = %d\n", (sint16)r_u16(0x8009678Au));
    path = gtos_arcade_path();
    if (!xport_cd_file_size(path, "DATA", &size) || size > 0x8013AF7Cu - archive ||
        !xport_cd_file_read(path, "DATA", archive, size, &received) || received != size)
        gt_draft_unresolved_call(0x80029DD4u, path, archive, size, 0u, 0u, 0u, 0u, 0u);
    gtos_arcade_texture(archive, 72u, 15729600u, 6292416u);
    gtos_arcade_texture(archive, 47u, 16712448u, 768u);
    gtos_arcade_texture(archive, 74u, 32506816u, 16778176u);
    gtos_arcade_texture(archive, 73u, 32506432u, 16777792u);
    {
        uint32 i, mode = r_u8(0x8007ABDDu);
        for (i = 0u; i < 8u; ++i) {
            uint32 row = (uint32)-2146990256 + 16u * i;
            uint32 px = 16u * (i & 3u) + 896u, py = (i >> 2u) + 504u;
            gtos_arcade_texture(archive, 75u + i, px | (py << 16u),
                (32u * (i & 3u) + 704u) | ((88u * ((i & 4u) >> 2u) + 256u) << 16u));
            w_u16(row, ((i & 3u) >> 1u) + 27u);
            w_u32(row + 4u, (uint8)(4u * (32u * (i & 3u) - 64u)) |
                ((uint32)(uint8)(88u * ((i & 4u) >> 2u)) << 8u) | (((px >> 4u) | (py << 6u)) << 16u));
            w_u16(row + 8u, 120u); w_u16(row + 10u, 88u); w_u32(row + 12u, 5767288u);
        }
        for (i = 0u; i < 3u; ++i) {
            uint32 row = (uint32)-2146990352 + 16u * i;
            sint32 x = (sint16)r_u16(0x8004B700u + 2u * i);
            sint32 y = (sint16)r_u16(0x8004B708u + 2u * i);
            gtos_arcade_texture(archive, 83u + i, (28311552u + (i << 16u)) | 0x3A0u,
                (uint32)x | ((uint32)y << 16u));
            w_u16(row, 29u);
            w_u32(row + 4u, (uint8)(4 * x) | ((uint32)(uint8)y << 8u) |
                (((27648u + 64u * i) | 0x3Au) << 16u));
            w_u16(row + 8u, 120u); w_u16(row + 10u, 88u); w_u32(row + 12u, 5767288u);
        }
        if (!mode) memcpy(psx_addr(0x8004DD70u, 16u), psx_addr(0x8004DE10u, 16u), 16u);
        if (mode >= 2u) {
            for (i = 0u; i < 4u; ++i) {
                uint32 row = 0x8004DEE0u + 16u * i;
                uint32 pair = 0x8004DB5Cu + 4u * (2u * mode - 4u) + 2u * i;
                w_u16(row, 15u);
                w_u32(row + 4u, (r_u32(row + 4u) & 0xFFFF0000u) | r_u8(pair) |
                    ((uint32)(uint8)(r_u8(pair + 1u) + 96u) << 8u));
            }
        }
        for (i = 0u; i < 3u; ++i) {
            uint32 row = (uint32)-2146990304 + 16u * i;
            uint32 px = 16u * (i & 3u) + 896u, py = (i >> 2u) + 506u;
            gtos_arcade_texture(archive, 3u * (mode + 1u) + 48u + i,
                px | (py << 16u), (1638400u * i) | 0x3C0u);
            w_u16(row, 15u);
            w_u32(row + 4u, ((uint32)(uint8)(25u * i) << 8u) | (((px >> 4u) | (py << 6u)) << 16u));
            w_u16(row + 8u, 208u); w_u16(row + 10u, 25u); w_u32(row + 12u, 1638608u);
        }
        gtos_arcade_texture(archive, 86u, 32506752u, 16778048u);
        gtos_arcade_unpack(archive, 88u, (uint32)-2146859140);
        w_u32(0x80076294u, gtos_arcade_sequence_relocate(0x8009877Cu));
        w_u8(0x80076298u, 0u);
        gtos_arcade_unpack(archive, 2u * (mode + 1u) + 89u, 0x80078890u);
        gtos_arcade_unpack(archive, 2u * (mode + 1u) + 90u, 0x80076ED0u);
        w_u32(0x800787E0u, archive);
        {
            uint32 end = (r_u32(0x8009E9C0u) + 0x8009E77Fu) & ~3u;
            uint32 cursor = end + 0x40000u;
            w_u32(0x800786E4u, end);
            w_u32(0x800786E0u, end + 0x20000u);
            printf("data top adr = %x, data last adr = %x\n", archive, end);
            for (i = 0u; i < 4u; ++i) {
                w_u32(0x80079240u + 4u * i, cursor);
                cursor += 10080u;
            }
            cursor = (cursor + 3u) & ~3u;
            w_u32(0x8007925Cu, cursor);
            w_u32(0x80079254u, cursor + 0x10000u);
        }
        for (i = 0u; i < 40u; ++i) {
            sint32 index = gtos_arcade_resource_index(r_u32(0x8004DA78u + 4u * i));
            uint32 record = r_u32(0x801B16E8u), skip;
            if (index < 0) {
                /* TODO Diagnose an absent original arcade resource name */
                gt_draft_unresolved_call(0x80012590u, i >> 2u, i & 3u, 0u, 0u, 0u, 0u, 0u, 0u);
            }
            for (skip = 0u; skip < (uint32)index; ++skip) record += 2u * r_u8(record) + 1u;
            w_u8(0x80078560u + i, r_u8(record));
        }
    }
}

void gtos_arcade_font_init(void)
{
    uint32 input = 0x800671F8u, written = 0u, flags = 0u, remaining = 0u;
    uint8 pixels[32768];
    PSX_RECT rect = { 320, 0, 64, 256 };
    if (!input)
        gt_draft_unresolved_call(0x80030F48u, input, 5u, 0u, 0u, 0u, 0u, 0u, 0u);
    DrawSync(0);
    while (written < sizeof(pixels)) {
        if (!remaining) { flags = r_u8(input++); remaining = 8u; }
        if (flags & 1u) {
            uint32 length = r_u8(input++) + 3u, part = r_u8(input++), distance = part & 127u;
            if (part & 128u) distance = (distance << 8u) | r_u8(input++);
            ++distance;
            if (distance > written)
                gt_draft_unresolved_call(0x8002BFE8u, distance, written, 0u, 0u, 0u, 0u, 0u, 0u);
            while (length-- && written < sizeof(pixels)) { pixels[written] = pixels[written - distance]; ++written; }
        } else pixels[written++] = r_u8(input++);
        flags >>= 1u; --remaining;
    }
    LoadImagePSX(&rect, (uint32 *)pixels);
    memset(psx_addr(0x80079FF0u, 32u), 0, 32u);
    w_u16(0x80079FF8u, 20u);
    w_u8(0x80079FFFu, 100u);
    w_u32(0x80079FF8u, (r_u32(0x80079FF8u) & 0xFE00FFFFu) | 0x00250000u);
    w_u8(0x80079FFCu, 128u); w_u8(0x80079FFDu, 128u); w_u8(0x80079FFEu, 128u);
    DrawSync(0);
    /* Select the original font-one glyph and mapping tables */
    w_u32(0x80079FF0u, 0x8005574Cu);
    w_u32(0x80079FF4u, 0x8005B24Cu);
    w_u16(0x8007A008u, r_u16(0x80079FF8u) + 64u);
    w_u16(0x8007A00Au, 1u);
    w_u16(0x8007A00Eu, 8u);
    w_u8(0x80079FFFu, 100u);
    w_u8(0x80079FFEu, 128u);
}
