#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static void textured_matrix(uint32 source, uint32 reg)
{
    uint32 a = r_u32(source), b = r_u32(source + 4u);
    gte_ctc2(a, reg); a = r_u32(source + 8u); gte_ctc2(b, reg + 1u);
    b = r_u32(source + 12u); gte_ctc2(a, reg + 2u);
    a = r_u32(source + 16u); gte_ctc2(b, reg + 3u); gte_ctc2(a, reg + 4u);
}

static void textured_link24(uint32 address, uint32 value)
{
    w_u8(address, value); w_u8(address + 1u, value >> 8); w_u8(address + 2u, value >> 16);
}

static void textured_vertex_packet(uint32 packet, uint32 vertex, uint32 offset)
{
    uint32 color = r_u32(vertex + 16u), screen = r_u32(vertex + 8u), uv;
    w_u32(packet + offset, color); uv = r_u32(vertex + 12u); w_u32(packet + offset + 4u, screen);
    uv >>= 7; w_u8(packet + offset + 8u, uv); w_u8(packet + offset + 9u, uv >> 16);
}

static uint32 textured_clip(uint32 context)

{
    const uint32 gouraud = 1;
    uint32 either = 0, every = 0xFFFFFFFFu, list = context + 204u, vertex = context + 324u;
    uint32 count, threshold, packet, plane, remaining, result = 0;
    xport_gte_write_data(0, r_u32(vertex)); xport_gte_write_data(1, r_u32(vertex + 4u)); xport_gte_execute(0x180001u);
    textured_matrix(context + 116u, 8); textured_matrix(context + 136u, 16);
    count = (uint32)(sint32)(sint16)r_u16(context + 184u); packet = r_u32(context + 172u); threshold = r_u16(context + 8u);
    w_u32(context + 192u, list); w_u32(context + 196u, context + 264u);
    w_u32(context + 200u, vertex + count * 32u); w_u32(context + 176u, packet); w_u32(context + 180u, packet);
    while (count)
    {
        uint32 x = xport_gte_read_data(9), y = xport_gte_read_data(10), z = xport_gte_read_data(11), bits, a, b;
        w_u32(vertex + 8u, xport_gte_read_data(14)); w_u32(list, vertex); list += 4u;
        xport_gte_execute(0x4BE012u); w_u16(vertex, x); w_u16(vertex + 2u, y); w_u16(vertex + 4u, z);
        xport_gte_write_data(0, r_u32(vertex)); xport_gte_write_data(1, r_u32(vertex + 4u));
        a = xport_gte_read_data(9); b = xport_gte_read_data(10); z -= threshold;
        xport_gte_execute(0x4C6012u); bits = z >> 31;
        w_u16(vertex + 20u, z); w_u16(vertex + 22u, a); if ((sint32)a < 0) bits |= 2u;
        w_u16(vertex + 24u, b); if ((sint32)b < 0) bits |= 4u;
        a = xport_gte_read_data(9); b = xport_gte_read_data(10);
        xport_gte_write_data(0, r_u32(vertex + 32u)); xport_gte_write_data(1, r_u32(vertex + 36u));
        --count; vertex += 32u; xport_gte_execute(0x180001u);
        w_u16(vertex - 6u, a); if ((sint32)a < 0) bits |= 8u;
        w_u16(vertex - 4u, b); if ((sint32)b < 0) bits |= 16u;
        w_u16(vertex - 2u, bits); either |= bits; every &= bits;
    }
    w_u16(context + 186u, either);
    if (every) return 0;
    gte_ctc2(1, 27); gte_ctc2(0, 28); gte_ctc2(0, 5); gte_ctc2(0, 6); gte_ctc2(0, 7);
    gte_ctc2(4096, 0); gte_ctc2(0, 1); gte_ctc2(4096, 2); gte_ctc2(0, 3); gte_ctc2(4096, 4);
    remaining = r_u16(context + 186u);
    for (plane = 0; plane < 5u && remaining; ++plane)
    {
        uint32 mask = 1u << plane, source, destination, destination_start;
        if (!(remaining & mask)) continue;
        remaining -= remaining & mask;
        source = r_u32(context + 192u); destination = r_u32(context + 196u);
        count = (uint32)(sint32)(sint16)r_u16(context + 184u); destination_start = destination;
        w_u32(context + 192u, destination); w_u32(context + 196u, source);
        w_u32(source + count * 4u, r_u32(source)); w_u16(context + 188u, 2);
        while (count)
        {
            uint32 inside = r_u32(source), outside = r_u32(source + 4u), first_flags = r_u16(inside + 30u) & mask, second_flags = r_u16(outside + 30u) & mask;
            if (!first_flags) { w_u32(destination, inside); destination += 4u; }
            --count;
            if (first_flags != second_flags)
            {
                uint32 budget = (uint32)(sint32)(sint16)r_u16(context + 188u) - 1u;
                if (first_flags) { uint32 swap = inside; inside = outside; outside = swap; }
                if (budget != 0xFFFFFFFFu)
                {
                    sint32 distance, other; uint32 a, b, new_vertex, fraction, x, y, z, bits;
                    w_u16(context + 188u, budget);
                    distance = (sint16)r_u16(inside + plane * 2u + 20u); other = (sint16)r_u16(outside + plane * 2u + 20u);
                    if (distance)
                    {
                        gte_ctc2((uint32)distance, 26); gte_ctc2((uint32)distance - (uint32)other, 4);
                        xport_gte_write_data(0, 0); xport_gte_write_data(1, 4096);
                        a = r_u32(inside); b = r_u32(outside); xport_gte_execute(0x180001u);
                        gte_ctc2(a, 8); gte_ctc2(b, 10); gte_ctc2((a & 0xFFFF0000u) | (b & 0xFFFFu), 9);
                        a = r_u32(inside + 4u); b = r_u32(outside + 4u); gte_ctc2(a, 11); gte_ctc2(b, 12);
                        a = r_u16(context + 32u); fraction = (uint32)((sint32)xport_gte_read_data(24) >> 4); gte_ctc2(a, 26);
                        xport_gte_write_data(2, (4096u - fraction) & 0xFFFFu); xport_gte_write_data(3, fraction);
                        new_vertex = r_u32(context + 200u); destination += 4u; xport_gte_execute(0x4AE012u);
                        w_u32(context + 200u, new_vertex + 32u);
                        a = r_u16(inside + 12u); b = r_u16(inside + 14u); gte_ctc2(a, 16);
                        a = r_u16(outside + 12u); gte_ctc2(b, 19); b = r_u16(outside + 14u); gte_ctc2(a, 17); gte_ctc2(b, 20);
                        x = xport_gte_read_data(9); y = xport_gte_read_data(10); z = xport_gte_read_data(11);
                        if (gouraud) xport_gte_write_data(8, fraction);
                        w_u16(new_vertex, x); if (!plane) z = r_u16(context + 8u);
                        xport_gte_execute(0x4CE012u);
                        w_u16(new_vertex + 2u, y); w_u16(new_vertex + 4u, z);
                        gte_ctc2(4096, 4); xport_gte_write_data(0, r_u32(new_vertex)); xport_gte_write_data(1, r_u32(new_vertex + 4u));
                        a = xport_gte_read_data(9); b = xport_gte_read_data(11); w_u16(new_vertex + 12u, a); w_u16(new_vertex + 14u, b);
                        a = r_u32(context + 136u); b = r_u32(context + 140u); xport_gte_execute(0x180001u);
                        textured_matrix(context + 116u, 8); gte_ctc2(a, 16); w_u32(new_vertex + 8u, xport_gte_read_data(14)); gte_ctc2(b, 17);
                        a = r_u32(context + 144u); b = r_u32(context + 148u); xport_gte_execute(0x4A6012u);
                        gte_ctc2(a, 18); a = r_u32(context + 152u); gte_ctc2(b, 19); gte_ctc2(a, 20);
                        a = xport_gte_read_data(9); b = xport_gte_read_data(10); destination_start = r_u32(context + 192u);
                        w_u32(destination - 4u, new_vertex); xport_gte_execute(0x4C6012u);
                        if (gouraud)
                        {
                            xport_gte_write_data(6, r_u32(outside + 16u));
                            x = r_u8(inside + 16u); y = r_u8(inside + 17u); gte_ctc2(x << 4, 21);
                            z = r_u8(inside + 18u); gte_ctc2(y << 4, 22); gte_ctc2(z << 4, 23);
                        }
                        x = xport_gte_read_data(9); y = xport_gte_read_data(10);
                        bits = (uint32)((sint32)a >> 31) & 2u;
                        if (gouraud) xport_gte_execute(0x780010u);
                        w_u16(new_vertex + 22u, a); w_u16(new_vertex + 24u, b);
                        if ((sint32)b < 0) bits |= 4u;
                        w_u16(new_vertex + 26u, x); if ((sint32)x < 0) bits |= 8u;
                        w_u16(new_vertex + 28u, y); if ((sint32)y < 0) bits |= 16u;
                        w_u16(new_vertex + 30u, bits);
                        if (gouraud) w_u32(new_vertex + 16u, xport_gte_read_data(22));
                    }
                }
            }
            source += 4u;
        }
        count = (uint32)((sint32)(destination - destination_start) >> 2); w_u16(context + 184u, count);
        if ((sint32)count < 3) goto restore;
    }
    {
        uint32 color = r_u8(context + 159u), clut, page, left, right, first, last, last_packet = 0, link, old_link;
        packet = r_u32(context + 172u); clut = r_u16(context + 164u); page = r_u16(context + 166u);
        count = (uint32)(sint32)(sint16)r_u16(context + 184u); color |= 8u; left = r_u32(context + 192u);
        right = left + count * 4u - 4u; first = r_u32(left); left += 4u; last = r_u32(right); right -= 4u;
        while (left < right)
        {
            uint32 next = packet + 52u;
            textured_link24(packet, next); w_u8(packet + 3u, 12);
            textured_vertex_packet(packet, first, 4u); textured_vertex_packet(packet, last, 28u);
            first = r_u32(left); left += 4u; last = r_u32(right); right -= 4u;
            textured_vertex_packet(packet, first, 16u); textured_vertex_packet(packet, last, 40u); last_packet = packet;
            w_u8(packet + 7u, color); w_u16(packet + 14u, clut); w_u16(packet + 26u, page); packet = next;
        }
        w_u32(context + 180u, packet);
        if (left == right)
        {
            uint32 middle;
            color &= 0xFFF7u;
            textured_vertex_packet(packet, first, 4u); textured_vertex_packet(packet, last, 28u);
            middle = r_u32(left); w_u32(packet, 0x09000000u); w_u8(packet + 7u, color);
            w_u16(packet + 14u, clut); w_u16(packet + 26u, page);
            textured_vertex_packet(packet, middle, 16u); last_packet = packet; w_u32(context + 180u, packet + 40u);
        }
        packet = r_u32(context + 172u); link = r_u32(context + 168u);
        old_link = r_u8(link) | (uint32)r_u8(link + 1u) << 8 | (uint32)r_u8(link + 2u) << 16;
        textured_link24(link, packet); textured_link24(last_packet, old_link); result = 1;
    }

restore:
    {
        uint32 a = r_u16(context + 34u), b = r_u32(context + 36u); gte_ctc2(a, 27); gte_ctc2(b, 28);
        a = r_u32(context + 12u); b = r_u32(context + 16u); gte_ctc2(a, 21); a = r_u32(context + 20u); gte_ctc2(b, 22); gte_ctc2(a, 23);
        a = r_u32(context + 84u); b = r_u32(context + 88u); gte_ctc2(a, 5); a = r_u32(context + 92u); gte_ctc2(b, 6); gte_ctc2(a, 7);
        textured_matrix(context + 96u, 0);
    }
    return result;
}

sint32 gtmain_polygon_clip_textured_gouraud(uint32 context)
{
    return (sint32)textured_clip(context);
}

static sint32 rectangle_divide(uint32 numerator, sint32 denominator, uint32 address)
{
    if (!denominator) gt_draft_failure("Original rectangle BREAK 7", address);
    if (numerator == 0x80000000u && denominator == -1) gt_draft_failure("Original rectangle BREAK 6", address);
    return (sint32)numerator / denominator;
}

uint32 gtmain_view_rectangle_prepare(uint32 view)
{
    uint32 a, b, values[9], result;
    sint32 left, right, top, bottom, denominator;
    a = (uint32)(sint32)(sint16)r_u16(view); b = (uint32)(sint32)(sint16)r_u16(view + 8u);
    denominator = (sint16)r_u16(view + 12u); left = rectangle_divide((a - b) << 12, denominator, 0x800720A4u);
    a = (uint32)(sint32)(sint16)r_u16(view + 4u); b = (uint32)(sint32)(sint16)r_u16(view + 8u);
    denominator = (sint16)r_u16(view + 12u); right = rectangle_divide((a - b) << 12, denominator, 0x800720E8u);
    a = (uint32)(sint32)(sint16)r_u16(view + 10u); b = (uint32)(sint32)(sint16)r_u16(view + 2u);
    denominator = (sint16)r_u16(view + 12u); top = rectangle_divide((a - b) << 12, denominator, 0x8007212Cu);
    a = (uint32)(sint32)(sint16)r_u16(view + 10u); b = (uint32)(sint32)(sint16)r_u16(view + 6u);
    denominator = (sint16)r_u16(view + 12u); bottom = rectangle_divide((a - b) << 12, denominator, 0x80072170u);
    values[0] = r_u16(view + 2u); values[1] = r_u16(view + 4u); values[2] = r_u16(view + 2u);
    values[3] = r_u16(view); values[4] = r_u16(view + 6u); values[5] = r_u16(view + 4u);
    values[6] = r_u16(view + 6u); values[7] = r_u16(view); values[8] = r_u16(view);
    w_u16(view + 46u, values[0]); w_u16(view + 48u, values[1]); w_u16(view + 50u, values[2]);
    w_u16(view + 52u, values[3]); w_u16(view + 54u, values[4]); w_u16(view + 56u, values[5]);
    w_u16(view + 58u, values[6]); w_u16(view + 44u, values[8]);
    w_u16(view + 36u, left); w_u16(view + 40u, right); w_u16(view + 38u, top); w_u16(view + 42u, bottom);
    w_u16(view + 60u, values[7]); result = r_u16(view + 2u); a = r_u16(view + 4u); b = r_u16(view + 6u);
    w_u16(view + 66u, result); w_u16(view + 68u, a); w_u16(view + 74u, b); return result;
}

sint32 gtmain_font_string_draw(uint32 font, uint32 ordering_link, sint32 x, sint32 y, uint32 text)
{
    uint32 result = gtmain_primitive_texture_page(font, ordering_link), packet = r_u32(0x800BFE4Cu);
    uint32 glyphs = r_u32(font), kern_table = r_u32(font + 4u), color = r_u32(font + 12u);
    uint32 glyph = (uint32)(sint32)(sint8)r_u8(text), clut = r_u16(font + 24u), link = result, xpos = (uint32)x;
    while (glyph)
    {
        uint32 metrics, layout, height, old_link, kern, next;
        glyph &= 255u; metrics = r_u32(glyphs + glyph * 8u); layout = r_u32(glyphs + glyph * 8u + 4u);
        height = metrics >> 24;
        if (height)
        {
            w_u16(packet + 10u, (uint32)y + ((layout >> 8) & 255u) - 4u);
            w_u16(packet + 16u, (metrics >> 16) & 255u); w_u32(packet, 0x04000000u); w_u32(packet + 4u, color);
            w_u16(packet + 8u, xpos); w_u16(packet + 18u, height); w_u16(packet + 12u, metrics); w_u16(packet + 14u, clut);
            old_link = r_u8(link) | (uint32)r_u8(link + 1u) << 8 | (uint32)r_u8(link + 2u) << 16;
            textured_link24(link, packet); textured_link24(packet, old_link); packet += 20u;
        }
        ++text; next = (uint32)(sint32)(sint8)r_u8(text); kern = r_u32(kern_table + glyph * 4u); glyph = next;
        kern = r_u32(kern + ((next >> 1) & 0x7Cu));
        result = ((layout >> 16) & 255u) - ((kern >> ((next & 7u) * 4u)) & 15u);
        xpos += 1u + result;
    }
    w_u32(0x800BFE4Cu, packet); return (sint32)result;
}

/* Native string storage with the original proportional glyph and kerning algorithm */
sint32 gtmain_font_string_draw_native(uint32 font, uint32 ordering_link, sint32 x, sint32 y, const char *text)
{
    uint32 result = gtmain_primitive_texture_page(font, ordering_link), packet = r_u32(0x800BFE4Cu);
    uint32 glyphs = r_u32(font), kern_table = r_u32(font + 4u), color = r_u32(font + 12u);
    uint32 glyph = (uint32)(sint32)(sint8)(uint8)*text, clut = r_u16(font + 24u), link = result, xpos = (uint32)x;
    while (glyph)
    {
        uint32 metrics, layout, height, old_link, kern, next;
        glyph &= 255u; metrics = r_u32(glyphs + glyph * 8u); layout = r_u32(glyphs + glyph * 8u + 4u);
        height = metrics >> 24;
        if (height)
        {
            w_u16(packet + 10u, (uint32)y + ((layout >> 8) & 255u) - 4u);
            w_u16(packet + 16u, (metrics >> 16) & 255u); w_u32(packet, 0x04000000u); w_u32(packet + 4u, color);
            w_u16(packet + 8u, xpos); w_u16(packet + 18u, height); w_u16(packet + 12u, metrics); w_u16(packet + 14u, clut);
            old_link = r_u8(link) | (uint32)r_u8(link + 1u) << 8 | (uint32)r_u8(link + 2u) << 16;
            textured_link24(link, packet); textured_link24(packet, old_link); packet += 20u;
        }
        ++text; next = (uint32)(sint32)(sint8)(uint8)*text; kern = r_u32(kern_table + glyph * 4u); glyph = next;
        kern = r_u32(kern + ((next >> 1) & 0x7Cu));
        result = ((layout >> 16) & 255u) - ((kern >> ((next & 7u) * 4u)) & 15u);
        xpos += 1u + result;
    }
    w_u32(0x800BFE4Cu, packet); return (sint32)result;
}
