#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

static uint32 front_missing(uint32 token, uint32 a0, uint32 a1)
{
    /* TODO Bind actual native device callback result */
    (void)a0; (void)a1;
    gt_draft_failure("Native GPU front callback adapter", token);
}
static uint32 front_read(uint32 address)
{
    /* TODO Bind the original GPU and DMA word bus contracts */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native GPU front word read", address);
    return r_u32(address);
}
static void front_write(uint32 address, uint32 value)
{
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native GPU front word write", address);
    w_u32(address, value);
}
static uint32 front_control(uint32 token, uint32 command)
{
    if (token == 0x80088790u) return gtmain_gpu_control_write(command);
    if (token == 0x80088F04u) return (uint32)gtmain_gpu_queue_sync((sint32)command);
    if (token == 0x80088DB4u) return (uint32)gtmain_gpu_device_reset(command);
    return front_missing(token, command, 0);
}
static sint32 front_enqueue(uint32 dispatcher, uint32 callback, uint32 data, sint32 count, uint32 argument)
{
    if (dispatcher == 0x800888A4u) return gtmain_gpu_command_enqueue(callback, data, count, argument);
    /* TODO Bind the unknown four-argument queue callback */
    (void)callback; (void)data; (void)count; (void)argument;
    gt_draft_failure("Native GPU queue dispatcher", dispatcher);
}
static sint32 front_log(uint32 format, uint32 a0, uint32 a1)
{
    uint32 token = r_u32(0x800B24B0u);
    if (token != 0x8008B7D8u) return (sint32)front_missing(token, format, a0);
    if (format == 0x8008EF8Cu) return printf("SetDispMask(%d)...\n", (int)a0);
    if (format == 0x8008EFA0u) return printf("DrawSync(%d)...\n", (int)a0);
    if (format == 0x8008F02Cu) return printf("ClearOTagR(%08x,%d)...\n", (unsigned)a0, (int)a1);
    if (format == 0x8008F044u) return printf("DrawOTag(%08x)...\n", (unsigned)a0);
    if (format == 0x8008F058u) return printf("PutDrawEnv(%08x)...\n", (unsigned)a0);
    return (sint32)front_missing(token, format, a0);
}
uint32 gtmain_gpu_bytes_fill(uint32 destination, uint8 value, uint32 count)
{
    uint32 remaining = count - 1u;
    if (count) do { w_u8(destination, value); --remaining; ++destination; } while (remaining != 0xFFFFFFFFu);
    return remaining;
}
uint32 gtmain_gpu_control_write(uint32 command)
{
    uint32 opcode;
    front_write(r_u32(0x800B258Cu), command);
    opcode = command >> 24;
    w_u8(0x800B5FC4u + opcode, (uint8)command);
    return opcode;
}
uint32 gtmain_gpu_dma_submit(uint32 packets)
{
    uint32 control;
    front_write(r_u32(0x800B258Cu), 0x04000002u);
    front_write(r_u32(0x800B2590u), packets);
    front_write(r_u32(0x800B2594u), 0);
    control = r_u32(0x800B2598u); front_write(control, 0x01000401u);
    return control;
}
sint32 gtmain_gpu_ordering_dma_clear(uint32 table, uint32 count)
{
    uint32 address = r_u32(0x800B25A8u);
    front_write(address, front_read(address) | 0x08000000u);
    front_write(r_u32(0x800B25A4u), 0);
    front_write(r_u32(0x800B259Cu), table + 4u * count - 4u);
    front_write(r_u32(0x800B25A0u), count);
    front_write(r_u32(0x800B25A4u), 0x11000002u);
    (void)gtmain_gpu_timeout_begin();
    while (front_read(r_u32(0x800B25A4u)) & 0x01000000u)
        if (gtmain_gpu_timeout_poll()) return -1;
    return (sint32)count;
}
sint32 gtmain_gpu_device_reset(uint32 mode)
{
    uint32 saved = front_missing(0x8008A3C4u, 0, 0), zero, selected = mode & 7u, address;
    w_u32(0x800B25B0u, 0); zero = r_u32(0x800B25B0u);
    w_u32(0x800B25BCu, saved); w_u32(0x800B25ACu, zero);
    if (selected == 0u || selected == 5u) {
        front_write(r_u32(0x800B2598u), 1025);
        address = r_u32(0x800B25A8u); front_write(address, front_read(address) | 0x800u);
        front_write(r_u32(0x800B258Cu), 0);
        (void)gtmain_gpu_bytes_fill(0x800B5FC4u, 0, 256);
        (void)gtmain_gpu_bytes_fill(0x800B6140u, 0, 384);
    } else if (selected == 1u || selected == 3u) {
        front_write(r_u32(0x800B2598u), 1025);
        address = r_u32(0x800B25A8u); front_write(address, front_read(address) | 0x800u);
        front_write(r_u32(0x800B258Cu), 0x02000000u);
        front_write(r_u32(0x800B258Cu), 0x01000000u);
    }
    (void)front_missing(0x8008A3C4u, r_u32(0x800B25BCu), 0);
    if (!selected) return (sint32)front_missing(0x800891B8u, mode, 0);
    return 0;
}
sint32 gtmain_gpu_queue_sync(sint32 mode)
{
    uint32 pending;
    if (mode) {
        pending = (r_u32(0x800B25ACu) - r_u32(0x800B25B0u)) & 3u;
        if (pending) (void)front_missing(0x80088B54u, 0, 0);
        if ((front_read(r_u32(0x800B2598u)) & 0x01000000u) ||
            !(front_read(r_u32(0x800B258Cu)) & 0x04000000u))
            return pending ? (sint32)pending : 1;
        return (sint32)pending;
    }
    (void)gtmain_gpu_timeout_begin();
    while (r_u32(0x800B25ACu) != r_u32(0x800B25B0u)) {
        (void)front_missing(0x80088B54u, 0, 0);
        if (gtmain_gpu_timeout_poll()) return -1;
    }
    for (;;) {
        if (!(front_read(r_u32(0x800B2598u)) & 0x01000000u))
            if (front_read(r_u32(0x800B258Cu)) & 0x04000000u) return 0;
        if (gtmain_gpu_timeout_poll()) return -1;
    }
}
uint32 gtmain_gpu_display_mask(uint32 enabled)
{
    uint32 table;
    if (r_u8(0x800B24B6u) >= 2u) (void)front_log(0x8008EF8Cu, enabled, 0);
    if (!enabled) (void)gtmain_gpu_bytes_fill(0x800B2520u, 255, 20);
    table = r_u32(0x800B24ACu);
    return front_control(r_u32(table + 16u), enabled ? 0x03000000u : 0x03000001u);
}
sint32 gtmain_gpu_draw_sync(sint32 mode)
{
    uint32 table;
    if (r_u8(0x800B24B6u) >= 2u) (void)front_log(0x8008EFA0u, (uint32)mode, 0);
    table = r_u32(0x800B24ACu);
    return (sint32)front_control(r_u32(table + 60u), (uint32)mode);
}
sint32 gtmain_gpu_rectangle_report(uint32 label, uint32 rectangle)
{
    uint32 debug = r_u8(0x800B24B6u), token;
    sint32 x, y, width, height, limit;
    if (debug != 1u && debug != 2u) return 2;
    if (debug == 1u) {
        width = (sint16)r_u16(rectangle + 4u); limit = (sint16)r_u16(0x800B24B8u);
        if (limit < width) goto report;
        x = (sint16)r_u16(rectangle);
        if (limit < width + x) goto report;
        y = (sint16)r_u16(rectangle + 2u); limit = (sint16)r_u16(0x800B24BAu);
        if (limit < y) goto report;
        height = (sint16)r_u16(rectangle + 6u);
        if (limit < y + height || width <= 0 || x < 0 || y < 0 || height <= 0) goto report;
        return 0;
    }
report:
    token = r_u32(0x800B24B0u);
    if (token != 0x8008B7D8u) return (sint32)front_missing(token, debug == 1u ? 0x8008EFB4u : 0x8008EFD4u, label);
    printf(debug == 1u ? "%s:bad RECT" : "%s:", (const char *)psx_addr(label, 1));
    x = (sint16)r_u16(rectangle); y = (sint16)r_u16(rectangle + 2u);
    width = (sint16)r_u16(rectangle + 4u); height = (sint16)r_u16(rectangle + 6u);
    token = r_u32(0x800B24B0u);
    if (token != 0x8008B7D8u) return (sint32)front_missing(token, 0x8008EFC0u, rectangle);
    return printf("(%d,%d)-(%d,%d)\n", (int)x, (int)y, (int)width, (int)height);
}
sint32 gtmain_gpu_image_clear(uint32 rectangle, uint8 red, uint8 green, uint8 blue)
{
    uint32 table, callback, dispatcher;
    (void)gtmain_gpu_rectangle_report(0x8008EFD8u, rectangle);
    table = r_u32(0x800B24ACu); callback = r_u32(table + 12u); dispatcher = r_u32(table + 8u);
    return front_enqueue(dispatcher, callback, rectangle, 8, ((uint32)blue << 16) | ((uint32)green << 8) | red);
}
sint32 gtmain_gpu_image_load(uint32 rectangle, uint32 source)
{
    uint32 table, callback, dispatcher;
    (void)gtmain_gpu_rectangle_report(0x8008EFF0u, rectangle);
    table = r_u32(0x800B24ACu); callback = r_u32(table + 32u); dispatcher = r_u32(table + 8u);
    return front_enqueue(dispatcher, callback, rectangle, 8, source);
}
sint32 gtmain_gpu_image_store(uint32 rectangle, uint32 destination)
{
    uint32 table, callback, dispatcher;
    (void)gtmain_gpu_rectangle_report(0x8008EFFCu, rectangle);
    table = r_u32(0x800B24ACu); callback = r_u32(table + 28u); dispatcher = r_u32(table + 8u);
    return front_enqueue(dispatcher, callback, rectangle, 8, destination);
}
uint32 gtmain_gpu_ordering_clear(uint32 table, uint32 count)
{
    uint32 device, callback;
    if (r_u8(0x800B24B6u) >= 2u) (void)front_log(0x8008F02Cu, table, count);
    device = r_u32(0x800B24ACu); callback = r_u32(device + 44u);
    if (callback == 0x80087FC4u) (void)gtmain_gpu_ordering_dma_clear(table, count);
    else (void)front_missing(callback, table, count);
    w_u32(0x800B2574u, (0x800B2560u & 0xFFFFFFu) | 0x04000000u);
    w_u32(table, 0x800B2574u & 0xFFFFFFu);
    return table;
}
sint32 gtmain_gpu_ordering_draw(uint32 table)
{
    uint32 device, callback, dispatcher;
    if (r_u8(0x800B24B6u) >= 2u) (void)front_log(0x8008F044u, table, 0);
    device = r_u32(0x800B24ACu); callback = r_u32(device + 24u); dispatcher = r_u32(device + 8u);
    return front_enqueue(dispatcher, callback, table, 0, 0);
}
uint32 gtmain_gpu_draw_environment(uint32 environment)
{
    uint32 packet = environment + 28u, value, table, callback, dispatcher;
    if (r_u8(0x800B24B6u) >= 2u) (void)front_log(0x8008F058u, environment, 0);
    (void)gtmain_gpu_draw_packet(packet, environment);
    value = r_u32(packet); table = r_u32(0x800B24ACu);
    w_u32(packet, value | 0xFFFFFFu);
    callback = r_u32(table + 24u); dispatcher = r_u32(table + 8u);
    (void)front_enqueue(dispatcher, callback, packet, 64, 0);
    (void)gtmain_memory_copy(0x800B24C4u, environment, 92);
    return environment;
}
