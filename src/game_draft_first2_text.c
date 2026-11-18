#include "game_first2_registry.h"
#include "psx_gpu.h"

static uint32 gtmain_text_link_read(uint32 address)
{
    return (uint32)r_u8(address) | ((uint32)r_u8(address + 1u) << 8) | ((uint32)r_u8(address + 2u) << 16);
}

static void gtmain_text_link_write(uint32 address, uint32 value)
{
    w_u8(address, (uint8)value);
    w_u8(address + 1u, (uint8)(value >> 8));
    w_u8(address + 2u, (uint8)(value >> 16));
}

/* Original: GTMAIN.EXE:sub_80071210. */ uint32 gtmain_primitive_texture_page(uint32 context, uint32 ordering_link)
{
    uint32 command, packet, previous;
    GpuRasterState raster;
    gpu_export_raster(&raster);
    command = raster.draw_flags;
    command = (command & 0x600u) | r_u16(context + 10u) | 0xE1000000u;
    packet = r_u32(0x800BFE4Cu);
    w_u32(packet, 0x01000000u);
    w_u32(packet + 4u, command);
    previous = gtmain_text_link_read(ordering_link);
    gtmain_text_link_write(ordering_link, packet);
    gtmain_text_link_write(packet, previous);
    w_u32(0x800BFE4Cu, packet + 8u);
    return packet;
}

/* Original: GTMAIN.EXE:sub_8006C57C. */ sint32 gtmain_text_glyph(uint8 glyph)
{
    uint32 glyph_address = r_u32(0x800C0070u) + (uint32)glyph * 8u;
    uint32 position = r_u32(0x800C0080u);
    sint32 offset = (sint16)r_u16(0x800C008Cu);
    sint32 advance = (sint16)r_u16(0x800C008Eu);
    uint32 descriptor = r_u32(glyph_address), bearing = r_u32(glyph_address + 4u);
    uint32 height = descriptor >> 24;
    if (height)
    {
        uint32 x = (uint32)(sint32)(sint16)position + (bearing & 255u) + (uint32)offset;
        uint32 y = (uint32)(sint32)(sint16)(position >> 16) + ((bearing >> 8) & 255u) - 4u;
        uint32 packet = r_u32(0x800BFE4Cu), link = r_u32(0x800C0084u);
        uint32 color = r_u32(0x800C007Cu), clut = r_u16(0x800C0088u), previous;
        w_u16(packet + 10u, (uint16)y);
        w_u32(packet, 0x04000000u); w_u32(packet + 4u, color);
        w_u16(packet + 8u, (uint16)x); w_u16(packet + 16u, (uint16)((descriptor >> 16) & 255u));
        w_u16(packet + 18u, (uint16)height); w_u16(packet + 12u, (uint16)descriptor);
        w_u16(packet + 14u, (uint16)clut);
        previous = gtmain_text_link_read(link);
        gtmain_text_link_write(link, packet);
        gtmain_text_link_write(packet, previous);
        w_u32(0x800C0084u, packet); w_u32(0x800BFE4Cu, packet + 20u);
    }
    w_u16(0x800C008Cu, (uint16)((uint32)offset + (uint32)advance));
    return 0;
}

/* Original: GTMAIN.EXE:sub_8006C664. */ void gtmain_text_position(sint16 x, sint16 y)
{
    w_u16(0x800C0080u, (uint16)x); w_u16(0x800C0082u, (uint16)y);
    w_u16(0x800C008Cu, 0u);
}

/* Original: GTMAIN.EXE:sub_8006C684. */ void gtmain_text_advance(sint16 advance)
{
    w_u16(0x800C008Eu, (uint16)advance);
}

/* Original: GTMAIN.EXE:sub_8006C694. */ uint32 gtmain_text_bind(uint32 ordering_link)
{
    uint32 packet = gtmain_primitive_texture_page(0x800C0070u, ordering_link);
    w_u32(0x800C0084u, packet);
    return packet;
}

/* Original: GTMAIN.EXE:sub_8006C720. */ uint32 gtmain_text_draw(uint32 ordering_link, sint16 x, sint16 y, uint32 guest_string)
{
    sint32 glyph;
    uint32 result = gtmain_text_bind(ordering_link);
    gtmain_text_position(x, y);
    glyph = (sint8)r_u8(guest_string);
    while (glyph)
    {
        ++guest_string;
        result = gtmain_text_glyph((uint8)glyph);
        glyph = (sint8)r_u8(guest_string);
    }
    return result;
}

/* Original: GTMAIN.EXE:sub_8006C790. */ sint32 gtmain_text_font(sint32 index)
{
    uint32 clut = r_u16(0x800C0078u), table, texture, result;
    if (index == 1)
    {
        table = 0x8009BE18u; texture = 0x800A1918u; clut += 64u; result = 8u;
    }
    else if (index < 2)
    {
        if (index) return 1;
        table = 0x80099998u; texture = 0x8009BA18u; result = 6u;
    }
    else if (index == 2)
    {
        table = 0x800A1D18u; texture = 0x800A7218u; clut += 128u; result = 12u;
    }
    else
    {
        if (index != 3) return 3;
        table = 0x800A7618u; texture = 0x800AD498u; clut += 192u; result = 24u;
    }
    w_u16(0x800C008Au, (uint16)index); w_u32(0x800C0070u, table); w_u32(0x800C0074u, texture);
    w_u16(0x800C008Eu, (uint16)result); w_u16(0x800C0088u, (uint16)clut);
    return (sint32)result;
}
