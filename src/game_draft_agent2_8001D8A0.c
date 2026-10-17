#include "game_draft_registry.h"
#include "game_first2_registry.h"

uint32 gtmain_camera_follow_update(uint32 camera)
{
    uint32 car = r_u32(camera + 180u), yaw, result;
    sint32 mode, next;
    uint32 pitch, roll, base_yaw, first_pitch, first_roll;
    uint8 rotation[20], vector[12];
    gtmain_camera_projection_set(camera, r_s16(camera + 262u));
    mode = r_s16(camera + 260u);
    w_u16(camera + 244u, r_u16(car + 20u));
    next = mode == 2 ? 2 : mode + 1;
    base_yaw = r_u16(car + 80u);
    if (!mode) {
        pitch = r_u16(car + 2240u); roll = r_u16(car + 2244u);
        if (r_s8(camera + 255u)) { pitch = 0u - pitch; roll = 0u - roll; }
        first_pitch = pitch; first_roll = roll; base_yaw = 0;
    } else {
        pitch = r_u16(0x8008F24Cu + 2u * (uint32)mode); roll = 0;
        first_pitch = first_roll = 0;
    }
    /* Original interpolation factor is zero; retain the next-angle preload */
    (void)r_u16(0x8008F24Cu + 2u * (uint32)next);
    yaw = base_yaw + r_u16(car + 2242u);
    if (r_s8(camera + 255u)) yaw += 2048u;
    gtmain_model_rotation(rotation, (sint16)yaw, (sint16)first_pitch, (sint16)first_roll);
    gtmain_model_rotation(psx_addr(camera + 140u, 20u), (sint16)yaw, (sint16)pitch, (sint16)roll);
    gtmain_vector_lerp(vector, psx_addr(0x8008F228u + 12u * (uint32)mode, 12u),
        psx_addr(0x8008F228u + 12u * (uint32)next, 12u), 0);
    gtmain_model_rotate_vector(vector, rotation, vector);
    w_u32(camera + 120u, r_u32(car + 2228u) + xport_load_le32(vector));
    w_u32(camera + 124u, r_u32(car + 2232u) + xport_load_le32(vector + 4u));
    result = r_u32(car + 2236u) + xport_load_le32(vector + 8u);
    w_u32(camera + 128u, result);
    return result;
}

/* Retain legacy draft callers until their ordinary ABI migration */
uint32 gt_draft_8001D8A0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_camera_follow_update(a0);
}
