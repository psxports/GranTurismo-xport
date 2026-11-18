#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_80035FB4. */ void gtmain_menu_frame_begin(void) { w_u8(0x80091B39u, 0); }
static uint32 panel_xy(uint32 x, uint32 y) { return (x & 65535u) | (y << 16); }
static void panel_rectangle(uint32 ordering, uint32 first, uint32 second, uint32 color)
{
    sint16 corners[4] = {(sint16)first, (sint16)(first >> 16),
        (sint16)second, (sint16)(second >> 16)};
    (void)gtmain_packet_rectangle_native(ordering, corners, color);
}
/* TODO Resolve independent five-argument callback target service */
/* GTMAIN.EXE 8003AF24 */

/* Original: GTMAIN.EXE:sub_8003AF24. */ uint32 gtmain_menu_panel_draw(uint32 guest_context, sint32 x, sint32 y, uint32 previous)
{
    uint32 ordering = r_u32(guest_context + 184u) + 4u;
    uint32 display_y = r_u32(0x800BFF6Cu) == 0 ? 240u : 0u;
    uint32 special = r_u8(guest_context + 274u), magnitude = (uint32)y, descriptor, fade, title;
    uint32 title_x = 0, title_y = 0, centered = 0, width, first, second, color;
    sint32 index, alignment;
    sint64 wide;
    if (y < 0) magnitude = 0u - magnitude;
    if ((sint32)magnitude >= 181) magnitude = 180;
    index = (sint8)r_u8(guest_context + 256u);
    descriptor = r_u32(guest_context + 192u + (uint32)index * 4u);
    {
        uint32 numerator = (180u - magnitude) * 255u;
        wide = (sint64)(sint32)numerator * (sint64)(sint32)0xB60B60B7u;
        fade = (uint32)((sint32)((uint32)((uint64)wide >> 32) + numerator) >> 7) - (uint32)((sint32)numerator >> 31);
    }
    if (previous == 1) descriptor = r_u32(guest_context + 188u);
    title = r_u32(descriptor + 4u);
    if (title) {
        if (!special) {
            width = gtmain_menu_text_width(3, title, 0);
            alignment = (sint8)r_u8(descriptor + 1u);
            if (!alignment) title_x = (uint32)x + 16u;
            else if (alignment == 2) title_x = (uint32)x - ((uint32)((sint32)width >> 1) - 160u);
            else title_x = (uint32)x - (width - 304u);
            title_y = (uint32)y + 19u;
            first = panel_xy(title_x, title_y + 19u);
            second = panel_xy(title_x + width - 1u, title_y + 19u);
            panel_rectangle(ordering + 16u, first, second, fade & 255u);
            (void)gtmain_text_font(3); gtmain_text_flags(2);
            (void)gtmain_text_color(gtmain_menu_gray((uint8)((sint32)fade >> 1)));
            (void)gtmain_menu_text_draw(ordering + 16u, (sint32)title_x, (sint32)(title_y - 5u), title, 0);
        } else {
            (void)gtmain_text_font(3); gtmain_text_flags(2);
            (void)gtmain_text_color(gtmain_menu_gray((uint8)((sint32)fade >> 1)));
            width = (uint32)gtmain_text_width(3, title);
            title_x = (uint32)x + (uint32)((sint32)(320u - width) >> 1);
            alignment = (sint8)r_u8(descriptor + 1u);
            title_y = (uint32)y + 17u;
            if (!alignment) title_x = (uint32)x + 16u;
            else if (alignment == 3) { title_y = (uint32)y + 109u; centered = 1; }
            (void)gtmain_font_text_draw(ordering + 16u, (sint32)title_x, (sint32)title_y, title);
            first = panel_xy(title_x, title_y + 24u);
            second = panel_xy(title_x + width - 1u, title_y + 24u);
            panel_rectangle(ordering + 16u, first, second, fade & 255u);
        }
        color = gtmain_menu_palette_color((sint32)r_u8(descriptor + 24u), (sint32)fade);
        if (centered) {
            uint32 left, right;
            panel_rectangle(ordering + 16u, panel_xy((uint32)x, title_y + 13u), panel_xy(title_x - 3u, title_y + 14u), color);
            left = (uint32)x - (title_x - (uint32)x - 322u);
            right = left + (title_x - (uint32)x) - 3u;
            panel_rectangle(ordering + 16u, panel_xy(left, title_y + 13u), panel_xy(right, title_y + 14u), color);
        } else panel_rectangle(ordering + 16u, panel_xy((uint32)x, (uint32)y), panel_xy((uint32)x + 319u, (uint32)y + 27u), color);
        (void)gtmain_menu_panel_callback(r_u32(descriptor + 12u), ordering + 32u, x, y, display_y, fade);
        if (!special) {
            (void)gtmain_packet_draw_area(ordering + 64u, display_y << 11, display_y << 10, ((display_y + 207u) << 10) | 319u);
            if (!previous) {
                uint32 flags = r_u8(guest_context + 258u) & 15u;
                if (flags == 2 || flags == 3) {
                    uint32 uv = r_u32(0x80092380u), dimensions = r_u32(0x8009237Cu);
                    (void)gtmain_packet_sprite(ordering, flags == 2 ? 0xD00070u : 0xD00078u, uv, dimensions, 0x808080u);
                }
                {
                    uint32 packet = r_u32(guest_context + 152u);
                    uint32 device = r_u8(packet + 1u), flags = r_u8(guest_context + 258u);
                    uint32 row = (device & 0xF0u) == 0x20u, position = 192u, uv, dimensions;
                    if (flags & 16u) { position = 160u; row += 2u; }
                    uv = r_u32(0x80092334u + row * 16u + 12u);
                    dimensions = r_u32(0x80092334u + row * 16u + 8u);
                    (void)gtmain_packet_sprite(ordering, position | 0xD00000u, uv, dimensions, 0x808080u);
                }
            }
            panel_rectangle(ordering, 0xD00000u, 0xD0013Fu, 0x333333u);
            (void)gtmain_packet_page(ordering, (uint16)((uint32)(sint32)(sint8)(uint8)(r_u8(0x80092335u) | 32u) | 512u));
        }
        (void)gtmain_packet_draw_area(ordering + 16u, display_y << 11, display_y << 10, ((display_y + 239u) << 10) | 319u);
    }
    return gtmain_packet_draw_area(ordering + 16360u, display_y << 11, display_y << 10, ((display_y + 239u) << 10) | 319u);
}
