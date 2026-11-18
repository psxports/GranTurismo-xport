#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified native draft from complete pseudocode and MIPS */
static uint32 cq_mul(sint32 a, sint32 b) { return (uint32)((sint64)a * b / 4096); }
static uint32 cq_abs(uint32 a) { return (sint32)a < 0 ? 0u - a : a; }
static void cq_angle_wrap(uint32 car)
{
    if (r_s16(car + 1790u) >= 4096) w_u16(car + 1790u, (uint16)(r_u16(car + 1790u) - 4096u));
    if (r_s16(car + 1790u) < -4095) w_u16(car + 1790u, (uint16)(r_u16(car + 1790u) + 4096u));
}
/* Original: GTMAIN.EXE:sub_80066D9C. */ uint32 gtmain_race_controls_query(uint32 car, const uint8 *controls, uint32 *output, sint32 mode)
{
    uint32 fraction, flags = 0, contact = 0, i;
    w_u32(car + 60u, r_u32(car + 60u) + xport_load_le32(controls));
    w_u32(car + 64u, r_u32(car + 64u) + xport_load_le32(controls + 4u));
    uint32 angle = r_u16(car + 1790u);
    w_u32(car + 68u, r_u32(car + 68u) + xport_load_le32(controls + 8u));
    w_u16(car + 1790u, (uint16)(angle + xport_load_le16(controls + 12u)));
    cq_angle_wrap(car);
    if (mode != 1) w_u8(car + 1961u, (uint8)(1u - r_u8(car + 1961u)));
    gtmain_race_vehicle_bounds_update(car);
    if ((sint8)r_u8(car + 1689u) != 4 && !r_s16(0x800B63DEu)) contact = gtmain_race_contact_query(car, &fraction, &flags);
    if (!contact) { *output = 0; return 0; }
    w_u8(car + 1706u, (uint8)flags);
    if (mode >= 0 && mode < 3) {
        if (!mode) w_u8(car + 1u, 0);
        gtmain_race_contact_response(car);
    }
    fraction = 4096u - fraction;
    *output = fraction;
    for (i = 0; i < 3u; ++i) {
        uint32 value = cq_mul((sint32)xport_load_le32(controls + i * 4u), (sint32)fraction);
        w_u32(car + 60u + i * 4u, r_u32(car + 60u + i * 4u) - value);
    }
    uint32 product = xport_load_le32(controls + 12u) * fraction;
    w_u16(car + 1790u, (uint16)(r_u16(car + 1790u) - (uint32)((sint32)product / 4096)));
    cq_angle_wrap(car);
    sint32 cosine = r_s16(car + 1974u) >> 4;
    sint32 sine = r_s16(car + 1972u) >> 4;
    w_u32(car + 60u, r_u32(car + 60u) - (uint32)cosine);
    w_u32(car + 64u, r_u32(car + 64u) + (uint32)sine);
    if (mode) gtmain_race_vehicle_bounds_update(car);
    return contact;
}
/* Original: GTMAIN.EXE:sub_80067094. */ uint32 gtmain_race_controls_commit(uint32 car, uint8 *controls, uint32 output, sint32 mode)
{
    uint32 first = r_u32(car + 1712u);
    sint32 sine = r_s16(car + 1972u);
    uint32 second = r_u32(car + 1716u);
    sint32 cosine = r_s16(car + 1974u);
    uint32 projection = cq_mul((sint32)first, sine) + cq_mul((sint32)second, cosine);
    w_u32(car + 1712u, cq_mul((sint32)projection, sine));
    uint32 projected_second = cq_mul((sint32)projection, cosine);
    sint32 magnitude = r_s32(car + 1720u);
    w_u32(car + 1716u, projected_second);
    if (magnitude >= 4097) {
        uint32 a = cq_abs(first), b = cq_abs(second), minimum = a;
        if ((sint32)b < (sint32)minimum) minimum = b;
        uint32 approximate = a + b - (uint32)((sint32)minimum >> 1);
        if ((sint32)approximate > 0) {
            uint32 ratio = (uint32)gtmain_signed_divide64((sint64)(sint32)projection * 4096, (sint32)approximate, 0x80067204u);
            w_u32(car + 1720u, cq_mul(r_s32(car + 1720u), (sint32)ratio));
        }
    }
    if (mode == 1) return 1;
    uint32 a = cq_abs(first - r_u32(car + 1712u));
    uint32 b = cq_abs(second - r_u32(car + 1716u));
    uint32 minimum = a;
    if ((sint32)b < (sint32)minimum) minimum = b;
    uint32 approximate = a + b - (uint32)((sint32)minimum >> 1);
    uint32 product = approximate * 41u;
    w_u16(car + 42u, (uint16)((sint32)product / 4096));
    w_u32(0x1F800000u, r_u32(car + 4u));
    gtmain_race_controls_impulse(car, controls, output);
    uint32 discarded_output;
    return gtmain_race_controls_query(car, controls, &discarded_output, 1);
}

