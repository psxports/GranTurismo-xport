#include "game_gtend_registry.h"
#include "psx_spu.h"

uint32 gtend_main(void)
{
    FUNCTION_MARKER(0x80010154u, "GTEND.EXE");
    gtend_8001d224();
    ResetCallbackPSX();
    CdInit();
    gtend_80016840();
    gtend_80015dd0();
    gtend_80019eb0(3u);
    InitGeom();
    gtend_80015e14(1u);
    w_u32(0x80042388u, 0x800168F0u);
    w_u32(0x8004238Cu, 0xFFFFFFFFu);
    gtend_80015e7c(0x80042380u);
    gtend_80011a84(0x8004D4A0u);
    if (!r_u8(0x80054094u)) gtend_return_to_menu();
    gtend_80011b20(0x80023840u, 0x80059840u, 0x4000u);
    gtend_80011c80();
    gtend_sound_resources_initialize();
    gtend_80011d00();
    gtend_80015f08(1u);
    w_u16(0x80025FD4u, 0x7FFFu);
    gtend_80010a18();
    return gtend_return_to_menu();
}

uint32 gtend_return_to_menu(void)
{
    uint32 image;
    FUNCTION_MARKER(0x80010250u, "GTEND.EXE");
    gtend_8001d310();
    image = gtend_80011d5c(0x8002384Cu, 0u, 0u, 0u);
    gtend_80011df0(0x8004D4A0u);
    gtend_80015f8c();
    gtend_80016be0();
    gtend_80015ff0();
    StopCallback();
    return gtend_80011f40(image);
}

uint32 gtend_sound_resources_initialize(void)
{
    uint32 bytes, buffer;
    FUNCTION_MARKER(0x80010324u, "GTEND.EXE");
    bytes = gtend_8001d370();
    buffer = gtend_8001d3c8(bytes);
    gtend_80011f94(0x800423E0u, 0x40000u, buffer, 2048u, 0u);
    return gtend_8001d478(buffer);
}

void gtend_ending_noop(void)
{
    FUNCTION_MARKER(0x80010C10u, "GTEND.EXE");
}

uint32 gtend_controller_initialize(void)
{
    FUNCTION_MARKER(0x800102C0u, "GTEND.EXE");
    return gtend_controller_state_initialize(0x80042390u);
}

uint32 gtend_controller_poll(void)
{
    uint32 packet;
    FUNCTION_MARKER(0x800102E8u, "GTEND.EXE");
    packet = gtend_controller_packet(0u);
    gtend_controller_state_update(0x80042390u, packet);
    return 0x80042390u;
}

uint32 gtend_entry_apply(uint32 index)
{
    uint32 entry;
    FUNCTION_MARKER(0x80010374u, "GTEND.EXE");
    entry = r_u32(0x800423FCu) + index * 20u;
    return gtend_80012058(entry, 0u);
}

uint32 gtend_frame_tick(void)
{
    uint32 frame, count;
    FUNCTION_MARKER(0x800103ACu, "GTEND.EXE");
    frame = r_u32(0x80042BE0u) + 1u;
    w_u32(0x80042BE0u, frame);
    if (frame == 2u) w_u32(0x80042BE0u, 0u);
    count = r_u32(0x8002565Cu);
    w_u8(0x800589F4u, 1u);
    ++count;
    w_u32(0x8002565Cu, count);
    return count;
}

uint32 gtend_80012058(uint32 entry, uint32 alternate)
{
    FUNCTION_MARKER(0x80012058u, "GTEND.EXE");
    if (alternate) {
        gtend_80013c08(entry, 0x4000u, 0u, 0u);
        return gtend_80013c08(entry + 20u, 0x4000u, 127u, 0u);
    }
    return gtend_80013c08(entry, 0x4000u, 64u, 0u);
}

uint32 gtend_controller_state_initialize(uint32 state)
{
    uint32 index, defaults;
    FUNCTION_MARKER(0x80016030u, "GTEND.EXE");
    gtend_controller_buttons_clear(state);
    w_u16(state + 8u, 0u);
    for (index = 0u; index < 16u; ++index) {
        defaults = r_u32(0x80040550u);
        w_u32(state + 10u + index * 4u, defaults);
    }
    return 64u;
}

uint32 gtend_controller_packet(uint32 port)
{
    uint32 packet;
    FUNCTION_MARKER(0x80016090u, "GTEND.EXE");
    packet = 0x80059620u + ((port >> 2) << 6);
    if ((r_u8(packet + 1u) & 0xF0u) == 0x80u)
        packet += (port & 3u) * 8u + 2u;
    return packet;
}

uint32 gtend_controller_state_update(uint32 state, uint32 packet)
{
    uint32 buttons = 0u, kind = 0u, index, repeats = 0u;
    uint32 clear = 1u;
    FUNCTION_MARKER(0x800160D0u, "GTEND.EXE");
    if (packet && !r_u8(packet)) {
        buttons = ~r_u16(packet + 2u);
        kind = r_u8(packet + 1u) & 0xF0u;
        if (kind == 0x20u) {
            uint32 first = r_u8(packet + 5u);
            uint32 second = r_u8(packet + 6u);
            uint32 third = r_u8(packet + 7u);
            if (first >= 104u) buttons |= 0x4000u;
            if (second >= 104u) buttons |= 0x8000u;
            if (third >= 104u) buttons |= 0x400u;
            clear = 0u;
        } else if (kind != 0x10u && kind != 0x30u && kind != 0x60u) {
            clear = 0u;
        }
    }
    if (clear) {
        buttons = 0u;
        gtend_controller_buttons_clear(state);
    } else {
        gtend_controller_buttons_update(state, buttons & 0xFFFFu);
    }
    for (index = 0u; index < 16u; ++index) {
        uint32 mask = 1u << index;
        sint32 repeat = gtend_controller_repeat(state + 10u + index * 4u,
            buttons & mask);
        repeats |= mask & repeat;
    }
    w_u16(state + 8u, repeats);
    return 0u;
}

void gtend_controller_buttons_clear(uint32 state)
{
    FUNCTION_MARKER(0x80016530u, "GTEND.EXE");
    w_u16(state, 0u);
    w_u16(state + 2u, 0u);
    w_u16(state + 4u, 0u);
    w_u16(state + 6u, 0u);
}

uint32 gtend_controller_buttons_update(uint32 state, uint32 buttons)
{
    uint32 valid, previous, changed, released;
    FUNCTION_MARKER(0x80016544u, "GTEND.EXE");
    valid = r_u16(state);
    previous = r_u16(state + 2u);
    w_u16(state, 0xFFFFu);
    w_u16(state + 2u, buttons);
    changed = previous ^ buttons;
    released = changed & previous;
    w_u16(state + 4u, valid & changed & buttons);
    w_u16(state + 6u, valid & released);
    return released;
}

sint32 gtend_controller_repeat(uint32 state, uint32 pressed)
{
    uint32 interval, count, delay;
    sint32 result = 0;
    FUNCTION_MARKER(0x80016578u, "GTEND.EXE");
    if (!pressed) {
        w_u16(state + 2u, 0u);
        return 0;
    }
    interval = r_u8(state + 1u);
    count = r_u16(state + 2u) + 1u;
    delay = r_u8(state);
    if (delay == count) result = -1;
    else if (delay + interval == count) {
        count = delay;
        result = -1;
    }
    w_u16(state + 2u, count);
    return result;
}

uint32 gtend_render_initialize(void)
{
    uint32 context, first;
    FUNCTION_MARKER(0x800103F8u, "GTEND.EXE");
    w_u8(0x800589FCu, 0u);
    gtend_80012124(0x800589A0u, 0u);
    context = r_u32(0x80058A40u);
    w_u32(context + 52u, 0x800103ACu);
    gtend_80012168(4u, 0x80058990u);
    first = r_u32(0x800589F0u);
    gtend_80012180(first, first + 0x20000u);
    first = r_u32(0x800589ECu);
    return gtend_80012198(first, first + 0x1400u);
}

uint32 gtend_render_available(void)
{
    FUNCTION_MARKER(0x8001047Cu, "GTEND.EXE");
    if (!r_s8(0x800589F4u)) return 0xFFFFFFFFu;
    if (!r_s8(0x800589FCu)) return 0xFFFFFFFFu;
    if (r_s8(0x80058980u) <= 0) return r_u32(0x80042BE0u);
    w_u8(0x80058980u, r_u8(0x80058980u) - 1u);
    return 0xFFFFFFFFu;
}

uint32 gtend_render_enable(uint8 mode)
{
    uint32 result;
    FUNCTION_MARKER(0x800104E8u, "GTEND.EXE");
    w_u8(0x800589FCu, 1u);
    w_u8(0x800589FDu, 15u);
    w_u8(0x80058980u, 2u);
    w_u8(0x800589F4u, 0u);
    w_u32(0x80042BE0u, 0u);
    w_u32(0x80042BE4u, 1u);
    gtend_80011cc0(mode);
    result = VSync(-1);
    w_u32(0x800589F8u, result);
    return result;
}

uint32 gtend_render_start(void)
{
    FUNCTION_MARKER(0x80010558u, "GTEND.EXE");
    w_u32(0x8002565Cu, 1u);
    return gtend_8001220c();
}

uint32 gtend_render_tick(void)
{
    sint32 delay;
    uint32 result, status, index;
    FUNCTION_MARKER(0x80010580u, "GTEND.EXE");
    if (!r_s8(0x800589FCu)) return 0u;
    delay = r_s8(0x800589FDu);
    if (delay > 0) {
        --delay;
        w_u8(0x800589FDu, delay);
        result = (uint32)delay << 24;
        if (!result) return gtend_render_start();
        return result;
    }
    status = gtend_80012254();
    if (status == 1u) VSync(-1);
    else if (!status) {
        sint32 x, y;
        index = r_u32(0x80042BE4u) * 4u;
        x = r_s16(0x80025654u + index);
        y = r_s16(0x80025656u + index);
        gtend_80012460(x, y);
        index = r_u32(0x80042BE4u) + 1u;
        w_u32(0x80042BE4u, index);
        if (index == 2u) w_u32(0x80042BE4u, 0u);
    }
    result = VSync(-1);
    w_u32(0x800589F8u, result);
    return result;
}

uint32 gtend_render_disable(void)
{
    FUNCTION_MARKER(0x80010680u, "GTEND.EXE");
    w_u8(0x800589FCu, 0u);
    return gtend_800122cc();
}

uint32 gtend_render_wait(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800106A8u, "GTEND.EXE");
    gtend_800122fc();
    do {
        result = gtend_800124a0(0u);
    } while (result);
    return result;
}

uint32 gtend_textured_quad(uint32 ordering, uint32 position, uint32 size, sint16 u, uint32 uv_size, uint16 tpage, uint16 clut, uint32 color)
{
    uint32 primitive, next, y, bottom, dv;
    sint32 x, right, du, end_u;
    FUNCTION_MARKER(0x800106D8u, "GTEND.EXE");
    x = (sint16)position;
    right = x + (sint16)size;
    y = position >> 16;
    bottom = y + (size >> 16);
    du = (sint16)uv_size;
    dv = uv_size >> 16;
    primitive = r_u32(0x80058A4Cu);
    w_u32(0x80058A4Cu, primitive + 40u);
    next = r_u32(ordering);
    w_masked_u32(ordering, primitive, 7u);
    w_masked_u32(primitive, next, 7u);
    end_u = u + du;
    w_u16(primitive + 10u, y);
    w_u16(primitive + 18u, y);
    w_u8(primitive + 13u, 0u);
    w_u8(primitive + 21u, 0u);
    w_u8(primitive + 3u, 9u);
    w_u32(primitive + 4u, color ^ 0x2D000000u);
    w_u16(primitive + 14u, clut);
    w_u16(primitive + 22u, tpage);
    w_u16(primitive + 8u, x);
    w_u16(primitive + 16u, right);
    w_u16(primitive + 24u, x);
    w_u16(primitive + 26u, bottom);
    w_u16(primitive + 32u, right);
    w_u16(primitive + 34u, bottom);
    w_u8(primitive + 12u, u);
    w_u8(primitive + 20u, end_u);
    w_u8(primitive + 28u, u);
    w_u8(primitive + 29u, dv);
    w_u8(primitive + 36u, end_u);
    w_u8(primitive + 37u, dv);
    return primitive;
}

uint32 gtend_ending_draw(void)
{
    uint32 frame, metric, brightness, count, progress, color, tpage;
    FUNCTION_MARKER(0x800107BCu, "GTEND.EXE");
    gtend_800115ec(0x80042BE8u);
    frame = gtend_render_available();
    if ((sint32)frame >= 0) {
        metric = gtend_80011a58();
        brightness = metric << 1;
        if ((sint32)metric >= 64) {
            count = r_u32(0x8002565Cu);
            progress = r_u32(0x80058A1Cu);
            if (count < 16u) brightness = (count << 3) - progress;
            else brightness = 128u - progress;
            if ((sint32)brightness < 0) brightness = 0u;
        }
        brightness &= 0xFFu;
        color = 0x01000000u | brightness | (brightness << 8) | (brightness << 16);
        tpage = 0x320u | (frame ? 0x1Au : 0xAu);
        gtend_textured_quad(r_u32(0x80042C00u), 0x00300020u, 0x013E01FEu,
            0, 0x009F00FFu, tpage, 0u, color);
    }
    gtend_80011878(0x80042BE8u);
    if (r_s32(0x80058A1Cu) >= 300) w_u32(0x80025660u, 1u);
    return 1u;
}

uint32 gtend_ending_input(void)
{
    uint32 state;
    FUNCTION_MARKER(0x8001090Cu, "GTEND.EXE");
    gtend_render_tick();
    gtend_80016200();
    state = gtend_controller_poll();
    if (r_u16(state + 4u) & 8u) w_u32(0x80058A28u, 2u);
    return 2u;
}

uint32 gtend_ending_complete(void)
{
    FUNCTION_MARKER(0x80010958u, "GTEND.EXE");
    return r_u32(0x80025660u);
}

uint32 gtend_wait_sixty_frames(void)
{
    uint32 frame;
    FUNCTION_MARKER(0x80010968u, "GTEND.EXE");
    for (frame = 0u; frame < 60u; ++frame) VSync(0);
    return 0u;
}

uint32 gtend_wait_frame(void)
{
    FUNCTION_MARKER(0x800109A0u, "GTEND.EXE");
    return VSync(0);
}

uint32 gtend_ending_clear(void)
{
    uint32 frame;
    FUNCTION_MARKER(0x800109C0u, "GTEND.EXE");
    gtend_80012534(0u);
    for (frame = 0u; frame < 4u; ++frame) {
        gtend_8001a3b0(0x80025664u, 0u, 0u, 0u);
        VSync(0);
    }
    return 0u;
}

uint32 gtend_packed_bytes_entry(uint32 base, uint32 index)
{
    FUNCTION_MARKER(0x8001362Cu, "GTEND.EXE");
    while (index)
    {
        uint32 size = r_u8(base);
        --index;
        base += size + 1u;
    }
    return base;
}

uint32 gtend_packed_pairs_entry(uint32 base, uint32 index)
{
    FUNCTION_MARKER(0x80013650u, "GTEND.EXE");
    while (index)
    {
        uint32 size = r_u8(base);
        --index;
        base += size * 2u + 1u;
    }
    return base;
}

uint32 gtend_bcd_decode(uint32 packed)
{
    FUNCTION_MARKER(0x80014140u, "GTEND.EXE");
    uint32 shift = 32u, value = 0u;
    do { shift -= 4u; value = value * 10u + ((packed >> shift) & 15u); } while (shift);
    return value;
}

uint32 gtend_data_checksum(uint32 address, uint32 size)
{
    /* Original occurrences
     * GTEND.EXE:0x80016434
     * GTMENU.EXE:0x80040AB0
     * SCUS_941.94:0x80013278
     */
    uint32 sum = 0xAAAAu, crc = 0x3770u, i, bit;
    for (i = 0u; i < size; ++i) {
        uint32 value = r_u8(address + i);
        sum = (sum + value) ^ (value << 8);
        for (bit = 0u; bit < 8u; ++bit) {
            crc <<= 1;
            if (crc & 0x10000u) crc ^= 0x11021u;
            crc |= (value >> 7) & 1u;
            value <<= 1;
        }
    }
    return (sum << 16) | (crc & 0xFFFFu);
}

uint32 gtend_gpu_draw_mode(uint32 dither, uint32 display, uint16 texture_page)
{
    FUNCTION_MARKER(0x8001B4F8u, "GTEND.EXE");
    uint32 command = display ? 0xE1000200u : 0xE1000000u;
    uint32 bits = texture_page & 0x9FFu;
    if (dither) bits |= 0x400u;
    return command | bits;
}

uint32 gtend_gpu_draw_offset(sint16 x, sint16 y)
{
    FUNCTION_MARKER(0x8001B648u, "GTEND.EXE");
    return 0xE5000000u | ((uint32)y & 0x7FFu) << 11 | ((uint32)x & 0x7FFu);
}

uint32 gtend_gpu_bytes_fill(uint32 destination, uint8 value, uint32 count)
{
    FUNCTION_MARKER(0x8001CDD0u, "GTEND.EXE");
    uint32 remaining = count - 1u;
    if (count) do { w_u8(destination, value); --remaining; ++destination; } while (remaining != 0xFFFFFFFFu);
    return remaining;
}

uint32 gtend_memory_copy(uint32 destination, uint32 source, uint32 size)
{
    FUNCTION_MARKER(0x8001D5A8u, "GTEND.EXE");
    uint32 output = destination;
    if (source != destination) while (size--) w_u8(output++, r_u8(source++));
    return destination;
}

sint32 gtend_path_separator(sint8 character)
{
    FUNCTION_MARKER(0x8001E264u, "GTEND.EXE");
    return character == 47 || character == 92;
}

void gtend_file_retry_hook(void)
{
    FUNCTION_MARKER(0x80013940u, "GTEND.EXE");
}

void gtend_runtime_noop(void)
{
    FUNCTION_MARKER(0x8001E784u, "GTEND.EXE");
}

uint32 gtend_credits_layout(void)
{
    uint32 total, index = 0u;
    FUNCTION_MARKER(0x80011154u, "GTEND.EXE");
    total = r_u32(0x80025FB8u);
    w_u32(0x80025FBCu, 0u);
    w_u32(0x80025FC0u, 0u);
    if ((sint32)total <= 0) return total;
    for (;;) {
        uint32 source = 0x80025678u + index * 8u;
        uint32 kind = r_u32(source);
        if (kind == 9u) return 8u;
        if (kind == 8u) {
            w_u32(0x80025FC0u, r_u32(0x80025FC0u) + 82u);
        } else {
            uint32 row = 0x80042C08u + r_u32(0x80025FBCu) * 28u;
            uint32 text;
            sint32 font, width;
            w_u32(row, r_u32(source));
            w_u32(row + 24u, r_u32(source + 4u));
            kind = r_u32(source);
            switch (kind) {
            case 0u:
            case 1u:
            case 2u: {
                uint32 height = r_u32(0x80025FC0u);
                uint32 number = r_u32(0x80025FBCu);
                height += kind == 0u ? 240u : kind == 1u ? 76u : 20u;
                w_u32(0x80025FC0u, height);
                w_u32(0x80042C1Cu + number * 28u, 3u);
                break;
            }
            case 4u:
            case 6u: {
                uint32 number = r_u32(0x80025FBCu);
                uint32 height = r_u32(0x80025FC0u);
                w_u32(0x80025FC0u, height + 84u);
                w_u32(0x80042C1Cu + number * 28u, 3u);
                break;
            }
            case 3u:
            case 5u:
            case 7u: {
                uint32 number = r_u32(0x80025FBCu);
                uint32 height = r_u32(0x80025FC0u);
                w_u32(0x80025FC0u, height + (kind == 3u ? 4u : 20u));
                w_u32(0x80042C1Cu + number * 28u, 2u);
                break;
            }
            default:
                break;
            }
            row = 0x80042C08u + r_u32(0x80025FBCu) * 28u;
            font = r_s32(row + 20u);
            w_u32(row + 16u, font == 3u ? 26u : 20u);
            row = 0x80042C08u + r_u32(0x80025FBCu) * 28u;
            total = r_u32(0x80025FC0u);
            font = r_s32(row + 20u);
            text = r_u32(row + 24u);
            w_u32(row + 8u, total);
            width = gtend_8001343c(font, text);
            row = 0x80042C08u + r_u32(0x80025FBCu) * 28u;
            w_u32(row + 12u, width);
            w_u32(row + 4u, 608u - width);
            kind = r_u32(source);
            switch (kind) {
            case 0u:
                w_u32(0x80025FC0u, r_u32(0x80025FC0u) + 100u);
                break;
            case 1u:
            case 2u:
            case 4u:
                w_u32(0x80025FC0u, r_u32(0x80025FC0u) + 26u);
                break;
            case 3u:
            case 5u:
                w_u32(0x80025FC0u, r_u32(0x80025FC0u) + 12u);
                break;
            case 6u:
            case 7u: {
                uint32 number = r_u32(0x80025FBCu);
                uint32 height = r_u32(0x80025FC0u);
                sint32 signed_width = r_s32(0x80042C14u + number * 28u);
                w_u32(0x80025FC0u, height + (kind == 6u ? 26u : 12u));
                w_u32(0x80042C0Cu + number * 28u, 320 - signed_width / 2);
                break;
            }
            default:
                break;
            }
            w_u32(0x80025FBCu, r_u32(0x80025FBCu) + 1u);
        }
        total = r_u32(0x80025FB8u);
        ++index;
        if ((sint32)index >= (sint32)total) return index << 3;
    }
}

uint32 gtend_credits_reset(void)
{
    FUNCTION_MARKER(0x800115B8u, "GTEND.EXE");
    w_u32(0x80044C6Cu, 0u);
    w_u32(0x80058A28u, 0u);
    w_u32(0x80044C68u, 0u);
    w_u32(0x80058A1Cu, 0u);
    w_u32(0x80044C70u, 90u);
    return 90u;
}

uint32 gtend_80011a58(void)
{
    uint32 scroll, height, distance;
    FUNCTION_MARKER(0x80011A58u, "GTEND.EXE");
    scroll = r_u32(0x80044C6Cu);
    height = r_u32(0x80025FC0u);
    distance = height - (scroll - 240u);
    return (sint32)distance < 0 ? 0u : distance;
}

uint32 gtend_800115ec(uint32 state)
{
    uint32 progress, ordering, fade, low, high, gray;
    uint32 index, scroll, threshold, y;
    FUNCTION_MARKER(0x800115ECu, "GTEND.EXE");
    progress = r_u32(0x80058A1Cu);
    ordering = r_u32(state + 24u);
    fade = 128u - progress;
    if ((sint32)fade < 0) fade = 0u;
    low = ((sint32)(fade * 48u) / 128) & 0xFFu;
    high = ((sint32)(fade * 80u) / 128) & 0xFFu;
    gray = high | (high << 8) | (high << 16);
    index = r_u32(0x80044C68u);
    w_u32(0x80058A20u, low | (low << 8) | (high << 16));
    scroll = r_u32(0x80044C6Cu);
    w_u32(0x80058A10u, gray);
    w_u32(0x80058A14u, high | (low << 8) | (low << 16));
    w_u32(0x80058A18u, gray);
    w_u32(0x80058A24u, gray);
    y = r_u32(0x80042C10u + index * 28u);
    threshold = scroll - 510u;
    while ((sint32)y < (sint32)threshold) {
        ++index;
        y = r_u32(0x80042C10u + index * 28u);
        w_u32(0x80044C68u, index);
    }
    index = r_u32(0x80044C68u);
    while ((sint32)index < r_s32(0x80025FBCu)) {
        uint32 row = 0x80042C08u + index * 28u;
        uint32 kind, color;
        sint32 x;
        uint32 text;
        gtend_800132cc(r_s32(row + 20u));
        scroll = r_u32(0x80044C6Cu);
        y = r_u32(row + 8u) - (scroll - 480u);
        if ((sint32)y >= 481) return 0u;
        kind = r_u32(row);
        switch (kind) {
        case 0u: color = r_u32(0x80058A14u); break;
        case 1u: color = r_u32(0x80058A20u); break;
        case 2u: color = r_u32(0x80058A18u); break;
        case 3u: color = r_u32(0x80058A24u); break;
        case 4u:
        case 5u:
        case 6u:
        case 7u: color = r_u32(0x80058A10u); break;
        default: color = 0u; break;
        }
        if (kind < 8u) gtend_800133b0(color);
        x = r_s32(0x80042C0Cu + index * 28u);
        text = r_u32(0x80042C20u + index * 28u);
        gtend_8001354c(ordering, x, (sint32)y, text);
        ++index;
    }
    return index << 3;
}

uint32 gtend_80011878(uint32 state)
{
    uint32 first, ordering, index = 0u, scroll, height, mode, result = 2u;
    FUNCTION_MARKER(0x80011878u, "GTEND.EXE");
    first = r_u32(0x80044C68u);
    ordering = r_u32(state + 24u);
    if ((sint32)first > 0) index = first - 1u;
    while ((sint32)index < r_s32(0x80025FBCu)) {
        uint32 row = 0x80042C08u + index * 28u;
        uint32 y, left, right, bottom, corners[2];
        scroll = r_u32(0x80044C6Cu);
        y = r_u32(row + 8u) - scroll + 484u;
        if ((sint32)y >= 481) break;
        left = r_u16(row + 4u);
        corners[0] = left | ((y & 0xFFFFu) << 16);
        left = r_u16(row + 4u);
        right = r_u16(row + 12u);
        right = left + right - 1u;
        bottom = r_u16(row + 16u);
        bottom += y - 1u;
        corners[1] = (right & 0xFFFFu) | ((bottom & 0xFFFFu) << 16);
        gtend_8001358c(ordering, corners, 0u);
        ++index;
    }
    height = r_u32(0x80025FC0u);
    scroll = r_u32(0x80044C6Cu);
    if ((sint32)scroll < (sint32)(height + 240u)) {
        w_u32(0x80044C6Cu, scroll + 2u);
        mode = r_u32(0x80058A28u);
    } else {
        mode = r_u32(0x80058A28u);
        if (!mode) {
            w_u32(0x80058A28u, 1u);
            w_u32(0x80044C70u, 90u);
            mode = r_u32(0x80058A28u);
        } else if (mode == 1u) {
            uint32 delay = r_u32(0x80044C70u) - 1u;
            w_u32(0x80044C70u, delay);
            if (!delay) w_u32(0x80058A28u, 2u);
            mode = r_u32(0x80058A28u);
        }
    }
    if (mode == 2u) {
        uint32 progress = r_u32(0x80058A1Cu);
        result = progress + 3u;
        if ((sint32)progress < 300) w_u32(0x80058A1Cu, result);
    }
    return result;
}

uint32 gtend_800132cc(sint32 font)
{
    uint32 page, metrics, glyphs, advance;
    FUNCTION_MARKER(0x800132CCu, "GTEND.EXE");
    page = r_u16(0x80058B48u);
    switch (font) {
    case 0:
        metrics = 0x8002BB30u; glyphs = 0x8002DBB0u; advance = 6u;
        break;
    case 1:
        metrics = 0x8002DFB0u; glyphs = 0x80033AB0u; advance = 8u; page += 64u;
        break;
    case 2:
        metrics = 0x80033EB0u; glyphs = 0x800393B0u; advance = 12u; page += 128u;
        break;
    case 3:
        metrics = 0x800397B0u; glyphs = 0x8003F630u; advance = 24u; page += 192u;
        break;
    default:
        return font < 2 ? 1u : 3u;
    }
    w_u16(0x80058B5Au, font);
    w_u32(0x80058B40u, metrics);
    w_u32(0x80058B44u, glyphs);
    w_u16(0x80058B5Eu, advance);
    w_u16(0x80058B58u, page);
    return advance;
}

uint32 gtend_800133b0(uint32 color)
{
    FUNCTION_MARKER(0x800133B0u, "GTEND.EXE");
    w_masked_u32(0x80058B4Cu, color, 7u);
    return 0x80058B4Eu;
}

uint32 gtend_8001354c(uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    FUNCTION_MARKER(0x8001354Cu, "GTEND.EXE");
    return gtend_80014b7c(0x80058B40u, ordering, x, y, text);
}

uint32 gtend_8001358c(uint32 ordering, const uint32 corners[2], uint32 color)
{
    uint32 first, second, primitive, next;
    sint32 left, top, right, bottom;
    sint32 reversed;
    FUNCTION_MARKER(0x8001358Cu, "GTEND.EXE");
    first = corners[0];
    second = corners[1];
    top = (sint16)(first >> 16);
    left = (sint16)first;
    bottom = (sint16)(second >> 16);
    reversed = bottom < top;
    top += reversed;
    bottom += reversed ^ 1;
    primitive = r_u32(0x80058A4Cu);
    right = (sint16)second;
    w_u32(primitive, 0x05000000u);
    reversed = right < left;
    left += reversed;
    right += reversed ^ 1;
    w_u32(primitive + 4u, color ^ 0x29000000u);
    w_u16(primitive + 8u, left);
    w_u16(primitive + 10u, top);
    w_u16(primitive + 12u, right);
    w_u16(primitive + 14u, top);
    w_u16(primitive + 16u, left);
    w_u16(primitive + 18u, bottom);
    w_u16(primitive + 20u, right);
    w_u16(primitive + 22u, bottom);
    next = r_u32(ordering);
    w_masked_u32(ordering, primitive, 7u);
    w_masked_u32(primitive, next, 7u);
    w_u32(0x80058A4Cu, primitive + 24u);
    return primitive;
}

sint32 gtend_8001343c(sint32 font, uint32 text)
{
    uint32 metrics, kerning, width = 0u;
    sint32 character;
    FUNCTION_MARKER(0x8001343Cu, "GTEND.EXE");
    switch (font) {
    case 0: metrics = 0x8002BB30u; kerning = 0x8002DBB0u; break;
    case 1: metrics = 0x8002DFB0u; kerning = 0x80033AB0u; break;
    case 2: metrics = 0x80033EB0u; kerning = 0x800393B0u; break;
    case 3: metrics = 0x800397B0u; kerning = 0x8003F630u; break;
    default: return 0u;
    }
    character = r_s8(text);
    while (character) {
        uint32 code = character & 0xFFu;
        uint32 glyph, packed, advance;
        sint32 next;
        ++text;
        next = r_s8(text);
        glyph = r_u32(kerning + code * 4u);
        packed = r_u32(glyph + (((uint32)next >> 1) & 0x7Cu));
        advance = r_u8(metrics + code * 8u + 6u);
        packed >>= (next & 7u) * 4u;
        width += 1u + advance - (packed & 15u);
        character = next;
    }
    return (sint32)width;
}

uint32 gtend_80011a84(uint32 state)
{
    uint32 checksum, stored;
    FUNCTION_MARKER(0x80011A84u, "GTEND.EXE");
    gtend_800163f4(0x2000u);
    gtend_8001640c(0u);
    gtend_80021830(state, 0xB3C0u);
    SpuIsTransferCompleted(1);
    checksum = gtend_data_checksum(state, 0xB398u);
    if (r_u32(state) == 0x47546F73u) {
        stored = r_u32(state + 0xB398u);
        if (stored == checksum) return stored;
    }
    gtend_8001d504(state, 0u, 0xB3C0u);
    w_u32(state, 0x47546F73u);
    return 0x47546F73u;
}

uint32 gtend_80011b20(uint32 path, uint32 buffer, uint32 size)
{
    uint32 image, cursor;
    sint32 first, second, third, fourth, fifth;
    FUNCTION_MARKER(0x80011B20u, "GTEND.EXE");
    image = gtend_80011d5c(path, buffer, size, 0u);
    cursor = (image + 0x4Bu) & ~3u;
    first = r_s16(image + 12u);
    w_u32(image + 24u, cursor);
    cursor = (gtend_packed_bytes_entry(cursor, first) + 3u) & ~3u;
    w_u32(image + 28u, cursor);
    cursor = (cursor + first * 16u + 3u) & ~3u;
    second = r_s16(image + 14u);
    w_u32(image + 32u, cursor);
    cursor = (gtend_packed_bytes_entry(cursor, first) + 3u) & ~3u;
    w_u32(image + 36u, cursor);
    cursor = (cursor + second * 6u + 35u) & ~3u;
    w_u32(image + 40u, cursor);
    cursor = (gtend_packed_pairs_entry(cursor, second) + 3u) & ~3u;
    third = r_s16(image + 16u);
    w_u32(image + 44u, cursor);
    cursor = (gtend_packed_bytes_entry(cursor, second) + 3u) & ~3u;
    w_u32(image + 48u, cursor);
    cursor = (cursor + third * 2u + 3u) & ~3u;
    fourth = r_s16(image + 18u);
    w_u32(image + 52u, cursor);
    cursor = (gtend_packed_bytes_entry(cursor, fourth) + 3u) & ~3u;
    w_u32(image + 56u, cursor);
    cursor = (cursor + fourth * 8u + 3u) & ~3u;
    fifth = r_s16(image + 20u);
    w_u32(image + 60u, cursor);
    cursor = (gtend_packed_bytes_entry(cursor, fourth) + 3u) & ~3u;
    w_u32(image + 64u, cursor);
    cursor = (cursor + fifth * 8u + 3u) & ~3u;
    w_u32(image + 68u, cursor);
    return 1u;
}

uint32 gtend_80015f08(uint32 enabled)
{
    uint32 previous, value;
    FUNCTION_MARKER(0x80015F08u, "GTEND.EXE");
    if (!spu_mmio_read(NULL, 0x1F801DAAu, 2u, &previous))
        gt_draft_failure("Native SPU halfword control read", 0x80015F08u);
    value = enabled ? previous | 1u : previous & 0xFFFEu;
    if (!spu_mmio_write(NULL, 0x1F801DAAu, 2u, value, 3u))
        gt_draft_failure("Native SPU halfword control write", 0x80015F08u);
    return previous;
}

void gtend_text_command(uint8 flags)
{
    FUNCTION_MARKER(0x8001339Cu, "GTEND.EXE");
    w_u8(0x80058B4Fu, flags | 0x64u);
}

uint32 gtend_text_fixed_width(sint32 font, uint32 text)
{
    uint32 advance;
    FUNCTION_MARKER(0x800133C4u, "GTEND.EXE");
    switch (font) {
    case 0: advance = 6u; break;
    case 1: advance = 8u; break;
    case 2: advance = 12u; break;
    case 3: advance = 24u; break;
    default: return 0u;
    }
    return gtend_8001e1e4(text) * advance;
}

sint32 gtend_music_enabled(void)
{
    FUNCTION_MARKER(0x800137ECu, "GTEND.EXE");
    return r_s16(0x80059852u);
}

uint32 gtend_music_path(void)
{
    FUNCTION_MARKER(0x800137FCu, "GTEND.EXE");
    return gtend_8001e600(0x80024978u);
}

uint32 gtend_sound_path(void)
{
    FUNCTION_MARKER(0x80013948u, "GTEND.EXE");
    return gtend_8001e600(0x80024984u);
}

void gtend_sprite_command(uint8 flags)
{
    FUNCTION_MARKER(0x80013CD8u, "GTEND.EXE");
    w_u8(0x80058C94u, flags ^ 0x68u);
}

void gtend_sprite_uv(uint8 u, uint8 v)
{
    FUNCTION_MARKER(0x80013CECu, "GTEND.EXE");
    w_u8(0x8002BB2Cu, u);
    w_u8(0x8002BB2Du, v);
}

void gtend_sprite_tpage(uint16 page)
{
    FUNCTION_MARKER(0x80013D04u, "GTEND.EXE");
    w_u16(0x8002BB28u, page);
}

uint32 gtend_sprite_scale_exchange(uint32 scale)
{
    uint32 previous;
    FUNCTION_MARKER(0x80013D70u, "GTEND.EXE");
    previous = r_u32(0x8002BB24u);
    w_u32(0x8002BB24u, scale);
    return previous;
}

uint32 gtend_ordering_initialize(uint32 first, uint32 second, uint32 count)
{
    FUNCTION_MARKER(0x8001403Cu, "GTEND.EXE");
    w_u32(0x80058C70u, 0u);
    w_u32(0x80058C74u, first);
    w_u32(0x80058C78u, second);
    w_u32(0x80058C7Cu, 0u);
    w_u32(0x80058C80u, count);
    return 0x80058C70u;
}

uint32 gtend_ordering_clear(sint32 mode)
{
    FUNCTION_MARKER(0x8001405Cu, "GTEND.EXE");
    if (mode == 0) return gtend_80015644(r_u32(0x80058C58u));
    if (mode == 1) return gtend_80015644(r_u32(0x80058C5Cu));
    if (mode == 2) {
        gtend_80015644(0u);
        gtend_80015644(1u);
        return 0u;
    }
    return mode < 2 ? 1u : 2u;
}

uint32 gtend_bcd_encode(uint32 value)
{
    uint32 result = 0u, shift = 0u;
    FUNCTION_MARKER(0x80014170u, "GTEND.EXE");
    while (value && shift < 32u) {
        uint32 digit = r_u8(0x80026098u + value % 100u);
        value /= 100u;
        result |= digit << shift;
        shift += 8u;
    }
    return result;
}

uint32 gtend_font_draw_mode(uint32 font_state, uint32 ordering)
{
    uint32 status, command, primitive, next;
    FUNCTION_MARKER(0x80014B24u, "GTEND.EXE");
    status = r_u32(0x1F801814u);
    command = r_u16(font_state + 10u) | (status & 0x600u) | 0xE1000000u;
    primitive = r_u32(0x80058A4Cu);
    w_u32(primitive, 0x01000000u);
    w_u32(primitive + 4u, command);
    next = r_masked_u32(ordering, 7u);
    w_masked_u32(ordering, primitive, 7u);
    w_masked_u32(primitive, next, 7u);
    w_u32(0x80058A4Cu, primitive + 8u);
    return primitive;
}

uint32 gtend_80014b7c(uint32 font_state, uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    uint32 result, link, primitive, metrics, kerning, color, clut;
    uint32 x_word = (uint32)x;
    sint32 character;
    FUNCTION_MARKER(0x80014B7Cu, "GTEND.EXE");
    result = gtend_font_draw_mode(font_state, ordering);
    primitive = r_u32(0x80058A4Cu);
    metrics = r_u32(font_state);
    kerning = r_u32(font_state + 4u);
    color = r_u32(font_state + 12u);
    character = r_s8(text);
    clut = r_u16(font_state + 24u);
    link = result;
    while (character) {
        uint32 code = character & 0xFFu;
        uint32 glyph = metrics + code * 8u;
        uint32 shape = r_u32(glyph);
        uint32 detail = r_u32(glyph + 4u);
        uint32 height = shape >> 24;
        uint32 table, packed;
        if (height) {
            uint32 next;
            w_u16(primitive + 10u, (uint32)y + ((detail >> 8) & 0xFFu) - 4u);
            w_u16(primitive + 16u, (shape >> 16) & 0xFFu);
            w_u32(primitive, 0x04000000u);
            w_u32(primitive + 4u, color);
            w_u16(primitive + 8u, x_word);
            w_u16(primitive + 18u, height);
            w_u16(primitive + 12u, shape);
            w_u16(primitive + 14u, clut);
            next = r_masked_u32(link, 7u);
            w_masked_u32(link, primitive, 7u);
            w_masked_u32(primitive, next, 7u);
            primitive += 20u;
        }
        ++text;
        character = r_s8(text);
        table = r_u32(kerning + code * 4u);
        packed = r_u32(table + (((uint32)character >> 1) & 0x7Cu));
        packed >>= (character & 7u) * 4u;
        result = ((detail >> 16) & 0xFFu) - (packed & 15u);
        x_word += 1u + result;
    }
    w_u32(0x80058A4Cu, primitive);
    return result;
}

uint32 gtend_ordering_partition(void)
{
    uint32 first, node, second, result = 0x80058C74u;
    FUNCTION_MARKER(0x8001420Cu, "GTEND.EXE");
    first = r_u32(0x80058C74u);
    node = r_u32(0x80058C70u);
    second = r_u32(0x80058C78u);
    while (node) {
        uint32 first_count = r_u32(node + 12u);
        uint32 second_count = r_u32(node + 12u);
        w_u32(node + 24u, first);
        w_u32(node + 16u, first);
        w_u32(node + 20u, second);
        node = r_u32(node + 4u);
        result = first_count * 4u;
        first += result;
        second += second_count * 4u;
    }
    return result;
}

uint32 gtend_ordering_submit(uint32 slot)
{
    uint32 end;
    FUNCTION_MARKER(0x8001437Cu, "GTEND.EXE");
    end = gtend_ordering_end(slot);
    return gtend_8001a820(end);
}

uint32 gtend_display_current(void)
{
    FUNCTION_MARKER(0x800143A4u, "GTEND.EXE");
    gtend_display_buffer(0u);
    return gtend_80012534(1u);
}

uint32 gtend_80015644(uint32 slot)
{
    uint32 ordering, count, result, node, offset;
    FUNCTION_MARKER(0x80015644u, "GTEND.EXE");
    ordering = r_u32(0x80058C74u + slot * 4u);
    count = r_u32(0x80058C80u);
    result = gtend_8001a718(ordering, count);
    node = r_u32(0x80058C70u);
    offset = slot * 4u;
    if (node) {
        result = offset + node;
        do {
            uint32 head = r_u32(result + 16u);
            w_u32(node + 24u, head);
            node = r_u32(node + 4u);
            result = offset + node;
        } while (node);
    }
    return result;
}

uint32 gtend_ordering_end(uint32 slot)
{
    uint32 count, base;
    FUNCTION_MARKER(0x800156C0u, "GTEND.EXE");
    count = r_u32(0x80058C80u);
    base = r_u32(0x80058C74u + slot * 4u);
    return count * 4u + base - 4u;
}

uint32 gtend_display_buffer(uint32 alternate)
{
    uint32 slot;
    FUNCTION_MARKER(0x800156E8u, "GTEND.EXE");
    slot = r_u32(alternate ? 0x80058C5Cu : 0x80058C58u);
    return gtend_8001aa5c(0x80058B78u + slot * 20u);
}

uint32 gtend_ordering_submit_buffer(uint32 alternate)
{
    uint32 slot;
    FUNCTION_MARKER(0x80012CD4u, "GTEND.EXE");
    slot = r_u32(alternate ? 0x80058C5Cu : 0x80058C58u);
    return gtend_ordering_submit(slot);
}

uint32 gtend_text_begin(uint32 ordering)
{
    uint32 primitive;
    FUNCTION_MARKER(0x800131D0u, "GTEND.EXE");
    primitive = gtend_font_draw_mode(0x80058B40u, ordering);
    w_u32(0x80058B54u, primitive);
    return primitive;
}

uint32 gtend_80011df0(uint32 state)
{
    sint32 frames;
    uint32 elapsed, checksum, voice;
    FUNCTION_MARKER(0x80011DF0u, "GTEND.EXE");
    frames = r_s32(0x8005952Cu);
    elapsed = r_u32(state + 64u);
    w_u32(state, 0x47546F73u);
    w_u32(state + 64u, elapsed + frames / 60);
    checksum = gtend_data_checksum(state, 0xB398u);
    w_u32(state + 0xB398u, checksum);
    for (voice = 0u; voice < 24u; ++voice) {
        uint32 address = 0x1F801C00u + voice * 16u;
        if (!spu_mmio_write(NULL, address + 2u, 2u, 0u, 3u))
            gt_draft_failure("Native SPU voice right volume write", 0x80011DF0u);
        if (!spu_mmio_write(NULL, address, 2u, 0u, 3u))
            gt_draft_failure("Native SPU voice left volume write", 0x80011DF0u);
    }
    gtend_800163f4(0x2000u);
    gtend_8001640c(0u);
    gtend_80021940(state, 0xB3C0u);
    return SpuIsTransferCompleted(1);
}

uint32 gtend_text_character(uint8 character)
{
    uint32 position, metrics, shape, detail, height;
    sint32 advance, spacing;
    FUNCTION_MARKER(0x800130B8, "GTEND.EXE");
    position = r_u32(0x80058B50);
    metrics = r_u32(0x80058B40);
    advance = r_s16(0x80058B5C);
    spacing = r_s16(0x80058B5E);
    metrics += character * 8;
    shape = r_u32(metrics);
    detail = r_u32(metrics + 4);
    height = shape >> 24;
    if (height) {
        sint32 x = (sint16)position + (uint8)detail + advance;
        sint32 y = (sint16)(position >> 16) + (uint8)(detail >> 8) - 4;
        uint32 primitive = r_u32(0x80058A4C);
        uint32 link = r_u32(0x80058B54);
        uint32 color = r_u32(0x80058B4C);
        uint32 clut = r_u16(0x80058B58);
        uint32 next;
        w_u16(primitive + 10, y);
        w_u32(primitive, 0x04000000);
        w_u32(primitive + 4, color);
        w_u16(primitive + 8, x);
        w_u16(primitive + 16, (shape >> 16) & 0xFF);
        w_u16(primitive + 18, height);
        w_u16(primitive + 12, shape);
        w_u16(primitive + 14, clut);
        next = r_masked_u32(link, 7);
        w_masked_u32(link, primitive, 7);
        w_masked_u32(primitive, next, 7);
        w_u32(0x80058B54, primitive);
        w_u32(0x80058A4C, primitive + 20);
    }
    w_u16(0x80058B5C, advance + spacing);
    return 0;
}

void gtend_text_position(sint16 x, sint16 y)
{
    FUNCTION_MARKER(0x800131A0, "GTEND.EXE");
    w_u16(0x80058B50, x);
    w_u16(0x80058B52, y);
    w_u16(0x80058B5C, 0);
}

void gtend_text_string(uint32 ordering, sint16 x, sint16 y, uint32 text)
{
    sint32 character;
    FUNCTION_MARKER(0x8001325C, "GTEND.EXE");
    gtend_text_begin(ordering);
    gtend_text_position(x, y);
    character = r_s8(text);
    while (character) {
        ++text;
        gtend_text_character(character);
        character = r_s8(text);
    }
}

uint32 gtend_display_enable(void)
{
    FUNCTION_MARKER(0x80012D14, "GTEND.EXE");
    gtend_80019eb0(1);
    return gtend_display_current();
}

uint32 gtend_frame_clear(sint32 mode)
{
    FUNCTION_MARKER(0x80012D3C, "GTEND.EXE");
    gtend_ordering_clear(mode);
    return gtend_primitive_buffer_select(mode);
}

uint32 gtend_frame_swap(sint32 mode)
{
    uint32 alternate, current, result;
    FUNCTION_MARKER(0x80012D68, "GTEND.EXE");
    alternate = r_u32(0x80058C5C);
    current = r_u32(0x80058C58);
    w_u32(0x80058C58, alternate);
    w_u32(0x80058C5C, current);
    if (mode == 2) gtend_8001a22c(0);
    if (mode == 1 || mode == 2) gtend_8001440c();
    gtend_display_current();
    result = r_u32(0x80058C60);
    if (!result) return gtend_80012c10(1);
    return result;
}

uint32 gtend_primitive_buffer_select(uint32 alternate)
{
    uint32 slot;
    FUNCTION_MARKER(0x800143CC, "GTEND.EXE");
    slot = r_u32(alternate ? 0x80058C5C : 0x80058C58);
    return gtend_primitive_buffer_reset(slot);
}

uint32 gtend_primitive_buffer_reset(uint32 slot)
{
    uint32 primitive, capacity, end;
    FUNCTION_MARKER(0x8001573C, "GTEND.EXE");
    primitive = r_u32(0x80058A58 + slot * 4);
    capacity = r_u32(0x80058A48);
    w_u32(0x80058A60, slot);
    w_u32(0x80058A4C, primitive);
    end = primitive + capacity;
    w_u32(0x80058A50, end);
    return end;
}

uint32 gtend_80013c08(uint32 entry, uint32 scale, uint32 pan, sint32 priority)
{
    uint32 gain;
    FUNCTION_MARKER(0x80013C08, "GTEND.EXE");
    gain = r_u32(0x80025FF4);
    return gtend_sound_play_gain(entry, scale, pan, priority, gain);
}

uint32 gtend_sound_play_gain(uint32 entry, uint32 scale, uint32 pan, sint32 priority, uint32 gain)
{
    GTEndSoundParameters parameters;
    uint32 product;
    sint32 amplitude;
    FUNCTION_MARKER(0x80013C10, "GTEND.EXE");
    if (!gain) return 0;
    product = scale * gain;
    amplitude = (sint32)product / 0x4000;
    product = (uint32)amplitude * r_s16(entry + 2);
    amplitude = (sint32)product / 0x4000;
    gtend_sound_pan(&parameters, amplitude, pan);
    {
        uint32 pitch = r_u8(entry + 12) << 8;
        uint32 base = r_u16(entry + 4);
        parameters.pitch = gtend_sound_pitch(pitch, base);
    }
    parameters.field_0c = r_u16(entry + 6);
    parameters.field_08 = r_u32(entry + 16);
    parameters.field_06 = r_u16(entry);
    parameters.priority = priority < 0 ? r_u8(entry + 8) : priority;
    parameters.flags = r_u8(entry + 10);
    return gtend_sound_queue(0, &parameters);
}

uint32 gtend_sound_pan(GTEndSoundParameters *parameters, sint32 amplitude, uint32 pan)
{
    uint32 offset, table, product;
    FUNCTION_MARKER(0x80016D80, "GTEND.EXE");
    offset = pan * 2;
    table = r_u16(0x80024DDA - offset);
    product = table * (uint32)amplitude;
    parameters->left = (sint32)product >> 14;
    table = r_u16(0x80024CDC + offset);
    product = table * (uint32)amplitude;
    parameters->right = (sint32)product >> 14;
    return offset;
}

uint32 gtend_sound_pitch(uint32 pitch, uint32 base)
{
    sint32 remainder, octave = -2;
    uint32 value;
    FUNCTION_MARKER(0x80016DD0, "GTEND.EXE");
    remainder = (sint32)(pitch - base);
    if (remainder < 0) {
        do {
            remainder += 0xC00;
            --octave;
        } while (remainder < 0);
    } else if (remainder >= 0xC00) {
        do {
            remainder -= 0xC00;
            ++octave;
        } while (remainder >= 0xC00);
    }
    value = r_u16(0x800249DC + (remainder >> 3) * 2);
    if (octave > 0) value <<= octave & 31;
    else value >>= -octave & 31;
    return value & 0xFFFF;
}

uint32 gtend_sound_queue(uint32 handle, const GTEndSoundParameters *parameters)
{
    sint32 voice;
    uint32 record;
    FUNCTION_MARKER(0x80016E50, "GTEND.EXE");
    voice = gtend_voice_claim(parameters->priority);
    if (handle) w_u8(handle, voice);
    if (voice < 0) return voice;
    record = 0x80044C8C + voice * 40;
    w_u8(record + 8, 1);
    w_u32(record + 28, handle);
    gtend_voice_parameters(voice, parameters);
    return record;
}

void gtend_voice_parameters(uint32 voice, const GTEndSoundParameters *parameters)
{
    uint32 node, value, gain, volumes, pitch, data;
    uint8 flags;
    FUNCTION_MARKER(0x800172C0, "GTEND.EXE");
    node = 0x80044C94 + voice * 40;
    w_u8(node + 4, 2);
    value = parameters->field_0c;
    gain = r_u16(0x80044C92);
    w_u8(node + 1, 0);
    value *= gain;
    w_u32(node + 24, 0);
    w_u32(node + 32, 0);
    volumes = (uint16)parameters->left | ((uint32)(uint16)parameters->right << 16);
    pitch = parameters->pitch | ((uint32)parameters->field_06 << 16);
    data = parameters->field_08;
    flags = parameters->flags;
    w_u32(node + 8, volumes);
    w_u32(node + 12, pitch);
    w_u32(node + 16, data);
    w_u32(node + 28, value);
    w_u8(node + 36, flags & 1);
    w_u8(0x80044C8E, 1);
    w_u32(node + 4, 0x10700);
}

sint32 gtend_voice_claim(uint32 priority)
{
    sint32 voice, selected = -1;
    uint32 node, candidate = 0, attempt;
    FUNCTION_MARKER(0x80017160, "GTEND.EXE");
    voice = r_s8(0x80044C8C);
    node = 0x80044C94 + voice * 40;
    for (attempt = 0; attempt < 24; ++attempt) {
        uint32 reserved, state;
        node += 40;
        if (voice == 23) {
            node = 0x80044C94;
            voice = -1;
        }
        reserved = r_u8(node + 4);
        state = r_u8(node);
        ++voice;
        if (!reserved && !state) {
            w_u8(node + 4, 1);
            state = r_u8(node);
            if (!state) {
                w_u8(node + 3, priority);
                w_u8(0x80044C8C, voice);
                return voice;
            }
            w_u8(node + 4, 0);
        }
    }
    for (attempt = 0; attempt < 24; ++attempt) {
        uint32 reserved;
        node += 40;
        if (voice == 23) {
            node = 0x80044C94;
            voice = -1;
        }
        reserved = r_u8(node + 4);
        ++voice;
        if (!reserved) {
            uint32 state;
            w_u8(node + 4, 1);
            state = r_u32(node);
            if (!(state & 0xFF)) {
                w_u8(node + 3, priority);
                w_u8(0x80044C8C, voice);
                return voice;
            }
            if ((state & 0xFF) == 1 && (state >> 24) >= priority) {
                /* The original comparison branches all reach the same replacement */
                (void)r_u32(node + 24);
                if (!((state >> 16) & 0xFF)) (void)r_u8(node + 6);
                if (candidate) w_u8(candidate + 4, 0);
                selected = voice;
                candidate = node;
            } else {
                w_u8(node + 4, 0);
            }
        }
    }
    if (!candidate) return -1;
    {
        uint32 handle = r_u32(candidate + 20);
        if (handle) {
            w_u32(candidate + 20, 0);
            w_u8(handle, 0xFF);
        }
    }
    w_u32(candidate + 4, 0x01000001);
    w_u8(0x80044C8C, selected);
    return selected;
}

uint32 gtend_sound_rate(uint32 rate)
{
    sint32 scaled;
    FUNCTION_MARKER(0x800168BC, "GTEND.EXE");
    scaled = (sint32)(rate << 8);
    w_u16(0x80044C90, rate);
    w_u16(0x80044C92, scaled / 60);
    return 0x80044C8C;
}

void gtend_sound_tick_if_enabled(void)
{
    FUNCTION_MARKER(0x800168F0, "GTEND.EXE");
    if (r_u32(0x8004055C)) gtend_sound_voice_tick();
}

uint32 gtend_sound_voice_tick(void)
{
    uint32 update = 0, off = 0, on = 0, i, voice, bit;
    uint32 result = 0x80044C94;
    FUNCTION_MARKER(0x8001691C, "GTEND.EXE");
    w_u32(0x8004056C, r_u32(0x8004056C) + 1);
    for (i = 0, bit = 1; i < 24; ++i, bit <<= 1) {
        voice = 0x80044C94 + i * 40;
        if (r_u8(voice + 4) != 2) {
            uint32 finished = 0;
            if (r_u8(voice + 1) && !r_u8(voice + 6)) {
                if (!r_u16(0x1F801C0C + i * 16)) finished = 1;
                else {
                    uint32 length = r_u32(voice + 28);
                    uint32 elapsed = r_u32(voice + 32);
                    finished = length < elapsed;
                }
            }
            if (finished) {
                uint32 kind = r_u8(voice);
                w_u8(voice + 1, 0);
                w_u8(voice + 7, 1);
                if (kind == 1) {
                    uint32 handle = r_u32(voice + 20);
                    if (handle) w_u8(handle, 0xFF);
                    w_u32(voice + 20, 0);
                    w_u8(voice, 0);
                }
            }
            if (r_u8(voice + 7)) {
                if (r_u8(voice + 2)) {
                    off |= bit;
                    w_u8(voice + 2, 0);
                }
                w_u8(voice + 7, 0);
            }
            {
                uint32 dirty = r_u8(voice + 5);
                uint32 pending = r_u8(voice + 6);
                if (dirty) update |= bit;
                if (pending) {
                    on |= bit;
                    if (r_u8(voice + 2)) off |= bit;
                    w_u8(voice + 2, 1);
                    w_u8(voice + 1, 1);
                    w_u8(voice + 6, 0);
                }
            }
        }
        if (r_u8(voice + 1)) {
            uint32 ticks = r_u32(voice + 24);
            uint32 length = r_u32(voice + 28);
            w_u32(voice + 24, ticks + 1);
            if (length) {
                uint32 pitch = r_u16(0x1F801C04 + i * 16);
                uint32 elapsed = r_u32(voice + 32);
                w_u32(voice + 32, elapsed + pitch);
            }
        }
    }
    if (off) {
        w_u16(0x1F801D8C, off);
        w_u16(0x1F801D8E, off >> 16);
    }
    for (i = 0; update; ++i, update >>= 1) {
        voice = 0x80044C94 + i * 40;
        if (update & 1) {
            uint32 flags = r_u8(voice + 5);
            uint32 registers = 0x1F801C00 + i * 16;
            w_u8(voice + 5, 0);
            if (flags & 1) w_u16(registers + 4, r_u16(voice + 12));
            if (flags & 2) {
                uint32 left = r_u16(voice + 8) & 0x7FFF;
                uint32 right = r_u16(voice + 10) & 0x7FFF;
                w_u16(registers, left);
                w_u16(registers + 2, right);
            }
            if (flags & 4) {
                uint32 adsr1 = r_u16(voice + 16);
                uint32 adsr2 = r_u16(voice + 18);
                uint32 start = r_u16(voice + 14);
                w_u16(registers + 8, adsr1);
                w_u16(registers + 10, adsr2);
                w_u16(registers + 6, start);
            }
        }
    }
    if (on) {
        w_u16(0x1F801D88, on);
        w_u16(0x1F801D8A, on >> 16);
    }
    if (r_u8(0x80044C8E)) {
        uint32 mask = 0;
        for (i = 0, bit = 1; i < 24; ++i, bit <<= 1)
            if (r_u8(0x80044C94 + i * 40 + 36)) mask |= bit;
        w_u16(0x1F801D98, mask);
        w_u16(0x1F801D9A, mask >> 16);
        w_u8(0x80044C8E, 0);
        result += 24 * 40;
    }
    return result;
}

uint32 gtend_sound_instrument_relocate(uint32 image, uint32 spu_base)
{
    uint32 old_base, base, first, samples, pointers, count, index, offset;
    FUNCTION_MARKER(0x80016C40, "GTEND.EXE");
    if (r_u32(image) != 0x54534E49) return 0;
    old_base = r_u32(image + 4);
    first = r_u32(image + 20);
    base = image - old_base;
    samples = r_u32(image + 28);
    pointers = r_u32(image + 36);
    count = r_u32(image + 32);
    w_u32(image + 4, image);
    w_u32(image + 20, first + base);
    w_u32(image + 28, samples + base);
    w_u32(image + 36, pointers + base);
    index = 0;
    offset = 0;
    if (count) do {
        uint32 table = r_u32(image + 36);
        uint32 pointer, value;
        ++index;
        pointer = table + offset;
        value = r_u32(pointer);
        offset += 4;
        w_u32(pointer, value + base);
        count = r_u32(image + 32);
    } while (index < count);
    if (spu_base) {
        uint32 old_spu = r_u32(image + 12);
        count = r_u32(image + 24);
        w_u32(image + 12, spu_base);
        offset = 0;
        index = 0;
        base = (spu_base - old_spu) >> 3;
        if (count) do {
            uint32 table = r_u32(image + 28);
            uint32 sample, value;
            ++index;
            sample = table + offset;
            value = r_u16(sample);
            offset += 20;
            w_u16(sample, value + base);
            count = r_u32(image + 24);
        } while (index < count);
    }
    return image;
}

uint32 gtend_sound_silence(void)
{
    uint32 enabled, voice, node, result;
    FUNCTION_MARKER(0x80016EEC, "GTEND.EXE");
    enabled = r_u32(0x8004055C);
    w_u32(0x8004055C, 0);
    for (voice = 0, node = 0x80044C94; voice < 24; ++voice, node += 40) {
        uint32 handle = r_u32(node + 20);
        w_u8(node + 5, 2);
        w_u32(node + 8, 0);
        w_u8(node + 7, 1);
        w_u8(node + 6, 0);
        w_u8(node + 4, 0);
        w_u8(node, 0);
        if (handle) {
            w_u8(handle, 0xFF);
            w_u32(node + 20, 0);
        }
    }
    result = gtend_sound_voice_tick();
    w_u32(0x8004055C, enabled);
    return result;
}

uint32 gtend_sound_reverb_enable(uint8 enabled)
{
    uint32 control;
    FUNCTION_MARKER(0x80017130, "GTEND.EXE");
    if (!spu_mmio_read(NULL, 0x1F801DAA, 2, &control))
        gt_draft_failure("Native SPU reverb control read", 0x80017130);
    control = (control & 0xFF7F) | ((enabled & 1) << 7);
    if (!spu_mmio_write(NULL, 0x1F801DAA, 2, control, 3))
        gt_draft_failure("Native SPU reverb control write", 0x80017130);
    return control;
}

uint32 gtend_sound_reverb(uint8 preset, uint32 delay, uint32 feedback)
{
    uint32 row = 0x80040590 + preset * 64;
    uint32 table = 0x8004057C + preset * 2;
    uint32 i, control;
    FUNCTION_MARKER(0x80016F80, "GTEND.EXE");
    if (preset == 7 || preset == 8) {
        uint32 value = (delay << 13) / 127;
        uint32 half = value >> 1;
        uint32 a = r_u16(row), b = r_u16(row + 2);
        uint32 c = r_u16(row + 26), d = r_u16(row + 34);
        uint32 e = r_u16(row + 56), f = r_u16(row + 58);
        w_u16(row + 20, value - a);
        w_u16(row + 22, half - b);
        w_u16(row + 24, c + half);
        w_u16(row + 32, d + half);
        w_u16(row + 52, e + half);
        w_u16(row + 54, f + half);
    }
    if (preset == 7) {
        uint32 product = feedback * 33024;
        w_u16(row + 14, (sint32)product / 127);
    }
    control = r_u16(0x1F801DAA);
    w_u16(0x1F801D84, 0);
    w_u16(0x1F801D86, 0);
    w_u16(0x1F801DAA, control & 0xFF7F);
    w_u16(0x1F801DA2, r_u16(table));
    for (i = 0; i < 4; ++i) {
        uint32 a = r_u32(row + i * 16);
        uint32 b = r_u32(row + i * 16 + 4);
        uint32 c = r_u32(row + i * 16 + 8);
        uint32 d = r_u32(row + i * 16 + 12);
        uint32 registers = 0x1F801DC0 + i * 16;
        w_u16(registers, a);
        w_u16(registers + 2, a >> 16);
        w_u16(registers + 6, b >> 16);
        w_u16(registers + 10, c >> 16);
        w_u16(registers + 14, d >> 16);
        w_u16(registers + 4, b);
        w_u16(registers + 8, c);
        w_u16(registers + 12, d);
    }
    w_u16(0x1F801DAA, r_u16(0x1F801DAA) | 0x80);
    w_u8(0x80044C8D, 1);
    return (0x10000 - r_u16(table)) << 3;
}

void gtend_80016840(void)
{
    FUNCTION_MARKER(0x80016840, "GTEND.EXE");
    if (r_u32(0x8004055C)) return;
    gtend_80022640();
    w_u16(0x1F801D80, 0x3FFF);
    w_u16(0x1F801D82, 0x3FFF);
    gtend_8001640c(0);
    gtend_8001d504(0x80044C8C, 0, 968);
    w_u8(0x80044C8C, 23);
    gtend_sound_rate(60);
    w_u32(0x8004055C, 1);
}

uint32 gtend_80016be0(void)
{
    FUNCTION_MARKER(0x80016BE0, "GTEND.EXE");
    if (!r_u32(0x8004055C)) return 0x8004055C;
    w_u32(0x8004055C, 0);
    gtend_sound_silence();
    if (r_u8(0x80044C8D)) {
        gtend_sound_reverb(0, 0, 0);
        gtend_sound_reverb_enable(0);
    }
    return gtend_80022660();
}

uint32 gtend_sound_sample_upload(uint32 spu_address, uint32 source, uint32 size, uint32 relocation_result)
{
    FUNCTION_MARKER(0x80016D30, "GTEND.EXE");
    if (!size) return relocation_result;
    gtend_800163f4(spu_address);
    gtend_8001640c(0);
    gtend_80021940(source, size);
    return SpuIsTransferCompleted(1);
}

void gtend_80015dd0(void)
{
    FUNCTION_MARKER(0x80015DD0, "GTEND.EXE");
    gtend_8001d504(0x80059540, 0, 96);
    PadInitDirectPSX(0x800595A0, 0x800595E0);
    PadStartCom();
}

uint32 gtend_controller_ready(uint32 port)
{
    sint32 state;
    FUNCTION_MARKER(0x80015F28, "GTEND.EXE");
    port = (port ^ (port & 4)) | ((port & 4) << 2);
    state = PadGetState((sint32)port);
    return state == 0 || state == 2 || state == 6;
}

void gtend_80015f8c(void)
{
    uint32 port;
    FUNCTION_MARKER(0x80015F8C, "GTEND.EXE");
    do {
        VSync(0);
        for (port = 0; port < 8; ++port)
            if (!gtend_controller_ready(port)) break;
    } while (port != 8);
    PadStopCom();
}

uint32 gtend_controller_port_update(uint32 state, uint32 port, uint32 slot)
{
    uint32 present, entry, request;
    uint8 sdk_state;
    sint32 actuators;
    FUNCTION_MARKER(0x800165C4, "GTEND.EXE");
    present = 1u << (slot & 31);
    entry = state + slot * 8 + 16;
    sdk_state = PadGetState((sint32)port);
    w_u8(entry, sdk_state);
    actuators = r_u8(entry + 1);
    if (sdk_state == 2) actuators = 1;
    else if (!sdk_state) {
        present = 0;
        actuators = 0;
    } else if (sdk_state == 6) {
        actuators = PadInfoAct((sint32)port, -1, 0);
    }
    request = r_u8(entry + 2);
    w_u8(entry + 1, actuators);
    if (!request) return present;
    {
        uint32 small = r_u8(entry + 3), large = r_u8(entry + 4);
        if (actuators == 1) {
            if (!small) small = large >= 97;
            large = small;
            small = 64;
        }
        w_u8(entry + 5, small);
        w_u8(entry + 6, large);
        PadSetAct((sint32)port, psx_addr(entry + 5, 2), 2);
    }
    if (sdk_state == 0 || sdk_state == 2)
        w_u8(entry + 2, 0);
    else if (sdk_state == 6 && PadSetActAlign((sint32)port, psx_addr(0x80040554, 6)))
        w_u8(entry + 2, 0);
    return present;
}

uint32 gtend_controller_scan(uint32 state)
{
    uint32 physical, present = 0;
    FUNCTION_MARKER(0x80016720, "GTEND.EXE");
    for (physical = 0; physical < 2; ++physical) {
        uint32 packet = 0x80059620 + physical * 64;
        uint32 port = physical * 16, slot = physical * 4;
        w_u8(state + 16 + physical * 8, 0);
        if (r_u8(packet)) continue;
        present |= 1 << (physical + 8);
        if ((r_u8(packet + 1) & 0xF0) == 0x80) {
            uint32 tap;
            present |= 1 << (physical + 10);
            for (tap = 0; tap < 4; ++tap)
                present |= gtend_controller_port_update(state, port + tap, slot + tap);
        } else {
            present |= gtend_controller_port_update(state, port, slot);
        }
    }
    return present;
}

uint32 gtend_80016200(void)
{
    uint32 present, previous, changed, slot;
    FUNCTION_MARKER(0x80016200, "GTEND.EXE");
    gtend_memory_copy(0x80059620, 0x800595A0, 128);
    gtend_8001d504(0x80059590, 0, 16);
    present = gtend_controller_scan(0x80059540);
    previous = r_u32(0x80059540);
    w_u32(0x80059540, present);
    changed = previous ^ present;
    w_u32(0x80059544, changed);
    for (slot = 0; slot < 8; ++slot) {
        uint32 type = 0;
        if (present & (1 << slot)) {
            uint32 packet = gtend_controller_packet(slot);
            uint32 count_address;
            type = r_u8(packet + 1);
            count_address = 0x80059590 + (type >> 4);
            w_u8(count_address, r_u8(count_address) + 1);
        }
        w_u8(0x80059548 + slot, type);
    }
    return changed;
}

uint32 gtend_8001d370(void)
{
    uint32 block, largest = 16;
    FUNCTION_MARKER(0x8001D370, "GTEND.EXE");
    block = r_u32(0x80040A44);
    while (block) {
        if (!(r_u32(block + 12) & 1)) {
            uint32 size = r_u32(block + 8);
            if (largest < size) largest = size;
        }
        block = r_u32(block + 4);
    }
    return largest - 16;
}

uint32 gtend_8001d3c8(uint32 bytes)
{
    uint32 block, best = 0, size, flags;
    FUNCTION_MARKER(0x8001D3C8, "GTEND.EXE");
    size = (bytes + 15) & ~15;
    block = r_u32(0x80040A44);
    while (block) {
        if (!(r_u32(block + 12) & 1)) {
            uint32 available = r_u32(block + 8);
            if (available >= size && (!best || available < r_u32(best + 8))) best = block;
        }
        block = r_u32(block + 4);
    }
    if (!best) return 0;
    gt_heap_block_split(best, size);
    flags = r_u32(best + 12);
    w_u32(best + 12, flags | 1);
    return best + 16;
}

uint32 gtend_8001d478(uint32 allocation)
{
    uint32 block, result, previous;
    FUNCTION_MARKER(0x8001D478, "GTEND.EXE");
    block = allocation - 16;
    result = r_u32(allocation - 4);
    previous = r_u32(allocation - 16);
    result &= ~1;
    w_u32(allocation - 4, result);
    while (previous) {
        result = r_u32(previous + 12) & 1;
        if (result) break;
        block = previous;
        previous = r_u32(previous);
    }
    for (;;) {
        uint32 next = r_u32(block + 4);
        if (!next) return result;
        result = r_u32(next + 12) & 1;
        if (result) return result;
        result = gt_heap_block_merge_next(block, result);
    }
}

uint32 gt_heap_block_split(uint32 block, uint32 size)
{
    uint32 available, split, previous, next, flags, result;
    /* Original occurrences
     * GTEND.EXE:0x8001E78C
     * GTMAIN.EXE:0x8007D534
     * GTMENU.EXE:0x80045D90
     * GTOS.EXE:0x8004232C
     * SCUS_941.94:0x80016900
     */
    available = r_u32(block + 8);
    if (available < size + 32) return 1;
    split = block + size + 16;
    previous = r_u32(block);
    next = r_u32(block + 4);
    available = r_u32(block + 8);
    flags = r_u32(block + 12);
    w_u32(split, previous);
    w_u32(split + 4, next);
    w_u32(split + 8, available);
    w_u32(split + 12, flags);
    next = r_u32(block + 4);
    w_u32(block + 4, split);
    w_u32(split, block);
    w_u32(block + 8, size);
    w_u32(split + 8, r_u32(split + 8) - 16 - size);
    result = r_u32(split + 12) & ~1;
    w_u32(split + 12, result);
    if (next) w_u32(next, split);
    return result;
}

uint32 gt_heap_block_merge_next(uint32 block, uint32 incoming_result)
{
    uint32 next, following, left, right, result = incoming_result;
    /* Original occurrences
     * GTEND.EXE:0x8001E810
     * GTMAIN.EXE:0x8007D5B8
     * GTMENU.EXE:0x80045D4C
     * GTOS.EXE:0x800423D0
     */
    next = r_u32(block + 4);
    if (next) {
        following = r_u32(next + 4);
        if (following) w_u32(following, block);
        w_u32(block + 4, following);
        left = r_u32(block + 8);
        right = r_u32(next + 8);
        result = left + 16 + right;
        w_u32(block + 8, result);
    }
    return result;
}

uint32 gtend_heap_initialize(uint32 start, uint32 size)
{
    uint32 usable, block, span, flags;
    FUNCTION_MARKER(0x8001D290, "GTEND.EXE");
    w_u32(0x80040A44, start);
    w_u32(0x80040A40, size);
    usable = size < 16 ? 0 : size - 16;
    block = r_u32(0x80040A44);
    span = r_u32(0x80040A40);
    gtend_8001d504(block, 0, span);
    block = r_u32(0x80040A44);
    flags = r_u32(block + 12);
    w_u32(block, 0);
    w_u32(block + 4, 0);
    w_u32(block + 8, usable);
    w_u32(block + 12, flags & ~1);
    return block;
}
