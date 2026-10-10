#include "game_first2_registry.h"

/* GTMAIN.EXE:8001ECDC */
sint32 gtmain_race_selected_vehicle_initialize(void)
{
    sint32 start_segment, x, z, sine, cosine;
    uint32 segment, angle_sine, angle_cosine;
    uint8 body, variant, transmission;
    /* TODO Selected vehicle initializer retains native geometry and state boundaries */
    gtmain_model_marker(r_u32(0x800B7170u), 0x800BB850u);
    start_segment = gtmain_course_start_segment();
    gtmain_car_grid_place(0x800B6850u, r_u8(0x800B6673u), (uint32)start_segment);
    body = r_u8(0x800BB7ECu);
    variant = r_u8(0x800BB7EDu);
    transmission = r_u8(0x800BB7EEu);
    sub_8007BD58(0x800B6878u, 0u, 1984u);
    segment = r_u32(0x800B6864u);
    z = (sint32)r_u32(0x800B710Cu) >> 4;
    angle_sine = r_u16(0x800B7112u);
    x = (sint32)r_u32(0x800B7104u) >> 4;
    angle_cosine = r_u16(0x800B7112u);
    sine = (sint16)r_u16(0x8009423Cu + ((angle_sine & 0xFFFu) << 1u));
    cosine = (sint16)r_u16(0x80094A3Cu + ((angle_cosine & 0xFFFu) << 1u));
    gtmain_race_vehicle_initialize(0x800B6878u, 0x800BB850u, segment, x, z,
        sine, cosine, body, variant, 0u, transmission);
    return gtmain_race_vehicle_initialization_finish();
}
