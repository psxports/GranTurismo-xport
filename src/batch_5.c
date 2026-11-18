#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include <stdio.h>

static void dropdown_corners(uint32 corners, uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
    w_u16(corners, x0);
    w_u16(corners + 2, y0);
    w_u16(corners + 4, x1);
    w_u16(corners + 6, y1);
}

uint32 gtmain_parts_record_matches_car(uint32 car, uint32 category, uint32 selection, uint32 archive)
{
    uint32 record, offset;
    sint32 owner;
    FUNCTION_MARKER(0x8006C058, "GTMAIN.EXE");
    record = gtmain_parts_record(category, selection, archive);
    offset = r_u8(0x80094175 + category);
    owner = r_s16(record + offset);
    return car == (uint32)owner;
}

uint32 gtmain_parts_inventory_flags(uint32 inventory, uint32 category)
{
    uint32 offset;
    FUNCTION_MARKER(0x8006C12C, "GTMAIN.EXE");
    offset = r_u16(0x800941F2 + category * 2);
    return inventory + offset;
}

uint32 gtmain_parts_available_build(uint32 descriptors, uint32 entries, sint32 count, uint32 category)
{
    uint32 context;
    FUNCTION_MARKER(0x8004AFCC, "GTMAIN.EXE");
    context = r_u32(0x800BEA64);
    return gtmain_parts_available_build_context(descriptors, entries, count, category, context);
}

uint32 gtmain_menu_category_draw(uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 selection_style, sint32 intensity, uint32 active)
{
    sint32 selected, spacing, visible, total, row, end, previous, phase, timer, fade_in = -1, fade_out = -1;
    sint32 brightness, tile, width;
    uint32 color, row_y = (uint32)y, records, sprites, sprite, uv, dimensions, page, packed;
    FUNCTION_MARKER(0x8004B2FC, "GTMAIN.EXE");
    (void)selection_style;
    phase = r_s8(0x80091B41);
    selected = r_s8(selector + 13);
    spacing = r_s8(selector + 12);
    color = gtmain_menu_palette_color(2, (sint32)((uint32)intensity * (uint32)phase));
    if (selected && (active & 1)) gtmain_packet_triangle_up(ordering, x, (sint32)((uint32)y - 8), color);
    total = r_s8(selector);
    visible = r_s8(selector + 1);
    if (visible < total - selected) {
        if (active) gtmain_packet_triangle_down(ordering, x,
            (sint32)((uint32)y + (uint32)spacing * (uint32)visible + 8), color);
    } else visible = total - selected;
    row = selected;
    gtmain_packet_page(ordering, 544);
    previous = r_s8(selector + 14);
    end = selected + visible;
    if (selected != previous) {
        w_u8(selector + 15, previous < selected ? 3 : 4);
        w_u8(selector + 16, 8);
    }
    if (r_s8(selector + 15)) {
        timer = r_u8(selector + 16) - 1;
        w_u8(selector + 16, timer);
        timer = (sint8)timer;
        if (timer <= 0) {
            w_u8(selector + 15, 0);
            w_u8(selector + 16, 0);
        } else {
            phase = r_s8(selector + 15);
            if (phase == 3) {
                row_y -= (uint32)(spacing * (8 - timer) / 8);
                --row;
                fade_in = row;
                fade_out = row + r_s8(selector + 1);
            } else if (phase == 4) {
                row_y -= (uint32)(spacing * timer / 8);
                total = r_s8(selector);
                ++end;
                if (total < end) end = total;
                fade_out = row;
                fade_in = row + r_s8(selector + 1);
            }
        }
    }
    records = r_u32(selector + 4) + (uint32)row * 3;
    sprites = r_u32(selector + 8);
    while (row < end) {
        phase = r_s8(selector + 15);
        brightness = intensity >> 1;
        if (phase) {
            if (row == fade_in) brightness = r_s8(selector + 16) * 16;
            else if (row == fade_out) brightness = 128 - r_s8(selector + 16) * 16;
        }
        if (row == selected) {
            if (active) brightness = gtmain_menu_pulse_component(0, brightness);
            tile = r_s8(records);
        } else tile = r_s8(records + 1);
        color = (uint32)brightness & 255;
        color |= (color << 8) | (color << 16) | 0x01000000;
        packed = row_y << 16;
        width = r_u8(records + 2);
        records += 3;
        ++row;
        row_y += (uint32)spacing;
        sprite = sprites + (uint32)tile * 16;
        uv = r_u32(sprite + 12);
        dimensions = r_u32(sprite + 8);
        packed |= (uint16)((uint32)x - (uint32)((sint8)width >> 1));
        gtmain_packet_sprite(ordering, packed, uv, dimensions, color);
        page = r_u8(sprite + 1);
        page = (uint16)((uint32)(sint32)(sint8)(page | 0x20) | 0x200);
        gtmain_packet_page(ordering, page);
    }
    return 0;
}

uint32 gtmain_menu_dropdown_row_style(uint32 flags, sint32 intensity)
{
    sint32 brightness;
    uint32 color;
    FUNCTION_MARKER(0x8004B778, "GTMAIN.EXE");
    gtmain_text_flags(1);
    brightness = flags & 0x40 ? intensity >> 2 : flags & 0x20 ? intensity >> 1 : intensity;
    color = gtmain_menu_gray(brightness);
    return gtmain_text_color(color);
}

uint32 gtmain_menu_dropdown_pair_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border, sint32 intensity)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 corners, text, color;
    sint32 value, precision, suffix, half = intensity >> 1;
    FUNCTION_MARKER(0x8004B7E0, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x68, 0x8004B7E0);
    corners = frame.address + 0x20;
    gtmain_text_flags(1);
    text = r_u32(record);
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 3), (sint32)((uint32)y + 4), text, 0);
    dropdown_corners(corners, x, y, (uint32)x + 112, (uint32)y + 17);
    gtmain_text_flags(2);
    color = gtmain_menu_gray(half);
    gtmain_text_color(color);
    value = r_s16(record + 4);
    precision = r_s8(record + 8);
    suffix = r_s8(record + 9);
    gtmain_menu_decimal_draw(ordering + 4, (sint32)((uint32)x + 148), y, value, precision, suffix, half, 2);
    value = r_s16(record + 6);
    precision = r_s8(record + 8);
    suffix = r_s8(record + 9);
    gtmain_menu_decimal_draw(ordering + 4, (sint32)((uint32)x + 212), y, value, precision, suffix, half, 2);
    gtmain_packet_outline(ordering, corners, border);
    dropdown_corners(corners, (uint32)x + 112, y, (uint32)x + 176, (uint32)y + 17);
    gtmain_packet_outline(ordering, corners, border);
    dropdown_corners(corners, (uint32)x + 176, y, (uint32)x + 239, (uint32)y + 17);
    gtmain_packet_outline(ordering, corners, border);
    gt_local_stack_pop(frame);
    return 17;
}

uint32 gtmain_menu_dropdown_number_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border, sint32 intensity)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 corners, text, color;
    sint32 value, precision, suffix, half = intensity >> 1;
    FUNCTION_MARKER(0x8004B9B4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x58, 0x8004B9B4);
    corners = frame.address + 0x20;
    text = r_u32(record);
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 2), (sint32)((uint32)y + 4), text, 0);
    gtmain_text_flags(2);
    color = gtmain_menu_gray(half);
    gtmain_text_color(color);
    value = r_s16(record + 4);
    precision = r_s8(record + 6);
    suffix = r_s8(record + 7);
    gtmain_menu_decimal_draw(ordering + 4, (sint32)((uint32)x + 200), y, value, precision, suffix, half, 2);
    dropdown_corners(corners, x, y, (uint32)x + 112, (uint32)y + 16);
    gtmain_packet_outline(ordering, corners, border);
    dropdown_corners(corners, (uint32)x + 112, y, (uint32)x + 239, (uint32)y + 16);
    gtmain_packet_outline(ordering, corners, border);
    gt_local_stack_pop(frame);
    return 16;
}

uint32 gtmain_menu_dropdown_list_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border, sint32 intensity)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 corners, text, color, row_y = (uint32)y + 2, height;
    sint32 count, index, value, half = intensity >> 1;
    FUNCTION_MARKER(0x8004BAF4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x60, 0x8004BAF4);
    corners = frame.address + 0x20;
    count = r_s8(record + 4);
    text = r_u32(record);
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 2), (sint32)((uint32)y + 4), text, 0);
    color = gtmain_menu_gray(half);
    gtmain_text_color(color);
    for (index = 0; index < count; ++index) {
        text = r_u32(record + 8 + (uint32)index * 4);
        gtmain_menu_caption_plain_draw(ordering, (sint32)((uint32)x + 120), (sint32)row_y, text);
        value = r_s16(record + 40 + (uint32)index * 2);
        gtmain_menu_decimal_draw(ordering, (sint32)((uint32)x + 168), (sint32)row_y, value, 3, 0, half, 1);
        row_y += 8;
    }
    height = (uint32)count * 8 + 4;
    dropdown_corners(corners, x, y, (uint32)x + 112, (uint32)y + height);
    gtmain_packet_outline(ordering, corners, border);
    dropdown_corners(corners, (uint32)x + 112, y, (uint32)x + 239, (uint32)y + height);
    gtmain_packet_outline(ordering, corners, border);
    gt_local_stack_pop(frame);
    return height;
}

uint32 gtmain_menu_dropdown_text_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 corners, text, width;
    FUNCTION_MARKER(0x8004BC84, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x48, 0x8004BC84);
    corners = frame.address + 0x18;
    gtmain_text_flags(1);
    text = r_u32(record);
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 2), (sint32)((uint32)y + 4), text, 0);
    gtmain_text_flags(2);
    text = r_u32(record + 4);
    width = gtmain_menu_text_width(2, text, 0);
    text = r_u32(record + 4);
    gtmain_menu_text_draw(ordering + 4, (sint32)((uint32)x - (uint32)((sint32)width >> 1) + 176), y, text, 0);
    dropdown_corners(corners, x, y, (uint32)x + 112, (uint32)y + 17);
    gtmain_packet_outline(ordering, corners, border);
    dropdown_corners(corners, (uint32)x + 112, y, (uint32)x + 239, (uint32)y + 17);
    gtmain_packet_outline(ordering, corners, border);
    gt_local_stack_pop(frame);
    return 17;
}

uint32 gtmain_menu_dropdown_draw(uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 selection_style, sint32 intensity, uint32 active)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 corners, rows, background, color, text, width, offset = 8, height, payload, row_y;
    uint32 red, green, blue, pulse, bottom;
    sint32 count, selected, index, half = intensity >> 1, flags, left;
    FUNCTION_MARKER(0x8004BDB0, "GTMAIN.EXE");
    (void)selection_style;
    frame = gt_local_stack_push(0xA8, 0x8004BDB0);
    corners = frame.address + 0x18;
    rows = r_u32(selector + 4);
    count = r_s8(selector);
    selected = r_s8(selector + 1);
    background = gtmain_menu_gray(intensity >> 2);
    gtmain_text_flags(1);
    gtmain_text_font(0);
    color = gtmain_menu_gray(half);
    gtmain_text_color(color);
    text = r_u32(selector + 12);
    width = gtmain_menu_text_width(1, text, 0);
    left = (sint32)((uint32)x + (uint32)((sint32)(64 - width) >> 1) + 112);
    text = r_u32(selector + 12);
    gtmain_menu_text_draw(ordering, left, y, text, 0);
    text = r_u32(selector + 16);
    width = gtmain_menu_text_width(1, text, 0);
    left = (sint32)((uint32)x + (uint32)((sint32)(64 - width) >> 1) + 176);
    text = r_u32(selector + 16);
    gtmain_menu_text_draw(ordering, left, y, text, 0);
    dropdown_corners(corners, (uint32)x + 112, y, (uint32)x + 239, (uint32)y + 7);
    gtmain_packet_rectangle(ordering, corners, background);
    for (index = 0; index < count; ++index) {
        flags = r_s8(rows);
        gtmain_menu_dropdown_row_style(flags, half);
        row_y = (uint32)y + (uint32)(sint32)(sint16)offset;
        height = 0;
        if ((flags & 31) <= 3) {
            payload = r_u32(rows + 4);
            switch (flags & 31) {
            case 0: height = gtmain_menu_dropdown_pair_row(ordering, payload, x, (sint32)row_y, background, intensity); break;
            case 1: height = gtmain_menu_dropdown_number_row(ordering, payload, x, (sint32)row_y, background, intensity); break;
            case 2: height = gtmain_menu_dropdown_list_row(ordering, payload, x, (sint32)row_y, background, intensity); break;
            case 3: height = gtmain_menu_dropdown_text_row(ordering, payload, x, (sint32)row_y, background); break;
            }
        }
        if (index == selected && (active & 1)) {
            pulse = gtmain_menu_pulse_component(0, half);
            red = r_u8(selector + 21) * pulse;
            green = r_u8(selector + 22) * pulse;
            blue = r_u8(selector + 23) * pulse;
            color = ((red >> 8) & 255) | (green & 0xFF00) | (((blue >> 8) & 255) << 16);
            dropdown_corners(corners, x, (uint32)y + offset, (uint32)x + 111, (uint32)y + offset + height - 1);
            gtmain_packet_rectangle(ordering, corners, color);
        }
        offset += height;
        rows += 8;
    }
    red = r_u8(selector + 21) * (uint32)half;
    green = r_u8(selector + 22) * (uint32)half;
    blue = r_u8(selector + 23) * (uint32)half;
    color = ((red >> 8) & 255) | (green & 0xFF00) | (((blue >> 8) & 255) << 16);
    gtmain_text_color(color);
    gtmain_text_flags(1);
    text = r_u32(selector + 8);
    gtmain_menu_text_draw(ordering, x, (sint32)((uint32)y - 5), text, 0);
    bottom = (uint32)y + offset;
    dropdown_corners(corners, (uint32)x - 3, bottom, (uint32)x + 242, bottom + 2);
    gtmain_packet_rectangle(ordering, corners, background);
    dropdown_corners(corners, (uint32)x - 3, (uint32)y + 5, (uint32)x - 1, bottom + 2);
    gtmain_packet_rectangle(ordering, corners, background);
    dropdown_corners(corners, (uint32)x + 240, (uint32)y - 3, (uint32)x + 242, bottom + 2);
    gtmain_packet_rectangle(ordering, corners, background);
    dropdown_corners(corners, (uint32)x - 3, (uint32)y + 5, (uint32)x + 111, (uint32)y + 7);
    gtmain_packet_rectangle(ordering, corners, background);
    dropdown_corners(corners, (uint32)x + 109, (uint32)y - 3, (uint32)x + 242, (uint32)y - 1);
    gtmain_packet_rectangle(ordering, corners, background);
    dropdown_corners(corners, (uint32)x + 109, (uint32)y - 3, (uint32)x + 111, (uint32)y + 7);
    gtmain_packet_rectangle(ordering, corners, background);
    dropdown_corners(corners, x, (uint32)y + 8, (uint32)x + 239, bottom - 1);
    color = (intensity >> 4) & 255;
    gtmain_packet_rectangle(ordering + 4, corners, color | (color << 8) | (color << 16));
    color = gtmain_packet_page(ordering + 4, 512);
    gt_local_stack_pop(frame);
    return color;
}

uint32 gtmain_race_settings_triplet_set(uint32 preset)
{
    FUNCTION_MARKER(0x80051F04, "GTMAIN.EXE");
    if (preset == 1) {
        w_u8(0x80093C08, 15);
        w_u8(0x80093C09, 20);
        w_u8(0x80093C0A, 100);
        return 100;
    }
    if (preset == 2) {
        w_u8(0x80093C08, 20);
        w_u8(0x80093C09, 10);
        w_u8(0x80093C0A, 20);
    } else {
        w_u8(0x80093C08, 0);
        w_u8(0x80093C09, 0);
        w_u8(0x80093C0A, 0);
    }
    return 0x80093C08;
}

void gtmain_race_controls_hud_clear(uint32 car)
{
    FUNCTION_MARKER(0x800521A4, "GTMAIN.EXE");
    w_u8(car + 1966, 0);
}

sint32 gtmain_race_route_fraction(sint32 time, sint32 *mode)
{
    uint32 duration, five;
    sint32 half, numerator;
    FUNCTION_MARKER(0x80052F60, "GTMAIN.EXE");
    duration = r_u32(0x800BF518);
    five = duration * 5;
    half = (sint32)duration / 2;
    if (time >= (sint32)(five - (uint32)half)) {
        *mode = 1;
        duration = r_u32(0x800BF518);
        numerator = (sint32)((duration * 5 - (uint32)time) * 1228);
        half = (sint32)duration / 2;
        if (!half) gt_draft_exception(7);
        if (half == -1 && numerator == (sint32)0x80000000) gt_draft_exception(6);
        return numerator / half;
    }
    duration = r_u32(0x800BF518);
    if (time < (sint32)duration / 2) {
        *mode = 0;
        return 0;
    }
    *mode = 2;
    return 1228;
}

sint32 gtmain_race_ai_mode3(uint32 mode)
{
    uint32 routes, available;
    FUNCTION_MARKER(0x80053C68, "GTMAIN.EXE");
    routes = r_u32(0x800BF51C);
    available = r_u32(routes + 20);
    if (!available || mode == 3) return -1;
    return 3;
}

sint32 gtmain_race_ai_mode4(uint32 mode)
{
    uint32 routes, available;
    FUNCTION_MARKER(0x80053CA0, "GTMAIN.EXE");
    routes = r_u32(0x800BF51C);
    available = r_u32(routes + 24);
    if (!available || mode == 4) return -1;
    return 4;
}

uint32 gtmain_race_ai_alternate_position(sint32 mode, sint32 unused_segment, sint32 progress, sint32 *x, sint32 *z)
{
    uint32 routes, path, records, count, length_address, length, first = 0, second = 0, index;
    uint32 left, right, start, end, x0, x1, z0, z1;
    sint32 fraction = 0, difference;
    sint64 product_x, product_z;
    FUNCTION_MARKER(0x800546BC, "GTMAIN.EXE");
    (void)unused_segment;
    routes = r_u32(0x800BF51C);
    path = r_u32(routes + (uint32)mode * 4 + 8);
    length_address = r_u32(0x800BC1A8);
    records = path + 4;
    length = r_u32(length_address);
    count = r_u32(path);
    if (progress < 0) progress = (sint32)((uint32)progress + length);
    else if (progress >= (sint32)length) progress = (sint32)((uint32)progress - length);
    if (progress < (sint32)length / 2) progress = (sint32)((uint32)progress + length);
    start = r_u32(records + 20);
    if (progress < (sint32)start) return 0;
    end = r_u32(records + count * 40 - 20);
    if (progress >= (sint32)end) return 0;
    if ((sint32)(count - 1) > 0) {
        for (index = 0; (sint32)index < (sint32)(count - 1); ++index) {
            first = index;
            start = r_u32(records + index * 40 + 20);
            second = index + 1;
            if (progress >= (sint32)start) {
                end = r_u32(records + second * 40 + 20);
                if (progress < (sint32)end) {
                    difference = (sint32)((uint32)progress - start);
                    fraction = (sint32)(uint32)gtmain_signed_divide64((sint64)difference * 4096,
                        (sint32)(end - start), 0x800547B4);
                    break;
                }
            }
        }
    }
    left = records + first * 40;
    right = records + second * 40;
    x0 = r_u32(left + 4);
    x1 = r_u32(right + 4);
    product_x = (sint64)fraction * (sint32)(x1 - x0);
    z0 = r_u32(left + 8);
    z1 = r_u32(right + 8);
    product_z = (sint64)fraction * (sint32)(z1 - z0);
    *x = (sint32)(x0 + (uint32)(product_x / 4096));
    *z = (sint32)(z0 + (uint32)(product_z / 4096));
    return 1;
}

sint32 gtmain_race_behavior_one(uint32 car)
{
    FUNCTION_MARKER(0x80054F70, "GTMAIN.EXE");
    return gtmain_race_behavior_set(car, 1);
}

sint32 gtmain_race_behavior_zero(uint32 car)
{
    FUNCTION_MARKER(0x80054F90, "GTMAIN.EXE");
    return gtmain_race_behavior_set(car, 0);
}

sint32 gtmain_race_behavior_toggle(uint32 car)
{
    uint32 behavior;
    FUNCTION_MARKER(0x80054FB0, "GTMAIN.EXE");
    behavior = r_u8(car + 1696);
    return gtmain_race_behavior_set(car, behavior != 1);
}

uint32 gtmain_race_objective_initialize(sint32 mode, uint32 car, uint32 first, uint32 second)
{
    sint32 progress;
    FUNCTION_MARKER(0x80056E48, "GTMAIN.EXE");
    w_u16(0x800BF522, 0);
    w_u16(0x800BF520, mode);
    w_u32(0x800BF524, first);
    w_u32(0x800BF528, second);
    w_u32(0x800BF52C, 359999999);
    if (mode == 4) {
        progress = gtmain_race_course_progress(car + 60);
        w_u32(car + 1804, progress);
        if (progress <= 4063231) w_u16(car + 56, 1);
    }
    return 0;
}

uint32 gtmain_race_objective_mode2_time(uint32 time)
{
    uint32 mode, best;
    FUNCTION_MARKER(0x80056EBC, "GTMAIN.EXE");
    mode = r_u16(0x800BF520);
    if (mode != 2) return 2;
    best = r_u32(0x800BF52C);
    if (time >= best) return 0;
    w_u32(0x800BF52C, time);
    return 1;
}

uint32 gtmain_race_objective_mode3_time(uint32 time)
{
    uint32 mode, best;
    FUNCTION_MARKER(0x80056EF8, "GTMAIN.EXE");
    mode = r_u16(0x800BF520);
    if (mode == 2) return 1;
    if (mode != 3) return 3;
    best = r_u32(0x800BF52C);
    if (time >= best) return 0;
    w_u32(0x800BF52C, time);
    return 1;
}

uint32 gtmain_race_objective_mode4_time(uint32 lap, uint32 time)
{
    uint32 mode, target;
    FUNCTION_MARKER(0x80056F44, "GTMAIN.EXE");
    mode = r_u16(0x800BF520);
    if (mode == 2) return 1;
    if (mode < 3) return 3;
    if (mode != 4) return 4;
    target = r_u32(0x800BF524);
    if (target == lap) w_u32(0x800BF52C, time);
    return target;
}

uint32 gtmain_race_wheel_class_count(uint32 car)
{
    uint32 index, count = 0, classification;
    FUNCTION_MARKER(0x80056F94, "GTMAIN.EXE");
    for (index = 0; index < 4; ++index) {
        classification = r_u8(car + 176 + index * 68);
        if (classification >= 2) ++count;
    }
    return count;
}

sint32 gtmain_race_objective_fail(uint32 car)
{
    FUNCTION_MARKER(0x80057100, "GTMAIN.EXE");
    w_u16(0x800BF522, 3);
    return gtmain_race_behavior_set(car, 5);
}

sint32 gtmain_race_objective_complete(uint32 car)
{
    uint32 mode;
    sint32 behavior;
    FUNCTION_MARKER(0x8005712C, "GTMAIN.EXE");
    w_u16(0x800BF522, 2);
    mode = r_u16(0x800BF520);
    behavior = mode == 3 ? 1 : mode == 4 ? 6 : 5;
    return gtmain_race_behavior_set(car, behavior);
}

sint32 gtmain_race_course_progress(uint32 position)
{
    sint32 delta[3], coefficient;
    uint32 index, value, origin, longitudinal = 0, lateral = 0, result;
    sint64 product;
    FUNCTION_MARKER(0x8005F2D0, "GTMAIN.EXE");
    for (index = 0; index < 3; ++index) {
        value = r_u32(position + index * 4);
        origin = r_u32(0x80093E7C + index * 4);
        delta[index] = (sint32)(value - origin);
    }
    for (index = 0; index < 3; ++index) {
        coefficient = r_s16(0x80093E88 + index * 2);
        product = (sint64)coefficient * delta[index];
        longitudinal += (uint32)(product / 4096);
        coefficient = r_s16(0x80093E90 + index * 2);
        product = (sint64)coefficient * delta[index];
        lateral += (uint32)(product / 4096);
    }
    if ((sint32)longitudinal > 40959) {
        result = gtmain_angle_atan_wide((sint32)(longitudinal - 40960), (sint32)lateral);
        result = result * 84 + 40960;
    } else if ((sint32)longitudinal <= -40960) {
        result = gtmain_angle_atan_wide((sint32)(0 - (longitudinal + 40960)), (sint32)(0 - lateral));
        result = (result + 2048) * 84 + 122880;
    } else if ((sint32)lateral > 0) {
        result = longitudinal;
        if ((sint32)result < 0) result += 507904;
    } else result = 253952 - longitudinal;
    return (sint32)(result * 16);
}

uint32 gtmain_race_wheel_progress_range(uint32 car, sint32 *minimum, sint32 *maximum)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 length_address, vector, index, variant, point, course;
    sint32 length, half, progress, low = 0x7FFFFFFF, high = (sint32)0x80000001;
    FUNCTION_MARKER(0x80056FDC, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x48, 0x80056FDC);
    vector = frame.address + 0x10;
    length_address = r_u32(0x800BC1A8);
    length = r_s32(length_address);
    half = length / 2;
    for (index = 0; index < 4; ++index) {
        variant = r_u8(car + 1961);
        point = car + index * 8 + variant * 32;
        w_u32(vector, r_u32(point + 1824) * 16);
        w_u32(vector + 4, r_u32(car + 68) * 16);
        w_u32(vector + 8, r_u32(point + 1828) * 16);
        course = r_u32(car + 1892);
        progress = gtmain_course_progress(course, psx_addr(vector, 12));
        if (progress > half) progress = (sint32)((uint32)progress - (uint32)length);
        if (progress < low) low = progress;
        if (progress > high) high = progress;
    }
    *minimum = low;
    *maximum = high;
    gt_local_stack_pop(frame);
    return 0;
}

uint32 gtmain_race_objective_update(uint32 car)
{
    uint32 state, mode, wheels, first, second, third, swap, speed, scale, score, base, elapsed;
    sint32 low, high;
    FUNCTION_MARKER(0x80057180, "GTMAIN.EXE");
    state = r_u16(0x800BF522);
    if (state == 3) return 0;
    state = r_u16(0x800BF522);
    if (state == 2) return 0;
    wheels = gtmain_race_wheel_class_count(car);
    if (wheels == 4) {
        gtmain_race_objective_fail(car);
        return 4;
    }
    if (r_s16(car + 42) >= 1025) {
        gtmain_race_objective_fail(car);
        return 5;
    }
    mode = r_u16(0x800BF520);
    if (mode == 2) {
        state = r_u16(0x800BF522);
        if (state != 1) w_u16(0x800BF522, 1);
        if (r_u8(car + 59) != 1) return 0;
        gtmain_race_objective_complete(car);
        return 1;
    }
    if (mode >= 3) {
        sint32 lap, target;
        if (mode >= 5) return 0;
        state = r_u16(0x800BF522);
        if (state != 1) w_u16(0x800BF522, 1);
        lap = r_s16(car + 56);
        target = r_s32(0x800BF524);
        if (target >= lap) return 0;
        gtmain_race_objective_complete(car);
        return 1;
    }
    if (mode != 1) return 0;
    gtmain_race_wheel_progress_range(car, &low, &high);
    if (r_s32(0x800BF528) < high) {
        gtmain_race_objective_fail(car);
        return 3;
    }
    state = r_u16(0x800BF522);
    if (state != 1) w_u16(0x800BF522, 1);
    if (low < r_s32(0x800BF524)) return 0;
    first = r_u32(car + 1712);
    second = r_u32(car + 1716);
    third = r_u32(car + 1720);
    first = (sint32)first < 0 ? 0 - first : first;
    second = (sint32)second < 0 ? 0 - second : second;
    third = (sint32)third < 0 ? 0 - third : third;
    if ((sint32)first < (sint32)second) { swap = first; first = second; second = swap; }
    if ((sint32)first < (sint32)third) { swap = first; first = third; third = swap; }
    speed = first + (uint32)((sint32)(second + third) >> 2);
    if ((sint32)speed >= 1138) return 0;
    if (wheels) {
        gtmain_race_objective_fail(car);
        return 4;
    }
    scale = r_u32(0x800B6318) == 1 ? 50 : 100;
    score = (uint32)((sint32)(speed * scale) / 1138);
    gtmain_race_objective_complete(car);
    base = r_u32(0x80093D78);
    elapsed = r_u32(car + 44);
    w_u32(0x800BF52C, (score + base) / 3 - elapsed);
    return 1;
}

uint32 gtmain_race_engine_specification_summary(uint32 specification, uint32 detail)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 curve, result;
    FUNCTION_MARKER(0x8005EFC4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0xC8, 0x8005EFC4);
    curve = frame.address + 0x10;
    gtmain_race_engine_curve_initialize(specification, curve, 0);
    gtmain_race_engine_curve_summary(curve, detail);
    result = r_u16(detail);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_race_course_projection_set(uint32 position, uint32 first_basis, uint32 second_basis)
{
    uint32 index, value;
    FUNCTION_MARKER(0x8005F260, "GTMAIN.EXE");
    for (index = 0; index < 3; ++index) {
        value = r_u32(position + index * 4);
        w_u32(0x80093E7C + index * 4, value);
        value = r_u16(first_basis + index * 2);
        w_u16(0x80093E88 + index * 2, value);
        value = r_u16(second_basis + index * 2);
        w_u16(0x80093E90 + index * 2, value);
    }
    return 0;
}

uint32 gtmain_race_route_pose_apply(uint32 car, uint32 route)
{
    uint32 offset, height, next;
    FUNCTION_MARKER(0x80060050, "GTMAIN.EXE");
    offset = r_u32(car + 104);
    height = r_u32(car + 68);
    next = route + offset;
    if ((sint32)next < (sint32)height) return 0;
    w_u32(car + 68, next);
    w_u32(car + 1800, 0);
    w_u32(car + 1796, 0);
    w_u16(car + 1788, 0);
    w_u16(car + 1786, 0);
    w_u32(car + 1720, 0);
    return 1;
}

uint32 gtmain_race_pair_cache_clear(sint32 car_index, sint32 count)
{
    uint32 bank, index, byte, address, base, column;
    sint32 row_count = (sint32)((uint32)count - 1);
    FUNCTION_MARKER(0x80062138, "GTMAIN.EXE");
    for (bank = 0; bank < 2; ++bank) {
        base = 0x800BFD10 + bank * 120;
        for (index = 0; (sint32)index < row_count; ++index) {
            address = base + (uint32)car_index * 20 + index * 4;
            for (byte = 0; byte < 4; ++byte) w_u8(address + byte, 0);
        }
        for (index = 0; (sint32)index < count; ++index) {
            if (index == (uint32)car_index) continue;
            column = (uint32)car_index - (car_index >= (sint32)index);
            address = base + index * 20 + column * 4;
            for (byte = 0; byte < 4; ++byte) w_u8(address + byte, 0);
        }
    }
    return 0;
}

sint32 gtmain_race_pair_boost_apply(uint32 unused_peer, uint32 car, sint32 laps, sint32 distance)
{
    uint32 flags, factor = 4096, coefficient, base;
    sint32 upper, lower, fraction, result;
    FUNCTION_MARKER(0x8006199C, "GTMAIN.EXE");
    (void)unused_peer;
    flags = r_u8(car + 58);
    if (!(flags & 0x10) && laps >= 0) {
        if (laps > 0) factor = r_u32(0x80093EAC) + 4096;
        else {
            upper = r_s32(0x80093EB4);
            if (distance >= upper) factor = r_u32(0x80093EAC) + 4096;
            else {
                lower = r_s32(0x80093EB0);
                if (distance > lower) {
                    upper = r_s32(0x80093EB4);
                    fraction = (sint32)(uint32)gtmain_signed_divide64(
                        (sint64)(sint32)((uint32)distance - (uint32)lower) * 4096,
                        (sint32)((uint32)upper - (uint32)lower), 0x80061A44);
                    coefficient = r_u32(0x80093EAC);
                    factor = (uint32)((sint32)((uint32)fraction * coefficient) / 4096) + 4096;
                }
            }
        }
    }
    base = r_u32(0x80093EA8);
    result = (sint32)(factor * base) / 4096;
    w_u16(car + 1702, result);
    return result;
}

sint32 gtmain_race_pair_boost_update(uint32 first_car, uint32 second_car)
{
    uint32 enabled;
    sint32 laps, distance;
    FUNCTION_MARKER(0x80061AAC, "GTMAIN.EXE");
    enabled = r_u32(0x80093EAC);
    if (!enabled) return 0;
    gtmain_race_position_delta(first_car, second_car, &laps, &distance);
    gtmain_race_pair_boost_apply(first_car, second_car, laps, distance);
    return gtmain_race_pair_boost_apply(second_car, first_car,
        (sint32)(0 - (uint32)laps), (sint32)(0 - (uint32)distance));
}

uint32 gtmain_race_tire_wear_update(uint32 car)
{
    uint32 index, contact, state, load, bias, axis, force, coefficient, average, combined, increment;
    uint32 wear, maximum, threshold, first, second, adjustment, flags;
    sint32 component, other, fraction, indicator, signed_state;
    sint64 product;
    FUNCTION_MARKER(0x8005D718, "GTMAIN.EXE");
    for (index = 0; index < 4; ++index) {
        contact = car + 132 + index * 68;
        state = r_u8(contact + 44);
        w_u8(contact + 42, 0);
        if (state) continue;
        load = r_u32(contact + 20);
        if (!load) continue;
        bias = r_u32(car + 1652);
        average = (uint32)((sint32)(load + bias) / 3);
        axis = index < 2 ? 0 : 2;
        force = (uint32)r_s16(contact + 32);
        coefficient = (uint32)r_s16(car + 1672 + axis);
        component = (sint32)(force * coefficient) / 4096;
        force = (uint32)r_s16(contact + 34);
        coefficient = (uint32)r_s16(car + 1668 + axis);
        other = (sint32)(force * coefficient) / 4096;
        combined = (uint32)component + (uint32)other;
        combined = (uint32)((sint32)(combined * average) / 4096);
        coefficient = r_u32(0x1F800000);
        increment = (uint32)((sint32)(combined * coefficient) / 65536);
        wear = r_u32(contact + 36);
        w_u32(contact + 36, wear + increment);
        if ((sint32)increment >= 256) increment = 255;
        w_u8(contact + 42, increment);
    }
    for (index = 0; index < 4; ++index) {
        contact = car + 132 + index * 68;
        wear = r_u32(contact + 36);
        maximum = r_u32(0x80093E30);
        if ((sint32)wear >= (sint32)maximum) {
            adjustment = r_u16(0x80093E34);
            w_u32(contact + 36, maximum);
            w_u8(contact + 43, 127);
            w_u16(contact + 40, 4096 - adjustment);
            continue;
        }
        wear = r_u32(contact + 36);
        threshold = r_u32(0x80093E44);
        if ((sint32)wear >= (sint32)threshold) {
            maximum = r_u32(0x80093E30);
            fraction = (sint32)(uint32)gtmain_signed_divide64(
                (sint64)(sint32)(wear - threshold) * 4096,
                (sint32)(maximum - threshold), 0x8005D9FC);
            first = r_u32(0x80093E34);
            second = r_u32(0x80093E48);
            product = (sint64)fraction * (sint32)(first - second);
            adjustment = (uint32)(product / 4096);
            second = r_u16(0x80093E48);
            w_u16(contact + 40, 4096 - adjustment - second);
            indicator = fraction >= 4096 ? 127 : (fraction >> 6) + 64;
        } else {
            wear = r_u32(contact + 36);
            if ((sint32)wear >= 0) {
                threshold = r_u32(0x80093E44);
                fraction = (sint32)(uint32)gtmain_signed_divide64(
                    (sint64)(sint32)wear * 4096, (sint32)threshold, 0x8005DAB0);
                coefficient = r_u32(0x80093E48);
                product = (sint64)fraction * (sint32)coefficient;
                w_u16(contact + 40, 4096 - (uint32)(product / 4096));
                indicator = fraction >= 4096 ? 63 : fraction >> 6;
            } else {
                wear = r_u32(contact + 36);
                threshold = r_u32(0x80093E3C);
                if ((sint32)wear < (sint32)threshold) {
                    w_u16(contact + 40, 4096);
                    indicator = -128;
                } else {
                    fraction = (sint32)(uint32)gtmain_signed_divide64(
                        (sint64)(sint32)(0 - wear) * 4096, (sint32)(0 - threshold), 0x8005DB54);
                    coefficient = r_u32(0x80093E40);
                    product = (sint64)fraction * (sint32)coefficient;
                    w_u16(contact + 40, 4096 - (uint32)(product / 4096));
                    indicator = fraction >= 4096 ? -63 : -(fraction >> 6);
                }
            }
        }
        w_u8(contact + 43, indicator);
    }
    flags = r_u32(car + 56) & 0xC0000;
    if (flags) return flags;
    for (index = 0; index < 4; ++index) {
        contact = car + 132 + index * 68;
        wear = r_u32(contact + 36);
        if ((sint32)wear < 0) continue;
        component = r_s16(contact + 40);
        threshold = r_u32(0x80093E38);
        if (component >= (sint32)threshold) continue;
        flags = r_u8(car + 58);
        signed_state = r_s8(car + 1689);
        w_u8(car + 58, flags | 4);
        if (signed_state) {
            w_u8(car + 58, flags | 12);
            return flags | 12;
        }
        return flags | 4;
    }
    return 64;
}

sint32 gtmain_settings_range_index(sint32 minimum, sint32 maximum, sint32 value, sint32 count)
{
    sint32 span, numerator, index;
    FUNCTION_MARKER(0x8006BD84, "GTMAIN.EXE");
    span = (sint32)((uint32)maximum - (uint32)minimum);
    if (!span) return 0;
    numerator = (sint32)(((uint32)value - (uint32)minimum) * ((uint32)count - 1));
    if (span == -1 && numerator == (sint32)0x80000000) gt_draft_exception(6);
    index = numerator / span;
    if (index < 0) index = 0;
    if (index >= count) return (sint32)((uint32)count - 1);
    return index;
}

void gtmain_parts_settings_import(uint32 settings, uint32 category, uint32 source)
{
    uint32 first, second, value, count, index;
    sint32 selected;
    FUNCTION_MARKER(0x8006BE00, "GTMAIN.EXE");
    switch (category) {
    case 17:
        w_u8(settings + 64, r_u8(source));
        first = r_u8(source + 2);
        second = r_u8(source + 3);
        value = r_u8(source);
        count = r_u8(source + 4);
        selected = gtmain_settings_range_index(first, second, value, count);
        w_u8(settings + 74, selected);
        w_u8(settings + 77, r_u8(source + 1));
        first = r_u8(source + 5);
        second = r_u8(source + 6);
        value = r_u8(source + 1);
        count = r_u8(source + 7);
        selected = gtmain_settings_range_index(first, second, value, count);
        w_u8(settings + 87, selected);
        break;
    case 23:
        gtmain_memory_copy(settings + 46, source + 2, 16);
        break;
    case 30:
        w_u8(settings + 65, r_u8(source + 8));
        w_u8(settings + 78, r_u8(source + 9));
        break;
    case 31:
        w_u8(settings + 66, r_u8(source));
        first = r_u8(source + 2);
        second = r_u8(source + 3);
        value = r_u8(source);
        count = r_u8(source + 1);
        selected = gtmain_settings_range_index(first, second, value, count);
        w_u8(settings + 75, selected);
        break;
    case 32:
        w_u8(settings + 79, r_u8(source));
        first = r_u8(source + 2);
        second = r_u8(source + 3);
        value = r_u8(source);
        count = r_u8(source + 1);
        selected = gtmain_settings_range_index(first, second, value, count);
        w_u8(settings + 88, selected);
        break;
    case 33:
        w_u8(settings + 68, r_u8(source + 11));
        w_u8(settings + 69, r_u8(source + 12));
        w_u8(settings + 67, r_u8(source + 10));
        for (index = 0; index < 4; ++index) w_u8(settings + 70 + index, r_u8(source + 13 + index));
        first = r_u8(source + 30);
        second = r_u8(source + 31);
        value = r_u8(source + 13);
        count = r_u8(source + 38);
        selected = gtmain_settings_range_index(first, second, value, count);
        w_u8(settings + 76, selected);
        w_u8(settings + 81, r_u8(source + 18));
        w_u8(settings + 82, r_u8(source + 19));
        w_u8(settings + 80, r_u8(source + 17));
        for (index = 0; index < 4; ++index) w_u8(settings + 83 + index, r_u8(source + 20 + index));
        first = r_u8(source + 45);
        second = r_u8(source + 46);
        value = r_u8(source + 20);
        count = r_u8(source + 53);
        selected = gtmain_settings_range_index(first, second, value, count);
        w_u8(settings + 89, selected);
        break;
    case 35:
        w_u8(settings + 63, r_u8(source + 16));
        break;
    default:
        break;
    }
}

uint32 gtmain_menu_fixed_text_extent(sint32 font, uint32 text)
{
    uint32 width;
    FUNCTION_MARKER(0x8006C888, "GTMAIN.EXE");
    switch (font) {
    case 0: width = 6; break;
    case 1: width = 8; break;
    case 2: width = 12; break;
    case 3: width = 24; break;
    default: return 0;
    }
    return gtmain_string_length(text) * width;
}

uint32 gtmain_text_character_encode(uint16 character)
{
    uint32 high, low, encoded;
    FUNCTION_MARKER(0x80071640, "GTMAIN.EXE");
    if (character == 0x2D7F) return 0x877F;
    high = character >> 8;
    low = character & 255;
    encoded = ((high + 33) << 7) ^ 0xA000;
    if (high & 1) encoded = encoded - (low < 96) + 32;
    else encoded -= 2;
    return (uint16)(encoded + low);
}

uint32 gtmain_menu_image_upload(uint32 image)
{
    uint32 source, cursor;
    FUNCTION_MARKER(0x8006F798, "GTMAIN.EXE");
    gtmain_gpu_draw_sync(0);
    source = r_u32(image);
    gtmain_gpu_image_load(image + 16, source);
    cursor = r_u32(image + 4);
    w_u32(image + 8, 0);
    w_u32(image + 28, 0);
    w_u32(image + 12, cursor);
    return cursor;
}

uint32 gtmain_stream_playlist_shuffle(uint32 output, sint32 count)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 seed, result, candidate, index = 0, previous;
    FUNCTION_MARKER(0x8007917C, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x38, 0x8007917C);
    seed = frame.address + 0x10;
    result = gtmain_timer_random();
    w_u32(seed, result);
    if (count > 0) {
        while ((sint32)index < count) {
            candidate = gtmain_random_step(seed) % (uint32)count;
            for (previous = 0; previous < index; ++previous) {
                if (r_u8(output + previous) == candidate) break;
            }
            if (previous != index) continue;
            w_u8(output + index, candidate);
            ++index;
        }
        result = 0;
    }
    gt_local_stack_pop(frame);
    return result;
}

sint32 gtmain_format_octal(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision,
    uint32 low, uint32 high, sint8 sign)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 digits, length = 0, digit;
    uint64 value = ((uint64)high << 32) | low;
    sint32 shift, result;
    FUNCTION_MARKER(0x8007C8F4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x48, 0x8007C8F4);
    digits = frame.address + 0x21;
    w_u8(frame.address + 0x20, sign);
    for (shift = 63; shift >= 0; shift -= 3) {
        digit = (uint32)(value >> shift) & 7;
        if (length || digit) w_u8(digits + length++, digit + 48);
    }
    sign = r_s8(frame.address + 0x20);
    result = gtmain_format_number(emit, flags, width, precision, (uint8)sign, digits, length, 8);
    gt_local_stack_pop(frame);
    return result;
}

sint32 gtmain_format_hex(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision,
    uint32 low, uint32 high, uint32 letter)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 digits, length = 0, digit, table, character;
    uint64 value = ((uint64)high << 32) | low;
    sint32 shift, result;
    FUNCTION_MARKER(0x8007C9B4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x48, 0x8007C9B4);
    digits = frame.address + 0x20;
    table = letter == 120 ? 0x8008E3A0 : 0x8008E3B4;
    for (shift = 60; shift >= 0; shift -= 4) {
        digit = (uint32)(value >> shift) & 15;
        if (length || digit) {
            character = r_u8(table + digit);
            w_u8(digits + length++, character);
        }
    }
    result = gtmain_format_number(emit, flags, width, precision, 0, digits, length, letter == 88 ? 17 : 16);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_heap_free(uint32 allocation)
{
    uint32 block, result, previous, next;
    FUNCTION_MARKER(0x8007CFE0, "GTMAIN.EXE");
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
        next = r_u32(block + 4);
        if (!next) return result;
        result = r_u32(next + 12) & 1;
        if (result) return result;
        result = gt_heap_block_merge_next(block, result);
    }
}

static uint32 sequence_variable_length(uint32 *cursor, uint32 value)
{
    uint32 next;
    if (!(value & 0x80)) return value;
    value &= 0x7F;
    do {
        next = r_u8((*cursor)++);
        value = (value << 7) + (next & 0x7F);
    } while (next & 0x80);
    return value;
}

void gtmain_sound_sequence_note(uint32 sequence, uint32 channel, sint32 key, sint32 velocity, uint32 deadline)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 bank, program_table, program, number, channel_gain, program_gain, velocity_gain, gain;
    uint32 pan_base, cursor, count, index, tone_table, tone, lower, upper, attempt, slot, slot_index;
    uint32 instrument, clock, value, pitch, reference;
    sint32 amplitude, bend, pan;
    FUNCTION_MARKER(0x8007E108, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x98, 0x8007E108);
    instrument = frame.address + 0x60;
    bank = r_u32(sequence + 24);
    number = r_u8(channel + 17);
    program_table = r_u32(bank + 36);
    program = r_u32(program_table + number * 4);
    channel_gain = r_u16(channel + 10);
    program_gain = (uint32)r_s16(program + 2);
    velocity_gain = r_u16(0x8008E888 + (uint32)velocity * 2);
    gain = (uint32)((sint32)(channel_gain * program_gain) >> 14);
    gain = (uint32)((sint32)(velocity_gain * gain) >> 14);
    pan_base = r_u8(channel + 16);
    pan_base += r_u8(program + 1);
    count = r_u8(program);
    cursor = program;
    for (index = 0; index < count; ++index, ++cursor) {
        number = r_u8(cursor + 4);
        tone_table = r_u32(bank + 28);
        tone = tone_table + number * 20;
        lower = r_u8(tone + 12);
        if (key < (sint32)lower) continue;
        upper = r_u8(tone + 13);
        if (key > (sint32)upper) continue;
        slot_index = r_u8(0x800B59A4);
        slot = 0x800B59B4 + slot_index * 28;
        for (attempt = 0; attempt < 25; ++attempt) {
            slot += 28;
            if (++slot_index == 25) { slot_index = 0; slot = 0x800B59B4; }
            if (r_s8(slot + 27) < 0) break;
        }
        if (attempt == 25) continue;
        value = (uint32)r_s16(tone + 2);
        pan = r_u8(tone + 9);
        bend = r_s16(channel + 12);
        amplitude = (sint32)(value * gain) >> 14;
        pan = (sint32)((uint32)pan + pan_base - 128);
        w_u8(0x800B59A4, slot_index);
        if (pan < 0) pan = 0;
        else if (pan >= 128) pan = 127;
        w_u32(slot, sequence);
        w_u32(slot + 4, channel);
        w_u32(slot + 12, tone);
        w_u32(slot + 8, program);
        clock = r_u32(sequence + 20);
        w_u16(slot + 22, bend);
        w_u32(slot + 16, deadline + clock);
        w_u8(slot + 24, key);
        w_u8(slot + 25, velocity);
        w_u16(slot + 20, r_u16(channel + 10));
        w_u8(slot + 26, r_u8(channel + 16));
        w_u32(instrument + 8, r_u32(tone + 16));
        w_u16(instrument + 6, r_u16(tone));
        w_u8(instrument + 14, r_u8(tone + 8));
        w_u16(instrument + 12, r_u16(tone + 6));
        w_u8(instrument + 15, r_u8(tone + 10));
        pitch = (uint32)key << 8;
        if (bend) {
            value = r_u8(tone + (bend > 0 ? 15 : 14));
            pitch += (uint32)((sint32)((uint32)bend * value) >> 5);
        }
        reference = r_u16(tone + 4);
        value = gtmain_sound_pitch_ratio(pitch, reference);
        w_u16(instrument + 4, value);
        gtmain_sound_pan(instrument, amplitude, pan);
        gtmain_sound_voice_start(slot + 27, instrument);
        count = r_u8(program);
    }
    gt_local_stack_pop(frame);
}

void gtmain_sound_sequence_events(uint32 sequence)
{
    uint32 channel, index, cursor, command, value, velocity, duration, clock, remaining, repeat, volume;
    uint32 first, second, third;
    sint32 depth;
    FUNCTION_MARKER(0x8007E3F8, "GTMAIN.EXE");
    channel = sequence + 40;
    for (index = 0; index < 16; ++index, channel += 40) {
        cursor = r_u32(channel);
        if (!cursor) continue;
        remaining = r_u32(channel + 4);
        clock = r_u32(sequence + 20);
        remaining -= clock;
        w_u32(channel + 4, remaining);
        while ((sint32)remaining <= 0) {
            command = r_u8(cursor++);
            if (command & 0x80) {
                duration = r_u8(cursor + 1);
                velocity = r_u8(cursor);
                cursor += 2;
                duration = sequence_variable_length(&cursor, duration);
                clock = r_u32(sequence + 20);
                gtmain_sound_sequence_note(sequence, channel, command & 0x7F, velocity, duration + clock + remaining);
                remaining = r_u32(channel + 4);
            } else if (command & 0x40) {
                value = r_u8(cursor++);
                remaining = r_u32(channel + 4);
                w_u16(channel + 12, (value | ((command & 0x3F) << 8)) - 0x2000);
            } else if (command & 0x20) {
                depth = r_s8(channel + 18) + 1;
                w_u8(channel + (uint32)depth + 36, (command & 31) + 1);
                w_u32(channel + (uint32)depth * 4 + 20, cursor);
                remaining = r_u32(channel + 4);
                w_u8(channel + 18, depth);
            } else {
                switch (command) {
                case 0:
                    cursor = 0;
                    break;
                case 1:
                    depth = r_s8(channel + 18) + 1;
                    value = r_u8(cursor++);
                    w_u8(channel + (uint32)depth + 36, value);
                    w_u32(channel + (uint32)depth * 4 + 20, cursor);
                    remaining = r_u32(channel + 4);
                    w_u8(channel + 18, depth);
                    break;
                case 2:
                    depth = r_s8(channel + 18);
                    repeat = r_u8(channel + (uint32)depth + 36);
                    if (!repeat) {
                        remaining = r_u32(channel + 4);
                        w_u8(channel + 18, depth - 1);
                    } else {
                        if (repeat != 255) {
                            w_u8(channel + (uint32)depth + 36, repeat - 1);
                            depth = r_s8(channel + 18);
                            remaining = r_u32(channel + 4);
                        }
                        cursor = r_u32(channel + (uint32)depth * 4 + 20);
                    }
                    break;
                case 3:
                    value = r_u8(cursor++);
                    remaining = r_u32(channel + 4);
                    w_u8(channel + 17, value);
                    break;
                case 4:
                    value = r_u8(cursor++);
                    remaining = r_u32(channel + 4);
                    value = r_u16(0x8008E888 + value * 2);
                    w_u16(channel + 8, value);
                    volume = r_u16(sequence + 6);
                    value = (uint32)((sint32)(volume * value) >> 14);
                    w_u16(channel + 10, value);
                    break;
                case 5:
                    value = r_u8(cursor++);
                    remaining = r_u32(channel + 4);
                    w_u8(channel + 16, value);
                    break;
                case 6:
                    first = r_u8(cursor);
                    third = r_u8(cursor + 2);
                    second = r_u8(cursor + 1);
                    w_u32(sequence + 8, third | (first << 16) | (second << 8));
                    remaining = r_u32(channel + 4);
                    cursor += 3;
                    break;
                default:
                    break;
                }
                if (!command) break;
            }
            value = r_u8(cursor++);
            value = sequence_variable_length(&cursor, value);
            remaining += value;
            w_u32(channel + 4, remaining);
        }
        w_u32(channel, cursor);
    }
}

sint32 gtmain_sound_ramp_initialize(uint32 ramp, sint32 current, sint32 target, sint32 milliseconds)
{
    uint32 difference, rate, magnitude;
    sint32 steps, quotient, remainder, direction;
    FUNCTION_MARKER(0x8007F2C8, "GTMAIN.EXE");
    difference = (uint32)target - (uint32)current;
    if (!difference) {
        w_u16(ramp, 0);
        return 0;
    }
    magnitude = (sint32)difference < 0 ? 0 - difference : difference;
    rate = r_u16(0x800B59A0);
    steps = (sint32)(rate * (uint32)milliseconds) / 1000;
    if (!steps) steps = 1;
    if (steps == -1 && magnitude == 0x80000000) gt_draft_exception(6);
    quotient = (sint32)magnitude / steps;
    remainder = (sint32)magnitude % steps;
    direction = remainder != 0;
    if ((sint32)difference < 0) {
        quotient = (sint32)(0 - (uint32)quotient);
        direction = -direction;
    }
    w_u16(ramp + 2, quotient);
    w_u16(ramp + 4, direction);
    w_u16(ramp + 6, remainder);
    w_u16(ramp + 8, steps);
    w_u16(ramp + 10, steps >> 1);
    w_u16(ramp, steps);
    return direction;
}

sint32 gtmain_sound_ramp_step(uint32 ramp)
{
    uint32 remaining;
    sint32 direction, delta, remainder, accumulator, steps;
    FUNCTION_MARKER(0x8007F39C, "GTMAIN.EXE");
    remaining = r_u16(ramp);
    if (!remaining) return 0;
    direction = r_s16(ramp + 4);
    delta = r_s16(ramp + 2);
    w_u16(ramp, remaining - 1);
    if (direction) {
        remainder = r_s16(ramp + 6);
        accumulator = r_s16(ramp + 10);
        w_u16(ramp + 10, accumulator - remainder);
        if (remainder < 0) {
            accumulator = r_s16(ramp + 10);
            steps = r_s16(ramp + 8);
            direction = r_s16(ramp + 4);
            w_u16(ramp + 10, accumulator + steps);
            delta += direction;
        }
    }
    return delta;
}

sint32 gtmain_sound_sequence_fade(uint32 sequence, uint16 target, sint32 milliseconds)
{
    uint32 current;
    FUNCTION_MARKER(0x8007F400, "GTMAIN.EXE");
    w_u16(sequence + 28, 0);
    current = r_u16(sequence + 4);
    return gtmain_sound_ramp_initialize(sequence + 28, current, target, milliseconds);
}

sint32 gtmain_sound_master_fade(uint16 target, sint32 milliseconds)
{
    uint32 current;
    FUNCTION_MARKER(0x8007F43C, "GTMAIN.EXE");
    current = r_u16(0x800B59A2);
    w_u16(0x800B59A8, 0);
    return gtmain_sound_ramp_initialize(0x800B59A8, current, target, milliseconds);
}

uint32 gtmain_sound_sequence_link(uint32 sequence)
{
    uint32 head;
    FUNCTION_MARKER(0x8007F488, "GTMAIN.EXE");
    head = r_u32(0x800B599C);
    w_u32(sequence, head);
    w_u32(0x800B599C, sequence);
    return 0x800B599C;
}

uint32 gtmain_sound_sequence_volume_apply(uint32 sequence)
{
    uint32 master, volume, gain, group, channel, first, second, third, fourth;
    FUNCTION_MARKER(0x8007F4A4, "GTMAIN.EXE");
    master = r_u16(0x800B59A2);
    volume = r_u16(sequence + 4);
    gain = (uint32)((sint32)(master * volume) >> 14);
    w_u16(sequence + 6, gain);
    for (group = 0; group < 4; ++group) {
        channel = sequence + 40 + group * 160;
        first = r_u16(channel + 8);
        second = r_u16(channel + 48);
        third = r_u16(channel + 88);
        fourth = r_u16(channel + 128);
        w_u16(channel + 10, (first * gain) >> 14);
        w_u16(channel + 50, (second * gain) >> 14);
        w_u16(channel + 90, (third * gain) >> 14);
        w_u16(channel + 130, (fourth * gain) >> 14);
    }
    return gain;
}

uint32 gtmain_sound_sequence_unlink(uint32 sequence)
{
    uint32 current, previous = 0, next;
    FUNCTION_MARKER(0x8007F808, "GTMAIN.EXE");
    current = r_u32(0x800B599C);
    while (current) {
        if (current == sequence) {
            next = r_u32(current);
            if (previous) w_u32(previous, next);
            else w_u32(0x800B599C, next);
            return previous;
        }
        previous = current;
        current = r_u32(current);
    }
    return previous;
}

uint32 gtmain_sound_sequence_start(uint32 sequence)
{
    FUNCTION_MARKER(0x8007F530, "GTMAIN.EXE");
    gtmain_sound_sequence_unlink(sequence);
    gtmain_sound_sequence_volume_apply(sequence);
    return gtmain_sound_sequence_link(sequence);
}

uint32 gtmain_sound_sequence_initialize(uint32 sequence, uint32 data, uint32 bank)
{
    uint32 volume, master, rate, step, tempo, index, channel, cursor, value = 0, remaining;
    FUNCTION_MARKER(0x8007F864, "GTMAIN.EXE");
    gtmain_sound_sequence_unlink(sequence);
    w_u32(sequence, 0);
    volume = r_u16(data);
    w_u16(sequence + 4, volume);
    master = r_u16(0x800B59A2);
    volume = r_u16(data);
    volume = (uint32)((sint32)(master * volume) >> 14);
    w_u16(sequence + 6, volume);
    w_u32(sequence + 24, bank);
    rate = r_u16(0x800B59A0);
    if (!rate) gt_draft_exception(7);
    step = 480000000 / rate;
    w_u32(sequence + 16, step);
    w_u32(sequence + 12, step);
    tempo = r_u32(data + 4);
    w_u32(sequence + 8, tempo);
    for (index = 0; index < 16; ++index) {
        channel = sequence + 40 + index * 40;
        cursor = r_u32(data + 8 + index * 4);
        w_u32(channel, cursor);
        cursor = r_u32(channel);
        value = r_u8(cursor);
        w_u32(channel, cursor + 1);
        w_u32(channel + 4, value);
        if (value & 0x80) {
            w_u32(channel + 4, value & 0x7F);
            do {
                cursor = r_u32(channel);
                remaining = r_u32(channel + 4);
                value = r_u8(cursor);
                remaining = (remaining << 7) + (value & 0x7F);
                w_u32(channel, cursor + 1);
                w_u32(channel + 4, remaining);
            } while (value & 0x80);
        }
        w_u16(channel + 8, 0x4000);
        volume = r_u16(sequence + 6);
        w_u16(channel + 12, 0);
        w_u8(channel + 16, 0);
        w_u8(channel + 17, 0);
        w_u8(channel + 18, 255);
        w_u16(channel + 10, volume);
    }
    return value;
}

sint32 gtmenu_record_count(void)
{
    FUNCTION_MARKER(0x80011B70, "GTMENU.EXE");
    return r_s16(0x8009B87A);
}

uint32 gtmenu_record_address(sint32 index)
{
    FUNCTION_MARKER(0x80011B90, "GTMENU.EXE");
    if (index < 0) return 0;
    return 0x8009BA0C + (uint32)index * 96;
}

sint32 gtmenu_record_counters_decrement(void)
{
    sint32 count, index;
    uint32 record;
    FUNCTION_MARKER(0x80010154, "GTMENU.EXE");
    count = gtmenu_record_count();
    if (count <= 0) return count;
    for (index = 0; index < count; ++index) {
        record = gtmenu_record_address(index);
        w_u16(record, r_u16(record) - 1);
    }
    return 0;
}

sint32 gtmenu_record_counters_increment(void)
{
    sint32 count, index;
    uint32 record;
    FUNCTION_MARKER(0x800101AC, "GTMENU.EXE");
    count = gtmenu_record_count();
    if (count <= 0) return count;
    for (index = 0; index < count; ++index) {
        record = gtmenu_record_address(index);
        w_u16(record, r_u16(record) + 1);
    }
    return 0;
}

sint32 gtmenu_angle_wrap(sint32 angle)
{
    FUNCTION_MARKER(0x800105E8, "GTMENU.EXE");
    if (angle < -2048) {
        do { angle += 4096; } while (angle < -2048);
    } else {
        while (angle >= 2048) angle -= 4096;
    }
    return angle;
}

sint32 gtmenu_angle_difference(sint32 first, sint32 second)
{
    sint32 difference;
    FUNCTION_MARKER(0x80010624, "GTMENU.EXE");
    first = gtmenu_angle_wrap(first);
    second = gtmenu_angle_wrap(second);
    difference = first - second;
    if (difference >= 2048) return difference - 4096;
    if (difference < -2048) return difference + 4096;
    return difference;
}

uint32 gtmain_pad_events_take(void)
{
    uint32 events;
    FUNCTION_MARKER(0x800807F0, "GTMAIN.EXE");
    events = r_u32(0x800B1F24);
    w_u32(0x800B1F24, 0);
    return events;
}

uint32 gtmain_runtime_finalizers(void)
{
    uint32 cursor = 0x801FB71C;
    uint32 callback;
    FUNCTION_MARKER(0x80082958, "GTMAIN.EXE");
    callback = r_u32(cursor);
    while (callback) {
        callback = r_u32(cursor);
        cursor += 4;
        gtmain_runtime_finalizer_invoke(callback);
        callback = r_u32(cursor);
    }
    return callback;
}

uint32 gtmain_game_exit(sint32 status)
{
    FUNCTION_MARKER(0x80082928, "GTMAIN.EXE");
    gtmain_runtime_finalizers();
    return gtmain_platform_exit(status);
}

uint32 gtmenu_cursor_snap(uint32 index)
{
    uint32 rectangles, rectangle, width, x, height, y;
    FUNCTION_MARKER(0x8001067C, "GTMENU.EXE");
    rectangles = r_u32(0x800A7570);
    w_u32(0x800A756C, index);
    rectangle = rectangles + index * 8;
    width = r_u16(rectangle + 4);
    x = r_u16(rectangle);
    x += width >> 1;
    height = r_u16(rectangle + 6);
    w_u32(0x800566F4, 0);
    w_u32(0x800A7560, x);
    w_u32(0x800A7574, x);
    y = r_u16(rectangle + 2);
    height >>= 1;
    y += height;
    w_u32(0x800A7568, y);
    w_u32(0x800A755C, y);
    return height;
}

uint32 gtmenu_cursor_target(uint32 index)
{
    uint32 rectangles, rectangle, width, x, height, y;
    FUNCTION_MARKER(0x800106E4, "GTMENU.EXE");
    w_u32(0x800566F4, 1);
    rectangles = r_u32(0x800A7570);
    w_u32(0x800A756C, index);
    rectangle = rectangles + index * 8;
    width = r_u16(rectangle + 4);
    x = r_u16(rectangle);
    x += width >> 1;
    height = r_u16(rectangle + 6);
    w_u32(0x800A7574, x);
    y = r_u16(rectangle + 2);
    height >>= 1;
    w_u32(0x800A755C, y + height);
    return height;
}

sint32 gtmenu_cursor_position(sint32 x, sint32 y)
{
    sint32 count, index, left, width, top, height;
    uint32 rectangles, rectangle;
    FUNCTION_MARKER(0x80010740, "GTMENU.EXE");
    count = r_s32(0x800A7564);
    w_u32(0x800A7560, x);
    w_u32(0x800A7568, y);
    w_u32(0x800A7574, x);
    w_u32(0x800A755C, y);
    w_u32(0x800A756C, 0xFFFFFFFF);
    if (count <= 0) return -1;
    rectangles = r_u32(0x800A7570);
    for (index = 0; index < count; ++index) {
        rectangle = rectangles + (uint32)index * 8;
        left = r_u16(rectangle);
        if (x < left) continue;
        width = r_u16(rectangle + 4);
        if (x >= left + width) continue;
        top = r_u16(rectangle + 2);
        if (y < top) continue;
        height = r_u16(rectangle + 6);
        if (y >= top + height) continue;
        w_u32(0x800A756C, index);
        w_u32(0x800566F4, 0);
        return index;
    }
    return -1;
}

sint32 gtmenu_cursor_nearest(sint32 x, sint32 y)
{
    sint32 count, index, selected = -1, best = 0x10000;
    sint32 dx, dy, minimum, distance;
    uint32 rectangles, rectangle, width, center_x, height, center_y;
    FUNCTION_MARKER(0x80010818, "GTMENU.EXE");
    count = r_s32(0x800A7564);
    if (count > 0) {
        rectangles = r_u32(0x800A7570);
        for (index = 0; index < count; ++index) {
            rectangle = rectangles + (uint32)index * 8;
            width = r_u16(rectangle + 4);
            center_x = r_u16(rectangle);
            center_x += width >> 1;
            height = r_u16(rectangle + 6);
            center_y = r_u16(rectangle + 2);
            center_y += height >> 1;
            dx = (sint32)((uint32)x - center_x);
            dy = (sint32)((uint32)y - center_y);
            if (dx < 0) dx = (sint32)(0u - (uint32)dx);
            minimum = dx;
            if (dy < 0) dy = (sint32)(0u - (uint32)dy);
            if (dy < minimum) minimum = dy;
            distance = (sint32)((uint32)dx + (uint32)dy - (uint32)(minimum >> 1));
            if (distance < best) {
                selected = index;
                best = distance;
            }
        }
    }
    if (selected != -1) {
        gtmenu_cursor_snap(selected);
        w_u32(0x800566F4, 0);
    }
    return selected;
}

uint32 gtmenu_cursor_step(void)
{
    uint32 moving, x, target_x, y, target_y, next_x, next_y;
    FUNCTION_MARKER(0x800108F8, "GTMENU.EXE");
    moving = r_u32(0x800566F4);
    if (!moving) return moving;
    x = r_u32(0x800A7560);
    target_x = r_u32(0x800A7574);
    if (x == target_x) {
        y = r_u32(0x800A7568);
        target_y = r_u32(0x800A755C);
        if (y == target_y) w_u32(0x800566F4, 0);
    }
    x = r_u32(0x800A7560);
    target_x = r_u32(0x800A7574);
    y = r_u32(0x800A7568);
    target_y = r_u32(0x800A755C);
    next_x = (sint32)((x << 1) + target_x) / 3;
    next_y = (sint32)((y << 1) + target_y) / 3;
    w_u32(0x800A7560, next_x);
    w_u32(0x800A7568, next_y);
    if (x == next_x) w_u32(0x800A7560, target_x);
    next_y = r_u32(0x800A7568);
    if (y == next_y) {
        next_y = r_u32(0x800A755C);
        w_u32(0x800A7568, next_y);
    }
    return next_y;
}

void gtmenu_list_initialize(uint32 list)
{
    FUNCTION_MARKER(0x80010C94, "GTMENU.EXE");
    w_u32(list, 0);
    w_u32(list + 4, 0);
    w_u32(list + 12, 0);
    w_u32(list + 8, 0);
}

void gtmenu_list_clear(uint32 list)
{
    uint32 node, next;
    FUNCTION_MARKER(0x80010CA8, "GTMENU.EXE");
    node = r_u32(list);
    while (node) {
        next = r_u32(node + 4);
        gtmenu_heap_free(node);
        node = next;
    }
    gtmenu_list_initialize(list);
}

uint32 gtmenu_list_append(uint32 list, uint32 node)
{
    uint32 tail, count;
    FUNCTION_MARKER(0x80010CFC, "GTMENU.EXE");
    if (r_u32(list) && (tail = r_u32(list + 4)) != 0) {
        w_u32(node, tail);
        tail = r_u32(list + 4);
        w_u32(tail + 4, node);
    } else {
        w_u32(list, node);
    }
    w_u32(list + 4, node);
    count = r_u32(list + 8) + 1;
    w_u32(list + 8, count);
    return count;
}

uint32 gtmenu_list_node_allocate(void)
{
    uint32 node;
    /* Original occurrences
     * GTMENU.EXE:0x80010D48
     * GTMENU.EXE:0x80019CD8
     */
    node = gtmenu_heap_alloc(24);
    if (node) gtmenu_memory_fill(node, 0, 24);
    return node;
}

sint32 gtmenu_cursor_move(uint32 direction)
{
    uint32 rectangle, vector, left, width, top, height, x, y, square_sum;
    uint32 scaled_x, scaled_y, value;
    sint32 count, selected = -1, best = 0, index, vector_x, vector_y;
    sint32 dx, dy, distance, unit_x, unit_y, dot, square, factor, score, component;
    sint64 product;
    FUNCTION_MARKER(0x800109F4, "GTMENU.EXE");
    rectangle = r_u32(0x800A7570);
    count = r_s32(0x800A7564);
    vector = 0x800566F8 + direction * 4;
    vector_x = r_s16(vector);
    x = r_u32(0x800A7560);
    y = r_u32(0x800A7568);
    vector_y = r_s16(vector + 2);
    for (index = 0; index < count; ++index, rectangle += 8) {
        left = r_u16(rectangle);
        width = r_u16(rectangle + 4);
        top = r_u16(rectangle + 2);
        height = r_u16(rectangle + 6);
        if (x - left < width && y - top < height) continue;
        dx = (sint32)(left + (width >> 1) - x);
        dy = (sint32)(top + (height >> 1) - y);
        square_sum = (uint32)dx * (uint32)dx + (uint32)dy * (uint32)dy;
        distance = gtmenu_square_root(square_sum, 0);
        if (!distance) continue;
        scaled_x = (uint32)dx << 12;
        if (distance == -1 && scaled_x == 0x80000000) gt_draft_exception(6);
        unit_x = (sint32)scaled_x / distance;
        scaled_y = (uint32)dy << 12;
        if (distance == -1 && scaled_y == 0x80000000) gt_draft_exception(6);
        unit_y = (sint32)scaled_y / distance;
        component = (sint32)((uint32)vector_x * (uint32)unit_x) >> 12;
        dot = (sint32)((uint32)component + (uint32)((sint32)((uint32)vector_y * (uint32)unit_y) >> 12));
        factor = 0x1000000 / distance;
        if (dot < 0) continue;
        square = (sint32)((uint32)dot * (uint32)dot) >> 12;
        product = (sint64)factor * square;
        score = (sint32)(product >> 12);
        if (best < score) {
            best = score;
            selected = index;
        }
    }
    if (selected >= 0) {
        gtmenu_cursor_target(selected);
    } else {
        w_u32(0x800566F4, 1);
        component = r_s16(vector);
        value = r_u32(0x800A7560);
        w_u32(0x800A7560, value + (uint32)(component >> 8));
        component = r_s16(vector + 2);
        value = r_u32(0x800A7568);
        w_u32(0x800A7568, value + (uint32)(component >> 8));
    }
    return selected;
}

uint32 gtmenu_resource_group(uint32 index)
{
    uint32 archive, offset;
    FUNCTION_MARKER(0x80012A04, "GTMENU.EXE");
    archive = r_u32(0x80080558);
    offset = r_u32(archive + index * 12 + 16);
    return archive + offset;
}

sint32 gtmenu_catalog_record_count(void)
{
    uint32 catalog;
    FUNCTION_MARKER(0x80014034, "GTMENU.EXE");
    catalog = r_u32(0x800805D0);
    return r_s16(catalog + 538);
}

uint32 gtmenu_catalog_record_address(sint32 index)
{
    uint32 catalog;
    FUNCTION_MARKER(0x8001404C, "GTMENU.EXE");
    catalog = r_u32(0x800805D0);
    return (uint32)index * 96 + 940 + catalog;
}

sint32 gtmenu_catalog_nodes_build(void)
{
    sint32 count, index;
    uint32 models, record, node, model;
    FUNCTION_MARKER(0x80010D88, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    count = gtmenu_catalog_record_count();
    if (count <= 0) return count;
    for (index = 0; index < count; ++index) {
        record = gtmenu_catalog_record_address(index);
        node = gtmenu_list_node_allocate();
        w_u8(node + 8, 0);
        w_u8(node + 9, 2);
        model = r_u16(record);
        w_u16(node + 10, index);
        w_u32(node + 16, 0);
        w_u8(node + 21, 0);
        w_u32(node + 12, models + (model & 0xFFF) * 424);
        gtmenu_list_append(0x8007DA40, node);
    }
    return 0;
}

sint32 gtmenu_record_nodes_build(uint8 kind)
{
    sint32 count, index;
    uint32 models, record, node, model;
    FUNCTION_MARKER(0x80010E5C, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    count = gtmenu_record_count();
    if (count <= 0) return count;
    for (index = 0; index < count; ++index) {
        record = gtmenu_record_address(index);
        node = gtmenu_list_node_allocate();
        w_u8(node + 8, 0);
        w_u8(node + 9, kind);
        model = r_u16(record);
        w_u16(node + 10, index);
        w_u32(node + 16, 0);
        w_u8(node + 21, 0);
        w_u32(node + 12, models + (model & 0xFFF) * 424);
        gtmenu_list_append(0x8007DA40, node);
    }
    return 0;
}

uint32 gtmenu_offer_nodes_build(sint8 group, uint8 kind)
{
    uint32 models, offers, table, entry, model_index, model, node, price, result;
    sint32 period, index, count;
    FUNCTION_MARKER(0x80010F28, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    period = gtmenu_offer_period();
    gtmenu_offer_archive_bind();
    offers = gtmenu_offer_period_table(period);
    table = gtmenu_offer_group_table(group, offers);
    result = r_u32(table);
    if ((sint32)result <= 0) return result;
    index = 0;
    do {
        entry = table + (uint32)index * 4;
        model_index = r_u16(entry + 6) & 0x1FF;
        model = models + model_index * 424;
        if (gtmenu_model_name_index(model) >= 0 && !r_u8(model + 417)) {
            node = gtmenu_list_node_allocate();
            w_u8(node + 8, 0);
            w_u8(node + 9, kind);
            w_u32(node + 12, model);
            w_u16(node + 10, model_index);
            price = r_u16(entry + 4) * 1000;
            w_u32(node + 16, price);
            w_u8(node + 21, price == 0);
            w_u8(node + 20, r_u16(table + (uint32)index * 4 + 6) >> 9);
            gtmenu_list_append(0x8007DA40, node);
        }
        count = r_s32(table);
        ++index;
        result = (uint32)index * 4;
    } while (index < count);
    return result;
}

uint32 gtmenu_model_nodes_build(sint8 make, uint8 kind, sint8 group, uint32 flags)
{
    uint32 archive, models, model, node, stored_model, result, count, index = 0;
    FUNCTION_MARKER(0x800110A8, "GTMENU.EXE");
    archive = gtmenu_resource_group(13);
    models = archive + 24;
    result = r_u16(archive + 14);
    if (!result) return result;
    do {
        model = models + index * 424;
        if (r_s8(model) == make && r_s8(model + 4) == group &&
            r_s8(model + 416) == 48 && r_u8(model + 417) == flags &&
            gtmenu_model_name_index(model) >= 0) {
            node = gtmenu_list_node_allocate();
            w_u32(node + 12, model);
            stored_model = r_u32(node + 12);
            w_u8(node + 8, 0);
            w_u8(node + 9, kind);
            w_u16(node + 10, index);
            w_u32(node + 16, 0);
            w_u8(node + 21, r_u32(stored_model + 388) == 0);
            gtmenu_list_append(0x8007DA40, node);
        }
        count = r_u16(archive + 14);
        ++index;
        result = index * 2;
    } while (index < count);
    return result;
}

sint32 gtmenu_days_get(void)
{
    FUNCTION_MARKER(0x8001176C, "GTMENU.EXE");
    return r_s32(0x8009B868);
}

sint32 gtmenu_offer_period(void)
{
    FUNCTION_MARKER(0x8001179C, "GTMENU.EXE");
    return gtmenu_days_get() % 600 / 10;
}

uint32 gtmenu_offer_archive_bind(void)
{
    uint32 archive, offset, offers;
    FUNCTION_MARKER(0x800128B0, "GTMENU.EXE");
    archive = r_u32(0x800A7AB4);
    offset = r_u32(archive + 64);
    offers = archive + offset;
    w_u32(0x8008055C, offers);
    return offers;
}
