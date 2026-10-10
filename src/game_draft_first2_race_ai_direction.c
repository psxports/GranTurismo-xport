#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* TODO Bind remaining original spline and course dependencies */
static uint32 direction_call(uint32 owner, uint32 a, uint32 b, uint32 c)
{
    return gt_draft_unresolved_call(owner, a, b, c, 0, 0, 0, 0, 0);
}

/* TODO Bind unrecorded behavior mode and pair cache dependencies */
static sint32 direction_behavior_mode3(uint32 mode)
{
    return (sint32)gt_draft_unresolved_call(0x80053C68u, mode, 0, 0, 0, 0, 0, 0, 0);
}
static sint32 direction_behavior_mode4(uint32 mode)
{
    return (sint32)gt_draft_unresolved_call(0x80053CA0u, mode, 0, 0, 0, 0, 0, 0, 0);
}
static uint32 direction_pair_cache_clear(sint32 car_index, sint32 count)
{
    return gt_draft_unresolved_call(0x80062138u, (uint32)car_index, (uint32)count, 0, 0, 0, 0, 0, 0);
}

static uint32 direction_fp(uint32 a, uint32 b)
{
    return (uint32)((sint32)(a * b) / 4096);
}

static sint32 direction_div(uint32 numerator, sint32 denominator)
{
    if (!denominator) gt_draft_exception(7);
    if (denominator == -1 && numerator == 0x80000000u) gt_draft_exception(6);
    return (sint32)numerator / denominator;
}

static void direction_spline(uint32 owner, uint32 mode, uint32 segment, uint32 progress, sint32 *x, sint32 *z)
{
    if (owner == 0x80054164u) { gtmain_race_ai_spline_position((sint32)mode, (sint32)segment, (sint32)progress, x, z); return; }
    /* TODO Bind the unselected alternate spline native outputs */
    (void)mode; (void)segment; (void)progress; (void)x; (void)z;
    gt_draft_failure("GTMAIN native AI spline outputs", owner);
}

/* GTMAIN.EXE 80055C0C */
void gtmain_race_ai_direction(uint32 car, sint32 distance, sint32 *first, sint32 *second)
{
    uint32 mode = r_u8(car + 1912u), segment = r_u8(car + 1913u), attempt = 0;
    sint32 x, z, dx, dz, absolute_x, absolute_z, minimum;
    do {
        direction_spline(mode == 4u ? 0x800546BCu : 0x80054164u, mode, segment,
            (uint32)distance + r_u32(car + 1804u), &x, &z);
        x = (sint32)((uint32)x - r_u32(car + 60u)); z = (sint32)((uint32)z - r_u32(car + 64u));
        dx = (sint32)(0u - (uint32)x); dz = (sint32)(0u - (uint32)z);
        absolute_x = dx < 0 ? x : dx; absolute_z = dz < 0 ? (sint32)(0u - (uint32)dz) : dz;
        minimum = absolute_x < absolute_z ? absolute_x : absolute_z;
        if ((sint32)((uint32)absolute_x + (uint32)absolute_z - (uint32)(minimum >> 1)) >= 20480) break;
        if (attempt < 9u) distance = (sint32)((uint32)distance + 65536u);
        ++attempt;
    } while (attempt < 10u);
    *first = x; *second = z;
}

/* GTMAIN.EXE 80053050 */
sint32 gtmain_race_ai_distance(uint32 parameters, sint32 speed)
{
    sint64 product;
    sint32 weight, difference;
    if (speed <= 0) return r_s32(parameters + 16u);
    product = (sint64)(sint16)r_u16(parameters + 14u) * speed;
    weight = (sint32)(uint32)(product / 4096);
    if (weight >= 4096) return r_s32(parameters + 20u);
    difference = (sint32)(r_u32(parameters + 20u) - r_u32(parameters + 16u));
    product = (sint64)weight * difference;
    return (sint32)(r_u32(parameters + 16u) + (uint32)(product / 4096));
}

/* GTMAIN.EXE 80055BA4 */
sint32 gtmain_race_speed_control(sint32 speed, sint32 target, sint32 base, sint32 slope)
{
    sint32 difference = (sint32)((uint32)speed - (uint32)target), value;
    if (difference < -11380) difference = -11380;
    else if (difference > 11380) difference = 11380;
    value = (sint32)((uint32)base + direction_fp((uint32)slope, (uint32)difference));
    if (value > 4096) return 4096;
    return value < 0 ? 0 : value;
}

/* GTMAIN.EXE 80053BCC */
sint32 gtmain_race_ai_mode_right(sint32 mode)
{
    if (mode == 1) { if (r_u32(r_u32(0x800BF51Cu) + 8u)) return 0; }
    else if (mode != 0 && mode != 6) return -1;
    return r_u32(r_u32(0x800BF51Cu) + 16u) ? 2 : -1;
}

/* GTMAIN.EXE 80053B64 */
sint32 gtmain_race_ai_mode_left(sint32 mode)
{
    if (mode == 2) { if (r_u32(r_u32(0x800BF51Cu) + 8u)) return 0; }
    else if (mode != 0 && mode != 6) return -1;
    return r_u32(r_u32(0x800BF51Cu) + 12u) ? 1 : -1;
}

/* GTMAIN.EXE 80053C34 */
sint32 gtmain_race_ai_mode_neutral(sint32 mode)
{
    return mode == 6 ? -1 : gtmain_race_ai_mode_select(6);
}

/* GTMAIN.EXE 80054E00 */
void gtmain_race_ai_mode_set(uint32 car, uint8 mode)
{
    w_u8(car + 1912u, mode); gtmain_race_ai_segment_bind(car);
}

/* GTMAIN.EXE 80054E20 */
sint32 gtmain_race_behavior_set(uint32 car, sint32 behavior)
{
    sint32 mode;
    switch (behavior) {
    case 0: w_u8(car + 1780u, 0); break;
    case 2: w_u8(car + 58u, (uint8)(r_u8(car + 58u) | 8u));
        /* Fall through */
    case 4:
        if (r_u8(car + 1912u) == 3u) goto publish;
        mode = direction_behavior_mode3(r_u8(car + 1912u));
        if (mode == -1) return 0;
        gtmain_race_ai_mode_set(car, (uint8)mode); goto publish;
    case 3:
        mode = direction_behavior_mode4(r_u8(car + 1912u));
        if (mode == -1) return 0;
        gtmain_race_ai_mode_set(car, (uint8)mode);
        direction_pair_cache_clear(r_u8(car), r_s16(0x800BC1A6u)); goto publish;
    case 5: goto neutral;
    case 6: case 7: goto publish;
    default: break;
    }
    mode = r_u8(car + 1696u);
    if (mode == 2 || (uint32)mode - 3u < 2u) {
neutral:
        mode = gtmain_race_ai_mode_select(6); gtmain_race_ai_mode_set(car, (uint8)mode);
    } else if (!r_s8(car + 1689u) && !r_u8(car + 1696u)) gtmain_race_ai_segment_bind(car);
publish:
    w_u8(car + 1696u, (uint8)behavior); return 1;
}

/* GTMAIN.EXE 80054FE4 */
uint32 gtmain_race_finish_mark(uint32 car)
{
    if (!r_u8(car + 1960u)) {
        uint32 behavior = r_u8(car + 1696u);
        uint32 result = behavior - 2u < 2u;
        w_u8(car + 1960u, 1);
        if (result) return result;
        if (r_u8(car + 1696u) == 4u) return 4;
    }
    gtmain_race_behavior_set(car, 1);
    uint32 value = r_u16(car + 1700u);
    w_u16(car + 1700u, (uint16)((sint16)value / 2));
    return value >> 15;
}

/* GTMAIN.EXE 80054A98 */
void gtmain_race_ai_segment_update(uint32 car)
{
    uint32 mode = r_u8(car + 1912u), table, rows, count, segment, wheel, sum, denominator, factor;
    sint32 course_length, speed, target, measure, divisor, bound, result, parameter;
    sint64 product;
    if (mode == 4u) return;
    table = r_u32(r_u32(0x800BF51Cu) + 8u + 4u * mode); rows = table + 4u;
    count = r_u32(table); course_length = r_s32(r_u32(0x800BC1A8u)); segment = r_u8(car + 1913u);
    if (segment == count) {
        if (r_s32(car + 1804u) < course_length / 2) {
            w_u8(car + 1913u, 0); gtmain_race_ai_segment_speed(car); goto braking;
        }
        segment = r_u8(car + 1913u);
    }
    if ((sint32)segment < (sint32)count && r_s32(car + 1804u) >= r_s32(rows + 40u * segment + 20u)) {
        w_u8(car + 1913u, (uint8)(r_u8(car + 1913u) + 1u)); gtmain_race_ai_segment_speed(car);
    }
braking:
    if (r_u8(car + 1914u) - 3u < 2u || !r_u8(car + 1915u)) return;
    speed = r_s32(car + 1724u); target = r_s32(car + 1920u); if (target >= speed) return;
    product = (sint64)(sint32)((uint32)speed + (uint32)target) * (sint32)((uint32)speed - (uint32)target);
    measure = (sint32)(uint32)(product / 4096);
    if (r_u32(0x80093E30u) && !(r_u8(car + 58u) & 16u)) {
        sum = 0; for (wheel = 0; wheel < 4u; ++wheel) sum += r_u16(car + 172u + 68u * wheel);
        denominator = direction_fp((uint32)((sint32)(sum << 16) >> 18), r_u32(car + 1212u)) << 1;
    } else denominator = r_u32(car + 1212u) << 1;
    divisor = direction_div(0x1000000u, (sint32)denominator); bound = r_s32(car + 1632u);
    if (bound < divisor) factor = (uint32)direction_div((uint32)bound << 12, divisor);
    else { bound = divisor; factor = 4096; }
    product = (sint64)measure * bound; w_u16(car + 1898u, (uint16)factor);
    measure = (sint32)(uint32)(product / 4096); parameter = (sint16)r_u16(r_u32(car + 1708u));
    result = (sint32)(uint32)gtmain_signed_divide64((sint64)measure * 4096, parameter, 0x80054D5Cu);
    factor = direction_fp(r_u16(car + 1898u), (uint32)parameter); w_u16(car + 1898u, (uint16)factor);
    if (r_u16(car + 1898u) >= 4097u) w_u16(car + 1898u, 4096);
    measure = (sint32)(r_u32(rows + 40u * r_u8(car + 1913u) + 20u) - r_u32(car + 1804u));
    if (result >= (measure >> 4)) w_u8(car + 1914u, r_u8(car + 1914u) ? 4 : 3);
}

/* GTMAIN.EXE 80051398 */
uint32 gtmain_race_wheel_surface_reset(uint32 car)
{
    uint32 wheel, base, enabled, surface;
    for (wheel = 0; wheel < 4u; ++wheel) {
        enabled = r_u32(0x80093E30u); base = car + 132u + 68u * wheel;
        w_u16(base + 32u, 0); w_u16(base + 34u, 0);
        if (enabled) {
            surface = r_u32(0x80093E3Cu); w_u32(base + 36u, surface); w_u8(base + 43u, (sint32)surface < 0 ? 193 : 0);
            w_u16(base + 40u, (uint16)(4096u - r_u16(0x80093E40u)));
        } else { w_u32(base + 36u, 0); w_u8(base + 43u, 0); w_u16(base + 40u, 4096); }
        w_u8(base + 42u, 0);
    }
    return 0;
}

/* GTMAIN.EXE 80051800 */
void gtmain_race_vehicle_reset(uint32 car)
{
    uint32 initial = r_u16(0x80093EA8u), mode, i;
    sint32 divisor, parameter;
    w_u16(car + 1740u, 0); w_u16(car + 1742u, 0); w_u16(car + 1776u, 0); w_u16(car + 1778u, 0);
    for (i = 1744; i <= 1772; i += 4) w_u32(car + i, 0);
    w_u16(car + 1702u, (uint16)initial); mode = r_u8(0x80093C62u);
    if (mode == 1) w_u16(car + 1704u, 4096); else if (mode == 0 || mode == 2) w_u16(car + 1704u, 0);
    w_u16(car + 14u, 0); w_u16(car + 52u, 0); w_u16(car + 54u, 0); w_u32(car + 1792u, 0); w_u16(car + 26u, 0);
    w_u32(car + 1808u, 0); w_u16(car + 1820u, 0); w_u16(car + 1822u, 0);
    for (i = 0; i < 8u; ++i) w_u8(car + 1812u + i, 0);
    parameter = r_s32(car + 1632u);
    w_u32(car + 1712u, 0); w_u32(car + 1716u, 0); w_u32(car + 1720u, 0);
    w_u16(car + 16u, 0); w_u16(car + 18u, 0); w_u16(car + 20u, 0); w_u16(car + 22u, 0); w_u16(car + 24u, 0);
    w_u8(car + 1966u, 0); w_u8(car + 1967u, 0); w_u8(car + 1707u, 0);
    divisor = (sint32)(uint32)gtmain_signed_divide64(0x800000, parameter, 0x80051920u);
    parameter = r_s32(car + 1212u);
    initial = parameter < divisor ? (uint32)gtmain_signed_divide64((sint64)parameter * 4096, divisor, 0x80051968u) : 4096u;
    w_u16(car + 1898u, (uint16)initial); w_u16(car + 50u, 65535);
    w_u16(car + 1954u, 0); w_u16(car + 1952u, 0); w_u16(car + 1958u, 0); w_u16(car + 1956u, 0); w_u16(car + 48u, 0);
    w_u32(car + 1680u, 0); w_u32(car + 1684u, 0); w_u8(car + 1962u, 0); w_u32(car + 28u, 0); w_u16(car + 36u, 0);
    w_u8(car + 32u, 1); w_u16(car + 38u, 0); w_u8(car + 33u, 0); w_u16(car + 34u, 0); w_u16(car + 42u, 0); w_u16(car + 2u, 0);
    w_u8(car + 1690u, 0); w_u8(car + 1782u, 0); w_u8(car + 1783u, 0); gtmain_race_wheel_state_init(car);
    w_u32(car + 1724u, 0); w_u32(car + 1728u, 0); w_u8(car + 1691u, 0); w_u32(car + 1888u, 0);
    w_u32(car + 1732u, 0); w_u32(car + 1736u, 0); w_u8(car + 1784u, 0); w_u8(car + 1785u, 0);
    w_u32(car + 1796u, 0); w_u32(car + 1800u, 0); w_u8(car + 1694u, 0); w_u8(car + 1698u, 0); w_u8(car + 1699u, 0);
    w_u32(car + 1904u, 0); w_u32(car + 1908u, 0); w_u32(car + 1924u, 0); w_u32(car + 1928u, 0);
    for (i = 1932; i <= 1948; i += 4) w_u32(car + i, 0);
    for (i = 0; i < 2u; ++i) w_u16(car + 1972u + 2u * i, 0);
    w_u8(car + 1706u, 0); w_u8(car + 1u, 0);
    for (i = 0; i < 2u; ++i) w_u32(car + 1976u + 4u * i, 0);
}

/* GTMAIN.EXE 8002DD9C */
void gtmain_race_split_record(sint32 rank, sint32 checkpoint, uint32 elapsed)
{
    uint32 finish = 0, show = 0, variant, timer;
    if (rank) return;
    w_u32(0x800BDB50u + 4u * (uint32)checkpoint, elapsed);
    if (r_u8(0x800CB7F5u) != 5u) return;
    variant = r_u8(0x800CB7F6u);
    if (checkpoint == 3 || checkpoint == 4) { show = 1; finish = !variant || (checkpoint == 4 && variant == 1u); }
    else if (checkpoint == 2) show = 1;
    if (finish) {
        w_u32(0x800B6870u, elapsed); gtmain_race_lap_time_append(elapsed);
        gtmain_race_best_time_set(elapsed); direction_call(0x80023474u, 0, 0, 0); gtmain_race_finish_mark(0x800B6878u);
    } else if (show) {
        gtmain_time_string(0x800B39A4u, elapsed); timer = r_u32(0x800BF518u);
        w_u16(0x80090718u, 0); w_u16(0x8009071Au, (uint16)(3u * timer));
    }
}

/* GTMAIN.EXE 8002DEB8 */
void gtmain_race_lap_record(sint32 rank, sint32 lap, uint32 time, uint32 elapsed)
{
    uint32 car = 0x800B6850u + 3380u * (uint32)rank, kind, timer;
    sint32 limit = r_s16(0x800B634Cu);
    uint32 finish;
    if (!limit) limit = 100;
    if (limit < lap) return;
    finish = lap == r_s16(0x800B634Cu);
    if (finish) { gtmain_race_finish_mark(car + 40u); w_u32(car + 32u, time); }
    if ((uint32)r_u16(car + 26u) - 1u >= 2u) return;
    if (finish) {
        kind = (uint32)r_s16(car + 26u);
        if (kind == 1u) w_u32(0x800BDB44u, r_u32(car + 32u)); else if (kind == 2u) w_u32(0x800BDB64u, r_u32(car + 32u));
        gtmain_race_mode_prepare(); gtmain_race_time_set(time); gtmain_race_best_time_set(time);
    } else {
        gtmain_time_string(0x800B39A4u, elapsed); timer = r_u32(0x800BF518u);
        w_u16(0x80090718u, 0); w_u16(0x8009071Au, (uint16)(3u * timer));
    }
    gtmain_race_lap_time_append(elapsed); gtmain_race_best_lap_set(elapsed);
}

