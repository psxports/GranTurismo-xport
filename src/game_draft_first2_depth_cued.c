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
    return (previous & 0xFFFF0000u) | r_u16(vertex + 2u);
}

static uint32 primitive_uv_header(uint32 context)
{
    /* LWL at context+165 places its two low bytes in the upper half */
    return (uint32)r_u16(context + 164u) << 16;
}

static uint32 primitive_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
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
                                          uint32 uv0, uint32 uv1, uint32 uv2,
                                          uint32 remaining, uint32 next_record)
{
    uint32 list = context + 204u, pool = context + 352u, current, first, second, third;
    sint32 depth = 0, child;
    w_u16(context + 188u, 5u);
    w_u32(context + 960u, remaining); w_u32(context + 952u, next_record);
    w_u32(context + 168u, ordering); w_u32(context + 172u, packet);
    w_u32(context + 184u, 1152u);
    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2);
    w_u32(context + 164u, uv1); w_u16(context + 164u, uv0 >> 16);
    w_u32(context + 156u, xport_gte_read_data(22u));
    primitive_save_vectors(context, 0u);
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

static void depth_quad_workspace(uint32 context, uint32 packet, uint32 ordering,
                                 uint32 uv0, uint32 uv1, uint32 uv2, uint32 next_record)
{
    w_u16(context + 188u, 5u); w_u32(context + 952u, next_record);
    w_u32(context + 168u, ordering); w_u32(context + 172u, packet);
    w_u32(context + 184u, 2048u);
    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2);
    w_u32(context + 352u, uv2);
    w_u32(context + 156u, xport_gte_read_data(22u));
    w_u32(context + 164u, uv1); w_u16(context + 164u, uv0 >> 16);
    primitive_save_vectors(context, 1u);
    primitive_screen(context, 348u, 12u); primitive_screen(context, 316u, 13u); primitive_screen(context, 364u, 14u);
}

static uint32 primitive_subdivide_quad(uint32 context)
{
    uint32 list = context + 204u, pool = context + 368u, current, first, second, third, fourth;
    sint32 depth = 0, child;
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
            second = r_u32(side_list + 4u); first = r_u32(side_list);
            if (!child) second = r_u32(list);
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

static uint32 depth_choose(uint32 a, uint32 b, uint32 selector)
{
    return (((sint32)a < (sint32)b) == selector) ? b : a;
}

static uint32 depth_bucket(uint32 depth, uint32 shift)
{
    sint32 value = (sint32)(depth << (shift & 31u)) >> 13;
    return value >= 4096 ? 4095u : (uint32)value;
}

static void depth_quad_six(uint32 context, uint32 packet, uint32 ordering)
{
    uint32 tagged = (packet & 0xFFFFFFu) | 0x09000000u;
    uint32 color = r_u32(context + 156u), clut = r_u16(context + 164u), page = r_u16(context + 166u);
    uint32 a, b, c, d, center, ab, bc, cd, da, previous;
    sint32 xa, xb, xc, xd, ya, yb, yc, yd, za, zb, zc, zd;
    sint32 xcenter, xab, xbc, xcd, xda, ycenter, yab, ybc, ycd, yda;
    w_u32(packet + 0u, tagged + 40u);
    w_u32(packet + 40u, tagged + 80u);
    w_u32(packet + 80u, tagged + 120u);
    w_u32(packet + 120u, tagged + 160u);
    w_u32(packet + 160u, tagged + 200u);
    w_u32(packet + 200u, tagged);
    w_u32(packet + 4u, color);
    w_u32(packet + 44u, color);
    w_u32(packet + 84u, color);
    w_u32(packet + 124u, color);
    w_u32(packet + 164u, color);
    w_u32(packet + 204u, color);
    w_u16(packet + 14u, clut);
    w_u16(packet + 54u, clut);
    w_u16(packet + 94u, clut);
    w_u16(packet + 134u, clut);
    w_u16(packet + 174u, clut);
    w_u16(packet + 214u, clut);
    w_u16(packet + 22u, page);
    w_u16(packet + 62u, page);
    w_u16(packet + 102u, page);
    w_u16(packet + 142u, page);
    w_u16(packet + 182u, page);
    w_u16(packet + 222u, page);
    a = r_u32(context + 316u); b = r_u32(context + 332u);
    c = r_u32(context + 348u); d = r_u32(context + 364u);
    w_u32(packet + 208u, a);
    w_u32(packet + 24u, a);
    w_u32(packet + 8u, b);
    w_u32(packet + 48u, b);
    w_u32(packet + 104u, b);
    w_u32(packet + 88u, c);
    w_u32(packet + 144u, c);
    w_u32(packet + 128u, d);
    w_u32(packet + 168u, d);
    w_u32(packet + 224u, d);
    a = r_u16(context + 306u); b = r_u16(context + 322u);
    c = r_u16(context + 338u); d = r_u16(context + 354u);
    w_u16(packet + 212u, a);
    w_u16(packet + 28u, a);
    w_u16(packet + 12u, b);
    w_u16(packet + 52u, b);
    w_u16(packet + 108u, b);
    w_u16(packet + 92u, c);
    w_u16(packet + 148u, c);
    w_u16(packet + 132u, d);
    w_u16(packet + 172u, d);
    w_u16(packet + 228u, d);
    xa = (sint16)r_u16(context + 308u); xb = (sint16)r_u16(context + 324u);
    xc = (sint16)r_u16(context + 340u); xd = (sint16)r_u16(context + 356u);
    xcenter = (xa + xb + xc + xd) >> 2; xab = (xa + xb) >> 1;
    xbc = (xb + xc) >> 1; xcd = (xc + xd) >> 1; xda = (xd + xa) >> 1;
    ya = (sint16)r_u16(context + 310u); w_u16(context + 404u, (uint32)xbc);
    yb = (sint16)r_u16(context + 326u); w_u16(context + 420u, (uint32)xcd);
    yc = (sint16)r_u16(context + 342u); w_u16(context + 436u, (uint32)xda);
    yd = (sint16)r_u16(context + 358u);
    w_u16(context + 372u, (uint32)xcenter); w_u16(context + 388u, (uint32)xab);
    ycenter = (ya + yb + yc + yd) >> 2; yab = (ya + yb) >> 1;
    ybc = (yb + yc) >> 1; ycd = (yc + yd) >> 1; yda = (yd + ya) >> 1;
    w_u16(context + 374u, (uint32)ycenter);
    za = (sint16)r_u16(context + 312u); w_u16(context + 406u, (uint32)ybc);
    zb = (sint16)r_u16(context + 328u); w_u16(context + 422u, (uint32)ycd);
    zc = (sint16)r_u16(context + 344u); w_u16(context + 438u, (uint32)yda);
    zd = (sint16)r_u16(context + 360u); w_u16(context + 390u, (uint32)yab);
    w_u16(context + 376u, (uint32)((za + zb + zc + zd) >> 2));
    w_u16(context + 392u, (uint32)((za + zb) >> 1));
    w_u16(context + 408u, (uint32)((zb + zc) >> 1));
    w_u16(context + 424u, (uint32)((zc + zd) >> 1));
    w_u16(context + 440u, (uint32)((zd + za) >> 1));
    primitive_vector(0u, context + 388u); primitive_vector(2u, context + 404u); primitive_vector(4u, context + 420u);
    a = r_u8(context + 306u); b = r_u8(context + 322u); xport_gte_execute(0x280030u);
    c = r_u8(context + 338u); d = r_u8(context + 354u);
    w_u8(context + 370u, (a + b + c + d) >> 2); w_u8(context + 386u, (a + b) >> 1);
    w_u8(context + 402u, (b + c) >> 1); w_u8(context + 418u, (c + d) >> 1); w_u8(context + 434u, (d + a) >> 1);
    primitive_screen(packet, 16u, 12u);
    primitive_screen(packet, 56u, 12u);
    primitive_screen(packet, 96u, 13u);
    primitive_screen(packet, 152u, 13u);
    primitive_screen(packet, 192u, 13u);
    primitive_screen(packet, 136u, 14u);
    primitive_screen(packet, 176u, 14u);
    primitive_vector(0u, context + 436u); primitive_vector(2u, context + 372u);
    a = r_u8(context + 307u); b = r_u8(context + 323u); xport_gte_execute(0x280030u);
    c = r_u8(context + 339u); d = r_u8(context + 355u);
    w_u8(context + 371u, (a + b + c + d) >> 2); w_u8(context + 387u, (a + b) >> 1);
    w_u8(context + 403u, (b + c) >> 1); w_u8(context + 419u, (c + d) >> 1); w_u8(context + 435u, (d + a) >> 1);
    primitive_screen(packet, 216u, 12u);
    primitive_screen(packet, 32u, 12u);
    primitive_screen(packet, 72u, 12u);
    primitive_screen(packet, 64u, 13u);
    primitive_screen(packet, 112u, 13u);
    primitive_screen(packet, 184u, 13u);
    primitive_screen(packet, 232u, 13u);
    center = r_u16(context + 370u); ab = r_u16(context + 386u); bc = r_u16(context + 402u);
    cd = r_u16(context + 418u); da = r_u16(context + 434u);
    w_u16(packet + 68u, center);
    w_u16(packet + 116u, center);
    w_u16(packet + 188u, center);
    w_u16(packet + 236u, center);
    w_u16(packet + 20u, ab);
    w_u16(packet + 60u, ab);
    w_u16(packet + 100u, bc);
    w_u16(packet + 156u, bc);
    w_u16(packet + 196u, bc);
    w_u16(packet + 140u, cd);
    w_u16(packet + 180u, cd);
    w_u16(packet + 220u, da);
    w_u16(packet + 36u, da);
    w_u16(packet + 76u, da);
    ordering = r_u32(context + 168u);
    w_u32(context + 176u, packet + 200u); w_u32(context + 180u, packet + 240u);
    previous = primitive_tag24(ordering);
    primitive_link24(ordering, packet); primitive_link24(packet + 200u, previous);
}

uint32 gtmain_model_depth_cued_draw(uint32 context, uint32 model_header)
{
    static const uint8 input_bytes[8] = {12, 12, 20, 24, 24, 24, 32, 36};
    static const uint8 output_bytes[8] = {20, 24, 28, 36, 32, 40, 40, 52};
    uint32 group, result = model_header;
    for (group = 0; group < 8u; ++group)
    {
        uint32 count = r_u16(result + 48u + group * 2u), input = r_u32(context + 932u);
        uint32 packet, vertices, ordering_base, cue_table, shift, packed;
        if (!count) continue;
        packed = r_u32(input); packet = r_u32(context + 928u); vertices = r_u32(context + 916u);
        ordering_base = r_u32(context + 924u); cue_table = r_u32(context + 920u); shift = r_u16(context + 40u);
        do
        {
            uint32 flags = packed, second, color, selector = (flags >> 30) & 1u;
            uint32 a = vertices + (flags & 1023u) * 8u;
            uint32 b = vertices + ((flags >> 7) & 0x1FF8u), c = vertices + ((flags >> 17) & 0x1FF8u);
            uint32 depth, d1, d2, area, area2 = 0u, ordering, cue, uv0, uv1, uv2;
            if (group & 1u)
            {
                uint32 fourth, xy, z;
                primitive_vector(0u, b); primitive_vector(2u, c);
                if (group == 5u)
                {
                    w_u32(context + 324u, xport_gte_read_data(0)); w_u32(context + 328u, xport_gte_read_data(1));
                }
                primitive_vector(4u, a); --count; xport_gte_execute(0x280030u);
                second = r_u32(input + 4u); packed = r_u32(input + input_bytes[group]);
                fourth = vertices + (second & 1023u) * 8u; xy = r_u32(fourth); z = r_u32(fourth + 4u);
                xport_gte_execute(0x1400006u); xport_gte_write_data(0u, xy); xport_gte_write_data(1u, z);
                area = xport_gte_read_data(24u); depth = xport_gte_read_data(17u);
                d1 = xport_gte_read_data(18u); d2 = xport_gte_read_data(19u);
                if (group == 5u) primitive_screen(context, 332u, 12u);
                primitive_screen(packet, group == 1u ? 12u : group == 7u ? 20u : 16u, 12u);
                xport_gte_execute(0x180001u); area = 0u - area;
                depth = depth_choose(depth, d1, selector); depth = depth_choose(depth, d2, selector);
                xport_gte_write_data(6u, r_u32(input + 8u)); d1 = xport_gte_read_data(19u);
                xport_gte_execute(0x1400006u); depth = depth_choose(depth, d1, selector);
                depth = depth_bucket(depth, shift); area2 = xport_gte_read_data(24u);
                primitive_screen(packet, group == 1u ? 20u : group == 7u ? 44u : 32u, 12u);
                primitive_screen(packet, 8u, 13u);
                if ((sint32)(flags & area & area2) < 0) goto depth_next;
                cue = (second & 0x400u) ? 0u : r_u16(cue_table + depth * 2u);
                primitive_screen(packet, group == 1u ? 16u : group == 7u ? 32u : 24u, 14u);
                xport_gte_write_data(8u, cue);
            }
            else
            {
                primitive_vector(0u, c); primitive_vector(2u, b); primitive_vector(4u, a);
                --count; xport_gte_execute(0x280030u);
                packed = r_u32(input + input_bytes[group]); second = r_u32(input + 4u);
                xport_gte_execute(0x1400006u);
                color = r_u32(input + 8u);
                if (group == 0u || group == 4u) xport_gte_write_data(6u, color);
                area = xport_gte_read_data(24u); depth = xport_gte_read_data(17u);
                w_u8(packet + 3u, group == 0u ? 4u : group == 2u ? 6u : group == 4u ? 7u : 9u);
                if ((sint32)(flags & area) < 0) goto depth_next;
                d1 = xport_gte_read_data(18u); d2 = xport_gte_read_data(19u);
                primitive_screen(packet, group == 0u ? 16u : group == 6u ? 32u : 24u, 12u);
                depth = depth_choose(depth, d1, selector);
                primitive_screen(packet, group == 0u ? 12u : group == 6u ? 20u : 16u, 13u);
                depth = depth_choose(depth, d2, selector); depth = depth_bucket(depth, shift);
                primitive_screen(packet, 8u, 14u);
                cue = (second & 0x400u) ? 0u : r_u16(cue_table + depth * 2u);
                if (group == 2u || group == 6u)
                {
                    xport_gte_write_data(6u, color); xport_gte_write_data(20u, color);
                    xport_gte_write_data(21u, r_u32(input + (group == 2u ? 12u : 24u)));
                    xport_gte_write_data(22u, r_u32(input + (group == 2u ? 16u : 28u)));
                }
                xport_gte_write_data(8u, cue);
            }
            ordering = ordering_base + (depth + ((0u - selector) & 0x60u)) * 4u;
            xport_gte_execute(group == 2u || group == 6u ? 0xF8002Au : 0x780010u);
            switch (group)
            {
            case 0u:
                primitive_link(ordering, packet); primitive_screen(packet, 4u, 22u); break;
            case 1u:
                w_u32(packet, 0x05000000u); primitive_link(ordering, packet); primitive_screen(packet, 4u, 22u); break;
            case 2u:
                primitive_link(ordering, packet);
                primitive_screen(packet, 4u, 20u); primitive_screen(packet, 12u, 21u); primitive_screen(packet, 20u, 22u); break;
            case 3u:
            case 7u:
                uv0 = r_u32(input + (group == 3u ? 12u : 24u));
                uv1 = r_u32(input + (group == 3u ? 16u : 28u));
                uv2 = r_u32(input + (group == 3u ? 20u : 32u));
                primitive_screen(packet, 4u, 22u);
                xport_gte_write_data(20u, uv0); xport_gte_write_data(21u, uv1); xport_gte_write_data(22u, uv2);
                xport_gte_execute(0xF8002Au);
                if (group == 3u)
                {
                    w_u32(packet, 0x08000000u); primitive_link(ordering, packet);
                    primitive_screen(packet, 12u, 20u); primitive_screen(packet, 28u, 21u); primitive_screen(packet, 20u, 22u);
                }
                else
                {
                    uv0 = r_u32(input + 12u); uv1 = r_u32(input + 16u); uv2 = r_u32(input + 20u);
                    w_u32(packet, 0x0C000000u); primitive_link(ordering, packet);
                    w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 48u, uv2);
                    w_u16(packet + 36u, uv2 >> 16);
                    primitive_screen(packet, 16u, 20u); primitive_screen(packet, 40u, 21u); primitive_screen(packet, 28u, 22u);
                }
                break;
            case 4u:
            case 5u:
                uv0 = r_u32(input + 12u); uv1 = r_u32(input + 16u); uv2 = r_u32(input + 20u);
                if ((sint32)(primitive_abs(area) + (group == 5u ? primitive_abs(area2) : 0u)) < (group == 5u ? 1568 : 800))
                {
                    if (group == 5u) w_u32(packet, 0x09000000u);
                    primitive_link(ordering, packet); w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1);
                    w_u32(packet + (group == 5u ? 36u : 28u), uv2);
                    if (group == 5u) w_u16(packet + 28u, uv2 >> 16);
                    primitive_screen(packet, 4u, 22u);
                }
                else
                {
                    if (group == 4u)
                        primitive_subdivide_triangle(context, packet, ordering, uv0, uv1, uv2, count, packed);
                    else
                    {
                        depth_quad_workspace(context, packet, ordering, uv0, uv1, uv2, packed);
                        if ((sint32)(primitive_abs(area) + primitive_abs(area2)) >= 3136)
                            primitive_subdivide_quad(context);
                        else
                            depth_quad_six(context, packet, ordering);
                    }
                    packet = r_u32(context + 180u); vertices = r_u32(context + 916u);
                    ordering_base = r_u32(context + 924u); cue_table = r_u32(context + 920u); shift = r_u16(context + 40u);
                    if (group == 4u) count = r_u32(context + 960u);
                    packed = r_u32(context + 952u); goto depth_next;
                }
                break;
            case 6u:
                uv0 = r_u32(input + 12u); uv1 = r_u32(input + 16u); uv2 = r_u32(input + 20u);
                primitive_link(ordering, packet); w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 36u, uv2);
                primitive_screen(packet, 4u, 20u); primitive_screen(packet, 16u, 21u); primitive_screen(packet, 28u, 22u); break;
            }
            packet += output_bytes[group];
depth_next:
            input += input_bytes[group];
        } while (count);
        result = r_u32(context + 912u);
        if (group & 1u) { w_u32(context + 928u, packet); w_u32(context + 932u, input); }
        else { w_u32(context + 932u, input); w_u32(context + 928u, packet); }
    }
    return result;
}

