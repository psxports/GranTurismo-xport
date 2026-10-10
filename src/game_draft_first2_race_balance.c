#include "game_draft_registry.h"
#include "game_first2_registry.h"

static sint32 gtmain_balance_multiply(sint32 left, sint32 right)
{
    return (sint32)((uint32)left * (uint32)right);
}

static sint32 gtmain_balance_shift(sint32 value)
{
    if (value < 0) value = (sint32)((uint32)value + 4095u);
    return value >> 12;
}

static sint32 gtmain_balance_fraction(sint32 numerator, sint32 denominator, uint32 caller_link)
{
    sint64 dividend = (sint64)numerator * 4096;
    return (sint32)(uint32)gtmain_signed_divide64(dividend, (sint64)denominator, caller_link);
}

/* GTMAIN.EXE:800617E8 */
sint32 gtmain_race_speed_percent(sint32 percent)
{
    sint32 value = (sint32)((uint32)percent << 12u) / 100;
    w_u32(0x80093EA8u, (uint32)value);
    return value;
}

/* GTMAIN.EXE:80061814 */
sint32 gtmain_race_balance_config(uint32 enabled)
{
    sint32 value;
    if (enabled) {
        uint32 boost = r_u8(0x80093C08u), slow = r_u8(0x80093C63u), gain = r_u8(0x80093C66u);
        uint32 slow_limit = 655360u * r_u8(0x80093C65u), boost_limit = 655360u * r_u8(0x80093C68u);
        uint32 first = r_u8(0x80093C09u), second = r_u8(0x80093C0Au);
        sint32 base = (sint32)r_u32(0x80093EA8u);
        uint32 scale = r_u8(0x80093C60u);
        w_u32(0x80093EB0u, first << 16u);
        w_u32(0x80093EB4u, second << 16u);
        w_u32(0x80093EBCu, (uint32)r_u8(0x80093C64u) << 16u);
        w_u32(0x80093EC0u, slow_limit);
        w_u32(0x80093ECCu, boost_limit);
        w_u32(0x80093EC8u, (uint32)r_u8(0x80093C67u) << 16u);
        w_u32(0x80093EACu, (boost << 12u) / 100u);
        w_u32(0x80093EB8u, (slow << 12u) / 100u);
        w_u32(0x80093EC4u, (gain << 12u) / 100u);
        value = gtmain_balance_multiply((sint32)scale, base) / 100;
    } else {
        value = (sint32)r_u32(0x80093EA8u);
        w_u32(0x80093EB0u, 0u); w_u32(0x80093EB4u, 0u); w_u32(0x80093EACu, 0u);
        w_u32(0x80093EBCu, 0u); w_u32(0x80093EC0u, 0u); w_u32(0x80093EB8u, 0u);
        w_u32(0x80093EC8u, 0u); w_u32(0x80093ECCu, 0u); w_u32(0x80093EC4u, 0u);
    }
    w_u32(0x80093ED0u, (uint32)value);
    return value;
}

/* GTMAIN.EXE:80061B20 */
sint32 gtmain_race_balance_boost(uint32 car, sint32 lap_delta, sint32 distance_delta)
{
    sint32 value = 4096;
    if (!(r_u8(car + 58u) & 16u)) {
        sint32 limit = (sint32)r_u32(0x80093ECCu);
        if (lap_delta > 0 || distance_delta >= limit)
            value = (sint32)(r_u32(0x80093EC4u) + 4096u);
        else {
            sint32 start = (sint32)r_u32(0x80093EC8u);
            if (start < distance_delta) {
                sint32 fraction = gtmain_balance_fraction((sint32)((uint32)distance_delta - (uint32)start),
                    (sint32)((uint32)limit - (uint32)start), 0x80061BC0u);
                value = (sint32)((uint32)gtmain_balance_shift(gtmain_balance_multiply(fraction,
                    (sint32)r_u32(0x80093EC4u))) + 4096u);
            }
        }
    }
    w_u16(car + 1702u, (uint32)value);
    return value;
}

/* GTMAIN.EXE:80061C04 */
sint32 gtmain_race_balance_slow(uint32 car, sint32 lap_delta, sint32 distance_delta)
{
    sint32 limit = (sint32)r_u32(0x80093EC0u), reduction, value;
    if (lap_delta > 0 || distance_delta >= limit) reduction = (sint32)r_u32(0x80093EB8u);
    else {
        sint32 start = (sint32)r_u32(0x80093EBCu);
        reduction = 0;
        if (start < distance_delta) {
            sint32 fraction = gtmain_balance_fraction((sint32)((uint32)distance_delta - (uint32)start),
                (sint32)((uint32)limit - (uint32)start), 0x80061C8Cu);
            reduction = gtmain_balance_shift(gtmain_balance_multiply(fraction, (sint32)r_u32(0x80093EB8u)));
        }
    }
    value = (sint32)(4096u - (uint32)reduction);
    w_u16(car + 1702u, (uint32)value);
    return value;
}

/* GTMAIN.EXE:80065194 */
sint32 gtmain_race_position_delta(uint32 first_car, uint32 second_car,
    sint32 *lap_delta, sint32 *distance_delta)
{
    sint32 first_lap = (sint16)r_u16(first_car + 56u), second_lap = (sint16)r_u16(second_car + 56u);
    sint32 result, laps, distance;
    if (r_u8(first_car + 58u) & 2u) --first_lap;
    if (r_u8(second_car + 58u) & 2u) --second_lap;
    result = (sint32)r_u32(second_car + 1804u);
    laps = first_lap - second_lap;
    distance = (sint32)(r_u32(first_car + 1804u) - (uint32)result);
    if (laps > 0 && distance < 0) {
        result = (sint32)r_u32(r_u32(0x800BC1A8u));
        --laps; distance = (sint32)((uint32)distance + (uint32)result);
    } else if (laps < 0 && distance > 0) {
        result = (sint32)r_u32(r_u32(0x800BC1A8u));
        ++laps; distance = (sint32)((uint32)distance - (uint32)result);
    }
    *lap_delta = laps; *distance_delta = distance;
    return result;
}

/* GTMAIN.EXE:80061CD4 */
uint32 gtmain_race_balance_update(uint32 reference_car, uint32 car)
{
    sint32 laps, distance, ahead;
    uint32 mode, result = r_u8(car + 1960u);
    if (result) return result;
    gtmain_race_position_delta(reference_car, car, &laps, &distance);
    ahead = laps > 0 || (!laps && distance > 0);
    mode = r_u8(0x80093C62u);
    if (mode > 2u) return 2u;
    if (!r_u32(ahead ? 0x80093EC4u : 0x80093EB8u)) {
        result = r_u16(0x80093ED0u);
        if (mode == 1u) { w_u16(car + 1704u, 4096u); w_u16(car + 1702u, result); return 4096u; }
        if (mode == 2u) w_u16(car + 1704u, 0u);
        w_u16(car + 1702u, result);
        return result;
    }
    if (ahead) gtmain_race_balance_boost(car, laps, distance);
    else gtmain_race_balance_slow(car, (sint32)(0u - (uint32)laps), (sint32)(0u - (uint32)distance));
    if (!mode) {
        sint32 value = gtmain_balance_shift(gtmain_balance_multiply((sint16)r_u16(car + 1702u),
            (sint32)r_u32(0x80093ED0u)));
        w_u16(car + 1702u, (uint32)value);
        return (uint32)value;
    }
    if (mode == 1u) {
        result = r_u16(car + 1702u);
        w_u16(car + 1704u, result);
        w_u16(car + 1702u, r_u16(0x80093ED0u));
        return result;
    }
    {
        sint64 product = (sint64)((sint16)r_u16(car + 1702u) - 4096) * 113800;
        if (product < 0) product += 4095;
        result = r_u16(0x80093ED0u);
        w_u16(car + 1704u, (uint32)(product >> 12));
        w_u16(car + 1702u, result);
        return result;
    }
}

/* GTMAIN.EXE:80061F78 */
sint32 gtmain_race_speed_apply(uint32 car)
{
    sint32 scale = (sint16)r_u16(car + 1702u), first, second;
    if (scale == 4096) {
        second = (sint32)(r_u32(0x800BF518u) << 12u);
        first = (sint32)r_u32(0x800BF514u);
    } else {
        first = gtmain_balance_shift(gtmain_balance_multiply(scale, (sint32)r_u32(0x800BF514u)));
        scale = (sint16)r_u16(car + 1702u);
        second = (sint32)(r_u32(0x800BF518u) << 24u);
        if (!scale) { gt_draft_exception(7u); return 0; }
        if (scale == -1 && second == (sint32)0x80000000u) { gt_draft_exception(6u); return 0; }
        second /= scale;
    }
    w_u32(car + 4u, (uint32)first); w_u32(car + 8u, (uint32)second);
    return second;
}

/* GTMAIN.EXE:80065230 */
sint32 gtmain_race_position_compare(uint32 cars, sint32 first, sint32 second)
{
    uint32 a = cars + 3380u * (uint32)first + 40u, b;
    sint32 first_lap, second_lap, first_distance, second_distance;
    if (r_u8(a + 1960u)) return 0;
    b = cars + 3380u * (uint32)second + 40u;
    first_lap = (sint16)r_u16(a + 56u); second_lap = (sint16)r_u16(b + 56u);
    if (r_u8(a + 58u) & 2u) --first_lap;
    if (r_u8(b + 58u) & 2u) --second_lap;
    if (second_lap < first_lap) return 1;
    if (first_lap < second_lap) return -1;
    first_distance = (sint32)r_u32(a + 1804u); second_distance = (sint32)r_u32(b + 1804u);
    if (second_distance < first_distance) return 1;
    return -(first_distance < second_distance);
}

/* GTMAIN.EXE:80065300 */
sint32 gtmain_race_positions_init(uint32 cars, sint32 count)
{
    sint32 i;
    for (i = 0; i < 6; ++i) w_u8(0x800BFE40u + (uint32)i, 255u);
    for (i = 0; i < count; ++i) {
        w_u8(cars + 3380u * (uint32)i + 1737u, (uint32)i + 1u);
        w_u8(0x800BFE40u + (uint32)i, (uint32)i);
    }
    return count > 0 ? (sint32)((uint32)count << 1u) : 0;
}

/* GTMAIN.EXE:80065380 */
sint32 gtmain_race_positions_sort(uint32 cars, sint32 count)
{
    sint32 i;
    for (i = 1; i < count; ++i) {
        sint32 selected = (sint8)r_u8(0x800BFE40u + (uint32)i), previous = i - 1;
        if (r_u8(cars + 3380u * (uint32)selected + 2000u)) continue;
        while (previous >= 0) {
            sint32 candidate = (sint8)r_u8(0x800BFE40u + (uint32)previous);
            if (gtmain_race_position_compare(cars, candidate, selected) >= 0) break;
            w_u8(0x800BFE41u + (uint32)previous, r_u8(0x800BFE40u + (uint32)previous));
            --previous;
        }
        w_u8(0x800BFE41u + (uint32)previous, (uint32)selected);
    }
    for (i = 0; i < count; ++i) {
        sint32 selected = (sint8)r_u8(0x800BFE40u + (uint32)i);
        w_u8(cars + 3380u * (uint32)selected + 1737u, (uint32)i + 1u);
    }
    return 0;
}
