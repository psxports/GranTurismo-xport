#include "game_draft_registry.h"
#include "game_first2_registry.h"

uint32 gtmain_camera_follow_controls(uint32 camera)
{
    uint32 controller = r_u32(camera + 184u);
    uint32 buttons = r_u16(controller + 8u);
    uint32 held = r_u16(controller + 80u), pressed = r_u16(controller + 78u);
    sint32 mode;
    if (r_u32(0x800B630Cu) == 1u)
        gtmain_camera_car_select(camera, buttons);
    if (held & 0x100u) w_u8(camera + 255u, 1);
    mode = r_s16(camera + 258u);
    if (pressed & 0x200u) mode = mode < 2 ? mode + 1 : 0;
    w_u16(camera + 258u, (uint32)mode);
    w_u16(camera + 260u, (uint32)mode);
    if (mode <= 0) w_u8(camera + 254u, 1);
    w_u16(camera + 262u, r_u16(0x800CB758u));
    return gtmain_camera_follow_update(camera);
}

/* Retain legacy draft callers until their ordinary ABI migration */
uint32 gt_draft_8001DCC8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_camera_follow_controls(a0);
}
