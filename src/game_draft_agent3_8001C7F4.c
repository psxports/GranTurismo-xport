#include "game_draft_registry.h"
#include "game_first2_registry.h"

uint32 gtmain_camera_car_select(uint32 camera, uint32 buttons)
{
    sint32 step = 0;
    uint32 result = 2u;
    if (r_u8(0x800CB7F5u) == 2u) return result;
    if (buttons & 0x10u) step = -1;
    result = buttons & 0x40u;
    if (result) ++step;
    if (step) {
        result = gtmain_race_car_by_rank(r_s8(r_u32(camera + 180u) + 1737u) + step);
        if (result) w_u32(camera + 180u, result);
    }
    return result;
}

/* Retain legacy draft ABI */
uint32 gt_draft_8001C7F4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_camera_car_select(a0, a1);
}
