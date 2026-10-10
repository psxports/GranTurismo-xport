#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified native draft from complete pseudocode and MIPS */
static uint32 cw_mul(sint32 a, sint32 b) { return (uint32)((sint64)a * b / 4096); }
static uint32 cw_abs(uint32 a) { return (sint32)a < 0 ? 0u - a : a; }
static uint32 cw_low(uint32 a, uint32 b, sint32 divisor) { return (uint32)((sint32)(a * b) / divisor); }
uint32 gtmain_race_contact_query(uint32 car, uint32 *fraction, uint32 *corner)
{
    uint32 toggle = r_u8(car + 1961u), old = (1u - toggle) * 32u, current = toggle * 32u, i;
    for (i = 0; i < 4u; ++i) {
        uint32 p = car + i * 8u + old, q = car + i * 8u + current, record = 0x1F800004u + i * 44u;
        w_u32(record, r_u32(p + 1824u) << 4);
        uint32 z = r_u32(p + 1828u);
        w_u32(record + 8u, 0); w_u32(record + 4u, z << 4);
        w_u32(record + 12u, r_u32(q + 1824u) << 4);
        z = r_u32(q + 1828u);
        w_u32(record + 20u, 0); w_u32(record + 16u, z << 4);
    }
    gtmain_collision_probes_update(r_u32(car + 1892u), (uint8 *)psx_addr(0x1F800004u, 176u));
    uint32 mask = 0, selected = 0;
    sint16 minimum = 4096;
    for (i = 0; i < 4u; ++i) {
        sint16 value = r_s16(0x1F80001Cu + i * 44u);
        if (value != 4096) {
            mask |= 1u << i;
            if (value < minimum) { selected = i; minimum = value; }
        }
    }
    *corner = selected; *fraction = (uint32)(sint32)minimum;
    if (!mask) return 0;
    w_u16(car + 1972u, r_u16(0x1F800020u + selected * 44u));
    w_u16(car + 1974u, r_u16(0x1F800022u + selected * 44u));
    return mask;
}
uint32 gtmain_race_contact_response(uint32 car)
{
    uint32 toggle = r_u8(car + 1961u), corner = r_u8(car + 1706u) * 8u;
    sint32 cosine = r_s16(car + 1974u);
    uint32 p = car + corner + toggle * 32u, q = car + corner + (1u - toggle) * 32u;
    uint32 x = r_u32(p + 1824u) - r_u32(q + 1824u);
    sint32 sine = r_s16(car + 1972u);
    uint32 z = r_u32(p + 1828u) - r_u32(q + 1828u);
    uint32 projection = cw_mul(-cosine, (sint32)x) + cw_mul(sine, (sint32)z);
    if ((sint32)projection > 0) return projection;
    uint32 tangent = cw_mul(sine, (sint32)x) + cw_mul(cosine, (sint32)z);
    sint32 rate = r_s32(0x800BF518u);
    uint32 product = tangent * (uint32)rate;
    if (!rate) gt_draft_exception(7u);
    sint32 threshold = 4096 / rate, impulse = (sint32)product / 2048;
    if (impulse >= 65) impulse = 64;
    else if (impulse < -64) impulse = -64;
    uint32 result = 0u - (uint32)threshold;
    if (threshold < (sint32)tangent) w_u8(car + 1u, (uint8)impulse);
    else {
        result = (uint32)((sint32)tangent < (sint32)result);
        w_u8(car + 1u, result ? (uint8)impulse : 0);
    }
    return result;
}
uint32 gtmain_race_controls_impulse(uint32 car, uint8 *controls, uint32 fraction)
{
    uint32 i, result = 0;
    for (i = 0; i < 4u; ++i) {
        uint32 field = i < 3u ? 1712u + i * 4u : 1792u;
        uint32 scaled = cw_low(fraction, r_u32(car + field), 4096);
        result = cw_low(scaled, r_u32(0x1F800000u), 65536);
        xport_store_le32(controls + i * 4u, result);
    }
    return result;
}
uint32 gtmain_race_wheel_effects_update(uint32 c)
{
    uint32 slow = r_u32(c + 1724u) + 2275u < 0x11C7u && r_u32(c + 1728u) + 2275u < 0x11C7u;
    uint32 friction = 0, spin = 0, skid = 0, brake = 0, i;
    uint32 value = cw_mul((sint32)(cw_mul(r_s16(c + 180u), r_s16(c + 180u)) + 409u), r_s32(c + 1760u));
    sint32 first = (sint32)(cw_abs(value) << 1) / 3;
    if (first >= 1366) first = 1365;
    sint32 rotation = r_s32(c + 1768u) / 12;
    sint32 second = (sint32)cw_mul(rotation, rotation);
    if (second >= 1366) second = 1365;
    if (slow) second = 0;
    uint32 combined = (uint32)first + (uint32)second;
    sint32 rate = r_s32(0x800BF518u);
    if (r_s16(c + 34u) == rate || (sint32)r_u16(c + 2u) < r_s16(c + 42u)) w_u16(c + 2u, r_u16(c + 42u));
    else if (r_s16(c + 34u) < (sint32)((uint32)rate * 3u) / 5) w_u16(c + 2u, 0);
    if (r_u16(c + 2u)) brake = r_u16(c + 2u) * 8u + 1024u;
    uint32 angle = r_u16(c + 14u) & 4095u;
    uint32 projected = cw_mul(r_s16(0x80094A3Cu + angle * 2u), r_s32(c + 1932u));
    angle = r_u16(c + 14u) & 4095u;
    projected += cw_mul(r_s16(0x8009423Cu + angle * 2u), r_s32(c + 1724u));
    uint32 velocities[4] = { projected, projected, r_u32(c + 1936u), r_u32(c + 1936u) };
    for (i = 0; i < 4u; ++i) {
        uint32 w = c + 132u + i * 68u;
        if (!r_u8(c + 1688u)) {
            sint32 load = r_s32(w + 64u);
            if (load > 0) { load /= 12; if (load >= 4097) load = 4096; spin += (uint32)load; }
            if (i < 2u && r_u8(w + 44u) == 1u) {
                uint32 v = cw_abs(cw_mul(r_s32(w + 20u) / 8, r_s32(c + 1724u) / 64));
                if ((sint32)v >= 1025) v = 1024;
                skid += v;
            }
        }
        if (!r_u8(w)) { w_u8(w + 2u, 0); w_u8(w + 3u, 0); w_u8(w + 1u, 0); }
        else {
            sint32 velocity = (sint32)(velocities[i] * 80u) / 4096;
            sint32 square = (sint32)((uint32)velocity * (uint32)velocity) / 4096;
            if (square >= 129) square = 128;
            sint32 intensity, slip = (sint8)r_u8(w + 3u);
            if (slip == 127) intensity = slow ? 0 : 192;
            else if (slip <= 0) { intensity = square - slip * 2 + 48; if (intensity >= 193) intensity = 192; }
            else { intensity = square + slip + 48; if (intensity >= 193) intensity = 192; if (slow) intensity = 0; }
            uint32 surface = r_u8(w + 44u);
            if (surface < 2u || surface >= 6u) {
                if (intensity >= 256) intensity = 255;
                else if (intensity < 96) intensity = 0;
                w_u8(w + 1u, (uint8)(intensity * 3 / 4)); w_u8(w + 2u, (uint8)intensity);
            } else {
                sint32 roughness = intensity + r_s16(w + 46u) / 256;
                sint32 output = intensity + 24 + roughness;
                if (roughness < 64) roughness = 0;
                friction += (uint32)roughness;
                if (output >= 256) output = 255;
                else if (output < 96) output = 0;
                w_u8(w + 1u, (uint8)output); w_u8(w + 2u, 0);
            }
        }
        if (r_u8(w + 1u)) {
            uint32 increment = cw_low((uint32)r_s16(w + 46u), r_u32(0x1F800000u), 65536);
            w_u16(w + 54u, (uint16)(r_u16(w + 54u) + increment));
            if (r_s16(w + 54u) < 34) w_u8(w + 1u, 0);
            else w_u16(w + 54u, 0);
        } else w_u16(w + 54u, 34);
    }
    uint32 maximum = spin + skid;
    if ((sint32)maximum < (sint32)combined) maximum = combined;
    if ((sint32)maximum < (sint32)brake) maximum = brake;
    sint32 intensity = (sint32)maximum >> 4;
    if (intensity >= 256) intensity = 255;
    w_u8(c + 1782u, (uint8)intensity); w_u8(c + 1783u, 0);
    if ((sint32)(r_u32(0x800BF518u) * 4u) / 5 < r_s16(c + 34u)) w_u8(c + 1783u, 1);
    uint32 speed = cw_abs(r_u32(c + 1724u));
    uint32 result = cw_low(friction * 8u + (uint32)((sint32)speed >> 10), r_u32(0x1F800000u), 65536) + r_u32(c + 1968u);
    w_u32(c + 1968u, result);
    if ((sint32)result < 0) w_u32(c + 1968u, 0x7FFFFFFFu);
    return result;
}
void gtmain_race_wheel_rotation_update(uint32 cars, sint32 count)
{
    sint32 i;
    for (i = 0; i < count; ++i) {
        uint32 base = cars + (uint32)i * 3380u, c = base + 40u, spec = base + 444u;
        uint32 first = cw_mul(r_s32(base + 1752u), r_s16(base + 112u));
        uint32 second = cw_mul(r_s32(base + 1756u), r_s16(base + 114u));
        uint32 third = cw_mul(r_s32(base + 1760u), r_s16(base + 116u));
        w_u32(c + 1724u, first + second + third);
        uint32 dot = (uint32)gtmain_vector_dot_fixed((const uint8 *)psx_addr(c + 1712u, 12u), (const uint8 *)psx_addr(base + 120u, 6u));
        uint32 flags = r_u8(c + 1784u);
        w_u32(c + 1728u, dot);
        if (!flags) w_u32(c + 1732u, r_u32(c + 1724u));
        if (!r_u8(c + 1785u)) w_u32(c + 1736u, r_u32(c + 1724u));
        w_u16(c + 36u, (uint16)((sint32)cw_mul(9164, (sint32)cw_abs(r_u32(c + 1724u))) >> 12));
        sint32 rpm = (sint32)(r_u32(c + 1888u) * 60u) / 4096;
        w_u16(c + 38u, (uint16)rpm);
        if ((sint16)rpm < (sint32)r_u16(spec + 258u)) w_u16(c + 38u, (uint16)(r_u16(spec + 258u) - 1u));
        uint32 factor = (uint32)((sint64)r_s32(0x1F800000u) * r_s32(c + 1732u) / 65536);
        factor = cw_mul((sint32)factor, r_s16(spec + 1160u));
        factor = cw_mul(652, (sint32)factor);
        uint32 a = r_u16(c + 138u), b = r_u16(c + 206u);
        w_u16(c + 138u, (uint16)(a + factor)); w_u16(c + 206u, (uint16)(b + factor));
        factor = (uint32)((sint64)r_s32(0x1F800000u) * r_s32(c + 1736u) / 65536);
        factor = cw_mul((sint32)factor, r_s16(spec + 1162u));
        factor = cw_mul(652, (sint32)factor);
        a = r_u16(c + 274u); b = r_u16(c + 342u);
        w_u16(c + 274u, (uint16)(a + factor)); w_u16(c + 342u, (uint16)(b + factor));
        uint32 wheel;
        for (wheel = 0; wheel < 4u; ++wheel) {
            uint32 field = c + 138u + wheel * 68u;
            while (r_s16(field) >= 4096) w_u16(field, (uint16)(r_u16(field) - 4096u));
            while (r_s16(field) < 0) w_u16(field, (uint16)(r_u16(field) + 4096u));
        }
    }
}

