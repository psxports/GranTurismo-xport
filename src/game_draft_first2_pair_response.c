#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"

static uint32 pair_wide(sint32 a, sint32 b)
{
    return (uint32)((sint64)a * b / 4096);
}

static uint32 pair_low(uint32 a, uint32 b, sint32 divisor)
{
    return (uint32)((sint32)(a * b) / divisor);
}

/* GTMAIN.EXE 80062D18 */
void gtmain_race_pair_response(uint32 cars, sint32 count)
{
    sint32 outer, inner;
    uint32 normal = gt_local_stack_workspace(0x2E00u, 6u, 0x80062D18u);
    for (outer = 1; outer < count; ++outer) {
        for (inner = 0; inner < outer; ++inner) {
            sint16 first = r_s16(0x800BFE00u + 2u * (uint32)outer + 10u * (uint32)inner - 2u);
            sint16 second = r_s16(0x800BFE00u + 2u * (uint32)inner + 10u * (uint32)outer);
            uint32 a, b, slot, weight_a = 4096, weight_b = 4096, sum = 0;
            uint32 impulse[3], magnitude, intensity, axis;
            sint16 delta[3], overlap;
            sint32 side, selector, quotient;
            if (first == 4096 && second == 4096) continue;
            if (first < second) {
                a = cars + 3380u * (uint32)inner + 40u;
                b = cars + 3380u * (uint32)outer + 40u;
                slot = 5u * (uint32)inner + (uint32)outer - 1u;
            } else {
                a = cars + 3380u * (uint32)outer + 40u;
                b = cars + 3380u * (uint32)inner + 40u;
                slot = 5u * (uint32)outer + (uint32)inner;
                first = second;
            }
            side = r_s8(0x800BF550u + slot);
            selector = r_s8(0x800BF570u + slot);
            overlap = (sint16)(uint16)(4096 - first);
            if (selector >= 0 && selector <= 3) {
                uint32 vector = a + (selector < 2 ? 72u : 80u);
                for (axis = 0; axis < 3; ++axis) {
                    uint32 component = r_u16(vector + axis * 2u);
                    if (selector == 1 || selector == 2) component = 0u - component;
                    w_u16(normal + axis * 2u, component);
                }
            }
            for (axis = 0; axis < 3; ++axis) {
                uint32 va = pair_wide(r_s16(a + 1702u), r_s32(a + 1712u + axis * 4u));
                uint32 vb = pair_wide(r_s16(b + 1702u), r_s32(b + 1712u + axis * 4u));
                sum += pair_wide((sint16)r_u16(normal + axis * 2u), (sint32)(va - vb));
            }
            quotient = (sint32)(uint32)gtmain_signed_divide64((sint64)(sint32)sum * 4096,
                (sint64)r_s16(a + 1702u) + r_s16(b + 1702u), 0x800631D0u);
            for (axis = 0; axis < 3; ++axis) impulse[axis] = pair_wide((sint16)r_u16(normal + axis * 2u), quotient);
            magnitude = quotient < 0 ? 0u - (uint32)quotient : (uint32)quotient;
            intensity = pair_low(magnitude,
                r_s8(a + 1689u) == 4 || r_s8(b + 1689u) == 4 ? 41u : 164u, 4096);
            w_u16(a + 42u, (uint16)(r_u16(a + 42u) + intensity));
            w_u16(b + 42u, (uint16)(r_u16(b + 42u) + intensity));
            if (r_s8(a + 1689u) == 4) {
                w_u16(b + 1972u, r_u16(normal + 2u));
                w_u8(b + 1u, 0);
                w_u8(b + 1706u, (uint8)side);
                w_u16(b + 1974u, (uint16)(0u - (uint32)(sint16)r_u16(normal)));
                gtmain_race_contact_response(b);
                weight_a = 0; weight_b = 8192;
                w_u8(a + 1698u, r_u8(a + 1698u) | 2u);
                w_u8(b + 1698u, r_u8(b + 1698u) | 1u);
            } else {
                if (r_s8(b + 1689u) == 4) {
                    weight_a = 8192; weight_b = 0;
                    w_u8(a + 1698u, r_u8(a + 1698u) | 1u);
                } else w_u8(a + 1698u, r_u8(a + 1698u) | 2u);
                w_u8(b + 1698u, r_u8(b + 1698u) | 2u);
            }
            for (axis = 0; axis < 3; ++axis) {
                uint32 offset = 1712u + axis * 4u;
                w_u32(a + offset, r_u32(a + offset) - pair_wide((sint16)weight_a, (sint32)impulse[axis]));
                w_u32(b + offset, r_u32(b + offset) + pair_wide((sint16)weight_b, (sint32)impulse[axis]));
            }
            for (axis = 0; axis < 3; ++axis) {
                sint32 shove = (sint32)pair_low((uint32)(sint32)(sint16)r_u16(normal + axis * 2u), 5690, 4096);
                uint32 offset = 1712u + axis * 4u;
                w_u32(a + offset, r_u32(a + offset) - pair_wide((sint16)weight_a, shove));
                w_u32(b + offset, r_u32(b + offset) + pair_wide((sint16)weight_b, shove));
            }
            {
                uint32 rate = r_u32(0x800BF514u);
                for (axis = 0; axis < 3; ++axis) {
                    uint32 displacement = pair_wide(overlap, (sint32)impulse[axis]);
                    delta[axis] = (sint16)(uint16)pair_low(rate, displacement, 65536);
                }
            }
            if (r_s8(a + 1689u) == 4 || r_s8(b + 1689u) == 4) {
                for (axis = 0; axis < 3; ++axis) {
                    uint32 offset = 60u + axis * 4u;
                    w_u32(a + offset, r_u32(a + offset) - pair_wide((sint16)weight_a, delta[axis]));
                    w_u32(b + offset, r_u32(b + offset) + pair_wide((sint16)weight_b, delta[axis]));
                }
                for (axis = 0; axis < 3; ++axis) {
                    sint32 shove = (sint32)(sint16)r_u16(normal + axis * 2u) >> 5;
                    uint32 offset = 60u + axis * 4u;
                    w_u32(a + offset, r_u32(a + offset) - pair_low(weight_a, (uint32)shove, 4096));
                    w_u32(b + offset, r_u32(b + offset) + pair_low(weight_b, (uint32)shove, 4096));
                }
            } else {
                uint32 fraction_a, fraction_b, result_a, result_b;
                for (axis = 0; axis < 3; ++axis) w_u32(0x1F8000B4u + axis * 4u,
                    0u - pair_wide((sint16)weight_a, delta[axis]));
                for (axis = 0; axis < 3; ++axis) w_u32(0x1F8000C4u + axis * 4u,
                    pair_wide((sint16)weight_b, delta[axis]));
                w_u32(0x1F8000C0u, 0);
                w_u32(0x1F8000D0u, 0);
                for (axis = 0; axis < 3; ++axis) {
                    sint32 shove = (sint32)(sint16)r_u16(normal + axis * 2u) >> 3;
                    w_u32(0x1F8000B4u + axis * 4u, r_u32(0x1F8000B4u + axis * 4u) - pair_wide((sint16)weight_a, shove));
                    w_u32(0x1F8000C4u + axis * 4u, r_u32(0x1F8000C4u + axis * 4u) + pair_wide((sint16)weight_b, shove));
                }
                result_a = gtmain_race_controls_query(a, (const uint8 *)psx_addr(0x1F8000B4u, 16u), &fraction_a, 2);
                result_b = gtmain_race_controls_query(b, (const uint8 *)psx_addr(0x1F8000C4u, 16u), &fraction_b, 2);
                if (result_a && !result_b) {
                    sint32 scale = (sint32)pair_low(weight_a, fraction_a, 4096);
                    for (axis = 0; axis < 3; ++axis) w_u32(b + 60u + axis * 4u,
                        r_u32(b + 60u + axis * 4u) + pair_wide(scale, delta[axis]));
                } else if (!result_a && result_b) {
                    sint32 scale = (sint32)pair_low(weight_b, fraction_b, 4096);
                    for (axis = 0; axis < 3; ++axis) w_u32(a + 60u + axis * 4u,
                        r_u32(a + 60u + axis * 4u) - pair_wide(scale, delta[axis]));
                }
            }
        }
    }
}
