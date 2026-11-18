#include "game_first2_registry.h"

static void instruments_link(uint32 ordering, uint32 packet)
{
    uint32 previous = r_u8(ordering) | ((uint32)r_u8(ordering + 1u) << 8) | ((uint32)r_u8(ordering + 2u) << 16);
    w_u8(ordering, (uint8)packet); w_u8(ordering + 1u, (uint8)(packet >> 8)); w_u8(ordering + 2u, (uint8)(packet >> 16));
    w_u8(packet, (uint8)previous); w_u8(packet + 1u, (uint8)(previous >> 8)); w_u8(packet + 2u, (uint8)(previous >> 16));
}

/* GTMAIN.EXE 8002C318 */
/* Original: GTMAIN.EXE:sub_8002C318. */ uint32 gtmain_race_instruments_draw(uint32 ordering, sint32 x, sint32 y, uint32 car)
{
    sint32 numerator = (sint16)r_u16(car + 78u) * 2298;
    sint32 denominator = (sint16)r_u16(car + 2348u), angle, speed;
    uint32 packet, label, words[1];
    sint16 bounds[4];
    GTMainFormatArguments arguments = {words, 0u, 1u, 0u};
    if (!denominator) gt_draft_failure("Original instrument BREAK 7", 0x8002C370u);
    angle = (sint16)((uint32)(numerator / denominator) + 1399u);
    packet = r_u32(0x800BFE4Cu);
    w_u32(packet + 4u, 0x388080FFu); w_u16(packet + 8u, 601u); w_u16(packet + 16u, 601u);
    w_u32(packet + 12u, 128u); w_u32(packet + 20u, 0x008080FFu);
    w_u16(packet + 24u, (uint16)-156); w_u32(packet + 28u, 128u); w_u16(packet + 32u, (uint16)-156);
    w_u32(packet, 0x08000000u);
    w_u16(packet + 10u, 9u); w_u16(packet + 18u, (uint16)-9);
    w_u16(packet + 26u, 24u); w_u16(packet + 34u, (uint16)-24);
    gtmain_race_point_rotate(packet + 8u, (uint32)angle, x, y);
    gtmain_race_point_rotate(packet + 16u, (uint32)angle, x, y);
    gtmain_race_point_rotate(packet + 24u, (uint32)angle, x, y);
    gtmain_race_point_rotate(packet + 32u, (uint32)angle, x, y);
    instruments_link(ordering, packet); w_u32(0x800BFE4Cu, packet + 36u);
    gtmain_race_rpm_scale_draw(ordering, x, y, car);
    speed = (sint16)r_u16(car + 76u);
    if (speed < 0) speed = -speed;
    gtmain_text_color(0x4080u); gtmain_text_font(2);
    words[0] = (uint32)speed;
    gtmain_text_format(ordering, (sint16)((uint32)x + 26u), (sint16)((uint32)y + 48u), 0x8008C030u, &arguments);
    gtmain_text_font(1); label = 0x8008C03Cu;
    if (!r_u8(0x800CB763u)) label = 0x8008C034u;
    gtmain_font_text_draw(ordering, (sint32)((uint32)x + 42u), (sint32)((uint32)y + 66u), label);
    bounds[0] = (sint16)((uint32)x + 47u); bounds[2] = (sint16)((uint32)x + 63u);
    bounds[1] = (sint16)((uint32)y + 28u); bounds[3] = (sint16)((uint32)y + 44u);
    return gtmain_race_gear_draw(ordering, (sint32)((uint32)x + 50u), (sint32)((uint32)y + 28u), bounds, car);
}

uint32 gt_draft_8002C318(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    (void)a4; (void)a5; (void)a6; (void)a7;
    return gtmain_race_instruments_draw(a0, (sint32)a1, (sint32)a2, a3);
}
