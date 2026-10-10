#include "game_first2_registry.h"
#include "game_draft_registry.h"
/* Unverified draft adapted from existing phase polygon translation */
/* TODO Preserve byte lane bus semantics in an eventual unaligned access adapter */
uint32 gt_draft_8001A0E0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 positions[3][4], lengths[3], colors[3];
    uint32 config = r_u32(0x8008F220u);
    uint32 material_first = r_u32(config + 16u), material_second = r_u32(config + 20u);
    uint32 height = r_u16(a0 + 78u), coordinate = r_u16(a0 + 72u);
    w_u16(a0 + 784u, r_u16(a0 + 66u));
    uint8 *source = gtmain_render_source(a0);
    w_u16(a0 + 788u, height);
    uint8 *vertices = source + 36u;
    w_u16(a0 + 786u, coordinate);
    uint32 head = r_u32(a0 + 772u) + 4u;
    uint32 phase = gtmain_view_plane_intersections_native(source, psx_addr(a0 + 784u, 6u));
    uint32 opaque_first = (phase < 8u ? material_first : material_second) & 0xF7FFFFFFu;
    uint32 translucent_first = (phase < 8u ? material_first : material_second) | 0x08000000u;
    uint32 opaque_second = (phase < 8u ? material_second : material_first) & 0xF7FFFFFFu;
    uint32 translucent_second = (phase < 8u ? material_second : material_first) | 0x08000000u;
    uint32 result = phase << 2;
    uint32 packets = 0;
    if (phase < 16u) {
        result = r_u32(0x8008B854u + phase * 4u);
        uint32 variant = phase < 8u ? phase : 15u - phase;
        switch (variant)
        {
        case 0u:
            packets = 1u; lengths[0] = 4u; colors[0] = translucent_second;
            positions[0][0] = 8u; positions[0][1] = 12u; positions[0][2] = 16u; positions[0][3] = 20u;
            break;
        case 1u:
            packets = 3u; lengths[0] = 3u; lengths[1] = 3u; lengths[2] = 4u;
            colors[0] = opaque_first; colors[1] = opaque_second; colors[2] = translucent_second;
            positions[0][0] = 8u; positions[0][1] = 24u; positions[0][2] = 28u;
            positions[1][0] = 28u; positions[1][1] = 12u; positions[1][2] = 20u;
            positions[2][0] = 28u; positions[2][1] = 24u; positions[2][2] = 20u; positions[2][3] = 16u;
            break;
        case 2u:
            packets = 3u; lengths[0] = 3u; lengths[1] = 3u; lengths[2] = 4u;
            colors[0] = opaque_first; colors[1] = opaque_second; colors[2] = translucent_second;
            positions[0][0] = 28u; positions[0][1] = 12u; positions[0][2] = 32u;
            positions[1][0] = 28u; positions[1][1] = 8u; positions[1][2] = 16u;
            positions[2][0] = 28u; positions[2][1] = 32u; positions[2][2] = 16u; positions[2][3] = 20u;
            break;
        case 3u:
            packets = 2u; lengths[0] = 4u; lengths[1] = 4u;
            colors[0] = translucent_first; colors[1] = translucent_second;
            positions[0][0] = 24u; positions[0][1] = 8u; positions[0][2] = 32u; positions[0][3] = 12u;
            positions[1][0] = 24u; positions[1][1] = 16u; positions[1][2] = 32u; positions[1][3] = 20u;
            break;
        case 4u:
            packets = 3u; lengths[0] = 3u; lengths[1] = 3u; lengths[2] = 4u;
            colors[0] = opaque_first; colors[1] = opaque_second; colors[2] = translucent_second;
            positions[0][0] = 36u; positions[0][1] = 16u; positions[0][2] = 24u;
            positions[1][0] = 36u; positions[1][1] = 8u; positions[1][2] = 24u;
            positions[2][0] = 36u; positions[2][1] = 8u; positions[2][2] = 20u; positions[2][3] = 12u;
            break;
        case 5u:
            packets = 2u; lengths[0] = 4u; lengths[1] = 4u;
            colors[0] = translucent_first; colors[1] = translucent_second;
            positions[0][0] = 28u; positions[0][1] = 8u; positions[0][2] = 36u; positions[0][3] = 16u;
            positions[1][0] = 28u; positions[1][1] = 12u; positions[1][2] = 36u; positions[1][3] = 20u;
            break;
        case 7u:
            packets = 3u; lengths[0] = 3u; lengths[1] = 3u; lengths[2] = 4u;
            colors[0] = opaque_second; colors[1] = opaque_first; colors[2] = translucent_first;
            positions[0][0] = 36u; positions[0][1] = 32u; positions[0][2] = 20u;
            positions[1][0] = 36u; positions[1][1] = 32u; positions[1][2] = 12u;
            positions[2][0] = 36u; positions[2][1] = 12u; positions[2][2] = 16u; positions[2][3] = 8u;
            break;
        default:
            break;
        }

        if (packets) {
            uint32 current = r_u32(0x800BFE4Cu), next = 0;
            w_u32(0x800BFE4Cu, current + 8u + 4u * lengths[0]);
            for (uint32 packet = 0; packet < packets; ++packet) {
                uint32 tag = r_u8(head) | ((uint32)r_u8(head + 1u) << 8) | ((uint32)r_u8(head + 2u) << 16);
                w_u8(head, current); w_u8(head + 1u, current >> 8); w_u8(head + 2u, current >> 16);
                w_u8(current, tag); w_u8(current + 1u, tag >> 8); w_u8(current + 2u, tag >> 16);
                w_u8(current + 3u, lengths[packet] + 1u);
                result = xport_load_le32(vertices + positions[packet][0]);
                if (packet + 1u < packets) next = r_u32(0x800BFE4Cu);
                w_u32(current + 8u, result);
                for (uint32 vertex = 1; vertex + 1u < lengths[packet]; ++vertex) {
                    result = xport_load_le32(vertices + positions[packet][vertex]);
                    w_u32(current + 8u + vertex * 4u, result);
                }
                uint32 final_vertex = xport_load_le32(vertices + positions[packet][lengths[packet] - 1u]);
                if (packet + 1u < packets) {
                    result = next + 8u + 4u * lengths[packet + 1u];
                    w_u32(0x800BFE4Cu, result);
                    result = next << 8;
                } else result = final_vertex;
                w_u32(current + 4u, colors[packet]);
                w_u32(current + 4u + lengths[packet] * 4u, final_vertex);
                current = next;
            }
        }
    }
    return result;
}
