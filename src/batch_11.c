#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include "psx_spu.h"

uint32 gtmenu_layout_list_measure(uint32 result, uint32 node)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 widget, kind, position = 0, width = 0, value, text;
    uint8 code;
    FUNCTION_MARKER(0x8002CBDC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x8002CBDC);
    while (node) {
        widget = r_u32(node);
        kind = r_u32(widget);
        if (kind == 0x8000) {
            text = r_u32(widget + 12);
            gtmenu_layout_text_measure(frame.address, position, text);
            position = r_u32(frame.address);
        } else {
            widget = r_u32(node);
            code = r_u8(widget);
            code -= 8;
            gtmenu_layout_tag_measure(frame.address + 8, position, code);
            position = r_u32(frame.address + 8);
        }
        value = position & 0xFFFF;
        if (width < value) width = value;
        node = r_u32(node + 4);
    }
    w_u32(result, width | (position & 0xFFFF0000));
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_layout_text_measure(uint32 result, uint32 position, uint32 text)
{
    uint32 index = 0, width = 0, height, line_height, extra;
    sint32 font_width;
    sint8 character;
    uint8 lead;
    FUNCTION_MARKER(0x8002CCD8, "GTMENU.EXE");
    character = r_s8(text);
    font_width = r_s16(0x800A7A1C);
    while (character) {
        lead = r_u8(text + index);
        width += (uint32)font_width;
        index += (lead & 0x80) ? 2 : 1;
        character = r_s8(text + index);
    }
    width = (position + width) & 0xFFFF;
    height = position >> 16;
    line_height = r_u16(0x800A7A1E);
    extra = r_u16(0x800A7A20);
    height += line_height + extra;
    w_u32(result, width | (height << 16));
    return result;
}

uint32 gtmenu_layout_tag_measure(uint32 result, uint32 position, uint8 code)
{
    uint32 line_height, extra, height;
    FUNCTION_MARKER(0x8002CD7C, "GTMENU.EXE");
    line_height = r_u16(0x800A7A1E);
    extra = r_u16(0x800A7A20);
    if (!(code & 0x80)) {
        code &= 0x7F;
        if (code == 14) {
            position &= 0xFFFF0000;
        } else if (code == 53) {
            if (position & 0xFFFF) {
                position &= 0xFFFF0000;
            } else {
                height = position >> 16;
                height += line_height + extra;
                position = (position & 0xFFFF) | (height << 16);
            }
        }
    }
    w_u32(result, position);
    return result;
}

uint32 gtmenu_polygon_draw(uint32 ordering, uint32 vertices, sint32 count, uint32 color)
{
    uint32 left = 0, right = (uint32)count - 1, cursor, first, last_packet;
    uint32 a, b, packet, link;
    FUNCTION_MARKER(0x80035328, "GTMENU.EXE");
    color ^= 0x29000000;
    cursor = r_u32(0x800A8434);
    a = r_u32(vertices);
    b = r_u32(vertices + right * 4);
    first = cursor;
    last_packet = cursor;
    if (count >= 4 && (sint32)left < (sint32)right) {
        do {
            packet = cursor;
            cursor += 24;
            w_u8(packet, cursor);
            w_u8(packet + 1, cursor >> 8);
            w_u8(packet + 2, cursor >> 16);
            ++left;
            --right;
            w_u8(packet + 3, 5);
            w_u32(packet + 4, color);
            w_u32(packet + 8, a);
            w_u32(packet + 12, b);
            a = r_u32(vertices + left * 4);
            last_packet = packet;
            b = r_u32(vertices + right * 4);
            w_u32(packet + 16, a);
            w_u32(packet + 20, b);
        } while ((sint32)left < (sint32)right);
    }
    if (left == right) {
        packet = cursor;
        color &= 0xF7FFFFFF;
        cursor += 20;
        w_u8(packet, cursor);
        w_u8(packet + 1, cursor >> 8);
        w_u8(packet + 2, cursor >> 16);
        ++left;
        w_u8(packet + 3, 4);
        w_u32(packet + 4, color);
        w_u32(packet + 8, a);
        w_u32(packet + 12, b);
        a = r_u32(vertices + left * 4);
        last_packet = packet;
        w_u32(packet + 16, a);
    }
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, first);
    w_u8(ordering + 1, first >> 8);
    w_u8(ordering + 2, first >> 16);
    w_u8(last_packet, link);
    w_u8(last_packet + 1, link >> 8);
    w_u8(last_packet + 2, link >> 16);
    w_u32(0x800A8434, cursor);
    return last_packet;
}

uint32 gtmenu_font_cache_acquire(uint32 context, uint16 code, uint32 x_out, uint32 y_out)
{
    uint32 node, parent = 0, mask = 1, side = 0, slot, base, x_offset, y_offset;
    sint32 x, y, right, bottom, next_x, next_y, pitch;
    FUNCTION_MARKER(0x80035430, "GTMENU.EXE");
    node = r_u32(context + 8);
    while (node) {
        if (code == r_u16(node + 12)) {
            slot = r_u16(node + 8);
            w_u32(x_out, slot);
            slot = r_u16(node + 10);
            w_u32(y_out, slot);
            return 0;
        }
        side = code & mask;
        parent = node;
        node = r_u32(node + ((side & 0xFFFF) ? 4 : 0));
        mask <<= 1;
    }
    x_offset = (r_s16(context + 16) & 0x3F) * 4;
    y = r_s16(context + 30);
    x = r_s16(context + 28);
    right = r_s16(context + 32);
    bottom = r_s16(context + 34);
    y_offset = r_u8(context + 18);
    next_x = x + 16;
    if (right < next_x || bottom < y + 16) {
        w_u32(x_out, x_offset);
        w_u32(y_out, y_offset);
        return 0;
    }
    next_y = y;
    if (next_x >= right) {
        next_x = 0;
        next_y += 16;
    }
    w_u16(context + 28, next_x);
    w_u16(context + 30, next_y);
    w_u32(x_out, (uint32)x + x_offset);
    w_u32(y_out, (uint32)y + y_offset);
    slot = r_u32(context + 12);
    w_u32(context + 12, slot + 16);
    if (parent) w_u32(parent + ((side & 0xFFFF) ? 4 : 0), slot);
    else w_u32(context + 8, slot);
    w_u16(slot + 8, (uint32)x + x_offset);
    w_u32(slot, 0);
    w_u32(slot + 4, 0);
    w_u16(slot + 12, code);
    w_u16(slot + 10, (uint32)y + y_offset);
    pitch = r_s16(context + 44);
    base = r_u32(context);
    return (uint32)(x >> 1) + base + (uint32)(y * pitch) * 4;
}

uint32 gtmenu_layout_glyph_draw(uint16 code, uint32 ordering, uint32 position, uint32 size, uint32 shade)
{
    GT_LOCAL_STACK_FRAME frame;
    const uint8 *bitmap;
    uint32 context, pixels, colors, offset = 0, page, command, uv, tex_x, tex_y;
    uint32 packet, link, x, width, y, height, right, bottom;
    sint32 pitch;
    FUNCTION_MARKER(0x8002C96C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8002C96C);
    context = gtmenu_text_surface_address();
    pixels = gtmenu_font_cache_acquire(context, code, frame.address, frame.address + 4);
    if (pixels) {
        bitmap = Krom2RawAdd2(code);
        colors = r_u32(context + 40);
        pitch = r_s16(context + 44);
        gtmenu_font_bitmap_unpack(pixels, bitmap, colors, pitch);
    }
    if (r_u32(0x800A79DC) & 0x20000) offset = r_u32(0x800A7A2C);
    page = r_u32(context + 24);
    command = 0x2C000000;
    if (shade == 1) {
        command = 0x2E000000;
        page &= 0xFF9FFFFF;
    }
    x = (sint16)position;
    width = (sint16)size;
    height = (size >> 16) - 1;
    y = position >> 16;
    width -= 1;
    colors = r_u32(context + 36);
    tex_y = r_u32(frame.address + 4);
    command = (command | colors) & 0xFF000000;
    colors = r_u32(0x800A7A00);
    command |= colors;
    tex_x = r_u32(frame.address);
    uv = ((tex_y & 0xFF) << 8) | (tex_x & 0xFF);
    page += (tex_x & 0x1F00) << 8;
    packet = r_u32(0x800A8434);
    page += (tex_y & 0x100) << 12;
    w_u32(0x800A8434, packet + 40);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u8(packet + 3, 9);
    w_u16(packet + 22, page >> 16);
    right = x + width;
    w_u16(packet + 8, x + offset);
    w_u16(packet + 10, y);
    w_u16(packet + 18, y);
    bottom = y + height;
    w_u16(packet + 16, right + offset);
    w_u16(packet + 20, uv + 15);
    w_u16(packet + 12, uv);
    w_u32(packet + 4, command);
    w_u16(packet + 14, page);
    w_u16(packet + 24, x);
    w_u16(packet + 26, bottom);
    w_u16(packet + 32, right);
    w_u16(packet + 34, bottom);
    w_u16(packet + 28, uv + 0xF00);
    w_u16(packet + 36, uv + 0xF0F);
    gtmenu_local_stack_pop(frame);
    return packet;
}

uint32 gtmenu_transmission_summary(uint32 transmission, uint32 curve, uint32 values)
{
    uint32 count, gear, rpm;
    sint32 divided, ratio, quotient, scaled;
    sint64 product;
    FUNCTION_MARKER(0x8002D764, "GTMENU.EXE");
    count = r_u8(transmission + 2);
    w_u32(values, count);
    for (gear = 1; gear <= count; ++gear) {
        rpm = gear == 1 ? 1000 : r_u16(transmission + gear * 2 + 20);
        w_u16(values + gear * 2 + 2, rpm);
        divided = (rpm << 12) / 60;
        ratio = r_s32(transmission + gear * 4 + 68);
        quotient = (uint32)gtmenu_signed_divide64((sint64)divided * 4096, ratio, 0x8002D830);
        product = (sint64)quotient * 9164;
        scaled = (uint32)(product / 4096);
        w_u16(values + gear * 2 + 30, scaled >> 12);
        rpm = gear == count ? r_u16(curve + 140) : r_u16(transmission + gear * 2 + 10);
        w_u16(values + gear * 2 + 16, rpm);
        divided = (rpm << 12) / 60;
        ratio = r_s32(transmission + gear * 4 + 68);
        quotient = (uint32)gtmenu_signed_divide64((sint64)divided * 4096, ratio, 0x8002D8EC);
        product = (sint64)quotient * 9164;
        scaled = (uint32)(product / 4096);
        w_u16(values + gear * 2 + 44, scaled >> 12);
    }
    return count != 0;
}

uint32 gtmenu_specification_gears(uint32 specification, uint32 values)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 curve, result;
    FUNCTION_MARKER(0x8002D964, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(336, 0x8002D964);
    curve = frame.address + 160;
    gtmenu_engine_curve_initialize(specification, curve, 0);
    gtmenu_transmission_build(specification, frame.address, curve);
    result = gtmenu_transmission_summary(frame.address, curve, values);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_stats_build(uint32 specification, uint32 values, uint32 detail)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 curve, result;
    FUNCTION_MARKER(0x8002D9CC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(336, 0x8002D9CC);
    curve = frame.address + 160;
    gtmenu_engine_curve_initialize(specification, curve, 0);
    gtmenu_transmission_build(specification, frame.address, curve);
    gtmenu_engine_curve_summary(curve, detail);
    gtmenu_transmission_summary(frame.address, curve, values);
    result = r_u16(detail);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_transmission_build(uint32 specification, uint32 transmission, uint32 curve)
{
    uint32 kind, mode, index, value, first, second, rpm, limit;
    sint32 coefficient, ratio, diameter, radius, inverse_front, inverse_rear, friction;
    sint32 factor = 0, denominator, maximum = 0, sample, quotient, scaled;
    sint64 product;
    FUNCTION_MARKER(0x8002CDF0, "GTMENU.EXE");
    kind = r_u8(specification + 138);
    if (kind == 0 || kind == 3) {
        w_u8(transmission, 0);
        w_u8(transmission + 3, 1);
        w_u32(transmission + 4, 0);
    } else if (kind == 1) {
        w_u8(transmission, 1);
        w_u8(transmission + 3, 0);
        w_u32(transmission + 4, 0);
    } else if (kind == 2) {
        mode = r_u8(specification + 81);
        if (mode == 0) {
            w_u8(transmission, 2);
            w_u8(transmission + 3, 0);
        } else if (mode < 3) {
            w_u8(transmission, 3);
            w_u8(transmission + 3, 1);
        } else if (mode == 3) {
            w_u8(transmission, 4);
            w_u8(transmission + 3, 1);
        }
        value = r_u8(specification + 281);
        w_u32(transmission + 4, (value << 12) / 100);
    }
    value = r_u8(specification + 42);
    w_u8(transmission + 2, value);
    for (index = 0; index <= r_u8(transmission + 2); ++index) {
        ratio = r_s16(specification + index * 2 + 24);
        coefficient = r_s16(specification + 40);
        product = (sint64)ratio * coefficient * 4096;
        value = gtmenu_signed_divide64(product, 1000000, 0x8002CFB0);
        w_u32(transmission + index * 4 + 36, value);
    }
    value = r_u8(specification + 104);
    diameter = (40960 * value + 20480) / 1000;
    w_u16(transmission + 108, diameter);
    first = r_u8(specification + 102);
    second = r_u8(specification + 106);
    radius = 1040384 * first / 20000 + diameter * 5 * second / 100;
    w_u16(transmission + 100, radius);
    radius = r_s16(transmission + 100);
    if (!radius) gt_draft_exception(7);
    inverse_front = 16777216 / radius;
    value = r_u8(specification + 105);
    diameter = (40960 * value + 20480) / 1000;
    w_u16(transmission + 110, diameter);
    second = r_u8(specification + 107);
    first = r_u8(specification + 103);
    radius = 1040384 * first / 20000 + diameter * 5 * second / 100;
    w_u16(transmission + 102, radius);
    radius = r_s16(transmission + 102);
    if (!radius) gt_draft_exception(7);
    inverse_rear = 16777216 / radius;
    w_u16(transmission + 104, inverse_front);
    w_u16(transmission + 106, inverse_rear);
    for (index = 0; index < 2; ++index) {
        friction = r_u8(specification + 100 + index) - 12;
        if (friction <= 0) friction = 1;
        w_u16(transmission + index * 2 + 120, friction);
    }
    value = r_u8(specification + 43) * 4;
    if (!value) gt_draft_exception(7);
    value = 267059200 / value;
    kind = r_u8(transmission);
    w_u32(transmission + 112, value);
    if (kind == 0 || kind == 3 || kind == 4) factor = r_s16(transmission + 122);
    else if (kind == 1 || kind == 2) factor = r_s16(transmission + 120);
    factor = (uint32)factor << 10;
    for (index = 0; index <= r_u8(transmission + 2); ++index) {
        value = r_u8(specification + 43);
        ratio = r_s32(transmission + index * 4 + 36);
        product = (sint64)(value << 12) * ratio;
        scaled = (uint32)(product / 4096);
        denominator = (uint32)factor + (uint32)(scaled / 100);
        value = gtmenu_signed_divide64(16777216, denominator, 0x8002D2C8);
        w_u32(transmission + index * 4 + 124, value);
    }
    for (index = 0; index < 2; ++index) {
        radius = r_s16(transmission + index * 2 + 104);
        friction = r_s16(transmission + index * 2 + 120);
        scaled = (sint64)radius * radius / 4096;
        product = (sint64)friction * scaled;
        w_u16(transmission + index * 2 + 156, product / 4096);
    }
    value = r_u8(specification + 93);
    w_u16(transmission + 8, value * 10);
    value = r_u8(specification + 49);
    w_u16(transmission + 10, value * 100);
    for (index = 0; (sint32)index < r_s16(curve); ++index) {
        sample = r_s32(curve + index * 4 + 12);
        sample = gtmenu_curve_evaluate(curve, sample);
        if (maximum < sample) maximum = sample;
    }
    scaled = (uint32)maximum * 15;
    w_u16(transmission + 118, scaled / 10);
    value = r_u8(specification + 139);
    w_u16(transmission + 116, 160564 * value / 4096);
    for (index = 0; index <= r_u8(transmission + 2); ++index) {
        mode = r_u8(transmission);
        if (mode == 0 || mode == 3 || mode == 4) {
            ratio = r_s32(transmission + index * 4 + 36);
            radius = r_s16(transmission + 102);
        } else if (mode == 1 || mode == 2) {
            ratio = r_s32(transmission + index * 4 + 36);
            radius = r_s16(transmission + 100);
        } else continue;
        denominator = radius * 25736 / 4096;
        value = gtmenu_signed_divide64((sint64)ratio * 4096, denominator, 0x8002D5C0);
        w_u32(transmission + index * 4 + 68, value);
    }
    rpm = r_u16(transmission + 8);
    limit = r_u16(curve + 142);
    if ((sint32)limit >= (sint32)rpm - 500) {
        limit = r_u16(curve + 142);
        w_u16(transmission + 8, limit + 500);
    }
    limit = r_u16(curve + 140);
    rpm = r_u16(transmission + 10);
    if ((sint32)rpm >= (sint32)limit - 500) {
        limit = r_u16(curve + 140);
        w_u16(transmission + 10, limit - 500);
    }
    for (index = 1; index < r_u8(transmission + 2); ++index) {
        rpm = r_u16(transmission + 10);
        w_u16(transmission + index * 2 + 10, rpm);
        rpm = r_u16(transmission + 10);
        first = r_u32(transmission + index * 4 + 40);
        second = r_u32(transmission + index * 4 + 36);
        quotient = (uint32)gtmenu_signed_divide64((sint64)(sint32)first * 4096, (sint32)second, 0x8002D6A8);
        product = (sint64)((rpm << 12) / 60) * quotient;
        value = product / 4096;
        scaled = value * 60;
        scaled /= 4096;
        rpm = r_u16(transmission + 8);
        if ((sint32)(rpm + 1000) < scaled) {
            w_u16(transmission + index * 2 + 22, (uint32)scaled - 500);
        } else {
            rpm = r_u16(transmission + 8);
            w_u16(transmission + index * 2 + 22, rpm + 500);
        }
    }
    return 0;
}

sint32 gtmenu_curve_evaluate(uint32 curve, sint32 position)
{
    uint32 last, xs, ys, first_x, last_x, x0, x1, y0, y1, lower = 0, upper, middle, scaled;
    sint64 product;
    FUNCTION_MARKER(0x8002DA50, "GTMENU.EXE");
    last = (r_u16(curve) - 1) & 0xFFFF;
    xs = r_u32(curve + 4);
    first_x = r_u32(xs);
    last_x = r_u32(xs + last * 4);
    if ((sint32)first_x >= position) {
        ys = r_u32(curve + 8);
        x1 = r_u32(xs + 4);
        y0 = r_u32(ys);
        y1 = r_u32(ys + 4);
        if ((uint32)position == first_x) return y0;
        if (!r_s8(curve + 2)) return y0;
        product = (sint64)(sint32)(y0 - y1) * (sint32)((uint32)position - x1);
        scaled = product / 4096;
        scaled = gtmenu_signed_divide64((sint64)(sint32)scaled * 4096, (sint32)(first_x - x1), 0x8002DB14);
        return y1 + scaled;
    }
    if (position >= (sint32)last_x) {
        xs = r_u32(curve + 4);
        ys = r_u32(curve + 8);
        x0 = r_u32(xs + last * 4 - 4);
        y0 = r_u32(ys + last * 4 - 4);
        y1 = r_u32(ys + last * 4);
        if ((uint32)position == last_x) return y1;
        if (!r_s8(curve + 3)) return y1;
        product = (sint64)(sint32)(y1 - y0) * (sint32)((uint32)position - x0);
        scaled = product / 4096;
        scaled = gtmenu_signed_divide64((sint64)(sint32)scaled * 4096, (sint32)(last_x - x0), 0x8002DBBC);
        return y0 + scaled;
    }
    upper = last;
    if (last >= 4) {
        xs = r_u32(curve + 4);
        do {
            middle = (upper + lower) >> 1;
            if (position < r_s32(xs + middle * 4)) upper = middle & 0xFFFF;
            else {
                x0 = r_u32(curve + 4);
                x0 = r_u32(x0 + (middle & 0xFFFF) * 4);
                if ((uint32)position == x0) {
                    ys = r_u32(curve + 8);
                    return r_s32(ys + (middle & 0xFFFF) * 4);
                }
                lower = middle & 0xFFFF;
            }
        } while ((sint32)(upper - lower) >= 4);
    }
    if (lower == upper) {
        ys = r_u32(curve + 8);
        return r_s32(ys + lower * 4);
    }
    if (lower < upper) {
        xs = r_u32(curve + 4);
        while (1) {
            if (position < r_s32(xs + lower * 4)) break;
            lower = (lower + 1) & 0xFFFF;
            if (lower >= upper) break;
        }
    }
    xs = r_u32(curve + 4);
    ys = r_u32(curve + 8);
    x0 = r_u32(xs + lower * 4 - 4);
    x1 = r_u32(xs + lower * 4);
    y0 = r_u32(ys + lower * 4 - 4);
    y1 = r_u32(ys + lower * 4);
    if ((uint32)position == x0) return y0;
    product = (sint64)(sint32)((uint32)position - x0) * (sint32)y1;
    scaled = product / 4096;
    product = (sint64)(sint32)(x1 - (uint32)position) * (sint32)y0;
    scaled += (uint32)(product / 4096);
    return gtmenu_signed_divide64((sint64)(sint32)scaled * 4096, (sint32)(x1 - x0), 0x8002DD7C);
}

uint32 gtmenu_engine_curve_initialize(uint32 specification, uint32 curve, uint32 enabled)
{
    uint32 value, count, offset, index, result;
    sint32 scaled, ratio, numerator, denominator, torque;
    sint16 rpm;
    sint64 product;
    FUNCTION_MARKER(0x8002E1B0, "GTMENU.EXE");
    value = r_u8(specification + 19);
    w_u16(curve + 142, value * 10);
    if (!r_u8(specification + 50)) {
        count = r_u8(specification + 86);
        value = r_u8(specification + count + 51);
        w_u8(specification + 50, value);
    }
    value = r_u8(specification + 50);
    w_u16(curve + 140, value * 100);
    count = r_u8(specification + 86);
    offset = specification + count * 2;
    if (!r_u16(offset + 316)) {
        value = r_u16(offset + 314);
        w_u16(offset + 316, value);
    }
    for (index = 0; index < r_u8(specification + 86); ++index) {
        value = r_u8(specification + index + 52);
        w_u32(curve + index * 4 + 12, 409600 * value / 60);
        value = r_u16(specification + index * 2 + 318);
        scaled = (sint32)(160564 * value) / 4096;
        value = r_u8(specification + 48);
        scaled = (uint32)scaled * value;
        w_u32(curve + index * 4 + 76, scaled / 10000);
    }
    count = r_u8(specification + 86);
    w_u32(curve + 4, curve + 12);
    w_u8(curve + 2, 0);
    w_u8(curve + 3, 0);
    w_u32(curve + 8, curve + 76);
    w_u16(curve, count);
    value = r_u8(specification + 44);
    scaled = 160564 * value / 4096;
    count = r_u8(specification + 86);
    denominator = r_s32(curve + count * 4 + 8);
    value = gtmenu_signed_divide64((sint64)scaled * 4096, denominator, 0x8002E3E4);
    w_u32(curve + 144, value);
    gtmenu_engine_boost_initialize(specification, curve, enabled);
    result = r_s16(curve);
    index = 0;
    while ((sint32)index < (sint32)result) {
        rpm = r_u8(specification + index + 52) * 100;
        ratio = gtmenu_engine_boost_limit(curve, rpm, rpm);
        denominator = gtmenu_engine_boost_curve(curve, rpm, rpm);
        numerator = (uint32)ratio << 12;
        if (!denominator) gt_draft_exception(7);
        if (denominator == -1 && numerator == (sint32)0x80000000) gt_draft_exception(6);
        ratio = numerator / denominator;
        torque = r_s32(curve + index * 4 + 76);
        product = (sint64)ratio * torque;
        w_u32(curve + index * 4 + 76, product / 4096);
        result = r_s16(curve);
        ++index;
        if ((sint32)index >= (sint32)result) return specification + index;
    }
    return result;
}

uint32 gtmenu_engine_curve_summary(uint32 curve, uint32 detail)
{
    sint32 count, index, scaled, rpm, limit, value, torque, quotient, torque_base, power, high;
    sint32 best_power = 0, best_power_rpm = 0, best_torque = 0, best_torque_rpm = 0;
    sint64 product;
    uint32 sample = 0, address;
    FUNCTION_MARKER(0x8002E4F0, "GTMENU.EXE");
    count = r_s16(curve);
    w_u16(detail + 10, count);
    for (index = 0; index < count; ++index) {
        value = r_s32(curve + index * 4 + 12);
        scaled = (uint32)value * 60;
        rpm = scaled / 4096;
        rpm = ((rpm + 50) / 100) * 100;
        w_u16(detail + index * 2 + 12, rpm);
    }
    rpm = r_s16(detail + 12);
    limit = r_u16(curve + 140);
    while (limit >= rpm) {
        value = gtmenu_curve_evaluate(curve, rpm * 4096 / 60);
        scaled = (uint32)value * 10;
        torque_base = scaled / 39;
        quotient = (uint32)gtmenu_signed_divide64((sint64)value * 4096, 40141, 0x8002E654);
        scaled = (uint32)torque_base * 72329;
        torque = scaled / 10000;
        product = (sint64)quotient * rpm;
        scaled = (uint32)(product / 4096);
        quotient = (uint32)gtmenu_signed_divide64((sint64)scaled * 4096, 2864, 0x8002E6F0);
        scaled = (uint32)quotient * 1000;
        product = (sint64)scaled * 0x2050C9F9;
        high = (uint64)product >> 32;
        power = (uint32)(high >> 7) - (uint32)(scaled >> 31);
        address = detail + sample * 2;
        if (r_s16(address + 12) == rpm) {
            ++sample;
            w_u16(address + 44, power);
            w_u16(address + 76, torque);
        }
        if (best_torque < torque) {
            best_torque = torque;
            best_torque_rpm = rpm;
        }
        if (best_power < power) {
            best_power = power;
            best_power_rpm = rpm;
        }
        limit = r_u16(curve + 140);
        rpm += 100;
    }
    if (detail) {
        w_u16(detail, best_power);
        w_u16(detail + 2, best_power_rpm);
        w_u16(detail + 4, best_torque);
        w_u16(detail + 6, best_torque_rpm);
    }
    return 1;
}

sint32 gtmenu_engine_boost_curve(uint32 curve, sint16 first, sint16 second)
{
    uint32 index, square, total = 0, correction;
    sint32 amplitude, center, point, delta, scaled, coefficient;
    sint64 product;
    FUNCTION_MARKER(0x8002DDB4, "GTMENU.EXE");
    for (index = 0; index < 2; ++index) {
        amplitude = r_s16(curve + index * 2 + 152);
        if (!amplitude) continue;
        center = r_s16(curve + index * 2 + 148);
        point = index ? second : first;
        if (point >= center * 2) continue;
        delta = point - center;
        square = (uint32)delta * (uint32)delta;
        coefficient = r_s32(curve + index * 4 + 164);
        scaled = (sint32)square / 4096;
        product = (sint64)scaled * coefficient;
        correction = product / 4096;
        total += (uint32)amplitude - correction;
    }
    return total + 4096;
}

sint32 gtmenu_engine_boost_limit(uint32 curve, sint16 first, sint16 second)
{
    sint32 result, limit;
    FUNCTION_MARKER(0x8002DE74, "GTMENU.EXE");
    result = gtmenu_engine_boost_curve(curve, first, second);
    limit = r_s16(curve + 172) + 4096;
    if (limit < result) result = limit;
    return result;
}

uint32 gtmenu_engine_boost_initialize(uint32 specification, uint32 curve, uint32 enabled)
{
    uint32 value, result, first, second, third;
    sint32 amplitude, center, denominator;
    sint64 square;
    FUNCTION_MARKER(0x8002DEC8, "GTMENU.EXE");
    if (!r_u8(specification + 45)) {
        w_u16(curve + 172, 0);
    } else {
        value = r_u8(specification + 45);
        w_u16(curve + 172, (value << 12) / 10);
        if (!r_u8(specification + 376) || !r_u8(specification + 377) || !r_u8(specification + 378)) {
            w_u16(curve + 172, 0);
        }
    }
    result = r_s16(curve + 172);
    if (!result) {
        w_u16(curve + 152, 0);
        w_u32(curve + 156, 0);
        w_u32(curve + 164, 0);
        w_u16(curve + 148, 0);
        w_u16(curve + 154, 0);
        w_u32(curve + 160, 0);
        w_u32(curve + 168, 0);
        w_u16(curve + 150, 0);
        return result;
    }
    value = r_u8(specification + 377);
    w_u16(curve + 152, (value << 12) / 10);
    value = r_u8(specification + 378);
    w_u32(curve + 156, (value << 12) / 10);
    center = r_u8(specification + 376) * 100;
    amplitude = r_s16(curve + 152);
    w_u16(curve + 148, center);
    square = (sint64)center * center;
    denominator = square / 4096;
    result = gtmenu_signed_divide64((sint64)amplitude * 4096, denominator, 0x8002E064);
    w_u32(curve + 164, result);
    w_u16(curve + 154, 0);
    w_u32(curve + 160, 0);
    w_u16(curve + 150, 0);
    w_u32(curve + 168, 0);
    first = r_u8(specification + 380);
    second = r_u8(specification + 381);
    third = r_u8(specification + 379);
    result = first + second + third;
    if (!result || !first || !third || !second) return result;
    w_u16(curve + 154, (first << 12) / 10);
    value = r_u8(specification + 381);
    w_u32(curve + 160, (value << 12) / 10);
    center = r_u8(specification + 379) * 100;
    amplitude = r_s16(curve + 154);
    w_u16(curve + 150, center);
    square = (sint64)center * center;
    denominator = square / 4096;
    result = gtmenu_signed_divide64((sint64)amplitude * 4096, denominator, 0x8002E198);
    w_u32(curve + 168, result);
    return result;
}

uint32 gtmenu_engine_specification_summary(uint32 specification, uint32 detail)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x8002E7C0, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(176, 0x8002E7C0);
    gtmenu_engine_curve_initialize(specification, frame.address, 0);
    gtmenu_engine_curve_summary(frame.address, detail);
    result = r_u16(detail);
    gtmenu_local_stack_pop(frame);
    return result;
}

void gtmenu_voices_initialize(void)
{
    FUNCTION_MARKER(0x8002E7FC, "GTMENU.EXE");
    if (r_u32(0x80070BB8)) return;
    gtmenu_spu_initialize();
    w_u16(0x1F801D80, 0x3FFF);
    w_u16(0x1F801D82, 0x3FFF);
    SpuSetTransferMode(0);
    gtmenu_memory_fill(0x8009AC88, 0, 968);
    w_u8(0x8009AC88, 23);
    gtmenu_voice_rate_set(60);
    w_u32(0x80070BB8, 1);
}

void gtmenu_voices_update(void)
{
    FUNCTION_MARKER(0x8002E8AC, "GTMENU.EXE");
    if (r_u32(0x80070BB8)) gtmenu_voices_commit();
}

uint32 gtmenu_voices_commit(void)
{
    uint32 value, voice, index, bit = 1, changed = 0, key_off = 0, key_on = 0;
    uint32 owner, length, cursor, elapsed, mask, device, envelope, pitch;
    uint16 left, right, adsr1, adsr2, start;
    uint8 dirty, pending;
    FUNCTION_MARKER(0x8002E8D8, "GTMENU.EXE");
    value = r_u32(0x80070BC8);
    w_u32(0x80070BC8, value + 1);
    voice = 0x8009AC90;
    for (index = 0; index < 24; ++index) {
        if (r_u8(voice + 4) != 2) {
            if (r_u8(voice + 1) && !r_u8(voice + 6)) {
                envelope = r_u16(0x1F801C0C + index * 16);
                if (envelope) {
                    length = r_u32(voice + 28);
                    cursor = r_u32(voice + 32);
                }
                if (!envelope || length < cursor) {
                    value = r_u8(voice);
                    w_u8(voice + 1, 0);
                    w_u8(voice + 7, 1);
                    if (value == 1) {
                        owner = r_u32(voice + 20);
                        if (owner) w_u8(owner, 0xFF);
                        w_u32(voice + 20, 0);
                        w_u8(voice, 0);
                    }
                }
            }
            if (r_u8(voice + 7)) {
                if (r_u8(voice + 2)) {
                    key_off |= bit;
                    w_u8(voice + 2, 0);
                }
                w_u8(voice + 7, 0);
            }
            dirty = r_u8(voice + 5);
            pending = r_u8(voice + 6);
            if (dirty) changed |= bit;
            if (pending) {
                value = r_u8(voice + 2);
                key_on |= bit;
                if (value) key_off |= bit;
                w_u8(voice + 2, 1);
                w_u8(voice + 1, 1);
                w_u8(voice + 6, 0);
            }
        }
        value = r_u8(voice + 1);
        bit <<= 1;
        if (value) {
            elapsed = r_u32(voice + 24);
            length = r_u32(voice + 28);
            w_u32(voice + 24, elapsed + 1);
            if (length) {
                pitch = r_u16(0x1F801C04 + index * 16);
                cursor = r_u32(voice + 32);
                w_u32(voice + 32, cursor + pitch);
            }
        }
        voice += 40;
    }
    voice = 0x8009AC90;
    if (key_off) {
        w_u16(0x1F801D8C, key_off);
        w_u16(0x1F801D8E, key_off >> 16);
    }
    device = 0x1F801C00;
    while (changed) {
        bit = changed & 1;
        changed >>= 1;
        if (bit) {
            dirty = r_u8(voice + 5);
            w_u8(voice + 5, 0);
            if (dirty & 1) {
                value = r_u16(voice + 12);
                w_u16(device + 4, value);
            }
            if (dirty & 2) {
                left = r_s16(voice + 8) & 0x7FFF;
                right = r_s16(voice + 10) & 0x7FFF;
                w_u16(device, left);
                w_u16(device + 2, right);
            }
            if (dirty & 4) {
                adsr1 = r_u16(voice + 16);
                adsr2 = r_u16(voice + 18);
                start = r_u16(voice + 14);
                w_u16(device + 8, adsr1);
                w_u16(device + 10, adsr2);
                w_u16(device + 6, start);
            }
        }
        device += 16;
        voice += 40;
    }
    if (key_on) {
        w_u16(0x1F801D88, key_on);
        w_u16(0x1F801D8A, key_on >> 16);
    }
    value = r_u8(0x8009AC8A);
    mask = 0;
    voice = 0x8009AC90;
    if (value) {
        bit = 1;
        for (index = 0; index < 24; ++index) {
            if (r_u8(voice + 36)) mask |= bit;
            voice += 40;
            bit <<= 1;
        }
        w_u16(0x1F801D98, mask);
        w_u16(0x1F801D9A, mask >> 16);
        w_u8(0x8009AC8A, 0);
    }
    return voice;
}

void gtmenu_sequence_note_start(uint32 sequence, uint32 track, sint32 note, uint32 velocity, uint32 delay)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 bank, table, program, cursor, layers, layer, sample, value, slot, index, tries, due, pitch, free_slot;
    sint32 gain, scaled, pan, bend, depth;
    uint16 level, velocity_gain;
    FUNCTION_MARKER(0x8002EB9C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x8002EB9C);
    bank = r_u32(sequence + 24);
    value = r_u8(track + 17);
    table = r_u32(bank + 36);
    program = r_u32(table + value * 4);
    level = r_u16(track + 10);
    gain = r_s16(program + 2);
    velocity_gain = r_u16(0x80055924 + velocity * 2);
    scaled = (sint32)((uint32)level * (uint32)gain) >> 14;
    scaled = (sint32)((uint32)velocity_gain * (uint32)scaled) >> 14;
    pan = r_u8(track + 16);
    pan += r_u8(program + 1);
    layers = r_u8(program);
    cursor = program;
    for (layer = 0; layer < layers; ++layer, ++cursor) {
        value = r_u8(cursor + 4);
        table = r_u32(bank + 28);
        sample = table + value * 20;
        if (note < r_u8(sample + 12)) continue;
        if (r_u8(sample + 13) < note) continue;
        index = r_u8(0x8009B060) + 1;
        slot = 0x8009B070 + index * 28;
        tries = 0;
        free_slot = 0;
        while (1) {
            if (index == 25) {
                index = 0;
                slot = 0x8009B070;
            }
            ++tries;
            if (r_s8(slot + 27) < 0) {
                free_slot = 1;
                break;
            }
            ++index;
            if (tries == 25) break;
            slot += 28;
        }
        if (!free_slot) continue;
        gain = r_s16(sample + 2);
        value = r_u8(sample + 9);
        bend = r_s16(track + 12);
        depth = (sint32)value + pan - 128;
        pitch = (uint32)note << 8;
        w_u8(0x8009B060, index);
        gain = (sint32)((uint32)gain * (uint32)scaled) >> 14;
        if (depth < 0) depth = 0;
        else if (depth >= 128) depth = 127;
        w_u32(slot, sequence);
        w_u32(slot + 4, track);
        w_u32(slot + 12, sample);
        w_u32(slot + 8, program);
        due = r_u32(sequence + 20);
        w_u16(slot + 22, bend);
        w_u32(slot + 16, delay + due);
        w_u8(slot + 24, note);
        w_u8(slot + 25, velocity);
        value = r_u16(track + 10);
        w_u16(slot + 20, value);
        value = r_u8(track + 16);
        w_u8(slot + 26, value);
        value = r_u32(sample + 16);
        w_u32(frame.address + 8, value);
        value = r_u16(sample);
        w_u16(frame.address + 6, value);
        value = r_u8(sample + 8);
        w_u8(frame.address + 14, value);
        value = r_u16(sample + 6);
        w_u16(frame.address + 12, value);
        value = r_u8(sample + 10);
        w_u8(frame.address + 15, value);
        if (bend) {
            value = r_u8(sample + (bend > 0 ? 15 : 14));
            pitch += (uint32)((bend * (sint32)value) >> 5);
        }
        value = r_u16(sample + 4);
        value = gtmenu_voice_pitch(pitch, value);
        w_u16(frame.address + 4, value);
        gtmenu_voice_pan(frame.address, gain, depth);
        gtmenu_voice_start(slot + 27, frame.address);
        layers = r_u8(program);
    }
    gtmenu_local_stack_pop(frame);
}

void gtmenu_sequence_tracks_update(uint32 sequence)
{
    uint32 channel, track = sequence + 40, stream, command, duration, velocity, value, delay;
    uint32 part, count, level, tempo, amount;
    sint32 depth;
    uint32 elapsed, step;
    FUNCTION_MARKER(0x8002EE8C, "GTMENU.EXE");
    for (channel = 0; channel < 16; ++channel, track += 40) {
        stream = r_u32(track);
        if (!stream) continue;
        elapsed = r_u32(track + 4);
        step = r_u32(sequence + 20);
        elapsed -= step;
        w_u32(track + 4, elapsed);
        while ((sint32)elapsed <= 0) {
            command = r_u8(stream);
            ++stream;
            if (command & 0x80) {
                duration = r_u8(stream + 1);
                velocity = r_u8(stream);
                stream += 2;
                if (duration & 0x80) {
                    duration &= 0x7F;
                    do {
                        part = r_u8(stream);
                        duration = (part & 0x7F) + (duration << 7);
                        ++stream;
                    } while (part & 0x80);
                }
                step = r_u32(sequence + 20);
                delay = duration + step + elapsed;
                gtmenu_sequence_note_start(sequence, track, command & 0x7F, velocity, delay);
                elapsed = r_u32(track + 4);
            } else if (command & 0x40) {
                value = r_u8(stream);
                value |= (command & 0x3F) << 8;
                elapsed = r_u32(track + 4);
                w_u16(track + 12, value - 0x2000);
                ++stream;
            } else if (command & 0x20) {
                depth = r_s8(track + 18) + 1;
                w_u8(track + (uint32)depth + 36, (command & 0x1F) + 1);
                w_u32(track + (uint32)depth * 4 + 20, stream);
                elapsed = r_u32(track + 4);
                w_u8(track + 18, depth);
            } else {
                switch (command) {
                case 0:
                    stream = 0;
                    goto track_done;
                case 1:
                    depth = r_s8(track + 18) + 1;
                    value = r_u8(stream);
                    ++stream;
                    w_u8(track + (uint32)depth + 36, value);
                    w_u32(track + (uint32)depth * 4 + 20, stream);
                    elapsed = r_u32(track + 4);
                    w_u8(track + 18, depth);
                    break;
                case 2:
                    depth = r_s8(track + 18);
                    count = r_u8(track + (uint32)depth + 36);
                    if (!count) {
                        elapsed = r_u32(track + 4);
                        w_u8(track + 18, depth - 1);
                    } else {
                        if (count != 255) {
                            w_u8(track + (uint32)depth + 36, count - 1);
                            depth = r_s8(track + 18);
                            elapsed = r_u32(track + 4);
                        }
                        stream = r_u32(track + (uint32)depth * 4 + 20);
                    }
                    break;
                case 3:
                    value = r_u8(stream);
                    elapsed = r_u32(track + 4);
                    ++stream;
                    w_u8(track + 17, value);
                    break;
                case 4:
                    value = r_u8(stream);
                    elapsed = r_u32(track + 4);
                    level = r_u16(0x80055924 + value * 2);
                    ++stream;
                    w_u16(track + 8, level);
                    value = r_u16(sequence + 6);
                    value *= level;
                    w_u16(track + 10, (sint32)value >> 14);
                    break;
                case 5:
                    value = r_u8(stream);
                    elapsed = r_u32(track + 4);
                    ++stream;
                    w_u8(track + 16, value);
                    break;
                case 6:
                    tempo = r_u8(stream);
                    value = r_u8(stream + 2);
                    part = r_u8(stream + 1);
                    tempo = value | (tempo << 16) | (part << 8);
                    w_u32(sequence + 8, tempo);
                    elapsed = r_u32(track + 4);
                    stream += 3;
                    break;
                default:
                    break;
                }
            }
            amount = r_u8(stream);
            ++stream;
            if (amount & 0x80) {
                amount &= 0x7F;
                do {
                    part = r_u8(stream);
                    amount = (part & 0x7F) + (amount << 7);
                    ++stream;
                } while (part & 0x80);
            }
            elapsed += amount;
            w_u32(track + 4, elapsed);
        }
track_done:
        w_u32(track, stream);
    }
}

sint32 gtmenu_sequence_fade_step(uint32 state)
{
    uint32 remaining;
    sint32 carry, increment, value, accumulator, extra;
    FUNCTION_MARKER(0x8002FDF0, "GTMENU.EXE");
    remaining = r_u16(state);
    if (!remaining) return 0;
    carry = r_s16(state + 4);
    increment = r_s16(state + 2);
    w_u16(state, remaining - 1);
    if (carry) {
        value = r_s16(state + 6);
        accumulator = r_s16(state + 10);
        w_u16(state + 10, accumulator - value);
        if (value < 0) {
            accumulator = r_s16(state + 10);
            extra = r_s16(state + 8);
            carry = r_s16(state + 4);
            w_u16(state + 10, accumulator + extra);
            increment += carry;
        }
    }
    return increment;
}

void gtmenu_sequences_update(void)
{
    uint32 sequence, elapsed, tempo, ticks, value;
    sint32 delta;
    FUNCTION_MARKER(0x8002F17C, "GTMENU.EXE");
    if (!r_u32(0x80070BB8)) return;
    if (!r_u32(0x80070BD8)) return;
    sequence = r_u32(0x8009B058);
    while (sequence) {
        elapsed = r_u32(sequence + 12);
        w_u32(sequence + 20, 0);
        if ((sint32)elapsed > 0) {
            tempo = r_u32(sequence + 8);
            do {
                ticks = r_u32(sequence + 20);
                elapsed -= tempo;
                w_u32(sequence + 20, ticks + 1);
                w_u32(sequence + 12, elapsed);
            } while ((sint32)elapsed > 0);
        }
        value = r_u32(sequence + 16);
        w_u32(sequence + 12, elapsed + value);
        gtmenu_sequence_tracks_update(sequence);
        if (r_u16(sequence + 28)) {
            delta = gtmenu_sequence_fade_step(sequence + 28);
            value = r_u16(sequence + 4);
            w_u16(sequence + 4, value + (uint32)delta);
            gtmenu_sequence_volume_apply(sequence);
        }
        sequence = r_u32(sequence);
    }
    if (r_u16(0x8009B064)) {
        delta = gtmenu_sequence_fade_step(0x8009B064);
        value = r_u16(0x8009B05E);
        w_u16(0x8009B05E, value + (uint32)delta);
        sequence = r_u32(0x8009B058);
        while (sequence) {
            gtmenu_sequence_volume_apply(sequence);
            sequence = r_u32(sequence);
        }
    }
    gtmenu_sequence_notes_update();
}

void gtmenu_sequence_notes_update(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 slot = 0x8009B070, index, owner, due, step, track, sample, program;
    uint32 note, reference, value, volume, old_volume, velocity, pitch;
    sint32 old_bend, bend, depth, pan, program_gain, sample_gain, gain;
    uint16 velocity_gain;
    FUNCTION_MARKER(0x8002F2B0, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8002F2B0);
    for (index = 0; index < 25; ++index, slot += 28) {
        if (r_s8(slot + 27) < 0) continue;
        owner = r_u32(slot);
        if (!owner) {
            gtmenu_voice_stop(slot + 27);
            gtmenu_voice_detach(slot + 27);
            continue;
        }
        due = r_u32(slot + 16);
        if ((sint32)due > 0) {
            step = r_u32(owner + 20);
            due -= step;
            w_u32(slot + 16, due);
            if ((sint32)due <= 0) gtmenu_voice_stop(slot + 27);
        }
        track = r_u32(slot + 4);
        old_bend = r_s16(slot + 22);
        bend = r_s16(track + 12);
        if (old_bend != bend) {
            sample = r_u32(slot + 12);
            depth = r_u8(sample + (bend > 0 ? 15 : 14));
            note = r_u8(slot + 24);
            reference = r_u16(sample + 4);
            pitch = (note << 8) + (uint32)((depth * bend) >> 5);
            value = gtmenu_voice_pitch(pitch, reference);
            gtmenu_voice_pitch_set(slot + 27, value & 0xFFFF);
            track = r_u32(slot + 4);
            bend = r_s16(track + 12);
            w_u16(slot + 22, bend);
        }
        volume = r_u16(track + 10);
        old_volume = r_u16(slot + 20);
        if (old_volume == volume) {
            value = r_u8(slot + 26);
            note = r_u8(track + 16);
            if (value == note) continue;
        }
        program = r_u32(slot + 8);
        velocity = r_u8(slot + 25);
        program_gain = r_s16(program + 2);
        velocity_gain = r_u16(0x80055924 + velocity * 2);
        sample = r_u32(slot + 12);
        pan = r_u8(track + 16);
        sample_gain = r_s16(sample + 2);
        depth = r_u8(sample + 9);
        value = r_u8(program + 1);
        pan += depth + (sint32)value - 128;
        gain = (sint32)(volume * (uint32)program_gain) >> 14;
        gain = (sint32)((uint32)velocity_gain * (uint32)gain) >> 14;
        gain = (sint32)((uint32)sample_gain * (uint32)gain) >> 14;
        if (pan < 0) pan = 0;
        else if (pan >= 128) pan = 127;
        gtmenu_voice_pan(frame.address, gain, pan);
        gtmenu_voice_volume_set(slot + 27, frame.address);
        track = r_u32(slot + 4);
        value = r_u16(track + 10);
        w_u16(slot + 20, value);
        value = r_u8(track + 16);
        w_u8(slot + 26, value);
    }
    gtmenu_local_stack_pop(frame);
}

uint32 gtmenu_voice_stop(uint32 handle)
{
    sint32 index;
    uint32 voice, owner;
    uint8 state;
    FUNCTION_MARKER(0x800300BC, "GTMENU.EXE");
    index = r_s8(handle);
    if (index < 0) return index;
    voice = 0x8009AC90 + (uint32)index * 40;
    state = r_u8(voice + 4);
    w_u8(voice + 4, 1);
    owner = r_u32(voice + 20);
    if (owner == handle) w_u16(voice + 6, 0x100);
    w_u8(voice + 4, state);
    return voice;
}

uint32 gtmenu_voice_detach(uint32 handle)
{
    sint32 index;
    uint32 voice, owner;
    uint8 state;
    FUNCTION_MARKER(0x8003010C, "GTMENU.EXE");
    index = r_s8(handle);
    if (index < 0) return index;
    voice = 0x8009AC90 + (uint32)index * 40;
    state = r_u8(voice + 4);
    w_u8(voice + 4, 1);
    owner = r_u32(voice + 20);
    if (owner == handle) {
        w_u8(handle, 0xFF);
        w_u32(voice + 20, 0);
    }
    w_u8(voice + 4, state);
    return voice;
}

uint32 gtmenu_voice_pitch_set(uint32 handle, uint16 pitch)
{
    sint32 index;
    uint32 voice, owner;
    uint8 state, dirty;
    FUNCTION_MARKER(0x8003016C, "GTMENU.EXE");
    index = r_s8(handle);
    if (index < 0) return index;
    voice = 0x8009AC90 + (uint32)index * 40;
    state = r_u8(voice + 4);
    w_u8(voice + 4, 1);
    owner = r_u32(voice + 20);
    if (owner == handle) {
        w_u8(voice + 4, 2);
        dirty = r_u8(voice + 5);
        w_u16(voice + 12, pitch);
        w_u8(voice + 5, dirty | 1);
    }
    w_u8(voice + 4, state);
    return voice;
}

uint32 gtmenu_voice_volume_set(uint32 handle, uint32 parameters)
{
    sint32 index;
    uint32 voice, owner, volume;
    uint8 state, dirty;
    FUNCTION_MARKER(0x800301CC, "GTMENU.EXE");
    index = r_s8(handle);
    if (index < 0) return index;
    voice = 0x8009AC90 + (uint32)index * 40;
    state = r_u8(voice + 4);
    w_u8(voice + 4, 1);
    owner = r_u32(voice + 20);
    volume = r_u32(parameters);
    if (owner == handle) {
        w_u8(voice + 4, 2);
        dirty = r_u8(voice + 5);
        w_u32(voice + 8, volume);
        w_u8(voice + 5, dirty | 2);
    }
    w_u8(voice + 4, state);
    return voice;
}

void gtmenu_sequences_initialize(void)
{
    uint32 index, slot;
    FUNCTION_MARKER(0x8002F53C, "GTMENU.EXE");
    if (r_u32(0x80070BD8)) return;
    gtmenu_memory_fill(0x8009B058, 0, 724);
    w_u32(0x8009B058, 0);
    w_u16(0x8009B05E, 0x3FFF);
    w_u8(0x8009B060, 0);
    w_u8(0x8009B08B, 0xFF);
    slot = 0x8009B08C;
    for (index = 1; index != 25; index += 4) {
        w_u8(slot + 55, 0xFF);
        w_u8(slot + 83, 0xFF);
        w_u8(slot + 111, 0xFF);
        slot += 112;
        w_u8(slot - 85, 0xFF);
    }
    gtmenu_sequence_rate_set(60);
    w_u32(0x80070BD8, 1);
}

uint32 gtmenu_reverb_preset_set(uint8 preset, uint32 delay, sint32 feedback)
{
    uint32 record, scaled, half, first, second, third, fourth, fifth, sixth;
    uint32 work, source, device, index, a, b, c, d, control;
    sint32 coefficient;
    FUNCTION_MARKER(0x8002F5EC, "GTMENU.EXE");
    if (preset == 7 || preset == 8) {
        record = 0x80070BFC + (uint32)preset * 64;
        scaled = (delay << 13) / 127;
        first = r_u16(record);
        second = r_u16(record + 2);
        third = r_u16(record + 26);
        half = scaled >> 1;
        fourth = r_u16(record + 34);
        fifth = r_u16(record + 56);
        sixth = r_u16(record + 58);
        w_u16(record + 20, scaled - first);
        w_u16(record + 22, half - second);
        w_u16(record + 24, third + half);
        w_u16(record + 32, fourth + half);
        w_u16(record + 52, fifth + half);
        w_u16(record + 54, sixth + half);
    }
    if (preset == 7) {
        coefficient = (uint32)feedback * 33024;
        w_u16(0x80070BFC + (uint32)preset * 64 + 14, coefficient / 127);
    }
    control = r_u16(0x1F801DAA);
    w_u16(0x1F801D84, 0);
    w_u16(0x1F801D86, 0);
    w_u16(0x1F801DAA, control & 0xFF7F);
    work = 0x80070BE8 + (uint32)preset * 2;
    first = r_u16(work);
    source = 0x80070BFC + (uint32)preset * 64;
    device = 0x1F801DC0;
    w_u16(0x1F801DA2, first);
    for (index = 0; index < 4; ++index) {
        a = r_u32(source);
        b = r_u32(source + 4);
        c = r_u32(source + 8);
        d = r_u32(source + 12);
        w_u16(device, a);
        w_u16(device + 2, a >> 16);
        w_u16(device + 6, b >> 16);
        w_u16(device + 10, c >> 16);
        w_u16(device + 14, d >> 16);
        source += 16;
        device += 16;
        w_u16(device - 12, b);
        w_u16(device - 8, c);
        w_u16(device - 4, d);
    }
    control = r_u16(0x1F801DAA);
    w_u16(0x1F801DAA, control | 0x80);
    w_u8(0x8009AC89, 1);
    first = r_u16(work);
    return (65536 - first) * 8;
}

uint32 gtmenu_reverb_enable(uint32 enabled)
{
    uint32 control;
    FUNCTION_MARKER(0x8002F79C, "GTMENU.EXE");
    control = r_u16(0x1F801DAA);
    control = (control & 0xFF7F) | ((enabled & 1) << 7);
    w_u16(0x1F801DAA, control);
    return control;
}

uint32 gtmenu_reverb_depth_set(sint16 left, sint16 right)
{
    FUNCTION_MARKER(0x8002F7CC, "GTMENU.EXE");
    w_u16(0x1F801D84, left);
    w_u16(0x1F801D86, right);
    return 0x1F801C00;
}

void gtmenu_voices_shutdown(void)
{
    FUNCTION_MARKER(0x8002F7EC, "GTMENU.EXE");
    if (!r_u32(0x80070BB8)) return;
    w_u32(0x80070BB8, 0);
    gtmenu_voices_stop_all();
    if (r_u8(0x8009AC89)) {
        gtmenu_reverb_preset_set(0, 0, 0);
        gtmenu_reverb_enable(0);
    }
    gtmenu_spu_shutdown();
}

uint32 gtmenu_voices_stop_all(void)
{
    uint32 saved, voice, index, owner, result;
    FUNCTION_MARKER(0x80030258, "GTMENU.EXE");
    saved = r_u32(0x80070BB8);
    w_u32(0x80070BB8, 0);
    voice = 0x8009AC90;
    for (index = 0; index < 24; ++index) {
        owner = r_u32(voice + 20);
        w_u8(voice + 5, 2);
        w_u32(voice + 8, 0);
        w_u8(voice + 7, 1);
        w_u8(voice + 6, 0);
        w_u8(voice + 4, 0);
        w_u8(voice, 0);
        if (owner) {
            w_u8(owner, 0xFF);
            w_u32(voice + 20, 0);
        }
        voice += 40;
    }
    result = gtmenu_voices_commit();
    w_u32(0x80070BB8, saved);
    return result;
}

uint32 gtmenu_sequence_volume_apply(uint32 sequence)
{
    uint32 master, gain, track, index, a, b, c, d, result;
    FUNCTION_MARKER(0x8002FC58, "GTMENU.EXE");
    master = r_u16(0x8009B05E);
    gain = r_u16(sequence + 4);
    result = (sint32)(master * gain) >> 14;
    w_u16(sequence + 6, result);
    track = sequence + 40;
    for (index = 0; index < 16; index += 4) {
        a = r_u16(track + 8);
        b = r_u16(track + 48);
        c = r_u16(track + 88);
        d = r_u16(track + 128);
        w_u16(track + 10, (a * result) >> 14);
        w_u16(track + 50, (b * result) >> 14);
        w_u16(track + 90, (c * result) >> 14);
        w_u16(track + 130, (d * result) >> 14);
        track += 160;
    }
    return result;
}

uint32 gtmenu_sequence_unlink(uint32 sequence)
{
    uint32 node, previous, next;
    FUNCTION_MARKER(0x8002FA7C, "GTMENU.EXE");
    node = r_u32(0x8009B058);
    previous = 0;
    while (node) {
        if (node == sequence) {
            next = r_u32(node);
            if (previous) w_u32(previous, next);
            else w_u32(0x8009B058, next);
            return previous;
        }
        previous = node;
        node = r_u32(node);
    }
    return previous;
}

sint32 gtmenu_sequence_fade_initialize(uint32 state, uint32 current, uint32 target, sint32 milliseconds)
{
    sint32 difference, magnitude, frames, increment, remainder, extra;
    uint32 rate;
    FUNCTION_MARKER(0x8002FD1C, "GTMENU.EXE");
    difference = target - current;
    if (!difference) {
        w_u16(state, 0);
        return 0;
    }
    magnitude = difference > 0 ? difference : 0u - (uint32)difference;
    rate = r_u16(0x8009B05C);
    frames = (sint32)(rate * (uint32)milliseconds) / 1000;
    if (!frames) frames = 1;
    if (frames == -1 && magnitude == (sint32)0x80000000) gt_draft_exception(6);
    increment = magnitude / frames;
    remainder = magnitude % frames;
    extra = remainder != 0;
    if (difference <= 0) {
        increment = 0u - (uint32)increment;
        extra = -extra;
    }
    w_u16(state + 2, increment);
    w_u16(state + 4, extra);
    w_u16(state + 6, remainder);
    w_u16(state + 8, frames);
    w_u16(state + 10, frames >> 1);
    w_u16(state, frames);
    return extra;
}

uint32 gtmenu_sequence_fade(uint32 sequence, uint16 target, sint32 milliseconds)
{
    uint32 current;
    FUNCTION_MARKER(0x8002FE54, "GTMENU.EXE");
    w_u16(sequence + 28, 0);
    current = r_u16(sequence + 4);
    return gtmenu_sequence_fade_initialize(sequence + 28, current, target, milliseconds);
}

uint32 gtmenu_sequence_stop(uint32 sequence)
{
    uint32 slot, index, owner;
    FUNCTION_MARKER(0x8002FEDC, "GTMENU.EXE");
    gtmenu_sequence_unlink(sequence);
    owner = r_u32(0x8009B070);
    if (sequence == owner) w_u32(0x8009B070, 0);
    slot = 0x8009B08C;
    for (index = 1; index < 25; ++index) {
        owner = r_u32(slot);
        if (sequence == owner) w_u32(slot, 0);
        slot += 28;
    }
    return slot;
}

uint32 gtmenu_sound_bank_relocate(uint32 archive, uint32 sample_address)
{
    uint32 previous, delta, programs, samples, table, count, index, offset, entry, value;
    FUNCTION_MARKER(0x8002F84C, "GTMENU.EXE");
    if (r_u32(archive) != 0x54534E49) return 0;
    previous = r_u32(archive + 4);
    programs = r_u32(archive + 20);
    delta = archive - previous;
    samples = r_u32(archive + 28);
    table = r_u32(archive + 36);
    count = r_u32(archive + 32);
    w_u32(archive + 4, archive);
    w_u32(archive + 20, programs + delta);
    w_u32(archive + 28, samples + delta);
    w_u32(archive + 36, table + delta);
    if (count) {
        index = 0;
        offset = 0;
        do {
            table = r_u32(archive + 36);
            entry = table + offset;
            value = r_u32(entry);
            w_u32(entry, value + delta);
            count = r_u32(archive + 32);
            ++index;
            offset += 4;
        } while (index < count);
    }
    if (sample_address) {
        previous = r_u32(archive + 12);
        count = r_u32(archive + 24);
        w_u32(archive + 12, sample_address);
        delta = (sample_address - previous) >> 3;
        if (count) {
            index = 0;
            offset = 0;
            do {
                samples = r_u32(archive + 28);
                entry = samples + offset;
                value = r_u16(entry);
                w_u16(entry, value + delta);
                count = r_u32(archive + 24);
                ++index;
                offset += 20;
            } while (index < count);
        }
    }
    return archive;
}

uint32 gtmenu_sequence_relocate(uint32 sequence)
{
    uint32 previous, count, delta, record, group, pointer, value, index;
    FUNCTION_MARKER(0x8002F98C, "GTMENU.EXE");
    if (r_u32(sequence) != 0x47514553) return 0;
    previous = r_u32(sequence + 4);
    count = r_u32(sequence + 8);
    w_u32(sequence + 4, sequence);
    delta = sequence - previous;
    if (count) {
        record = sequence;
        index = 0;
        do {
            for (group = 0; group < 64; group += 16) {
                pointer = record + group + 20;
                if (r_u32(pointer)) {
                    value = r_u32(pointer);
                    w_u32(pointer, value + delta);
                }
                pointer += 4;
                if (r_u32(pointer)) {
                    value = r_u32(pointer);
                    w_u32(pointer, value + delta);
                }
                pointer += 4;
                if (r_u32(pointer)) {
                    value = r_u32(pointer);
                    w_u32(pointer, value + delta);
                }
                pointer += 4;
                if (r_u32(pointer)) {
                    value = r_u32(pointer);
                    w_u32(pointer, value + delta);
                }
            }
            count = r_u32(sequence + 8);
            ++index;
            record += 72;
        } while (index < count);
    }
    return sequence;
}

uint32 gtmenu_sequence_initialize(uint32 sequence, uint32 data, uint32 bank)
{
    uint32 gain, master, rate, period, tempo, track, source, cursor, byte, elapsed, index;
    FUNCTION_MARKER(0x8002FAD8, "GTMENU.EXE");
    gtmenu_sequence_unlink(sequence);
    w_u32(sequence, 0);
    gain = r_u16(data);
    w_u16(sequence + 4, gain);
    master = r_u16(0x8009B05E);
    gain = r_u16(data);
    w_u16(sequence + 6, (sint32)(master * gain) >> 14);
    w_u32(sequence + 24, bank);
    rate = r_u16(0x8009B05C);
    if (!rate) gt_draft_exception(7);
    period = 480000000 / rate;
    w_u32(sequence + 16, period);
    w_u32(sequence + 12, period);
    tempo = r_u32(data + 4);
    w_u32(sequence + 8, tempo);
    track = sequence + 40;
    source = data;
    byte = 0;
    for (index = 0; index < 16; ++index) {
        cursor = r_u32(source + 8);
        w_u32(track, cursor);
        cursor = r_u32(track);
        byte = r_u8(cursor);
        w_u32(track, cursor + 1);
        w_u32(track + 4, byte);
        if (byte & 0x80) {
            w_u32(track + 4, byte & 0x7F);
            do {
                cursor = r_u32(track);
                elapsed = r_u32(track + 4);
                byte = r_u8(cursor);
                elapsed = (elapsed << 7) + (byte & 0x7F);
                w_u32(track, cursor + 1);
                w_u32(track + 4, elapsed);
            } while (byte & 0x80);
        }
        w_u16(track + 8, 0x4000);
        gain = r_u16(sequence + 6);
        w_u16(track + 12, 0);
        w_u8(track + 16, 0);
        w_u8(track + 17, 0);
        w_u8(track + 18, 0xFF);
        w_u16(track + 10, gain);
        track += 40;
        source += 4;
    }
    return byte;
}

uint32 gtmenu_sequence_link(uint32 sequence)
{
    uint32 head;
    FUNCTION_MARKER(0x8002FC3C, "GTMENU.EXE");
    head = r_u32(0x8009B058);
    w_u32(sequence, head);
    w_u32(0x8009B058, sequence);
    return 0x8009B058;
}

uint32 gtmenu_sequence_start(uint32 sequence)
{
    FUNCTION_MARKER(0x8002FCE4, "GTMENU.EXE");
    gtmenu_sequence_unlink(sequence);
    gtmenu_sequence_volume_apply(sequence);
    return gtmenu_sequence_link(sequence);
}

void gtmenu_sound_samples_upload(uint32 sample_address, uint32 data, uint32 bytes)
{
    uint32 count, span;
    FUNCTION_MARKER(0x8002F93C, "GTMENU.EXE");
    if (!bytes) return;
    SpuSetTransferStartAddr(sample_address & ~7);
    SpuSetTransferMode(0);
    count = bytes > 0x7EFF0 ? 0x7EFF0 : bytes;
    span = (count + 63) & ~63;
    SpuWrite(psx_addr(data, span), count);
    SpuIsTransferCompleted(1);
}

sint32 gtmenu_voice_allocate(uint32 priority)
{
    sint32 index, selected_index = 0;
    uint32 voice, count, selected = 0, owner, flags, duration;
    uint8 state, type, length;
    FUNCTION_MARKER(0x800302EC, "GTMENU.EXE");
    index = r_s8(0x8009AC88);
    voice = 0x8009AC90 + (uint32)index * 40;
    for (count = 0; count < 24; ++count) {
        voice += 40;
        if (index == 23) {
            voice = 0x8009AC90;
            index = -1;
        }
        state = r_u8(voice + 4);
        type = r_u8(voice);
        ++index;
        if (!state && !type) {
            w_u8(voice + 4, 1);
            type = r_u8(voice);
            if (!type) {
                w_u8(voice + 3, priority);
                w_u8(0x8009AC88, index);
                return index;
            }
            w_u8(voice + 4, 0);
        }
    }
    for (count = 0; count < 24; ++count) {
        voice += 40;
        if (index == 23) {
            voice = 0x8009AC90;
            index = -1;
        }
        state = r_u8(voice + 4);
        ++index;
        if (state) continue;
        w_u8(voice + 4, 1);
        flags = r_u32(voice);
        if (!(flags & 0xFF)) {
            w_u8(voice + 3, priority);
            w_u8(0x8009AC88, index);
            return index;
        }
        if ((flags & 0xFF) == 1 && (flags >> 24) >= priority) {
            duration = r_u32(voice + 24);
            length = (flags >> 16) & 0xFF;
            if (!length) length = r_u8(voice + 6);
            /* Original candidate comparisons converge on the same replacement */
            (void)duration;
            (void)length;
            if (selected) w_u8(selected + 4, 0);
            selected_index = index;
            selected = voice;
        } else {
            w_u8(voice + 4, 0);
        }
    }
    if (!selected) return -1;
    owner = r_u32(selected + 20);
    if (owner) {
        w_u32(selected + 20, 0);
        w_u8(owner, 0xFF);
    }
    w_u32(selected + 4, 0x01000001);
    w_u8(0x8009AC88, selected_index);
    return selected_index;
}

void gtmenu_voice_configure(uint32 index, uint32 parameters)
{
    uint32 voice, duration, rate, volume, pitch_start, adsr;
    sint8 flags;
    FUNCTION_MARKER(0x8003044C, "GTMENU.EXE");
    voice = 0x8009AC90 + index * 40;
    w_u8(voice + 4, 2);
    duration = r_u16(parameters + 12);
    rate = r_u16(0x8009AC8E);
    w_u8(voice + 1, 0);
    w_u32(voice + 24, 0);
    w_u32(voice + 32, 0);
    volume = r_u32(parameters);
    pitch_start = r_u32(parameters + 4);
    adsr = r_u32(parameters + 8);
    flags = r_s8(parameters + 15);
    w_u32(voice + 8, volume);
    w_u32(voice + 12, pitch_start);
    w_u32(voice + 16, adsr);
    w_u32(voice + 28, duration * rate);
    w_u8(voice + 36, flags & 1);
    w_u8(0x8009AC8A, 1);
    w_u32(voice + 4, 0x00010700);
}

uint32 gtmenu_voice_start(uint32 handle, uint32 parameters)
{
    uint32 priority, voice;
    sint32 index;
    FUNCTION_MARKER(0x8003004C, "GTMENU.EXE");
    priority = r_u8(parameters + 14);
    index = gtmenu_voice_allocate(priority);
    if (handle) w_u8(handle, index);
    if (index < 0) return index;
    voice = 0x8009AC90 + (uint32)index * 40;
    w_u8(voice, 1);
    w_u32(voice + 20, handle);
    gtmenu_voice_configure(index, parameters);
    return voice - 8;
}

sint32 gtmenu_sound_master_fade(uint16 target, sint32 milliseconds)
{
    uint32 current;
    FUNCTION_MARKER(0x8002FE90, "GTMENU.EXE");
    current = r_u16(0x8009B05E);
    w_u16(0x8009B064, 0);
    return gtmenu_sequence_fade_initialize(0x8009B064, current, target, milliseconds);
}

uint32 gtmenu_voice_pan(uint32 parameters, sint32 volume, sint32 pan)
{
    uint32 offset, factor;
    sint32 scaled;
    FUNCTION_MARKER(0x8002FF7C, "GTMENU.EXE");
    offset = (uint32)pan << 1;
    factor = r_u16(0x80055B22 - offset);
    scaled = factor * (uint32)volume;
    w_u16(parameters, scaled >> 14);
    factor = r_u16(0x80055A24 + offset);
    scaled = factor * (uint32)volume;
    w_u16(parameters + 2, scaled >> 14);
    return offset;
}

uint32 gtmenu_voice_pitch(uint32 note, uint32 reference)
{
    sint32 difference, octave = -2;
    uint32 index, pitch, shift;
    FUNCTION_MARKER(0x8002FFCC, "GTMENU.EXE");
    difference = note - reference;
    if (difference < 0) {
        do {
            difference += 3072;
            --octave;
        } while (difference < 0);
    } else if (difference >= 3072) {
        do {
            difference -= 3072;
            ++octave;
        } while (difference >= 3072);
    }
    index = (uint32)difference >> 3;
    pitch = r_u16(0x80055624 + index * 2);
    if (octave > 0) {
        shift = (uint32)octave & 31;
        return (pitch << shift) & 0xFFFF;
    }
    shift = (0 - (uint32)octave) & 31;
    return ((sint32)pitch >> shift) & 0xFFFF;
}

sint32 gtmenu_car_record_find(uint32 name, uint32 archive, uint32 section, uint32 require_alternate)
{
    uint32 records, names, count, record, id;
    sint32 index;
    FUNCTION_MARKER(0x800313F0, "GTMENU.EXE");
    records = r_u32(archive + section * 12 + 16);
    names = r_u32(archive + 172);
    records += archive;
    names += archive + 24;
    count = r_u16(records + 14);
    records += 24;
    for (index = 0; index < count; ++index) {
        record = records + (uint32)index * 108;
        id = r_u16(record);
        if (id && !gtmenu_string_compare(names + id * 424 - 424, name)) {
            if (!require_alternate || r_u16(record + 34)) return index;
        }
    }
    return -1;
}

sint32 gtmenu_car_record_find_variant(uint32 name, uint32 archive, uint32 section)
{
    GT_LOCAL_STACK_FRAME frame;
    sint32 result;
    FUNCTION_MARKER(0x80031500, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80031500);
    gtmenu_string_copy(frame.address, name);
    result = gtmenu_car_record_find(frame.address, archive, section, 0);
    if (result < 0 && r_s8(frame.address + 4) == 'r') {
        w_u8(frame.address + 4, 'n');
        result = gtmenu_car_record_find(frame.address, archive, section, 1);
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_car_config_copy(uint32 destination, uint32 source)
{
    FUNCTION_MARKER(0x80031618, "GTMENU.EXE");
    return gtmenu_memory_copy(destination, source, 386);
}

uint32 gtmenu_car_config_fields(uint32 destination, uint32 source, uint32 descriptors, uint32 archive)
{
    uint32 word, output, input, type, size, value, factor, section, stride, nested;
    sint32 id;
    FUNCTION_MARKER(0x80031638, "GTMENU.EXE");
    word = r_u32(descriptors);
    while (word) {
        word = r_u32(descriptors);
        output = destination + (word & 0x3FF);
        input = source + ((word >> 10) & 0x3FF);
        type = (word >> 20) & 15;
        if ((word >> 24) == 1) {
            size = 0;
            switch (type) {
            case 0:
                value = r_u8(input);
                w_u8(output, value);
                size = 1;
                break;
            case 1:
                value = r_u16(input);
                w_u16(output, value);
                size = 2;
                break;
            case 2:
                value = r_u32(input);
                w_u32(output, value);
                size = 4;
                break;
            case 3: size = 6; break;
            case 5: size = 12; break;
            case 6: size = 13; break;
            case 7: size = 17; break;
            case 8: size = 19; break;
            case 9:
            case 10:
                section = archive + r_u32(archive + (type == 9 ? 36 : 37) * 12 + 16);
                id = r_s16(input);
                stride = r_u32(section + 20);
                nested = section + ((uint32)id - 1) * stride + 24;
                gtmenu_car_config_fields(destination, nested, type == 9 ? 0x80070F74 : 0x80070FC8, archive);
                break;
            default:
                break;
            }
            if (size >= 5) gtmenu_memory_copy(output, input, size);
        } else {
            factor = r_u8(input);
            if (type) {
                value = r_u16(output);
                w_u16(output, value * factor / 100);
            } else {
                value = r_u8(output);
                w_u8(output, value * factor / 100);
            }
        }
        descriptors += 4;
        word = r_u32(descriptors);
    }
    return word;
}
