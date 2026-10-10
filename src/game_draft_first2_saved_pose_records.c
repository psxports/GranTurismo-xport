#include "game_first2_registry.h"

/* GTMAIN.EXE 8001D438 */
sint32 gtmain_camera_saved_pose_load(uint32 camera)
{
    uint32 saved = r_u32(0x800B6400u);
    uint32 px, py, pz;
    sint16 x, y, z;
    sint32 segment;
    gtmain_camera_projection_set(camera, (sint32)r_u32(saved + 16u));
    px = r_u32(saved + 20u);
    py = r_u32(saved + 24u);
    pz = r_u32(saved + 28u);
    w_u32(camera + 120u, px);
    w_u32(camera + 124u, py);
    w_u32(camera + 128u, pz);
    x = (sint16)r_u16(saved + 34u);
    y = (sint16)r_u16(saved + 32u);
    z = (sint16)r_u16(saved + 36u);
    gtmain_model_rotation((uint8 *)psx_addr(camera + 140u, 20u), x, y, z);
    segment = gtmain_course_segment_find(r_u16(saved + 6u), (const uint8 *)psx_addr(camera + 120u, 12u));
    w_u16(camera + 244u, (uint16)segment);
    return segment;
}

/* GTMAIN.EXE 8002CC9C */
uint32 gtmain_race_records_export(uint32 destination)
{
    uint32 first = 0x800CB8E0u;
    uint32 size = gtmain_race_record_size(first);
    gtmain_memory_copy(destination, first, size);
    if (r_u8(0x800CBC11u) == 2u) {
        uint32 second = first + 0x2354u;
        uint32 second_size = gtmain_race_record_size(second);
        return gtmain_memory_copy(destination + size, second, second_size);
    }
    return 2u;
}
