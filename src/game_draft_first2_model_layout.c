#include "game_first2_registry.h"

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

sint32 gtmain_render_context_init(uint32 context)
{
    sub_8007BD58(context, 0u, 40u);
    w_u16(context + 20u, 4096u); w_u16(context + 28u, 4096u); w_u16(context + 36u, 4096u);
    return 4096;
}

uint32 gtmain_model_sections_relocate(uint32 model)
{
    uint32 cursor = model + 80u;
    uint32 count0 = r_u16(model + 40u), count1 = r_u16(model + 42u), index;
    w_u32(model, cursor);
    cursor += count0 * 8u;
    w_u32(model + 4u, cursor);
    cursor += count1 * 8u;
    for (index = 0u; index < 8u; ++index)
    {
        uint32 count, stride;
        w_u32(model + index * 4u + 8u, cursor);
        count = r_u16(model + index * 2u + 44u);
        stride = r_u16(0x8008E254u + index * 2u);
        cursor += count * stride;
    }
    return cursor;
}

sint32 gtmain_model_texture_bits(uint32 model, uint32 page)
{
    uint32 index, bits = (page & 31u) << 16;
    for (index = 0u; index < 8u; ++index)
        if (index & 4u)
        {
            uint32 entry = r_u32(model + index * 4u + 8u);
            uint32 count = r_u16(model + index * 2u + 44u);
            uint32 stride = r_u16(0x8008E254u + index * 2u), item;
            for (item = 0u; item < count; ++item)
            {
                w_u32(entry + 20u, (r_u32(entry + 20u) & 0xFFE0FFFFu) | bits);
                entry += stride;
            }
        }
    return 0;
}

sint32 gtmain_model_distances_square(uint32 entries)
{
    uint32 count = r_u16(entries), index;
    for (index = 0u; index < count; ++index)
    {
        uint32 address = entries + index * 8u + 4u;
        sint32 value = (sint32)r_u32(address) >> 14;
        w_u32(address, (uint32)value * (uint32)value);
    }
    return 0;
}

uint32 gtmain_model_materials_relocate(uint32 materials)
{
    uint32 count = r_u16(materials), cursor = materials + 32u;
    w_u32(materials + 24u, cursor);
    cursor += count * 24u;
    count = r_u16(materials + 2u);
    w_u32(materials + 28u, cursor);
    return cursor + count * 32u;
}

uint32 gtmain_model_bundle_relocate(uint32 bundle, uint32 page)
{
    uint32 count = r_u16(bundle + 60u), index, cursor = bundle + count * 8u + 64u;
    uint32 result;
    for (index = 0u; index < count; ++index)
    {
        w_u32(bundle + index * 8u + 68u, cursor);
        cursor = gtmain_model_sections_relocate(cursor);
    }
    gtmain_model_distances_square(bundle + 60u);
    w_u32(bundle + 56u, cursor);
    result = gtmain_model_materials_relocate(cursor);
    for (index = 0u; index < count; ++index)
    {
        gtmain_model_texture_bits(r_u32(bundle + index * 8u + 68u), page);
        result = (index + 1u) * 8u;
    }
    return result;
}

uint32 gtmain_matrix_negate_axes(uint8 *out, const uint8 *matrix)
{
    uint32 word0, word3, word4, word2, word1, negated, result;
    if (matrix != out)
    {
        word0 = xport_load_le32(matrix); word3 = xport_load_le32(matrix + 12u);
        xport_store_le32(out, word0);
        word4 = xport_load_le32(matrix + 16u);
        xport_store_le32(out + 12u, word3); xport_store_le32(out + 16u, word4);
    }
    word2 = xport_load_le32(matrix + 8u); word1 = xport_load_le32(matrix + 4u);
    negated = 0u - word2;
    if (negated & 65535u) negated += 65536u;
    result = (word1 ^ 0xFFFF0000u) + 65536u;
    xport_store_le32(out + 8u, negated); xport_store_le32(out + 4u, result);
    return result;
}
