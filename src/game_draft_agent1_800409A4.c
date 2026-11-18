#include "game_first2_registry.h"

static sint32 rank_bar_ratio(sint32 numerator, sint32 denominator)
{
    /* Preserve original division faults through the fail-fast exception boundary */
    if (!denominator) gt_draft_exception(7);
    if (numerator == (sint32)0x80000000u && denominator == -1)
        gt_draft_exception(6);
    return numerator / denominator;
}

/* GTMAIN.EXE 800409A4 */
/* Original: GTMAIN.EXE:sub_800409A4. */ uint32 gtmain_menu_rank_bar_draw(uint32 ordering, sint32 value, sint32 maximum,
    sint32 x, sint32 y, sint32 intensity, uint32 full_height)
{
    uint32 selected = (uint32)value, offset = 0, color, ratio;
    uint32 horizontal = (uint32)x, vertical = (uint32)y;
    uint32 brightness = (uint32)intensity, height = 30;
    sint16 corners[4];
    if (value < 0) { selected = (uint32)maximum; offset = 24; }
    color = gtmain_menu_palette_color(6, intensity);
    corners[0] = (sint16)(horizontal + offset);
    corners[1] = (sint16)(vertical + 30u);
    corners[2] = (sint16)(horizontal + 243u);
    corners[3] = corners[1];
    gtmain_packet_rectangle_native(ordering, corners, color);
    ratio = (uint32)rank_bar_ratio((sint32)(selected * 255u), maximum);
    corners[0] = (sint16)(horizontal + 31u);
    corners[1] = (sint16)(vertical + 28u);
    corners[2] = (sint16)(horizontal + 248u);
    corners[3] = corners[1];
    color = (uint8)((sint32)((255u - ratio) * brightness) >> 7)
        | ((brightness << 4) & 0xFF00u)
        | ((uint32)(uint8)((sint32)(ratio * brightness) >> 7) << 16);
    gtmain_packet_rectangle_native(ordering, corners, color);
    if (!full_height) { vertical += 6u; height = 24; }
    corners[0] = (sint16)(horizontal + offset);
    corners[1] = (sint16)vertical;
    corners[2] = (sint16)(horizontal + 247u);
    corners[3] = (sint16)(vertical + height - 1u);
    color = (uint8)(intensity >> 3) | ((brightness << 6) & 0xFF00u)
        | ((uint32)(uint8)(intensity >> 1) << 16) | 0x02000000u;
    gtmain_packet_rectangle_native(ordering, corners, color);
    return gtmain_packet_page(ordering, 544);
}
