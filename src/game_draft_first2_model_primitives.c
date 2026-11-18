#include "game_first2_registry.h"
#include "psx_gte.h"

static void primitive_link24(uint32 address, uint32 value)
{
    w_u8(address, value); w_u8(address + 1u, value >> 8); w_u8(address + 2u, value >> 16);
}

static uint32 primitive_tag24(uint32 address)
{
    return r_u32(address) & 0xFFFFFFu;
}

static void primitive_link(uint32 ordering, uint32 packet)
{
    uint32 previous = primitive_tag24(ordering);
    primitive_link24(ordering, packet); primitive_link24(packet, previous);
}

static void primitive_vector(uint32 reg, uint32 vertex)
{
    uint32 xy = r_u32(vertex), z = r_u32(vertex + 4u);
    xport_gte_write_data(reg, xy); xport_gte_write_data(reg + 1u, z);
}

static void primitive_screen(uint32 packet, uint32 offset, uint32 reg)
{
    w_u32(packet + offset, xport_gte_read_data(reg));
}

static uint32 primitive_uv(uint32 vertex, uint32 previous)
{
    return (previous & 0xFFFF0000u) | (r_u32(vertex) >> 16);
}

static uint32 primitive_uv_header(uint32 context)
{
    /* LWL at context+165 places its two low bytes in the upper half */
    return r_u32(context + 164u) << 16;
}

static uint32 primitive_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static uint32 primitive_overlay(uint32 context, uint32 packet, uint32 ordering,
                                uint32 second, uint32 third, uint32 quad)
{
    uint32 table, a, b, command, clut;
    if (quad)
    {
        a = r_u32(context + 332u);
        primitive_screen(packet, 32u, 12u); primitive_screen(packet, 8u, 13u);
        primitive_screen(packet, 24u, 14u); w_u32(packet + 16u, a);
    }
    else
    {
        primitive_screen(packet, 24u, 12u); primitive_screen(packet, 16u, 13u);
        primitive_screen(packet, 8u, 14u);
    }
    table = r_u32(context + 944u);
    primitive_vector(0u, table + ((second >> 6) & 0x1FF8u));
    a = table + ((second >> 16) & 0x1FF8u);
    xport_gte_execute(0x4A2012u); command = r_u32(context + (quad ? 972u : 968u));
    primitive_vector(2u, a);
    a = xport_gte_read_data(9); b = xport_gte_read_data(10);
    w_u8(packet + 12u, a); w_u8(packet + 13u, b);
    xport_gte_execute(0x4AA012u);
    primitive_vector(4u, table + (third & 1023u) * 8u);
    a = xport_gte_read_data(9); b = xport_gte_read_data(10);
    w_u8(packet + 20u, a); w_u8(packet + 21u, b);
    xport_gte_execute(0x4B2012u);
    if (quad)
    {
        primitive_vector(0u, table + ((third >> 7) & 0x1FF8u));
        a = xport_gte_read_data(9); b = xport_gte_read_data(10);
        w_u8(packet + 36u, a); w_u8(packet + 37u, b);
        xport_gte_execute(0x4A2012u);
    }
    clut = r_u32(context + 916u); w_u32(packet, quad ? 0x09000000u : 0x07000000u);
    w_u16(packet + 14u, clut); w_u32(packet + 4u, command); w_u16(packet + 22u, clut >> 16);
    a = xport_gte_read_data(9); b = xport_gte_read_data(10);
    w_u8(packet + 28u, a); w_u8(packet + 29u, b);
    primitive_link(ordering, packet);
    return packet + (quad ? 40u : 32u);
}

static void primitive_save_vectors(uint32 context, uint32 quad)
{
    uint32 base = quad ? 356u : 340u;
    w_u32(context + base, xport_gte_read_data(0));
    w_u32(context + base + 4u, xport_gte_read_data(1));
    w_u32(context + 340u - (quad ? 0u : 16u), xport_gte_read_data(2));
    w_u32(context + 344u - (quad ? 0u : 16u), xport_gte_read_data(3));
    w_u32(context + 308u, xport_gte_read_data(4));
    w_u32(context + 312u, xport_gte_read_data(5));
}

static void primitive_tri_midpoints(uint32 list, uint32 pool)
{
    uint32 a = r_u32(list), b = r_u32(list + 4u), c = r_u32(list + 8u), offset;
    for (offset = 4u; offset <= 8u; offset += 2u)
    {
        sint32 av = (sint16)r_u16(a + offset), bv = (sint16)r_u16(b + offset), cv = (sint16)r_u16(c + offset);
        w_u16(pool + offset, (uint32)((av + bv) >> 1));
        w_u16(pool + 16u + offset, (uint32)((bv + cv) >> 1));
        w_u16(pool + 32u + offset, (uint32)((cv + av) >> 1));
    }
    primitive_vector(0u, pool + 4u); primitive_vector(2u, pool + 20u); primitive_vector(4u, pool + 36u);
    {
        uint32 av = r_u8(a + 2u), bv = r_u8(b + 2u), cv;
        xport_gte_execute(0x280030u); cv = r_u8(c + 2u);
        w_u8(pool + 2u, (av + bv) >> 1); w_u8(pool + 18u, (bv + cv) >> 1); w_u8(pool + 34u, (cv + av) >> 1);
        av = r_u8(a + 3u); bv = r_u8(b + 3u); cv = r_u8(c + 3u);
        w_u8(pool + 3u, (av + bv) >> 1); w_u8(pool + 19u, (bv + cv) >> 1); w_u8(pool + 35u, (cv + av) >> 1);
    }
    primitive_screen(pool, 12u, 12u); primitive_screen(pool, 28u, 13u); primitive_screen(pool, 44u, 14u);
}

static void primitive_quad_midpoints(uint32 list, uint32 pool)
{
    uint32 b = r_u32(list + 4u), c = r_u32(list + 8u), a = r_u32(list), d = r_u32(list + 12u), offset;
    for (offset = 4u; offset <= 8u; offset += 2u)
    {
        sint32 bv = (sint16)r_u16(b + offset), cv = (sint16)r_u16(c + offset);
        sint32 av = (sint16)r_u16(a + offset), dv = (sint16)r_u16(d + offset);
        w_u16(pool + 32u + offset, (uint32)((bv + cv) >> 1));
        w_u16(pool + 64u + offset, (uint32)((dv + av) >> 1));
        w_u16(pool + 16u + offset, (uint32)((av + bv) >> 1));
        w_u16(pool + 48u + offset, (uint32)((cv + dv) >> 1));
        w_u16(pool + offset, (uint32)((av + bv + cv + dv) >> 2));
    }
    primitive_vector(0u, pool + 4u); primitive_vector(2u, pool + 20u); primitive_vector(4u, pool + 36u);
    {
        uint32 av = r_u8(a + 2u), bv = r_u8(b + 2u), cv, dv;
        xport_gte_execute(0x280030u); cv = r_u8(c + 2u); dv = r_u8(d + 2u);
        w_u8(pool + 34u, (bv + cv) >> 1); w_u8(pool + 66u, (dv + av) >> 1);
        w_u8(pool + 18u, (av + bv) >> 1); w_u8(pool + 50u, (cv + dv) >> 1); w_u8(pool + 2u, (av + bv + cv + dv) >> 2);
    }
    primitive_screen(pool, 12u, 12u); primitive_screen(pool, 28u, 13u); primitive_screen(pool, 44u, 14u);
    primitive_vector(2u, pool + 52u); primitive_vector(4u, pool + 68u);
    {
        uint32 av = r_u8(a + 3u), bv = r_u8(b + 3u), cv, dv;
        xport_gte_execute(0x280030u); cv = r_u8(c + 3u); dv = r_u8(d + 3u);
        w_u8(pool + 35u, (bv + cv) >> 1); w_u8(pool + 67u, (dv + av) >> 1);
        w_u8(pool + 19u, (av + bv) >> 1); w_u8(pool + 51u, (cv + dv) >> 1); w_u8(pool + 3u, (av + bv + cv + dv) >> 2);
    }
    primitive_screen(pool, 60u, 13u); primitive_screen(pool, 76u, 14u);
}

static uint32 primitive_subdivide_triangle(uint32 context, uint32 packet, uint32 ordering,
                                          uint32 uv0, uint32 uv1, uint32 uv2, uint32 saved)
{
    uint32 list = context + 204u, pool = context + 352u, current, first, second, third;
    sint32 depth = 0, child;
    w_u16(context + 188u, 5u); w_u32(context + 168u, ordering); w_u32(context + 172u, packet);
    w_u32(context + 184u, 1152u);
    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2);
    if (!saved) primitive_save_vectors(context, 0u);
    w_u32(context + 156u, xport_gte_read_data(6)); w_u32(context + 164u, uv1);
    w_u16(context + 164u, uv0 >> 16);
    primitive_screen(context, 348u, 12u); primitive_screen(context, 332u, 13u); primitive_screen(context, 316u, 14u);
    current = r_u32(context + 172u);
    w_u32(context + 204u, context + 304u); w_u32(context + 208u, context + 320u); w_u32(context + 212u, context + 336u);
    first = r_u32(context + 156u);
    w_u32(context + 176u, current); w_u32(context + 180u, current); w_u32(context + 160u, first | 0x08000000u);
tri_midpoints:
    primitive_tri_midpoints(list, pool);
    current = r_u32(context + 180u);
    {
        uint32 side_color = r_u32(context + 156u), side_uv0 = primitive_uv_header(context), side_uv1 = r_u32(context + 164u);
    for (child = 2; child >= 0; --child)
    {
        sint32 previous = child - 1;
        uint32 color, uv, area;
        if (previous < 0) previous = 2;
        first = r_u32(list + (uint32)child * 4u); second = pool + (uint32)previous * 16u;
        third = r_u32(list + (uint32)previous * 4u);
        xport_gte_write_data(12u, r_u32(first + 12u)); xport_gte_write_data(13u, r_u32(second + 12u));
        xport_gte_write_data(14u, r_u32(third + 12u));
        color = side_color; w_u8(current + 3u, 7u);
        uv = primitive_uv(third, third); xport_gte_execute(0x1400006u);
        side_uv0 = primitive_uv(first, side_uv0); w_u32(current + 28u, uv);
        side_uv1 = primitive_uv(second, side_uv1);
        w_u32(current + 12u, side_uv0); w_u32(current + 20u, side_uv1);
        primitive_link24(current, current + 32u);
        area = xport_gte_read_data(24u);
        primitive_screen(current, 8u, 12u); primitive_screen(current, 16u, 13u);
        if (area)
        {
            primitive_screen(current, 24u, 14u); w_u32(current + 4u, color);
            w_u32(context + 176u, current); current += 32u;
        }
    }
    }
    w_u32(context + 180u, current); child = 3;
tri_children:
    while (child >= 0)
    {
        uint32 threshold, maximum, area, uv, color;
        sint32 previous = child - 1;
        if (previous < 0) previous = 2;
        if (child == 3) { first = pool; second = pool + 16u; third = pool + 32u; }
        else { first = r_u32(list + (uint32)child * 4u); second = pool + (uint32)child * 16u; third = pool + (uint32)previous * 16u; }
        --child;
        xport_gte_write_data(12u, r_u32(second + 12u)); xport_gte_write_data(13u, r_u32(first + 12u));
        xport_gte_write_data(14u, r_u32(third + 12u));
        maximum = r_u16(context + 188u); threshold = r_u32(context + 184u);
        xport_gte_execute(0x1400006u); current = r_u32(context + 180u);
        uv = primitive_uv_header(context); color = r_u32(context + 164u);
        uv = primitive_uv(second, uv); color = primitive_uv(first, color);
        w_u32(current + 12u, uv); w_u32(current + 20u, color);
        area = xport_gte_read_data(24u); primitive_screen(current, 8u, 12u); primitive_screen(current, 16u, 13u);
        area = primitive_abs(area); primitive_screen(current, 24u, 14u);
        if ((sint32)area >= (sint32)threshold && depth < (sint32)maximum)
        {
            uint32 resume = context + 196u + (uint32)depth;
            list += 12u; pool += 48u; ++depth; w_u8(resume, (uint32)child);
            w_u32(list, first); w_u32(list + 4u, second); w_u32(list + 8u, third);
            goto tri_midpoints;
        }
        uv = r_u32(context + 156u); color = primitive_uv(third, color);
        w_u32(current + 4u, uv); w_u32(current + 28u, color); w_u8(current + 3u, 7u);
        primitive_link24(current, current + 32u); w_u32(context + 176u, current); w_u32(context + 180u, current + 32u);
    }
    --depth; child = (sint8)r_u8(context + 196u + (uint32)depth); list -= 12u; pool -= 48u;
    if (depth >= 0) goto tri_children;
    first = r_u32(context + 172u); second = r_u32(context + 168u); current = r_u32(context + 176u);
    third = primitive_tag24(second); primitive_link24(second, first); primitive_link24(current, third);
    return first << 8;
}

static uint32 primitive_subdivide_quad(uint32 context, uint32 packet, uint32 ordering,
                                      uint32 uv0, uint32 uv1, uint32 uv2, uint32 saved)
{
    uint32 list = context + 204u, pool = context + 368u, current, first, second, third, fourth;
    sint32 depth = 0, child;
    w_u16(context + 188u, 5u); w_u32(context + 168u, ordering); w_u32(context + 172u, packet);
    w_u32(context + 184u, 2048u);
    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2); w_u32(context + 352u, uv2);
    if (!saved) primitive_save_vectors(context, 1u);
    w_u32(context + 156u, xport_gte_read_data(6)); w_u32(context + 164u, uv1); w_u16(context + 164u, uv0 >> 16);
    primitive_screen(context, 348u, 12u); primitive_screen(context, 316u, 13u); primitive_screen(context, 364u, 14u);
    current = r_u32(context + 172u); first = r_u32(context + 156u);
    w_u32(context + 204u, context + 304u); w_u32(context + 208u, context + 320u);
    w_u32(context + 212u, context + 336u); w_u32(context + 216u, context + 352u);
    w_u32(context + 176u, current); w_u32(context + 180u, current); w_u32(context + 160u, first & 0xF7FFFFFFu);
quad_midpoints:
    primitive_quad_midpoints(list, pool); current = r_u32(context + 180u);
    {
        uint32 side_list = list, midpoint = pool + 16u;
        uint32 side_uv0 = primitive_uv_header(context), side_uv1 = r_u32(context + 164u);
        for (child = 3; child >= 0; --child)
        {
            uint32 color, uv, area;
            first = r_u32(side_list); second = r_u32(child ? side_list + 4u : list);
            xport_gte_write_data(12u, r_u32(first + 12u)); xport_gte_write_data(13u, r_u32(second + 12u));
            xport_gte_write_data(14u, r_u32(midpoint + 12u));
            xport_gte_execute(0x1400006u); color = r_u32(context + 160u);
            side_uv0 = primitive_uv(first, side_uv0); w_u32(current + 4u, color);
            side_uv1 = primitive_uv(second, side_uv1); w_u32(current + 12u, side_uv0);
            uv = primitive_uv(midpoint, color); w_u32(current + 20u, side_uv1); w_u32(current + 28u, uv);
            area = xport_gte_read_data(24u); primitive_screen(current, 8u, 12u); primitive_screen(current, 16u, 13u);
            if (area)
            {
                primitive_screen(current, 24u, 14u); w_u8(current + 3u, 7u); primitive_link24(current, current + 32u);
                w_u32(context + 176u, current); current += 32u;
            }
            midpoint += 16u; side_list += 4u;
        }
    }
    w_u32(context + 180u, current); child = 3;
quad_children:
    while (child >= 0)
    {
        uint32 previous = (uint32)(child - 1) & 3u, uv0n, uv1n, color, area, area2, maximum, threshold;
        if (child & 1)
        {
            first = pool + 16u + previous * 16u; second = r_u32(list + (uint32)child * 4u);
            third = pool + 16u + (uint32)child * 16u; fourth = pool;
        }
        else
        {
            first = r_u32(list + (uint32)child * 4u); second = pool + 16u + (uint32)child * 16u;
            third = pool; fourth = pool + 16u + previous * 16u;
        }
        --child;
        xport_gte_write_data(12u, r_u32(second + 12u)); xport_gte_write_data(13u, r_u32(first + 12u));
        xport_gte_write_data(14u, r_u32(third + 12u));
        current = r_u32(context + 180u); color = r_u32(context + 156u); xport_gte_execute(0x1400006u);
        w_u32(current + 4u, color); uv0n = primitive_uv_header(context); uv1n = r_u32(context + 164u);
        uv0n = primitive_uv(second, uv0n); uv1n = primitive_uv(first, uv1n);
        w_u32(current + 12u, uv0n); w_u32(current + 20u, uv1n);
        uv0n = primitive_uv(third, uv0n); area = xport_gte_read_data(24u);
        xport_gte_write_data(12u, r_u32(fourth + 12u)); uv1n = primitive_uv(fourth, uv1n);
        w_u32(current + 28u, uv0n); color = r_u32(second + 12u); xport_gte_execute(0x1400006u);
        w_u32(current + 36u, uv1n); w_u32(current + 8u, color); w_u8(current + 3u, 9u);
        primitive_link24(current, current + 40u); maximum = r_u16(context + 188u);
        area2 = xport_gte_read_data(24u); threshold = r_u32(context + 184u);
        area = primitive_abs(area - area2); primitive_screen(current, 16u, 13u);
        if ((sint32)area >= (sint32)threshold && depth < (sint32)maximum)
        {
            uint32 resume = context + 196u + (uint32)depth;
            list += 16u; pool += 80u; ++depth; w_u8(resume, (uint32)child);
            w_u32(list, first); w_u32(list + 4u, second); w_u32(list + 8u, third); w_u32(list + 12u, fourth);
            goto quad_midpoints;
        }
        primitive_screen(current, 24u, 14u); primitive_screen(current, 32u, 12u);
        w_u32(context + 176u, current); w_u32(context + 180u, current + 40u);
    }
    --depth; child = (sint8)r_u8(context + 196u + (uint32)depth); list -= 16u; pool -= 80u;
    if (depth >= 0) goto quad_children;
    first = r_u32(context + 172u); second = r_u32(context + 168u); current = r_u32(context + 176u);
    third = primitive_tag24(second); primitive_link24(second, first); primitive_link24(current, third);
    return first << 8;
}

/* Original: GTMAIN.EXE:sub_80072548. */ uint32 gtmain_model_primitives_draw(uint32 model, uint32 context)
{
    static const uint8 strides[8] = { 16, 16, 24, 28, 28, 28, 36, 40 };
    uint32 result = r_u32(model + 8u), group;
    w_u32(context + 992u, result);
    for (group = 0u; group < 8u; ++group)
    {
        uint32 remaining = r_u16(model + 44u + group * 2u), records = r_u32(context + 992u);
        uint32 packet, vertices, ordering_base, shift, winding, overlay_mask, packed, va, vb, vc;
        uint32 quad = group & 1u;
        if (!remaining) continue;
        packed = r_u32(records); packet = r_u32(context + 932u);
        vertices = r_u32(context + 940u);
        if (!quad)
        {
            ordering_base = r_u32(context + 928u); shift = r_u16(context + 40u);
            winding = (uint32)(sint32)(sint16)r_u16(context + 908u); overlay_mask = r_u32(context + 912u);
        }
        else
        {
            ordering_base = 0u; shift = 0u;
            winding = (uint32)(sint32)(sint16)r_u16(context + 908u); overlay_mask = 0u;
        }
        va = vertices + (packed & 511u) * 8u; vb = vertices + ((packed >> 6) & 0xFF8u);
        result = (packed >> 15) & 0xFF8u; vc = vertices + result;
        do
        {
            uint32 flags = packed, second, third, depth, depth1, depth2, depth3 = 0u;
            uint32 area, area2 = 0u, ordering, overlay, magnitude, uv0, uv1, uv2;
            uint32 next_vertices = vertices, next_a, next_b, next_c;
            if (quad)
            {
                primitive_vector(0u, vb); primitive_vector(2u, vc);
                if (group == 5u)
                {
                    w_u32(context + 324u, xport_gte_read_data(0)); w_u32(context + 328u, xport_gte_read_data(1));
                }
                primitive_vector(4u, va); next_vertices = r_u32(context + 940u);
                xport_gte_execute(0x280030u); second = r_u32(records + 4u);
                packed = r_u32(records + strides[group]);
                uv0 = next_vertices + (second & 511u) * 8u;
                uv1 = r_u32(uv0); uv2 = r_u32(uv0 + 4u);
                xport_gte_execute(0x1400006u);
                next_a = next_vertices + (packed & 511u) * 8u;
                next_b = next_vertices + ((packed >> 6) & 0xFF8u);
                xport_gte_write_data(0u, uv1); xport_gte_write_data(1u, uv2);
                area = xport_gte_read_data(24u); primitive_screen(context, 332u, 12u);
                depth = xport_gte_read_data(17u); depth1 = xport_gte_read_data(18u); depth2 = xport_gte_read_data(19u);
                ordering_base = r_u32(context + 928u); shift = r_u16(context + 40u);
                xport_gte_execute(0x180001u); third = r_u32(records + 8u);
                next_c = next_vertices + ((packed >> 15) & 0xFF8u);
                xport_gte_write_data(6u, r_u32(records + 12u)); depth3 = xport_gte_read_data(19u);
                overlay_mask = r_u32(context + 912u); xport_gte_execute(0x1400006u); --remaining;
                if (((sint32)depth < (sint32)depth1) == (second >> 31)) depth = depth1;
                if (((sint32)depth < (sint32)depth2) == (second >> 31)) depth = depth2;
                if (((sint32)depth < (sint32)depth3) == (second >> 31)) depth = depth3;
                area = 0u - area; area2 = xport_gte_read_data(24u);
                if (winding) { area = 0u - area; area2 = 0u - area2; }
                result = group == 5u ? area2 : area & area2 & flags;
                if ((sint32)(area & area2 & flags) < 0) goto primitive_next;
                depth = (uint32)((sint32)(depth << (shift & 31u)) >> 13);
                if ((sint32)depth >= 4096) depth = 4095u;
                result = depth * 4u; ordering = ordering_base + result;
                overlay = flags & overlay_mask; magnitude = primitive_abs(area) + primitive_abs(area2);
            }
            else
            {
                primitive_vector(0u, vc); primitive_vector(2u, vb); primitive_vector(4u, va); --remaining;
                xport_gte_execute(0x280030u); packed = r_u32(records + strides[group]);
                second = r_u32(records + 4u); third = r_u32(records + 8u);
                next_a = vertices + (packed & 511u) * 8u; next_b = vertices + ((packed >> 6) & 0xFF8u);
                xport_gte_execute(0x1400006u); next_c = vertices + ((packed >> 15) & 0xFF8u);
                xport_gte_write_data(6u, r_u32(records + 12u)); area = xport_gte_read_data(24u);
                depth = xport_gte_read_data(17u); depth1 = xport_gte_read_data(18u); depth2 = xport_gte_read_data(19u);
                if (winding) area = 0u - area;
                if (((sint32)depth < (sint32)depth1) == (second >> 31)) depth = depth1;
                result = depth;
                if ((sint32)(area & flags) < 0) goto primitive_next;
                if (((sint32)depth < (sint32)depth2) == (second >> 31)) depth = depth2;
                depth = (uint32)((sint32)(depth << (shift & 31u)) >> 13);
                if ((sint32)depth >= 4096) depth = 4095u;
                result = depth * 4u; ordering = ordering_base + result;
                overlay = flags & overlay_mask; magnitude = primitive_abs(area);
            }
            if (overlay)
            {
                if (group == 4u || group == 5u) primitive_save_vectors(context, quad);
                packet = primitive_overlay(context, packet, ordering, second, third, quad);
            }
            switch (group)
            {
            case 0u:
                w_u32(packet, 0x04000000u); result = packet << 8; primitive_link(ordering, packet);
                primitive_screen(packet, 16u, 12u); primitive_screen(packet, 12u, 13u); primitive_screen(packet, 8u, 14u);
                primitive_screen(packet, 4u, 6u); packet += 20u;
                break;
            case 1u:
                w_u32(packet, 0x05000000u); primitive_link(ordering, packet);
                result = r_u32(context + 332u);
                primitive_screen(packet, 20u, 12u); primitive_screen(packet, 8u, 13u); primitive_screen(packet, 16u, 14u);
                w_u32(packet + 12u, result); primitive_screen(packet, 4u, 6u); packet += 24u;
                break;
            case 2u:
                xport_gte_write_data(20u, r_u32(records + 16u)); xport_gte_write_data(21u, r_u32(records + 20u));
                w_u32(packet, 0x06000000u); result = packet << 8; primitive_link(ordering, packet);
                primitive_screen(packet, 24u, 12u); primitive_screen(packet, 16u, 13u); primitive_screen(packet, 8u, 14u);
                primitive_screen(packet, 4u, 6u); primitive_screen(packet, 12u, 20u); primitive_screen(packet, 20u, 21u); packet += 28u;
                break;
            case 3u:
                xport_gte_write_data(20u, r_u32(records + 16u)); xport_gte_write_data(21u, r_u32(records + 20u));
                xport_gte_write_data(22u, r_u32(records + 24u)); w_u32(packet, 0x08000000u); primitive_link(ordering, packet);
                result = r_u32(context + 332u);
                primitive_screen(packet, 32u, 12u); primitive_screen(packet, 8u, 13u); primitive_screen(packet, 24u, 14u);
                w_u32(packet + 16u, result); primitive_screen(packet, 4u, 6u); primitive_screen(packet, 12u, 20u);
                primitive_screen(packet, 28u, 21u); primitive_screen(packet, 20u, 22u); packet += 36u;
                break;
            case 4u:
                uv0 = r_u32(records + 16u); result = r_u32(context + 904u);
                uv1 = r_u32(records + 20u); uv2 = r_u32(records + 24u); uv0 += result; result = packet << 8;
                if ((sint32)magnitude < 800)
                {
                    w_u32(packet, 0x07000000u); primitive_link(ordering, packet);
                    w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1); w_u32(packet + 28u, uv2);
                    primitive_screen(packet, 24u, 12u); primitive_screen(packet, 16u, 13u); primitive_screen(packet, 8u, 14u);
                    primitive_screen(packet, 4u, 6u); packet += 32u;
                }
                else
                {
                    result = primitive_subdivide_triangle(context, packet, ordering, uv0, uv1, uv2, overlay);
                    packet = r_u32(context + 180u); ordering_base = r_u32(context + 928u); vertices = r_u32(context + 940u);
                    shift = r_u16(context + 40u); winding = (uint32)(sint32)(sint16)r_u16(context + 908u); overlay_mask = r_u32(context + 912u);
                }
                break;
            case 5u:
                uv0 = r_u32(records + 16u); result = r_u32(context + 904u); uv2 = r_u32(records + 24u);
                uv0 += result; uv1 = r_u32(records + 20u);
                if ((sint32)magnitude < 1568)
                {
                    w_u32(packet, 0x09000000u); primitive_link(ordering, packet);
                    w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1); w_u32(packet + 36u, uv2);
                    w_u16(packet + 28u, uv2 >> 16); result = r_u32(context + 332u);
                    primitive_screen(packet, 32u, 12u); primitive_screen(packet, 8u, 13u); primitive_screen(packet, 24u, 14u);
                    w_u32(packet + 16u, result); primitive_screen(packet, 4u, 6u); packet += 40u;
                }
                else
                {
                    result = primitive_subdivide_quad(context, packet, ordering, uv0, uv1, uv2, overlay);
                    packet = r_u32(context + 180u);
                }
                break;
            case 6u:
                xport_gte_write_data(20u, r_u32(records + 28u)); xport_gte_write_data(21u, r_u32(records + 32u));
                uv0 = r_u32(records + 16u); result = r_u32(context + 904u);
                uv1 = r_u32(records + 20u); uv2 = r_u32(records + 24u); w_u32(packet, 0x09000000u);
                uv0 += result; result = packet << 8; primitive_link(ordering, packet);
                w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 36u, uv2);
                primitive_screen(packet, 32u, 12u); primitive_screen(packet, 20u, 13u); primitive_screen(packet, 8u, 14u);
                primitive_screen(packet, 4u, 6u); primitive_screen(packet, 16u, 20u); primitive_screen(packet, 28u, 21u); packet += 40u;
                break;
            case 7u:
                xport_gte_write_data(20u, r_u32(records + 28u)); xport_gte_write_data(21u, r_u32(records + 32u));
                xport_gte_write_data(22u, r_u32(records + 36u)); uv0 = r_u32(records + 16u);
                result = r_u32(context + 904u); uv1 = r_u32(records + 20u); uv2 = r_u32(records + 24u);
                w_u32(packet, 0x0C000000u); uv0 += result; primitive_link(ordering, packet);
                w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 48u, uv2);
                w_u16(packet + 36u, uv2 >> 16); result = r_u32(context + 332u);
                primitive_screen(packet, 44u, 12u); primitive_screen(packet, 8u, 13u); primitive_screen(packet, 32u, 14u);
                w_u32(packet + 20u, result); primitive_screen(packet, 4u, 6u); primitive_screen(packet, 16u, 20u);
                primitive_screen(packet, 40u, 21u); primitive_screen(packet, 28u, 22u); packet += 52u;
                break;
            }
primitive_next:
            records += strides[group]; va = next_a; vb = next_b; vc = next_c;
        }
        while (remaining);
        if (group != 7u) model = r_u32(context + 936u);
        w_u32(context + 992u, records); w_u32(context + 932u, packet);
    }
    return result;
}
