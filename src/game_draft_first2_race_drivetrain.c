#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified native draft from complete pseudocode and MIPS */
static sint32 dt_div(sint32 a, sint32 b, uint32 owner)
{
    if (!b) gt_draft_failure("GTMAIN drivetrain BREAK7", owner);
    if ((uint32)a == 0x80000000u && b == -1) gt_draft_failure("GTMAIN drivetrain BREAK6", owner);
    return a / b;
}
static uint32 dt_mul(sint32 a, sint32 b) { return (uint32)((sint64)a * b / 4096); }
static uint32 dt_low(sint32 a, sint32 b) { return (uint32)((sint32)((uint32)a * (uint32)b) / 4096); }

sint32 gtmain_race_engine_torque(uint32 c)
{
    uint32 s = c + 404u, a;
    if (!r_s16(c + 692u)) a = (uint32)r_s16(c + 18u);
    else {
        sint32 target = (sint32)dt_low(r_s16(c + 1952u), r_s16(c + 38u));
        uint32 i;
        for (i = 0; i < 2u; ++i) if (r_s16(s + 268u + i * 2u)) {
            uint32 value = dt_mul((sint32)((uint32)target - (uint32)r_s16(c + 1956u + i * 2u)), r_s32(s + 272u + i * 4u));
            sint32 delta = (sint32)(uint32)((sint64)(sint32)value * r_s32(0x1F800000u) / 65536);
            w_u16(c + 1956u + i * 2u, (uint16)(r_u16(c + 1956u + i * 2u) + (uint32)delta));
            if ((delta > 0 && r_s16(c + 38u) < r_s16(c + 1956u + i * 2u)) || (delta < 0 && r_s16(c + 1956u + i * 2u) < r_s16(c + 38u)))
                w_u16(c + 1956u + i * 2u, r_u16(c + 38u));
        }
        sint32 torque = (sint32)dt_low(r_s16(c + 18u), gtmain_race_engine_boost_limit(s + 116u, r_s16(c + 1956u), r_s16(c + 1958u)));
        w_u16(c + 1954u, (uint16)torque);
        sint32 denominator = gtmain_race_engine_boost_limit(s + 116u, r_s16(c + 38u), r_s16(c + 38u));
        a = (uint32)dt_div((sint32)((uint32)torque << 12), denominator, 0x800675A8u);
        w_u16(c + 1952u, (uint16)a);
        if (!r_s16(c + 50u)) {
            if (r_s16(c + 18u) < 2048) { w_u16(c + 50u, (uint16)torque); if ((sint16)torque <= 0) w_u16(c + 50u, 1); }
        } else if (r_s16(c + 50u) != -1) w_u16(c + 50u, 0xFFFFu);
        else if (r_s16(c + 18u) >= 3073) w_u16(c + 50u, 0);
        w_u16(c + 48u, r_u16(c + 1956u));
        if ((sint32)r_u16(c + 48u) < r_s16(c + 1958u)) w_u16(c + 48u, r_u16(c + 1958u));
    }
    if (!r_u8(c + 1962u)) { if (r_s16(c + 38u) >= (sint32)r_u16(s + 256u)) w_u8(c + 1962u, 1); }
    else if (r_s16(c + 38u) < (sint32)r_u16(s + 256u) - 500) w_u8(c + 1962u, 0);
    if (r_u8(c + 1962u)) return (sint32)(0u - r_u32(c + 1684u));
    return (sint32)(dt_mul((sint32)a, (sint32)(r_u32(c + 1680u) + r_u32(c + 1684u))) - r_u32(c + 1684u));
}

sint32 gtmain_race_gear_select(uint32 c, const uint8 *controls)
{
    uint32 s = c + 404u, gear = r_u8(c + 32u), mode;
    if (r_u16(c + 12u)) return gear == 1u ? -1 : 1;
    mode = r_u8(c + 1696u) ? 0u : r_u8(c + 1461u);
    if (!mode) {
        if ((sint8)controls[0]) return gear == 0u ? -1 : 0;
        if (r_s32(c + 1724u) >= 1139 && !gear) return 1;
        if (r_s16(c + 18u) && r_s32(c + 1724u) < 1138) return gear == 1u ? -1 : 1;
        if (gear && gear < r_u8(c + 1462u)) {
            sint32 scaled = (sint32)(((uint32)r_u16(c + 660u) + 1000u) << 12);
            sint32 limit = (sint32)(uint32)gtmain_signed_divide64((sint64)(scaled / 60) * 4096, r_s32(s + 1124u + gear * 4u), 0x80067A48u);
            if (limit < r_s32(c + 1724u)) return (sint32)gear + 1;
        }
        if (r_u8(c + 1691u) == 1u && gear) {
            sint32 low, high;
            if (gear == 1u) { if (r_u8(c + 1462u) == 1u) return -1; low = (sint32)0x80000001u; high = r_u16(c + 1472u); }
            else { high = gear < r_u8(c + 1462u) ? r_u16(s + 1066u + gear * 2u) : 0x7FFFFFFF; low = r_u16(s + 1076u + gear * 2u); }
            if (r_s16(c + 38u) < low) {
                if ((sint8)r_u8(c + 1689u) == 2 || r_u8(c + 1696u)) { if (r_u8(c + 1699u)) return -1; }
                return (sint32)gear - 1;
            }
            if (r_s16(c + 38u) <= high) return -1;
            uint32 kind = r_u8(s + 1056u);
            if (kind == 0u || kind == 3u || kind == 4u) {
                sint32 rpm = r_s16(c + 1064u);
                if (r_u8(c + 268u) && rpm < r_s16(c + 316u)) return (sint32)gear + 1;
                if (r_u8(c + 336u) && rpm < r_s16(c + 384u)) return (sint32)gear + 1;
                if (kind != 3u) return -1;
            }
            if (kind == 1u || kind == 2u || kind == 3u) {
                sint32 rpm = r_s16(c + 1000u);
                if (r_u8(c + 132u) && rpm < r_s16(c + 180u)) return (sint32)gear + 1;
                if (r_u8(c + 200u) && rpm < r_s16(c + 248u)) return (sint32)gear + 1;
            }
        }
        return -1;
    }
    if (mode != 1u) return -1;
    if ((sint8)controls[0]) return gear == 0u ? -1 : 0;
    if (!gear && r_s16(c + 16u)) return 1;
    if (r_u8(c + 1699u)) {
        w_u8(c + 1699u, (uint8)(r_u8(c + 1699u) - 1u));
        if (!r_u8(c + 1699u) && (sint8)controls[2]) w_u8(c + 1699u, 1);
        return -1;
    }
    if ((sint8)controls[2] > 0 && gear < r_u8(s + 1058u)) { w_u8(c + 1699u, 1); return (sint32)r_u8(c + 32u) + 1; }
    if ((sint8)controls[2] < 0 && gear >= 2u) { w_u8(c + 1699u, 1); return (sint32)r_u8(c + 32u) - 1; }
    return -1;
}

void gtmain_race_controls_reset(uint32 c)
{
    uint32 i;
    w_u16(c + 1820u, 0); w_u32(c + 1808u, 0);
    for (i = 0; i < 8u; ++i) w_u8(c + 1812u + i, 0);
}
uint32 gtmain_race_controls_apply_basic(uint32 c, uint8 *controls)
{
    uint32 output, result = gtmain_race_controls_query(c, controls, &output, 0);
    w_u8(c + 33u, (uint8)result);
    result &= 255u;
    if (!result) { w_u16(c + 42u, 0); return result; }
    w_u8(c + 1698u, (uint8)(r_u8(c + 1698u) | 1u));
    return gtmain_race_controls_commit(c, controls, output, 0);
}

static void dt_split(uint32 c, uint32 scratch, uint32 force, uint32 differential, uint32 kind)
{
    switch (kind) {
    case 0: case 4: w_u32(scratch + 8u, 0); w_u32(scratch + 36u, force); break;
    case 1: w_u32(scratch + 8u, force); w_u32(scratch + 36u, 0); break;
    case 2: w_u32(scratch + 8u, force - differential); w_u32(scratch + 36u, differential); break;
    case 3:
        w_u32(scratch + 8u, dt_mul((sint32)force, r_s32(c + 1464u)));
        w_u32(scratch + 36u, force - r_u32(scratch + 8u)); break;
    }
}
uint32 gtmain_race_drive_forces_update(uint32 c, uint32 index)
{
    uint32 offset = index * 84u, scratch = 0x1F800000u + offset, s = c + 404u;
    uint32 mode, kind, differential = 0, force;
    w_u32(scratch + 64u, 0); w_u32(scratch + 60u, 0);
    mode = r_u8(c + 1691u);
    if (mode) {
        if (mode == 1u && !(sint8)r_u8(0x1F8001FDu + index * 4u)) w_u8(c + 1691u, 0);
    } else if ((sint8)r_u8(0x1F8001FDu + index * 4u) == 1) {
        w_u8(c + 1691u, (r_u32(c + 1724u) + 11380u < 0x58E9u) ? 2 : 3);
    }
    w_u32(scratch + 12u, dt_low(r_s16(c + 22u), r_s32(c + 1620u)));
    w_u32(scratch + 40u, dt_low(r_s16(c + 24u), r_s32(c + 1624u)) + dt_low(r_s16(c + 1692u), r_s32(c + 1628u)));
    if (!r_u8(c + 1691u)) {
        w_u32(scratch + 8u, 0); w_u32(scratch + 36u, 0);
        w_u8(c + 1784u, 0); w_u8(c + 1785u, 0);
        goto flags;
    }
    kind = r_u8(c + 1460u);
    if (kind == 2u) {
        uint32 value = dt_mul((sint32)dt_low(r_s16(c + 1562u), r_s16(c + 1564u)), r_s32(c + 1732u));
        differential = dt_mul((sint32)(value - r_u32(c + 1736u)), r_s32(c + 1464u));
        kind = r_u8(c + 1460u);
    }
    switch (kind) {
    case 0: case 4: w_u8(c + 1784u, 0); w_u8(c + 1785u, 8); break;
    case 1: w_u8(c + 1784u, 8); w_u8(c + 1785u, 0); break;
    case 2: w_u8(c + 1784u, 8); w_u8(c + 1785u, 4); break;
    case 3: w_u8(c + 1784u, 4); w_u8(c + 1785u, 8); break;
    }
    if (r_u8(c + 1691u) == 1u) {
        sint32 torque = gtmain_race_engine_torque(c);
        force = dt_low(torque, r_s32(s + 1092u + r_u8(c + 32u) * 4u));
        if (!r_u8(c + 32u)) force = 0u - force;
        kind = r_u8(s + 1056u);
        if (kind == 4u) {
            uint32 speed = r_u32(c + 1736u), magnitude = speed;
            uint32 difference = speed - r_u32(c + 1732u);
            if ((sint32)speed < 0) { difference = 0u - difference; magnitude = 0u - magnitude; }
            if ((sint32)difference <= 0) dt_split(c, scratch, force, differential, 0);
            else {
                sint32 weight = (sint32)magnitude > 45519 ? 4096 : (sint32)(magnitude << 12) / 45520;
                weight = (sint32)dt_mul(weight, (sint32)difference) / 16;
                if (weight >= 2049) weight = 2048;
                w_u32(scratch + 8u, dt_mul(weight, (sint32)force));
                w_u32(scratch + 36u, force - r_u32(scratch + 8u));
                w_u8(c + 1784u, 4);
            }
        } else dt_split(c, scratch, force, differential, kind);
    } else if ((uint32)r_u8(c + 1691u) - 2u < 2u) {
        w_u32(scratch + 60u, r_u32(c + 1888u));
        kind = r_u8(c + 1460u);
        if (kind < 5u) {
            uint32 speed = (kind == 1u || kind == 2u) ? c + 1732u : c + 1736u;
            w_u32(scratch + 64u, dt_mul(r_s32(s + 1124u + r_u8(c + 32u) * 4u), r_s32(speed)));
        }
        if (!r_u8(c + 32u)) w_u32(scratch + 64u, 0u - r_u32(scratch + 64u));
        if (r_u8(c + 1691u) == 3u) w_u16(c + 18u, r_s32(scratch + 64u) >= r_s32(scratch + 60u) ? 4096 : 0);
        sint32 torque = gtmain_race_engine_torque(c);
        uint32 resistance = (uint32)r_s16(s + (r_u8(c + 1691u) == 3u ? 1172u : 1174u));
        if (r_s32(scratch + 60u) < r_s32(scratch + 64u)) resistance = 0u - resistance;
        uint32 delta = dt_mul((sint32)((uint32)torque - resistance), r_s32(s + 1168u));
        delta = (uint32)((sint64)r_s32(0x1F800000u) * (sint32)delta / 65536);
        w_u32(c + 1888u, r_u32(c + 1888u) + delta);
        if (!r_u8(c + 32u)) resistance = 0u - resistance;
        force = dt_mul((sint32)resistance, r_s32(s + 1092u + r_u8(c + 32u) * 4u));
        dt_split(c, scratch, force, differential, r_u8(s + 1056u));
    }
flags:
    if (r_u32(scratch + 12u)) w_u8(c + 1784u, (uint8)(r_u8(c + 1784u) | 2u));
    if (r_u32(scratch + 40u)) w_u8(c + 1785u, (uint8)(r_u8(c + 1785u) | 2u));
    if (!r_u32(c + 152u) && !r_u32(c + 220u)) w_u8(c + 1784u, (uint8)(r_u8(c + 1784u) | 1u));
    if (!r_u32(c + 288u) && !r_u32(c + 356u)) w_u8(c + 1785u, (uint8)(r_u8(c + 1785u) | 1u));
    uint32 denominator = r_u16(s + 40u);
    if (!(r_u8(c + 1784u) & 1u) && !(r_u8(c + 1784u) & 12u)) denominator += (uint32)r_s16(s + 1212u);
    if (!(r_u8(c + 1785u) & 1u) && !(r_u8(c + 1785u) & 12u)) denominator += (uint32)r_s16(s + 1214u);
    w_u32(scratch + 72u, (uint32)dt_div(0x1000000, (sint16)denominator, 0x80059D30u));
    return offset;
}

uint32 gtmain_race_traction_apply(uint32 c, uint32 index)
{
    uint32 result = r_u8(c + 1698u) & 1u;
    if (!result) return result;
    result = (uint32)(sint8)r_u8(c + 1u);
    if (!result) return result;
    if (r_u8(c + 58u) & 16u) return index * 4u;
    uint32 scratch = 0x1F800000u + index * 84u;
    sint32 cosine = r_s16(scratch + 70u), sine = r_s16(scratch + 68u);
    uint32 total = r_u32(c + 1760u) + r_u32(c + 1768u);
    uint32 first = r_u32(c + 1764u) + r_u32(c + 1772u) + dt_mul(cosine, (sint32)total) - dt_mul(sine, r_s32(c + 1752u));
    uint32 second = r_u32(c + 1756u) + dt_mul(cosine, r_s32(c + 1752u)) + dt_mul(sine, (sint32)total);
    sint32 direction = -r_s16(c + 1972u), scale = r_s16(c + 1974u);
    uint32 ax = dt_low(r_s16(c + 72u), scale) + dt_low(r_s16(c + 74u), direction);
    uint32 az = dt_low(r_s16(c + 80u), scale) + dt_low(r_s16(c + 82u), direction);
    result = (uint32)(r_s32(c + 1644u) >> 1);
    uint32 force = dt_mul((sint32)ax, (sint32)first) - dt_mul((sint32)az, (sint32)second) + result;
    if ((sint32)force <= 0) return result;
    uint32 factor = (uint32)((sint32)(force * r_u32(0x800BF518u)) / 30) << 3;
    uint32 impulse = 0u - dt_mul((sint32)((uint32)(sint8)r_u8(c + 1u) << 6), (sint32)factor);
    ax = dt_low(r_s16(c + 1972u), r_s16(c + 72u)) + dt_low(r_s16(c + 1974u), r_s16(c + 74u));
    az = dt_low(r_s16(c + 1972u), r_s16(c + 80u)) + dt_low(r_s16(c + 1974u), r_s16(c + 82u));
    uint32 dx = dt_mul((sint32)ax, (sint32)impulse);
    uint32 dz = dt_mul((sint32)az, (sint32)impulse);
    uint32 half = (uint32)((sint32)dx / 2);
    uint32 left = r_u32(c + 1940u), right = r_u32(c + 1948u), center = r_u32(c + 1944u);
    w_u32(c + 1940u, left + half); w_u32(c + 1948u, right + half); w_u32(c + 1944u, center - dz);
    return dz;
}


