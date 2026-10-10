#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified native draft from complete GTMAIN pseudocode and MIPS */
static sint32 product_low(uint32 first, uint32 second)
{
    return (sint32)(first * second);
}

static sint32 fixed_product(uint32 first, uint32 second)
{
    return product_low(first, second) / 4096;
}

static sint32 divide_guarded(sint32 numerator, sint32 denominator, uint32 owner)
{
    if (!denominator) gt_draft_failure("GTMAIN race tuning BREAK7", owner);
    if ((uint32)numerator == 0x80000000u && denominator == -1)
        gt_draft_failure("GTMAIN race tuning BREAK6", owner);
    return numerator / denominator;
}

uint32 gtmain_race_engine_tuning_initialize(void)
{
    uint32 source = r_u8(0x80093DE4u);
    uint32 denominator = r_u8(0x80093DE4u);
    w_u32(0x80093D7Cu, (source << 12) / 10u);
    if (denominator) w_u32(0x80093D80u, (r_u8(0x80093DE5u) << 12) / denominator);
    else w_u32(0x80093D80u, 0);
    uint32 count = r_u8(0x80093DE6u);
    w_u32(0x80093DC4u, count);
    if (count) {
        sint32 limit = (sint32)r_u32(0x80093DC4u);
        for (sint32 i = 0; i < limit; ++i) {
            w_u32(0x80093D84u + (uint32)i * 4u, 2276u * r_u8(0x80093DE7u + (uint32)i));
            w_u32(0x80093DA4u + (uint32)i * 4u, (r_u8(0x80093DEFu + (uint32)i) << 12) / 1000u);
        }
    }
    for (uint32 i = 0; i < 8u; ++i) w_u16(0x80093E98u + i * 2u, (uint16)((r_u8(0x80093DF7u + i) << 12) / 100u));
    for (uint32 i = 0; i < 8u; ++i) w_u16(0x80093E4Cu + i * 2u, (uint16)((r_u8(0x80093DFFu + i) << 12) / 100u));
    for (uint32 i = 0; i < 8u; ++i) w_u32(0x80093E5Cu + i * 4u, (r_u8(0x80093E07u + i) << 12) / 10u);
    uint32 first = r_u8(0x80093E0Fu) << 12;
    uint32 second = r_u8(0x80093E10u) << 12;
    w_u8(0x800BF532u, 0); w_u8(0x800BF533u, 0);
    w_u32(0x800BF534u, 0x80093D84u); w_u32(0x800BF538u, 0x80093DA4u);
    uint32 third = r_u8(0x80093E11u) << 12;
    uint32 a = r_u8(0x80093E13u) << 12, b = r_u8(0x80093E14u) << 12;
    w_u32(0x80093DD8u, a); w_u32(0x80093DDCu, b);
    uint32 c = r_u8(0x80093E15u) << 12;
    uint16 published_count = r_u16(0x80093DC4u);
    w_u32(0x80093DE0u, c); w_u16(0x800BF530u, published_count);
    uint32 fourth = r_u8(0x80093E12u) << 12;
    w_u32(0x80093DC8u, first / 100u); w_u32(0x80093DCCu, second / 10u);
    w_u32(0x80093DD0u, third / 100u);
    uint32 result = fourth / 100u;
    w_u32(0x80093DD4u, result);
    return result;
}

uint32 gtmain_race_tuning_initialize(void)
{
    gtmain_race_engine_tuning_initialize();
    for (uint32 i = 0; i < 4u; ++i) {
        uint32 source = i * 10u, row = i * 24u;
        w_u16(0x80093C78u + row, (uint16)((r_u8(0x80093C12u + source) << 12) / 100u));
        w_u32(0x80093C80u + row, r_u8(0x80093C16u + source) << 16);
        w_u32(0x80093C84u + row, r_u8(0x80093C17u + source) << 16);
        sint32 denominator = (sint32)(2276u * r_u8(0x80093C15u + source));
        w_u16(0x80093C7Eu + row, (uint16)gtmain_signed_divide64(0x1000000, denominator, 0x800593C0u));
        w_u16(0x80093C70u + row, (uint16)((r_u8(0x80093C0Fu + source) << 12) / 100u));
        w_u16(0x80093C72u + row, (uint16)((r_u8(0x80093C10u + source) << 12) / 100u));
        w_u32(0x80093C74u + row, 1138u * (r_u8(0x80093C11u + source) - 100u));
        w_u16(0x80093C7Au + row, (uint16)((r_u8(0x80093C13u + source) << 12) / 100u));
        w_u16(0x80093C7Cu + row, (uint16)((r_u8(0x80093C14u + source) << 12) / 100u));
    }
    for (uint32 i = 0; i < 4u; ++i) {
        uint32 row = i * 24u, value;
        if (!r_u8(0x80093C61u)) {
            sint32 product = product_low((uint32)(sint32)(sint16)r_u16(0x80093C72u + row), r_u8(0x80093C58u + i));
            w_u16(0x80093C72u + row, (uint16)(product / 100));
            uint32 scale = r_u8(0x80093C58u + i), base = r_u32(0x80093C74u + row);
            value = (uint32)(product_low(scale, base) / 100);
        } else {
            uint32 scale = r_u8(0x80093C58u + i) - 100u;
            uint32 base = r_u32(0x80093C74u + row);
            value = 1138u * scale + base;
        }
        w_u32(0x80093C74u + row, value);
        sint32 product = product_low((uint32)(sint32)(sint16)r_u16(0x80093C70u + row), r_u8(0x80093C5Cu + i));
        w_u16(0x80093C70u + row, (uint16)(product / 100));
    }
    uint32 first = r_u8(0x80093C69u);
    if (first && r_u8(0x80093C6Eu) < first && r_u8(0x80093C6Fu) < r_u8(0x80093C6Au)
        && r_u8(0x80093C6Bu) < r_u8(0x80093C6Au)) {
        uint32 percentage = r_u8(0x80093C6Au) << 12;
        w_u32(0x80093E30u, first * 10000u);
        uint32 difference = (100u - r_u8(0x80093C6Bu)) << 12;
        uint32 c = r_u8(0x80093C6Du) << 12, d = r_u8(0x80093C6Eu);
        uint32 negative = 0u - r_u8(0x80093C6Cu);
        w_u32(0x80093E3Cu, negative * 10000u); w_u32(0x80093E44u, d * 10000u);
        uint32 e = r_u8(0x80093C6Fu) << 12;
        w_u32(0x80093E34u, percentage / 100u);
        w_u32(0x80093E38u, (uint32)((sint32)difference / 100));
        w_u32(0x80093E40u, c / 100u); w_u32(0x80093E48u, e / 100u);
    } else {
        for (uint32 i = 0; i < 7u; ++i) w_u32(0x80093E30u + i * 4u, 0);
    }
    uint32 frequency = r_u32(0x800BF518u);
    for (uint32 row = 0; row < 2u; ++row) {
        uint32 source = 0x80093C37u + row * 11u;
        for (uint32 column = 0; column < 6u; ++column) {
            sint32 denominator = product_low(r_u8(source + column), frequency);
            uint32 owner = 0x80059870u + column * 76u;
            sint32 quotient = divide_guarded(409600, denominator, owner);
            w_u16(0x80093F08u + column * 4u + row * 2u, (uint16)quotient);
        }
        for (uint32 column = 0; column < 6u; ++column) {
            uint32 address = 0x80093F08u + column * 4u + row * 2u;
            if ((sint16)r_u16(address) >= 4097) w_u16(address, 4096);
        }
    }
    return 0;
}

uint32 gtmain_race_assist_preset(uint32 preset)
{
    if (preset == 1) {
        w_u8(0x80093C6Cu, 5); w_u8(0x80093C6Eu, 30); w_u8(0x80093C69u, 35);
        w_u8(0x80093C6Du, 10); w_u8(0x80093C6Bu, 13); w_u8(0x80093C6Fu, 15); w_u8(0x80093C6Au, 25);
        return 25;
    }
    if (preset == 2) {
        w_u8(0x80093C6Cu, 3); w_u8(0x80093C69u, 18); w_u8(0x80093C6Du, 10);
        w_u8(0x80093C6Bu, 13); w_u8(0x80093C6Eu, 15); w_u8(0x80093C6Fu, 15); w_u8(0x80093C6Au, 25);
        return 25;
    }
    w_u8(0x80093C6Cu, 0); w_u8(0x80093C6Eu, 0); w_u8(0x80093C69u, 0);
    w_u8(0x80093C6Du, 0); w_u8(0x80093C6Bu, 0); w_u8(0x80093C6Fu, 0); w_u8(0x80093C6Au, 0);
    return 0x80093C6Cu;
}

uint32 gtmain_race_vehicle_bounds_update(uint32 car)
{
    uint32 angle = (r_u16(car + 1790u) & 4095u) * 2u;
    uint32 length = r_u32(car + 404u);
    sint32 sine = (sint16)r_u16(0x8009423Cu + angle);
    uint32 selector = r_u8(car + 1961u);
    sint32 cosine = (sint16)r_u16(0x80094A3Cu + angle);
    uint32 forward_x = 0u - (uint32)fixed_product((uint32)sine, length);
    uint32 forward_z = (uint32)fixed_product((uint32)cosine, r_u32(car + 404u));
    uint32 rear_length = r_u32(car + 408u);
    sint32 rear_product = product_low((uint32)sine, rear_length);
    uint32 rear_x = 0u - (uint32)(rear_product / 4096);
    if (rear_product < 0) rear_length = r_u32(car + 408u);
    uint32 rear_z = (uint32)fixed_product((uint32)cosine, rear_length);
    sint32 first_half = fixed_product((uint32)(sint32)(sint16)r_u16(car + 412u), (uint32)cosine) / 2;
    sint32 second_half = fixed_product((uint32)(sint32)(sint16)r_u16(car + 412u), (uint32)sine) / 2;
    uint32 destination = car + selector * 32u;
    w_u32(destination + 1824u, r_u32(car + 60u) - (uint32)first_half + forward_x);
    w_u32(destination + 1828u, r_u32(car + 64u) - (uint32)second_half + forward_z);
    w_u32(destination + 1832u, (uint32)first_half + r_u32(car + 60u) + forward_x);
    w_u32(destination + 1836u, (uint32)second_half + r_u32(car + 64u) + forward_z);
    w_u32(destination + 1840u, r_u32(car + 60u) - (uint32)first_half - rear_x);
    w_u32(destination + 1844u, r_u32(car + 64u) - (uint32)second_half - rear_z);
    w_u32(destination + 1848u, (uint32)first_half + r_u32(car + 60u) - rear_x);
    uint32 result = r_u32(car + 64u);
    w_u32(destination + 1852u, (uint32)second_half + result - rear_z);
    return result;
}

uint32 gtmain_race_vehicle_bounds_copy(uint32 car)
{
    uint32 selector = r_u8(car + 1961u);
    uint32 target = (1u - selector) << 5, source = selector << 5;
    for (uint32 i = 0; i < 4u; ++i) {
        uint32 from = car + source + i * 8u, to = car + target + i * 8u;
        w_u32(to + 1824u, r_u32(from + 1824u));
        w_u32(to + 1828u, r_u32(from + 1828u));
    }
    return 0;
}

void gtmain_race_vehicle_pose_update(uint32 car)
{
    uint32 rear = r_u32(car + 284u), front = r_u32(car + 148u);
    uint32 interpolation = (uint32)(sint32)(sint16)r_u16(car + 424u);
    sint32 first = fixed_product(rear - front, interpolation);
    uint32 second_rear = r_u32(car + 352u), second_front = r_u32(car + 216u);
    uint32 second_scale = (uint32)(sint32)(sint16)r_u16(car + 424u);
    sint32 second = fixed_product(second_rear - second_front, second_scale);
    uint32 first_height = (uint32)first + r_u32(car + 148u);
    uint32 x = r_u32(car + 60u), z = r_u32(car + 64u);
    uint32 front_sum = r_u32(car + 148u) + r_u32(car + 216u);
    w_u32(car + 96u, x);
    uint32 second_height = (uint32)second + r_u32(car + 216u);
    w_u32(car + 100u, z);
    w_u32(car + 104u, (uint32)((sint32)(first_height + second_height) / 2));
    uint32 back_sum = r_u32(car + 284u) + r_u32(car + 352u);
    sint32 length = (sint16)r_u16(car + 420u);
    sint32 pitch = gtmain_angle_atan_wide((sint32)(front_sum - back_sum) / 2, length);
    w_u16(car + 1900u, (uint16)pitch);
    sint32 width = (sint16)r_u16(car + 422u);
    sint32 roll = gtmain_angle_atan_wide((sint32)(first_height - second_height), width);
    w_u16(car + 1902u, (uint16)roll);
    gtmain_race_basis_set(car + 108u, car + 116u, car + 124u,
        (sint16)r_u16(car + 1900u), (sint16)(uint16)roll, (sint16)r_u16(car + 1790u));
}

sint32 gtmain_race_vehicle_initialization_finish(void)
{
    gtmain_race_event_tables_reset();
    gtmain_race_speed_tables_reset();
    gtmain_race_positions_init(0x800B6850u, (sint16)r_u16(0x800BC1A6u));
    gtmain_race_positions_sort(0x800B6850u, (sint16)r_u16(0x800BC1A6u));
    sint32 count = (sint16)r_u16(0x800BC1A6u);
    if (count <= 0) return count;
    uint32 index = 0;
    do {
        uint32 car = 0x800B6878u + index * 3380u;
        w_u8(car, (uint8)index);
        uint32 ai_default = gtmain_race_ai_default(index);
        sint16 throttle = (sint16)r_u16(car + 1700u);
        w_u8(car + 1695u, (uint8)ai_default);
        if (throttle == -1) {
            uint32 selector = (uint32)(sint32)(sint8)r_u8(car + 1697u);
            w_u16(car + 1700u, r_u16(0x80093E14u + selector * 4u));
        }
        count = (sint16)r_u16(0x800BC1A6u);
        ++index;
    } while ((sint32)index < count);
    return 0;
}
