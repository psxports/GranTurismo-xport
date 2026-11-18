#include "game_first2_registry.h"

/* TODO Complete the independent driver configuration callee 8001E988 */
/* GTMAIN.EXE 8002A570 */
/* Original: GTMAIN.EXE:sub_8002A570. */ uint32 gtmain_race_record_selection_prepare(void)
{
    uint32 record = gtmain_race_mode_record();
    uint32 course_name = r_u32(record + 12u);
    uint32 car_name = r_u32(record + 8u), result;
    w_u8(0x800B63BCu, (uint8)gtmain_course_name_find(course_name));
    w_u16(0x800CB7FAu, (uint16)gtmain_car_find(car_name));
    w_u8(0x800CB7FCu, 0);
    w_u8(0x800CB7FDu, 0);
    w_u8(0x800CB7FEu, 0);
    w_u8(0x800CB7FFu, 0);
    w_u8(0x800B6394u, 1);
    gtmain_race_track_invalidate();
    gtmain_race_driver_configurations(0x800D6FDCu, 1, 4, 3);
    gtmain_course_route_caption_update();
    result = r_u32(record) == 4u ? 5u : 1u;
    w_u16(0x800B634Cu, (uint16)result);
    return result;
}
