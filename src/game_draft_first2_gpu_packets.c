#include "game_first2_registry.h"
#include <stdio.h>

static uint32 gpu_word_read(uint32 address)
{
    /* TODO Bind original GPU and DMA register word contracts */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native GPU device word read", address);
    return r_u32(address);
}
static void gpu_word_write(uint32 address, uint32 value)
{
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native GPU device word write", address);
    w_u32(address, value);
}
static uint32 gpu_missing(uint32 token, uint32 a0, uint32 a1)
{
    /* TODO Bind actual native device callback and its meaningful result */
    (void)a0; (void)a1;
    gt_draft_failure("Native GPU device callback adapter", token);
}
static uint32 gpu_callback(uint32 token, uint32 data, uint32 argument)
{
    if (token == 0x80088790u) return gtmain_gpu_control_write(data);
    if (token == 0x80088808u) return gtmain_gpu_dma_submit(data);
    if (token == 0x800880A4u) return (uint32)gtmain_gpu_clear_packet(data, argument);
    if (token == 0x800882D4u) return (uint32)gtmain_gpu_pixels_write(data, argument);
    if (token == 0x80088510u) return (uint32)gtmain_gpu_pixels_read(data, argument);
    return gpu_missing(token, data, argument);
}
static void gpu_control(uint32 command)
{
    uint32 table = r_u32(0x800B24ACu);
    (void)gpu_callback(r_u32(table + 16u), command, 0);
}
static void gpu_log_environment(uint32 format, uint32 argument)
{
    uint32 token = r_u32(0x800B24B0u);
    if (token == 0x8008B7D8u) printf("PutDispEnv(%08x)...\n", (unsigned)argument);
    else (void)gpu_missing(token, format, argument);
}
uint32 gtmain_gpu_draw_mode(uint32 dither, uint32 display, uint16 texture_page)
{
    uint32 command = display ? 0xE1000200u : 0xE1000000u;
    uint32 bits = texture_page & 0x9FFu;
    if (dither) bits |= 0x400u;
    return command | bits;
}
uint32 gtmain_gpu_draw_offset(sint16 x, sint16 y)
{
    return 0xE5000000u | ((uint32)y & 0x7FFu) << 11 | ((uint32)x & 0x7FFu);
}
uint32 gtmain_gpu_texture_window(uint32 rectangle)
{
    uint32 x, y, width, height;
    if (!rectangle) return 0;
    x = r_u8(rectangle) >> 3;
    width = ((0u - (uint32)(sint16)r_u16(rectangle + 4u)) & 255u) >> 3;
    y = r_u8(rectangle + 2u) >> 3;
    height = ((0u - (uint32)(sint16)r_u16(rectangle + 6u)) & 255u) >> 3;
    return 0xE2000000u | (y << 15) | (x << 10) | (height << 5) | width;
}
static uint32 gpu_clip(uint32 opcode, sint16 x, sint16 y)
{
    uint32 cx = 0, cy;
    if (x >= 0) {
        sint32 limit = (sint16)r_u16(0x800B24B8u) - 1;
        uint32 unsigned_limit = r_u16(0x800B24B8u) - 1u;
        cx = limit < x ? unsigned_limit : (uint32)(sint32)x;
    }
    if (y < 0) cy = 0;
    else {
        sint32 limit = (sint16)r_u16(0x800B24BAu) - 1;
        uint32 unsigned_limit = r_u16(0x800B24BAu) - 1u;
        cy = limit < y ? unsigned_limit : (uint32)(sint32)y;
    }
    return opcode | (cy & 1023u) << 10 | (cx & 1023u);
}
uint32 gtmain_gpu_clip_begin(sint16 x, sint16 y) { return gpu_clip(0xE3000000u, x, y); }
uint32 gtmain_gpu_clip_end(sint16 x, sint16 y) { return gpu_clip(0xE4000000u, x, y); }
uint32 gtmain_gpu_timeout_begin(void)
{
    uint32 value = gpu_missing(0x8008A908u, (uint32)-1, 0) + 240u;
    w_u32(0x800B25C0u, value); w_u32(0x800B25C4u, 0);
    return value;
}
sint32 gtmain_gpu_timeout_poll(void)
{
    uint32 now = gpu_missing(0x8008A908u, (uint32)-1, 0), old, result;
    if ((sint32)r_u32(0x800B25C0u) >= (sint32)now) {
        old = r_u32(0x800B25C4u); w_u32(0x800B25C4u, old + 1u);
        if ((sint32)old <= 983040) return 0;
    }
    {
        uint32 status_address = r_u32(0x800B258Cu);
        uint32 discarded = gpu_word_read(status_address);
        uint32 head = r_u32(0x800B25ACu);
        uint32 madr_address = r_u32(0x800B2590u);
        uint32 tail = r_u32(0x800B25B0u);
        uint32 madr = gpu_word_read(madr_address);
        uint32 chcr_address = r_u32(0x800B2598u);
        uint32 status = gpu_word_read(status_address), chcr = gpu_word_read(chcr_address);
        (void)discarded;
        printf("GPU timeout:que=%d,stat=%08x,chcr=%08x,madr=%08x,", (int)((head-tail)&3u), (unsigned)status, (unsigned)chcr, (unsigned)madr);
    }
    result = gpu_missing(0x8008A3C4u, 0, 0);
    w_u32(0x800B25B0u, 0);
    old = r_u32(0x800B25B0u);
    w_u32(0x800B25BCu, result); w_u32(0x800B25ACu, old);
    gpu_word_write(r_u32(0x800B2598u), 1025);
    old = r_u32(0x800B25A8u); gpu_word_write(old, gpu_word_read(old) | 0x800u);
    gpu_word_write(r_u32(0x800B258Cu), 0x02000000u);
    gpu_word_write(r_u32(0x800B258Cu), 0x01000000u);
    (void)gpu_missing(0x8008A3C4u, r_u32(0x800B25BCu), 0);
    return -1;
}
static uint32 gpu_transfer_shape(uint32 rectangle, uint32 *blocks, uint32 *remainder)
{
    sint32 width = (sint16)r_u16(rectangle + 4u), height;
    uint32 clipped_width = r_u16(rectangle + 4u), clipped_height, adjusted, product;
    if (width < 0) clipped_width = 0;
    else {
        sint32 limit = (sint16)r_u16(0x800B24B8u);
        uint32 limit_bits = r_u16(0x800B24B8u);
        if (limit < width) clipped_width = limit_bits;
    }
    height = (sint16)r_u16(rectangle + 6u); clipped_height = r_u16(rectangle + 6u);
    w_u16(rectangle + 4u, clipped_width);
    if (height < 0) clipped_height = 0;
    else {
        sint32 limit = (sint16)r_u16(0x800B24BAu);
        uint32 limit_bits = r_u16(0x800B24BAu);
        if (limit < height) clipped_height = limit_bits;
    }
    product = (uint32)((sint16)r_u16(rectangle + 4u) * (sint16)clipped_height);
    w_u16(rectangle + 6u, clipped_height);
    adjusted = product + 1u; adjusted += adjusted >> 31;
    *blocks = (uint32)((sint32)adjusted >> 5);
    *remainder = (uint32)((sint32)adjusted >> 1) - (*blocks << 4);
    return (uint32)((sint32)adjusted >> 1);
}
sint32 gtmain_gpu_pixels_write(uint32 rectangle, uint32 source)
{
    uint32 blocks, remainder, count, command = 0xA0000000u;
    (void)gtmain_gpu_timeout_begin();
    if ((sint32)gpu_transfer_shape(rectangle, &blocks, &remainder) <= 0) return -1;
    while (!(gpu_word_read(r_u32(0x800B258Cu)) & 0x04000000u)) {
        if (gtmain_gpu_timeout_poll()) return -1;
        command = 0xA0000000u;
    }
    gpu_word_write(r_u32(0x800B258Cu), 0x04000000u);
    gpu_word_write(r_u32(0x800B2588u), 0x01000000u);
    gpu_word_write(r_u32(0x800B2588u), command);
    { uint32 output = r_u32(0x800B2588u); gpu_word_write(output, r_u32(rectangle)); }
    { uint32 output = r_u32(0x800B2588u); gpu_word_write(output, r_u32(rectangle + 4u)); }
    count = remainder - 1u;
    while (count != 0xFFFFFFFFu) {
        uint32 value = r_u32(source), output;
        source += 4u; output = r_u32(0x800B2588u); --count;
        gpu_word_write(output, value);
    }
    if (blocks) {
        gpu_word_write(r_u32(0x800B258Cu), 0x04000002u);
        gpu_word_write(r_u32(0x800B2590u), source);
        gpu_word_write(r_u32(0x800B2594u), (blocks << 16) | 16u);
        gpu_word_write(r_u32(0x800B2598u), 0x01000201u);
    }
    return 0;
}
sint32 gtmain_gpu_pixels_read(uint32 rectangle, uint32 destination)
{
    uint32 blocks, remainder, count;
    (void)gtmain_gpu_timeout_begin();
    if ((sint32)gpu_transfer_shape(rectangle, &blocks, &remainder) <= 0) return -1;
    while (!(gpu_word_read(r_u32(0x800B258Cu)) & 0x04000000u))
        if (gtmain_gpu_timeout_poll()) return -1;
    gpu_word_write(r_u32(0x800B258Cu), 0x04000000u);
    gpu_word_write(r_u32(0x800B2588u), 0x01000000u);
    gpu_word_write(r_u32(0x800B2588u), 0xC0000000u);
    { uint32 output = r_u32(0x800B2588u); gpu_word_write(output, r_u32(rectangle)); }
    { uint32 output = r_u32(0x800B2588u); gpu_word_write(output, r_u32(rectangle + 4u)); }
    while (!(gpu_word_read(r_u32(0x800B258Cu)) & 0x08000000u))
        if (gtmain_gpu_timeout_poll()) return -1;
    count = remainder - 1u;
    while (count != 0xFFFFFFFFu) {
        uint32 value = gpu_word_read(r_u32(0x800B2588u));
        --count; w_u32(destination, value); destination += 4u;
    }
    if (blocks) {
        gpu_word_write(r_u32(0x800B258Cu), 0x04000003u);
        gpu_word_write(r_u32(0x800B2590u), destination);
        gpu_word_write(r_u32(0x800B2594u), (blocks << 16) | 16u);
        gpu_word_write(r_u32(0x800B2598u), 0x01000200u);
    }
    return 0;
}
sint32 gtmain_gpu_command_enqueue(uint32 callback_token, uint32 data, sint32 copy_bytes, uint32 argument)
{
    uint32 head, tail, mask, table, index, pointer;
    (void)gtmain_gpu_timeout_begin();
    while (((r_u32(0x800B25ACu) + 1u) & 3u) == r_u32(0x800B25B0u)) {
        if (gtmain_gpu_timeout_poll()) return -1;
        (void)gpu_missing(0x80088B54u, 0, 0);
    }
    mask = gpu_missing(0x8008A3C4u, 0, 0); w_u32(0x800B25B4u, mask);
    { uint32 enabled = r_u8(0x800B24B5u); w_u32(0x800B24BCu, 1);
      if (enabled) {
        head = r_u32(0x800B25ACu); tail = r_u32(0x800B25B0u);
        if (head != tail) goto queued;
        if (gpu_word_read(r_u32(0x800B2598u)) & 0x01000000u) goto queued;
        if (r_u32(0x800B24C0u)) goto queued;
      }
    }
    pointer = r_u32(0x800B258Cu);
    while (!(gpu_word_read(pointer) & 0x04000000u)) {}
    (void)gpu_callback(callback_token, data, argument);
    (void)gpu_missing(0x8008A3C4u, r_u32(0x800B25B4u), 0);
    return 0;
queued:
    (void)gpu_missing(0x8008A2A8u, 2, 0x80088B54u);
    if (copy_bytes) {
        pointer = data; index = 0;
        while ((sint32)index < copy_bytes / 4) {
            uint32 value = r_u32(pointer); pointer += 4u;
            head = r_u32(0x800B25ACu); table = 0x800B614Cu + 96u * head + 4u * index;
            ++index; w_u32(table, value);
        }
        head = r_u32(0x800B25ACu); tail = r_u32(0x800B25ACu);
        w_u32(0x800B6144u + 96u * head, 0x800B614Cu + 96u * tail);
    } else w_u32(0x800B6144u + 96u * r_u32(0x800B25ACu), data);
    w_u32(0x800B6148u + 96u * r_u32(0x800B25ACu), argument);
    w_u32(0x800B6140u + 96u * r_u32(0x800B25ACu), callback_token);
    head = r_u32(0x800B25ACu); mask = r_u32(0x800B25B4u);
    w_u32(0x800B25ACu, (head + 1u) & 3u);
    (void)gpu_missing(0x8008A3C4u, mask, 0);
    (void)gpu_missing(0x80088B54u, 0, 0);
    head = r_u32(0x800B25ACu); tail = r_u32(0x800B25B0u);
    return (sint32)((head - tail) & 3u);
}

static uint16 gpu_dimension(uint16 bits, uint32 limit_address)
{
    if ((sint16)bits < 0) return 0;
    {
        sint32 limit = (sint16)r_u16(limit_address) - 1;
        uint32 limit_bits = r_u16(limit_address) - 1u;
        return (uint16)(limit < (sint16)bits ? limit_bits : bits);
    }
}
uint32 gtmain_gpu_draw_packet(uint32 packet, uint32 environment)
{
    uint32 value, length = 7, xy, wh, color;
    sint16 x = (sint16)r_u16(environment), y = (sint16)r_u16(environment + 2u);
    w_u32(packet + 4u, gtmain_gpu_clip_begin(x, y));
    {
        uint32 width = r_u16(environment + 4u), xbits = r_u16(environment);
        uint32 ybits = r_u16(environment + 2u), height;
        x = (sint16)(width + xbits - 1u); height = r_u16(environment + 6u);
        y = (sint16)(ybits + height - 1u);
    }
    w_u32(packet + 8u, gtmain_gpu_clip_end(x, y));
    x = (sint16)r_u16(environment + 8u); y = (sint16)r_u16(environment + 10u);
    w_u32(packet + 12u, gtmain_gpu_draw_offset(x, y));
    {
        uint32 dither = r_u8(environment + 23u), display = r_u8(environment + 22u);
        uint16 texture = r_u16(environment + 20u);
        value = gtmain_gpu_draw_mode(dither, display, texture);
    }
    w_u32(packet + 16u, value);
    w_u32(packet + 20u, gtmain_gpu_texture_window(environment + 12u));
    w_u32(packet + 24u, 0xE6000000u);
    if (r_u8(environment + 24u)) {
        uint32 xb = r_u16(environment), yb = r_u16(environment + 2u);
        uint16 width = r_u16(environment + 4u), height = r_u16(environment + 6u);
        width = gpu_dimension(width, 0x800B24B8u);
        height = gpu_dimension(height, 0x800B24BAu);
        xy = xb | (yb << 16); wh = width | ((uint32)height << 16);
        if ((xb & 63u) || (width & 63u)) {
            uint32 offset_x = r_u16(environment + 8u), offset_y;
            xb = (xb - offset_x) & 65535u;
            offset_y = r_u16(environment + 10u); yb = (yb - offset_y) & 65535u;
            xy = xb | (yb << 16); value = 0x60000000u;
        } else value = 0x02000000u;
        color = r_u8(environment + 27u) << 16;
        color |= r_u8(environment + 26u) << 8;
        color |= r_u8(environment + 25u);
        w_u32(packet + 28u, value | color);
        w_u32(packet + 32u, xy); w_u32(packet + 36u, wh); length = 10;
    }
    value = length - 1u; w_u8(packet + 3u, (uint8)value); return value;
}
sint32 gtmain_gpu_clear_packet(uint32 rectangle, uint32 packed_color)
{
    uint16 width = r_u16(rectangle + 4u), height;
    width = gpu_dimension(width, 0x800B24B8u);
    height = r_u16(rectangle + 6u); w_u16(rectangle + 4u, width);
    height = gpu_dimension(height, 0x800B24BAu);
    { uint32 x = r_u16(rectangle) & 63u; w_u16(rectangle + 6u, height);
      if (x || (r_u16(rectangle + 4u) & 63u)) {
        uint32 status;
        w_u32(0x800B5F7Cu, 0x080B5FA4u);
        w_u32(0x800B5F8Cu, 0xE6000000u);
        {
            uint32 status_address = r_u32(0x800B258Cu);
            w_u32(0x800B5F80u, 0xE3000000u);
            w_u32(0x800B5F84u, 0xE4FFFFFFu);
            w_u32(0x800B5F88u, 0xE5000000u);
            status = gpu_word_read(status_address);
        }
        w_u32(0x800B5F94u, (packed_color & 0xFFFFFFu) | 0x60000000u);
        w_u32(0x800B5F90u, (status & 0x7FFu) | ((packed_color >> 31) << 10) | 0xE1000000u);
        w_u32(0x800B5F98u, r_u32(rectangle));
        { uint32 wh = r_u32(rectangle + 4u); w_u32(0x800B5FA4u, 0x03FFFFFFu); w_u32(0x800B5F9Cu, wh); }
        w_u32(0x800B5FA8u, gpu_missing(0x80088850u, 3, 0) | 0xE3000000u);
        w_u32(0x800B5FACu, gpu_missing(0x80088850u, 4, 0) | 0xE4000000u);
        w_u32(0x800B5FB0u, gpu_missing(0x80088850u, 5, 0) | 0xE5000000u);
      } else {
        uint32 status;
        w_u32(0x800B5F7Cu, 0x05FFFFFFu); w_u32(0x800B5F80u, 0xE6000000u);
        status = gpu_word_read(r_u32(0x800B258Cu));
        w_u32(0x800B5F88u, (packed_color & 0xFFFFFFu) | 0x02000000u);
        w_u32(0x800B5F84u, (status & 0x7FFu) | ((packed_color >> 31) << 10) | 0xE1000000u);
        w_u32(0x800B5F8Cu, r_u32(rectangle)); w_u32(0x800B5F90u, r_u32(rectangle + 4u));
      }
    }
    (void)gtmain_gpu_dma_submit(0x800B5F7Cu);
    return 0;
}
uint32 gtmain_gpu_display_environment(uint32 environment)
{
    uint32 mode = 0x08000000u, command, changed, index;
    sint32 start_x, end_x, start_y, end_y, value;
    if (r_u8(0x800B24B6u) >= 2u) gpu_log_environment(0x8008F08Cu, environment);
    {
        uint32 y = r_u16(environment + 2u), x = r_u16(environment);
        command = 0x05000000u | ((y & 1023u) << 10) | (x & 1023u);
    }
    gpu_control(command);
    changed = 0;
    for (index = 0; index < 4u; ++index) {
        sint16 cached = (sint16)r_u16(0x800B2528u + 2u * index);
        sint16 input = (sint16)r_u16(environment + 8u + 2u * index);
        if (cached != input) { changed = 1; break; }
    }
    if (changed) {
        uint32 pal = gtmain_video_mode_get();
        sint32 screen_x = (sint16)r_u16(environment + 8u), screen_y, width, height;
        w_u8(environment + 18u, (uint8)pal);
        screen_y = (sint16)r_u16(environment + 10u);
        start_x = 10 * screen_x + 608;
        start_y = screen_y + ((pal & 255u) ? 19 : 16);
        width = (sint16)r_u16(environment + 12u);
        end_x = start_x + (width ? 10 * width : 2560);
        height = (sint16)r_u16(environment + 14u);
        end_y = start_y + (height ? height : 240);
        if (start_x < 500) start_x = 500;
        else if (start_x >= 3291) start_x = 3290;
        value = start_x + 80;
        if (end_x >= value) value = end_x < 3291 ? end_x : 3290;
        end_x = value;
        if (start_y < 16) start_y = 16;
        else if (r_u8(environment + 18u)) {
            if (start_y >= 311) start_y = r_u8(environment + 18u) ? 310 : 256;
        } else if (start_y >= 257) start_y = r_u8(environment + 18u) ? 310 : 256;
        value = start_y + 2;
        if (end_y >= value) {
            if (r_u8(environment + 18u)) {
                value = end_y;
                if (end_y >= 313) value = r_u8(environment + 18u) ? 312 : 258;
            } else {
                value = end_y;
                if (end_y >= 259) value = r_u8(environment + 18u) ? 312 : 258;
            }
        }
        end_y = value;
        gpu_control(0x06000000u | ((uint32)end_x & 4095u) << 12 | ((uint32)start_x & 4095u));
        gpu_control(0x07000000u | ((uint32)end_y & 1023u) << 10 | ((uint32)start_y & 1023u));
    }
    changed = r_u32(0x800B2530u) != r_u32(environment + 16u);
    if (!changed) for (index = 0; index < 4u; ++index) {
        sint16 cached = (sint16)r_u16(0x800B2520u + 2u * index);
        sint16 input = (sint16)r_u16(environment + 2u * index);
        if (cached != input) { changed = 1; break; }
    }
    if (changed) {
        uint32 pal = gtmain_video_mode_get();
        w_u8(environment + 18u, (uint8)pal);
        if ((pal & 255u) == 1u) mode |= 8u;
        if (r_u8(environment + 17u)) mode |= 16u;
        if (r_u8(environment + 16u)) mode |= 32u;
        if (r_u8(0x800B24B7u)) mode |= 128u;
        value = (sint16)r_u16(environment + 4u);
        if (value >= 281) {
            if (value < 353) mode |= 1u;
            else if (value < 401) mode |= 64u;
            else if (value < 561) mode |= 2u;
            else mode |= 3u;
        }
        {
            uint32 pal_field = r_u8(environment + 18u);
            value = (sint16)r_u16(environment + 6u);
            if (value >= (pal_field ? 289 : 257)) mode |= 0x24u;
        }
        gpu_control(mode);
    }
    (void)gtmain_memory_copy(0x800B2520u, environment, 20);
    return environment;
}

uint32 gtmain_video_mode_get(void)
{
    return r_u32(0x800B3874u);
}
