#include "game_first2_registry.h"

static uint32 gtmain_packet_link_read(uint32 address)
{
    return r_u8(address) | ((uint32)r_u8(address + 1u) << 8) | ((uint32)r_u8(address + 2u) << 16);
}

static void gtmain_packet_link_write(uint32 address, uint32 link)
{
    w_u8(address, (uint8)link); w_u8(address + 1u, (uint8)(link >> 8)); w_u8(address + 2u, (uint8)(link >> 16));
}

static void gtmain_packet_link(uint32 ordering_link, uint32 first, uint32 last)
{
    uint32 previous = gtmain_packet_link_read(ordering_link);
    gtmain_packet_link_write(ordering_link, first);
    gtmain_packet_link_write(last, previous);
}

sint32 gtmain_frame_defaults(uint32 context)
{
    uint32 first, second, third;
    sint32 result;
    if (r_u8(context)) return 1;
    w_u8(context, 1u);
    first = r_u32(0x800AD938u); second = r_u32(0x800AD93Cu); third = r_u8(0x800AD940u);
    w_u32(context + 1u, first); w_u32(context + 5u, second); w_u8(context + 9u, (uint8)third);
    first = r_u32(0x800AD944u); second = r_u32(0x800AD948u); third = r_u32(0x800AD94Cu);
    w_u32(context + 10u, first); w_u32(context + 14u, second); w_u32(context + 18u, third);
    first = r_u32(0x800AD950u); w_u32(context + 22u, first);
    first = r_u32(0x800AD954u); second = r_u32(0x800AD958u); third = r_u32(0x800AD95Cu);
    w_u32(context + 26u, first); w_u32(context + 30u, second); w_u32(context + 34u, third);
    result = (sint8)r_u8(0x800AD960u); second = r_u8(0x800AD961u); third = r_u8(0x800AD962u);
    w_u8(context + 38u, (uint8)result); w_u8(context + 39u, (uint8)second); w_u8(context + 40u, (uint8)third);
    return result;
}

uint32 gtmain_packet_draw_area(uint32 ordering_link, uint32 offset, uint32 top_left, uint32 bottom_right)
{
    uint32 packet = r_u32(0x800BFE4Cu);
    w_u32(packet, 0x03000000u);
    w_u32(packet + 4u, top_left | 0xE3000000u); w_u32(packet + 8u, bottom_right | 0xE4000000u);
    w_u32(packet + 12u, offset | 0xE5000000u);
    gtmain_packet_link(ordering_link, packet, packet);
    w_u32(0x800BFE4Cu, packet + 16u);
    return packet;
}

static uint32 packet_rectangle_words(uint32 ordering_link, uint32 first, uint32 second, uint32 color)
{
    uint32 packet;
    sint32 x0 = (sint16)first, y0 = (sint16)(first >> 16);
    sint32 x1 = (sint16)second, y1 = (sint16)(second >> 16);
    uint32 comparison = y1 < y0;
    y0 += (sint32)comparison; y1 += (sint32)(comparison ^ 1u);
    packet = r_u32(0x800BFE4Cu);
    w_u32(packet, 0x05000000u);
    comparison = x1 < x0;
    x0 += (sint32)comparison; x1 += (sint32)(comparison ^ 1u);
    w_u32(packet + 4u, color ^ 0x29000000u);
    w_u16(packet + 8u, (uint16)x0); w_u16(packet + 10u, (uint16)y0);
    w_u16(packet + 12u, (uint16)x1); w_u16(packet + 14u, (uint16)y0);
    w_u16(packet + 16u, (uint16)x0); w_u16(packet + 18u, (uint16)y1);
    w_u16(packet + 20u, (uint16)x1); w_u16(packet + 22u, (uint16)y1);
    gtmain_packet_link(ordering_link, packet, packet);
    w_u32(0x800BFE4Cu, packet + 24u);
    return packet;
}

uint32 gtmain_packet_rectangle(uint32 ordering_link, uint32 corners, uint32 color)
{
    uint32 first = r_u32(corners), second = r_u32(corners + 4u);
    return packet_rectangle_words(ordering_link, first, second, color);
}

uint32 gtmain_packet_rectangle_native(uint32 ordering_link, const sint16 *corners4, uint32 color)
{
    uint32 first = (uint16)corners4[0] | ((uint32)(uint16)corners4[1] << 16);
    uint32 second = (uint16)corners4[2] | ((uint32)(uint16)corners4[3] << 16);
    return packet_rectangle_words(ordering_link, first, second, color);
}

uint32 gtmain_packet_page(uint32 ordering_link, uint16 page)
{
    uint32 packet = r_u32(0x800BFE4Cu);
    w_u32(packet, 0x01000000u); w_u32(packet + 4u, (uint32)page | 0xE1000000u);
    gtmain_packet_link(ordering_link, packet, packet);
    w_u32(0x800BFE4Cu, packet + 8u);
    return packet;
}

static void gtmain_packet_line(uint32 packet, uint32 color, sint32 x0, sint32 y0, sint32 x1, sint32 y1)
{
    w_u32(packet + 4u, color);
    w_u16(packet + 8u, (uint16)x0); w_u16(packet + 10u, (uint16)y0);
    w_u16(packet + 12u, (uint16)x1); w_u16(packet + 14u, (uint16)y1);
}

static uint32 packet_outline_words(uint32 ordering_link, uint32 first, uint32 second, uint32 color)
{
    sint32 x0 = (sint16)first, y0 = (sint16)(first >> 16);
    sint32 x1 = (sint16)second, y1 = (sint16)(second >> 16), swap;
    uint32 packet, cursor, last;
    if (x1 < x0) { swap = x0; x0 = x1; x1 = swap; }
    if (y1 < y0) { swap = y0; y0 = y1; y1 = swap; }
    packet = r_u32(0x800BFE4Cu); cursor = packet; last = packet;
    color ^= 0x41000000u;
    if (x0 == x1 || y0 == y1)
    {
        w_u32(cursor, 0x03000000u);
        gtmain_packet_line(cursor, color, x0, y0, x1, y1);
        cursor += 16u;
    }
    else
    {
        gtmain_packet_link_write(cursor, cursor + 16u); w_u8(cursor + 3u, 3u);
        gtmain_packet_line(cursor, color, x0, y0, x1, y0); cursor += 16u;
        gtmain_packet_link_write(cursor, cursor + 16u); last = cursor; w_u8(cursor + 3u, 3u);
        gtmain_packet_line(cursor, color, x0, y1, x1, y1); cursor += 16u;
        if (y1 - y0 >= 2)
        {
            gtmain_packet_link_write(cursor, cursor + 16u); ++y0; --y1; w_u8(cursor + 3u, 3u);
            gtmain_packet_line(cursor, color, x0, y0, x0, y1); cursor += 16u;
            gtmain_packet_link_write(cursor, cursor + 16u); last = cursor; w_u8(cursor + 3u, 3u);
            gtmain_packet_line(cursor, color, x1, y0, x1, y1); cursor += 16u;
        }
    }
    gtmain_packet_link(ordering_link, packet, last);
    w_u32(0x800BFE4Cu, cursor);
    return last;
}

uint32 gtmain_packet_outline(uint32 ordering_link, uint32 corners, uint32 color)
{
    uint32 first = r_u32(corners), second = r_u32(corners + 4u);
    return packet_outline_words(ordering_link, first, second, color);
}

uint32 gtmain_packet_outline_native(uint32 ordering_link, const sint16 *corners4, uint32 color)
{
    uint32 first = (uint16)corners4[0] | ((uint32)(uint16)corners4[1] << 16);
    uint32 second = (uint16)corners4[2] | ((uint32)(uint16)corners4[3] << 16);
    return packet_outline_words(ordering_link, first, second, color);
}

uint32 gtmain_packet_sprite(uint32 ordering_link, uint32 xy, uint32 uv_clut, uint32 wh, uint32 color)
{
    uint32 packet = r_u32(0x800BFE4Cu);
    w_u32(packet, 0x04000000u);
    w_u32(packet + 8u, xy); w_u32(packet + 12u, uv_clut); w_u32(packet + 16u, wh);
    w_u32(packet + 4u, color ^ 0x65000000u);
    gtmain_packet_link(ordering_link, packet, packet);
    w_u32(0x800BFE4Cu, packet + 20u);
    return packet;
}

uint32 gtmain_packet_buffer_select(uint32 index)
{
    uint32 packet = r_u32(0x800BFE58u + (index << 2)), bytes = r_u32(0x800BFE48u);
    w_u32(0x800BFE60u, index); w_u32(0x800BFE4Cu, packet);
    packet += bytes; w_u32(0x800BFE50u, packet);
    return packet;
}

uint32 gtmain_packet_buffers(uint32 first, uint32 second, uint32 bytes)
{
    w_u32(0x800BFE58u, first); w_u32(0x800BFE5Cu, second); w_u32(0x800BFE48u, bytes);
    return 0x800BFE58u;
}

sint32 gtmain_draw_background(uint8 enabled, uint8 red, uint8 green, uint8 blue)
{
    uint32 index;
    for (index = 0u; index < 2u; ++index)
    {
        uint32 draw = 0x800BFE70u + index * 92u;
        w_u8(draw + 88u, enabled); w_u8(draw + 89u, red); w_u8(draw + 90u, green); w_u8(draw + 91u, blue);
    }
    return 0;
}

uint32 gtmain_race_screen_rectangle(uint32 ordering_link)
{
    return gtmain_packet_rectangle(ordering_link, 0x800912F4u, 0u);
}
