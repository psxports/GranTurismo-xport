#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified translation from complete pseudocode and original MIPS */
uint32 gtmain_camera_path_phase_update(uint32 camera)
{
    uint32 config = r_u32(0x800B6400u);
    uint32 car = r_u32(camera + 180u);
    uint8 vector[12];
    uint32 first, second, z_first, z_second, lower, upper, phase, product, distance;

    gtmain_vector_path_dispatch(camera + 120u, config + 24u);
    w_u16(camera + 244u, (uint16)gtmain_course_segment_find(r_u16(config + 6u), psx_addr(camera + 120u, 12u)));
    first = r_u32(car + 2228u);
    second = r_u32(camera + 120u);
    xport_store_le32(vector, first - second);
    first = r_u32(car + 2232u);
    second = r_u32(camera + 124u);
    xport_store_le32(vector + 4u, first - second);
    z_first = r_u32(car + 2236u);
    z_second = r_u32(camera + 128u);
    xport_store_le32(vector + 4u, first - second + 52428u);
    xport_store_le32(vector + 8u, z_first - z_second);
    gtmain_camera_direction_matrix(psx_addr(camera + 140u, 18u), vector);
    lower = r_u16(config + 16u);
    upper = r_u16(config + 18u);
    phase = r_u32(0x800B3910u);
    product = (upper - lower) * phase;
    distance = (uint32)((sint32)product >> 12) + lower;
    return gtmain_camera_projection_set(camera, (sint32)distance);
}
