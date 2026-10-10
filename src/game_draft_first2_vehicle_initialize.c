#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

/* Unverified native draft from complete GTMAIN pseudocode and MIPS */
static uint32 unresolved1(uint32 address, uint32 value)
{
    return gt_draft_unresolved_call(address, value, 0, 0, 0, 0, 0, 0, 0);
}

static void zero_bytes(uint32 destination, uint32 count)
{
    for (uint32 i = 0; i < count; ++i) w_u8(destination + i, 0);
}

uint32 gtmain_camera_resource_path(void)
{
    return gtmain_environment_value(0x8008B894u);
}

uint32 gtmain_race_simulation_configure(void)
{
    gtmain_race_speed_percent(r_u8(0x800CB7F4u) == 1 ? 125 : 100);
    uint32 mode = r_u8(0x800CB7F5u);
    switch (mode) {
    case 0: case 4:
        gtmain_race_balance_config(mode == 0 ? 1u : 0u);
        gtmain_race_assist_preset(0);
        break;
    case 1:
        gtmain_race_balance_config(0);
        break;
    case 2: case 3: case 5:
        gtmain_race_balance_config(1);
        break;
    default:
        break;
    }
    return gtmain_race_tuning_initialize();
}

void gtmain_race_controls_initialize(uint32 controls, uint32 preserve_history)
{
    if (!controls) return;
    if (preserve_history) {
        zero_bytes(controls + 18u, 5);
        uint32 previous = r_u32(controls);
        w_u16(controls + 12u, 0); w_u16(controls + 14u, 0);
        w_u32(controls + 4u, previous);
    } else zero_bytes(controls, 8216);
    w_u32(controls + 8u, 0xffffffffu);
}

sint32 gtmain_race_points_print(uint32 guest_label)
{
    printf((const char *)psx_addr(0x8008B980u, 1u),
        (const char *)psx_addr(guest_label, 1u));
    for (uint32 i = 0; i < 6u; ++i)
        printf((const char *)psx_addr(0x8008B98Cu, 1u), (int)r_u16(0x800B6840u + i * 2u));
    return printf((const char *)psx_addr(0x8008B990u, 1u));
}

sint32 gtmain_race_points_apply(void)
{
    sint32 count = (sint16)r_u16(0x800B6666u);
    for (sint32 i = 0; i < count; ++i)
        w_u16(0x800B6850u + (uint32)i * 3380u + 38u, r_u16(0x800B6840u + (uint32)i * 2u));
    return gtmain_race_points_print(0x8008B99Cu);
}

uint32 gtmain_race_vehicle_variant_set(uint32 car, sint8 variant)
{
    sint32 mode = (sint8)r_u8(car + 1689u);
    if (mode == 2) { w_u8(car + 1461u, 0); return 2; }
    if (mode < 3) {
        if (!mode) w_u8(car + 1461u, (uint8)variant);
        return 1;
    }
    if (mode == 4) w_u8(car + 1461u, 0);
    return 4;
}

uint32 gtmain_race_ai_default(uint32 offset)
{
    return r_u32(r_u32(0x800BF51Cu) + 24u) ? offset + 3u : 0;
}

static void basis_initialize(uint32 car)
{
    sint32 first = (sint16)r_u16(car + 1786u);
    sint32 second = (sint16)r_u16(car + 1788u);
    sint32 third = (sint16)r_u16(car + 1790u);
    gtmain_race_basis_set(car + 72u, car + 80u, car + 88u, first, second, third);
}

uint32 gtmain_race_vehicle_grid_initialize(uint32 car, uint32 segment, sint32 x, sint32 z, sint32 sine, sint32 cosine)
{
    w_u32(car + 1892u, segment);
    sint32 height = gtmain_collision_height_query(x, z, 409600, (uint32 *)psx_addr(car + 1892u, 4));
    sint32 suspension = (sint16)r_u16(car + 438u);
    w_u32(car + 60u, (uint32)x); w_u32(car + 64u, (uint32)z);
    w_u16(car + 1786u, 0); w_u16(car + 1788u, 0);
    uint32 y = (uint32)height + (uint32)suspension;
    w_u32(car + 68u, y);
    w_u16(car + 1790u, (uint16)gtmain_angle_atan_wide((sint32)(0u - (uint32)sine), cosine));
    uint8 position[12];
    xport_store_le32(position, (uint32)x << 4);
    xport_store_le32(position + 4u, (uint32)height << 4);
    xport_store_le32(position + 8u, (uint32)z << 4);
    w_u32(car + 1804u, (uint32)gtmain_course_progress(r_u32(car + 1892u), position));
    basis_initialize(car);
    if ((sint8)r_u8(car + 1689u) == 4) {
        /* TODO Original alternate coordinate basis owner */
        gt_draft_unresolved_call(0x8005F260u, car + 60u, car + 72u, car + 80u, 0, 0, 0, 0, 0);
    }
    gtmain_race_wheel_pose(car);
    uint32 flags = 0;
    uint8 query[24];
    for (uint32 i = 0; i < 4u; ++i) {
        uint32 wheel = car + 132u + i * 68u;
        xport_store_le32(query + 8u, ((uint32)(sint32)(sint16)r_u16(wheel + 8u) + r_u32(car + 60u)) << 4);
        xport_store_le32(query + 12u, ((uint32)(sint32)(sint16)r_u16(wheel + 12u) + r_u32(car + 68u)) << 4);
        xport_store_le32(query + 16u, ((uint32)(sint32)(sint16)r_u16(wheel + 10u) + r_u32(car + 64u)) << 4);
        xport_store_le16(query + 4u, r_u16(car + 1892u));
        gtmain_collision_query(query);
        uint32 ground = xport_load_le32(query + 20u);
        if (ground == 0x7fffffffu) {
            sint32 offset = (sint16)r_u16(wheel + 12u);
            w_u8(wheel + 44u, 0); w_u8(wheel + 45u, 0);
            w_u32(wheel + 16u, (uint32)height + (uint32)offset);
        } else {
            w_u32(wheel + 16u, (uint32)((sint32)ground >> 4));
            w_u8(wheel + 44u, query[0]); w_u8(wheel + 45u, query[1]);
            flags += query[2] & 2u;
        }
    }
    uint8 status = r_u8(car + 58u);
    w_u8(car + 58u, flags ? status | 16u : status & 239u);
    for (uint32 i = 0; i < 4u; ++i) {
        uint32 wheel = car + 132u + i * 68u;
        sint32 stiffness = (sint32)r_u32(car + (i < 2u ? 1648u : 1652u));
        uint32 base = r_u32(car + (i < 2u ? 768u : 844u));
        uint32 value = (uint32)(stiffness / 2) + base;
        w_u32(wheel + 20u, value);
        uint32 readback = r_u32(wheel + 20u);
        w_u32(wheel + 24u, value); w_u32(wheel + 28u, readback);
        w_u16(wheel + 14u, r_u16(car + (i < 2u ? 700u : 776u)));
        w_u16(wheel + 4u, r_u16(car + (i < 2u ? 702u : 778u)));
    }
    w_u8(car + 1961u, 1);
    gtmain_race_vehicle_bounds_update(car);
    gtmain_race_vehicle_bounds_copy(car);
    gtmain_race_vehicle_pose_update(car);
    uint16 pitch = r_u16(car + 1900u), roll = r_u16(car + 1902u);
    w_u16(car + 1788u, roll); w_u16(car + 1786u, pitch);
    basis_initialize(car);
    gtmain_race_wheel_pose(car);
    return 0;
}

void gtmain_race_vehicle_initialize(uint32 car, uint32 specification, uint32 segment, sint32 x, sint32 z,
    sint32 sine, sint32 cosine, uint32 body, uint8 variant, uint8 mode, uint32 transmission)
{
    gtmain_race_car_mode_set(car, (sint8)body);
    gtmain_race_specification_initialize(car + 404u, specification, (sint8)r_u8(car + 1689u), 1, (uint8)transmission);
    w_u8(car + 58u, 0);
    gtmain_race_vehicle_grid_initialize(car, segment, x, z, sine, cosine);
    gtmain_race_gear_reset(car); gtmain_race_vehicle_reset(car);
    gtmain_race_wheel_surface_reset(car); gtmain_race_speed_apply(car);
    w_u16(car + 1700u, body ? 65535u : 4096u);
    switch (r_u8(car + 1460u)) {
    case 0: w_u32(car + 1708u, 0x80093C70u); break;
    case 1: case 2: w_u32(car + 1708u, 0x80093C88u); break;
    case 3: w_u32(car + 1708u, 0x80093CA0u); break;
    case 4: w_u32(car + 1708u, 0x80093CB8u); break;
    default: break;
    }
    gtmain_race_vehicle_variant_set(car, (sint8)variant);
    w_u8(car + 1963u, mode);
    uint32 course = r_u32(0x800BC1A8u);
    w_u16(car + 56u, 0);
    uint32 half_length = r_u32(course) >> 1;
    if (r_u32(car + 1804u) < half_length) w_u16(car + 56u, 1);
    w_u32(car + 44u, 0); w_u8(car + 59u, 0);
    w_u8(car + 1781u, 0); w_u8(car + 1780u, 0); w_u8(car + 1688u, 0);
    w_u16(car + 12u, 0); w_u8(car + 1960u, 0); w_u32(car + 1968u, 0);
    w_u8(car + 1964u, 0); w_u8(car + 1965u, 0);
    w_u32(car + 1916u, r_u32(car + 848u));
    uint32 value = r_u32(car + 848u);
    w_u8(car, 0); w_u32(car + 1920u, value);
    w_u8(car + 1695u, (uint8)gtmain_race_ai_default(0));
    w_u8(car + 1697u, 1);
    sint32 ai_mode = gtmain_race_ai_mode_select(6);
    gtmain_race_ai_mode_set(car, (uint8)ai_mode);
}
