#include "game_first2_registry.h"

/* TODO Bind unselected vehicle mode initializer */
static uint32 race_vehicle_mode_initialize(sint32 mode, uint32 car, uint32 first, uint32 second)
{
    gt_draft_failure("GTMAIN vehicle mode initializer", 0x80056E48u);
}

/* GTMAIN.EXE 80029AA8 */
uint32 gtmain_race_mode_vehicle_prepare(void)
{
    uint32 record, configuration, first = 0, second = 0;
    sint32 mode;
    w_u16(0x800B63D6u, 0);
    gtmain_driver_mode_set(0x800B6850u, 0x800B6410u, 2);
    w_u8(0x800B63D8u, 0);
    record = gtmain_race_mode_record();
    mode = (sint32)r_u32(record);
    w_u8(0x8008F893u, 0);
    if (mode == 3) {
        first = (uint32)r_s16(0x800B634Cu);
        second = 150;
    } else if (mode == 4) first = 5;
    else if (mode == 1) {
        configuration = r_u32(0x800BC1B0u);
        first = r_u32(configuration + 32u);
        second = r_u32(configuration + 36u);
    }
    return race_vehicle_mode_initialize(mode, 0x800B6878u, first, second);
}
