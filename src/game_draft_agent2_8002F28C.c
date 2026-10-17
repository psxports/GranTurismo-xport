#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

/* GTMAIN.EXE 8002F28C */
void gt_draft_8002F28C(uint32 context, uint32 objects)
{
    uint32 record = objects + 4u;
    sint32 count = (sint32)r_u32(objects);
    sint32 index;
    for (index = 0; index < count; ++index, record += 28u)
    {
        uint32 px = r_u32(record + 16u);
        uint32 py = r_u32(record + 20u);
        uint32 pz = r_u32(record + 24u);
        sint16 yaw, pitch, roll, variant, radius;
        uint32 x, y, z, camera_x, camera_y, camera_z, table, ranges, model;
        uint32 scale, magnitude, bits, result, sprite_records, word0, word1;
        const uint8 *source;
        sint32 selected, scale_bits, exponent, minimum, difference, shift, scale_shift;
        uint16 x0, y0, z0, x1, y1, z1;
        sint64 product;
        w_u32(context + 872u, px);
        w_u32(context + 876u, py);
        w_u32(context + 880u, pz);
        yaw = (sint16)r_u16(record + 2u);
        pitch = (sint16)r_u16(record);
        roll = (sint16)r_u16(record + 4u);
        gtmain_model_rotation(psx_addr(context + 892u, 18u), yaw, pitch, roll);
        gtmain_matrix_scale_axes(psx_addr(context + 892u, 20u),
            psx_addr(context + 892u, 20u), psx_addr(record + 8u, 8u));
        px = r_u32(context + 872u);
        py = r_u32(context + 876u);
        camera_x = r_u32(context + 44u);
        camera_y = r_u32(context + 48u);
        x = px - camera_x;
        y = py - camera_y;
        table = r_u32(0x800BC1B0u);
        variant = (sint16)r_u16(record + 6u);
        radius = (sint16)r_u16(record + 14u);
        source = gtmain_render_source(context);
        table = r_u32(table + 20u);
        ranges = r_u32(table + (uint32)(sint32)variant * 4u + 4u);
        pz = r_u32(context + 880u);
        camera_z = r_u32(context + 52u);
        w_u32(context + 248u, x);
        w_u32(context + 252u, y);
        z = pz - camera_z;
        w_u32(context + 256u, z);
        word0 = xport_load_le32(source + 28u);
        scale = (uint32)(sint32)radius << 4;
        product = (sint64)(sint32)scale * (sint64)(sint32)word0;
        selected = gtmain_model_range_index(ranges, psx_addr(context + 248u, 12u),
            (sint32)(uint32)((uint64)product >> 12));
        if (selected == -1) continue;
        model = r_u32(ranges + (uint32)selected * 8u + 8u);
        xport_gte_write_data(30u, scale);
        scale_bits = (sint32)(31u - xport_gte_read_data(31u));
        magnitude = ((sint32)x < 0 ? 0u - x : x) |
            ((sint32)y < 0 ? 0u - y : y) |
            ((sint32)z < 0 ? 0u - z : z);
        xport_gte_write_data(30u, magnitude);
        bits = xport_gte_read_data(31u);
        minimum = (sint16)r_u16(model + 84u) + scale_bits - 16;
        exponent = (sint32)(31u - bits);
        if (exponent < minimum) exponent = minimum;
        difference = exponent - minimum;
        shift = exponent - 12;
        if (shift < 0) shift = 0;
        x = (uint32)((sint32)x >> ((uint32)shift & 31u));
        y = (uint32)((sint32)y >> ((uint32)shift & 31u));
        z = (uint32)((sint32)z >> ((uint32)shift & 31u));
        scale_shift = scale_bits - 12;
        w_u16(context + 40u, shift);
        w_u16(context + 42u, difference);
        w_u16(context + 260u, x);
        w_u16(context + 262u, y);
        w_u16(context + 264u, z);
        w_u16(context + 864u, 20 - minimum - difference);
        if (scale_shift > 0) scale = (uint32)((sint32)scale >> ((uint32)scale_shift & 31u));
        else if (scale_shift < 0) scale <<= (0u - (uint32)scale_shift) & 31u;
        gtmain_matrix_bind_packed_negation(psx_addr(context + 64u, 20u));
        xport_gte_write_data(0u, r_u32(context + 260u));
        xport_gte_write_data(1u, r_u32(context + 264u));
        xport_gte_execute(0x486012u);
        w_u32(context + 84u, xport_gte_read_data(9u));
        w_u32(context + 88u, xport_gte_read_data(10u));
        w_u32(context + 92u, xport_gte_read_data(11u));
        word0 = r_u32(context + 84u);
        word1 = r_u32(context + 88u);
        gte_ctc2(word0, 5u);
        word0 = r_u32(context + 92u);
        gte_ctc2(word1, 6u);
        gte_ctc2(word0, 7u);
        gtmain_matrix_multiply(psx_addr(context + 96u, 20u),
            psx_addr(context + 64u, 20u), psx_addr(context + 892u, 20u));
        gtmain_matrix_scale_gte(psx_addr(context + 96u, 20u), psx_addr(context + 96u, 20u), (uint16)scale);
        gtmain_matrix_shift(psx_addr(context + 96u, 18u), psx_addr(context + 96u, 18u), difference);
        gtmain_model_matrix_bind(context, psx_addr(context + 96u, 20u));
        gtmain_lighting_bind(context);
        x0 = r_u16(model + 68u);
        y0 = r_u16(model + 70u);
        z0 = r_u16(model + 72u);
        x1 = r_u16(model + 76u);
        y1 = r_u16(model + 78u);
        z1 = r_u16(model + 80u);
        w_u16(context + 184u, x0); w_u16(context + 186u, y0); w_u16(context + 188u, z0);
        w_u16(context + 192u, x1); w_u16(context + 194u, y0); w_u16(context + 196u, z0);
        w_u16(context + 200u, x0); w_u16(context + 202u, y1); w_u16(context + 204u, z0);
        w_u16(context + 208u, x1); w_u16(context + 210u, y1); w_u16(context + 212u, z0);
        w_u16(context + 216u, x0); w_u16(context + 218u, y0); w_u16(context + 220u, z1);
        w_u16(context + 224u, x1); w_u16(context + 226u, y0); w_u16(context + 228u, z1);
        w_u16(context + 232u, x0); w_u16(context + 234u, y1); w_u16(context + 236u, z1);
        w_u16(context + 240u, x1); w_u16(context + 242u, y1); w_u16(context + 244u, z1);
        bits = gtmain_model_bounds_classify(context, psx_addr(context + 184u, 76u), 8u);
        if (bits & 31u) continue;
        gtmain_matrix_transpose(psx_addr(context + 268u, 18u), psx_addr(context + 892u, 18u));
        gtmain_matrix_scale_gte(psx_addr(context + 268u, 20u), psx_addr(context + 268u, 20u), (uint16)(0u - scale));
        gtmain_matrix_rotate_short(psx_addr(context + 56u, 8u),
            psx_addr(context + 268u, 20u), psx_addr(context + 260u, 8u));
        word0 = r_u32(context + 96u);
        word1 = r_u32(context + 100u);
        gte_ctc2(word0, 0u);
        word0 = r_u32(context + 104u);
        gte_ctc2(word1, 1u);
        word1 = r_u32(context + 108u);
        gte_ctc2(word0, 2u);
        word0 = r_u32(context + 112u);
        gte_ctc2(word1, 3u);
        gte_ctc2(word0, 4u);
        w_u32(context + 912u, model);
        w_u32(context + 916u, r_u32(model));
        w_u32(context + 932u, r_u32(model + 4u));
        if ((bits >> 16) & 63u) result = gtmain_model_depth_cued_clipped_draw(context, model);
        else result = gtmain_model_depth_cued_draw(context, model);
        sprite_records = r_u32(context + 932u);
        word0 = r_u16(result + 64u);
        sprite_records = gtmain_model_sprites_draw(context, sprite_records, word0);
        word0 = r_u16(result + 66u);
        gtmain_model_radial_effect_draw(context, sprite_records, word0);
    }
}
