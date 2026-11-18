#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include <stdio.h>


uint32 gtmain_menu_grouped_decimal(sint32 value, uint32 destination)
{
    uint8 reverse[32];
    uint32 count = 0, group = 0, iteration, result;
    sint32 negative;
    FUNCTION_MARKER(0x8003434C, "GTMAIN.EXE");
    negative = value < 0;
    if (negative)
        value = 0 - (uint32)value;
    if (!value) {
        w_u8(destination, 48);
        w_u8(destination + 1, 0);
        return 48;
    }
    for (iteration = 0; iteration < 16; ++iteration) {
        sint32 quotient = value / 10;
        uint32 digit = (uint32)value + 48 - (uint32)quotient * 10;
        reverse[count++] = digit;
        value = quotient;
        if (!value)
            break;
        if (++group >= 3) {
            group = 0;
            reverse[count++] = 44;
        }
    }
    if (negative)
        w_u8(destination++, 45);
    result = 45;
    while (count) {
        result = reverse[--count];
        w_u8(destination++, result);
    }
    w_u8(destination, 0);
    return result;
}

sint32 gtmain_menu_fixed_text_draw(uint32 ordering, sint32 x, sint32 y, uint32 text, sint32 font)
{
    uint32 digit_step, separator_step, sign_step, character, position = x;
    sint32 result, width;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80036068, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x40, 0x80036068);
    w_u8(frame.address + 0x11, 0);
    if (font == 2) {
        digit_step = 8;
        separator_step = 4;
        sign_step = 8;
        result = 2;
    } else if (font == 1) {
        digit_step = 7;
        separator_step = 3;
        sign_step = 7;
        result = 1;
    } else {
        digit_step = 13;
        separator_step = 5;
        sign_step = 9;
        result = 1;
    }
    while ((character = r_u8(text++)) != 0) {
        uint32 draw_x;
        w_u8(frame.address + 0x10, character);
        width = gtmain_text_width(font, frame.address + 0x10);
        draw_x = position;
        if (character - 48 < 10 || character == 45) {
            uint32 sum = digit_step + (uint32)width;
            position += character == 45 ? sign_step : digit_step;
            draw_x = position - (uint32)((sint32)sum >> 1);
        } else {
            position += separator_step;
        }
        result = gtmain_font_text_draw(ordering, draw_x, y, frame.address + 0x10);
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_menu_fixed_text_width(sint32 font, uint32 text)
{
    uint32 digit_step, separator_step, sign_step, character, width = 0;
    FUNCTION_MARKER(0x80035FC0, "GTMAIN.EXE");
    if (font == 2) {
        digit_step = 8; separator_step = 4; sign_step = 8;
    } else if (font == 1) {
        digit_step = 7; separator_step = 3; sign_step = 7;
    } else {
        digit_step = 13; separator_step = 5; sign_step = 9;
    }
    while ((character = r_u8(text++)) != 0) {
        if (character - 48 < 10)
            width += digit_step;
        else if (character == 45)
            width += sign_step;
        else
            width += separator_step;
    }
    return width;
}

uint32 gtmain_menu_reward_amount_draw(uint32 ordering, sint32 x, sint32 y, sint32 palette,
    sint32 intensity, uint32 amount, sint32 span, sint32 row_y, sint32 elapsed)
{
    uint32 width, label_width, half, number_x, label_x, baseline, result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800346F0, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x68, 0x800346F0);
    gtmain_text_font(3);
    gtmain_menu_grouped_decimal(amount, frame.address + 0x20);
    width = gtmain_menu_fixed_text_width(3, frame.address + 0x20);
    label_width = gtmain_text_width(3, 0x800BCD36);
    half = (sint32)((uint32)span + label_width + 8) >> 1;
    number_x = (uint32)x + half + 160 - width;
    baseline = (uint32)y + (uint32)row_y;
    gtmain_menu_reward_title_draw(ordering, number_x, baseline,
        frame.address + 0x20, -1, intensity, elapsed);
    label_x = (uint32)x - (half - 160);
    result = gtmain_menu_reward_title_draw(ordering, label_x, baseline,
        0x800BCD36, palette, intensity, elapsed);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_menu_reward_initialize(void)
{
    uint32 asset, total, amount, difference;
    sint32 width;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80034A7C, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x38, 0x80034A7C);
    w_u16(0x801D3022, 0);
    w_u32(0x80091A10, 0x800BCCE9);
    asset = r_s8(0x801D2FDE) ? 21 : 20;
    if (r_u8(0x801D2FDC) & 1) {
        w_u32(0x80091A10, 0x800BCD39);
        asset = 19;
    }
    gtmain_menu_asset_activate(asset);
    gtmain_menu_grouped_decimal(r_u32(0x801D2FE0), frame.address + 0x10);
    width = gtmain_menu_fixed_text_width(3, frame.address + 0x10);
    w_u16(0x801D2FEC, width);
    gtmain_menu_grouped_decimal(r_u32(0x801D2FE4), frame.address + 0x10);
    width = gtmain_menu_fixed_text_width(3, frame.address + 0x10);
    total = r_u32(0x801D2FE4);
    amount = r_u32(0x801D2FE0);
    w_u16(0x801D2FEE, width);
    difference = total - amount;
    w_u32(0x801D2FE8, difference);
    gtmain_menu_grouped_decimal(difference, frame.address + 0x10);
    width = gtmain_menu_fixed_text_width(3, frame.address + 0x10);
    if (r_s16(0x801D2FEE) < width)
        w_u16(0x801D2FEE, width);
    w_u32(0x801D2FF0, 1);
    w_u16(0x801D2FF4, 0);
    gt_local_stack_pop(frame);
    return 0;
}

uint32 gtmain_menu_prize_draw(uint32 ordering, sint32 x, sint32 y, uint32 unused, sint32 intensity)
{
    sint32 phase, elapsed, palette, width, selection;
    uint32 center;
    FUNCTION_MARKER(0x80034CD0, "GTMAIN.EXE");
    gtmain_text_font(3);
    gtmain_text_flags(2);
    elapsed = r_s16(0x801D3004) - 30;
    selection = r_s16(0x801D3020);
    palette = r_s8(0x80091B28 + (uint32)selection);
    if (elapsed >= 0) {
        width = gtmain_text_width(3, 0x800BCB8C);
        center = (uint32)x - ((uint32)(width >> 1) - 160);
        gtmain_menu_reward_title_draw(ordering, center, (uint32)y + 112,
            0x800BCB8C, palette, intensity, elapsed);
    }
    phase = r_s16(0x801D3004);
    elapsed = phase - 42;
    if (elapsed >= 0) {
        width = gtmain_text_width(3, 0x800BCB9A);
        center = (uint32)x - ((uint32)(width >> 1) - 160);
        gtmain_menu_reward_title_draw(ordering, center, (uint32)y + 136,
            0x800BCB9A, palette, intensity, elapsed);
        phase = r_s16(0x801D3004);
    }
    if (phase >= 90) {
        gtmain_text_font(2);
        gtmain_text_color(gtmain_menu_pulse_color(intensity >> 1));
        width = gtmain_text_width(2, 0x800BCC0C);
        center = (uint32)x - ((uint32)(width >> 1) - 160);
        gtmain_font_text_draw(ordering, center, (uint32)y + 204, 0x800BCC0C);
    }
    gtmain_menu_pose_model_draw(0x801D3008, ordering, (uint32)x + 160, (uint32)y + 106,
        0x15F90, 0x80184FDC, 8, 36, 8, 101, 0x21C, 192, 96);
    gtmain_menu_gradient_draw(ordering + 16000, x, y, 320, 240, intensity, 0, 64, 128, 64, 32, 32);
    return 0;
}

uint32 gtmain_menu_prize_initialize(void)
{
    uint32 offset, first;
    FUNCTION_MARKER(0x80034F08, "GTMAIN.EXE");
    w_u16(0x801D3004, 0);
    gtmain_menu_pose_initialize(0x801D3008);
    offset = r_u16(0x801D300C) + 128;
    first = r_u16(0x801D3008);
    w_u16(0x801D3008, first);
    w_u16(0x801D300C, offset);
    return 0;
}

uint32 gtmain_menu_pose_initialize(uint32 pose)
{
    FUNCTION_MARKER(0x8003D1A8, "GTMAIN.EXE");
    w_u16(pose, 0x4C0);
    w_u16(pose + 2, 0x6A0);
    w_u32(pose + 12, 0x2EE0);
    w_u16(pose + 4, 0);
    w_u32(pose + 8, 0);
    w_u32(pose + 16, 0xFFF2D100);
    w_u16(pose + 20, 120);
    return 120;
}

uint32 gtmain_menu_gradient_draw(uint32 ordering, sint32 x, sint32 y, sint32 width, sint32 height,
    sint32 intensity, uint8 r0, uint8 g0, uint8 b0, uint8 r1, uint8 g1, uint8 b1)
{
    uint32 colors[6], right, bottom, first, second, result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003B784, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x38, 0x8003B784);
    colors[0] = r0 * (uint32)intensity;
    colors[1] = g0 * (uint32)intensity;
    colors[2] = b0 * (uint32)intensity;
    colors[3] = r1 * (uint32)intensity;
    colors[4] = g1 * (uint32)intensity;
    right = (uint32)x + (uint32)width;
    w_u16(frame.address + 0x12, y);
    w_u16(frame.address + 0x16, y);
    bottom = (uint32)y + (uint32)height;
    w_u16(frame.address + 0x10, x);
    w_u16(frame.address + 0x18, x);
    w_u16(frame.address + 0x1A, bottom);
    w_u16(frame.address + 0x1E, bottom);
    colors[5] = b1 * (uint32)intensity;
    w_u8(frame.address + 0x23, 0x38);
    w_u8(frame.address + 0x20, (sint32)colors[0] >> 8);
    w_u8(frame.address + 0x21, (sint32)colors[1] >> 8);
    w_u8(frame.address + 0x22, (sint32)colors[2] >> 8);
    w_u8(frame.address + 0x28, (sint32)colors[3] >> 8);
    w_u8(frame.address + 0x29, (sint32)colors[4] >> 8);
    w_u8(frame.address + 0x2A, (sint32)colors[5] >> 8);
    first = r_u32(frame.address + 0x20);
    second = r_u32(frame.address + 0x28);
    w_u16(frame.address + 0x14, right);
    w_u16(frame.address + 0x1C, right);
    w_u32(frame.address + 0x24, first);
    w_u32(frame.address + 0x2C, second);
    result = gtmain_packet_gouraud_quad(ordering, frame.address + 0x10, frame.address + 0x20);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_packet_gouraud_quad(uint32 ordering, uint32 coordinates, uint32 colors)
{
    uint32 packet, value, previous, address, lane, byte;
    FUNCTION_MARKER(0x8003B6D8, "GTMAIN.EXE");
    packet = r_u32(0x800BFE4C);
    w_u32(packet, 0x08000000);
    value = r_u32(coordinates);
    w_u32(packet + 8, value);
    value = r_u32(colors);
    w_u32(packet + 4, value);
    value = r_u32(coordinates + 4);
    w_u32(packet + 16, value);
    value = r_u32(colors + 4);
    w_u32(packet + 12, value);
    value = r_u32(coordinates + 8);
    w_u32(packet + 24, value);
    value = r_u32(colors + 8);
    w_u32(packet + 20, value);
    value = r_u32(coordinates + 12);
    w_u32(packet + 32, value);
    value = r_u32(colors + 12);
    w_u32(packet + 28, value);
    previous = ordering + 2;
    lane = previous & 3;
    address = previous & ~3;
    for (byte = 0; byte <= lane; ++byte) {
        uint32 shift = (3 - lane + byte) * 8;
        value = r_u8(address + byte);
        previous = (previous & ~((uint32)255 << shift)) | (value << shift);
    }
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, (packet << 8) >> ((3 - lane + byte) * 8));
    address = packet + 2;
    lane = address & 3;
    address &= ~3;
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, previous >> ((3 - lane + byte) * 8));
    packet += 36;
    w_u32(0x800BFE4C, packet);
    return packet;
}

uint32 gtmain_80034f5c(uint32 mode, sint32 rank, uint32 amount, uint32 balance, uint32 flags)
{
    uint32 path, table;
    sint32 palettes, index;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80034F5C, "GTMAIN.EXE");
    printf((const char *)psx_addr(0x8008CB20, 19));
    if (!mode && !amount)
        return 1;
    frame = gt_local_stack_push(0x50, 0x80034F5C);
    if (flags & 1) {
        gtmain_stream_mode_set(128);
        path = gtmain_environment_value(0x8008CB34);
        table = gtmain_archive_table_load_guest(path, frame.address + 0x18, 0x801D2FDC, 4096);
        path = gtmain_environment_value(0x8008CB34);
        gtmain_archive_section_load(path, (const uint8 *)psx_addr(table, 4096),
            r_u32(frame.address + 0x18), 6, 0x80184FDC, 0);
        path = gtmain_environment_value(0x8008CB34);
        gtmain_archive_section_load(path, (const uint8 *)psx_addr(table, 4096),
            r_u32(frame.address + 0x18), 7, 0x801D6FDC, 0);
        gtmain_texture_page_upload(0x801D6FDC, 8);
        palettes = r_s16(0x801D6FEA);
        for (index = 0; index < palettes; ++index)
            gtmain_texture_palette_upload(0x801D6FDC, ((uint32)index << 8) | 0x24, index);
        gtmain_menu_model_bundle_relocate(0x80184FDC, 8);
    }
    w_u8(0x801D2FDD, mode);
    w_u8(0x801D2FDE, rank);
    w_u32(0x801D2FE0, amount);
    w_u32(0x801D2FE4, balance);
    w_u8(0x801D2FDC, flags);
    gtmain_menu_draw_callback_set(0x80091A0C);
    gtmain_menu_panel_special_bind(0x800BDD10, 0x80092154);
    gtmain_music_sector_init();
    gtmain_stream_stop_wait(1);
    gtmain_menu_frame_run();
    {
        uint32 result = gtmain_stream_stop_wait(1);
        gt_local_stack_pop(frame);
        return result;
    }
}

uint32 gtmain_event_competitor_complete(sint32 competitor)
{
    uint32 cursor, end;
    FUNCTION_MARKER(0x80035138, "GTMAIN.EXE");
    if (competitor < 4)
        return 1;
    cursor = 0x800CAE3C + ((uint32)competitor << 2);
    end = cursor + 0x8EF;
    cursor += 0x8EC;
    for (;;) {
        if (!r_u8(cursor))
            return 0;
        ++cursor;
        if ((sint32)cursor >= (sint32)end)
            return 1;
    }
}

uint32 gtmain_event_special_complete_count(void)
{
    uint32 index, total = 0;
    FUNCTION_MARKER(0x8003518C, "GTMAIN.EXE");
    for (index = 0; index < 4; ++index)
        total += gtmain_event_competitor_complete(index + 8);
    return total;
}

uint32 gtmain_event_all_medals_two(void)
{
    uint32 row, column;
    FUNCTION_MARKER(0x800351D4, "GTMAIN.EXE");
    for (row = 0; row < 8; ++row) {
        for (column = 0; column < 3; ++column) {
            if (r_u8(0x800CB738 + row * 4 + column) < 2)
                return 0;
        }
    }
    return 1;
}

sint32 gtmain_menu_stack_pop(void)
{
    uint32 context;
    sint32 count;
    FUNCTION_MARKER(0x8003532C, "GTMAIN.EXE");
    context = r_u32(0x800BDD08);
    count = r_s8(context + 256);
    if (count <= 0)
        return -1;
    w_u8(context + 256, count - 1);
    return 0;
}

uint32 gtmain_menu_stack_replace(uint32 descriptor)
{
    uint32 context, destination;
    sint32 index;
    FUNCTION_MARKER(0x80035364, "GTMAIN.EXE");
    context = r_u32(0x800BDD08);
    index = r_s8(context + 256);
    destination = context + ((uint32)index << 2);
    w_u32(destination + 192, descriptor);
    return destination;
}

uint32 gtmain_menu_font_prepare(void)
{
    FUNCTION_MARKER(0x80035DC0, "GTMAIN.EXE");
    gtmain_menu_font_context_initialize(0x801F3FDC);
    w_u32(0x801F3FE8, 0x801F400C);
    w_u32(0x801F3FE0, 0x801F400C);
    w_u32(0x801F3FDC, 0x801F480C);
    w_u16(0x801F3FF4, 0x114);
    w_u16(0x801F3FF6, 0x220);
    gtmain_menu_font_context_bind(0x801F3FDC, 0x80091B44);
    w_u32(0x801F4000, 0x02C0C0C0);
    return 0x02C0C0C0;
}

uint32 gtmain_menu_font_context_initialize(uint32 context)
{
    FUNCTION_MARKER(0x80070F9C, "GTMAIN.EXE");
    sub_8007BD58(context, 0, 48);
    w_u32(context + 36, 0x808080);
    w_u32(context + 40, 15);
    return 15;
}

uint32 gtmain_menu_font_context_bind(uint32 context, uint32 rectangle)
{
    uint32 x, y, width, height, flags, expanded;
    FUNCTION_MARKER(0x80070FDC, "GTMAIN.EXE");
    x = r_u16(rectangle);
    y = r_u16(rectangle + 2);
    width = r_u16(rectangle + 4);
    height = r_u16(rectangle + 6);
    flags = r_u32(context + 24);
    w_u16(context + 28, 0);
    w_u16(context + 30, 0);
    flags = (flags & 0xFFE0FFFF) | (((x >> 6) & 15) << 16) | (((y >> 8) & 1) << 20);
    width &= 0xFFFE;
    expanded = width << 2;
    w_u16(context + 20, width);
    w_u16(context + 16, x);
    w_u16(context + 18, y);
    w_u16(context + 22, height);
    w_u16(context + 32, expanded);
    w_u16(context + 34, height);
    w_u32(context + 24, flags);
    w_u16(context + 44, (sint16)width >> 1);
    return expanded;
}

uint32 gtmain_menu_caption_plain_draw(uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    FUNCTION_MARKER(0x80035E40, "GTMAIN.EXE");
    if (r_s8(0x80091B39) != 1)
        return 1;
    return gtmain_menu_text_draw(ordering, x, (uint32)y + 2, text, 0);
}

uint32 gtmain_menu_caption_mixed_draw(uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    uint32 character, position = x, vertical = (uint32)y << 16;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80035E74, "GTMAIN.EXE");
    if (r_s8(0x80091B39) != 1)
        return 1;
    frame = gt_local_stack_push(0x40, 0x80035E74);
    w_u8(frame.address + 0x19, 0);
    character = r_u8(text);
    if (!character) {
        gt_local_stack_pop(frame);
        return 1;
    }
    do {
        if ((((character ^ 0xA0) - 0x20) & 255) < 64) {
            uint32 xy = (position & 0xFFFF) | vertical;
            uint32 low;
            position += 12;
            low = r_u8(text + 1);
            text += 2;
            gtmain_menu_cached_glyph_draw(0x801F3FDC, (character << 8) | low,
                ordering, xy, 0x000C000C);
        } else {
            w_u8(frame.address + 0x18, character);
            gtmain_menu_text_draw(ordering, position, (uint32)y + 2, frame.address + 0x18, 0);
            position += 8;
            ++text;
        }
        character = r_u8(text);
    } while (character);
    gt_local_stack_pop(frame);
    return 0xA0;
}

uint32 gtmain_event_all_medals_four(void)
{
    uint32 row, column;
    FUNCTION_MARKER(0x80035228, "GTMAIN.EXE");
    for (row = 0; row < 8; ++row) {
        for (column = 0; column < 3; ++column) {
            if (r_u8(0x800CB738 + row * 4 + column) < 4)
                return 0;
        }
    }
    return 1;
}

uint32 gtmain_font_cache_acquire(uint32 context, uint16 glyph, uint32 x_out, uint32 y_out)
{
    uint32 node, parent = 0, branch = 0, mask = 1, page_x, page_y, allocated, base, product;
    sint32 row, column, width, height, next_column, next_row, pitch;
    FUNCTION_MARKER(0x800792E8, "GTMAIN.EXE");
    node = r_u32(context + 8);
    while (node) {
        if (r_u16(node + 12) == glyph) {
            w_u32(x_out, r_u16(node + 8));
            w_u32(y_out, r_u16(node + 10));
            return 0;
        }
        branch = glyph & mask;
        parent = node;
        node = r_u32(node + ((branch & 0xFFFF) ? 4 : 0));
        mask <<= 1;
    }
    page_x = r_s16(context + 16);
    row = r_s16(context + 30);
    column = r_s16(context + 28);
    width = r_s16(context + 32);
    height = r_s16(context + 34);
    page_y = r_u8(context + 18);
    page_x = (page_x & 63) << 2;
    next_column = column + 16;
    next_row = row;
    if (width < next_column || height < row + 16) {
        w_u32(x_out, page_x);
        w_u32(y_out, page_y);
        return 0;
    }
    if (next_column >= width) {
        next_column = 0;
        next_row += 16;
    }
    w_u16(context + 28, next_column);
    w_u16(context + 30, next_row);
    w_u32(x_out, (uint32)column + page_x);
    w_u32(y_out, (uint32)row + page_y);
    allocated = r_u32(context + 12);
    w_u32(context + 12, allocated + 16);
    if (!parent)
        w_u32(context + 8, allocated);
    else
        w_u32(parent + ((branch & 0xFFFF) ? 4 : 0), allocated);
    w_u16(allocated + 8, (uint32)column + page_x);
    w_u32(allocated, 0);
    w_u32(allocated + 4, 0);
    w_u16(allocated + 12, glyph);
    w_u16(allocated + 10, (uint32)row + page_y);
    pitch = r_s16(context + 44);
    product = (uint32)row * (uint32)pitch;
    base = r_u32(context);
    return (uint32)(column >> 1) + base + (product << 2);
}

uint32 gtmain_font_glyph_unpack(uint32 destination, const uint8 *rom_glyph30, sint32 pitch)
{
    uint32 previous_left = 0, previous_right = 0, weight = 2, stride, row, result = 0;
    FUNCTION_MARKER(0x8007944C, "GTMAIN.EXE");
    stride = pitch < 0 ? 0 - (uint32)pitch : (uint32)pitch;
    stride <<= 2;
    for (row = 0; row < 16; ++row) {
        uint32 bits, first, second, third, fourth, left, right;
        /* The original discards the sixteenth ROM row */
        bits = row == 15 ? 0 : xport_load_le16(rom_glyph30 + row * 2);
        first = r_u16(0x800AD970 + ((bits & 15) << 1));
        second = r_u16(0x800AD970 + ((bits >> 3) & 30));
        third = r_u16(0x800AD970 + ((bits >> 7) & 30));
        fourth = r_u16(0x800AD970 + ((bits >> 12) << 1));
        left = (first << 15) | second;
        result = third << 15;
        right = result | fourth;
        if (weight) {
            uint32 mixed[2], index;
            mixed[0] = previous_left * weight + left * (4 - weight);
            mixed[1] = previous_right * weight + right * (4 - weight);
            for (index = 0; index < 6; ++index) {
                uint32 shift = (index % 3) * 10;
                uint32 value = mixed[index / 3];
                uint32 low = (value >> shift) & 31;
                uint32 high = (value >> (shift + 1)) & 0x1F0;
                if (low == 16)
                    low = 15;
                if (high == 256)
                    high = 240;
                result = low | high;
                w_u8(destination + index, result);
            }
            destination += stride;
        }
        weight = (weight - 1) & 3;
        previous_left = left;
        previous_right = right;
    }
    return result;
}

uint32 gtmain_menu_cached_glyph_draw(uint32 context, uint16 glyph, uint32 ordering, uint32 xy, uint32 wh)
{
    uint32 pixels, tex_x, tex_y, page, color, uv, packet, page_y;
    uint32 address, lane, byte, previous, x, y, right, bottom, offset;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80071064, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x38, 0x80071064);
    pixels = gtmain_font_cache_acquire(context, glyph, frame.address + 0x10, frame.address + 0x14);
    if (pixels) {
        const uint8 *bitmap = Krom2RawAdd2(glyph);
        sint32 pitch = r_s16(context + 44);
        gtmain_font_glyph_unpack(pixels, bitmap, pitch);
    }
    x = (sint16)xy;
    right = x + (uint32)(sint16)wh;
    y = xy >> 16;
    bottom = y + (wh >> 16);
    tex_y = r_u32(frame.address + 0x14);
    page = r_u32(context + 24);
    tex_x = r_u32(frame.address + 0x10);
    color = r_u32(context + 36);
    uv = ((tex_y & 255) << 8) | (tex_x & 255);
    page += (tex_x & 0xF00) << 8;
    page_y = (tex_y & 0x100) << 12;
    packet = r_u32(0x800BFE4C);
    page += page_y;
    w_u32(0x800BFE4C, packet + 40);
    color |= 0x2C000000;
    previous = page_y;
    address = ordering + 2;
    lane = address & 3;
    address &= ~3;
    for (byte = 0; byte <= lane; ++byte) {
        uint32 shift = (3 - lane + byte) * 8;
        uint32 value = r_u8(address + byte);
        previous = (previous & ~((uint32)255 << shift)) | (value << shift);
    }
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, (packet << 8) >> ((3 - lane + byte) * 8));
    address = packet + 2;
    lane = address & 3;
    address &= ~3;
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, previous >> ((3 - lane + byte) * 8));
    w_u16(packet + 14, page);
    page >>= 16;
    w_u8(packet + 3, 9);
    w_u32(packet + 4, color);
    w_u16(packet + 22, page);
    offset = r_u16(context + 46);
    w_u16(packet + 10, y);
    w_u16(packet + 18, y);
    w_u16(packet + 20, uv + 12);
    w_u16(packet + 12, uv);
    w_u16(packet + 16, right);
    w_u16(packet + 24, x);
    w_u16(packet + 26, bottom);
    w_u16(packet + 32, right);
    w_u16(packet + 34, bottom);
    w_u16(packet + 28, uv + 0xC00);
    w_u16(packet + 36, uv + 0xC0C);
    w_u16(packet + 8, x + offset);
    gt_local_stack_pop(frame);
    return packet;
}

uint32 gtmain_menu_caption_width(uint32 text)
{
    FUNCTION_MARKER(0x80035F7C, "GTMAIN.EXE");
    return gtmain_menu_text_width(2, text, 0);
}

uint32 gtmain_menu_caption_enable(void)
{
    FUNCTION_MARKER(0x80035FA4, "GTMAIN.EXE");
    w_u8(0x80091B39, 1);
    return 1;
}

uint32 gtmain_menu_option_selector_initialize(void)
{
    FUNCTION_MARKER(0x80036ABC, "GTMAIN.EXE");
    gtmain_menu_selector_reset(0x80091BD8);
    return 0;
}

uint32 gtmain_menu_option_background_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    FUNCTION_MARKER(0x80036AE0, "GTMAIN.EXE");
    return gtmain_menu_gradient_draw(ordering + 16000, x, y, 320, 240, intensity,
        0, 32, 64, 32, 16, 16);
}

uint32 gtmain_menu_option_column_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    sint32 selection, uint32 list, uint32 resources, uint32 names, uint32 highlight)
{
    uint32 color, text, width, result, row = 0;
    sint32 half = intensity >> 1, selected;
    FUNCTION_MARKER(0x80036BD8, "GTMAIN.EXE");
    gtmain_text_flags(2);
    color = gtmain_menu_palette_color(3, half);
    gtmain_text_color(color);
    if (highlight) {
        sint32 pulse = gtmain_menu_pulse_component(0, half);
        gtmain_text_color(gtmain_menu_palette_color(6, pulse));
    }
    gtmain_text_font(2);
    text = r_u32(names + ((uint32)selection << 2));
    text = gtmain_course_name_find(text);
    text = gtmain_course_resource_name(text);
    text = gtmain_menu_name(text);
    width = gtmain_menu_text_width(2, text, 0);
    result = gtmain_menu_text_draw(ordering, (uint32)x - (uint32)((sint32)width >> 1),
        (uint32)y + 48, text, 0);
    selected = r_s8(list + 2);
    while (selected >= 0) {
        uint32 entry = resources + (uint32)selected * 20;
        sint32 position = r_s16(list + 98 + row * 2);
        uint32 brightness = r_u8(list + 26 + row);
        uint32 image = r_u32(entry + 4);
        uint32 size = r_u16(entry + 8);
        result = gtmain_menu_option_entry_draw(ordering, selected, 8, (uint32)x - 122,
            position, brightness, entry, image, size, 0, 1);
        ++row;
        selected = r_s8(list + row + 2);
    }
    return result;
}

uint32 gtmain_menu_option_columns_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    uint32 resources, names, record, list, color, first, second;
    sint32 selected, position, brightness, current;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80036D74, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x108, 0x80036D74);
    if (r_s8(0x801F2FEB) == 8) {
        resources = 0x800CB51C;
        names = 0x80091A68;
    } else {
        resources = 0x800CAF7C;
        names = 0x80091A44;
    }
    record = frame.address + 0x28;
    list = frame.address + 0x38;
    gtmain_menu_item_transition_draw(record, ordering, 0x80092324,
        (uint32)x + 160, (uint32)y + 56, intensity, 1);
    selected = r_s8(record + 1);
    if (selected >= 0) {
        position = r_s16(record + 6);
        brightness = r_u8(record + 11);
        gtmain_menu_descriptor_draw(ordering, 0x80092314, position, (uint32)y + 76,
            brightness, 1, (uint8 *)psx_addr(list, 152), 152);
        position = r_s16(record + 6);
        brightness = r_u8(record + 11);
        current = r_s8(0x801F2FEC);
        gtmain_menu_option_column_draw(ordering, position, y, brightness, selected, list,
            resources + (uint32)selected * 180 + 20, names, selected == current);
    }
    selected = r_s8(record);
    position = r_s16(record + 2);
    brightness = r_u8(record + 10);
    gtmain_menu_descriptor_draw(ordering, 0x80092314, position, (uint32)y + 76,
        brightness, 1, (uint8 *)psx_addr(list, 152), 152);
    position = r_s16(record + 2);
    brightness = r_u8(record + 10);
    current = r_s8(0x801F2FEC);
    gtmain_menu_option_column_draw(ordering, position, y, brightness, selected, list,
        resources + (uint32)selected * 180 + 20, names, selected == current);
    w_u16(frame.address + 0xD8, x);
    w_u16(frame.address + 0xDA, (uint32)y + 48);
    w_u16(frame.address + 0xDC, (uint32)x + 319);
    w_u16(frame.address + 0xDE, (uint32)y + 64);
    first = r_u32(frame.address + 0xD8);
    second = r_u32(frame.address + 0xDC);
    w_u32(frame.address + 0xD0, first);
    w_u32(frame.address + 0xD4, second);
    color = gtmain_menu_palette_color(5, intensity >> 1);
    gtmain_packet_rectangle(ordering, frame.address + 0xD0, color);
    gtmain_menu_option_background_draw(ordering, x, y, intensity);
    gt_local_stack_pop(frame);
    return 0;
}

uint32 gtmain_menu_option_entry_draw(uint32 ordering, sint32 selection, sint32 maximum, sint32 x,
    sint32 y, uint32 brightness, uint32 text, uint32 value, sint32 extra, uint32 flags, uint32 full_height)
{
    uint32 result, width, position, number, digit_x;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80040BBC, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x60, 0x80040BBC);
    w_u8(frame.address + 0x20, 0);
    w_u8(frame.address + 0x21, 0);
    gtmain_text_flags(2);
    gtmain_menu_rank_bar_draw(ordering + 8, selection, maximum, x, y, brightness, full_height);
    if (selection >= 0) {
        gtmain_text_font(2);
        gtmain_text_color(gtmain_menu_gray(brightness));
        number = (uint32)selection + 1;
        digit_x = (uint32)x + 2;
        if ((sint32)number >= 10) {
            uint32 tens = (sint32)number / 10;
            digit_x = (uint32)x + 10;
            w_u8(frame.address + 0x20, tens + 48);
            gtmain_font_text_draw(ordering, (uint32)x + 2, (uint32)y + 12, frame.address + 0x20);
            number -= tens * 10;
        }
        w_u8(frame.address + 0x20, number + 48);
        gtmain_font_text_draw(ordering, digit_x, (uint32)y + 12, frame.address + 0x20);
        w_u8(frame.address + 0x20, 46);
        gtmain_font_text_draw(ordering, digit_x + 8, (uint32)y + 12, frame.address + 0x20);
    }
    gtmain_text_font(2);
    if (!value) {
        gtmain_text_color(gtmain_menu_gray((sint32)brightness >> 1));
        width = gtmain_menu_text_width(2, 0x800BCB70, 1);
        position = (uint32)x + (uint32)((sint32)(220 - width) >> 1) + 24;
        result = gtmain_menu_text_draw(ordering, position, (uint32)y + 11, 0x800BCB70, 1);
        gt_local_stack_pop(frame);
        return result;
    }
    gtmain_text_color(gtmain_menu_gray(brightness));
    if (flags & 3)
        gtmain_menu_text_draw(ordering, (uint32)x + 31, (uint32)y + 12, text, 1);
    else
        gtmain_font_text_draw(ordering, (uint32)x + 31, (uint32)y + 12, text);
    if (!(flags & 4)) {
        gtmain_time_string(frame.address + 0x28, value);
        result = gtmain_menu_text_wave(ordering, (uint32)x + 170,
            (uint32)y + 12, frame.address + 0x28);
    } else {
        uint32 arguments[1];
        if (value > 10000)
            value = 9999;
        arguments[0] = value;
        gtmain_string_format(frame.address + 0x28, 0x800BCE26, arguments, 1);
        width = gtmain_menu_text_width(2, frame.address + 0x28, 0);
        result = gtmain_menu_text_draw(ordering, (uint32)x - (width - 240),
            (uint32)y + 12, frame.address + 0x28, 0);
    }
    if (extra >= 0) {
        gtmain_text_font(1);
        gtmain_text_color(gtmain_menu_palette_color(6, brightness));
        text = gtmain_resource_name(extra);
        result = gtmain_menu_text_draw(ordering, (uint32)x + 46, (uint32)y + 2, text, 0);
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_menu_option_columns_initialize(void)
{
    uint32 index, count, first, second;
    FUNCTION_MARKER(0x80036FC4, "GTMAIN.EXE");
    if (r_s8(0x801F2FEB) == 8) {
        count = 3;
        for (index = 0; index < 3; ++index)
            w_u8(0x801D31E4 + index * 12, 3);
    } else {
        count = 8;
        for (index = 0; index < 8; ++index) {
            uint32 address = 0x801D31E4 + index * 12;
            w_u8(address, 0x43);
            if (gtmain_event_competitor_complete(index))
                w_u8(address, r_u8(address) & 63);
        }
    }
    gtmain_menu_parameter_selector_reset(0x80092324);
    first = r_u8(0x801F2FEC);
    second = r_u8(0x801F2FEC);
    w_u32(0x8009232C, 0x801D31E4);
    w_u8(0x80092324, count);
    w_u8(0x80092325, first);
    w_u8(0x80092331, second);
    for (index = 0; index < 8; ++index) {
        w_u8(0x801D2FE4 + index * 8, 8);
        w_u32(0x801D2FE8 + index * 8, 0x800921D4);
    }
    w_u8(0x80092314, 8);
    w_u32(0x80092318, 0x801D2FE4);
    gtmain_menu_selector_reset(0x80092314);
    return 0;
}

uint32 gtmain_menu_option_list_tick(void)
{
    uint32 input;
    sint32 selected;
    FUNCTION_MARKER(0x80037128, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    if (!r_s8(input))
        return 0;
    selected = gtmain_menu_selector_input(0x80092314, input + 2);
    gtmain_menu_selector_layout(0x80092314);
    if (selected == -1)
        return 0;
    gtmain_menu_sound(selected == -2 ? 2 : 4);
    return 1;
}

uint32 gtmain_menu_option_list_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    uint32 flags, resources, list, row, entry, text, maximum, value;
    sint32 selected, row_y;
    uint32 brightness;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800371B8, "GTMAIN.EXE");
    frame = gt_local_stack_push(0xF0, 0x800371B8);
    flags = r_s8(0x801F2FEB) == 2 ? 5 : 1;
    resources = r_u32(0x801D2FE0);
    list = frame.address + 0x30;
    gtmain_menu_descriptor_draw(ordering, 0x80092314, (uint32)x + 160, (uint32)y + 72,
        intensity, 1, (uint8 *)psx_addr(list, 152), 152);
    gtmain_text_font(2);
    row = 0;
    selected = r_s8(list + 2);
    while (selected >= 0) {
        entry = resources + (uint32)selected * 28;
        row_y = r_s16(list + 0x62 + row * 2);
        brightness = r_u8(list + 0x1A + row);
        text = r_u32(entry + 12);
        maximum = r_u8(0x801D2FDC);
        value = r_u16(entry + 16);
        gtmain_menu_option_entry_draw(ordering, selected, maximum, (uint32)x + 38,
            row_y, brightness, entry, text, value, flags, 1);
        ++row;
        selected = r_s8(list + 2 + row);
    }
    gtmain_menu_option_background_draw(ordering, x, y, intensity);
    gt_local_stack_pop(frame);
    return 0;
}

uint32 gtmain_menu_selector_layout(uint32 selector)
{
    uint8 raw_visible;
    sint32 selected, count, visible, before, after;
    uint32 result;
    FUNCTION_MARKER(0x8003B88C, "GTMAIN.EXE");
    raw_visible = r_u8(selector + 2);
    selected = r_s8(selector + 1);
    count = r_s8(selector);
    visible = (sint8)raw_visible;
    before = visible >> 1;
    after = visible - before;
    if (selected < before)
        selected = before;
    if (selected + after >= count)
        selected = count - after;
    result = selected < count;
    if (!result)
        selected = count - 1;
    if (selected < 0)
        selected = 0;
    w_u8(selector + 1, selected);
    return result;
}

uint32 gtmain_menu_option_list_initialize(void)
{
    sint32 mode;
    uint32 resources, index;
    FUNCTION_MARKER(0x8003731C, "GTMAIN.EXE");
    mode = r_s8(0x801F2FEB);
    resources = mode == 0 ? 0x800CA7AC : mode == 1 ? 0x800CA9DC : 0x800CAC0C;
    w_u8(0x801D2FDC, 20);
    w_u32(0x801D2FE0, resources);
    for (index = 0; index < 20; ++index) {
        w_u8(0x801D2FE4 + index * 8, 8);
        w_u32(0x801D2FE8 + index * 8, 0x800921D4);
    }
    w_u32(0x80092318, 0x801D2FE4);
    w_u8(0x80092314, 20);
    gtmain_menu_selector_reset(0x80092314);
    return 0;
}

uint32 gtmain_menu_option_detail_tick(void)
{
    uint32 input, buttons, result = 0;
    FUNCTION_MARKER(0x800373D4, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    if (!r_s8(input))
        return 0;
    gtmain_menu_parameter_selector_input(0x80092324, input + 2);
    buttons = r_u16(input + 6);
    if (buttons & 0x6000) {
        gtmain_menu_sound(4);
        result = 1;
    }
    if (buttons & 0x9000) {
        gtmain_menu_sound(2);
        result = 1;
    }
    return result;
}

uint32 gtmain_menu_option_detail_entry_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    uint32 entry, uint32 caption, sint32 palette)
{
    uint32 staged, endpoints, first, second, color, text, extra, result;
    sint32 left, half;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80037460, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x68, 0x80037460);
    gtmain_text_flags(2);
    left = (uint32)x - 122;
    staged = frame.address + 0x38;
    endpoints = frame.address + 0x30;
    w_u16(staged, left);
    w_u16(staged + 2, (uint32)y - 2);
    w_u16(staged + 4, (uint32)x + 197);
    w_u16(staged + 6, (uint32)y - 2);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(endpoints, first);
    w_u32(endpoints + 4, second);
    color = gtmain_menu_palette_color(palette, intensity);
    gtmain_packet_outline(ordering, endpoints, color | 0x02000000);
    half = intensity >> 1;
    gtmain_text_color(gtmain_menu_gray(half));
    gtmain_menu_text_draw(ordering, left, (uint32)y - 18, caption, 0);
    text = r_u32(entry + 12);
    extra = r_u16(entry + 16);
    result = gtmain_menu_option_entry_draw(ordering, -1, 1, (uint32)x - 130,
        y, half, entry, text, extra, 1, 1);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_menu_option_detail_column_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    sint32 selected, uint32 resources, uint32 highlight)
{
    uint32 name, text, entry;
    sint32 half, width, pulse;
    FUNCTION_MARKER(0x8003758C, "GTMAIN.EXE");
    name = r_u32(0x8008F7E4 + (uint32)selected * 4);
    gtmain_text_font(2);
    gtmain_text_flags(2);
    half = intensity >> 1;
    if (r_s8(0x801F2FEB) == 9)
        name = r_u32(0x8008F83C + (uint32)selected * 4);
    gtmain_text_color(gtmain_menu_palette_color(3, half));
    if (highlight) {
        pulse = gtmain_menu_pulse_component(0, half);
        gtmain_text_color(gtmain_menu_palette_color(6, pulse));
    }
    text = gtmain_menu_name(gtmain_course_resource_name(gtmain_course_name_find(name)));
    width = gtmain_menu_text_width(2, text, 0);
    gtmain_menu_text_draw(ordering, (uint32)x - (width >> 1), (uint32)y + 48, text, 0);
    entry = resources + (uint32)selected * 56;
    gtmain_menu_option_detail_entry_draw(ordering, x, (uint32)y + 104, intensity,
        entry + 28, 0x800BC9FB, 4);
    return gtmain_menu_option_detail_entry_draw(ordering, x, (uint32)y + 176, intensity,
        entry, 0x800BCA08, 0);
}

uint32 gtmain_menu_option_detail_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    uint32 resources, record, staged, rectangle, first, second, color;
    sint32 selected, position, brightness, current;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80037728, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x58, 0x80037728);
    resources = r_u32(0x801D2FE0);
    record = frame.address + 0x20;
    gtmain_menu_item_transition_draw(record, ordering, 0x80092324,
        (uint32)x + 160, (uint32)y + 56, intensity, 1);
    selected = r_s8(record + 1);
    if (selected >= 0) {
        position = r_s16(record + 6);
        brightness = r_u8(record + 11);
        current = r_s8(0x801F2FEC);
        gtmain_menu_option_detail_column_draw(ordering, position, y, brightness,
            selected, resources, selected == current);
    }
    selected = r_s8(record);
    position = r_s16(record + 2);
    brightness = r_u8(record + 10);
    current = r_s8(0x801F2FEC);
    gtmain_menu_option_detail_column_draw(ordering, position, y, brightness,
        selected, resources, selected == current);
    staged = frame.address + 0x38;
    rectangle = frame.address + 0x30;
    w_u16(staged, x);
    w_u16(staged + 2, (uint32)y + 48);
    w_u16(staged + 4, (uint32)x + 319);
    w_u16(staged + 6, (uint32)y + 64);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(rectangle, first);
    w_u32(rectangle + 4, second);
    color = gtmain_menu_palette_color(5, intensity >> 1);
    gtmain_packet_rectangle(ordering, rectangle, color);
    gtmain_menu_option_background_draw(ordering, x, y, intensity);
    gt_local_stack_pop(frame);
    return 0;
}

uint32 gtmain_menu_option_detail_initialize(void)
{
    sint32 mode;
    uint32 count = 21, index, first, second;
    FUNCTION_MARKER(0x80037878, "GTMAIN.EXE");
    mode = r_s8(0x801F2FEB);
    w_u32(0x801D2FE0, 0x800CA1C4);
    if (mode == 9) {
        count = 3;
        w_u32(0x801D2FE0, 0x800CA65C);
    }
    for (index = 0; index < count; ++index)
        w_u8(0x801D31E4 + index * 12, 3);
    gtmain_menu_parameter_selector_reset(0x80092324);
    first = r_u8(0x801F2FEC);
    second = r_u8(0x801F2FEC);
    w_u32(0x8009232C, 0x801D31E4);
    w_u8(0x80092324, count);
    w_u8(0x80092325, first);
    w_u8(0x80092331, second);
    return 0;
}

uint32 gtmain_menu_quick_options_initialize(void)
{
    uint32 first, second;
    sint32 enabled;
    FUNCTION_MARKER(0x80037948, "GTMAIN.EXE");
    gtmain_menu_asset_activate(22);
    first = r_u8(0x80091E54) & 63;
    w_u8(0x80091E54, first);
    second = r_u8(0x80091E6C) & 63;
    enabled = r_s8(0x801F2FDC);
    w_u8(0x80091E6C, second);
    if (!enabled) {
        w_u8(0x80091E54, first | 64);
        w_u8(0x80091E6C, second | 64);
    }
    gtmain_menu_selector_reset(0x80091E84);
    return 0;
}

uint32 gtmain_menu_selector_1c20_initialize(void)
{
    FUNCTION_MARKER(0x80037A38, "GTMAIN.EXE");
    gtmain_menu_asset_activate(22);
    gtmain_menu_selector_reset(0x80091C20);
    return 0;
}

uint32 gtmain_menu_selector_1ca8_initialize(void)
{
    uint32 first, second;
    sint32 enabled;
    FUNCTION_MARKER(0x80037A64, "GTMAIN.EXE");
    gtmain_menu_asset_activate(22);
    first = r_u8(0x80091C68) & 63;
    w_u8(0x80091C68, first);
    second = r_u8(0x80091C88) & 63;
    enabled = r_s8(0x801F2FDC);
    w_u8(0x80091C88, second);
    if (!enabled) {
        w_u8(0x80091C68, first | 64);
        w_u8(0x80091C88, second | 64);
    }
    gtmain_menu_selector_reset(0x80091CA8);
    return 0;
}

uint32 gtmain_menu_selector_1d4c_initialize(void)
{
    uint32 first, second;
    sint32 enabled;
    FUNCTION_MARKER(0x80037ADC, "GTMAIN.EXE");
    gtmain_menu_asset_activate(22);
    first = r_u8(0x80091D28) & 63;
    w_u8(0x80091D28, first);
    second = r_u8(0x80091D38) & 63;
    enabled = r_s8(0x801F2FDC);
    w_u8(0x80091D38, second);
    if (!enabled) {
        w_u8(0x80091D28, first | 64);
        w_u8(0x80091D38, second | 64);
    }
    gtmain_menu_selector_reset(0x80091D4C);
    return 0;
}

uint32 gtmain_menu_selector_1da8_initialize(void)
{
    uint32 first, second;
    sint32 enabled;
    FUNCTION_MARKER(0x80037B54, "GTMAIN.EXE");
    gtmain_menu_asset_activate(22);
    first = r_u8(0x80091D78) & 63;
    w_u8(0x80091D78, first);
    second = r_u8(0x80091D90) & 63;
    enabled = r_s8(0x801F2FDC);
    w_u8(0x80091D90, second);
    if (!enabled) {
        w_u8(0x80091D78, first | 64);
        w_u8(0x80091D90, second | 64);
    }
    gtmain_menu_selector_reset(0x80091DA8);
    return 0;
}

uint32 gtmain_menu_selector_1e28_initialize(void)
{
    uint32 first, second, third, fourth;
    sint32 enabled, mode;
    FUNCTION_MARKER(0x80037BCC, "GTMAIN.EXE");
    gtmain_menu_asset_activate(22);
    first = r_u8(0x80091DF0) & 63;
    w_u8(0x80091DF0, first);
    second = r_u8(0x80091E00) & 63;
    third = r_u8(0x80091E0C) & 63;
    w_u8(0x80091E00, second);
    w_u8(0x80091E0C, third);
    fourth = r_u8(0x80091E14) & 63;
    enabled = r_s8(0x801F2FDC);
    w_u8(0x80091E14, fourth);
    if (!enabled) {
        w_u8(0x80091DF0, first | 64);
        w_u8(0x80091E00, second | 64);
        w_u8(0x80091E0C, third | 64);
        w_u8(0x80091E14, fourth | 64);
    }
    w_u32(0x801F2FE4, 0x80091E24);
    mode = r_s8(0x801F2FEA);
    w_u32(0x80091E2C, 0x80091E0C);
    if (mode) {
        w_u32(0x80091E2C, 0x80091DF0);
        w_u32(0x801F2FE4, 0x80091E08);
    }
    w_u8(0x80091E2A, 3);
    w_u8(0x80091E28, 3);
    gtmain_menu_selector_reset(0x80091E28);
    return 0;
}

sint32 gtmain_alternate_trial_mode8_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80037DC0, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091E94, disabled, 0x80091E84, 0x80091E7C, 8);
}

sint32 gtmain_trial_mode4_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80037E40, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091CB8, disabled, 0x80091CA8, 0x80091CA0, 4);
}

sint32 gtmain_trial_mode4_course_options(void)
{
    FUNCTION_MARKER(0x80037E80, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091C30, 0, 0x80091C20, 0x80091C1C, 4);
}

sint32 gtmain_menu_end_title_transition_draw(uint32 ordering, sint32 x, sint32 y, uint32 text,
    sint32 palette, sint32 intensity)
{
    sint32 phase, product, faded;
    FUNCTION_MARKER(0x800381A4, "GTMAIN.EXE");
    phase = r_s16(0x801F300A);
    product = (uint32)intensity * (uint32)phase;
    faded = (uint32)intensity - (uint32)(product / 8);
    gtmain_text_color(gtmain_menu_palette_color(palette, faded >> 1));
    return gtmain_font_text_draw(ordering, (uint32)x + (uint32)phase * 18, y, text);
}

uint32 gtmain_menu_license_name_tick(void)
{
    uint32 input, name, flags, descriptor, input_span, name_span;
    sint32 selected, group, section, index;
    FUNCTION_MARKER(0x8003872C, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    if (!r_s8(input))
        return 0;
    selected = gtmain_menu_name_edit(0x8009208C, input + 2);
    if (selected == -2) {
        gtmain_menu_sound(6);
        return 0;
    }
    if (selected != 0)
        return 0;
    gtmain_string_copy(0x800CFF88, 0x801F2FEA);
    group = r_s8(0x801F2FE2);
    section = r_s8(0x801F2FE3);
    index = r_s8(0x801F2FE1);
    name = 0x800C77AC + (uint32)group * 1792 + (uint32)section * 224 + (uint32)index * 28;
    gtmain_string_copy(name, 0x801F2FEA);
    index = r_s8(0x801F2FE1);
    input_span = gtmain_string_length(0x801F2FEA) + 1;
    name_span = gtmain_string_length(name) + 1;
    printf((const char *)psx_addr(0x8008CC64, 26), index,
        (const char *)psx_addr(0x801F2FEA, input_span), (const char *)psx_addr(name, name_span));
    flags = r_u8(0x801F2FE6);
    descriptor = 0x8009209C;
    if (flags & 1)
        descriptor = 0x800920D4;
    else if (flags & 4) {
        descriptor = 0x80092248;
        printf((const char *)psx_addr(0x8008CC80, 11));
    }
    gtmain_menu_push(descriptor);
    gtmain_menu_sound(4);
    return 3;
}

uint32 gtmain_menu_license_name_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    FUNCTION_MARKER(0x800388B0, "GTMAIN.EXE");
    gtmain_menu_name_keyboard_draw(ordering, 0x8009208C, (uint32)x + 32, (uint32)y + 56, intensity);
    return 0;
}

uint32 gtmain_menu_license_name_initialize(void)
{
    FUNCTION_MARKER(0x800388E8, "GTMAIN.EXE");
    gtmain_menu_resource_request_initialize(0x8009208C, 11, 128, 0x801F2FEA);
    gtmain_menu_name_text_set(0x8009208C, 0x800CFF88);
    return 0;
}

uint32 gtmain_menu_license_award_tick(void)
{
    uint32 input, phase, next, flags, descriptor;
    FUNCTION_MARKER(0x80038938, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    gtmain_menu_model_pose_tick(0x801F3018, 0);
    if (!r_s8(input))
        return 0;
    gt_draft_800380AC();
    phase = r_u16(0x801F3004);
    if (phase == 30) {
        gtmain_menu_sound(1);
        next = r_u16(0x801F3004);
        w_u16(0x801F300A, 8);
        w_u16(0x801F3004, next + 1);
    } else if (phase == 60) {
        w_u16(0x801F3004, phase + 1);
    } else if (phase == 90) {
        if (!(r_u16(input + 6) & 0xFF0F))
            return 0;
        flags = r_u8(0x801F2FE6);
        descriptor = flags & 4 ? 0x80092248 : 0x8009209C;
        gtmain_menu_push(descriptor);
        gtmain_menu_sound(4);
        return 3;
    } else {
        next = r_u16(0x801F3004);
        w_u16(0x801F3004, next + 1);
    }
    return 0;
}

uint32 gtmain_menu_license_award_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    sint32 group;
    uint32 phase;
    FUNCTION_MARKER(0x80038A88, "GTMAIN.EXE");
    group = r_s8(0x801F2FE2);
    phase = r_u16(0x801F3004);
    return gtmain_menu_license_award_draw_carried(ordering, x, y, unused, intensity, group, phase);
}

uint32 gtmain_menu_license_award_draw_carried(uint32 ordering, sint32 x, sint32 y, sint32 unused,
    sint32 intensity, sint32 group, uint32 phase)
{
    uint32 text, toggle;
    sint32 width, centered, style, mode;
    FUNCTION_MARKER(0x80038A98, "GTMAIN.EXE");
    text = r_u32(0x80091ED0 + (uint32)group * 4);
    if (phase >= 31) {
        gtmain_text_flags(3);
        gtmain_text_font(0);
        width = gtmain_text_width(3, text);
        centered = (uint32)x - ((uint32)(width >> 1) - 160);
        mode = r_s8(0x801F2FE2);
        toggle = r_u8(0x801F3008);
        style = r_s8(0x80091EE4 + mode * 2 + toggle);
        gtmain_menu_end_title_transition_draw(ordering, centered, (uint32)y + 64, text, style, intensity);
    }
    if (r_u16(0x801F3004) >= 90) {
        gtmain_text_flags(2);
        gtmain_text_color(gtmain_menu_pulse_color(intensity >> 1));
        width = gtmain_text_width(2, 0x800BCC0C);
        centered = (uint32)x - ((uint32)(width >> 1) - 160);
        gtmain_font_text_draw(ordering, centered, (uint32)y + 204, 0x800BCC0C);
    }
    if (r_s8(0x801F2FE5) == 1)
        gtmain_menu_pose_model_draw(0x801F3018, ordering, (uint32)x + 160, (uint32)y + 120,
            0x8000, 0x80190FDC, 9, 0x4018, 6, 0x7FD3, 0x122, 64, 64);
    return 0;
}

uint32 gtmain_menu_event_options_tick(void)
{
    uint32 input, buttons, repeat, value, previous, descriptor;
    sint32 selected, current, limit;
    FUNCTION_MARKER(0x80038D14, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    if (!r_s8(input))
        return 0;
    gtmain_menu_caption_enable();
    selected = gtmain_menu_selector_input(0x8009207C, input + 2);
    switch (selected) {
    case -2:
    case 5:
        gtmain_menu_sound(selected == -2 ? 2 : 4);
        w_u8(0x801F2FE4, 255);
        descriptor = 0x80092170;
        break;
    case -1:
        if (r_s8(0x8009207D) != 2)
            return 0;
        buttons = r_u16(input + 6);
        repeat = r_u16(input + 10);
        buttons |= repeat;
        if (buttons & 0x80) {
            gtmain_menu_sound(0);
            value = r_u8(0x801F2FE7) - 1;
            w_u8(0x801F2FE7, value);
            if ((sint8)value < 0) {
                value = r_u8(0x801F2FE8) - 1;
                w_u8(0x801F2FE7, value);
            }
        } else if (buttons & 0x20) {
            gtmain_menu_sound(0);
            value = r_u8(0x801F2FE7);
            limit = r_s8(0x801F2FE8);
            ++value;
            w_u8(0x801F2FE7, value);
            if ((sint8)value >= limit)
                w_u8(0x801F2FE7, 0);
        }
        return 0;
    case 3:
        gtmain_menu_sound(4);
        gtmain_menu_result_record_request(0x8003A304, 0x80038CA8);
        descriptor = 0x80092270;
        break;
    case 4:
        gtmain_menu_sound(4);
        descriptor = 0x8009210C;
        break;
    default:
        gtmain_menu_sound(4);
        w_u8(0x801F2FE4, selected);
        if (selected == 2) {
            previous = r_u8(0x800CB7F9);
            current = r_s8(0x801F2FE7);
            if (previous == current)
                w_u8(0x801F2FE4, 1);
            value = r_u8(0x801F2FE7);
            w_u8(0x800CB7F9, value);
        }
        descriptor = 0x80092170;
        break;
    }
    gtmain_menu_push(descriptor);
    return 3;
}

uint32 gtmain_menu_event_options_initialize(void)
{
    sint32 count, selected;
    FUNCTION_MARKER(0x80039468, "GTMAIN.EXE");
    gtmain_tim_upload(0x80091EEC, 0x01000250, -1, 0x01000240, -1, 1);
    count = gt_draft_8002A390();
    w_u8(0x801F2FE9, count);
    if (!gt_draft_8002A474())
        --count;
    selected = r_s8(0x801F2FE3);
    w_u8(0x801F2FE8, count);
    ++selected;
    if (selected >= count)
        selected = count - 1;
    w_u8(0x801F2FE7, selected);
    gtmain_menu_selector_reset(0x8009207C);
    if (!r_s8(0x801F2FE0))
        gtmain_menu_list_select(0x8009207C, 1);
    gtmain_menu_asset_release();
    gtmain_menu_asset_activate(22);
    return 0;
}

uint32 gtmain_menu_asset_release(void)
{
    FUNCTION_MARKER(0x8003A2DC, "GTMAIN.EXE");
    w_u8(0x800B52FA, 0);
    return gtmain_stream_stop_wait(1);
}

uint32 gtmain_menu_event_results_tick(void)
{
    uint32 input;
    sint32 selected;
    FUNCTION_MARKER(0x80039544, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    if (!r_s8(input))
        return 0;
    gtmain_menu_caption_enable();
    gtmain_menu_parameter_selector_input(0x80092324, input + 2);
    selected = gtmain_menu_selector_input(0x80092314, input + 2);
    gtmain_menu_selector_layout(0x80092314);
    if (selected == -1)
        return 0;
    gtmain_menu_sound(selected == -2 ? 2 : 4);
    return 1;
}

uint32 gtmain_menu_event_results_column_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    sint32 selection, uint32 list, uint32 resources)
{
    uint32 names, buffer, title_slot, text, result, row, brightness, entry, value;
    uint32 arguments[1];
    sint32 half, width, selected, row_y, icon;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800395EC, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x158, 0x800395EC);
    names = gt_draft_8002A4DC();
    gtmain_text_font(2);
    half = intensity >> 1;
    gtmain_text_color(gtmain_menu_palette_color(3, half));
    gtmain_text_flags(2);
    buffer = frame.address + 0x30;
    arguments[0] = (uint32)selection + 1;
    gtmain_string_format(buffer, 0x800BCCA2, arguments, 1);
    width = gtmain_menu_text_width(2, buffer, 0);
    gtmain_font_text_draw(ordering, (uint32)x - (width >> 1), (uint32)y + 48, buffer);
    title_slot = names + (uint32)selection * 4;
    text = r_u32(title_slot);
    width = gtmain_menu_caption_width(text);
    gtmain_text_color(gtmain_menu_palette_color(6, half));
    text = r_u32(title_slot);
    result = gtmain_menu_caption_plain_draw(ordering, (uint32)x - (width >> 1), (uint32)y + 63, text);
    row = 0;
    selected = r_s8(list + 2);
    while (selected >= 0) {
        icon = 3 - selected;
        row_y = r_s16(list + 0x62 + row * 2);
        brightness = r_u8(list + 0x1A + row);
        if (icon <= 0)
            icon = 1;
        gtmain_menu_sprite(0x8009200C + icon * 16, ordering, (uint32)x - 107,
            (uint32)row_y + 6, brightness);
        entry = resources + (uint32)selected * 28;
        value = r_u32(entry + 12);
        result = gtmain_menu_option_entry_draw(ordering, selected, 8, (uint32)x - 122,
            (uint32)row_y - 5, brightness, entry, value, -1, 1, 0);
        ++row;
        selected = r_s8(list + 2 + row);
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_menu_event_results_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity)
{
    uint32 record, list, resources, staged, rectangle, first, second, color;
    sint32 selected, position, brightness;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800397C8, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x110, 0x800397C8);
    record = frame.address + 0x30;
    list = frame.address + 0x40;
    gtmain_menu_item_transition_draw(record, ordering, 0x80092324,
        (uint32)x + 160, (uint32)y + 56, intensity, 1);
    selected = r_s8(record + 1);
    if (selected >= 0) {
        position = r_s16(record + 6);
        brightness = r_u8(record + 11);
        gtmain_menu_descriptor_draw(ordering, 0x80092314, position, (uint32)y + 100,
            brightness, 1, (uint8 *)psx_addr(list, 152), 152);
        position = r_s16(record + 6);
        brightness = r_u8(record + 11);
        resources = r_u32(0x801F30D0);
        gtmain_menu_event_results_column_draw(ordering, position, y, brightness, selected,
            list, resources + (uint32)selected * 224);
    }
    selected = r_s8(record);
    position = r_s16(record + 2);
    brightness = r_u8(record + 10);
    gtmain_menu_descriptor_draw(ordering, 0x80092314, position, (uint32)y + 100,
        brightness, 1, (uint8 *)psx_addr(list, 152), 152);
    position = r_s16(record + 2);
    brightness = r_u8(record + 10);
    resources = r_u32(0x801F30D0);
    gtmain_menu_event_results_column_draw(ordering, position, y, brightness, selected,
        list, resources + (uint32)selected * 224);
    staged = frame.address + 0xE0;
    rectangle = frame.address + 0xD8;
    w_u16(staged, x);
    w_u16(staged + 2, (uint32)y + 48);
    w_u16(staged + 4, (uint32)x + 319);
    w_u16(staged + 6, (uint32)y + 81);
    first = r_u32(staged);
    second = r_u32(staged + 4);
    w_u32(rectangle, first);
    w_u32(rectangle + 4, second);
    color = gtmain_menu_palette_color(20, intensity >> 2);
    gtmain_packet_rectangle(ordering, rectangle, color);
    gtmain_menu_gradient_draw(ordering + 16000, x, y, 320, 240, intensity, 0, 32, 64, 32, 16, 16);
    gt_local_stack_pop(frame);
    return 0;
}

uint32 gtmain_menu_event_results_initialize(void)
{
    sint32 group, selected;
    uint32 index;
    FUNCTION_MARKER(0x800399CC, "GTMAIN.EXE");
    group = r_s8(0x801F2FE2);
    selected = r_s8(0x801F2FE3);
    w_u32(0x801F30D0, 0x800C77AC + (uint32)group * 1792);
    for (index = 0; index < 8; ++index)
        w_u8(0x801F3030 + index * 12, 3);
    gtmain_menu_parameter_selector_reset(0x80092324);
    w_u32(0x8009232C, 0x801F3030);
    w_u8(0x80092324, 8);
    w_u8(0x80092325, selected);
    w_u8(0x80092331, selected);
    for (index = 0; index < 8; ++index) {
        w_u8(0x801F3090 + index * 8, 8);
        w_u32(0x801F3094 + index * 8, 0x800921D4);
    }
    w_u8(0x8009231D, 18);
    w_u8(0x8009231E, 8);
    w_u8(0x80092314, 8);
    w_u32(0x80092318, 0x801F3090);
    gtmain_menu_selector_reset(0x80092314);
    return 0;
}

uint32 gtmain_resource_entry_decode(uint32 resource, uint32 index, uint32 destination, uint32 unused)
{
    uint32 record, offset, bytes, source;
    FUNCTION_MARKER(0x800711D0, "GTMAIN.EXE");
    record = resource + index * 12;
    offset = r_u32(record + 16);
    bytes = r_u32(record + 24);
    source = resource + offset;
    if ((sint32)bytes > 0)
        gtmain_lz_decode((uint8 *)psx_addr(destination, bytes), bytes,
            (const uint8 *)psx_addr(source, 1), 1, bytes);
    return destination;
}

uint32 gtmain_pit_menu_textures_load(void)
{
    uint32 resource, path, palette_count = 0, entry = 0, index, descriptor;
    uint32 palette_row, clut, u, v, previous, level, source;
    sint32 page, texture_x, texture_y, left, top;
    FUNCTION_MARKER(0x80039CA8, "GTMAIN.EXE");
    gtmain_stream_mode_set(128);
    path = gtmain_environment_value(0x8008CCD4);
    resource = sections_read_retry(path, 0x801BB70C, 0);
    for (index = 0; index < 17; ++index) {
        descriptor = 0x80092334 + index * 16;
        page = r_s8(descriptor + 1);
        if (page >= 16) {
            texture_x = (uint32)(page - 16) << 6;
            texture_y = 256;
        } else {
            texture_x = (uint32)page << 6;
            texture_y = 0;
        }
        left = r_s16(descriptor + 2);
        texture_x = (uint32)texture_x + (uint32)(left / 4);
        top = r_s16(descriptor + 4);
        texture_y = (uint32)texture_y + (uint32)top;
        if (r_s8(descriptor)) {
            palette_row = palette_count;
            clut = (palette_count << 6) | 32;
            ++palette_count;
        } else {
            palette_row = 23;
            clut = 32;
        }
        v = r_u8(descriptor + 4);
        u = r_u8(descriptor + 2);
        w_u32(descriptor + 12, u | (v << 8) | (clut << 16));
        gtmain_resource_entry_decode(resource, entry++, 0x80190FDC, 0);
        gtmain_tim_upload(0x80190FDC, (palette_row << 16) | 0x200, -1,
            (uint32)texture_x | ((uint32)texture_y << 16), -1, 0);
    }
    palette_row = palette_count;
    clut = (palette_count << 6) | 32;
    ++palette_count;
    gtmain_resource_entry_decode(resource, entry++, 0x80190FDC, 0);
    gtmain_tim_upload(0x80190FDC, (palette_row << 16) | 0x200, -1, 0x214, -1, 0);
    for (index = 0; index < 11; ++index) {
        descriptor = 0x80092444 + index * 16;
        v = r_u8(descriptor + 4);
        u = r_u8(descriptor + 2);
        w_u32(descriptor + 12, u | (v << 8) | (clut << 16));
    }
    gtmain_resource_entry_decode(resource, entry, 0x80190FDC, 0);
    gtmain_tim_upload(0x80190FDC, (palette_count << 16) | 0x200, 0x00070010, 0x00380218, -1, 0);
    for (index = 0; index < 5; ++index) {
        descriptor = 0x800924F4 + index * 32;
        clut = ((palette_count + index) << 6) | 32;
        v = r_u8(descriptor + 4);
        u = r_u8(descriptor + 2);
        w_u32(descriptor + 12, u | (v << 8) | (clut << 16));
        descriptor += 16;
        clut = ((palette_count + 5) << 6) | 32;
        v = r_u8(descriptor + 4);
        u = r_u8(descriptor + 2);
        w_u32(descriptor + 12, u | (v << 8) | (clut << 16));
    }
    gtmain_resource_entry_decode(resource, ++entry, 0x80190FDC, 0);
    gtmain_tim_upload(0x80190FDC, 0x01F001C0, -1, 0x010001C0, -1, 0);
    gtmain_resource_entry_decode(resource, ++entry, 0x80190FDC, 0);
    gtmain_tim_upload(0x80190FDC, 0x01F00200, -1, 0x01000200, -1, 0);
    gtmain_resource_entry_decode(resource, ++entry, 0x80190FDC, 0);
    gtmain_tim_upload(0x80190FDC, 0x01000240, -1, 0x01000240, -1, 0);
    ++entry;
    for (index = 0; index < 30; ++index) {
        descriptor = 0x80092594 + index * 16;
        page = r_s8(descriptor);
        clut = ((uint32)(511 - page) << 6) | 36;
        v = r_u8(descriptor + 4);
        u = r_u8(descriptor + 2);
        w_u32(descriptor + 12, u | (v << 8) | (clut << 16));
    }
    gtmain_resource_entry_decode(resource, entry, 0x80190FDC, 0);
    gtmain_tim_upload(0x80190FDC, 0x00F00240, -1, 0x240, -1, 0);
    if (r_u8(0x800C4C3D) >= 2) {
        for (index = 0; index < 5; ++index) {
            descriptor = 0x80092334 + index * 16;
            w_u8(descriptor + 1, 9);
            level = r_u8(0x800C4C3D);
            source = 0x80092128 + (level - 2) * 10 + index * 2;
            u = r_u8(source);
            w_u16(descriptor + 2, u);
            level = r_u8(0x800C4C3D);
            source = 0x80092128 + (level - 2) * 10 + index * 2;
            v = r_u8(source + 1);
            previous = r_u32(descriptor + 12);
            u = r_u8(descriptor + 2);
            w_u16(descriptor + 4, v);
            w_u32(descriptor + 12, (previous & 0xFFFF0000) | u | (v << 8));
        }
    }
    level = r_u8(0x800C4C3D);
    return gtmain_resource_entry_decode(resource, level + 24, 0x800BDE30, 0);
}

sint32 gtmain_pit_menu_run(sint32 state)
{
    sint32 result;
    FUNCTION_MARKER(0x8003A1A0, "GTMAIN.EXE");
    printf((const char *)psx_addr(0x8008CCE4, 13));
    w_u8(0x800BEA24, state);
    gtmain_menu_draw_callback_set(state == 2 ? 0x80092C98 : 0x80092C4C);
    gtmain_menu_panel_special_bind(0x800BDD10, 0x80092154);
    gtmain_pit_menu_textures_load();
    gtmain_music_sector_init();
    gtmain_stream_stop_wait(1);
    gtmain_menu_frame_run();
    gtmain_stream_stop_wait(1);
    result = r_s8(0x800BEA24);
    printf((const char *)psx_addr(0x8008CCF4, 19), result);
    return r_s8(0x800BEA24);
}

uint32 gtmain_menu_effect_fade_begin(void)
{
    FUNCTION_MARKER(0x8003A3D0, "GTMAIN.EXE");
    w_u8(0x800B52F9, 1);
    w_u8(0x800B52F8, 24);
    return 0;
}

uint32 gtmain_menu_text_cells_draw(uint32 ordering, sint32 x, sint32 y, uint32 text,
    sint32 cell_width, sint32 font)
{
    sint32 character, width, gap;
    FUNCTION_MARKER(0x8003A95C, "GTMAIN.EXE");
    character = r_s8(text);
    while (character) {
        w_u8(0x8008CDB0, character);
        ++text;
        width = gtmain_text_width(font, 0x8008CDB0);
        gap = (uint32)cell_width - (uint32)width;
        gtmain_font_text_draw(ordering, (uint32)x + (uint32)(gap >> 1), y, 0x8008CDB0);
        x = (uint32)x + (uint32)cell_width;
        character = r_s8(text);
    }
    return 0;
}

uint32 gtmain_packet_flat_triangle(uint32 ordering, uint32 coordinates, uint32 color)
{
    uint32 packet, value, previous, address, lane, byte;
    FUNCTION_MARKER(0x8003B54C, "GTMAIN.EXE");
    packet = r_u32(0x800BFE4C);
    w_u32(packet, 0x04000000);
    w_u32(packet + 4, (color & 0x00FFFFFF) | 0x20000000);
    value = r_u32(coordinates);
    w_u32(packet + 8, value);
    value = r_u32(coordinates + 4);
    w_u32(packet + 12, value);
    value = r_u32(coordinates + 8);
    w_u32(packet + 16, value);
    previous = ordering + 2;
    lane = previous & 3;
    address = previous & ~3;
    for (byte = 0; byte <= lane; ++byte) {
        uint32 shift = (3 - lane + byte) * 8;
        value = r_u8(address + byte);
        previous = (previous & ~((uint32)255 << shift)) | (value << shift);
    }
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, (packet << 8) >> ((3 - lane + byte) * 8));
    address = packet + 2;
    lane = address & 3;
    address &= ~3;
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, previous >> ((3 - lane + byte) * 8));
    w_u32(0x800BFE4C, packet + 20);
    return previous;
}

uint32 gtmain_race_specification_summary(uint32 specification, uint32 values, uint32 detail)
{
    uint32 transmission, curve, result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80058BB0, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x178, 0x80058BB0);
    transmission = frame.address + 0x10;
    curve = frame.address + 0xB0;
    gtmain_race_engine_curve_initialize(specification, curve, 0);
    gtmain_race_transmission_initialize(specification, transmission, curve);
    gtmain_race_engine_curve_summary(curve, detail);
    gtmain_race_transmission_summary(transmission, curve, values);
    result = r_u16(detail);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_race_transmission_summary(uint32 transmission, uint32 curve, uint32 values)
{
    uint32 count, gear, rpm;
    sint32 divided, ratio, quotient, scaled;
    sint64 product;
    FUNCTION_MARKER(0x80058948, "GTMAIN.EXE");
    count = r_u8(transmission + 2);
    w_u32(values, count);
    for (gear = 1; gear <= count; ++gear) {
        rpm = gear == 1 ? 1000 : r_u16(transmission + gear * 2 + 0x14);
        w_u16(values + gear * 2 + 2, rpm);
        divided = (rpm << 12) / 60;
        ratio = r_s32(transmission + gear * 4 + 0x44);
        quotient = (uint32)gtmain_signed_divide64((sint64)divided * 4096, ratio, 0x80058A14);
        product = (sint64)quotient * 9164;
        scaled = (uint32)(product / 4096);
        w_u16(values + gear * 2 + 0x1E, scaled >> 12);
        rpm = gear == count ? r_u16(curve + 0x8C) : r_u16(transmission + gear * 2 + 0x0A);
        w_u16(values + gear * 2 + 0x10, rpm);
        divided = (rpm << 12) / 60;
        ratio = r_s32(transmission + gear * 4 + 0x44);
        quotient = (uint32)gtmain_signed_divide64((sint64)divided * 4096, ratio, 0x80058AD0);
        product = (sint64)quotient * 9164;
        scaled = (uint32)(product / 4096);
        w_u16(values + gear * 2 + 0x2C, scaled >> 12);
    }
    return count != 0;
}

uint32 gtmain_race_engine_curve_summary(uint32 curve, uint32 detail)
{
    sint32 count, index, scaled, rpm, limit, value, torque_base, torque, power, quotient, high;
    sint32 peak_power = 0, peak_power_rpm = 0, peak_torque = 0, peak_torque_rpm = 0;
    uint32 product, sample = 0, slot, result;
    sint64 wide;
    FUNCTION_MARKER(0x8005ECF4, "GTMAIN.EXE");
    count = r_s16(curve);
    w_u16(detail + 10, count);
    for (index = 0; index < count; ++index) {
        product = r_u32(curve + (uint32)index * 4 + 12) * 60;
        scaled = (sint32)product / 4096;
        rpm = ((scaled + 50) / 100) * 100;
        w_u16(detail + (uint32)index * 2 + 12, rpm);
    }
    rpm = r_s16(detail + 12);
    limit = r_u16(curve + 0x8C);
    result = limit < rpm;
    while (!result) {
        scaled = (uint32)rpm << 12;
        value = gtmain_race_curve32(curve, scaled / 60);
        product = (uint32)value * 10;
        torque_base = (sint32)product / 39;
        quotient = (uint32)gtmain_signed_divide64((sint64)value * 4096, 40141, 0x8005EE58);
        product = (uint32)torque_base * 72329;
        torque = (sint32)product / 10000;
        wide = (sint64)quotient * rpm;
        scaled = (uint32)(wide / 4096);
        quotient = (uint32)gtmain_signed_divide64((sint64)scaled * 4096, 2864, 0x8005EEF4);
        product = (uint32)quotient * 1000;
        wide = (sint64)(sint32)product * 0x2050C9F9;
        high = (uint64)wide >> 32;
        power = (uint32)(high >> 7) - (uint32)((sint32)product >> 31);
        slot = detail + sample * 2;
        if (r_s16(slot + 12) == rpm) {
            ++sample;
            w_u16(slot + 0x2C, power);
            w_u16(slot + 0x4C, torque);
        }
        if (peak_torque < torque) {
            peak_torque = torque;
            peak_torque_rpm = rpm;
        }
        if (peak_power < power) {
            peak_power = power;
            peak_power_rpm = rpm;
        }
        limit = r_u16(curve + 0x8C);
        rpm = (uint32)rpm + 100;
        result = limit < rpm;
    }
    if (detail) {
        w_u16(detail, peak_power);
        w_u16(detail + 2, peak_power_rpm);
        w_u16(detail + 4, peak_torque);
        w_u16(detail + 6, peak_torque_rpm);
    }
    return result;
}

uint32 gtmain_packet_triangle_up(uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    uint32 result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003B5B8, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x28, 0x8003B5B8);
    w_u16(frame.address + 0x12, y);
    w_u16(frame.address + 0x10, x);
    w_u16(frame.address + 0x18, (uint32)x - 6);
    w_u16(frame.address + 0x16, (uint32)y + 6);
    w_u16(frame.address + 0x1A, (uint32)y + 6);
    w_u16(frame.address + 0x14, (uint32)x + 6);
    result = gtmain_packet_flat_triangle(ordering, frame.address + 0x10, color);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_packet_triangle_down(uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    uint32 result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003B600, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x28, 0x8003B600);
    w_u16(frame.address + 0x10, (uint32)x - 6);
    w_u16(frame.address + 0x18, x);
    w_u16(frame.address + 0x1A, y);
    w_u16(frame.address + 0x12, (uint32)y - 6);
    w_u16(frame.address + 0x14, (uint32)x + 6);
    w_u16(frame.address + 0x16, (uint32)y - 6);
    result = gtmain_packet_flat_triangle(ordering, frame.address + 0x10, color);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_packet_triangle_right(uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    uint32 result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003B648, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x28, 0x8003B648);
    w_u16(frame.address + 0x16, y);
    w_u16(frame.address + 0x14, x);
    w_u16(frame.address + 0x1A, (uint32)y + 6);
    w_u16(frame.address + 0x10, (uint32)x - 6);
    w_u16(frame.address + 0x12, (uint32)y - 6);
    w_u16(frame.address + 0x18, (uint32)x - 6);
    result = gtmain_packet_flat_triangle(ordering, frame.address + 0x10, color);
    gt_local_stack_pop(frame);
    return result;
}
