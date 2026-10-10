#include "game_first2_registry.h"

uint32 gtmain_menu_wave_select(uint32 guest_text)
{
    uint32 first = r_u8(guest_text);
    sint32 second = (sint8)r_u8(guest_text + 1u);
    return (first - 48u < 10u) & (second == 45);
}
uint32 gtmain_font_spaced_draw(uint32 guest_font, uint32 ordering_link, sint32 x, sint32 y, uint32 guest_text, sint32 spacing)
{
    uint32 text = guest_text, x_word = (uint32)x, y_word = (uint32)y - 4u;
    uint32 result = gtmain_primitive_texture_page(guest_font, ordering_link);
    uint32 packet = r_u32(0x800BFE4Cu);
    uint32 glyphs = r_u32(guest_font), kernings = r_u32(guest_font + 4u);
    uint32 color = r_u32(guest_font + 12u);
    sint32 next = (sint8)r_u8(text);
    uint32 clut = r_u16(guest_font + 24u), link = result;
    if (next) {
        uint32 link_byte = link + 2u, field = packet + 2u;
        do {
            uint32 character = (uint8)next, glyph = glyphs + character * 8u;
            uint32 shape = r_u32(glyph), metrics = r_u32(glyph + 4u);
            uint32 height = shape >> 24;
            if (height) {
                uint32 previous_high;
                w_u32(packet, 0x04000000u);
                w_u16(field + 8u, y_word + ((metrics >> 8) & 255u));
                w_u16(field + 14u, (shape >> 16) & 255u);
                w_u32(field + 2u, color);
                w_u16(field + 6u, x_word);
                w_u16(field + 16u, height);
                w_u16(field + 10u, shape);
                w_u16(field + 12u, clut);
                /* LWL and SWL update only the high link byte at these aligned offsets */
                previous_high = r_u8(link_byte);
                w_u8(link, packet); w_u8(link + 1u, packet >> 8); w_u8(link + 2u, packet >> 16);
                w_u8(field, previous_high);
                field += 20u; packet += 20u;
            }
            ++text;
            next = (sint8)r_u8(text);
            {
                uint32 kerning = r_u32(kernings + character * 4u);
                uint32 packed = r_u32(kerning + (((uint32)next >> 1) & 0x7Cu));
                result = ((metrics >> 16) & 255u) - ((packed >> (((uint32)next & 7u) * 4u)) & 15u) + (uint32)spacing;
            }
            x_word += result;
        } while (next);
    }
    w_u32(0x800BFE4Cu, packet);
    return result;
}
