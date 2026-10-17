#include "game_first2_registry.h"
#include "psx_gte.h"

void gtmain_text_flags(uint8 flags)
{
    w_u8(0x800C007Fu, flags | 0x64u);
}

uint32 gtmain_text_color(uint32 rgb)
{
    /* SWL writes the low three color bytes and preserves the command byte */
    w_u8(0x800C007Cu, (uint8)rgb);
    w_u8(0x800C007Du, (uint8)(rgb >> 8));
    w_u8(0x800C007Eu, (uint8)(rgb >> 16));
    return 0x800C007Eu;
}

uint32 gtmain_course_entry(uint32 index)
{
    return r_u32(0x800C0C0Cu) + (index << 4);
}

uint32 gtmain_course_name(uint32 index)
{
    return gtmain_packed_bytes_entry(r_u32(0x800C0C08u), index) + 1u;
}

uint32 gtmain_matrix_bind_packed_negation(const uint8 *matrix)
{
    uint32 negated = 0u - xport_load_le32(matrix + 8u);
    uint32 word1 = xport_load_le32(matrix + 4u), word0 = xport_load_le32(matrix);
    uint32 word3 = xport_load_le32(matrix + 12u), word4 = xport_load_le32(matrix + 16u);
    uint32 result = negated & 65535u;
    word1 = (word1 ^ 0xFFFF0000u) + 65536u;
    if (result) negated += 65536u;
    gte_ctc2(word0, 0u); gte_ctc2(word1, 1u); gte_ctc2(negated, 2u);
    gte_ctc2(word3, 3u); gte_ctc2(word4, 4u);
    return result;
}

uint32 gtmain_lighting_bind(uint32 context)
{
    uint32 first = r_u32(context + 116u), second = r_u32(context + 120u);
    gte_ctc2(first, 8u);
    first = r_u32(context + 124u); gte_ctc2(second, 9u);
    second = r_u32(context + 128u); gte_ctc2(first, 10u);
    first = r_u32(context + 132u); gte_ctc2(second, 11u); gte_ctc2(first, 12u);
    first = r_u32(context + 136u); second = r_u32(context + 140u); gte_ctc2(first, 16u);
    first = r_u32(context + 144u); gte_ctc2(second, 17u);
    second = r_u32(context + 148u); gte_ctc2(first, 18u);
    first = r_u32(context + 152u); gte_ctc2(second, 19u); gte_ctc2(first, 20u);
    return first;
}

uint32 gtmain_model_position(uint32 context, sint32 x, sint32 y, sint32 z)
{
    uint32 px, py, pz, base_x, base_y, base_z, result;
    w_u32(context + 944u, (uint32)y << 4);
    w_u32(context + 940u, (uint32)x << 4);
    w_u32(context + 948u, (uint32)z << 4);
    gtmain_model_rotate_vector((uint8 *)psx_addr(context + 940u, 12u),
        (const uint8 *)psx_addr(context + 884u, 20u), (const uint8 *)psx_addr(context + 940u, 12u));
    px = r_u32(context + 940u); py = r_u32(context + 944u); base_x = r_u32(context + 864u);
    pz = r_u32(context + 948u); base_z = r_u32(context + 872u);
    w_u32(context + 940u, base_x + px);
    base_y = r_u32(context + 868u);
    w_u32(context + 948u, base_z + pz);
    result = base_y + py;
    w_u32(context + 944u, result);
    return result;
}

sint32 gtmain_resource_pair_value(uint32 key, uint32 pairs)
{
    uint32 index = 0u, count;
    if (!r_u8(pairs)) return 0;
    count = r_u8(pairs);
    do
    {
        uint32 entry = pairs + index * 2u;
        uint32 candidate = r_u8(entry + 1u);
        ++index;
        if (key == candidate) return (sint8)r_u8(entry + 2u);
    }
    while (index < count);
    return 0;
}

sint32 gtmain_car_count(void)
{
    return (sint16)r_u16(0x800C0BFEu);
}

void gtmain_course_car_filter(uint32 enabled)
{
    if (enabled) w_u32(0x80097484u, (uint32)gtmain_car_count());
    else w_u32(0x80097484u, 0u);
}
