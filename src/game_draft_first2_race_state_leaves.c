#include "game_first2_registry.h"

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

/* Original: GTMAIN.EXE:sub_8002DD70. */ uint32 gtmain_race_split_header_clear(void)
{
    return sub_8007BD58(0x800BDB50u, 0u, 20u);
}

/* Original: GTMAIN.EXE:sub_8005142C. */ sint32 gtmain_race_wheel_state_init(uint32 car)
{
    uint32 i;
    for (i = 0; i < 4u; ++i) {
        uint32 wheel = car + 0x84u + 68u * i;
        w_u8(wheel, 1u);
        w_u8(wheel + 1u, 0u);
        w_u16(wheel + 0x36u, 0x1000u);
        w_u8(wheel + 2u, 0u);
        w_u8(wheel + 3u, 0u);
        w_u16(wheel + 6u, 0u);
        w_u16(wheel + 0x2Eu, 0u);
        w_u16(wheel + 0x30u, 0u);
        w_u16(wheel + 0x32u, 0u);
        w_u16(wheel + 0x34u, 0u);
        w_u32(wheel + 0x38u, 0u);
        w_u32(wheel + 0x3Cu, 0u);
        w_u32(wheel + 0x40u, 0u);
    }
    return 0;
}

/* Original: GTMAIN.EXE:sub_800517EC. */ uint32 gtmain_race_gear_reset(uint32 car)
{
    uint32 gear = r_u16(car + 0x6FEu);
    w_u16(car + 0x68Eu, 0u);
    w_u16(car + 0x28u, 0u);
    w_u16(car + 0x68Cu, gear);
    return gear;
}

/* Original: GTMAIN.EXE:sub_8004EEE8. */ void gtmain_race_car_kind_set(uint32 car, uint8 kind)
{
    w_u8(car + 0x1Cu, kind);
}

/* Original: GTMAIN.EXE:sub_8004EEF0. */ sint32 gtmain_race_car_mode_set(uint32 car, sint8 mode)
{
    sint32 result;
    w_u8(car + 0x699u, (uint8)mode);
    result = mode < 3;
    if (mode == 2 || mode == 0) {
        w_u8(car + 0x6A0u, 0u);
        return result;
    }
    if (mode < 3) return result;
    if (mode == 4) w_u8(car + 0x6A0u, 7u);
    return 7;
}
