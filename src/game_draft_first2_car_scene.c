#include "game_draft_registry.h"
#include "game_first2_registry.h"

static uint32 car_scene_distance(uint32 x, uint32 y, uint32 z)
{
    uint32 swap;
    if ((sint32)x < 0) x = 0u - x;
    if ((sint32)y < 0) y = 0u - y;
    if ((sint32)z < 0) z = 0u - z;
    if ((sint32)x < (sint32)y) { swap = x; x = y; y = swap; }
    if ((sint32)x < (sint32)z) { swap = x; x = z; z = swap; }
    if ((sint32)y < (sint32)z) { swap = y; y = z; z = swap; }
    return x + (uint32)((sint32)y >> 1) + (uint32)((sint32)z >> 2);
}



static void car_scene_copy40(uint32 destination, uint32 source)
{
    uint32 block, a, b, c, d;
    for (block = 0; block < 32u; block += 16u)
    {
        a = r_u32(source + block); b = r_u32(source + block + 4u);
        c = r_u32(source + block + 8u); d = r_u32(source + block + 12u);
        w_u32(destination + block, a); w_u32(destination + block + 4u, b);
        w_u32(destination + block + 8u, c); w_u32(destination + block + 12u, d);
    }
    a = r_u32(source + 32u); b = r_u32(source + 36u);
    w_u32(destination + 32u, a); w_u32(destination + 36u, b);
}

/* Original: GTMAIN.EXE:sub_80021154. */ uint32 gtmain_race_rank_list_sort(void)
{
    sint32 index, count = (sint16)r_u16(0x800B6666u);
    uint32 result, current;
    w_u32(0x8008F3E4u, 0);
    for (index = 0; index < count; ++index)
    {
        uint32 car = 0x800B6850u + (uint32)index * 3380u;
        uint32 next = r_u32(0x8008F3E4u), previous = 0, time = r_u32(car + 32u);
        sint32 rank = (sint8)r_u8(car + 1737u);
        while (next)
        {
            uint32 other = r_u32(next + 32u);
            if (time < other || (time == other && time == 0xFFFFFFFFu && rank < (sint8)r_u8(next + 1737u))) break;
            previous = next; next = r_u32(next + 4u);
        }
        w_u32(car + 4u, next);
        if (previous) w_u32(previous + 4u, car); else w_u32(0x8008F3E4u, car);
    }
    result = r_u32(0x800B630Cu);
    if (!result)
    {
        current = r_u32(0x8008F3E4u); index = 0;
        while (current)
        {
            result = (uint32)(sint32)(sint16)r_u16(current + 16u);
            w_u8(0x800B6660u + result, index); w_u16(current + 36u, index);
            current = r_u32(current + 4u); ++index;
        }
    }
    return result;
}

/* Original: GTMAIN.EXE:sub_80021290. */ void gtmain_race_player_positions(void)
{
    uint32 car = r_u32(0x8008F3E4u), index = 0;
    while (car)
    {
        sint32 mode = (sint16)r_u16(car + 26u);
        if (mode == 1) w_u8(0x800B63D1u, index);
        else if (mode == 2) w_u8(0x800B63D4u, index);
        car = r_u32(car + 4u); ++index;
    }
}

/* Original: GTMAIN.EXE:sub_80021300. */ uint32 gtmain_race_player_positions_latch(void)
{
    uint32 result = r_u32(0x800B630Cu);
    if (!result)
    {
        uint32 first = r_u8(0x800B63D1u), second = r_u8(0x800B63D4u);
        result = 0x800B63D2u;
        w_u8(result, first); w_u8(result + 3u, second);
    }
    return result;
}

/* Original: GTMAIN.EXE:sub_8002180C. */ uint32 gtmain_car_distance_insert(uint32 car)
{
    uint32 driver = r_u32(0x800B6638u), x, y, z, distance, divisor, numerator = 0x80000u, small = 0, next, previous = 0, gain, result;
    x = r_u32(car + 2228u) - r_u32(driver + 120u); w_u32(car + 2208u, x);
    y = r_u32(car + 2232u) - r_u32(driver + 124u);
    z = r_u32(car + 2236u); w_u32(car + 2212u, y);
    z -= r_u32(driver + 128u); w_u32(car + 2216u, z);
    distance = car_scene_distance(x, y, z); divisor = (distance >> 16) + 1u;
    if (!r_u32(0x800B630Cu))
    {
        sint32 mode = (sint16)r_u16(car + 26u);
        if (mode >= 3 || mode <= 0) { numerator = 0x18000u; small = 1; }
    }
    next = r_u32(0x8008F370u); w_u32(car + 2200u, distance);
    while (next)
    {
        if (distance < r_u32(next + 2200u)) break;
        previous = next; next = r_u32(next);
    }
    if (previous) w_u32(previous, car); else w_u32(0x8008F370u, car);
    gain = numerator / divisor; w_u32(car, next);
    if (gain >= 0x4001u) gain = 0x4000u;
    result = gain < 0x400u;
    if (small && result) gain = 0;
    w_u32(car + 2204u, gain);
    return result;
}

/* Original: GTMAIN.EXE:sub_80021988. */ uint32 gtmain_car_render_defaults(uint32 context)
{
    w_u16(context + 910u, 6); w_u32(context + 916u, 0x267FD3u);
    w_u16(context + 920u, 64); w_u16(context + 922u, 64);
    return 64;
}

/* Original: GTMAIN.EXE:sub_800219AC. */ sint32 gtmain_car_model_draw(uint32 context, uint32 car, uint32 mode)
{
    sint32 fraction = (sint32)(uint32)gtmain_signed_divide64((sint64)(sint32)r_u32(car + 2008u) * 4096, 600000, 0x800219FCu);
    uint32 model, tile, value, classification, distance, detail, wheel;
    uint32 camera_x, model_x, camera_z, model_z, camera_y, model_y;
    if (fraction >= 4096) fraction = 4096;
    w_u16(context + 924u, fraction); model = r_u32(car + 2336u);
    car_scene_copy40(context + 864u, car + 2228u);
    tile = r_u16(0x800B6674u + (uint32)(sint32)(sint16)r_u16(car + 16u) * 76u);
    value = r_u8(car + 217u);
    if (!value) tile += 512u; else if (value == 2u) tile += 256u;
    w_u32(context + 904u, tile << 16);
    classification = gtmain_model_transform_draw(context, model + 60u, mode);
    if ((sint32)classification < 0) return (sint32)classification;
    if (r_u16(car + 60u))
    {
        gtmain_packet_three_tiles(r_u32(0x800B63F8u), tile, 0x1FF0120u);
        gtmain_packet_three_tiles(r_u32(0x800B6308u), tile, 0x1FF0120u);
    }
    camera_x = r_u32(context + 44u); model_x = r_u32(context + 864u);
    camera_z = r_u32(context + 52u); model_z = r_u32(context + 872u);
    camera_y = r_u32(context + 48u); model_y = r_u32(context + 868u);
    distance = car_scene_distance(model_x - camera_x, model_y - camera_y, model_z - camera_z);
    fraction = (sint32)(uint32)gtmain_signed_divide64((sint64)(sint32)(12u * distance), (sint64)(sint32)xport_load_le32(gtmain_render_source(context) + 24u), 0x80021BE4u);
    if (fraction <= 0x3FFFFF)
    {
        if (fraction > 0x200000) detail = 4; else if (fraction > 0x100000) detail = 3;
        else if (fraction > 0x80000) detail = 2; else detail = fraction > 0x40000;
        w_u16(context + 936u, tile); w_u16(context + 938u, r_u16(car + 2342u));
        for (wheel = 0; wheel < 4; ++wheel)
        {
            sint32 angle = 0, rotation, contact, position_x, position_y, position_z;
            uint32 record = car + wheel * 24u, a, b, c;
            if (wheel & 2u) rotation = (sint16)r_u16(car + 1498u);
            else { angle = -(sint16)r_u16(car + 54u); rotation = (sint16)r_u16(car + 1496u); }
            contact = (sint16)r_u16(car + wheel * 68u + 178u);
            if (wheel & 1u) { angle -= 1024; contact = -contact; } else angle += 1024;
            gtmain_model_rotation(psx_addr(context + 960u, 18u), (sint16)angle, (sint16)rotation, (sint16)contact);
            gtmain_model_wheel_matrix(context, (sint16)r_u16(record + 2046u));
            position_x = (sint16)r_u16(record + 2036u);
            position_y = (sint16)r_u16(record + 2038u);
            position_z = (sint16)r_u16(record + 2040u);
            gtmain_model_position(context, position_x, position_y, position_z);
            a = r_u32(context + 940u); b = r_u32(context + 944u); c = r_u32(context + 948u);
            w_u32(record + 2024u, a); w_u32(record + 2028u, b); w_u32(record + 2032u, c);
            gtmain_model_wheel_draw(context, (sint32)((uint32)(sint32)(sint16)r_u16(record + 2044u) << 4), detail);
        }
    }
    if (classification < 2u)
    {
        car_scene_copy40(context + 864u, car + 2268u);
        gtmain_model_shadow_draw(context, r_u32(model + 56u));
    }
    return (sint32)classification;
}

/* Original: GTMAIN.EXE:sub_80021DA8. */ uint32 gtmain_race_cars_draw(uint32 context)
{
    sint32 index = 0, count; uint32 car = 0x800B6850u, result = 0x10000000u, links;
    w_u32(0x8008F370u, 0); gtmain_car_render_defaults(context);
    links = r_u32(0x800B6338u); count = (sint16)r_u16(0x800BC1A6u);
    w_u16(context + 908u, 0); w_u32(context + 912u, result); w_u32(context + 928u, links);
    while (index < count)
    {
        uint32 draw = r_u32(car + 12u) & 1u;
        if (draw && r_u16(car + 26u) == 3u)
        {
            if (r_u16(0x800B63DEu)) draw = 0;
            else
            {
                uint32 alternate = r_u16(0x8008F3ECu);
                w_u16(0x8008F3ECu, alternate + 1u);
                if (!(alternate & 1u) || r_u8(0x800CB764u)) draw = 0;
            }
        }
        if (draw)
        {
            gtmain_car_distance_insert(car); w_u16(car + 2224u, gtmain_driver_assist(car));
            if (!gtmain_driver_hidden(car)) gtmain_car_model_draw(context, car, !!gtmain_driver_is_primary(car));
        }
        ++index; count = (sint16)r_u16(0x800BC1A6u); result = index < count; car += 3380u;
    }
    return result;
}

/* Original: GTMAIN.EXE:sub_80021EE4. */ sint32 gtmain_race_cars_secondary_draw(uint32 context, uint32 links)
{
    uint32 car = r_u32(0x8008F370u), index = 0; sint32 result = -1;
    w_u32(context + 928u, links + 16u); w_u16(context + 908u, 0xFFFFu); w_u32(context + 912u, 0);
    while (car)
    {
        if (index)
        {
            if (r_u32(car + 2200u) > 0x95FFFFu) return 1;
            result = gtmain_car_model_draw(context, car, 2);
        }
        car = r_u32(car); ++index;
    }
    return result;
}

/* Original: GTMAIN.EXE:sub_80021F80. */ uint32 gtmain_race_car_by_rank(sint32 rank)
{
    sint32 count = (sint16)r_u16(0x800BC1A6u), index;
    if (rank <= 0) rank = count; else if (count < rank) rank = 1;
    for (index = 0; index < count; ++index)
    {
        uint32 car = 0x800B6850u + (uint32)index * 3380u;
        if (r_u16(car + 26u) != 3u && rank == (sint8)r_u8(car + 1737u)) return car;
    }
    return 0;
}
