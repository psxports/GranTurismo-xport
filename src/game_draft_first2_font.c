#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8006ED0C */
uint32 gtmain_font_load(sint32 font)
{
    uint32 buffer[8192];
    const uint8 *data;
    sint32 current = (sint8)r_u8(0x8009948Cu);
    if (current == font) return (uint32)current;
    data = gtmain_resource_read_retry(0x8008E1A0u, (uint8 *)buffer, sizeof(buffer), 0u);
    w_u8(0x8009948Cu, (uint8)font);
    gtmain_font_texture_upload(0x800C0070u, (sint8)font, data);
    gtmain_text_font(1);
    gtmain_text_flags(0u);
    return gtmain_text_color(0x808080u);
}

/* GTMAIN.EXE 8006C900 */
sint32 gtmain_text_width(sint32 font, uint32 text)
{
    uint32 metrics, kerning, total = 0u, glyph;
    switch (font) {
    case 0: metrics = 0x80099998u; kerning = 0x8009BA18u; break;
    case 1: metrics = 0x8009BE18u; kerning = 0x800A1918u; break;
    case 2: metrics = 0x800A1D18u; kerning = 0x800A7218u; break;
    case 3: metrics = 0x800A7618u; kerning = 0x800AD498u; break;
    default: return 0;
    }
    glyph = (uint32)(sint32)(sint8)r_u8(text);
    while (glyph) {
        uint32 next, table, packed, width;
        ++text;
        glyph &= 255u;
        next = (uint32)(sint32)(sint8)r_u8(text);
        table = r_u32(kerning + 4u * glyph);
        packed = r_u32(table + ((next >> 1) & 0x7Cu));
        width = r_u8(metrics + 8u * glyph + 6u);
        total = total + 1u + width - ((packed >> (4u * (next & 7u))) & 15u);
        glyph = next;
    }
    return (sint32)total;
}

/* GTMAIN.EXE 8006F038 */
uint32 gtmain_frame_ordering(uint32 alternate)
{
    return gtmain_audio_buffer_end(r_u32(alternate ? 0x800BFF6Cu : 0x800BFF68u));
}
