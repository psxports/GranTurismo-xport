#include "game_first2_registry.h"
#include "psx_gte.h"

static uint32 dcclip_flag(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return (uint32)state.flag;
}

static void dcclip_link24(uint32 address, uint32 value)
{
    w_u8(address, value); w_u8(address + 1u, value >> 8); w_u8(address + 2u, value >> 16);
}

static void dcclip_link(uint32 ordering, uint32 packet)
{
    uint32 previous = r_u32(ordering) & 0xFFFFFFu;
    dcclip_link24(ordering, packet); dcclip_link24(packet, previous);
}

static void dcclip_vector(uint32 reg, uint32 vertex)
{
    uint32 xy = r_u32(vertex), z = r_u32(vertex + 4u);
    xport_gte_write_data(reg, xy); xport_gte_write_data(reg + 1u, z);
}

static void dcclip_screen(uint32 address, uint32 reg)
{
    w_u32(address, xport_gte_read_data(reg));
}

static uint32 dcclip_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static uint32 dcclip_project_run(uint32 context, uint32 vertex, uint32 count,
                                  uint32 depth_register, uint32 depth_first, uint32 *minimum, uint32 *maximum)
{
    uint32 every = 65535u, near_depth = r_u16(context + 8u);
    sint32 width = (sint16)r_u16(context + 4u), height = (sint16)r_u16(context + 6u);
    *minimum = 0; *maximum = 0;
    do
    {
        uint32 flag = dcclip_flag(), mask, depth;
        sint32 x, y;
        if (depth_first) { depth = xport_gte_read_data(depth_register); dcclip_screen(vertex + 12u, 14u); }
        else { dcclip_screen(vertex + 12u, 14u); depth = xport_gte_read_data(depth_register); }
        dcclip_vector(0u, vertex + 20u);
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

static void dcclip_midpoints(uint32 context, uint32 list, uint32 pool, uint32 quad)
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
    dcclip_vector(0u, pool + 4u); xport_gte_execute(0x180001u);
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
    dcclip_project_run(context, pool, quad ? 5u : 3u, quad ? 11u : 19u, !quad, &minimum, &maximum);
}

static uint32 dcclip_uv(uint32 vertex, uint32 carrier)
{
    return (carrier & 0xFFFF0000u) | (r_u32(vertex) >> 16);
}

static void dcclip_subdivide(uint32 context, uint32 quad)
{
    uint32 list = context + 204u, pool = context + (quad ? 368u : 352u);
    uint32 current, first, second, third, fourth = 0, minimum, maximum, every;
    uint32 count = quad ? 4u : 3u, i;
    sint32 depth = 0, child;
    first = context + 304u; dcclip_vector(0u, first + 4u); xport_gte_execute(0x180001u);
    for (i = 1u; i != 3u; ++i) w_u32(list + i * 4u, first + i * 16u);
    w_u32(list, first);
    if (quad) w_u32(list + 12u, first + 48u);
    current = r_u32(context + 172u);
    w_u32(context + 176u, current); w_u32(context + 180u, current);
    first = r_u32(context + 156u);
    w_u32(context + 160u, quad ? first & 0xF7FFFFFFu : first | 0x08000000u);
    every = dcclip_project_run(context, context + 304u, count, 11u, !quad, &minimum, &maximum);
    if (every & 31u) return;
    i = (r_u16(context + 188u) + 1u) & 31u;
    if ((sint32)(maximum - (uint32)((sint32)(maximum - minimum) >> i)) < (sint32)r_u16(context + 8u)) return;
dcclip_midpoint_level:
    dcclip_midpoints(context, list, pool, quad); current = r_u32(context + 180u);
    {
        uint32 side_uv0 = r_u32(context + 164u) << 16, side_uv1 = r_u32(context + 164u);
        uint32 side_color = r_u32(context + (quad ? 160u : 156u));
        for (child = (sint32)count - 1; child >= 0; --child)
        {
            uint32 area, third_uv, mask;
            if (quad)
            {
                uint32 slot = count - 1u - (uint32)child;
                first = r_u32(list + slot * 4u); second = r_u32(list + (slot + 1u) * 4u);
                if (!child) second = r_u32(list);
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
            xport_gte_write_data(14u, r_u32(third + 12u));
            if (!quad) w_u8(current + 3u, 7u);
            third_uv = dcclip_uv(third, quad ? side_color : mask);
            xport_gte_execute(0x1400006u);
            side_uv0 = dcclip_uv(first, side_uv0); side_uv1 = dcclip_uv(second, side_uv1);
            if (quad) w_u32(current + 4u, side_color);
            else w_u32(current + 28u, third_uv);
            w_u32(current + 12u, side_uv0); w_u32(current + 20u, side_uv1);
            if (quad) w_u32(current + 28u, third_uv);
            else dcclip_link24(current, current + 32u);
            area = xport_gte_read_data(24u);
            dcclip_screen(current + 8u, 12u); dcclip_screen(current + 16u, 13u);
            if (area)
            {
                dcclip_screen(current + 24u, 14u);
                if (quad) { w_u8(current + 3u, 7u); dcclip_link24(current, current + 32u); }
                else w_u32(current + 4u, side_color);
                w_u32(context + 176u, current); current += 32u;
            }
        }
    }
    w_u32(context + 180u, current); child = 3;
dcclip_child_level:
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
            uv1 = r_u32(context + 164u); uv0 = dcclip_uv(second, uv0); uv1 = dcclip_uv(first, uv1);
            w_u32(current + 12u, uv0); w_u32(current + 20u, uv1);
            if (quad)
            {
                uv0 = dcclip_uv(third, uv0); uv1 = dcclip_uv(fourth, uv1);
                w_u32(current + 28u, uv0); area1 = xport_gte_read_data(24u);
                xport_gte_write_data(12u, r_u32(fourth + 12u)); w_u32(current + 36u, uv1);
                color = r_u32(context + 156u); uv3 = r_u32(second + 12u); xport_gte_execute(0x1400006u);
                w_u32(current + 8u, uv3); w_u32(current + 4u, color); w_u8(current + 3u, 9u);
                dcclip_link24(current, current + 40u); threshold = r_u32(context + 184u);
                area = xport_gte_read_data(24u); dcclip_screen(current + 32u, 12u);
                area = dcclip_abs(area1 - area); dcclip_screen(current + 16u, 13u); dcclip_screen(current + 24u, 14u);
            }
            else
            {
                color = r_u32(context + 156u); uv2 = dcclip_uv(third, color);
                w_u32(current + 4u, color); w_u32(current + 28u, uv2);
                area = dcclip_abs(xport_gte_read_data(24u)); dcclip_screen(current + 8u, 12u);
                threshold = r_u32(context + 184u);
            }
            descend = (sint32)threshold < (sint32)area && depth < (sint32)maximum_depth;
            w_u32(list + (quad ? 16u : 12u), first);
            if (!descend)
            {
                if (!quad)
                {
                    dcclip_screen(current + 16u, 13u); dcclip_screen(current + 24u, 14u);
                    w_u8(current + 3u, 7u); dcclip_link24(current, current + 32u);
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
            goto dcclip_midpoint_level;
        }
    }
    --depth; child = (sint8)r_u8(context + 196u + (uint32)depth);
    list -= quad ? 16u : 12u; pool -= quad ? 80u : 48u;
    if (depth >= 0) goto dcclip_child_level;
    first = r_u32(context + 168u); second = r_u32(context + 172u);
    third = r_u32(context + 180u); fourth = r_u32(context + 176u);
    if (second != third)
    {
        uint32 old = r_u32(first) & 0xFFFFFFu;
        dcclip_link24(first, second); dcclip_link24(fourth, old);
    }
}


static void dcclip_vectors_save(uint32 context, uint32 quad, uint32 compact, uint32 complete)
{
    uint32 base = compact ? 308u : 324u, step = compact ? 16u : 32u;
    if (!quad || complete) { uint32 offset = base + step * (quad ? 3u : 2u); dcclip_screen(context + offset, 0u); dcclip_screen(context + offset + 4u, 1u); }
    dcclip_screen(context + base + step * (quad ? 2u : 1u), 2u); dcclip_screen(context + base + step * (quad ? 2u : 1u) + 4u, 3u);
    dcclip_screen(context + base, 4u); dcclip_screen(context + base + 4u, 5u);
}

static uint32 dcclip_fallback(uint32 context, uint32 quad, uint32 compact)
{
    uint32 base = compact ? 308u : 324u, step = compact ? 16u : 32u;
    sint32 x, y, z;
    uint32 a, b, c, r0, r2, r4;
    dcclip_vectors_save(context, quad, compact, 0u);
    x = (sint16)r_u16(context + 56u); y = (sint16)r_u16(context + 58u); z = (sint16)r_u16(context + 60u);
    gte_ctc2((uint32)((sint16)r_u16(context + base + step) - x), 0u);
    gte_ctc2((uint32)((sint16)r_u16(context + base + step + 2u) - y), 2u);
    gte_ctc2((uint32)((sint16)r_u16(context + base + step + 4u) - z), 4u);
    xport_gte_write_data(9u, (uint32)((sint16)r_u16(context + base + step * 2u) - x));
    xport_gte_write_data(10u, (uint32)((sint16)r_u16(context + base + step * 2u + 2u) - y));
    xport_gte_write_data(11u, (uint32)((sint16)r_u16(context + base + step * 2u + 4u) - z));
    xport_gte_execute(0x170000Cu);
    a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    xport_gte_write_data(9u, (uint32)((sint16)r_u16(context + base) - x));
    xport_gte_write_data(10u, (uint32)((sint16)r_u16(context + base + 2u) - y));
    xport_gte_write_data(11u, (uint32)((sint16)r_u16(context + base + 4u) - z));
    gte_ctc2((a & 4095u) | ((b & 4095u) << 16), 16u);
    a = (uint32)((sint32)a >> 12); b = (uint32)((sint32)b >> 12);
    gte_ctc2((c & 4095u) | ((a & 4095u) << 16), 17u);
    c = (uint32)((sint32)c >> 12); a = (uint32)((sint32)a >> 12);
    gte_ctc2((b & 4095u) | ((c & 4095u) << 16), 18u);
    b = (uint32)((sint32)b >> 12); c = (uint32)((sint32)c >> 12);
    gte_ctc2((a & 65535u) | (b << 16), 19u); gte_ctc2(c, 20u);
    r0 = r_u32(context + 96u); r2 = r_u32(context + 104u);
    xport_gte_execute(0x45E012u); r4 = r_u32(context + 112u);
    gte_ctc2(r0, 0u); gte_ctc2(r2, 2u); gte_ctc2(r4, 4u);
    a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    return c + (uint32)((sint32)(b + (uint32)((sint32)a >> 12)) >> 12);
}

static uint32 dcclip_bounds(uint32 context, uint32 quad, uint32 compact, uint32 *flags)
{
    uint32 base = compact ? 316u : 332u, step = compact ? 16u : 32u, axis, i;
    for (axis = 0; axis != 4u; axis += 2u)
    {
        sint32 minimum = (sint16)r_u16(context + base + axis), maximum = minimum;
        for (i = 1u; i != (quad ? 4u : 3u); ++i)
        {
            sint32 value = (sint16)r_u16(context + base + i * step + axis);
            if (value < minimum) minimum = value;
            if (maximum < value) maximum = value;
        }
        if (maximum <= 0 || minimum >= (sint16)r_u16(context + 4u + axis)) return 0;
        *flags |= (uint32)((axis ? 511 : 1023) - (maximum - minimum));
    }
    return 1;
}

static void dcclip_call_save(uint32 context, uint32 records, uint32 remaining, uint32 packed,
                             uint32 va, uint32 vb, uint32 vc, uint32 ordering, uint32 packet)
{
    w_u32(context + 932u, records); w_u32(context + 960u, remaining); w_u32(context + 952u, packed);
    w_u32(context + 936u, va); w_u32(context + 940u, vb); w_u32(context + 944u, vc);
    w_u32(context + 168u, ordering); w_u32(context + 172u, packet);
}

static void dcclip_call_reload(uint32 context, uint32 *packet, uint32 *records, uint32 *remaining,
                               uint32 *packed, uint32 *va, uint32 *vb, uint32 *vc)
{
    *packet = r_u32(context + 180u); *records = r_u32(context + 932u);
    *remaining = r_u32(context + 960u); *packed = r_u32(context + 952u);
    *va = r_u32(context + 936u); *vb = r_u32(context + 940u); *vc = r_u32(context + 944u);
}

static void dcclip_fixed_uv(uint32 context, uint32 uv0, uint32 uv1, uint32 uv2, uint32 quad)
{
    w_u16(context + 336u, ((uv0 << 7) & 0x7F80u) | 64u);
    w_u16(context + 338u, ((uv0 >> 1) & 0x7F80u) | 64u);
    w_u16(context + 368u, ((uv1 << 7) & 0x7F80u) | 64u);
    w_u16(context + 370u, ((uv1 >> 1) & 0x7F80u) | 64u);
    if (quad) w_u16(context + 432u, (((uv2 >> 16) << 7) & 0x7F80u) | 64u);
    w_u16(context + 400u, ((uv2 << 7) & 0x7F80u) | 64u);
    w_u16(context + 402u, ((uv2 >> 1) & 0x7F80u) | 64u);
    if (quad) w_u16(context + 434u, ((uv2 >> 17) & 0x7F80u) | 64u);
}

uint32 gtmain_model_depth_cued_clipped_draw(uint32 context, uint32 model_header)
{
    static const uint32 stride[8] = {12u, 12u, 20u, 24u, 24u, 24u, 32u, 36u};
    static const uint32 size[8] = {20u, 24u, 28u, 36u, 32u, 40u, 40u, 52u};
    static const uint32 tag[8] = {4u, 5u, 6u, 8u, 7u, 9u, 9u, 12u};
    static const uint32 prefix[4] = {12u, 16u, 16u, 20u};
    uint32 group, header = model_header;
    for (group = 0; group != 8u; ++group)
    {
        uint32 remaining = r_u16(header + 48u + group * 2u), records = r_u32(context + 932u);
        uint32 packet, packed, vertices, va, vb, vc;
        if (!remaining) continue;
        packet = r_u32(context + 928u); packed = r_u32(records); vertices = r_u32(context + 916u);
        va = vertices + (packed & 1023u) * 8u; vb = vertices + ((packed >> 7) & 8184u);
        vc = vertices + ((packed >> 17) & 8184u);
        do
        {
            uint32 quad = group & 1u, compact = group == 4u || group == 5u;
            uint32 base = compact ? 308u : 324u, step = compact ? 16u : 32u;
            uint32 first = packed, second, disable_fog, next_a, next_b, next_c;
            uint32 flag, minimum, maximum, depth1, depth2, depth3, area, area2 = 0;
            uint32 cull, shift, bucket, ordering, fog, uv0 = 0, uv1 = 0, uv2 = 0;
            if (quad)
            {
                uint32 xy, z, fourth;
                dcclip_vector(0u, vb); dcclip_vector(2u, vc);
                dcclip_screen(context + base + step, 0u); dcclip_screen(context + base + step + 4u, 1u);
                dcclip_vector(4u, va); vertices = r_u32(context + 916u);
                xport_gte_execute(0x280030u);
                second = r_u32(records + 4u); packed = r_u32(records + stride[group]);
                fourth = vertices + (second & 1023u) * 8u; xy = r_u32(fourth); z = r_u32(fourth + 4u);
                dcclip_screen(context + base + step + 8u, 12u); dcclip_screen(packet + prefix[group / 2u], 12u);
                flag = dcclip_flag(); --remaining; xport_gte_execute(0x1400006u);
                next_a = vertices + (packed & 1023u) * 8u; next_b = vertices + ((packed >> 7) & 8184u);
                xport_gte_write_data(0u, xy); xport_gte_write_data(1u, z);
                area = 0u - xport_gte_read_data(24u); minimum = xport_gte_read_data(17u);
                depth1 = xport_gte_read_data(18u); depth2 = xport_gte_read_data(19u);
                disable_fog = second & 1024u; xport_gte_execute(0x180001u);
                next_c = vertices + ((packed >> 17) & 8184u); maximum = minimum;
                if ((sint32)depth1 < (sint32)minimum) minimum = depth1;
                if ((sint32)maximum < (sint32)depth1) maximum = depth1;
                if ((sint32)depth2 < (sint32)minimum) minimum = depth2;
                if ((sint32)maximum < (sint32)depth2) maximum = depth2;
                flag |= dcclip_flag(); depth3 = xport_gte_read_data(19u); shift = r_u16(context + 40u);
                xport_gte_execute(0x1400006u);
                if ((sint32)maximum < (sint32)depth3) maximum = depth3;
                if ((sint32)depth3 < (sint32)minimum) minimum = depth3;
                if ((sint32)maximum < (sint32)r_u16(context + 8u)) goto dcclip_next;
                area2 = xport_gte_read_data(24u); dcclip_screen(context + base + step * 2u + 8u, 12u);
                cull = area & area2 & first;
            }
            else
            {
                dcclip_vector(0u, vc); dcclip_vector(2u, vb); dcclip_vector(4u, va); --remaining;
                xport_gte_execute(0x280030u);
                packed = r_u32(records + stride[group]); second = r_u32(records + 4u);
                vertices = r_u32(context + 916u); next_a = vertices + (packed & 1023u) * 8u;
                next_b = vertices + ((packed >> 7) & 8184u); next_c = vertices + ((packed >> 17) & 8184u);
                flag = dcclip_flag(); minimum = xport_gte_read_data(17u);
                depth1 = xport_gte_read_data(18u); depth2 = xport_gte_read_data(19u);
                xport_gte_execute(0x1400006u); maximum = minimum;
                if ((sint32)depth1 < (sint32)minimum) minimum = depth1;
                if ((sint32)maximum < (sint32)depth1) maximum = depth1;
                if ((sint32)depth2 < (sint32)minimum) minimum = depth2;
                if ((sint32)maximum < (sint32)depth2) maximum = depth2;
                disable_fog = second & 1024u; area = xport_gte_read_data(24u);
                dcclip_screen(context + base + step * 2u + 8u, 12u);
                if ((sint32)maximum < (sint32)r_u16(context + 8u)) goto dcclip_next;
                cull = area & first; shift = r_u16(context + 40u);
            }
            if ((sint32)flag < 0)
            {
                cull = 0;
                if ((sint32)first < 0) cull = dcclip_fallback(context, quad, compact);
            }
            dcclip_screen(context + base + (quad ? 0u : step) + 8u, 13u);
            if ((sint32)cull < 0) goto dcclip_next;
            bucket = ((first & 0x40000000u) ? maximum : minimum) << (shift & 31u);
            bucket = (uint32)((sint32)bucket >> 13);
            dcclip_screen(context + base + (quad ? 3u : 0u) * step + 8u, 14u);
            if ((sint32)bucket >= 4096) bucket = 4095u;
            fog = disable_fog ? 0u : r_u16(r_u32(context + 920u) + bucket * 2u);
            if (group == 2u || group == 6u)
            {
                uint32 color = r_u32(records + 8u), offset = group == 2u ? 12u : 24u;
                xport_gte_write_data(21u, r_u32(records + offset)); xport_gte_write_data(22u, r_u32(records + offset + 4u));
                xport_gte_write_data(6u, color); xport_gte_write_data(20u, color); xport_gte_write_data(8u, fog);
                xport_gte_execute(0xF8002Au);
            }
            else
            {
                xport_gte_write_data(6u, r_u32(records + 8u));
                xport_gte_write_data(8u, fog); xport_gte_execute(0x780010u);
                if (group == 3u || group == 7u)
                {
                    uint32 offset = group == 3u ? 12u : 24u;
                    uint32 c1 = r_u32(records + offset), c2 = r_u32(records + offset + 4u), c3 = r_u32(records + offset + 8u);
                    uint32 color = xport_gte_read_data(22u);
                    xport_gte_write_data(20u, c1); xport_gte_write_data(21u, c2); xport_gte_write_data(22u, c3);
                    w_u32(context + 156u, color); w_u32(context + 340u, color);
                    xport_gte_execute(0xF8002Au); w_u32(packet + 4u, color);
                }
            }
            ordering = r_u32(context + 924u) + 4u * (bucket + ((uint32)((sint32)(first << 1) >> 31) & 96u));
            if (!dcclip_bounds(context, quad, compact, &flag)) goto dcclip_next;
            if (group >= 4u) { uv0 = r_u32(records + 12u); uv1 = r_u32(records + 16u); uv2 = r_u32(records + 20u); }
            if (compact && (sint32)(quad ? dcclip_abs(area) + dcclip_abs(area2) : dcclip_abs(area)) >= (quad ? 2048 : 800)) flag = 0xFFFFFFFFu;
            if ((sint32)flag < 0)
            {
                dcclip_call_save(context, records, remaining, packed, next_a, next_b, next_c, ordering, packet);
                if (compact)
                {
                    w_u16(context + 188u, 5u); w_u32(context + 184u, quad ? 2048u : 1152u);
                    w_u16(context + 306u, uv0); w_u16(context + 322u, uv1); w_u16(context + 338u, uv2);
                    if (quad) w_u32(context + 352u, uv2);
                    dcclip_screen(context + 156u, 22u); w_u32(context + 164u, uv1);
                    w_u8(context + 164u, uv0 >> 16); w_u8(context + 165u, uv0 >> 24);
                    dcclip_vectors_save(context, quad, compact, 1u); dcclip_subdivide(context, quad);
                }
                else
                {
                    w_u16(context + 184u, quad ? 4u : 3u);
                    if (group >= 6u)
                    {
                        w_u32(context + 164u, uv1); w_u8(context + 164u, uv0 >> 16); w_u8(context + 165u, uv0 >> 24);
                        dcclip_fixed_uv(context, uv0, uv1, uv2, quad);
                    }
                    if (group == 0u || group == 1u) dcclip_screen(context + 156u, 22u);
                    else if (!quad)
                    {
                        dcclip_screen(context + 156u, 20u); dcclip_screen(context + 340u, 20u);
                        dcclip_screen(context + 372u, 21u); dcclip_screen(context + 404u, 22u);
                    }
                    else
                    {
                        dcclip_screen(context + 372u, 20u); dcclip_screen(context + 404u, 21u); dcclip_screen(context + 436u, 22u);
                    }
                    dcclip_vectors_save(context, quad, compact, 1u);
                    if (group >= 6u) gtmain_polygon_clip_textured_gouraud(context);
                    else if (group >= 2u) gtmain_polygon_clip_gouraud(context);
                    else gtmain_polygon_clip_flat(context);
                }
                dcclip_call_reload(context, &packet, &records, &remaining, &packed, &next_a, &next_b, &next_c);
                goto dcclip_next;
            }
            w_u32(packet, tag[group] << 24); dcclip_link(ordering, packet);
            if (group >= 4u)
            {
                w_u32(packet + 12u, uv0); w_u32(packet + (group >= 6u ? 24u : 20u), uv1);
                w_u32(packet + (group == 4u ? 28u : group == 5u || group == 6u ? 36u : 48u), uv2);
                if (quad) { uint32 offset = group == 5u ? 28u : 36u; w_u8(packet + offset, uv2 >> 16); w_u8(packet + offset + 1u, uv2 >> 24); }
            }
            if (quad)
            {
                dcclip_screen(packet + (group == 1u ? 20u : group == 3u || group == 5u ? 32u : 44u), 12u);
                dcclip_screen(packet + 8u, 13u); dcclip_screen(packet + (group == 1u ? 16u : group == 3u || group == 5u ? 24u : 32u), 14u);
            }
            else
            {
                dcclip_screen(packet + (group == 0u ? 16u : group == 2u || group == 4u ? 24u : 32u), 12u);
                dcclip_screen(packet + (group == 0u ? 12u : group == 2u || group == 4u ? 16u : 20u), 13u);
                dcclip_screen(packet + 8u, 14u);
            }
            if (group == 2u || group == 3u || group >= 6u)
            {
                if (!quad) dcclip_screen(packet + 4u, 20u);
                dcclip_screen(packet + (group == 2u || group == 3u ? 12u : 16u), quad ? 20u : 21u);
                dcclip_screen(packet + (group == 2u || group == 3u ? 20u : 28u), 22u);
                if (quad) dcclip_screen(packet + (group == 3u ? 28u : 40u), 21u);
            }
            else dcclip_screen(packet + 4u, 22u);
            packet += size[group];
dcclip_next:
            records += stride[group]; va = next_a; vb = next_b; vc = next_c;
        }
        while (remaining);
        header = r_u32(context + 912u); w_u32(context + 932u, records); w_u32(context + 928u, packet);
    }
    return header;
}
