#include "game_draft_registry.h"
#include <stdio.h>

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

uint32 gtmain_font_init(uint32 page)
{
    uint8 input[32768];
    uint32 pixels[8192];
    uint8 *output = (uint8 *)pixels;
    uint32 cursor = 0u, written = 0u, flags = 0u, bit = 0u;
    uint32 x = (page & 15u) << 6, y = (page >> 4) << 8;
    size_t input_size;
    FILE *file = NULL;
    PSX_RECT rectangle = {(sint16)x, (sint16)y, 64, 256};
    if ((sint32)(sint8)r_u8(0x8009948Cu) == (sint32)page)
        return (uint32)(sint32)(sint8)r_u8(0x8009948Cu);
    if (fopen_s(&file, "DATA/GAMEFONT.DAT", "rb") || !file) goto failed;
    input_size = fread(input, 1u, sizeof(input), file);
    if (ferror(file)) { fclose(file); goto failed; }
    fclose(file);
    w_u8(0x8009948Cu, page);
    DrawSync(0);
    while (written < sizeof(pixels)) {
        if (!bit) {
            if (cursor >= input_size) goto failed;
            flags = input[cursor++];
        } else flags >>= 1;
        if (cursor >= input_size) goto failed;
        if (flags & 1u) {
            uint32 length = input[cursor++] + 3u;
            uint32 distance, high;
            if (cursor >= input_size) goto failed;
            high = input[cursor++];
            distance = high & 127u;
            if (high & 128u) {
                if (cursor >= input_size) goto failed;
                distance = (distance << 8) | input[cursor++];
            }
            ++distance;
            if (distance > written) goto failed;
            if (length > sizeof(pixels) - written) length = sizeof(pixels) - written;
            while (length--) {
                output[written] = output[written - distance];
                ++written;
            }
        } else output[written++] = input[cursor++];
        bit = (bit + 1u) & 7u;
    }
    if (LoadImagePSX(&rectangle, pixels) < 0) goto failed;
    sub_8007BD58(0x800C0070u, 0u, 32u);
    w_u16(0x800C0078u, (x >> 4) | (y << 6));
    w_u8(0x800C007Fu, 100u);
    w_u32(0x800C0078u, (((r_u32(0x800C0078u) & 0xFFE0FFFFu) | ((page & 31u) << 16)) & 0xFF9FFFFFu | 0x200000u) & 0xFE7FFFFFu);
    w_u8(0x800C007Cu, 0x80u);
    w_u8(0x800C007Du, 0x80u);
    w_u8(0x800C007Eu, 0x80u);
    DrawSync(0);
    w_u16(0x800C008Au, 1u);
    w_u32(0x800C0070u, 0x8009BE18u);
    w_u32(0x800C0074u, 0x800A1918u);
    w_u16(0x800C008Eu, 8u);
    w_u16(0x800C0088u, r_u16(0x800C0078u) + 64u);
    w_u8(0x800C007Fu, 100u);
    w_u8(0x800C007Cu, 0x80u);
    w_u8(0x800C007Du, 0x80u);
    w_u8(0x800C007Eu, 0x80u);
    return 0x800C007Eu;
failed:
    return gt_draft_unresolved_call(0x80078914u, 0x800C0070u, page, 0u, 0u, 0u, 0u, 0u, 0u);
}
