#include "game_first2_registry.h"

/* GTMAIN.EXE 8002C568 */
/* Original: GTMAIN.EXE:sub_8002C568. */ sint32 gtmain_race_meters_prepare(uint32 meters)
{
    sint32 ceiling = (sint16)r_u16(meters + 2u) + 999;
    sint32 lower = (sint16)r_u16(meters);
    sint32 thousands = ceiling / 1000;
    sint32 limit = thousands * 4 - 2;
    sint32 result = lower / 250;
    uint32 index = 0u;
    w_u16(meters + 4u, (uint16)(thousands * 1000));
    w_u16(meters + 6u, (uint16)(thousands * 4 - 1));
    w_u16(meters + 8u, (uint16)result);
    /* Entered limits are 2 modulo 4, and index stays within 0..130 */
    while ((sint32)index <= limit)
    {
        uint32 entry = meters + index * 16u;
        uint32 width = !(index & 3u) ? 512u : (index & 1u) ? 556u : 534u;
        sint32 angle = (sint16)((2298 * (sint32)index) / limit + 1399);
        uint32 point;
        w_u16(entry + 16u, width);
        w_u16(entry + 20u, 624u);
        w_u16(entry + 18u, 0u);
        w_u16(entry + 22u, 0u);
        w_u16(entry + 24u, 512u);
        w_u16(entry + 26u, 0u);
        for (point = 0u; point < 3u; ++point)
            gtmain_race_point_rotate(entry + 16u + point * 4u, (uint32)angle, 0, 0);
        w_u16(entry + 12u, 416u);
        w_u16(entry + 14u, 0u);
        gtmain_race_point_rotate(entry + 12u, (uint32)angle, 0, 0);
        ++index;
        result = limit < (sint32)index;
    }
    return result;
}
