#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include <stdio.h>

uint32 gtmain_packet_triangle_left(uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    uint32 result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003B690, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x28, 0x8003B690);
    w_u16(frame.address + 0x10, x);
    w_u16(frame.address + 0x12, y);
    w_u16(frame.address + 0x14, (uint32)x + 6);
    w_u16(frame.address + 0x18, (uint32)x + 6);
    w_u16(frame.address + 0x1A, (uint32)y + 6);
    w_u16(frame.address + 0x16, (uint32)y - 6);
    result = gtmain_packet_flat_triangle(ordering, frame.address + 0x10, color);
    gt_local_stack_pop(frame);
    return result;
}

sint32 gtmain_menu_name_field_draw(uint32 ordering, uint32 request, sint32 x,
    sint32 y, sint32 intensity)
{
    uint32 left, text, cursor, saved, pulse, width, arrow_x, bottom;
    uint32 staged, corners, first_word, second_word, color;
    sint32 result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003C6C8, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x48, 0x8003C6C8);
    staged = frame.address + 0x18;
    corners = frame.address + 0x10;
    left = (uint32)x - (r_u16(request + 8) >> 1);
    pulse = gtmain_menu_pulse_component(2, intensity);
    text = r_u32(request + 4);
    cursor = text + (uint32)(sint32)r_s8(request);
    saved = r_u8(cursor);
    pulse &= 255;
    w_u8(cursor, 0);
    width = gtmain_text_width(2, text);
    arrow_x = left + width;
    w_u8(cursor, saved);
    gtmain_packet_triangle_up(ordering, arrow_x, (uint32)y + 27, pulse);
    gtmain_packet_triangle_down(ordering, arrow_x, (uint32)y + 8, pulse);
    bottom = (uint32)y + 26;
    w_u16(staged, left);
    w_u16(staged + 2, bottom);
    width = r_u16(request + 8);
    w_u16(staged + 6, bottom);
    w_u16(staged + 4, width + left - 1);
    first_word = r_u32(staged);
    second_word = r_u32(staged + 4);
    w_u32(corners, first_word);
    w_u32(corners + 4, second_word);
    color = gtmain_menu_gray(intensity >> 2);
    gtmain_packet_outline(ordering, corners, color);
    gtmain_text_flags(2);
    text = r_u32(request + 4);
    result = gtmain_font_text_draw(ordering, left, (uint32)y + 8, text);
    gt_local_stack_pop(frame);
    return result;
}

sint32 gtmain_menu_scene_model_draw(uint32 ordering, sint16 x, sint16 y,
    sint16 camera_y, sint16 camera_x, sint16 camera_z,
    sint16 model_y, sint16 model_x, sint16 model_z,
    uint32 position_x, uint32 position_y, uint32 position_z,
    uint32 offset_y, uint32 model, uint32 unused,
    uint16 palette, uint32 command, uint16 page, sint16 projection,
    uint16 scale_x, uint16 scale_y)
{
    uint32 view, vector, matrix, temporary, first, second, third;
    sint32 result;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003CF14, "GTMAIN.EXE");
    (void)unused;
    frame = gt_local_stack_push(0x110, 0x8003CF14);
    view = frame.address + 0x10;
    vector = frame.address + 0x88;
    matrix = frame.address + 0x9C;
    temporary = frame.address + 0xC8;
    w_u32(frame.address + 0xD8, ordering);
    w_u16(view + 4, 320);
    w_u16(view + 6, 240);
    w_u32(frame.address + 0xE0, 0x1F800360);
    w_u16(view, 0);
    w_u16(view + 2, 0);
    w_u16(view + 8, x);
    w_u16(view + 10, y);
    w_u16(view + 14, 64);
    w_u16(view + 16, 64);
    w_u16(view + 12, projection);
    gtmain_view_projection_prepare(view);
    gtmain_view_projection_prepare(view);
    gtmain_render_context_init(vector);
    gtmain_model_rotation((uint8 *)psx_addr(matrix, 20), camera_x, camera_y, camera_z);
    w_u32(vector + 4, 52428);
    w_u32(vector, 0);
    w_u32(vector + 8, 0xFFF40000);
    gtmain_model_rotate_vector((uint8 *)psx_addr(vector, 12),
        (const uint8 *)psx_addr(matrix, 20), (const uint8 *)psx_addr(vector, 12));
    gtmain_model_orientation(view, 0x80099490);
    gtmain_render_view_copy(0x1F800000, view);
    gtmain_render_fog_set(0x1F800000, 0, 256, 0);
    first = r_u16(0x1F800020);
    second = r_u32(0x1F800018);
    xport_gte_write_control(26, first);
    third = r_u32(0x1F80001C);
    xport_gte_write_control(24, second);
    xport_gte_write_control(25, third);
    w_u32(0x1F800390, 0x10000000);
    w_u16(0x1F80038E, 6);
    w_u32(0x1F800388, (uint32)palette << 16);
    w_u32(0x1F800394, (uint32)page | ((command | 32) << 16));
    w_u16(0x1F80039C, 0);
    w_u16(0x1F800398, scale_x);
    w_u16(0x1F80039A, scale_y);
    gtmain_render_context_init(0x1F800360);
    gtmain_model_rotation((uint8 *)psx_addr(0x1F800374, 20), model_x, model_y, model_z);
    first = r_u32(frame.address + 0xE0);
    w_u32(first, position_x);
    w_u32(0x1F800364, position_y);
    w_u32(temporary, 0);
    w_u32(temporary + 8, 0);
    w_u32(0x1F800368, position_z);
    w_u32(temporary + 4, offset_y);
    gtmain_model_rotate_vector((uint8 *)psx_addr(temporary, 12),
        (const uint8 *)psx_addr(0x1F800374, 20), (const uint8 *)psx_addr(temporary, 12));
    gtmain_vector_subtract((uint8 *)psx_addr(0x1F800360, 12),
        (const uint8 *)psx_addr(0x1F800360, 12), (const uint8 *)psx_addr(temporary, 12));
    ordering = r_u32(frame.address + 0xD8);
    w_u16(0x1F80038C, 0);
    w_u32(0x1F8003A0, ordering);
    result = gtmain_model_transform_draw(0x1F800000, model + 60, 1);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_menu_draw_callback_invoke(GTMenuDrawCallback callback, uint32 ordering,
    sint32 x, sint32 y, sint32 intensity)
{
    FUNCTION_MARKER(0x8003D8F4, "GTMAIN.EXE");
    return callback(ordering, x, y, intensity);
}

uint32 gtmain_menu_caption_style(sint32 intensity)
{
    uint32 color;
    FUNCTION_MARKER(0x8003D928, "GTMAIN.EXE");
    gtmain_text_flags(2);
    gtmain_text_font(2);
    color = gtmain_menu_gray(intensity >> 1);
    return gtmain_text_color(color);
}

uint32 gtmain_menu_one_line_caption_draw(uint32 ordering, sint32 x, sint32 y,
    uint32 text, sint32 intensity)
{
    sint32 width;
    FUNCTION_MARKER(0x8003D968, "GTMAIN.EXE");
    gtmain_menu_caption_style(intensity);
    width = gtmain_menu_caption_width(text);
    return gtmain_menu_caption_plain_draw(ordering,
        (uint32)x - ((uint32)(width >> 1) - 160), (uint32)y + 114, text);
}

uint32 gtmain_menu_selection_peak_update(uint32 state, uint32 values, uint32 detail)
{
    uint32 count, value, previous, changed;
    FUNCTION_MARKER(0x8004D78C, "GTMAIN.EXE");
    count = r_u32(values);
    value = r_u16(values + count * 2 + 16);
    if (r_s32(state) < (sint32)value) w_u32(state, value);
    value = r_u16(detail);
    previous = r_u16(state + 4);
    if (previous < value) w_u16(state + 4, value);
    value = r_u16(detail + 4);
    previous = r_u16(state + 6);
    changed = previous < value;
    if (changed) w_u16(state + 6, value);
    return changed;
}

uint32 gtmain_menu_selection_peak_reset(uint32 state)
{
    FUNCTION_MARKER(0x8004D7F8, "GTMAIN.EXE");
    w_u32(state, 1);
    w_u16(state + 4, 1);
    w_u16(state + 6, 1);
    return 1;
}

uint32 gtmain_menu_parts_parameters_apply(uint32 category)
{
    uint32 cursor = 0x80092904, callback;
    sint32 entry;
    FUNCTION_MARKER(0x80042C48, "GTMAIN.EXE");
    for (;;) {
        entry = r_s16(cursor);
        if (entry < 0) break;
        w_u16(0x800BEA20, category);
        if (category == (uint32)entry) {
            callback = r_u32(cursor + 8);
            gtmain_menu_callback(callback);
        }
        cursor += 12;
    }
    return (uint32)printf((const char *)psx_addr(0x8008D8A8, 20));
}

sint32 gtmain_parts_parameter_level(sint32 minimum, sint32 maximum, sint32 value, sint32 count)
{
    sint32 width, distance, steps, product, level, restored, interval;
    FUNCTION_MARKER(0x800417AC, "GTMAIN.EXE");
    width = (sint32)((uint32)maximum - (uint32)minimum);
    distance = (sint32)((uint32)value - (uint32)minimum);
    if (!width) return 0;
    steps = (sint32)((uint32)count - 1);
    product = (sint32)((uint32)distance * (uint32)steps);
    if (width == -1 && product == (-2147483647 - 1)) gt_draft_exception(6);
    level = product / width;
    product = (sint32)((uint32)level * (uint32)width);
    if (!steps) gt_draft_exception(7);
    if (steps == -1 && product == (-2147483647 - 1)) gt_draft_exception(6);
    restored = product / steps;
    if (steps == -1 && width == (-2147483647 - 1)) gt_draft_exception(6);
    interval = width / steps;
    product = (sint32)((uint32)interval * (uint32)steps);
    if (restored != distance) {
        printf((const char *)psx_addr(0x8008D478, 68));
        printf((const char *)psx_addr(0x8008D4BC, 49), value,
            (sint32)((uint32)level + 1), (sint32)((uint32)minimum + (uint32)restored));
        printf((const char *)psx_addr(0x8008D478, 68));
        product = (sint32)((uint32)interval * (uint32)steps);
    }
    if (product != width) {
        printf((const char *)psx_addr(0x8008D478, 68));
        printf((const char *)psx_addr(0x8008D4F0, 46), minimum, maximum, count);
        printf((const char *)psx_addr(0x8008D478, 68));
    }
    if (level < 0) level = 0;
    if (level >= count) level = steps;
    return level;
}

uint32 gtmain_parts_rear_stabilizer_apply(void)
{
    uint32 table, offset, context, part, record, minimum, maximum, value, count;
    sint32 category, level;
    FUNCTION_MARKER(0x80042598, "GTMAIN.EXE");
    category = r_s16(0x800BEA20);
    table = gtmain_parts_table(category);
    category = r_s16(0x800BEA20);
    offset = r_u8(0x800941BD + (uint32)category);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    if (!part) return 0xFFFFFFFF;
    record = table + 20 * (part - 1);
    minimum = r_u8(record + 2);
    maximum = r_u8(record + 3);
    value = r_u8(record);
    count = r_u8(record + 1);
    level = gtmain_parts_parameter_level(minimum, maximum, value, count);
    context = r_u32(0x800BEA64);
    w_u8(context + 88, level);
    context = r_u32(0x800BEA64);
    value = r_u8(record);
    w_u8(context + 79, value);
    printf((const char *)psx_addr(0x8008D520, 59));
    category = r_s16(0x800BEA20);
    printf((const char *)psx_addr(0x8008D55C, 55), category, part);
    context = r_u32(0x800BEA64);
    count = r_u8(record + 1);
    value = r_u8(context + 79);
    printf((const char *)psx_addr(0x8008D720, 37), value, (sint32)((uint32)level + 1), count);
    minimum = r_u8(record + 2);
    maximum = r_u8(record + 3);
    printf((const char *)psx_addr(0x8008D6FC, 36), minimum, maximum);
    return 2;
}

uint32 gtmain_parts_clutch_apply(void)
{
    uint32 table, context, offset, part, record, value;
    sint32 category;
    FUNCTION_MARKER(0x80042B08, "GTMAIN.EXE");
    category = r_s16(0x800BEA20);
    table = gtmain_parts_table(category);
    category = r_s16(0x800BEA20);
    context = r_u32(0x800BEA64);
    offset = r_u8(0x800941BD + (uint32)category);
    part = r_u16(context + offset);
    if (!part) return 0xFFFFFFFF;
    record = table + 32 * (part - 1);
    value = r_u8(record + 8);
    w_u8(context + 65, value);
    context = r_u32(0x800BEA64);
    value = r_u8(record + 9);
    w_u8(context + 78, value);
    printf((const char *)psx_addr(0x8008D520, 59));
    category = r_s16(0x800BEA20);
    printf((const char *)psx_addr(0x8008D55C, 55), category, part);
    context = r_u32(0x800BEA64);
    value = r_u8(context + 65);
    printf((const char *)psx_addr(0x8008D878, 21), value);
    context = r_u32(0x800BEA64);
    value = r_u8(context + 78);
    printf((const char *)psx_addr(0x8008D890, 21), value);
    return 5;
}

sint32 gtmain_parts_parameter_value(sint32 minimum, sint32 maximum, sint32 level, sint32 count)
{
    sint32 product, divisor, result;
    FUNCTION_MARKER(0x800419FC, "GTMAIN.EXE");
    product = (sint32)((uint32)level * ((uint32)maximum - (uint32)minimum));
    divisor = (sint32)((uint32)count - 1);
    if (!divisor) gt_draft_exception(7);
    if (divisor == -1 && product == (-2147483647 - 1)) gt_draft_exception(6);
    result = (sint32)((uint32)minimum + (uint32)(product / divisor));
    if (result < minimum) result = minimum;
    if (maximum < result) result = maximum;
    return result;
}

sint32 gtmain_parts_kind(uint32 category, uint32 selection, uint32 archive)
{
    uint32 record, offset;
    FUNCTION_MARKER(0x8006C0B8, "GTMAIN.EXE");
    record = gtmain_parts_record(category, selection, archive);
    offset = r_u8(0x8009418D + category);
    return r_s8(record + offset);
}

uint32 gtmain_parts_suspension_options_initialize(void)
{
    uint32 first, second, third, fourth, offset, context, part, record;
    uint32 minimum, maximum, count, value, front = 0, rear = 0;
    sint32 category, kind, level;
    FUNCTION_MARKER(0x80041A68, "GTMAIN.EXE");
    first = r_u8(0x800927DC);
    w_u8(0x800927DC, first | 32);
    second = r_u8(0x800927E4);
    third = r_u8(0x800927EC);
    w_u8(0x800927E4, second | 32);
    fourth = r_u8(0x800927F4);
    w_u8(0x800927EC, third | 32);
    category = r_s16(0x800BEA20);
    w_u8(0x800927F4, fourth | 32);
    offset = r_u8(0x800941BD + (uint32)category);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    if (part) {
        record = gtmain_parts_table(33) + part * 72;
        minimum = r_u8(record - 42);
        maximum = r_u8(record - 41);
        context = r_u32(0x800BEA60);
        count = r_u8(record - 34);
        value = r_u8(context + 115);
        level = gtmain_menu_range_index(minimum, maximum, value, count);
        front = (uint32)level + 1;
        minimum = r_u8(record - 27);
        maximum = r_u8(record - 26);
        context = r_u32(0x800BEA60);
        count = r_u8(record - 19);
        value = r_u8(context + 127);
        level = gtmain_menu_range_index(minimum, maximum, value, count);
        rear = (uint32)level + 1;
        kind = gtmain_parts_kind(33, part, 0x800D6FDC);
        if (kind == 2) {
            first = r_u8(0x800927DC);
            w_u8(0x800927DC, first & 31);
        }
        if (kind == 1 || kind == 2) {
            second = r_u8(0x800927E4);
            w_u8(0x800927E4, second & 31);
        }
        if (kind == 0 || kind == 1 || kind == 2) {
            third = r_u8(0x800927EC);
            fourth = r_u8(0x800927F4);
            w_u8(0x800927EC, third & 31);
            w_u8(0x800927F4, fourth & 31);
        }
    }
    context = r_u32(0x800BEA60);
    value = r_u8(context + 110);
    w_u16(0x80092798, value);
    value = r_u8(context + 122);
    w_u16(0x8009279A, value);
    value = r_u8(context + 278);
    w_u16(0x800927A4, value);
    value = r_u8(context + 279);
    w_u16(0x800927A6, value);
    value = r_u8(context + 108);
    w_u16(0x800927BC, value);
    value = r_u8(context + 109);
    w_u16(0x800927B0, front);
    w_u16(0x800927B2, rear);
    w_u16(0x800927BE, value);
    return 0;
}

uint32 gtmain_parts_front_stabilizer_options_for_category(sint32 category)
{
    uint32 offset, context, part, record, minimum, maximum, count, value, flags, level = 0;
    sint32 index;
    FUNCTION_MARKER(0x80041C88, "GTMAIN.EXE");
    offset = r_u8(0x800941BD + (uint32)category);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    if (part) {
        record = gtmain_parts_table(31) + 20 * part;
        minimum = r_u8(record - 18);
        context = r_u32(0x800BEA60);
        maximum = r_u8(record - 17);
        count = r_u8(record - 19);
        value = r_u8(context + 111);
        index = gtmain_menu_range_index(minimum, maximum, value, count);
        flags = r_u8(0x800927FC);
        level = (uint32)index + 1;
        w_u8(0x800927FC, flags & 31);
    } else {
        flags = r_u8(0x800927FC);
        w_u8(0x800927FC, flags | 32);
    }
    w_u16(0x800927C8, level);
    return 1;
}

uint32 gtmain_parts_rear_stabilizer_options_initialize(void)
{
    sint32 category;
    FUNCTION_MARKER(0x80041D48, "GTMAIN.EXE");
    category = r_s16(0x800BEA20);
    return gt_draft_80041D50(category);
}

uint32 gtmain_parts_brake_options_for_category(sint32 category)
{
    uint32 offset, context, part, record, minimum, maximum, count, value, flags;
    sint32 index;
    FUNCTION_MARKER(0x80041DFC, "GTMAIN.EXE");
    offset = r_u8(0x800941BD + (uint32)category);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    if (part) {
        record = gtmain_parts_table(17) + part * 28;
        minimum = r_u8(record - 26);
        maximum = r_u8(record - 25);
        context = r_u32(0x800BEA60);
        count = r_u8(record - 24);
        value = r_u8(context + 46);
        index = gtmain_menu_range_index(minimum, maximum, value, count);
        w_u16(0x800927D4, (uint32)index + 1);
        minimum = r_u8(record - 23);
        maximum = r_u8(record - 22);
        context = r_u32(0x800BEA60);
        count = r_u8(record - 21);
        value = r_u8(context + 47);
        index = gtmain_menu_range_index(minimum, maximum, value, count);
        flags = r_u8(0x80092804);
        w_u16(0x800927D6, (uint32)index + 1);
        w_u8(0x80092804, flags & 31);
    } else {
        flags = r_u8(0x80092804);
        w_u16(0x800927D4, 0);
        w_u16(0x800927D6, 0);
        w_u8(0x80092804, flags | 32);
    }
    return 2;
}

uint32 gtmain_parts_turbo_options_initialize(void)
{
    uint32 table, offset, context, part, record, flags, value, minimum, level = 0;
    FUNCTION_MARKER(0x80041EFC, "GTMAIN.EXE");
    table = gtmain_parts_table(35);
    offset = r_u8(0x800941E0);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    if (part) {
        flags = r_u8(0x80092898);
        w_u8(0x80092898, flags & 31);
        record = table + (part - 1) * 56;
        value = r_u8(context + 63);
        minimum = r_u8(record + 23);
        level = value - minimum + 1;
    } else {
        flags = r_u8(0x80092898);
        w_u8(0x80092898, flags | 32);
    }
    w_u16(0x80092840, level);
    return 2;
}

uint32 gtmain_parts_spring_ranges_initialize(void)
{
    uint32 table, offset, context, current, part, record, value;
    FUNCTION_MARKER(0x80042CB8, "GTMAIN.EXE");
    table = gtmain_parts_table(33);
    offset = r_u8(0x800941DE);
    context = r_u32(0x800BEA64);
    current = r_u8(context + 69);
    part = r_u16(context + offset);
    w_u16(0x8009296C, current);
    current = r_u8(context + 82);
    w_u16(0x8009297C, current);
    record = table + (part - 1) * 72;
    value = r_u8(record + 28);
    w_u16(0x80092968, value);
    value = r_u8(record + 29);
    w_u16(0x8009296A, value);
    value = r_u8(record + 43);
    w_u16(0x80092978, value);
    value = r_u8(record + 44);
    w_u16(0x8009297A, value);
    return value;
}

uint32 gtmain_parts_ride_height_ranges_initialize(void)
{
    uint32 table, offset, context, current, part, record, value;
    FUNCTION_MARKER(0x80042D50, "GTMAIN.EXE");
    table = gtmain_parts_table(33);
    offset = r_u8(0x800941DE);
    context = r_u32(0x800BEA64);
    current = r_u8(context + 68);
    part = r_u16(context + offset);
    w_u16(0x8009298C, current);
    current = r_u8(context + 81);
    w_u16(0x8009299C, current);
    record = table + (part - 1) * 72;
    value = r_u8(record + 26);
    w_u16(0x80092988, value);
    value = r_u8(record + 27);
    w_u16(0x8009298A, value);
    value = r_u8(record + 41);
    w_u16(0x80092998, value);
    value = r_u8(record + 42);
    w_u16(0x8009299A, value);
    return value;
}

uint32 gtmain_parts_damper_ranges_initialize(void)
{
    uint32 table, offset, context, part, current, record, value;
    FUNCTION_MARKER(0x80042DE8, "GTMAIN.EXE");
    table = gtmain_parts_table(33);
    offset = r_u8(0x800941DE);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    current = r_u8(context + 76);
    w_u16(0x800929A8, 1);
    w_u16(0x800929AC, current + 1);
    record = table + (part - 1) * 72;
    value = r_u8(record + 38);
    w_u16(0x800929AA, value);
    current = r_u8(context + 89);
    w_u16(0x800929B8, 1);
    w_u16(0x800929BC, current + 1);
    value = r_u8(record + 53);
    w_u16(0x800929BA, value);
    return value;
}

uint32 gtmain_parts_turbo_range_initialize(void)
{
    uint32 table, offset, context, current, part, record, value;
    FUNCTION_MARKER(0x800430BC, "GTMAIN.EXE");
    table = gtmain_parts_table(35);
    offset = r_u8(0x800941E0);
    context = r_u32(0x800BEA64);
    current = r_u8(context + 63);
    part = r_u16(context + offset);
    record = table + (part - 1) * 56;
    w_u16(0x80092A2C, current);
    value = r_u8(record + 23);
    w_u16(0x80092A28, value);
    w_u16(0x80092A30, value - 1);
    value = r_u8(record + 24);
    w_u16(0x80092A2A, value);
    return value;
}

uint32 gtmain_parts_gear_ranges_initialize(void)
{
    uint32 table, offset, context, part, record, current, minimum, maximum;
    uint32 index, descriptor, count;
    FUNCTION_MARKER(0x80043140, "GTMAIN.EXE");
    table = gtmain_parts_table(23);
    offset = r_u8(0x800941D4);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    w_u32(0x80092A34, 0x800BD280);
    current = r_u16(context + 46);
    record = table + (part - 1) * 68;
    for (index = 0; index < 7; ++index) {
        descriptor = 0x80092A34 + index * 16;
        w_u16(descriptor + 8, current);
        minimum = r_u16(record + 18 + index * 4);
        w_u16(descriptor + 4, minimum);
        maximum = r_u16(record + 20 + index * 4);
        if (index < 6) w_u32(descriptor + 16, 0x800BD284 + index * 4);
        w_u16(descriptor + 6, maximum);
        if (index < 6) current = r_u16(context + 48 + index * 2);
    }
    count = r_u8(record);
    descriptor = 0x80092A34 + count * 16;
    w_u8(0x80092B08, count + 1);
    w_u32(descriptor, 0x800BD29C);
    current = r_u16(context + 60);
    w_u16(descriptor + 8, current);
    minimum = r_u16(record + 46);
    w_u16(descriptor + 4, minimum);
    maximum = r_u16(record + 48);
    w_u16(descriptor + 6, maximum);
    return descriptor;
}

/* Unindexed original entry GTMAIN.EXE:80041FB0, frozen inventory unchanged */
uint32 gtmain_parts_transmission_options_initialize(void)
{
    sint32 category;
    uint32 offset, context, part, flags;
    category = r_s16(0x800BEA20);
    offset = r_u8(0x800941BD + (uint32)category);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    flags = r_u8(0x800928A0);
    flags = part ? flags & 31 : flags | 32;
    return gtmain_parts_transmission_options_finish(flags);
}

/* Unindexed original entry GTMAIN.EXE:800420CC, frozen inventory unchanged */
uint32 gtmain_parts_clutch_options_initialize(void)
{
    sint32 category;
    uint32 offset, context, part, flags, value;
    category = r_s16(0x800BEA20);
    offset = r_u8(0x800941BD + (uint32)category);
    context = r_u32(0x800BEA64);
    part = r_u16(context + offset);
    flags = r_u8(0x800928D4);
    w_u8(0x800928D4, part ? flags & 31 : flags | 32);
    context = r_u32(0x800BEA60);
    value = r_u8(context + 136);
    w_u16(0x800928CC, value);
    value = r_u8(context + 137);
    w_u16(0x800928CE, value);
    return 4;
}

sint32 gtmain_parts_all_options_initialize(void)
{
    uint32 cursor = 0x80092904, callback;
    sint32 category;
    FUNCTION_MARKER(0x80042BF8, "GTMAIN.EXE");
    for (;;) {
        category = r_s16(cursor);
        if (category < 0) return category;
        callback = r_u32(cursor + 4);
        w_u16(0x800BEA20, category);
        cursor += 12;
        gtmain_menu_callback(callback);
    }
}

uint32 gtmain_parts_camber_apply(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x80043594, "GTMAIN.EXE");
    context = r_u32(0x800BEA64);
    value = r_u16(0x800929CC);
    w_u8(context + 67, value);
    context = r_u32(0x800BEA64);
    value = r_u16(0x800929DC);
    w_u8(context + 80, value);
    return value;
}

uint32 gtmain_parts_stabilizer_levels_apply(void)
{
    uint32 front_table, rear_table, front_offset, rear_offset, context;
    uint32 front_part, rear_part, record, minimum, maximum, count, result;
    sint32 front_index, rear_index, level;
    FUNCTION_MARKER(0x800435C4, "GTMAIN.EXE");
    front_table = gtmain_parts_table(31);
    rear_table = gtmain_parts_table(32);
    front_offset = r_u8(0x800941DC);
    context = r_u32(0x800BEA64);
    rear_offset = r_u8(0x800941DD);
    front_part = r_u16(context + front_offset);
    rear_part = r_u16(context + rear_offset);
    front_index = (sint32)front_part - 1;
    rear_index = (sint32)rear_part - 1;
    result = rear_part;
    if (front_index >= 0) {
        level = r_s16(0x800929EC) - 1;
        record = front_table + (uint32)front_index * 20;
        w_u8(context + 75, level);
        minimum = r_u8(record + 2);
        maximum = r_u8(record + 3);
        count = r_u8(record + 1);
        result = gtmain_parts_parameter_value(minimum, maximum, level, count);
        context = r_u32(0x800BEA64);
        w_u8(context + 66, result);
    }
    if (rear_index >= 0) {
        level = r_s16(0x800929FC) - 1;
        context = r_u32(0x800BEA64);
        record = rear_table + (uint32)rear_index * 20;
        w_u8(context + 88, level);
        minimum = r_u8(record + 2);
        maximum = r_u8(record + 3);
        count = r_u8(record + 1);
        result = gtmain_parts_parameter_value(minimum, maximum, level, count);
        context = r_u32(0x800BEA64);
        w_u8(context + 79, result);
    }
    return result;
}

uint32 gtmain_parts_brake_levels_apply(void)
{
    uint32 table, offset, context, part, record, minimum, maximum, count, result;
    sint32 level;
    FUNCTION_MARKER(0x800436D8, "GTMAIN.EXE");
    table = gtmain_parts_table(17);
    offset = r_u8(0x800941CE);
    context = r_u32(0x800BEA64);
    level = r_s16(0x80092A0C);
    part = r_u16(context + offset);
    --level;
    w_u8(context + 74, level);
    record = table + (part - 1) * 28;
    minimum = r_u8(record + 2);
    maximum = r_u8(record + 3);
    count = r_u8(record + 4);
    result = gtmain_parts_parameter_value(minimum, maximum, level, count);
    context = r_u32(0x800BEA64);
    w_u8(context + 64, result);
    level = r_s16(0x80092A1C);
    context = r_u32(0x800BEA64);
    --level;
    w_u8(context + 87, level);
    minimum = r_u8(record + 5);
    maximum = r_u8(record + 6);
    count = r_u8(record + 7);
    result = gtmain_parts_parameter_value(minimum, maximum, level, count);
    context = r_u32(0x800BEA64);
    w_u8(context + 77, result);
    return result;
}

uint32 gtmain_parts_turbo_selection_apply(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x80043798, "GTMAIN.EXE");
    context = r_u32(0x800BEA64);
    value = r_u16(0x80092A2C);
    return gtmain_parts_turbo_value_apply(context, value);
}

uint32 gtmain_parts_turbo_value_apply(uint32 context, uint32 value)
{
    FUNCTION_MARKER(0x800437A8, "GTMAIN.EXE");
    w_u8(context + 63, value);
    return gtmain_player_configuration_build();
}

uint32 gtmain_parts_gear_values_apply(void)
{
    sint32 count;
    uint32 index = 0, value, context;
    FUNCTION_MARKER(0x800437C8, "GTMAIN.EXE");
    count = r_s8(0x80092B08) - 1;
    if (count > 0) {
        do {
            value = r_u16(0x80092A3C + index * 16);
            context = r_u32(0x800BEA64);
            w_u16(context + 46 + index * 2, value);
            ++index;
        } while ((sint32)index < count);
    }
    context = r_u32(0x800BEA64);
    value = r_u16(0x80092A3C + (uint32)count * 16);
    w_u16(context + 60, value);
    return value;
}

uint32 gtmain_parts_clutch_values_apply(void)
{
    uint32 context, value;
    FUNCTION_MARKER(0x80043840, "GTMAIN.EXE");
    context = r_u32(0x800BEA64);
    value = r_u16(0x80092ABC);
    w_u8(context + 65, value);
    context = r_u32(0x800BEA64);
    value = r_u16(0x80092ACC);
    w_u8(context + 78, value);
    return value;
}

uint32 gtmain_parts_options_select(sint32 group, sint32 selection)
{
    uint32 table, callback;
    FUNCTION_MARKER(0x80043870, "GTMAIN.EXE");
    table = group == 1 ? 0x80092B9C : group == 0 ? 0x80092B6C : 0x80092BAC;
    callback = r_u32(table + (uint32)selection * 8);
    gtmain_menu_callback(callback);
    return 0;
}

uint32 gtmain_parts_options_apply(sint32 group, sint32 selection)
{
    uint32 table, callback;
    FUNCTION_MARKER(0x800438EC, "GTMAIN.EXE");
    table = group == 1 ? 0x80092BA0 : group == 0 ? 0x80092B70 : 0x80092BB0;
    callback = r_u32(table + (uint32)selection * 8);
    gtmain_menu_callback(callback);
    gtmain_player_configuration_build();
    return 0;
}

sint32 gtmain_parts_field_input(uint32 selector, uint32 input)
{
    sint32 selection, minimum, maximum, value, count, decrease = 0, increase = 0;
    uint32 table, field, pressed, repeated, held, combined, movement = 0;
    FUNCTION_MARKER(0x8004D820, "GTMAIN.EXE");
    selection = r_s8(selector + 1);
    table = r_u32(selector + 4);
    w_u8(selector + 9, selection);
    pressed = r_u16(input + 4);
    field = table + (uint32)selection * 16;
    if (pressed & 0x6000) return -3;
    if (pressed & 0x9000) return -2;
    minimum = r_s16(field + 4);
    maximum = r_s16(field + 6);
    value = r_s16(field + 8);
    if (maximum - minimum > 0) {
        gtmain_menu_flags(2);
        pressed = r_u16(input + 4);
        repeated = r_u16(input + 8);
        held = r_u16(input + 2);
        combined = pressed | repeated;
        if (held & 0x400) decrease = held & 0x80 ? 10 : 1;
        else if (combined & 0x80) decrease = 1;
        value -= decrease;
        if (value < minimum) value = minimum;
        movement += decrease;
        if (held & 0x800) increase = held & 0x20 ? 10 : 1;
        else if (combined & 0x20) increase = 1;
        value += increase;
        if (maximum < value) value = maximum;
        movement += increase;
        w_u16(field + 8, value);
    }
    if (!movement) {
        selection = r_s8(selector + 1);
        pressed = r_u16(input + 4);
        repeated = r_u16(input + 8);
        count = r_s8(selector);
        selection = gtmain_menu_selection(pressed | repeated, selection, count, 0x10, 64, 0, 0);
        w_u8(selector + 1, selection);
    }
    return -1;
}

sint32 gtmain_parts_settings_bind(void)
{
    FUNCTION_MARKER(0x80043970, "GTMAIN.EXE");
    w_u32(0x800BEA64, 0x800BB790);
    w_u32(0x800BEA68, 0x800C736C);
    w_u32(0x800BEA60, 0x800BB850);
    return printf((const char *)psx_addr(0x8008D8BC, 29));
}

uint32 gtmain_parts_icon_draw(uint32 ordering, uint16 x, sint32 y, uint32 icon, uint32 color)
{
    uint32 record, uv, dimensions;
    FUNCTION_MARKER(0x800439C8, "GTMAIN.EXE");
    record = 0x80092444 + icon * 16;
    uv = r_u32(record + 12);
    dimensions = r_u32(record + 8);
    return gtmain_packet_sprite(ordering, x | ((uint32)y << 16), uv, dimensions, color);
}

uint32 gtmain_parts_number_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 value, sint32 decimals, uint32 suffix)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 digits, cursor, color, component, position = (uint32)x, record, uv, dimensions, result;
    sint32 last = 0, index, quotient;
    uint32 icon = 0, suffix_x;
    FUNCTION_MARKER(0x80043A10, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x50, 0x80043A10);
    digits = frame.address + 0x18;
    component = (uint32)(intensity >> 1) & 255;
    color = component | (component << 8) | (component << 16) | 0x01000000;
    while (decimals < 0) {
        ++decimals;
        value = (sint32)((uint32)value * 10);
    }
    cursor = digits;
    do {
        quotient = value / 10;
        w_u8(cursor, (uint32)value - (uint32)quotient * 10);
        value = quotient;
        if (!value) break;
        ++last;
        ++cursor;
    } while (last < 16);
    suffix_x = position;
    switch (suffix) {
    case 1: icon = 12; break;
    case 2: icon = 13; suffix_x -= 2; break;
    case 3: icon = 14; suffix_x -= 2; break;
    case 4: icon = 15; break;
    case 5: icon = 16; break;
    default: break;
    }
    if (icon) {
        record = 0x80092334 + icon * 16;
        uv = r_u32(record + 12);
        dimensions = r_u32(record + 8);
        position -= 10;
        gtmain_packet_sprite(ordering, (uint16)suffix_x | ((uint32)y << 16), uv, dimensions, color);
    }
    cursor = digits;
    for (index = 0; index <= last; ++index, ++cursor) {
        uint16 digit_x = (uint16)position;
        sint32 digit = r_s8(cursor);
        position -= 14;
        gtmain_parts_icon_draw(ordering, digit_x, y, (uint32)digit, color);
        decimals = (sint32)((uint32)decimals - 1);
        if (!decimals) {
            gtmain_parts_icon_draw(ordering, (uint16)(position + 4), y, 10, color);
            position -= 6;
            if (index == last) {
                gtmain_parts_icon_draw(ordering, (uint16)position, y, 0, color);
                position -= 14;
            }
        }
    }
    while (decimals > 0) {
        gtmain_parts_icon_draw(ordering, (uint16)position, y, 0, color);
        --decimals;
        position -= 14;
        if (!decimals) {
            gtmain_parts_icon_draw(ordering, (uint16)(position + 4), y, 10, color);
            position -= 6;
            gtmain_parts_icon_draw(ordering, (uint16)position, y, 0, color);
            position -= 14;
        }
    }
    result = gtmain_packet_page(ordering, 552);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_parts_caption_draw(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity)
{
    sint32 width;
    uint32 left, component;
    FUNCTION_MARKER(0x80043F34, "GTMAIN.EXE");
    width = gtmain_menu_text_width(2, text, 0);
    left = (uint32)x - ((uint32)(width >> 1) - 160);
    gtmain_text_flags(2);
    gtmain_text_font(2);
    component = (uint32)(intensity >> 1) & 255;
    gtmain_text_color(component | (component << 8) | (component << 16));
    return gtmain_menu_text_draw(ordering, (sint32)left, (sint32)((uint32)y + 191), text, 0);
}

uint32 gtmain_parts_menu_initialize(void)
{
    sint32 language;
    uint32 flags, mode, category;
    FUNCTION_MARKER(0x800444EC, "GTMAIN.EXE");
    w_u8(0x800B5376, 0);
    w_u8(0x800B5304, 0);
    gtmain_parts_settings_bind();
    gtmain_menu_settings_masks_update();
    w_u32(0x80092D44, 0x800BDE42);
    w_u32(0x80092C50, 0x800BDE73);
    w_u32(0x80092C78, 0x800BCEA6);
    w_u32(0x80092C88, 0x800BCEE1);
    w_u32(0x80092BB8, 0x800BCFDB);
    w_u32(0x80092BD0, 0x800BD019);
    w_u32(0x80092BFC, 0x800BE244);
    w_u32(0x80092BEC, 0x800BE1E0);
    language = r_s8(0x800BEA24);
    w_u32(0x80092BF8, 0x800BE272);
    if (!language) {
        w_u32(0x80092C50, 0x800BDE56);
        w_u32(0x80092D44, 0x800BDE30);
        w_u32(0x80092C78, 0x800BCE8A);
        w_u32(0x80092C88, 0x800BCEC5);
        w_u32(0x80092BB8, 0x800BCFC1);
        w_u32(0x80092BD0, 0x800BD004);
        w_u32(0x80092BFC, 0x800BE229);
        w_u32(0x80092BEC, 0x800BE1D0);
        w_u32(0x80092BF8, 0x800BE258);
    }
    flags = r_u8(0x80092BC4);
    mode = r_u8(0x800CB7F5);
    category = r_u8(0x800CB7F8);
    flags &= 0x3F;
    w_u8(0x80092BC4, flags);
    if (mode == 3 && category == 4) w_u8(0x80092BC4, flags | 0x40);
    gtmain_menu_asset_activate(16);
    gtmain_menu_selector_reset(0x80092BDC);
    return 0;
}

uint32 gtmain_parts_category_descriptor(sint32 category)
{
    FUNCTION_MARKER(0x80044DEC, "GTMAIN.EXE");
    return r_u32(0x80092CD0 + (uint32)category * 4);
}

sint32 gtmain_parts_menu_input(void)
{
    uint32 input, buttons, descriptor, title_table, fields_table, state_table, mode_table, count_table;
    uint32 title, fields, state, mode, count;
    sint32 selection, category;
    FUNCTION_MARKER(0x80044E04, "GTMAIN.EXE");
    input = r_u32(0x800BDD08);
    gtmain_parts_all_options_initialize();
    gtmain_menu_stream_check();
    if (!r_s8(input)) return 0;
    if (!r_s8(0x800B5304)) {
        gtmain_menu_flags(0);
        selection = gtmain_menu_category_input(0x80092780, input + 2);
        if (selection == -2) {
            gtmain_menu_sound(2);
            return 1;
        }
        if (selection == -1) {
            buttons = r_u16(input + 6);
            if (buttons & 0x800) goto leave_menu;
            if (!(buttons & 0x20)) return 0;
        }
        gtmain_menu_sound(5);
        category = r_s8(0x8009278D);
        descriptor = gtmain_parts_category_descriptor(category);
        gtmain_menu_dropdown_reset(descriptor);
        w_u8(0x800B5304, 1);
        return 0;
    }
    gtmain_menu_flags(1);
    category = r_s8(0x8009278D);
    descriptor = gtmain_parts_category_descriptor(category);
    selection = gtmain_menu_dropdown_input(descriptor, input + 2);
    if (selection == -2) goto close_dropdown;
    if (selection == -1) {
        buttons = r_u16(input + 6);
        if (buttons & 0x800) goto leave_menu;
        if (!(buttons & 0x80)) return 0;
        goto close_dropdown;
    }
    category = r_s8(0x8009278D);
    if (category == 0) {
        title_table = 0x80092B14; fields_table = 0x80092AE4;
        state_table = 0x80092B38; mode_table = 0x80092B5C; count_table = 0x80092AD4;
    } else if (category == 1) {
        title_table = 0x80092B2C; fields_table = 0x80092AFC;
        state_table = 0x80092B50; mode_table = 0x80092B64; count_table = 0x80092ADC;
    } else {
        title_table = 0x80092B34; fields_table = 0x80092B04;
        state_table = 0x80092B58; mode_table = 0x80092B68; count_table = 0x80092AE0;
    }
    title = r_u32(title_table + (uint32)selection * 4);
    w_u32(0x80092CE0, title);
    fields = r_u32(fields_table + (uint32)selection * 4);
    state = r_u32(state_table + (uint32)selection * 4);
    w_u32(0x800B5308, state);
    mode = r_u8(mode_table + (uint32)selection);
    count = r_u8(count_table + (uint32)selection);
    w_u8(0x800B5305, mode);
    w_u32(0x80092B0C, fields);
    w_u8(0x80092B08, count);
    if (count >= 3) count = 2;
    category = r_s8(0x8009278D);
    w_u8(0x80092B10, count);
    w_u8(0x800B5375, selection);
    gtmain_parts_options_select(category, selection);
    gtmain_menu_sound(4);
    gtmain_menu_push(0x80092CDC);
    return 3;
leave_menu:
    gtmain_menu_sound(1);
    gtmain_menu_stack_replace(0x80092CF8);
    w_u8(0x800B5304, 0);
    return 6;
close_dropdown:
    gtmain_menu_sound(3);
    w_u8(0x800B5304, 0);
    return 0;
}

sint32 gtmain_menu_category_input(uint32 selector, uint32 input)
{
    uint32 selection, pressed, repeated;
    sint32 count, next;
    FUNCTION_MARKER(0x8004B25C, "GTMAIN.EXE");
    selection = r_u8(selector + 13);
    w_u8(selector + 14, selection);
    pressed = r_u16(input + 4);
    if (pressed & 0x6000) return (sint8)selection;
    if (pressed & 0x9000) return -2;
    repeated = r_u16(input + 8);
    count = r_s8(selector);
    next = gtmain_menu_selection(pressed | repeated, (sint8)selection, count, 0x10, 0x40, 0, 0);
    w_u8(selector + 13, next);
    return -1;
}

void gtmain_menu_dropdown_reset(uint32 descriptor)
{
    FUNCTION_MARKER(0x8004B6A0, "GTMAIN.EXE");
    w_u8(descriptor + 1, 0);
}

sint32 gtmain_menu_dropdown_input(uint32 descriptor, uint32 input)
{
    uint32 table, pressed, repeated;
    sint32 selection, count, result = -1;
    FUNCTION_MARKER(0x8004B6A8, "GTMAIN.EXE");
    table = r_u32(descriptor + 4);
    pressed = r_u16(input + 4);
    repeated = r_u16(input + 8);
    selection = r_s8(descriptor + 1);
    count = r_s8(descriptor + 20);
    selection = gtmain_menu_selection(pressed | repeated, selection, count, 0x10, 0x40, 0, 1);
    w_u8(descriptor + 1, selection);
    pressed = r_u16(input + 4);
    if (pressed & 0x6000) {
        selection = (sint8)selection;
        if (r_u8(table + (uint32)selection * 8) & 0x20) gtmain_menu_sound(6);
        else result = selection;
    }
    if (pressed & 0x9000) result = -2;
    return result;
}

void gtmain_menu_category_reset(uint32 selector)
{
    FUNCTION_MARKER(0x8004B24C, "GTMAIN.EXE");
    w_u8(selector + 13, 0);
    w_u8(selector + 14, 0);
    w_u8(selector + 15, 0);
}

uint32 gtmain_parts_change_initialize(void)
{
    FUNCTION_MARKER(0x800458C0, "GTMAIN.EXE");
    printf((const char *)psx_addr(0x8008D988, 19));
    gtmain_menu_category_reset(0x8009303C);
    gtmain_menu_category_reset(0x80092780);
    w_u8(0x800B5304, 0);
    return 0;
}

uint32 gtmain_parts_availability_draw(uint32 ordering, sint32 x, sint32 y, uint32 flags)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 argument, result, width;
    FUNCTION_MARKER(0x80045BF4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x40, 0x80045BF4);
    if (flags & 4) {
        width = gtmain_text_width(2, 0x800BD2D5);
        result = gtmain_font_text_draw(ordering, (sint32)((uint32)x - width), y, 0x800BD2D5);
    } else {
        argument = flags & 1 ? 0x800BD3A0 : 0x800BD3AF;
        gtmain_string_format(frame.address + 0x10, 0x800BD2E3, &argument, 1);
        gtmain_font_text_draw(ordering, (sint32)((uint32)x - 24), y, frame.address + 0x10);
        argument = flags & 2 ? 0x800BD3A0 : 0x800BD3AF;
        gtmain_string_format(frame.address + 0x10, 0x800BD2DD, &argument, 1);
        result = gtmain_font_text_draw(ordering, (sint32)((uint32)x - 120), y, frame.address + 0x10);
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_parts_preview_draw(uint32 ordering, sint32 x, sint32 y, sint32 selection_style, sint32 intensity)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 snapshot, peaks, values, detail, copy_source, copy_destination, restoring = 0;
    uint32 source, destination, offset, first, second, third, fourth;
    uint32 word, lane, base, count, shift, index, words[4];
    uint32 primary, secondary, component, mode, field, table, record, color, sprite;
    sint32 selected, category, half = intensity >> 1, sixth = intensity / 6;
    sint32 saved, flag, dx, dy;
    FUNCTION_MARKER(0x800463A4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x180, 0x800463A4);
    snapshot = frame.address + 0x30;
    peaks = frame.address + 0x90;
    detail = frame.address + 0x98;
    values = frame.address + 0x104;
    copy_source = r_u32(0x800BEA64);
    copy_destination = snapshot;
copy_state:
    for (offset = 0; offset < 96; offset += 16) {
        source = copy_source + offset;
        destination = copy_destination + offset;
        if ((source | destination) & 3) {
            for (index = 0; index < 4; ++index) {
                word = source + index * 4;
                base = (word + 3) & ~3;
                count = ((word + 3) & 3) + 1;
                shift = (4 - count) * 8;
                words[index] = 0;
                for (lane = 0; lane < count; ++lane)
                    words[index] |= (uint32)r_u8(base + lane) << (shift + lane * 8);
                base = word & ~3;
                count = word & 3;
                for (lane = count; lane < 4; ++lane) {
                    shift = (lane - count) * 8;
                    words[index] = (words[index] & ~((uint32)255 << shift)) |
                        ((uint32)r_u8(base + lane) << shift);
                }
            }
            for (index = 0; index < 4; ++index) {
                word = destination + index * 4;
                base = (word + 3) & ~3;
                count = ((word + 3) & 3) + 1;
                shift = (4 - count) * 8;
                for (lane = 0; lane < count; ++lane)
                    w_u8(base + lane, words[index] >> (shift + lane * 8));
                base = word & ~3;
                count = word & 3;
                for (lane = count; lane < 4; ++lane)
                    w_u8(base + lane, words[index] >> ((lane - count) * 8));
            }
        } else {
            first = r_u32(source);
            second = r_u32(source + 4);
            third = r_u32(source + 8);
            fourth = r_u32(source + 12);
            w_u32(destination, first);
            w_u32(destination + 4, second);
            w_u32(destination + 8, third);
            w_u32(destination + 12, fourth);
        }
    }
    if (restoring) {
        ordering += 4;
        gtmain_player_configuration_build();
        half >>= 2;
        goto draw_footer;
    }
    selected = r_s8(0x80092B09);
    category = r_s8(0x8009278D);
    component = (uint32)(selected ? sixth : half) & 255;
    primary = component | (component << 8) | (component << 16) | 0x03000000;
    component = (uint32)(selected ? half : sixth) & 255;
    secondary = component | (component << 8) | (component << 16) | 0x03000000;
    mode = r_u8(0x800B5305) >> 4;
    if (mode == 4) {
        field = r_u32(0x80092B0C);
        count = r_u16(field + 6);
        saved = r_s16(field + 8);
        w_u16(field + 8, count - 1);
        selected = r_s8(0x800B5375);
        gtmain_parts_options_apply(category, selected);
        gtmain_menu_selection_peak_reset(peaks);
        gtmain_race_specification_summary(0x800BB850, values, detail);
        gtmain_menu_selection_peak_update(peaks, values, detail);
        w_u16(field + 8, saved);
        selected = r_s8(0x800B5375);
        gtmain_parts_options_apply(category, selected);
        gtmain_race_specification_summary(0x800BB850, values, detail);
        gtmain_parts_detail_draw(ordering, (sint32)((uint32)x + 176), (sint32)((uint32)y + 80), intensity,
            peaks, intensity, (sint32)((uint32)x + 140), (sint32)((uint32)y + 100),
            (sint32)((uint32)x + 140), (sint32)((uint32)y + 132), 1, 1);
    } else if (mode == 6) {
        sprite = (uint32)selected;
        selected = r_s8(0x800B5375);
        gtmain_parts_options_apply(category, selected);
        gtmain_parts_gearing_draw(ordering, (sint32)((uint32)x + 176), (sint32)((uint32)y + 102),
            intensity, (sint32)sprite, (sint32)((uint32)x + 32), (sint32)((uint32)y + 110));
    } else {
        table = r_u32(0x80092E18 + mode * 4);
        record = table + 4;
        for (;;) {
            flag = r_s8(record + 1);
            color = primary;
            if (flag == -1) break;
            if (flag == 1) color = secondary;
            dy = r_s16(record - 2);
            sprite = (uint32)r_s8(record);
            record += 6;
            dx = r_s16(table);
            table += 6;
            gtmain_menu_table_sprite_draw(ordering, (uint32)x + (uint32)dx,
                (uint32)y + (uint32)dy, sprite, color);
        }
        goto draw_footer;
    }
    copy_source = snapshot;
    copy_destination = r_u32(0x800BEA64);
    restoring = 1;
    goto copy_state;
draw_footer:
    mode = r_u8(0x800B5305) & 15;
    component = (uint32)half & 255;
    color = component | (component << 8) | (component << 16) | 0x03000000;
    switch (mode) {
    case 0:
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 56, (uint32)y + 112, 0, color);
        break;
    case 1:
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 56, (uint32)y + 96, 1, color);
        break;
    case 2:
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 112, (uint32)y + 112, 2, color);
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 160, (uint32)y + 112, 3, color);
        break;
    case 3:
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 8, (uint32)y + 112, 2, color);
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 56, (uint32)y + 112, 3, color);
        gtmain_menu_table_sprite_draw(ordering, (uint32)x + 104, (uint32)y + 112, 0, color);
        break;
    default: break;
    }
    gtmain_packet_page(ordering, 569);
    gtmain_parts_fields_draw(ordering, 0x80092B08, (sint32)((uint32)x + 16),
        (sint32)((uint32)y + 48), selection_style, intensity);
    table = r_u32(0x800B5308);
    gtmain_parts_caption_draw(ordering, table, x, y, intensity);
    gt_local_stack_pop(frame);
    return 0;
}

void gtmain_parts_field_selector_reset(uint32 selector)
{
    FUNCTION_MARKER(0x8004D810, "GTMAIN.EXE");
    w_u8(selector + 1, 0);
    w_u8(selector + 9, 0);
    w_u8(selector + 10, 0);
}

uint32 gtmain_parts_preview_initialize(void)
{
    FUNCTION_MARKER(0x800469C8, "GTMAIN.EXE");
    gtmain_parts_field_selector_reset(0x80092B08);
    return 0;
}

uint32 gtmain_parts_overlay_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    uint32 table = 0x80092E34, record, component, color, sprite;
    sint32 dx, dy;
    FUNCTION_MARKER(0x800469F0, "GTMAIN.EXE");
    gtmain_menu_stream_check();
    component = (uint32)(intensity >> 3) & 255;
    color = component | (component << 8) | (component << 16) | 0x03000000;
    if (r_s8(table + 5) >= 0) {
        record = table + 5;
        do {
            dx = r_s16(table);
            dy = r_s16(record - 3);
            sprite = (uint32)r_s8(record - 1);
            record += 6;
            gtmain_menu_table_sprite_draw(ordering, (uint32)x + (uint32)dx,
                (uint32)y + (uint32)dy, sprite, color);
            table += 6;
        } while (r_s8(record) >= 0);
    }
    return gtmain_packet_page(ordering, 569);
}

uint32 gtmain_parts_fields_draw(uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 selection_style, sint32 intensity)
{
    sint32 visible, selected, total, first, previous, end, timer, phase, half, row_intensity;
    sint32 fade_in = 0, fade_out = 0;
    uint32 row_y = (uint32)y, field, color;
    FUNCTION_MARKER(0x8004DD80, "GTMAIN.EXE");
    (void)selection_style;
    gtmain_text_font(0);
    gtmain_text_flags(2);
    visible = r_s8(selector + 8);
    selected = r_s8(selector + 1);
    half = visible / 2;
    first = selected - half;
    if (first < 0) first = 0;
    total = r_s8(selector);
    if (total < first + visible) first = total - visible;
    previous = r_s8(selector + 9) - half;
    end = first + visible;
    if (previous < 0) previous = 0;
    if (total < previous + visible) previous = total - visible;
    if (first != previous) {
        w_u8(selector + 10, previous < first ? 3 : 4);
        w_u8(selector + 11, 8);
    }
    if (r_s8(selector + 10)) {
        timer = r_u8(selector + 11) - 1;
        w_u8(selector + 11, timer);
        timer = (sint8)timer;
        if (timer <= 0) {
            w_u8(selector + 10, 0);
            w_u8(selector + 11, 0);
        } else {
            phase = r_s8(selector + 10);
            if (phase == 3) {
                --first;
                row_y -= 3 * (uint32)(8 - timer);
                fade_in = first;
                fade_out = end - 1;
            } else if (phase == 4) {
                ++end;
                row_y -= 3 * (uint32)timer;
                fade_out = first;
                fade_in = end - 1;
            }
        }
    }
    if (r_s8(0x80091B41)) {
        color = gtmain_menu_palette_color(2, intensity);
        if (first) gtmain_packet_triangle_up(ordering, (sint32)((uint32)x + 36),
            (sint32)((uint32)y - 8), color);
        total = r_s8(selector);
        if (end != total) gtmain_packet_triangle_down(ordering, (sint32)((uint32)x + 36),
            (sint32)((uint32)y + 24 * (uint32)visible + 8), color);
        gtmain_packet_page(ordering, 544);
    }
    field = r_u32(selector + 4) + (uint32)first * 16;
    while (first < end) {
        phase = r_s8(selector + 10);
        row_intensity = intensity;
        if (phase) {
            if (first == fade_in) row_intensity = 255 * r_s8(selector + 11) / 8;
            else if (first == fade_out) row_intensity = 255 - 255 * r_s8(selector + 11) / 8;
        }
        gtmain_parts_field_draw(ordering, field, x, (sint16)row_y, row_intensity, first == selected);
        row_y += 24;
        ++first;
        field += 16;
    }
    return 0;
}

uint32 gtmain_parts_field_draw(uint32 ordering, uint32 field, sint32 x, sint32 y, sint32 intensity, uint32 selected)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 name, left, right, corners, color, background = 0, outline, label_color, component, uv, dimensions, page, result;
    sint32 minimum, maximum, value, width, marker, bias, decimals;
    uint32 suffix;
    FUNCTION_MARKER(0x8004D9D0, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x88, 0x8004D9D0);
    left = frame.address + 0x20;
    right = frame.address + 0x28;
    corners = frame.address + 0x30;
    name = r_u32(field);
    w_u16(left, x);
    w_u16(left + 2, (uint32)y + 17);
    w_u16(left + 4, (uint32)x + 71);
    w_u16(left + 6, (uint32)y + 22);
    w_u16(right, (uint32)x + 72);
    w_u16(right + 2, y);
    w_u16(right + 4, (uint32)x + 287);
    w_u16(right + 6, (uint32)y + 22);
    maximum = r_s16(field + 6);
    minimum = r_s16(field + 4);
    value = r_s16(field + 8);
    width = maximum - minimum;
    if (width > 0) {
        marker = 79 * (value - minimum) / width;
        w_u16(corners, (uint32)x + (uint32)marker + 189);
        w_u16(corners + 2, (uint32)y + 12);
        w_u16(corners + 4, (uint32)x + (uint32)marker + 191);
        w_u16(corners + 6, (uint32)y + 17);
        color = gtmain_menu_palette_color(3, intensity);
        gtmain_packet_rectangle(ordering, corners, color);
        component = (uint32)(intensity >> 1) & 255;
        color = component | (component << 8) | (component << 16) | 0x01000000;
        uv = r_u32(0x800923B0);
        dimensions = r_u32(0x800923AC);
        gtmain_packet_sprite(ordering + 4, (uint16)((uint32)x + 166) | (((uint32)y + 4) << 16), uv, dimensions, color);
        page = r_u8(0x800923A5);
        page = (uint16)((uint32)(sint32)(sint8)(page | 0x20) | 0x200);
        gtmain_packet_page(ordering + 4, page);
    }
    if (selected) {
        label_color = ((uint32)(gtmain_menu_pulse_component(0, intensity) >> 1)) & 255;
        if (width > 0) {
            color = (uint8)gtmain_menu_pulse_component(0, intensity);
            w_u16(corners, (uint32)x + 190);
            w_u16(corners + 2, (uint32)y + 14);
            w_u16(corners + 4, (uint32)x + 269);
            w_u16(corners + 6, (uint32)y + 15);
            gtmain_packet_rectangle(ordering, corners, color);
        }
        background = gtmain_menu_palette_color(4, intensity >> 2);
        gtmain_packet_rectangle(ordering + 4, left, background);
        gtmain_packet_rectangle(ordering + 4, right, background);
        outline = gtmain_menu_palette_color(4, intensity);
    } else {
        outline = gtmain_menu_gray(intensity);
        label_color = gtmain_menu_gray(intensity >> 1);
    }
    bias = r_s16(field + 12);
    decimals = r_s8(field + 10);
    suffix = r_u8(field + 11) >> 4;
    gtmain_parts_number_draw(ordering, (sint32)((uint32)x + 134), (sint32)((uint32)y + 3),
        intensity, value - bias, decimals, suffix);
    width = gtmain_menu_text_width(2, name, 0);
    marker = (sint32)((uint32)x - ((uint32)(width >> 1) - 36));
    gtmain_text_color(label_color);
    gtmain_menu_text_draw(ordering, marker, y, name, 0);
    w_u16(corners, (uint32)x + 1);
    w_u16(corners + 2, (uint32)y + 18);
    w_u16(corners + 4, (uint32)x + 286);
    w_u16(corners + 6, (uint32)y + 21);
    gtmain_packet_rectangle(ordering, corners, background);
    gtmain_packet_outline(ordering, left, outline);
    result = gtmain_packet_outline(ordering, right, outline);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_race_gearing_summary(uint32 specification, uint32 values)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80058B48, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x170, 0x80058B48);
    gtmain_race_engine_curve_initialize(specification, frame.address + 0xB0, 0);
    gtmain_race_transmission_initialize(specification, frame.address + 0x10, frame.address + 0xB0);
    result = gtmain_race_transmission_summary(frame.address + 0x10, frame.address + 0xB0, values);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_parts_graph_line(uint32 ordering, sint32 x, sint32 y, sint32 first_x, sint32 first_y, sint32 second_x, sint32 second_y, uint32 scale, sint32 palette, sint32 intensity)
{
    GT_LOCAL_STACK_FRAME frame;
    sint32 inputs[4], numerator, denominator, factor, quotient;
    uint32 index, first, second, color, result;
    FUNCTION_MARKER(0x8004C3D4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x28, 0x8004C3D4);
    inputs[0] = first_x; inputs[1] = first_y;
    inputs[2] = second_x; inputs[3] = second_y;
    for (index = 0; index < 4; ++index) {
        factor = r_s16(scale + (index & 1) * 2);
        numerator = (sint32)((uint32)inputs[index] * (uint32)factor);
        denominator = r_s16(scale + 4 + (index & 1) * 2);
        if (!denominator || (denominator == -1 && (uint32)numerator == 0x80000000))
            gt_draft_failure("Original graph division trap", 0x8004C3D4);
        quotient = numerator / denominator;
        w_u16(frame.address + 0x18 + index * 2, index & 1 ? (uint32)y - (uint32)quotient : (uint32)x + (uint32)quotient);
    }
    first = r_u32(frame.address + 0x18);
    second = r_u32(frame.address + 0x1C);
    w_u32(frame.address + 0x10, first);
    w_u32(frame.address + 0x14, second);
    color = gtmain_menu_palette_color(palette, intensity);
    result = gtmain_packet_line_segment(ordering, frame.address + 0x10, color | 0x02000000);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_parts_gearing_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 selection, sint32 graph_x, sint32 graph_y)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 summary, scale, text, corners, words[2], index, pass, lane, argument, length, color, field;
    uint32 maximum, first_y, second_x, second_y, first_x, palette, result;
    uint32 bottom = (uint32)y + 80, label_y = (uint32)graph_y;
    sint32 gears, gear, tick, numerator, plot_limit, half = intensity >> 1;
    FUNCTION_MARKER(0x8004C568, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x100, 0x8004C568);
    summary = frame.address + 0x28;
    corners = frame.address + 0x68;
    text = frame.address + 0x70;
    scale = frame.address + 0x80;
    for (index = 0; index < 2; ++index) {
        for (pass = 0; pass < 2; ++pass) {
            words[index] = 0;
            for (lane = 0; lane < 4; ++lane)
                words[index] |= (uint32)r_u8(0x8008DE78 + index * 4 + lane) << (lane * 8);
        }
    }
    for (index = 0; index < 2; ++index)
        for (pass = 0; pass < 2; ++pass)
            for (lane = 0; lane < 4; ++lane)
                w_u8(scale + index * 4 + lane, words[index] >> (lane * 8));
    gtmain_text_flags(1);
    color = gtmain_menu_gray((uint32)intensity - (uint32)(intensity >> 2));
    w_u16(corners, x);
    w_u16(corners + 2, y);
    w_u16(corners + 4, (uint32)x + 1);
    w_u16(corners + 6, (uint32)y + 79);
    gtmain_packet_rectangle(ordering, corners, color);
    w_u16(corners, x);
    w_u16(corners + 2, (uint32)y + 78);
    w_u16(corners + 4, (uint32)x + 109);
    w_u16(corners + 6, (uint32)y + 79);
    gtmain_packet_rectangle(ordering, corners, color);
    gtmain_text_color(color);
    gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)x + 4), (sint32)((uint32)y - 4), 0x800BCF2C, 7, 1);
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 94), (sint32)((uint32)y + 69), 0x800BCF30, 0);
    gtmain_race_gearing_summary(0x800BB850, summary);
    gears = r_s32(summary);
    maximum = r_u16(frame.address + 0x38 + (uint32)gears * 2);
    w_u16(scale + 6, maximum);
    color = gtmain_menu_palette_color(7, half);
    gtmain_text_color(color);
    field = summary;
    for (gear = 0; gear < gears; ++gear, field += 2) {
        palette = 2;
        if (selection == gear) {
            first_y = r_u16(field + 0x12);
            second_x = r_u16(field + 0x2E);
            second_y = r_u16(field + 0x12);
            gtmain_parts_graph_line(ordering, x, (sint32)bottom, 0, first_y, second_x, second_y, scale, 7, half);
            argument = r_u16(field + 0x12);
            gtmain_string_format(text, 0x8008DE80, &argument, 1);
            length = gtmain_string_length(text);
            argument = r_u16(field + 0x12);
            if (!maximum) gt_draft_failure("Original gearing division trap", 0x8004C568);
            gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)x - (length * 7 + 1)),
                (sint32)(bottom - 74 * argument / maximum - 4), text, 7, 1);
            first_y = r_u16(field + 4);
            second_x = r_u16(field + 0x20);
            second_y = r_u16(field + 4);
            gtmain_parts_graph_line(ordering, x, (sint32)bottom, 0, first_y, second_x, second_y, scale, 7, half);
            argument = r_u16(field + 4);
            gtmain_string_format(text, 0x8008DE80, &argument, 1);
            length = gtmain_string_length(text);
            argument = r_u16(field + 4);
            if (!maximum) gt_draft_failure("Original gearing division trap", 0x8004C568);
            gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)x - (length * 7 + 1)),
                (sint32)(bottom - 74 * argument / maximum - 4), text, 7, 1);
            palette = 4;
        }
        first_x = r_u16(field + 0x20);
        first_y = r_u16(field + 4);
        second_x = r_u16(field + 0x2E);
        second_y = r_u16(field + 0x12);
        gtmain_parts_graph_line(ordering, x, (sint32)bottom, first_x, first_y, second_x, second_y, scale, palette, intensity);
    }
    color = gtmain_menu_palette_color(6, intensity >> 2);
    gtmain_text_color(color);
    plot_limit = 80 * maximum / 74;
    numerator = 11000;
    for (tick = 100; tick < 400; tick += 100, numerator += 11000) {
        gtmain_parts_graph_line(ordering, x, (sint32)bottom, tick, 0, tick, plot_limit, scale, 5, half);
        argument = tick;
        gtmain_string_format(text, 0x8008DE80, &argument, 1);
        gtmain_menu_text_draw(ordering, (sint32)((uint32)x + (uint32)(numerator / 350) - 16), (sint32)bottom, text, 0);
    }
    numerator = 148000;
    for (tick = 2000; tick < (sint32)maximum; tick += 2000, numerator += 148000) {
        gtmain_parts_graph_line(ordering, x, (sint32)bottom, 0, tick, 350, tick, scale, 5, half);
        argument = tick;
        gtmain_string_format(text, 0x8008DE80, &argument, 1);
        length = gtmain_string_length(text);
        gtmain_menu_text_cells_draw(ordering + 4, (sint32)((uint32)x - (length * 7 + 1)),
            (sint32)(bottom - (uint32)numerator / maximum), text, 7, 1);
    }
    gtmain_parts_graph_line(ordering, x, (sint32)bottom, 0, maximum, 350, maximum, scale, 5, half);
    argument = maximum;
    gtmain_string_format(text, 0x8008DE80, &argument, 1);
    length = gtmain_string_length(text);
    color = gtmain_menu_palette_color(6, half);
    gtmain_text_color(color);
    gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)x - (length * 7 + 1)),
        (sint32)((uint32)y + 2), text, 7, 1);
    field = 0x800935F8;
    for (gear = 0; gear < (sint32)((uint32)gears - 1); ++gear, field += 4) {
        color = gtmain_menu_gray(half);
        gtmain_text_color(color);
        argument = r_u32(field);
        gtmain_string_format(text, 0x8008DE84, &argument, 1);
        gtmain_menu_text_cells_draw(ordering, graph_x, (sint32)label_y, text, 7, 1);
        gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)graph_x + 75), (sint32)label_y, 0x800BCF30, 7, 1);
        color = gtmain_menu_palette_color(6, half);
        gtmain_text_color(color);
        argument = r_u16(frame.address + 0x56 + (uint32)gear * 2);
        gtmain_string_format(text, 0x8008DE80, &argument, 1);
        length = gtmain_string_length(text);
        gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)graph_x - (length * 7 - 75)), (sint32)label_y, text, 7, 1);
        label_y += 9;
    }
    color = gtmain_menu_gray(half);
    gtmain_text_color(color);
    argument = r_u16(frame.address + 0x3A);
    gtmain_string_format(text, 0x800BCF35, &argument, 1);
    result = gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)graph_x + 47), (sint32)(label_y + 2), text, 7, 1);
    gt_local_stack_pop(frame);
    return result;
}

void gtmain_parts_graph_curve(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 segments, uint32 x_values, uint32 y_values, uint32 scale, sint32 palette)
{
    uint32 index = 1, first_x, first_y, second_x, second_y;
    sint32 more;
    FUNCTION_MARKER(0x8004CD7C, "GTMAIN.EXE");
    if (segments <= 0) return;
    do {
        first_x = r_u16(x_values);
        first_y = r_u16(y_values);
        second_x = r_u16(x_values + 2);
        x_values += 2;
        second_y = r_u16(y_values + 2);
        y_values += 2;
        gtmain_parts_graph_line(ordering, x, y, first_x, first_y, second_x, second_y, scale, palette, intensity);
        more = (sint32)index < segments;
        ++index;
    } while (more);
}

uint32 gtmain_parts_peak_label(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 rpm, uint32 value_text, uint32 unit, sint32 palette, uint32 title)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 text, color, length, rpm_x, unit_x, result;
    FUNCTION_MARKER(0x8004CE54, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x50, 0x8004CE54);
    text = frame.address + 0x18;
    gtmain_text_flags(1);
    color = gtmain_menu_palette_color(palette, intensity >> 1);
    gtmain_text_color(color);
    gtmain_menu_text_draw(ordering, (sint32)((uint32)x - 112), y, title, 0);
    gtmain_string_format(text, 0x8008DE80, &rpm, 1);
    color = gtmain_menu_gray(intensity >> 2);
    gtmain_text_color(color);
    length = gtmain_string_length(0x800BCF2C);
    rpm_x = (uint32)x - length * 7;
    gtmain_menu_text_cells_draw(ordering, (sint32)rpm_x, (sint32)((uint32)y + 16), 0x800BCF2C, 7, 1);
    length = gtmain_string_length(text);
    rpm_x -= length * 7;
    length = gtmain_string_length(unit);
    unit_x = rpm_x - length * 7;
    gtmain_menu_text_cells_draw(ordering, (sint32)unit_x, (sint32)((uint32)y + 16), unit, 7, 1);
    color = gtmain_menu_palette_color(6, intensity >> 1);
    gtmain_text_color(color);
    gtmain_menu_text_cells_draw(ordering, (sint32)rpm_x, (sint32)((uint32)y + 16), text, 7, 1);
    gtmain_text_flags(2);
    length = gtmain_string_length(value_text);
    result = gtmain_menu_text_cells_draw(ordering, (sint32)(unit_x - length * 8), (sint32)((uint32)y + 9), value_text, 8, 2);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_parts_detail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 peaks, sint32 text_intensity, sint32 first_x, sint32 first_y, sint32 second_x, sint32 second_y, uint32 first_flags, uint32 second_flags)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 scale, corners, text, words[2], index, pass, lane, color, argument, length, result;
    uint32 bottom = (uint32)y + 100, summary = peaks + 8, tick, numerator;
    sint32 rpm_limit, power_limit, torque_limit, samples, step, level;
    sint32 half = intensity >> 1;
    FUNCTION_MARKER(0x8004D020, "GTMAIN.EXE");
    frame = gt_local_stack_push(0xC8, 0x8004D020);
    scale = frame.address + 0x28;
    corners = frame.address + 0x30;
    text = frame.address + 0x38;
    for (index = 0; index < 2; ++index) {
        for (pass = 0; pass < 2; ++pass) {
            words[index] = 0;
            for (lane = 0; lane < 4; ++lane)
                words[index] |= (uint32)r_u8(0x8008DE8C + index * 4 + lane) << (lane * 8);
        }
    }
    for (index = 0; index < 2; ++index)
        for (pass = 0; pass < 2; ++pass)
            for (lane = 0; lane < 4; ++lane)
                w_u8(scale + index * 4 + lane, words[index] >> (lane * 8));
    color = gtmain_menu_gray((uint32)intensity - (uint32)(intensity >> 2));
    gtmain_text_flags(1);
    gtmain_text_color(color);
    if (first_flags) {
        gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)x - 16), (sint32)((uint32)y - 4), 0x800BCF1E, 7, 1);
        gtmain_menu_text_cells_draw(ordering, (sint32)((uint32)x + 101), (sint32)((uint32)y - 4), 0x800BCF44, 7, 1);
        gtmain_menu_text_draw(ordering, (sint32)((uint32)x + 76), (sint32)((uint32)y + 91), 0x800BCF3B, 0);
        w_u16(corners, x);
        w_u16(corners + 2, y);
        w_u16(corners + 4, (uint32)x + 1);
        w_u16(corners + 6, (uint32)y + 99);
        gtmain_packet_rectangle(ordering, corners, color);
        w_u16(corners, (uint32)x + 99);
        w_u16(corners + 2, y);
        w_u16(corners + 4, (uint32)x + 99);
        w_u16(corners + 6, (uint32)y + 99);
        gtmain_packet_rectangle(ordering, corners, color);
        w_u16(corners, x);
        w_u16(corners + 2, bottom);
        w_u16(corners + 4, (uint32)x + 99);
        w_u16(corners + 6, (uint32)y + 101);
        gtmain_packet_rectangle(ordering, corners, color);
    }
    torque_limit = r_u16(peaks + 6);
    rpm_limit = r_s32(peaks);
    power_limit = r_u16(peaks + 4);
    samples = r_u16(peaks + 18);
    w_u16(scale + 4, rpm_limit);
    w_u16(scale + 6, power_limit);
    if (first_flags) {
        color = gtmain_menu_palette_color(6, intensity >> 2);
        gtmain_text_color(color);
        tick = 2000;
        numerator = 190000;
        while ((sint32)tick < rpm_limit) {
            gtmain_parts_graph_line(ordering, x, (sint32)bottom, (sint32)tick, 0, (sint32)tick, power_limit, scale, 5, half);
            argument = (uint32)((sint32)tick / 1000);
            gtmain_string_format(text, 0x8008DE80, &argument, 1);
            tick += 2000;
            gtmain_menu_text_draw(ordering, (sint32)((uint32)x + (uint32)((sint32)numerator / rpm_limit) - 4),
                (sint32)(bottom + 2), text, 0);
            numerator += 190000;
        }
    }
    w_u16(scale + 2, 95);
    gtmain_parts_graph_curve(ordering, x, (sint32)bottom, text_intensity, samples - 1, summary + 12, summary + 44, scale, 2);
    if (first_flags) {
        w_u16(scale, 100);
        color = gtmain_menu_palette_color(2, half);
        gtmain_text_color(color);
        step = power_limit >= 601 ? 200 : 100;
        for (level = step; level < power_limit; level += step) {
            gtmain_parts_graph_line(ordering, x, (sint32)bottom, 0, level, rpm_limit, level, scale, 7, half);
            argument = level;
            gtmain_string_format(text, 0x8008DE80, &argument, 1);
            length = gtmain_string_length(text);
            gtmain_menu_text_cells_draw(ordering + 4, (sint32)((uint32)x - (length * 7 + 1)),
                (sint32)(bottom - (uint32)(95 * level / power_limit + 4)), text, 7, 1);
        }
    }
    w_u16(scale, 95);
    w_u16(scale + 2, 80);
    w_u16(scale + 6, torque_limit);
    gtmain_parts_graph_curve(ordering, x, (sint32)bottom, text_intensity, samples - 1, summary + 12, summary + 76, scale, 4);
    result = 100;
    if (first_flags) {
        w_u16(scale, 100);
        color = gtmain_menu_palette_color(4, half);
        gtmain_text_color(color);
        step = torque_limit >= 601 ? 200 : 100;
        result = 4 * step;
        for (level = step; level < torque_limit + step; level += step) {
            gtmain_parts_graph_line(ordering, x, (sint32)bottom, 0, level, rpm_limit, level, scale, 5, half);
            argument = level / 10;
            gtmain_string_format(text, 0x8008DE80, &argument, 1);
            gtmain_menu_text_cells_draw(ordering + 4, (sint32)((uint32)x + 101),
                (sint32)(bottom - (uint32)(80 * level / torque_limit + 4)), text, 7, 1);
            result = 0;
        }
    }
    if (second_flags) {
        argument = r_u16(summary);
        gtmain_string_format(text, 0x8008DE80, &argument, 1);
        argument = r_u16(summary + 2);
        gtmain_parts_peak_label(ordering, first_x, first_y, intensity, argument, text, 0x800BCF51, 2, 0x800BCF55);
        torque_limit = r_u16(summary + 4);
        argument = torque_limit / 10;
        gtmain_string_format(text, 0x8008DE80, &argument, 1);
        length = gtmain_string_length(text);
        w_u8(text + length, 46);
        w_u8(text + length + 1, torque_limit % 10 + 48);
        w_u8(text + length + 2, 0);
        argument = r_u16(summary + 6);
        result = gtmain_parts_peak_label(ordering, second_x, second_y, intensity, argument, text, 0x800BCF4A, 4, 0x800BCF65);
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_saved_setting_restore(uint32 data, sint32 index)
{
    uint32 target, record, offset, source, destination, first, second, third, fourth;
    uint32 word, lane, base, count, shift, words[4], copy_index;
    FUNCTION_MARKER(0x80047168, "GTMAIN.EXE");
    if (index >= 120) return 0;
    target = r_u32(0x800BEA64);
    record = data + 512 + (uint32)index * 116 + 18;
    for (offset = 0; offset < 96; offset += 16) {
        source = record + offset;
        destination = target + offset;
        if ((source | destination) & 3) {
            for (copy_index = 0; copy_index < 4; ++copy_index) {
                word = source + copy_index * 4;
                base = (word + 3) & ~3;
                count = ((word + 3) & 3) + 1;
                shift = (4 - count) * 8;
                words[copy_index] = 0;
                for (lane = 0; lane < count; ++lane)
                    words[copy_index] |= (uint32)r_u8(base + lane) << (shift + lane * 8);
                base = word & ~3;
                count = word & 3;
                for (lane = count; lane < 4; ++lane) {
                    shift = (lane - count) * 8;
                    words[copy_index] = (words[copy_index] & ~((uint32)255 << shift)) |
                        ((uint32)r_u8(base + lane) << shift);
                }
            }
            for (copy_index = 0; copy_index < 4; ++copy_index) {
                word = destination + copy_index * 4;
                base = (word + 3) & ~3;
                count = ((word + 3) & 3) + 1;
                shift = (4 - count) * 8;
                for (lane = 0; lane < count; ++lane)
                    w_u8(base + lane, words[copy_index] >> (shift + lane * 8));
                base = word & ~3;
                count = word & 3;
                for (lane = count; lane < 4; ++lane)
                    w_u8(base + lane, words[copy_index] >> ((lane - count) * 8));
            }
        } else {
            first = r_u32(source);
            second = r_u32(source + 4);
            third = r_u32(source + 8);
            fourth = r_u32(source + 12);
            w_u32(destination, first);
            w_u32(destination + 4, second);
            w_u32(destination + 8, third);
            w_u32(destination + 12, fourth);
        }
    }
    printf((const char *)psx_addr(0x8008D99C, 68), index);
    return 1;
}

uint32 gtmain_settings_checksum_publish(uint32 data)
{
    uint32 checksum;
    FUNCTION_MARKER(0x80047260, "GTMAIN.EXE");
    checksum = gtmain_data_checksum(data, 14432);
    w_u32(data + 14432, checksum);
    return checksum;
}

sint32 gtmain_settings_checksum_validate(uint32 data)
{
    uint32 checksum, stored;
    FUNCTION_MARKER(0x8004728C, "GTMAIN.EXE");
    checksum = gtmain_data_checksum(data, 14432);
    stored = r_u32(data + 14432);
    return stored == checksum ? 0 : -1;
}

uint32 gtmain_parts_category_names_update(sint32 group)
{
    uint32 count, descriptors, names, categories, category, offset, context, descriptor, part, choices, name, index;
    sint32 kind;
    FUNCTION_MARKER(0x8004AE44, "GTMAIN.EXE");
    count = r_u8(0x800935F4 + (uint32)group);
    descriptors = r_u32(0x800935C4 + (uint32)group * 4);
    names = r_u32(0x800935D4 + (uint32)group * 4);
    categories = r_u32(0x800935E4 + (uint32)group * 4);
    if (!count) return (uint32)group * 4;
    for (index = 0; index < count; ++index) {
        category = r_u8(categories);
        offset = r_u8(0x800941BD + category);
        context = r_u32(0x800BEA64);
        descriptor = r_u32(descriptors + 4);
        part = r_u16(context + offset);
        choices = r_u32(names);
        if (!part) {
            w_u32(descriptor + 4, 0x8008DDDC);
            if ((r_u8(0x800BEA6C + (uint32)group) >> (index & 31)) & 1) {
                name = r_u32(choices + 4);
                w_u32(descriptor + 4, name);
            }
        } else {
            kind = gtmain_parts_kind(category, part, 0x800D6FDC);
            name = r_u32(choices + ((uint32)kind + 1) * 8 + 4);
            w_u32(descriptor + 4, name);
        }
        ++categories;
        names += 4;
        descriptors += 8;
    }
    return 0;
}

uint32 gtmain_parts_all_names_update(void)
{
    sint32 group;
    FUNCTION_MARKER(0x8004AF94, "GTMAIN.EXE");
    for (group = 0; group < 4; ++group) gtmain_parts_category_names_update(group);
    return 0;
}

uint32 gtmain_parts_available_build_context(uint32 descriptors, uint32 entries, sint32 count, uint32 category, uint32 context)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 car, flags, address, inventory, record, offset, found = 0, result = 0;
    sint32 index, available, kind, slot;
    FUNCTION_MARKER(0x8004AFD4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x148, 0x8004AFD4);
    car = r_u16(context) & 0xFFF;
    for (index = 0; index < count; ++index) {
        address = descriptors + (uint32)index * 8;
        flags = r_u8(address);
        w_u8(address, flags | 0x40);
        address = entries + (uint32)index * 8;
        w_u16(address, 0);
        w_u32(address + 4, 0x800921D4);
    }
    flags = r_u8(descriptors);
    w_u8(descriptors, flags & 0x3F);
    context = r_u32(0x800BEA64);
    if (r_u8(context + 95)) {
        printf((const char *)psx_addr(0x8008DDE4, 27));
        flags = r_u8(descriptors);
        w_u8(descriptors, flags | 0x40);
    }
    inventory = r_u32(0x800BEA68);
    inventory = gtmain_parts_inventory_flags(inventory, category);
    available = (sint32)gtmain_parts_count(category);
    for (index = 1; index <= available; ++index) {
        found = gtmain_parts_record_matches_car(car, category, index, 0x800D6FDC);
        if (found) break;
    }
    if (!found) {
        printf((const char *)psx_addr(0x8008DE00, 23));
        flags = r_u8(descriptors);
        result = flags | 0x40;
        w_u8(descriptors, result);
    }
    for (index = 1; index <= available; ++index) {
        offset = (uint32)index - 1;
        flags = r_u8(inventory + (offset >> 3));
        if (flags & (1u << (offset & 7))) {
            kind = gtmain_parts_kind(category, index, 0x800D6FDC);
            context = r_u32(0x800BEA64);
            car = r_u16(context) & 0xFFF;
            slot = kind + 1;
            if (gtmain_parts_record_matches_car(car, category, index, 0x800D6FDC)) {
                printf((const char *)psx_addr(0x8008DE18, 42), index, kind, (const char *)psx_addr(0x8008DE44, 5));
                if (slot < count && slot > 0) {
                    if (category == 34) slot = r_s8(0x800931EC + (uint32)slot);
                    record = entries + (uint32)slot * 8;
                    address = descriptors + (uint32)slot * 8;
                    w_u16(record, index);
                    flags = r_u8(address);
                    w_u8(address, flags & 0x3F);
                }
            } else {
                /* Original diagnostic reads the untouched local buffer */
                printf((const char *)psx_addr(0x8008DE4C, 42), index, kind,
                    (const char *)psx_addr(frame.address + 0x10, 256));
            }
        }
        result = 1;
    }
    gt_local_stack_pop(frame);
    return result;
}
