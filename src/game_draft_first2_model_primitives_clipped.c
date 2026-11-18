#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 clipped_flag(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return (uint32)state.flag;
}

static void clipped_link24(uint32 address, uint32 value)
{
    w_u8(address, value); w_u8(address + 1u, value >> 8); w_u8(address + 2u, value >> 16);
}

static void clipped_link(uint32 ordering, uint32 packet)
{
    uint32 previous = r_u32(ordering) & 0xFFFFFFu;
    clipped_link24(ordering, packet); clipped_link24(packet, previous);
}

static void clipped_vector(uint32 reg, uint32 vertex)
{
    uint32 xy = r_u32(vertex), z = r_u32(vertex + 4u);
    xport_gte_write_data(reg, xy); xport_gte_write_data(reg + 1u, z);
}

static void clipped_screen(uint32 address, uint32 reg)
{
    w_u32(address, xport_gte_read_data(reg));
}

static uint32 clipped_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static uint32 clipped_restore(uint32 context)
{
    uint32 a = (uint32)(sint32)(sint16)r_u16(context + 920u);
    uint32 b = (uint32)(sint32)(sint16)r_u16(context + 922u), result;
    gte_ctc2(a, 13u); gte_ctc2(b, 14u);
    a = r_u32(context + 948u); b = r_u32(context + 952u);
    gte_ctc2(a, 8u); a = r_u32(context + 956u); gte_ctc2(b, 9u);
    b = r_u32(context + 960u); gte_ctc2(a, 10u);
    result = r_u32(context + 964u); gte_ctc2(b, 11u); gte_ctc2(result, 12u);
    return result;
}

static void clipped_save_vectors(uint32 context, uint32 quad)
{
    uint32 v0 = quad ? 420u : 388u, v1 = quad ? 388u : 356u;
    clipped_screen(context + v0, 0u); clipped_screen(context + v0 + 4u, 1u);
    clipped_screen(context + v1, 2u); clipped_screen(context + v1 + 4u, 3u);
    clipped_screen(context + 324u, 4u); clipped_screen(context + 328u, 5u);
}

static uint32 clipped_fallback_cull(uint32 context, uint32 quad, uint32 winding, uint32 *return_value)
{
    sint32 x = (sint16)r_u16(context + 56u), y = (sint16)r_u16(context + 58u);
    sint32 z = (sint16)r_u16(context + 60u);
    uint32 a, b, c, r0, r2, result;
    if (!quad)
    {
        clipped_screen(context + 388u, 0u); clipped_screen(context + 392u, 1u);
        clipped_screen(context + 356u, 2u); clipped_screen(context + 360u, 3u);
    }
    else
    {
        clipped_screen(context + 388u, 2u); clipped_screen(context + 392u, 3u);
    }
    clipped_screen(context + 324u, 4u); clipped_screen(context + 328u, 5u);
    gte_ctc2((uint32)((sint16)r_u16(context + 356u) - x), 0u);
    gte_ctc2((uint32)((sint16)r_u16(context + 358u) - y), 2u);
    gte_ctc2((uint32)((sint16)r_u16(context + 360u) - z), 4u);
    xport_gte_write_data(9u, (uint32)((sint16)r_u16(context + 388u) - x));
    xport_gte_write_data(10u, (uint32)((sint16)r_u16(context + 390u) - y));
    xport_gte_write_data(11u, (uint32)((sint16)r_u16(context + 392u) - z));
    xport_gte_execute(0x170000Cu);
    a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    xport_gte_write_data(9u, (uint32)((sint16)r_u16(context + 324u) - x));
    xport_gte_write_data(10u, (uint32)((sint16)r_u16(context + 326u) - y));
    xport_gte_write_data(11u, (uint32)((sint16)r_u16(context + 328u) - z));
    gte_ctc2((a & 4095u) | ((b & 4095u) << 16), 16u);
    a = (uint32)((sint32)a >> 12); b = (uint32)((sint32)b >> 12);
    gte_ctc2((c & 4095u) | ((a & 4095u) << 16), 17u);
    c = (uint32)((sint32)c >> 12); a = (uint32)((sint32)a >> 12);
    gte_ctc2((b & 4095u) | ((c & 4095u) << 16), 18u);
    b = (uint32)((sint32)b >> 12); c = (uint32)((sint32)c >> 12);
    gte_ctc2((a & 65535u) | (b << 16), 19u); gte_ctc2(c, 20u);
    r0 = r_u32(context + 96u); r2 = r_u32(context + 104u);
    xport_gte_execute(0x45E012u); result = r_u32(context + 112u);
    gte_ctc2(r0, 0u); gte_ctc2(r2, 2u); gte_ctc2(result, 4u);
    a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    result = (uint32)((sint32)(b + (uint32)((sint32)a >> 12)) >> 12);
    *return_value = result; return (c + result) ^ winding;
}

static uint32 clipped_bounds(uint32 context, uint32 quad, uint32 *flags, uint32 *result)
{
    uint32 axis, vertex;
    for (axis = 0; axis != 4u; axis += 2u)
    {
        sint32 minimum = (sint16)r_u16(context + 332u + axis), maximum = minimum;
        for (vertex = 364u; vertex <= (quad ? 428u : 396u); vertex += 32u)
        {
            sint32 value = (sint16)r_u16(context + vertex + axis);
            if (value < minimum) minimum = value;
            if (value > maximum) maximum = value;
        }
        {
            sint32 limit = (sint16)r_u16(context + 4u + axis);
            if (maximum <= 0) { *result = (uint32)minimum; return 0; }
            *result = (uint32)((axis ? 511 : 1023) - (maximum - minimum));
            if (minimum >= limit) return 0;
            *flags |= *result;
        }
    }
    return 1;
}

static void clipped_backup(uint32 context, uint32 quad)
{
    clipped_screen(context + (quad ? 796u : 788u), 0u);
    clipped_screen(context + (quad ? 800u : 792u), 1u);
    clipped_screen(context + (quad ? 788u : 780u), 2u);
    clipped_screen(context + (quad ? 792u : 784u), 3u);
    clipped_screen(context + 772u, 4u); clipped_screen(context + 776u, 5u);
    if (quad)
    {
        uint32 a = r_u32(context + 356u), b = r_u32(context + 360u);
        w_u32(context + 780u, a); w_u32(context + 784u, b);
    }
}

static uint32 clipped_overlay(uint32 context, uint32 *packet, uint32 ordering,
                               uint32 second, uint32 third, uint32 quad,
                               uint32 *flags, uint32 *result)
{
    uint32 table, a, b, color, clut, i;
    uint32 uv[8];
    clipped_screen(context + (quad ? 420u : 388u), 0u);
    clipped_screen(context + (quad ? 424u : 392u), 1u);
    clipped_screen(context + (quad ? 388u : 356u), 2u);
    clipped_screen(context + (quad ? 392u : 360u), 3u);
    if ((sint32)*flags < 0)
    {
        clipped_screen(context + 324u, 4u); clipped_screen(context + 328u, 5u);
        clipped_backup(context, quad);
    }
    table = r_u32(context + 944u);
    clipped_vector(0u, table + ((second >> 6) & 0x1FF8u));
    a = table + ((second >> 16) & 0x1FF8u);
    xport_gte_execute(0x4A2012u); clipped_vector(2u, a);
    uv[0] = xport_gte_read_data(9u); uv[1] = xport_gte_read_data(10u);
    w_u8(*packet + 12u, uv[0]); w_u8(*packet + 13u, uv[1]);
    xport_gte_execute(0x4AA012u); clipped_vector(0u, table + (third & 1023u) * 8u);
    uv[2] = xport_gte_read_data(9u); uv[3] = xport_gte_read_data(10u);
    w_u8(*packet + 20u, uv[2]); w_u8(*packet + 21u, uv[3]);
    xport_gte_execute(0x4A2012u);
    if (quad)
    {
        clipped_vector(2u, table + ((third >> 7) & 0x1FF8u));
        uv[6] = xport_gte_read_data(9u); uv[7] = xport_gte_read_data(10u);
        w_u8(*packet + 36u, uv[6]); w_u8(*packet + 37u, uv[7]);
        xport_gte_execute(0x4AA012u);
    }
    clut = r_u32(context + 916u); color = r_u32(context + (quad ? 972u : 968u));
    if ((sint32)*flags >= 0)
    {
        w_u32(*packet, quad ? 0x09000000u : 0x07000000u); w_u32(*packet + 4u, color);
        w_u16(*packet + 22u, clut >> 16); w_u16(*packet + 14u, clut);
    }
    else
    {
        for (i = 0; i < 3u; ++i)
        {
            if (i == 2u && !quad) continue;
            w_u16(context + 336u + i * 32u, (uv[(i == 2u ? 6u : i * 2u)] << 7) | 64u);
            w_u16(context + 338u + i * 32u, (uv[(i == 2u ? 7u : i * 2u + 1u)] << 7) | 64u);
        }
        w_u32(context + 168u, ordering); w_u32(context + 172u, *packet);
        w_u16(context + 184u, quad ? 4u : 3u); w_u32(context + 156u, color);
        w_u16(context + 164u, clut); w_u16(context + 166u, clut >> 16);
    }
    uv[4] = xport_gte_read_data(9u); uv[5] = xport_gte_read_data(10u);
    w_u8(*packet + 28u, uv[4]); w_u8(*packet + 29u, uv[5]);
    if ((sint32)*flags >= 0)
    {
        if (quad)
        {
            a = r_u32(context + 1004u);
            clipped_screen(*packet + 32u, 12u); clipped_screen(*packet + 8u, 13u);
            clipped_screen(*packet + 24u, 14u); w_u32(*packet + 16u, a);
        }
        else
        {
            clipped_screen(*packet + 24u, 12u); clipped_screen(*packet + 16u, 13u); clipped_screen(*packet + 8u, 14u);
        }
        *result = *packet << 8; clipped_link(ordering, *packet); *packet += quad ? 40u : 32u;
        return 1;
    }
    w_u16(context + (quad ? 432u : 400u), (uv[4] << 7) | 64u);
    w_u16(context + (quad ? 434u : 402u), (uv[5] << 7) | 64u);
    a = gtmain_polygon_clip_draw(context); *packet = r_u32(context + 180u); *flags = 0xFFFFFFFFu;
    if (!a) { *result = clipped_restore(context); return 0; }
    clipped_vector(0u, context + (quad ? 796u : 788u));
    clipped_vector(2u, context + (quad ? 788u : 780u)); clipped_vector(4u, context + 772u);
    if (quad)
    {
        a = r_u32(context + 780u); b = r_u32(context + 784u);
        w_u32(context + 356u, a); w_u32(context + 360u, b);
    }
    return 1;
}

static uint32 clipped_project_run(uint32 context, uint32 vertex, uint32 count,
                                  uint32 depth_register, uint32 *minimum, uint32 *maximum)
{
    uint32 every = 65535u, near_depth = r_u16(context + 8u);
    sint32 width = (sint16)r_u16(context + 4u), height = (sint16)r_u16(context + 6u);
    *minimum = 0; *maximum = 0;
    do
    {
        uint32 flag = clipped_flag(), mask, depth;
        sint32 x, y;
        clipped_screen(vertex + 12u, 14u); depth = xport_gte_read_data(depth_register);
        clipped_vector(0u, vertex + 20u);
        x = (sint16)r_u16(vertex + 12u); y = (sint16)r_u16(vertex + 14u);
        xport_gte_execute(0x180001u);
        mask = (sint32)depth < (sint32)near_depth;
        if (x < 0) mask |= 2u;
        if (width < x) mask |= 4u;
        if (y < 0) mask |= 8u;
        if (height < y) mask |= 16u;
        if ((sint32)flag < 0) mask |= 32u;
        w_u16(vertex, mask);
        if ((sint32)depth < (sint32)*minimum) *minimum = depth;
        if ((sint32)*maximum < (sint32)depth) *maximum = depth;
        every &= mask; vertex += 16u;
    }
    while (--count);
    return every;
}

static void clipped_midpoints(uint32 context, uint32 list, uint32 pool, uint32 quad)
{
    uint32 a = r_u32(list), b = r_u32(list + 4u), c = r_u32(list + 8u), d = 0, offset;
    uint32 minimum, maximum;
    if (quad) d = r_u32(list + 12u);
    for (offset = 4u; offset <= 8u; offset += 2u)
    {
        sint32 av = (sint16)r_u16(a + offset), bv = (sint16)r_u16(b + offset);
        sint32 cv = (sint16)r_u16(c + offset);
        if (quad)
        {
            sint32 dv = (sint16)r_u16(d + offset);
            w_u16(pool + 32u + offset, (uint32)((bv + cv) >> 1));
            w_u16(pool + 64u + offset, (uint32)((dv + av) >> 1));
            w_u16(pool + 16u + offset, (uint32)((av + bv) >> 1));
            w_u16(pool + 48u + offset, (uint32)((cv + dv) >> 1));
            w_u16(pool + offset, (uint32)((av + bv + cv + dv) >> 2));
        }
        else
        {
            w_u16(pool + offset, (uint32)((av + bv) >> 1));
            w_u16(pool + 16u + offset, (uint32)((bv + cv) >> 1));
            w_u16(pool + 32u + offset, (uint32)((cv + av) >> 1));
        }
    }
    clipped_vector(0u, pool + 4u); xport_gte_execute(0x180001u);
    for (offset = 2u; offset <= 3u; ++offset)
    {
        uint32 av = r_u8(a + offset), bv = r_u8(b + offset), cv = r_u8(c + offset);
        if (quad)
        {
            uint32 dv = r_u8(d + offset);
            w_u8(pool + 32u + offset, (bv + cv) >> 1); w_u8(pool + 64u + offset, (dv + av) >> 1);
            w_u8(pool + 16u + offset, (av + bv) >> 1); w_u8(pool + 48u + offset, (cv + dv) >> 1);
            w_u8(pool + offset, (av + bv + cv + dv) >> 2);
        }
        else
        {
            w_u8(pool + offset, (av + bv) >> 1); w_u8(pool + 16u + offset, (bv + cv) >> 1);
            w_u8(pool + 32u + offset, (cv + av) >> 1);
        }
    }
    clipped_project_run(context, pool, quad ? 5u : 3u, quad ? 11u : 19u, &minimum, &maximum);
}

static uint32 clipped_uv(uint32 vertex, uint32 carrier)
{
    return (carrier & 0xFFFF0000u) | (r_u32(vertex) >> 16);
}

static void clipped_subdivide(uint32 context, uint32 quad)
{
    uint32 list = context + 204u, pool = context + (quad ? 368u : 352u);
    uint32 current, first, second, third, fourth = 0, minimum, maximum, every;
    uint32 count = quad ? 4u : 3u, i;
    sint32 depth = 0, child;
    first = context + 304u; clipped_vector(0u, first + 4u); xport_gte_execute(0x180001u);
    for (i = 1u; i != count; ++i) w_u32(list + i * 4u, first + i * 16u);
    w_u32(list, first); current = r_u32(context + 172u);
    w_u32(context + 176u, current); w_u32(context + 180u, current);
    first = r_u32(context + 156u);
    w_u32(context + 160u, quad ? first & 0xF7FFFFFFu : first | 0x08000000u);
    every = clipped_project_run(context, context + 304u, count, 11u, &minimum, &maximum);
    if (every & 31u) return;
    i = (r_u16(context + 188u) + 1u) & 31u;
    if ((sint32)(maximum - (uint32)((sint32)(maximum - minimum) >> i)) < (sint32)r_u16(context + 8u)) return;
clipped_midpoint_level:
    clipped_midpoints(context, list, pool, quad); current = r_u32(context + 180u);
    {
        uint32 side_uv0 = r_u32(context + 164u) << 16, side_uv1 = r_u32(context + 164u);
        uint32 side_color = r_u32(context + (quad ? 160u : 156u));
        for (child = (sint32)count - 1; child >= 0; --child)
        {
            uint32 area, third_uv, mask;
            if (quad)
            {
                uint32 slot = count - 1u - (uint32)child;
                first = r_u32(list + slot * 4u); second = r_u32(list + ((slot + 1u) & 3u) * 4u);
                third = pool + 16u + slot * 16u;
            }
            else
            {
                sint32 previous = child - 1;
                if (previous < 0) previous = 2;
                first = r_u32(list + (uint32)child * 4u); second = pool + (uint32)previous * 16u;
                third = r_u32(list + (uint32)previous * 4u);
            }
            mask = (r_u16(first) | r_u16(second) | r_u16(third)) & 32u;
            if (mask) continue;
            xport_gte_write_data(12u, r_u32(first + 12u)); xport_gte_write_data(13u, r_u32(second + 12u));
            xport_gte_write_data(14u, r_u32(third + 12u)); xport_gte_execute(0x1400006u);
            third_uv = clipped_uv(third, quad ? side_color : mask);
            side_uv0 = clipped_uv(first, side_uv0); side_uv1 = clipped_uv(second, side_uv1);
            w_u32(current + 4u, side_color); w_u32(current + 12u, side_uv0);
            w_u32(current + 20u, side_uv1); w_u32(current + 28u, third_uv);
            area = xport_gte_read_data(24u);
            clipped_screen(current + 8u, 12u); clipped_screen(current + 16u, 13u);
            if (area)
            {
                clipped_screen(current + 24u, 14u); w_u8(current + 3u, 7u);
                clipped_link24(current, current + 32u); w_u32(context + 176u, current); current += 32u;
            }
        }
    }
    w_u32(context + 180u, current); child = 3;
clipped_child_level:
    while (child >= 0)
    {
        uint32 all, any, maximum_depth, area, threshold, uv0, uv1, uv2, uv3 = 0, color;
        uint32 descend = 0;
        sint32 previous = child - 1;
        if (previous < 0) previous = quad ? 3 : 2;
        if (quad)
        {
            if ((child & 1) == 0)
            {
                first = r_u32(list + (uint32)child * 4u); second = pool + 16u + (uint32)child * 16u;
                third = pool; fourth = pool + 16u + (uint32)previous * 16u;
            }
            else
            {
                first = pool + 16u + (uint32)previous * 16u; second = r_u32(list + (uint32)child * 4u);
                third = pool + 16u + (uint32)child * 16u; fourth = pool;
            }
        }
        else if (child == 3) { first = pool; second = pool + 16u; third = pool + 32u; }
        else { first = r_u32(list + (uint32)child * 4u); second = pool + (uint32)child * 16u; third = pool + (uint32)previous * 16u; }
        --child;
        all = r_u16(first) & r_u16(second) & r_u16(third);
        any = r_u16(first) | r_u16(second) | r_u16(third);
        if (quad) { all &= r_u16(fourth); any |= r_u16(fourth); }
        if (all & 31u) continue;
        maximum_depth = r_u16(context + 188u);
        if (any & 32u)
        {
            w_u32(list + (quad ? 16u : 12u), first);
            if (depth >= (sint32)maximum_depth) continue;
            descend = 1;
        }
        else
        {
            uint32 area1;
            xport_gte_write_data(12u, r_u32(second + 12u)); xport_gte_write_data(13u, r_u32(first + 12u));
            xport_gte_write_data(14u, r_u32(third + 12u)); current = r_u32(context + 180u);
            uv0 = r_u32(context + 164u) << 16; xport_gte_execute(0x1400006u);
            uv1 = r_u32(context + 164u); uv0 = clipped_uv(second, uv0); uv1 = clipped_uv(first, uv1);
            w_u32(current + 12u, uv0); w_u32(current + 20u, uv1);
            if (quad)
            {
                uv0 = clipped_uv(third, uv0); uv1 = clipped_uv(fourth, uv1);
                w_u32(current + 28u, uv0); area1 = xport_gte_read_data(24u);
                xport_gte_write_data(12u, r_u32(fourth + 12u)); w_u32(current + 36u, uv1);
                color = r_u32(context + 156u); uv3 = r_u32(second + 12u); xport_gte_execute(0x1400006u);
                w_u32(current + 8u, uv3); w_u32(current + 4u, color); w_u8(current + 3u, 9u);
                clipped_link24(current, current + 40u); threshold = r_u32(context + 184u);
                area = xport_gte_read_data(24u); clipped_screen(current + 32u, 12u);
                area = clipped_abs(area1 - area); clipped_screen(current + 16u, 13u); clipped_screen(current + 24u, 14u);
            }
            else
            {
                color = r_u32(context + 156u); uv2 = clipped_uv(third, color);
                w_u32(current + 4u, color); w_u32(current + 28u, uv2);
                area = clipped_abs(xport_gte_read_data(24u)); clipped_screen(current + 8u, 12u);
                threshold = r_u32(context + 184u);
            }
            descend = (sint32)threshold < (sint32)area && depth < (sint32)maximum_depth;
            w_u32(list + (quad ? 16u : 12u), first);
            if (!descend)
            {
                if (!quad)
                {
                    clipped_screen(current + 16u, 13u); clipped_screen(current + 24u, 14u);
                    w_u8(current + 3u, 7u); clipped_link24(current, current + 32u);
                }
                w_u32(context + 176u, current); w_u32(context + 180u, current + (quad ? 40u : 32u));
            }
        }
        if (descend)
        {
            uint32 resume = context + 196u + (uint32)depth;
            pool += quad ? 80u : 48u; list += quad ? 16u : 12u; ++depth;
            w_u8(resume, (uint32)child); w_u32(list + 4u, second); w_u32(list + 8u, third);
            if (quad) w_u32(list + 12u, fourth);
            goto clipped_midpoint_level;
        }
    }
    --depth; child = (sint8)r_u8(context + 196u + (uint32)depth);
    list -= quad ? 16u : 12u; pool -= quad ? 80u : 48u;
    if (depth >= 0) goto clipped_child_level;
    first = r_u32(context + 168u); second = r_u32(context + 172u);
    third = r_u32(context + 180u); fourth = r_u32(context + 176u);
    if (second != third)
    {
        uint32 old = r_u32(first) & 0xFFFFFFu;
        clipped_link24(first, second); clipped_link24(fourth, old);
    }
}

static void clipped_save_call(uint32 context, uint32 quad, uint32 records, uint32 remaining,
                              uint32 packed, uint32 a, uint32 b, uint32 c)
{
    if (quad)
    {
        w_u32(context + 992u, records);
        w_u32(context + 976u, a); w_u32(context + 980u, b); w_u32(context + 984u, c);
    }
    w_u32(context + 996u, remaining); w_u32(context + 1000u, packed);
}

static void clipped_reload_call(uint32 context, uint32 quad, uint32 *records, uint32 *remaining,
                                uint32 *packed, uint32 *a, uint32 *b, uint32 *c)
{
    if (quad)
    {
        *records = r_u32(context + 992u);
        *a = r_u32(context + 976u); *b = r_u32(context + 980u); *c = r_u32(context + 984u);
    }
    *remaining = r_u32(context + 996u); *packed = r_u32(context + 1000u);
}

static void clipped_fixed_uv(uint32 context, uint32 uv0, uint32 uv1, uint32 uv2, uint32 quad)
{
    uint32 uv[4], i;
    w_u32(context + 164u, uv1); w_u16(context + 164u, uv0 >> 16);
    uv[0] = uv0; uv[1] = uv1; uv[2] = uv2; uv[3] = uv2 >> 16;
    for (i = 0; i != (quad ? 4u : 3u); ++i)
    {
        w_u16(context + 336u + i * 32u, ((uv[i] << 7) & 0x7F80u) | 64u);
        w_u16(context + 338u + i * 32u, ((uv[i] >> 1) & 0x7F80u) | 64u);
    }
}

static void clipped_prepare_subdivision(uint32 context, uint32 quad, uint32 packet,
                                        uint32 ordering, uint32 uv0, uint32 uv1, uint32 uv2)
{
    uint32 a = 0, b = 0;
    if (quad) { a = r_u32(context + 356u); b = r_u32(context + 360u); }
    w_u16(context + 188u, 5u); w_u32(context + 168u, ordering); w_u32(context + 172u, packet);
    w_u32(context + 184u, quad ? 2048u : 1152u);
    if (quad) { w_u32(context + 324u, a); w_u32(context + 328u, b); }
    clipped_screen(context + 156u, 6u); w_u32(context + 164u, uv1); w_u16(context + 164u, uv0 >> 16);
    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2);
    if (quad) { w_u32(context + 352u, uv2); }
    clipped_screen(context + (quad ? 356u : 340u), 0u);
    clipped_screen(context + (quad ? 360u : 344u), 1u);
    clipped_screen(context + (quad ? 340u : 324u), 2u);
    clipped_screen(context + (quad ? 344u : 328u), 3u);
    clipped_screen(context + 308u, 4u); clipped_screen(context + 312u, 5u);
}

/* Original: GTMAIN.EXE:sub_800743C0. */ uint32 gtmain_model_primitives_clipped_draw(uint32 model, uint32 context)
{
    static const uint32 stride[8] = {16u, 16u, 24u, 28u, 28u, 28u, 36u, 40u};
    uint32 group, records, result = r_u32(model + 8u);
    w_u32(context + 992u, result);
    for (group = 0; group != 8u; ++group)
    {
        uint32 remaining = r_u16(model + 44u + group * 2u), packet;
        uint32 packed, va, vb, vc, vertices;
        if (!remaining) continue;
        records = r_u32(context + 992u); packed = r_u32(records); packet = r_u32(context + 932u);
        vertices = r_u32(context + 940u);
        result = (packed >> 15) & 0xFF8u;
        va = vertices + (packed & 511u) * 8u; vb = vertices + ((packed >> 6) & 0xFF8u); vc = vertices + result;
        do
        {
            uint32 quad = group & 1u, first = packed, second, third, next_a, next_b, next_c;
            uint32 flag, minimum, maximum, depth1, depth2, depth3, shift, winding;
            uint32 area, area2 = 0, magnitude, cull, ordering, overlay, near_depth;
            uint32 uv0 = 0, uv1 = 0, uv2 = 0;
            if (quad)
            {
                uint32 xy, z;
                clipped_vector(0u, vb); clipped_vector(2u, vc);
                clipped_screen(context + 356u, 0u); clipped_screen(context + 360u, 1u);
                clipped_vector(4u, va); vertices = r_u32(context + 940u);
                xport_gte_execute(0x280030u);
                second = r_u32(records + 4u); packed = r_u32(records + stride[group]);
                result = vertices + (second & 511u) * 8u;
                xy = r_u32(result); z = r_u32(result + 4u);
                clipped_screen(context + 1004u, 12u); clipped_screen(context + 364u, 12u);
                flag = clipped_flag(); winding = (uint32)(sint32)(sint16)r_u16(context + 908u);
                xport_gte_execute(0x1400006u);
                next_a = vertices + (packed & 511u) * 8u; next_b = vertices + ((packed >> 6) & 0xFF8u);
                xport_gte_write_data(0u, xy); xport_gte_write_data(1u, z);
                area = xport_gte_read_data(24u); minimum = xport_gte_read_data(17u);
                depth1 = xport_gte_read_data(18u); depth2 = xport_gte_read_data(19u);
                near_depth = r_u16(context + 8u); shift = r_u16(context + 40u);
                xport_gte_execute(0x180001u);
                next_c = vertices + ((packed >> 15) & 0xFF8u);
                if (!winding) area = 0u - area;
                maximum = minimum;
                if ((sint32)depth1 < (sint32)minimum) minimum = depth1;
                if ((sint32)maximum < (sint32)depth1) maximum = depth1;
                if ((sint32)depth2 < (sint32)minimum) minimum = depth2;
                if ((sint32)maximum < (sint32)depth2) maximum = depth2;
                --remaining; flag |= clipped_flag(); depth3 = xport_gte_read_data(19u);
                xport_gte_execute(0x1400006u);
                if ((sint32)maximum < (sint32)depth3) maximum = depth3;
                if ((sint32)depth3 < (sint32)minimum) minimum = depth3;
                result = (sint32)maximum < (sint32)near_depth;
                if (result) goto clipped_next;
                ordering = ((second >> 31) ? maximum : minimum) << (shift & 31u);
                area2 = xport_gte_read_data(24u); if (winding) area2 = 0u - area2;
                cull = first & area & area2;
            }
            else
            {
                clipped_vector(0u, vc); clipped_vector(2u, vb); clipped_vector(4u, va);
                vertices = r_u32(context + 940u); xport_gte_execute(0x280030u);
                packed = r_u32(records + stride[group]); second = r_u32(records + 4u);
                next_a = vertices + (packed & 511u) * 8u; next_b = vertices + ((packed >> 6) & 0xFF8u);
                next_c = vertices + ((packed >> 15) & 0xFF8u);
                shift = r_u16(context + 40u); flag = clipped_flag(); minimum = xport_gte_read_data(17u);
                depth1 = xport_gte_read_data(18u); depth2 = xport_gte_read_data(19u);
                near_depth = r_u16(context + 8u); winding = (uint32)(sint32)(sint16)r_u16(context + 908u);
                xport_gte_execute(0x1400006u); maximum = minimum;
                if ((sint32)depth1 < (sint32)minimum) minimum = depth1;
                if ((sint32)maximum < (sint32)depth1) maximum = depth1;
                if ((sint32)depth2 < (sint32)minimum) minimum = depth2;
                if ((sint32)maximum < (sint32)depth2) maximum = depth2;
                --remaining; area = xport_gte_read_data(24u); if (winding) area = 0u - area;
                result = minimum;
                if ((sint32)maximum < (sint32)near_depth) goto clipped_next;
                ordering = ((second >> 31) ? maximum : minimum) << (shift & 31u);
                cull = area & first;
            }
            if ((sint32)flag < 0)
            {
                cull = 0;
                if ((sint32)first < 0) cull = clipped_fallback_cull(context, quad, winding, &result);
            }
            ordering = (uint32)((sint32)ordering >> 13);
            if ((sint32)cull < 0) goto clipped_next;
            overlay = r_u32(context + 912u) & first;
            if ((sint32)ordering >= 4096) ordering = 4095u;
            result = ordering * 4u; ordering = r_u32(context + 928u) + result;
            clipped_screen(context + 396u, 12u);
            clipped_screen(context + (quad ? 332u : 364u), 13u);
            clipped_screen(context + (quad ? 428u : 332u), 14u);
            if (!clipped_bounds(context, quad, &flag, &result)) goto clipped_next;
            magnitude = quad ? clipped_abs(area) + clipped_abs(area2) : clipped_abs(area);
            third = r_u32(records + 8u);
            if (overlay)
            {
                if ((sint32)flag < 0) clipped_save_call(context, quad, records, remaining, packed, next_a, next_b, next_c);
                if (!clipped_overlay(context, &packet, ordering, second, third, quad, &flag, &result))
                {
                    clipped_reload_call(context, quad, &records, &remaining, &packed, &next_a, &next_b, &next_c);
                    goto clipped_next;
                }
                if ((sint32)flag < 0)
                {
                    clipped_reload_call(context, quad, &records, &remaining, &packed, &next_a, &next_b, &next_c);
                    if (group == 4u || group == 5u) magnitude = quad ? 1568u : 800u;
                }
                if ((group == 4u || group == 5u) && (sint32)flag >= 0)
                {
                    clipped_vector(0u, context + (quad ? 420u : 388u));
                    clipped_vector(2u, context + (quad ? 388u : 356u));
                }
            }
            xport_gte_write_data(6u, r_u32(records + 12u));
            if (group == 2u || group == 3u || group == 6u || group == 7u)
            {
                uint32 color_offset = group >= 6u ? 28u : 16u;
                xport_gte_write_data(20u, r_u32(records + color_offset));
                xport_gte_write_data(21u, r_u32(records + color_offset + 4u));
                if (quad) xport_gte_write_data(22u, r_u32(records + color_offset + 8u));
            }
            if (group >= 4u)
            {
                uv0 = r_u32(records + 16u); result = r_u32(context + 904u);
                uv1 = r_u32(records + 20u); uv2 = r_u32(records + 24u); uv0 += result;
            }
            if ((group == 4u || group == 5u) && ((sint32)flag < 0 || (sint32)magnitude >= (quad ? 1568 : 800)))
            {
                clipped_save_call(context, quad, records, remaining, packed, next_a, next_b, next_c);
                clipped_prepare_subdivision(context, quad, packet, ordering, uv0, uv1, uv2);
                clipped_subdivide(context, quad); packet = r_u32(context + 180u);
                clipped_reload_call(context, quad, &records, &remaining, &packed, &next_a, &next_b, &next_c);
                result = clipped_restore(context); goto clipped_next;
            }
            if ((sint32)flag < 0)
            {
                clipped_save_call(context, quad, records, remaining, packed, next_a, next_b, next_c);
                w_u32(context + 168u, ordering); w_u32(context + 172u, packet); w_u16(context + 184u, quad ? 4u : 3u);
                if (group >= 6u) clipped_fixed_uv(context, uv0, uv1, uv2, quad);
                clipped_save_vectors(context, quad); clipped_screen(context + 156u, 6u);
                if (group == 2u || group == 3u || group >= 6u)
                {
                    clipped_screen(context + 340u, 6u); clipped_screen(context + 372u, 20u);
                    clipped_screen(context + 404u, 21u); if (quad) clipped_screen(context + 436u, 22u);
                }
                if (group >= 6u) gtmain_polygon_clip_textured_gouraud(context);
                else if (group >= 2u) gtmain_polygon_clip_gouraud(context);
                else gtmain_polygon_clip_flat(context);
                packet = r_u32(context + 180u);
                clipped_reload_call(context, quad, &records, &remaining, &packed, &next_a, &next_b, &next_c);
                result = clipped_restore(context); goto clipped_next;
            }
            switch (group)
            {
            case 0u:
                w_u32(packet, 0x04000000u); clipped_screen(packet + 16u, 12u);
                clipped_screen(packet + 12u, 13u); clipped_screen(packet + 8u, 14u); clipped_screen(packet + 4u, 6u);
                break;
            case 1u:
                w_u32(packet, 0x05000000u); result = r_u32(context + 1004u);
                clipped_screen(packet + 20u, 12u); clipped_screen(packet + 8u, 13u); clipped_screen(packet + 16u, 14u);
                w_u32(packet + 12u, result); clipped_screen(packet + 4u, 6u);
                break;
            case 2u:
                w_u32(packet, 0x06000000u); clipped_screen(packet + 24u, 12u); clipped_screen(packet + 16u, 13u);
                clipped_screen(packet + 8u, 14u); clipped_screen(packet + 4u, 6u);
                clipped_screen(packet + 12u, 20u); clipped_screen(packet + 20u, 21u);
                break;
            case 3u:
                w_u32(packet, 0x08000000u); result = r_u32(context + 1004u);
                clipped_screen(packet + 32u, 12u); clipped_screen(packet + 8u, 13u); clipped_screen(packet + 24u, 14u);
                w_u32(packet + 16u, result); clipped_screen(packet + 4u, 6u); clipped_screen(packet + 12u, 20u);
                clipped_screen(packet + 28u, 21u); clipped_screen(packet + 20u, 22u);
                break;
            case 4u:
                w_u32(packet, 0x07000000u); w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1); w_u32(packet + 28u, uv2);
                clipped_screen(packet + 24u, 12u); clipped_screen(packet + 16u, 13u); clipped_screen(packet + 8u, 14u);
                clipped_screen(packet + 4u, 6u);
                break;
            case 5u:
                w_u32(packet, 0x09000000u); w_u32(packet + 12u, uv0); w_u32(packet + 20u, uv1); w_u32(packet + 36u, uv2);
                w_u16(packet + 28u, uv2 >> 16); result = r_u32(context + 1004u);
                clipped_screen(packet + 32u, 12u); clipped_screen(packet + 8u, 13u); clipped_screen(packet + 24u, 14u);
                w_u32(packet + 16u, result); clipped_screen(packet + 4u, 6u);
                break;
            case 6u:
                w_u32(packet, 0x09000000u); w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 36u, uv2);
                clipped_screen(packet + 32u, 12u); clipped_screen(packet + 20u, 13u); clipped_screen(packet + 8u, 14u);
                clipped_screen(packet + 4u, 6u); clipped_screen(packet + 16u, 20u); clipped_screen(packet + 28u, 21u);
                break;
            case 7u:
                w_u32(packet, 0x0C000000u); w_u32(packet + 12u, uv0); w_u32(packet + 24u, uv1); w_u32(packet + 48u, uv2);
                w_u16(packet + 36u, uv2 >> 16); result = r_u32(context + 1004u);
                clipped_screen(packet + 44u, 12u); clipped_screen(packet + 8u, 13u); clipped_screen(packet + 32u, 14u);
                w_u32(packet + 20u, result); clipped_screen(packet + 4u, 6u); clipped_screen(packet + 16u, 20u);
                clipped_screen(packet + 40u, 21u); clipped_screen(packet + 28u, 22u);
                break;
            }
            result = packet << 8; clipped_link(ordering, packet);
            {
                static const uint32 sizes[8] = {20u, 24u, 28u, 36u, 32u, 40u, 40u, 52u};
                packet += sizes[group];
            }
clipped_next:
            records += stride[group]; va = next_a; vb = next_b; vc = next_c;
        }
        while (remaining);
        if (group != 7u) model = r_u32(context + 936u);
        w_u32(context + 992u, records); w_u32(context + 932u, packet);
    }
    return result;
}


