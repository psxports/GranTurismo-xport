#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified translation from complete pseudocode and original MIPS */
/* Original: GTMAIN.EXE:sub_8001DB0C. */ uint32 gtmain_camera_pose_prepare(uint32 camera, uint32 mode, sint32 distance)
{
    uint32 car = r_u32(camera + 180u);
    uint32 pitch, roll, base_yaw, table_yaw, table_pitch, yaw, first_yaw, result;
    /* R33 consumes only low16; initialize the unused matrix padding */
    uint8 rotation[20] = {0};
    uint8 vector[12];

    gtmain_camera_projection_set(camera, distance);
    w_u16(camera + 244u, r_u16(car + 20u));
    pitch = r_u16(car + 2240u);
    roll = r_u16(car + 2244u);
    base_yaw = r_u16(car + 80u);
    table_yaw = r_u16(0x8008F2DCu + (mode << 4));
    table_pitch = r_u16(0x8008F2DEu + (mode << 4));
    yaw = r_u16(car + 2242u);
    switch (mode)
    {
        case 0u:
            w_u16(camera + 260u, 0u);
            w_u8(camera + 254u, 1u);
            w_u16(camera + 262u, (uint16)distance);
            return gtmain_camera_follow_update(camera);
        case 1u:
        case 2u:
        case 3u:
            roll = 0u;
            pitch = 0u;
            break;
        case 4u:
        case 5u:
            base_yaw = 0u;
            break;
        default:
            break;
    }
    first_yaw = yaw + base_yaw;
    gtmain_model_rotation(rotation, (sint16)first_yaw, (sint16)pitch, (sint16)roll);
    gtmain_model_rotation(psx_addr(camera + 140u, 20u),
        (sint16)(first_yaw + table_yaw), (sint16)(pitch + table_pitch), (sint16)roll);
    gtmain_model_rotate_vector(vector, rotation, psx_addr(0x8008F2E0u + (mode << 4), 12u));
    w_u32(camera + 120u, r_u32(car + 2228u) + xport_load_le32(vector));
    w_u32(camera + 124u, r_u32(car + 2232u) + xport_load_le32(vector + 4u));
    result = r_u32(car + 2236u) + xport_load_le32(vector + 8u);
    w_u32(camera + 128u, result);
    return result;
}
