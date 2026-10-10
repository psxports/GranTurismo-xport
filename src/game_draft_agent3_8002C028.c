#include "game_first2_registry.h"

static void rpm_link(uint32 ordering, uint32 packet)
{
    uint32 previous = r_u8(ordering) | ((uint32)r_u8(ordering + 1u) << 8) | ((uint32)r_u8(ordering + 2u) << 16);
    w_u8(ordering, (uint8)packet); w_u8(ordering + 1u, (uint8)(packet >> 8)); w_u8(ordering + 2u, (uint8)(packet >> 16));
    w_u8(packet, (uint8)previous); w_u8(packet + 1u, (uint8)(previous >> 8)); w_u8(packet + 2u, (uint8)(previous >> 16));
}

/* GTMAIN.EXE 8002C028 */
void gtmain_race_rpm_scale_draw(uint32 ordering, sint32 x, sint32 y, uint32 car)
{
    uint32 position = ((uint32)y << 16) | (uint32)x, data = car + 2344u;
    sint32 count = (sint16)r_u16(car + 2350u), index, filled;
    uint32 packet = r_u32(0x800BFE4Cu), record;
    for (index = 0; index < count; ++index) {
        w_u32(packet, 0x03000000u); w_u32(packet + 4u, (index & 3) ? 0x40C0C0C0u : 0x40FFFFFFu);
        record = data + (uint32)index * 16u;
        w_u32(packet + 8u, position + r_u32(record + 16u));
        w_u32(packet + 12u, position + r_u32(record + 20u));
        rpm_link(ordering, packet); packet += 16u;
    }
    filled = (sint16)r_u16(data + 8u);
    for (index = 0; index < count - 1; ++index) {
        w_u32(packet, 0x05000000u); w_u32(packet + 4u, index < filled ? 0x28000000u : 0x28000080u);
        record = data + (uint32)index * 16u;
        w_u32(packet + 8u, position + r_u32(record + 20u));
        w_u32(packet + 12u, position + r_u32(record + 24u));
        w_u32(packet + 16u, position + r_u32(record + 36u));
        w_u32(packet + 20u, position + r_u32(record + 40u));
        rpm_link(ordering, packet); packet += 24u;
    }
    w_u32(0x800BFE4Cu, packet); gtmain_text_font(0); gtmain_text_color(0x00808080u);
    count = (sint16)r_u16(data + 6u);
    for (index = 0; index < count; index += 4) {
        sint32 tx, ty;
        uint32 words[1] = {(uint32)(index / 4)};
        GTMainFormatArguments arguments = {words, 0u, 1u, 0u};
        record = data + (uint32)index * 16u;
        tx = (sint32)((uint32)x + (uint32)(sint32)(sint16)r_u16(record + 12u) - (index < 40 ? 3u : 7u));
        ty = (sint32)((uint32)y + (uint32)(sint32)(sint16)r_u16(record + 14u) - 3u);
        gtmain_text_format(ordering, (sint16)tx, (sint16)ty, 0x8008C02Cu, &arguments);
    }
}
