#include "game_first2_registry.h"

/* GTMAIN.EXE 8002C8E4 */
uint32 gtmain_race_meter_color(sint32 value)
{
    uint32 scaled = (uint32)value << 2;
    if ((sint32)scaled < -255) return 0u;
    if ((sint32)scaled < 0)
        return (((scaled + 255u) & 255u) << 8) | (((0u - scaled) & 255u) << 16);
    if ((sint32)scaled < 256) return (scaled & 255u) | 0xFF00u;
    return (((scaled & 0xFCu) ^ 0xFFu) << 8) | 0xFFu;
}

/* GTMAIN.EXE 8002C944 */
uint32 gtmain_race_meter_rectangle_draw(uint32 ordering, sint16 x, sint16 y, sint32 value)
{
    sint16 corners[4];
    uint32 color;
    corners[0] = x;
    corners[1] = y;
    corners[2] = (sint16)((uint32)(sint32)x + 2u);
    corners[3] = (sint16)((uint32)(sint32)y + 6u);
    color = gtmain_race_meter_color(value);
    return gtmain_packet_rectangle_native(ordering, corners, color);
}
