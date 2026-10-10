#include "game_first2_registry.h"

static sint32 simulation_fixed(sint64 value) { return (sint32)(uint32)(value / 4096); }
static sint32 simulation_divide(sint32 value, sint32 denominator, uint32 caller)
{
    return (sint32)(uint32)gtmain_signed_divide64((sint64)value * 4096, denominator, caller);
}

/* GTMAIN.EXE 80059AE4 */
void gtmain_race_slip_ratio(uint32 row, sint32 reference, sint32 speed)
{
    sint32 difference = (sint32)((uint32)reference - (uint32)speed), value;
    w_u32(row, (uint32)speed); w_u8(row + 20u, 0);
    if ((uint32)speed + 568u < 1137u) {
        if (speed < reference) { w_u16(row + 24u, 4096); w_u8(row + 21u, 255); }
        else if (reference < speed) { w_u16(row + 24u, 4096); w_u8(row + 21u, 1); }
        else { w_u16(row + 24u, 0); w_u8(row + 21u, 0); }
        w_u8(row + 20u, 255);
    } else if (speed > 0) {
        if (speed < reference) {
            if ((uint32)reference + 568u < 1137u) w_u16(row + 24u, 0);
            else w_u16(row + 24u, (uint16)simulation_divide(difference, reference, 0x80059BC8u));
            w_u8(row + 21u, 255); w_u8(row + 20u, 255);
        } else {
            w_u16(row + 24u, (uint16)simulation_divide(difference, speed, 0x80059B80u));
            w_u8(row + 21u, 1); w_u8(row + 20u, 1);
        }
    } else if (reference < speed) {
        if ((uint32)reference + 568u < 1137u) w_u16(row + 24u, 0);
        else w_u16(row + 24u, (uint16)simulation_divide(difference, reference, 0x80059C58u));
        w_u8(row + 21u, 1); w_u8(row + 20u, 255);
    } else {
        w_u16(row + 24u, (uint16)simulation_divide(difference, speed, 0x80059C08u));
        w_u8(row + 21u, 255); w_u8(row + 20u, 1);
    }
    if (r_s16(row + 24u) > 4096) w_u16(row + 24u, 4096);
    else if (r_s16(row + 24u) < -4096) w_u16(row + 24u, (uint16)-4096);
    if (!r_s8(row + 20u)) { w_u16(row + 24u, 0); return; }
    value = r_s8(row + 20u) > 0 ? speed : reference;
    if (value < 0) value = (sint32)(0u - (uint32)value);
    if (value < 22760) {
        value = simulation_divide(value, 22760, 0x80059CF4u);
        value = (sint32)((uint32)value * (uint32)r_s16(row + 24u));
        w_u16(row + 24u, (uint16)(value / 4096));
    }
}

/* GTMAIN.EXE 8005A780 */
void gtmain_race_axle_slip_update(uint32 car, sint32 index)
{
    uint32 scratch = 0x1F800000u + 84u * (uint32)index, reference, value, offset;
    sint32 axle, coefficient, sum;
    reference = r_u32(scratch + 76u); value = r_u32(scratch + 76u);
    w_u32(scratch + 80u, value);
    gtmain_race_slip_ratio(scratch + 4u, (sint32)reference, r_s32(car + 1732u));
    gtmain_race_slip_ratio(scratch + 32u, r_s32(scratch + 76u), r_s32(car + 1736u));
    value = r_u16(scratch + 28u); w_u16(car + 248u, (uint16)value); w_u16(car + 180u, (uint16)value);
    value = r_u16(scratch + 56u); w_u16(car + 384u, (uint16)value); w_u16(car + 316u, (uint16)value);
    value = (uint32)gtmain_race_curve16(car + 940u, r_s16(scratch + 28u));
    coefficient = r_s16(scratch + 56u); w_u16(scratch + 30u, (uint16)value);
    value = (uint32)gtmain_race_curve16(car + 1004u, coefficient);
    coefficient = r_s8(scratch + 24u); w_u16(scratch + 58u, (uint16)value);
    for (axle = 0; axle < 2; ++axle) {
        offset = 28u * (uint32)axle;
        if (axle) coefficient = r_s8(scratch + offset + 24u);
        if (coefficient > 0) {
            coefficient = r_s16(scratch + offset + 30u);
            value = r_u32(car + 224u + 136u * (uint32)axle);
            sum = (sint32)(r_u32(car + 156u + 136u * (uint32)axle) + value);
            w_u32(scratch + offset + 16u, (uint32)simulation_fixed((sint64)coefficient * sum));
            if (r_s8(scratch + offset + 25u) == -1) w_u32(scratch + offset + 16u, 0u - r_u32(scratch + offset + 16u));
            w_u32(scratch + offset + 20u, 0);
        } else {
            coefficient = r_s16(scratch + offset + 30u); w_u32(scratch + offset + 16u, 0);
            value = r_u32(car + 224u + 136u * (uint32)axle);
            sum = (sint32)(r_u32(car + 156u + 136u * (uint32)axle) + value);
            w_u32(scratch + offset + 20u, (uint32)simulation_fixed((sint64)coefficient * sum));
            if (r_s8(scratch + offset + 25u) == -1) w_u32(scratch + offset + 20u, 0u - r_u32(scratch + offset + 20u));
        }
    }
    w_u32(car + 1760u, r_u32(scratch + 16u)); w_u32(car + 1764u, r_u32(scratch + 44u));
    w_u32(car + 1768u, r_u32(scratch + 20u)); w_u32(car + 1772u, r_u32(scratch + 48u));
}

/* GTMAIN.EXE 8005AD5C */
void gtmain_race_slip_publish(uint32 car, sint32 index)
{
    uint32 scratch = 0x1F800000u + 84u * (uint32)index;
    sint32 value = r_s16(scratch + 28u);
    if (value < 0) value = -value;
    w_u16(car + 232u, (uint16)value); w_u16(car + 164u, (uint16)value);
    value = r_s16(scratch + 56u); if (value < 0) value = -value;
    w_u16(car + 368u, (uint16)value); w_u16(car + 300u, (uint16)value);
    value = r_s16(scratch + 28u) / 32; if (value < -128) value = -128; if (value > 127) value = 127;
    w_u8(car + 203u, (uint8)value); w_u8(car + 135u, (uint8)value);
    value = r_s16(scratch + 56u) / 32; if (value < -128) value = -128; if (value > 127) value = 127;
    w_u8(car + 339u, (uint8)value); w_u8(car + 271u, (uint8)value);
}

/* GTMAIN.EXE 8005AE80 */
void gtmain_race_axle_velocity_integrate(uint32 car, sint32 index, sint32 axle)
{
    uint32 parameters = car + 404u, row = 0x1F800000u + 84u * (uint32)index + 28u * (uint32)axle;
    uint32 axis = car + 1732u + 4u * (uint32)axle, gear, first, second;
    sint32 value, force, coefficient, current, updated;
    if (r_u8(car + 1784u + (uint32)axle) & 8u) {
        first = r_u32(row + 16u); second = r_u32(row + 20u); force = (sint32)(0u - (first + second));
        coefficient = r_s16(parameters + 1156u + 2u * (uint32)axle);
        value = simulation_fixed((sint64)coefficient * force);
        force = r_s32(row + 8u); gear = r_u8(car + 32u);
        value = simulation_divide((sint32)((uint32)value + (uint32)force), r_s32(parameters + 1092u + 4u * gear), 0x8005AFE8u);
        coefficient = r_s32(parameters + 1180u + 4u * gear);
        value = simulation_fixed((sint64)(sint32)((uint32)value << 10) * coefficient);
    } else if (r_u8(car + 1784u + (uint32)axle) & 4u) {
        first = r_u32(row + 16u); second = r_u32(row + 20u); force = (sint32)(0u - (first + second));
        coefficient = r_s16(parameters + 1156u + 2u * (uint32)axle);
        value = simulation_fixed((sint64)coefficient * force);
        value = (sint32)((uint32)value + r_u32(row + 8u)); gear = r_u8(car + 32u);
        value = simulation_divide(value, r_s32(parameters + 1092u + 4u * gear), 0x8005B164u);
        value = simulation_divide(value, r_s16(parameters + 1176u + 2u * (uint32)axle), 0x8005B1A0u);
    } else goto braking;
    value = (sint32)(uint32)(((sint64)value * r_s32(0x1F800000u)) / 65536);
    coefficient = r_s16(parameters + 1156u + 2u * (uint32)axle);
    w_u32(axis, r_u32(axis) + (uint32)simulation_fixed((sint64)value * coefficient));
braking:
    if (!(r_u8(car + 1784u + (uint32)axle) & 2u)) return;
    value = r_s32(row + 12u); gear = r_u8(car + 32u);
    value = simulation_divide(value, r_s32(parameters + 1092u + 4u * gear), 0x8005B2F8u);
    value = simulation_divide(value, r_s16(parameters + 1176u + 2u * (uint32)axle), 0x8005B334u);
    value = (sint32)(uint32)(((sint64)value * r_s32(0x1F800000u)) / 65536);
    coefficient = r_s16(parameters + 1156u + 2u * (uint32)axle);
    force = simulation_fixed((sint64)value * coefficient); current = r_s32(axis);
    if (current > 0) {
        updated = (sint32)((uint32)current - (uint32)force); w_u32(axis, (uint32)updated); if (updated < 0) w_u32(axis, 0);
    } else {
        current = r_s32(axis);
        if (current < 0) { updated = (sint32)((uint32)force + (uint32)current); w_u32(axis, (uint32)updated); if (updated > 0) w_u32(axis, 0); }
    }
}

static void simulation_clamp(uint32 car, sint32 index, int self_store)
{
    uint32 scratch = 0x1F800000u + 84u * (uint32)index, axle, axis;
    sint32 previous, current, first, second;
    for (axle = 0; axle < 2; ++axle) {
        axis = car + 1732u + 4u * axle;
        if (!r_u8(car + 1784u + axle)) { if (self_store) w_u32(axis, r_u32(axis)); continue; }
        first = r_s32(scratch + 4u + 28u * axle); second = r_s32(scratch + 80u);
        if (first < second) {
            current = r_s32(axis); previous = r_s32(scratch + 76u);
            if (previous < current) { w_u32(axis, (uint32)previous); continue; }
        }
        first = r_s32(scratch + 4u + 28u * axle); second = r_s32(scratch + 80u);
        if (second < first) {
            current = r_s32(axis); previous = r_s32(scratch + 76u);
            if (current < previous) w_u32(axis, (uint32)previous);
        }
    }
}

static void simulation_missing(uint32 owner, uint32 car, sint32 index)
{
    switch (owner) {
    case 0x8005BDECu: gtmain_race_slip_curve_update(car); break;
    case 0x8005C65Cu: gtmain_race_lateral_force_update(car); break;
    case 0x8005CD5Cu: gtmain_race_lateral_drag_update(car, index); break;
    case 0x8005BB54u: gtmain_race_longitudinal_integrate(car, index); break;
    case 0x8005B470u: gtmain_race_engine_velocity_update(car, index); break;
    case 0x8005D114u: gtmain_race_yaw_integrate(car, index); break;
    case 0x8005D478u: gtmain_race_world_velocity_integrate(car); break;
    default:
        /* TODO: Bind the unrecorded final simulation dependency 8005D718 */
        gt_draft_unresolved_call(owner, car, 0, 0, 0, 0, 0, 0, 0); break;
    }
}

/* GTMAIN.EXE 8005DC70 */
void gtmain_race_simulation_step(uint32 nodes, sint32 count)
{
    sint32 index, axle, speed, coefficient, sum;
    uint32 node, car, scratch, wheel, first, second, stage;
    static const uint32 missing[] = {0x8005BDECu, 0x8005C65Cu, 0x8005CD5Cu, 0x8005BB54u};
    for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; car = node + 40u; scratch = 0x1F800000u + 84u * (uint32)index;
        w_u16(scratch + 68u, r_u16(0x8009423Cu + 2u * (r_u16(node + 54u) & 4095u)));
        w_u16(scratch + 70u, r_u16(0x80094A3Cu + 2u * (r_u16(node + 54u) & 4095u)));
        w_u32(0x1F800000u, r_u32(node + 44u)); gtmain_race_drive_forces_update(car, (uint32)index);
        w_u32(node + 1980u, 0); w_u32(node + 1988u, 0); w_u32(node + 1984u, 0); w_u32(node + 1964u, 0);
        gtmain_race_traction_apply(car, (uint32)index); sum = 0;
        for (wheel = 0; wheel < 4; ++wheel) {
            first = r_u8(car + 176u + 68u * wheel); second = r_u32(car + 152u + 68u * wheel);
            coefficient = r_s32(0x80093E5Cu + 4u * first);
            sum = (sint32)((uint32)sum + (uint32)simulation_fixed((sint64)(sint32)second * coefficient));
        }
        speed = r_s32(car + 1724u);
        if (speed > 0) coefficient = (sint32)(0u - (uint32)gtmain_race_curve32(0x800BF530u, speed));
        else { speed = r_s32(car + 1724u); coefficient = speed < 0 ? gtmain_race_curve32(0x800BF530u, (sint32)(0u - (uint32)speed)) : 0; }
        w_u32(scratch + 84u, (uint32)simulation_fixed((sint64)coefficient * sum));
    }
    for (index = 0; index < count; ++index) w_u32(0x1F80004Cu + 84u * (uint32)index, r_u32(nodes + 3380u * (uint32)index + 1764u));
    for (index = 0; index < count; ++index) gtmain_race_axle_slip_update(nodes + 3380u * (uint32)index + 40u, index);
    for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; w_u32(0x1F800000u, (uint32)(r_s32(node + 44u) >> 1));
        for (axle = 0; axle < 2; ++axle) gtmain_race_axle_velocity_integrate(node + 40u, index, axle);
    }
    for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; w_u32(0x1F800000u, (uint32)(r_s32(node + 44u) >> 1)); simulation_clamp(node + 40u, index, 1);
    }
    for (index = 0; index < count; ++index) gtmain_race_axle_slip_update(nodes + 3380u * (uint32)index + 40u, index);
    for (index = 0; index < count; ++index) { node = nodes + 3380u * (uint32)index; w_u32(0x1F800000u, r_u32(node + 44u)); gtmain_race_slip_publish(node + 40u, index); }
    for (stage = 0; stage < 4; ++stage) for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; w_u32(0x1F800000u, r_u32(node + 44u)); simulation_missing(missing[stage], node + 40u, index);
    }
    for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; first = r_u32(node + 44u); w_u32(0x1F800000u, first - (uint32)((sint32)first >> 1));
        for (axle = 0; axle < 2; ++axle) gtmain_race_axle_velocity_integrate(node + 40u, index, axle);
        simulation_clamp(node + 40u, index, 0);
    }
    for (stage = 0; stage < 3; ++stage) for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; w_u32(0x1F800000u, r_u32(node + 44u));
        simulation_missing(stage == 0 ? 0x8005B470u : stage == 1 ? 0x8005D114u : 0x8005D478u, node + 40u, index);
    }
    if (r_u32(0x80093E30u)) for (index = 0; index < count; ++index) {
        node = nodes + 3380u * (uint32)index; if (r_u8(node + 98u) & 16u) continue;
        w_u32(0x1F800000u, r_u32(node + 44u)); simulation_missing(0x8005D718u, node + 40u, index);
    }
}
