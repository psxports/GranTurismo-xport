#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include <ctype.h>

uint32 gtmenu_model_preview_load(uint32 model)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 index, count, entry;
    FUNCTION_MARKER(0x8001D96C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(49156, 0x8001D96C);
    for (index = 0; index < 16; ++index) {
        entry = 0x8008E42C + 76 * index;
        w_u8(entry + 2, index);
        w_u16(entry, model);
    }
    w_u32(frame.address + 49152, 0x8008642C);
    count = gtmenu_model_color_count(model);
    gtmenu_model_groups_load(0x8008E42C, count, frame.address + 49152, frame.address);
    w_u16(0x8005863E, 900);
    w_u16(0x8005863A, -100);
    w_u32(0x8008E8F4, 0x10000000);
    gtmenu_local_stack_pop(frame);
    return 0x10000000;
}

uint32 gtmenu_model_preview_bind(uint32 node, uint32 mode, uint32 unused_extra)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 code, previous, root, result = 0;
    FUNCTION_MARKER(0x8001DA2C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8001DA2C);
    gtmenu_string_copy(frame.address, r_u32(node + 12));
    if (mode) w_u8(frame.address + 4, 'r');
    gtmenu_debug_string(0x80054178, frame.address);
    code = gtmenu_model_name_index(frame.address);
    previous = r_u32(0x80058634);
    if (code != previous) {
        w_u32(0x80058634, code);
        root = gtmenu_model_resource_root();
        gtmenu_model_resource_tables_bind(root, 0x80054154, 0x80054158, 1, 16);
        gtmenu_model_resource_directory_load(0x8008242C, 0x4000);
        gtmenu_model_resource_base_set(0);
        gtmenu_model_preview_load(r_u32(0x80058634));
        result = 1;
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_color_previous(uint32 model)
{
    uint32 count;
    uint16 selection;
    FUNCTION_MARKER(0x8001DC20, "GTMENU.EXE");
    count = gtmenu_model_color_count(model);
    selection = r_u16(0x80058638) - 1;
    w_u16(0x80058638, selection);
    if (selection & 0x8000) w_u16(0x80058638, count);
    return count;
}

void gtmenu_model_preview_pitch_set(sint16 pitch)
{
    FUNCTION_MARKER(0x8001DD34, "GTMENU.EXE");
    w_u16(0x8005863C, pitch);
}

uint32 gtmenu_model_info_page_restore(void)
{
    uint32 callback;
    sint32 category;
    FUNCTION_MARKER(0x8001E3C4, "GTMENU.EXE");
    callback = r_u32(0x8008E8FC);
    category = r_s16(0x80058648);
    w_u32(0x800A77B4, callback);
    return gtmenu_model_info_page_open(category);
}

uint32 gtmenu_model_info_actions_activate(void)
{
    FUNCTION_MARKER(0x8001E3FC, "GTMENU.EXE");
    w_u32(0x800A77B4, 0x8001FDA4);
    return 0x8001FDA4;
}

uint32 gtmenu_model_rotor_count(uint32 name)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 entry = 0x80058658, key, result = 0;
    FUNCTION_MARKER(0x8001E414, "GTMENU.EXE");
    if (!name) return 0;
    frame = gtmenu_local_stack_push(8, 0x8001E414);
    gtmenu_string_copy(frame.address, name);
    w_u8(frame.address + 4, 0);
    key = r_u32(entry);
    while (key) {
        if (!gtmenu_string_compare(frame.address, key)) result = r_u32(entry + 4);
        entry += 8;
        key = r_u32(entry);
        if (result) break;
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_resource_tables_bind(uint32 root, uint32 cluts, uint32 pages, uint16 mode, uint16 count)
{
    FUNCTION_MARKER(0x80032E78, "GTMENU.EXE");
    w_u32(0x800A85B0, root);
    w_u32(0x800A85A8, cluts);
    w_u32(0x800A85B8, pages);
    w_u16(0x800A85A4, mode);
    w_u16(0x800A85A6, count);
    return count;
}

uint32 gtmenu_preview_transform_initialize(uint32 transform)
{
    FUNCTION_MARKER(0x80032F18, "GTMENU.EXE");
    gtmenu_memory_fill(transform, 0, 40);
    w_u16(transform + 20, 4096);
    w_u16(transform + 28, 4096);
    w_u16(transform + 36, 4096);
    return 4096;
}

uint32 gtmenu_preview_depth_cue_set(uint32 context, uint32 color, sint32 coefficient, sint32 offset)
{
    uint32 red, green, blue;
    FUNCTION_MARKER(0x80033254, "GTMENU.EXE");
    red = (color << 4) & 0xFF0;
    green = (color >> 4) & 0xFF0;
    blue = (color >> 12) & 0xFF0;
    // The original writes control register 21 three times
    xport_gte_write_control(21, red);
    xport_gte_write_control(21, green);
    xport_gte_write_control(21, blue);
    xport_gte_write_control(27, coefficient);
    xport_gte_write_control(28, offset);
    w_u32(context + 12, red);
    w_u32(context + 16, green);
    w_u32(context + 20, blue);
    w_u16(context + 34, coefficient);
    w_u32(context + 36, offset);
    return green;
}

uint32 gtmenu_preview_rotation_bind(uint32 matrix)
{
    uint32 second, first, third, fourth, fifth, result;
    FUNCTION_MARKER(0x80034718, "GTMENU.EXE");
    second = r_u32(matrix + 4);
    first = r_u32(matrix);
    xport_gte_write_control(0, first);
    second = (second ^ 0xFFFF0000) + 0x10000;
    xport_gte_write_control(1, second);
    third = r_u32(matrix + 8);
    result = ((0u - (third >> 16)) << 16) | ((0u - (third & 0xFFFF)) & 0xFFFF);
    xport_gte_write_control(2, result);
    fifth = r_u32(matrix + 16);
    fourth = r_u32(matrix + 12);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    return result;
}

uint32 gtmenu_model_preview_frame_draw(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 ordering, view, vector, rotation, descriptor, geometry, clut, tint;
    uint32 screen_distance, screen_x, screen_y, extent_bits, shift, radius_shift;
    uint32 negative_x, negative_y, negative_z, magnitude_x, magnitude_y, magnitude_z;
    uint32 center, previous, current, packet, link, angle, table_offset, tx, ty, tz, result;
    sint32 x, y, width, height, yaw, pitch, minimum_y, maximum_y, bits, circle_x, circle_z;
    FUNCTION_MARKER(0x8001DD40, "GTMENU.EXE");
    ordering = gtmenu_secondary_ordering();
    if (!r_s8(0x80058640)) return ordering;
    // Keep the overlapping view, vector and matrix workspaces addressable
    frame = gtmenu_local_stack_push(180, 0x8001DD40);
    view = frame.address;
    vector = view + 120;
    rotation = view + 140;
    y = r_s16(0x8008E8EE);
    x = r_u16(0x8008E8EC);
    width = r_u16(0x8008E8F0);
    height = r_u16(0x8008E8F2);
    w_u16(view + 14, 16);
    w_u16(view + 16, 16);
    w_u16(view + 4, x + width);
    w_u16(view + 6, y + height);
    w_u16(view, x);
    w_u16(view + 2, y);
    screen_distance = r_u16(0x8005863E);
    w_u16(view + 8, x + (sint16)width / 2);
    w_u16(view + 10, y + (sint16)height / 2);
    w_u16(view + 12, screen_distance);
    gtmenu_preview_view_initialize(view);
    gtmenu_preview_view_initialize(view);
    gtmenu_preview_transform_initialize(vector);
    yaw = (sint16)(r_u16(0x8005863A) + 2048);
    pitch = r_s16(0x8005863C);
    gtmenu_preview_rotation_set(rotation, yaw, pitch, 0);
    w_u32(vector, 0);
    w_u32(vector + 4, 52428);
    w_u32(vector + 8, -786432);
    gtmenu_preview_vector_rotate(vector, rotation, vector);
    gtmenu_preview_view_finalize(view, 0x800711C0);
    gtmenu_preview_context_bind(0x1F800000, view);
    gtmenu_preview_depth_cue_set(0x1F800000, 0, 256, 0);
    screen_distance = r_u16(0x1F800020);
    screen_x = r_u32(0x1F800018);
    xport_gte_write_control(26, screen_distance);
    screen_y = r_u32(0x1F80001C);
    xport_gte_write_control(24, screen_x);
    xport_gte_write_control(25, screen_y);
    descriptor = 0x8008E42Cu + 76u * (uint32)r_s16(0x80058638);
    clut = r_u16(descriptor + 4);
    tint = r_u32(0x8008E8F4);
    w_u32(0x1F800394, 0x003F7968);
    screen_distance = r_u16(0x80058644);
    w_u32(0x1F800390, tint);
    w_u16(0x1F80038E, 6);
    w_u16(0x1F800398, 64);
    w_u16(0x1F80039A, 64);
    w_u16(0x1F80039C, screen_distance);
    w_u32(0x1F800388, clut << 16);
    gtmenu_preview_transform_initialize(0x1F800360);
    geometry = r_u32(0x8008E434);
    maximum_y = r_s16(geometry + 50);
    minimum_y = r_s16(geometry + 18);
    w_u16(0x1F80038C, 0);
    w_u32(0x1F8003A0, ordering);
    w_u32(0x1F800364, 16 * (maximum_y - minimum_y));
    gtmenu_preview_geometry_transform(0x1F800000, geometry + 60, 1);
    descriptor = 0x8008E42Cu + 76u * (uint32)r_s16(0x80058638);
    w_u16(0x1F8003A8, r_u16(descriptor + 4));
    w_u16(0x1F8003AA, r_u16(descriptor + 6));
    gtmenu_preview_geometry_draw(0x1F800000, geometry, 0, 0, 0);
    w_u32(0x1F800364, 0);
    gtmenu_preview_mesh_draw(0x1F800000, r_u32(geometry + 56));
    gtmenu_preview_rotation_set(rotation, 0, r_s16(0x8005863C), 0);
    w_u32(vector, 0);
    w_u32(vector + 4, 52428);
    w_u32(vector + 8, -786432);
    gtmenu_preview_vector_rotate(vector, rotation, vector);
    gtmenu_preview_view_finalize(view, 0x800711C0);
    ordering += 512;
    negative_x = 0u - r_u32(vector);
    negative_y = 0u - r_u32(vector + 4);
    negative_z = 0u - r_u32(vector + 8);
    magnitude_x = (sint32)negative_x < 0 ? 0u - negative_x : negative_x;
    magnitude_y = (sint32)negative_y < 0 ? 0u - negative_y : negative_y;
    magnitude_z = (sint32)negative_z < 0 ? 0u - negative_z : negative_z;
    extent_bits = magnitude_x | magnitude_y | magnitude_z;
    xport_gte_write_data(30, extent_bits);
    bits = 31 - (sint32)xport_gte_read_data(31);
    if (bits < 18) bits = 18;
    radius_shift = (bits - 18) & 31;
    shift = (bits - 12) & 31;
    xport_gte_write_data(0, ((uint32)((sint32)negative_x >> shift) & 0xFFFF) | ((uint32)((sint32)negative_y >> shift) << 16));
    xport_gte_write_data(1, (sint32)negative_z >> shift);
    gtmenu_preview_rotation_bind(view + 160);
    xport_gte_execute(0x486012);
    tx = xport_gte_read_data(25);
    ty = xport_gte_read_data(26);
    tz = xport_gte_read_data(27);
    xport_gte_write_control(5, tx);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0);
    xport_gte_execute(0x180001);
    circle_x = r_s16(0x800711D4) >> radius_shift;
    circle_z = r_s16(0x800719D4) >> radius_shift;
    center = xport_gte_read_data(14);
    xport_gte_write_data(0, circle_x & 0xFFFF);
    xport_gte_write_data(1, circle_z);
    xport_gte_execute(0x180001);
    circle_x = r_s16(0x80071254);
    circle_z = r_s16(0x80071A54);
    packet = r_u32(0x800A8434);
    circle_x >>= radius_shift;
    circle_z >>= radius_shift;
    previous = xport_gte_read_data(14);
    angle = 64;
    do {
        xport_gte_write_data(0, circle_x & 0xFFFF);
        xport_gte_write_data(1, circle_z);
        xport_gte_execute(0x180001);
        angle += 64;
        table_offset = (angle & 0xFFF) * 2;
        circle_x = r_s16(0x800711D4 + table_offset) >> radius_shift;
        circle_z = r_s16(0x800719D4 + table_offset) >> radius_shift;
        circle_x = (2867 * circle_x) >> 12;
        circle_z = (2867 * circle_z) >> 12;
        current = xport_gte_read_data(14);
        w_u32(packet + 16, previous);
        w_u32(packet, 0x06000000);
        w_u32(packet + 4, 0x32404040);
        w_u32(packet + 8, center);
        w_u32(packet + 12, 0);
        w_u32(packet + 20, 0);
        w_u32(packet + 24, current);
        link = r_masked_u32(ordering, 7);
        w_masked_u32(ordering, packet, 7);
        w_masked_u32(packet, link, 7);
        packet += 28;
        previous = current;
    } while (angle < 4097);
    w_u32(0x800A8434, packet);
    result = gtmenu_texture_page_draw(ordering, 544);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_current_car_preview_bind(uint32 rectangle)
{
    uint32 models, record, model, mode, first, second;
    sint32 index;
    FUNCTION_MARKER(0x8001E2C4, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    w_u8(0x8008241D, 2);
    index = gtmenu_current_record_index();
    if (index < 0) return (uint32)index;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    model = models + 424 * (r_u16(record) & 0xFFF);
    w_u32(0x80082420, model);
    w_u16(0x8008241E, r_u16(record) & 0xFFF);
    mode = r_u16(record + 34) != 0;
    first = r_u32(rectangle);
    second = r_u32(rectangle + 4);
    gtmenu_model_info_bind(0x80082414, first, second, mode, 0);
    gtmenu_model_info_selection_set(r_u16(record) >> 12);
    gtmenu_preview_control_set(r_u16(record + 90));
    return gtmenu_model_logo_load(model);
}

uint32 gtmenu_model_logo_load(uint32 model)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 filename, make, entry, key, alternate, index = 0;
    sint8 initial;
    FUNCTION_MARKER(0x8001FA40, "GTMENU.EXE");
    if (!gtmenu_string_compare(model, 0x8008E900)) return 0;
    frame = gtmenu_local_stack_push(72, 0x8001FA40);
    filename = frame.address;
    make = frame.address + 64;
    alternate = r_u8(0x8009B85D);
    w_u32(0x800588D8, 0);
    if (!alternate) {
        initial = (sint8)tolower(r_u8(model));
        if (initial == 'h') w_u32(0x800588D8, 1);
    }
    gtmenu_string_copy(0x8008E900, model);
    gtmenu_string_copy(filename, model);
    w_u8(filename + 4, 0);
    gtmenu_string_copy(make, filename);
    if (r_u32(0x800588D8)) {
        gtmenu_string_append(filename, 0x80054438);
        entry = 0x80058878;
    } else {
        gtmenu_string_append(filename, 0x80054440);
        entry = 0x800586B8;
    }
    gtmenu_debug_string(0x80054448, filename);
    gtmenu_menu_texture_load(filename, 0x01E60280, -1, 0x01D003C0, -1, 0);
    key = r_u32(entry);
    w_u32(0x800588D0, -1);
    while (key) {
        if (r_s32(0x800588D0) >= 0) break;
        key = r_u32(entry);
        if (!gtmenu_string_compare(make, key)) w_u32(0x800588D0, index);
        entry += 8;
        key = r_u32(entry);
        ++index;
    }
    gtmenu_local_stack_pop(frame);
    return 1;
}

uint32 gtmenu_preview_rotation_set(uint32 matrix, sint16 yaw, sint16 pitch, sint16 roll)
{
    uint32 yaw_entry, pitch_entry, roll_entry, first, second, third, result;
    sint32 sin_yaw, cos_yaw, sin_pitch, cos_pitch, sin_roll, cos_roll;
    sint32 yaw_pitch, forward_pitch, forward_yaw;
    FUNCTION_MARKER(0x80032F54, "GTMENU.EXE");
    yaw_entry = 0x800711D4 + (yaw & 0xFFF) * 2;
    pitch_entry = 0x800711D4 + (pitch & 0xFFF) * 2;
    sin_pitch = r_s16(pitch_entry);
    sin_yaw = r_s16(yaw_entry);
    cos_yaw = r_s16(yaw_entry + 0x800);
    cos_pitch = r_s16(pitch_entry + 0x800);
    yaw_pitch = (sin_yaw * -sin_pitch) >> 12;
    forward_pitch = (cos_yaw * -sin_pitch) >> 12;
    roll_entry = 0x800711D4 + (roll & 0xFFF) * 2;
    sin_roll = r_s16(roll_entry);
    cos_roll = r_s16(roll_entry + 0x800);
    forward_yaw = (sin_yaw * cos_pitch) >> 12;
    xport_gte_write_control(0, cos_yaw);
    xport_gte_write_control(1, yaw_pitch & 0xFFFF);
    xport_gte_write_control(2, (uint32)cos_pitch << 16);
    xport_gte_write_control(3, -sin_yaw);
    xport_gte_write_control(4, forward_pitch);
    xport_gte_write_data(0, cos_roll & 0xFFFF);
    xport_gte_write_data(1, sin_roll);
    xport_gte_execute(0x486012);
    xport_gte_write_data(2, -sin_roll & 0xFFFF);
    xport_gte_write_data(3, cos_roll);
    w_u16(matrix + 4, forward_yaw);
    first = xport_gte_read_data(9);
    second = xport_gte_read_data(10);
    third = xport_gte_read_data(11);
    w_u16(matrix, first);
    w_u16(matrix + 6, second);
    xport_gte_execute(0x48E012);
    w_u16(matrix + 12, third);
    w_u16(matrix + 10, sin_pitch);
    w_u16(matrix + 16, (cos_yaw * cos_pitch) >> 12);
    result = xport_gte_read_data(9);
    second = xport_gte_read_data(10);
    third = xport_gte_read_data(11);
    w_u16(matrix + 2, result);
    w_u16(matrix + 8, second);
    w_u16(matrix + 14, third);
    return result;
}

uint32 gtmenu_preview_vector_rotate(uint32 output, uint32 matrix, uint32 input)
{
    uint32 first, second, low_x, low_y, low_z, mid_x, mid_y, mid_z, high_x, high_y, high_z;
    uint32 result;
    sint32 x, y, z;
    FUNCTION_MARKER(0x80033064, "GTMENU.EXE");
    first = r_u32(matrix);
    second = r_u32(matrix + 4);
    xport_gte_write_control(0, first);
    first = r_u32(matrix + 8);
    xport_gte_write_control(1, second);
    second = r_u32(matrix + 12);
    xport_gte_write_control(2, first);
    first = r_u32(matrix + 16);
    xport_gte_write_control(3, second);
    xport_gte_write_control(4, first);
    x = r_s32(input);
    y = r_s32(input + 4);
    z = r_s32(input + 8);
    xport_gte_write_data(0, (x & 0xFFF) | ((uint32)(y & 0xFFF) << 16));
    xport_gte_write_data(1, z & 0xFFF);
    x >>= 12;
    y >>= 12;
    xport_gte_execute(0x486012);
    z >>= 12;
    low_x = xport_gte_read_data(9);
    low_y = xport_gte_read_data(10);
    low_z = xport_gte_read_data(11);
    xport_gte_write_data(0, (x & 0xFFF) | ((uint32)(y & 0xFFF) << 16));
    xport_gte_write_data(1, z & 0xFFF);
    x >>= 12;
    y >>= 12;
    xport_gte_execute(0x406012);
    z >>= 12;
    mid_x = xport_gte_read_data(25);
    mid_y = xport_gte_read_data(26);
    mid_z = xport_gte_read_data(27);
    xport_gte_write_data(0, (x & 0xFFFF) | ((uint32)y << 16));
    xport_gte_write_data(1, z);
    xport_gte_execute(0x406012);
    high_x = xport_gte_read_data(25);
    high_y = xport_gte_read_data(26);
    high_z = xport_gte_read_data(27);
    result = high_y << 12;
    w_u32(output, low_x + mid_x + (high_x << 12));
    w_u32(output + 4, low_y + mid_y + result);
    w_u32(output + 8, low_z + mid_z + (high_z << 12));
    return result;
}

uint32 gtmenu_preview_context_bind(uint32 context, uint32 view)
{
    uint32 first, second, third, fourth, result;
    FUNCTION_MARKER(0x800331E0, "GTMENU.EXE");
    gtmenu_preview_projection_bind(context, view);
    first = r_u32(view + 120);
    second = r_u32(view + 124);
    third = r_u32(view + 128);
    w_u32(context + 44, first);
    w_u32(context + 48, second);
    w_u32(context + 52, third);
    first = r_u32(view + 160);
    second = r_u32(view + 164);
    third = r_u32(view + 168);
    fourth = r_u32(view + 172);
    w_u32(context + 64, first);
    w_u32(context + 68, second);
    w_u32(context + 72, third);
    w_u32(context + 76, fourth);
    result = r_u32(view + 176);
    w_u32(context + 80, result);
    return result;
}

uint32 gtmenu_preview_projection_bind(uint32 context, uint32 view)
{
    uint32 first, second, result;
    FUNCTION_MARKER(0x800373A0, "GTMENU.EXE");
    w_u32(context, view);
    first = r_u32(view + 76);
    second = r_u32(view + 80);
    w_u32(context + 116, first);
    first = r_u32(view + 84);
    w_u32(context + 120, second);
    second = r_u32(view + 88);
    w_u32(context + 124, first);
    first = r_u32(view + 92);
    w_u32(context + 128, second);
    w_u32(context + 132, first);
    first = r_u32(view + 96);
    second = r_u32(view + 100);
    w_u32(context + 136, first);
    first = r_u32(view + 104);
    w_u32(context + 140, second);
    second = r_u32(view + 108);
    w_u32(context + 144, first);
    first = r_u32(view + 112);
    w_u32(context + 148, second);
    w_u32(context + 152, first);
    first = r_s16(view + 116);
    second = r_s16(view + 118);
    w_u16(context + 8, first);
    w_u16(context + 10, second);
    first = r_s16(view + 8);
    second = r_s16(view + 10);
    w_u32(context + 24, first << 16);
    w_u32(context + 28, second << 16);
    second = r_s16(view + 12);
    result = r_s16(view + 18);
    w_u16(context + 32, second);
    second = r_s16(view + 20);
    w_u16(context + 4, result);
    w_u16(context + 6, second);
    return result;
}

uint32 gtmenu_installed_parts_draw(uint32 record)
{
    const uint8 fields[18] = {3, 4, 7, 15, 2, 14, 8, 10, 5, 9, 16, 11, 22, 20, 18, 19, 21, 12};
    const uint8 copy_caption[18] = {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0};
    GT_LOCAL_STACK_FRAME frame;
    uint32 ordering, node, text, buffer, index, field, result = 0;
    sint32 x = 100, y = 100;
    FUNCTION_MARKER(0x8001E4AC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(76, 0x8001E4AC);
    buffer = frame.address;
    node = frame.address + 64;
    ordering = gtmenu_current_ordering();
    gtmenu_text_color_set(0x7F7F7F);
    gtmenu_text_font_set(0);
    if (r_u16(record + 26)) {
        w_u8(node + 9, 26);
        y = 120;
        w_u16(node + 10, r_u16(record + 26) - 1);
        text = gtmenu_part_caption(node, 1);
        gtmenu_text_draw(ordering, 100, 100, text);
    }
    for (index = 0; index < 18; ++index) {
        if ((index == 14 || index == 16) && y >= 321) {
            x = 328;
            y = 100;
        }
        field = fields[index];
        if (!r_u16(record + 2 * field)) continue;
        w_u8(node + 9, field + 13);
        w_u16(node + 10, r_u16(record + 2 * field) - 1);
        text = gtmenu_part_caption(node, 1);
        if (copy_caption[index]) {
            gtmenu_string_copy(buffer, text);
            text = buffer;
        }
        if (index == 17) result = gtmenu_text_draw(ordering, x, y, text);
        else {
            gtmenu_text_draw(ordering, x, y, text);
            y += 20;
        }
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

sint32 gtmenu_torque_display_value(sint32 value)
{
    uint32 scaled;
    FUNCTION_MARKER(0x8001F89C, "GTMENU.EXE");
    if (r_u8(0x8009B85D)) return value;
    scaled = (uint32)value * 72329;
    return (sint32)scaled / 10000;
}

sint32 gtmenu_mass_display_value(sint32 value)
{
    uint32 scaled;
    FUNCTION_MARKER(0x8001F900, "GTMENU.EXE");
    if (r_u8(0x8009B85D)) return value;
    scaled = (uint32)value * 22046;
    return (sint32)scaled / 10000;
}

sint32 gtmenu_length_display_value(sint32 value)
{
    uint32 scaled;
    FUNCTION_MARKER(0x8001F96C, "GTMENU.EXE");
    if (r_u8(0x8009B85D)) return value;
    scaled = (uint32)value * 10;
    return (sint32)scaled / 254;
}

sint32 gtmenu_displacement_display_value(sint32 value)
{
    uint32 scaled;
    FUNCTION_MARKER(0x8001F9BC, "GTMENU.EXE");
    if (r_u8(0x8009B85D)) return value;
    scaled = (uint32)value * 61;
    return (sint32)scaled / 1000;
}

uint32 gtmenu_model_specifications_draw(uint32 unused_first, uint32 model)
{
    const uint32 captions[16] = {0x800A6DBA, 0x800A6DC4, 0x800A6DCE, 0x800A6DD6,
        0x800A6DE5, 0x800A6DF6, 0x800A6E02, 0x800A6E0E, 0x800A6E18, 0x800A6E26,
        0x800A6E37, 0x800A6E48, 0x800A6E56, 0x800A6E64, 0x800A6E71, 0x800A6E7F};
    const uint8 dimensions[5] = {12, 14, 20, 22, 16};
    GT_LOCAL_STACK_FRAME frame;
    uint32 ordering, stats, rotors, units, length_unit, mass_unit, torque_unit, displacement_unit;
    uint32 buffer, rectangle, peaks, index, text, rpm, first, second, third, result;
    sint32 value;
    FUNCTION_MARKER(0x8001EB08, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(264, 0x8001EB08);
    buffer = frame.address;
    rectangle = frame.address + 256;
    ordering = gtmenu_current_ordering();
    stats = r_u32(0x80058650);
    gtmenu_resource_group(13);
    rotors = gtmenu_model_rotor_count(model);
    units = r_u8(0x8009B85D);
    if (!units) {
        mass_unit = r_u32(0x80058614);
        length_unit = r_u32(0x80058630);
        torque_unit = r_u32(0x80058628);
        displacement_unit = r_u32(0x8005861C);
    } else {
        mass_unit = r_u32(0x80058610);
        length_unit = r_u32(0x8005862C);
        torque_unit = r_u32(0x80058620);
        displacement_unit = r_u32(0x80058618);
    }
    w_u32(rectangle, 0x003A0060);
    w_u32(rectangle + 4, 0x017B01EF);
    gtmenu_rectangle_outline(ordering, rectangle, 0x8080);
    w_u32(rectangle, 0x0039005F);
    w_u32(rectangle + 4, 0x017C01F0);
    gtmenu_rectangle_outline(ordering, rectangle, 0x8080);
    w_u32(rectangle, 0x00390109);
    w_u32(rectangle + 4, 0x017C010A);
    gtmenu_rectangle_outline(ordering, rectangle, 0x8080);
    for (index = 1; index < 16; ++index) {
        w_u16(rectangle, 96);
        w_u16(rectangle + 2, 58 + 20 * index);
        w_u16(rectangle + 4, 495);
        w_u16(rectangle + 6, 59 + 20 * index);
        gtmenu_rectangle_outline(ordering, rectangle, 0x8080);
    }
    gtmenu_text_color_set(0x7F7F7F);
    gtmenu_text_font_set(0);
    for (index = 0; index < 16; ++index) gtmenu_text_draw(ordering, 100, 60 + 20 * index, captions[index]);
    value = gtmenu_length_display_value(r_u16(model + 404));
    gtmenu_format_integer_string(buffer, 0x800541DC, value, length_unit);
    gtmenu_text_draw(ordering, 270, 60, buffer);
    for (index = 0; index < 5; ++index) {
        value = gtmenu_length_display_value(r_s16(stats + dimensions[index]));
        gtmenu_format_integer_string(buffer, 0x800541DC, value, length_unit);
        gtmenu_text_draw(ordering, 270, 80 + 20 * index, buffer);
    }
    if (r_s8(0x8005864A) == 2) {
        value = gtmenu_mass_display_value(r_s16(stats + 90));
        gtmenu_format_integer_string(buffer, 0x800541DC, value, mass_unit);
        gtmenu_text_draw(ordering, 270, 180, buffer);
        peaks = r_u32(0x80058654);
        value = gtmenu_power_display_value(r_u16(peaks));
        peaks = r_u32(0x80058654);
        rpm = r_u16(peaks + 2);
        gtmenu_format_integer_string_integer(buffer, 0x800541E4, value, 0x800A7556, rpm);
        gtmenu_text_draw(ordering, 270, 200, buffer);
        peaks = r_u32(0x80058654);
        value = gtmenu_torque_display_value(r_u16(peaks + 4));
        peaks = r_u32(0x80058654);
        rpm = r_u16(peaks + 6);
    } else {
        value = gtmenu_mass_display_value(r_u16(model + 90));
        gtmenu_format_integer_string(buffer, 0x800541DC, value, mass_unit);
        gtmenu_text_draw(ordering, 270, 180, buffer);
        value = gtmenu_power_display_value(r_u16(model + 408));
        rpm = r_u16(model + 410);
        gtmenu_format_integer_string_integer(buffer, 0x800541E4, value, 0x800A7556, rpm);
        gtmenu_text_draw(ordering, 270, 200, buffer);
        value = gtmenu_torque_display_value(r_u16(model + 412) / 10);
        rpm = r_u16(model + 414);
    }
    gtmenu_format_integer_string_integer(buffer, 0x800541F0, value, torque_unit, rpm);
    gtmenu_text_draw(ordering, 270, 220, buffer);
    text = r_u32(0x800584A8 + r_u8(model + 422) * 4);
    gtmenu_format_string(buffer, 0x80054200, text);
    gtmenu_text_draw(ordering, 270, 240, buffer);
    text = r_u32(0x800584A8 + r_u8(model + 423) * 4);
    gtmenu_format_string(buffer, 0x80054200, text);
    gtmenu_text_draw(ordering, 270, 260, buffer);
    first = 10 * r_u8(stats + 104) + 5;
    second = 5 * r_u8(stats + 106);
    third = r_u8(stats + 102);
    gtmenu_format_integers3(buffer, 0x80054204, first, second, third);
    gtmenu_text_draw(ordering, 270, 280, buffer);
    first = 10 * r_u8(stats + 105) + 5;
    second = 5 * r_u8(stats + 107);
    third = r_u8(stats + 103);
    gtmenu_format_integers3(buffer, 0x80054204, first, second, third);
    gtmenu_text_draw(ordering, 270, 300, buffer);
    if (rotors) {
        value = gtmenu_displacement_display_value(654);
        gtmenu_format_integer_string_char_integer(buffer, 0x80054210, value, displacement_unit, 'x', rotors);
    } else {
        value = gtmenu_displacement_display_value(r_u16(model + 406));
        gtmenu_format_integer_string(buffer, 0x800541DC, value, displacement_unit);
    }
    gtmenu_text_draw(ordering, 270, 320, buffer);
    first = r_u8(model + 420);
    second = r_u8(model + 421);
    first = r_u32(0x80058498 + first * 4);
    second = r_u32(0x80058478 + second * 4);
    gtmenu_format_strings2(buffer, 0x8005421C, first, second);
    gtmenu_text_draw(ordering, 270, 340, buffer);
    text = r_u32(0x8005846C + r_u8(model + 419) * 4);
    gtmenu_format_string(buffer, 0x80054200, text);
    result = gtmenu_text_draw(ordering, 270, 360, buffer);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_detail_frame_draw(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 ordering, record, resources, model, packed, names, text, rectangle, result = 1;
    FUNCTION_MARKER(0x8001F450, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(264, 0x8001F450);
    text = frame.address;
    rectangle = frame.address + 256;
    ordering = gtmenu_current_ordering();
    record = r_u32(0x8005864C);
    resources = gtmenu_resource_group(13);
    model = resources + 424 * (r_u16(record) & 0xFFF) - 400;
    packed = r_u32(model + 392);
    names = r_u32(resources + 16);
    names = r_u32(names + (packed >> 16) * 4 + 4);
    names = gtmenu_packed_text_entry(names, packed & 0xFFFF);
    gtmenu_string_copy(text, names + 1);
    gtmenu_string_append(text, 0x80054224);
    packed = r_u32(model + 396);
    names = r_u32(resources + 16);
    names = r_u32(names + (packed >> 16) * 4 + 4);
    names = gtmenu_packed_text_entry(names, packed & 0xFFFF);
    gtmenu_string_append(text, names + 1);
    gtmenu_text_draw(ordering, 32, 32, text);
    w_u32(rectangle, 0x003A0222);
    w_u32(rectangle + 4, 0x005D0267);
    if (r_s8(0x8005864A) == 2) gtmenu_rectangle_pulse(ordering, rectangle, 0);
    else gtmenu_rectangle_shade(ordering, rectangle, 255);
    if (!r_s8(0x8005864B)) result = gtmenu_model_specifications_draw(record, model);
    else if (r_s8(0x8005864B) == 1) result = gtmenu_installed_parts_draw(record);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_detail_activate(void)
{
    sint32 mode;
    uint32 action;
    FUNCTION_MARKER(0x8001F5F0, "GTMENU.EXE");
    mode = r_s8(0x8005864A);
    action = r_u32(0x800A7A5C);
    if (mode == 2) {
        if (!gtmenu_string_compare(action, 0x80054228)) {
            gtmenu_sound_effect(5);
            gtmenu_page_open(0x80054234);
            w_u8(0x8005864B, 0);
            return 1;
        }
        if (!gtmenu_string_compare(action, 0x80054244)) {
            gtmenu_sound_effect(5);
            gtmenu_page_open(0x80054250);
            w_u8(0x8005864B, 1);
            return 1;
        }
    }
    if (!gtmenu_string_compare(action, 0x80054260)) {
        gtmenu_sound_effect(5);
        gtmenu_model_info_page_restore();
        return 1;
    }
    if (gtmenu_ui_default_activate()) {
        gtmenu_model_info_actions_activate();
        return 1;
    }
    gtmenu_sound_effect(6);
    return 0;
}

uint32 gtmenu_model_detail_event(uint32 event)
{
    uint32 kind, buttons;
    FUNCTION_MARKER(0x8001F70C, "GTMENU.EXE");
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_model_detail_frame_draw();
        return gtmenu_scene_draw(r_u32(event + 4));
    }
    if (kind != 24) return 24;
    if (r_u32(event + 12) & 0x9000) {
        gtmenu_sound_effect(3);
        return gtmenu_model_info_page_restore();
    }
    buttons = r_u32(event + 12) & 0x6000;
    if (buttons) return gtmenu_model_detail_activate();
    return buttons;
}

uint32 gtmenu_model_spec_open(uint32 record, uint32 settings, uint32 stats, sint16 category, sint8 mode)
{
    uint32 previous_callback, previous_page;
    FUNCTION_MARKER(0x8001F7B0, "GTMENU.EXE");
    previous_callback = r_u32(0x800A77B4);
    w_u16(0x80058648, category);
    w_u32(0x8005864C, record);
    w_u32(0x80058650, settings);
    w_u32(0x80058654, stats);
    w_u8(0x8005864B, 0);
    w_u8(0x8005864A, mode);
    w_u32(0x8008E8FC, previous_callback);
    gtmenu_car_spec_filter_set(mode != 2);
    previous_page = r_u16(0x800A77C4);
    w_u32(0x800A77B4, 0x8001F70C);
    w_u16(0x800A77C4, 660);
    w_u16(0x800A77C0, previous_page);
    return gtmenu_page_open(0x80054234);
}

uint32 gtmenu_model_logo_reset(void)
{
    FUNCTION_MARKER(0x8001FA08, "GTMENU.EXE");
    gtmenu_memory_fill(0x8008E900, 0, 8);
    w_u32(0x800588D0, -1);
    return 0xFFFFFFFF;
}

uint32 gtmenu_current_car_logo_load(void)
{
    uint32 models, record, model;
    sint32 index;
    FUNCTION_MARKER(0x8001FBC8, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    index = gtmenu_current_record_index();
    if (index < 0) return 0;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    model = models + 424 * (r_u16(record) & 0xFFF);
    return gtmenu_model_logo_load(model);
}

uint32 gtmenu_textured_rectangle_packet(uint32 ordering, uint32 position, uint32 uv_clut, uint32 size, uint32 color)
{
    uint32 packet, link;
    FUNCTION_MARKER(0x80034780, "GTMENU.EXE");
    packet = r_u32(0x800A8434);
    w_u32(packet, 0x04000000);
    w_u32(packet + 8, position);
    w_u32(packet + 12, uv_clut);
    w_u32(packet + 16, size);
    w_u32(packet + 4, color ^ 0x65000000);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, packet, 7);
    w_masked_u32(packet, link, 7);
    w_u32(0x800A8434, packet + 20);
    return packet;
}

uint32 gtmenu_model_preview_draw(uint32 ordering, sint32 x, sint32 y)
{
    uint32 position, entry, scaled, color;
    sint32 code, offset, selected;
    FUNCTION_MARKER(0x8001FC3C, "GTMENU.EXE");
    position = ((uint32)x & 0xFFFF) | ((uint32)y << 16);
    if (r_u32(0x800588D8)) entry = 0x80058878;
    else entry = 0x800586B8;
    entry += r_u32(0x800588D0) * 8;
    code = r_s16(entry + 4);
    offset = r_s16(entry + 6);
    scaled = 1280u * r_u32(0x800588D4);
    color = (uint32)((sint32)scaled / 100) & 0xFF;
    color = color | (color << 8) | (color << 16) | 0x01000000;
    gtmenu_textured_rectangle_packet(ordering, position, 0x79A8D000, 0x00300100, color);
    gtmenu_texture_page_draw(ordering, 607);
    selected = r_s32(0x800588D0);
    if (selected < 0) return (uint32)selected;
    return gtmenu_logo_caption_draw(ordering, (sint16)((uint32)x + (uint32)offset), (sint16)y, code);
}

uint32 gtmenu_logo_caption_draw(uint32 ordering, sint16 x, sint16 y, sint32 code)
{
    uint32 position, scaled, entry, page, width, clut_x, clut_y, clut, height, u, v, uv, color;
    FUNCTION_MARKER(0x800241A4, "GTMENU.EXE");
    position = (uint16)x | ((uint32)(uint16)y << 16);
    scaled = 1280u * r_u32(0x80058B28);
    entry = (uint32)code * 4;
    width = r_u16(0x80058BFC + entry);
    clut_x = r_u16(0x80058B2C + entry);
    clut_y = r_u16(0x80058B2E + entry);
    clut = (uint32)((sint32)(clut_x << 16) >> 20) | (clut_y << 6);
    clut <<= 16;
    height = r_s16(0x80058BFE + entry);
    v = r_u8(0x80058B96 + entry);
    u = r_s16(0x80058B94 + entry);
    u = ((u - 0x3C0) << 2) & 0xFF;
    uv = u | (v << 8) | clut;
    color = (uint32)((sint32)scaled / 100) & 0xFF;
    color = color | (color << 8) | (color << 16) | 0x01000000;
    page = (uint32)code - 24 < 3 ? 13 : 31;
    gtmenu_textured_rectangle_packet(ordering, position, uv, width | (height << 16), color);
    return gtmenu_texture_page_draw(ordering, page | 0x240);
}

uint32 gtmenu_page_back_target(void)
{
    FUNCTION_MARKER(0x80024AB8, "GTMENU.EXE");
    if (!r_s8(0x80058C8C)) return 0;
    return 0x800AAB6C;
}

uint32 gtmenu_ui_list_has_entries(uint32 secondary)
{
    uint32 widget, entries;
    FUNCTION_MARKER(0x8001A8C4, "GTMENU.EXE");
    widget = r_u32(secondary ? 0x80058444 : 0x80058440);
    entries = r_u32(widget + 16);
    return r_u32(entries) != 0;
}

uint32 gtmenu_model_info_actions_event(uint32 event)
{
    uint32 kind, page, target, result;
    FUNCTION_MARKER(0x8001FDA4, "GTMENU.EXE");
    kind = r_u32(event);
    page = r_u16(0x800A77C6);
    if (kind != 24) {
        if (kind != 3) return 3;
        gtmenu_scene_draw(r_u32(event + 4));
        return gtmenu_page_timed_action();
    }
    if (r_u32(event + 12) & 0x9000) {
        if (!gtmenu_page_back_target()) return gtmenu_sound_effect(6);
        gtmenu_sound_effect(3);
        target = gtmenu_page_back_target();
        gtmenu_page_transition_prepare(target);
        target = gtmenu_page_back_target();
        return gtmenu_page_open(target);
    }
    if (r_u32(event + 12) & 0x6000) return gtmenu_ui_default_activate();
    if (!(r_u32(event + 12) & 0x20) || page != 57) return 57;
    result = gtmenu_ui_list_has_entries(0);
    if (result) return gtmenu_ui_selection_mode_set(2);
    return result;
}

uint32 gtmenu_model_more_restore(void)
{
    uint32 callback, target, previous;
    FUNCTION_MARKER(0x8001FEBC, "GTMENU.EXE");
    callback = r_u32(0x8008E908);
    w_u32(0x800A77B4, callback);
    gtmenu_model_preview_pitch_set(-96);
    target = r_u16(0x800588DC);
    previous = r_u16(0x800A77C4);
    w_u16(0x800A77C0, previous);
    w_u16(0x800A77C4, target);
    return gtmenu_model_info_page_open((sint16)target);
}

uint32 gtmenu_model_more_activate(void)
{
    uint32 action;
    FUNCTION_MARKER(0x8001FF18, "GTMENU.EXE");
    action = r_u32(0x800A7A5C);
    if (!gtmenu_string_compare(action, 0x80054458)) {
        gtmenu_sound_effect(5);
        gtmenu_model_more_restore();
        return 1;
    }
    if (!gtmenu_string_compare_folded(action, 0x8008E910)) {
        gtmenu_sound_effect(5);
        gtmenu_page_open(0x8008E910);
        return 0;
    }
    if (!gtmenu_string_compare_folded(action, 0x800A7B00)) {
        gtmenu_sound_effect(5);
        gtmenu_page_open(0x800A7B00);
        return 0;
    }
    if (!gtmenu_string_compare_folded(action, 0x8008E950)) {
        gtmenu_sound_effect(5);
        gtmenu_page_open(0x8008E950);
        return 0;
    }
    if (action) {
        gtmenu_sound_effect(5);
        gtmenu_page_open(action);
    } else gtmenu_sound_effect(6);
    return 0;
}

uint32 gtmenu_model_more_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x8002004C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8002004C);
    w_u32(frame.address, 0x00200032);
    w_u32(frame.address + 4, 0x00BE024D);
    gtmenu_model_preview_draw(ordering, 80, 48);
    gtmenu_color_picker_bounds_set(frame.address);
    result = gtmenu_model_preview_frame_draw();
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_more_event(uint32 event)
{
    uint32 kind, buttons;
    FUNCTION_MARKER(0x800200E4, "GTMENU.EXE");
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_model_info_angle_add(5);
        gtmenu_model_more_draw(r_u32(event + 4));
        return gtmenu_scene_draw(r_u32(event + 4));
    }
    if (kind != 24) return 24;
    if (r_u32(event + 12) & 0x9000) {
        gtmenu_sound_effect(3);
        return gtmenu_model_more_restore();
    }
    buttons = r_u32(event + 12) & 0x6000;
    if (buttons) return gtmenu_model_more_activate();
    return buttons;
}

uint32 gtmenu_model_more_open(uint32 record, sint16 category, sint8 mode)
{
    uint32 models, model, callback, previous;
    uint16 index;
    FUNCTION_MARKER(0x800201C0, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    index = (r_u16(record) & 0xFFF) - 1;
    model = models + 424 * index;
    if (!gtmenu_model_has_more_info(model)) return 0;
    callback = r_u32(0x800A77B4);
    w_u16(0x800588DC, category);
    w_u8(0x800588DE, mode);
    w_u32(0x800588E0, record);
    w_u32(0x8008E908, callback);
    gtmenu_string_copy(0x8008E910, 0x80054464);
    gtmenu_string_append(0x8008E910, model);
    gtmenu_string_copy(0x800A7B00, 0x8008E910);
    gtmenu_string_append(0x800A7B00, 0x80054468);
    gtmenu_string_copy(0x8008E950, 0x8008E910);
    gtmenu_string_append(0x8008E950, 0x8005446C);
    gtmenu_string_append(0x8008E910, 0x80054470);
    gtmenu_string_append(0x800A7B00, 0x80054470);
    gtmenu_string_append(0x8008E950, 0x80054470);
    gtmenu_model_preview_pitch_set(-10);
    previous = r_u16(0x800A77C4);
    w_u32(0x800A77B4, 0x800200E4);
    w_u16(0x800A77C4, 48);
    w_u16(0x800A77C0, previous);
    return gtmenu_page_open(0x8008E910);
}

uint32 gtmenu_confirmation_restore(void)
{
    uint32 callback, previous, target, category;
    FUNCTION_MARKER(0x8002037C, "GTMENU.EXE");
    callback = r_u32(0x8008E990);
    previous = r_u16(0x800A77C4);
    target = r_u16(0x800588E8);
    category = r_u16(0x800588EA);
    w_u32(0x800588E4, 0);
    w_u16(0x800A77C4, target);
    w_u32(0x800A77B4, callback);
    w_u16(0x800A77C0, previous);
    w_u16(0x800A77C6, category);
    return gtmenu_model_info_page_open((sint16)target);
}

uint32 gtmenu_proposed_power(void)
{
    FUNCTION_MARKER(0x8001D684, "GTMENU.EXE");
    return r_u16(0x80082334);
}

uint32 gtmenu_confirmation_draw(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 ordering, text, buffer, rectangle, current, proposed, result = 14, difference;
    sint32 style, width, first_power, second_power;
    FUNCTION_MARKER(0x800203E8, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(40, 0x800203E8);
    buffer = frame.address;
    rectangle = frame.address + 32;
    ordering = gtmenu_current_ordering();
    style = r_s8(0x800588F0);
    text = r_u32(0x800588EC);
    if (!style) gtmenu_model_preview_draw(ordering, 192, 160);
    else if (r_s8(0x800588F0) == 1) {
        gtmenu_text_font_set(0);
        gtmenu_text_color_set(0x808080);
        if (text) {
            width = gtmenu_text_width(0, text);
            difference = 640u - (uint32)width;
            gtmenu_text_draw(ordering, (sint16)((sint32)difference / 2), 180, text);
        }
    }
    if (r_s8(0x800588F2)) {
        current = gtmenu_current_power();
        proposed = gtmenu_proposed_power();
        if (current == proposed) {
            w_u32(rectangle, 0x01120000);
            w_u32(rectangle + 4, 0x0142027F);
            gtmenu_rectangle_shade(ordering, rectangle, 255);
        } else {
            first_power = (sint32)(current * 1000) / 1014;
            second_power = (sint32)(proposed * 1000) / 1014;
            gtmenu_format_integer_string(buffer, 0x800545CC, first_power, 0x800A7556);
            gtmenu_text_color_set(0x347500);
            gtmenu_text_font_set(1);
            gtmenu_text_draw(ordering, 229, 288, buffer);
            gtmenu_format_integer_string(buffer, 0x800545CC, second_power, 0x800A7556);
            gtmenu_text_color_set(0x8080);
            gtmenu_text_font_set(1);
            gtmenu_text_draw(ordering, 352, 288, buffer);
        }
    }
    if (r_s8(0x800588F1) == 14) {
        gtmenu_hud_current_car_draw(ordering);
        gtmenu_hud_credits_draw(ordering);
        result = gtmenu_hud_days_draw(ordering);
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_confirmation_accept(void)
{
    FUNCTION_MARKER(0x80020638, "GTMENU.EXE");
    gtmenu_sound_effect(5);
    return gtmenu_confirmation_restore();
}

uint32 gtmenu_confirmation_cancel(void)
{
    FUNCTION_MARKER(0x80020660, "GTMENU.EXE");
    gtmenu_sound_effect(3);
    return gtmenu_confirmation_restore();
}

uint32 gtmenu_hud_current_car_draw(uint32 ordering)
{
    uint32 resources, record, model, packed, names;
    sint32 index;
    FUNCTION_MARKER(0x80022AFC, "GTMENU.EXE");
    resources = gtmenu_resource_group(13);
    index = gtmenu_current_record_index();
    if (index < 0) return (uint32)index;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    model = resources + 24 + 424 * (r_u16(record) & 0xFFF);
    packed = r_u32(model + 392);
    names = r_u32(resources + 16);
    names = r_u32(names + (packed >> 16) * 4 + 4);
    names = gtmenu_packed_text_entry(names, packed & 0xFFFF);
    return gtmenu_text_draw(ordering, 400, 416, names + 1);
}

uint32 gtmenu_hud_credits_draw(uint32 ordering)
{
    sint64 credits;
    FUNCTION_MARKER(0x80022A6C, "GTMENU.EXE");
    credits = (sint64)(sint32)gtmenu_credits_get() * 100;
    gtmenu_text_color_set(credits < 0 ? 0x80 : 0x8080);
    gtmenu_text_font_set(0);
    return gtmenu_price_draw(ordering, 310, 416, 1, credits);
}

uint32 gtmenu_hud_days_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    sint32 days;
    FUNCTION_MARKER(0x80022BDC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(32, 0x80022BDC);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    days = gtmenu_days_get();
    gtmenu_format_integer(frame.address, 0x800A6C90, days);
    result = gtmenu_text_draw(ordering, 24, 416, frame.address);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_price_draw(uint32 ordering, sint32 x, sint16 y, uint32 alignment, sint64 price)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 plain, grouped, cursor, position, result = 2;
    uint8 character;
    sint64 value;
    FUNCTION_MARKER(0x8001B4EC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(256, 0x8001B4EC);
    plain = frame.address;
    grouped = frame.address + 128;
    value = gtmenu_signed_divide64(price, 100, 0x8001B530);
    gtmenu_format_signed64(plain, 0x80053D50, value);
    gtmenu_decimal_group(grouped, plain);
    position = x;
    if (alignment == 1) {
        result = gtmenu_text_comma_width(r_s16(0x800A7AFA), grouped);
        position -= result;
    } else if (alignment == 2) {
        result = gtmenu_text_comma_width(r_s16(0x800A7AFA), grouped);
        position -= (sint32)result / 2;
    }
    character = r_u8(grouped);
    cursor = grouped;
    w_u16(0x800A7AF0, position);
    w_u16(0x800A7AF2, y);
    w_u16(0x800A7AFC, 0);
    w_u32(0x800A7AF4, ordering);
    while (character) {
        if (character == ',') w_u16(0x800A7AFC, r_u16(0x800A7AFC) - 6);
        ++cursor;
        result = gtmenu_glyph_draw_numeric(character);
        character = r_u8(cursor);
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_confirmation_event(uint32 event)
{
    uint32 kind, buttons;
    FUNCTION_MARKER(0x80020CC4, "GTMENU.EXE");
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_confirmation_draw();
        return gtmenu_scene_draw(r_u32(event + 4));
    }
    if (kind != 24) return 24;
    buttons = r_u32(event + 12) & 0x6000;
    if (buttons) return gtmenu_confirmation_activate();
    return buttons;
}

uint32 gtmenu_confirmation_open(uint32 kind, uint32 text, sint16 target, sint8 style)
{
    uint32 callback, category, previous;
    FUNCTION_MARKER(0x80020D4C, "GTMENU.EXE");
    w_u32(0x800588EC, text);
    callback = r_u32(0x800A77B4);
    w_u32(0x800588E4, 1);
    w_u32(0x800A77B4, 0x80020CC4);
    category = r_u16(0x800A77C6);
    previous = r_u16(0x800A77C4);
    w_u16(0x800A77C4, 658);
    w_u16(0x800588E8, target);
    w_u8(0x800588F0, style);
    w_u8(0x800588F1, kind);
    w_u8(0x800588F2, 0);
    w_u16(0x800A77C6, 0);
    w_u16(0x800588EA, category);
    w_u32(0x8008E990, callback);
    w_u16(0x800588EA, category);
    w_u16(0x800A77C0, previous);
    if (kind == 9) gtmenu_current_car_logo_load();
    else if (kind == 14) {
        previous = r_u16(0x800A77C4);
        w_u16(0x800A77C4, 659);
        w_u16(0x800A77C0, previous);
    }
    return gtmenu_page_open(r_u32(0x800588F4 + kind * 4));
}

uint32 gtmenu_special_model_action(uint32 action)
{
    uint32 index, entry, id;
    FUNCTION_MARKER(0x80020E58, "GTMENU.EXE");
    for (index = 0; index < 6; ++index) {
        entry = 0x80058AD0 + index * 8;
        if (!gtmenu_string_compare_folded(action, r_u32(entry))) {
            if (gtmenu_model_name_index(r_u32(entry + 4)) < 0) gtmenu_sound_effect(6);
            else {
                gtmenu_sound_effect(5);
                id = gtmenu_model_index_by_name(r_u32(entry + 4));
                gtmenu_model_info_open_id(id, r_u16(0x800A77C4));
            }
            return 1;
        }
    }
    return 0;
}

uint32 gtmenu_model_code_action(uint32 action)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 index, id;
    FUNCTION_MARKER(0x80020F24, "GTMENU.EXE");
    if (r_s8(action) != 'C' || r_s8(action + 1) != 'C') return 0;
    frame = gtmenu_local_stack_push(8, 0x80020F24);
    gtmenu_string_copy(frame.address, action + 2);
    for (index = 0; index < 5; ++index)
        w_u8(frame.address + index, r_u8(frame.address + index) | 0x20);
    if (gtmenu_model_name_index(frame.address) < 0) gtmenu_sound_effect(6);
    else {
        gtmenu_sound_effect(5);
        id = gtmenu_model_index_by_name(frame.address);
        gtmenu_model_info_open_id(id, r_u16(0x800A77C4));
    }
    gtmenu_local_stack_pop(frame);
    return 1;
}

uint32 gtmenu_progress_three_complete(void)
{
    FUNCTION_MARKER(0x80020FD4, "GTMENU.EXE");
    if (gtmenu_progress_byte(0) != 1) return 0;
    if (gtmenu_progress_byte(1) != 1) return 0;
    return gtmenu_progress_byte(2) == 1;
}

uint32 gtmenu_event_license_check(sint32 kind)
{
    uint32 prompt;
    sint32 index;
    FUNCTION_MARKER(0x80021038, "GTMENU.EXE");
    index = gtmenu_current_record_index();
    if (index < 0) return 0;
    index = gtmenu_current_record_index();
    gtmenu_record_address(index);
    switch (kind) {
    case 0:
        if (gtmenu_license_grade(1)) return 1;
        prompt = 5;
        break;
    case 1:
    case 2:
        if (gtmenu_license_grade(2)) return 1;
        prompt = 6;
        break;
    case 3:
        if (!gtmenu_progress_three_complete()) prompt = 16;
        else {
            if (gtmenu_license_grade(3)) return 1;
            prompt = 7;
        }
        break;
    default: return 0;
    }
    gtmenu_sound_effect(6);
    gtmenu_confirmation_open(prompt, 0, r_u16(0x800A77C4), 2);
    return 0;
}

uint32 gtmenu_special_event_check(uint32 kind)
{
    sint32 index;
    uint32 record, prompt;
    FUNCTION_MARKER(0x80021188, "GTMENU.EXE");
    index = gtmenu_current_record_index();
    if (index < 0) return 0;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    switch (kind) {
    case 0:
        if (!gtmenu_license_grade(2)) prompt = 6;
        else {
            if (!gtmenu_record_has_parts(record) && !gtmenu_record_matches_filter(record, 2, 0)) return 1;
            prompt = 20;
        }
        break;
    case 1:
        if (!gtmenu_license_grade(3)) prompt = 7;
        else {
            if (!gtmenu_record_matches_filter(record, 2, 0) && !gtmenu_record_matches_filter(record, 4, 0)) return 1;
            prompt = 25;
        }
        break;
    case 2:
        if (!gtmenu_license_grade(1)) prompt = 5;
        else {
            if (gtmenu_record_matches_filter(record, 3, 0)) return 1;
            prompt = 24;
        }
        break;
    case 3:
        if (!gtmenu_license_grade(1)) prompt = 5;
        else {
            if (gtmenu_record_matches_filter(record, 1, 1)) return 1;
            prompt = 21;
        }
        break;
    case 4:
        if (!gtmenu_license_grade(1)) prompt = 5;
        else {
            if (gtmenu_record_matches_filter(record, 1, 0)) return 1;
            prompt = 22;
        }
        break;
    case 5:
        if (!gtmenu_license_grade(1)) prompt = 5;
        else {
            if (gtmenu_record_matches_filter(record, 1, 2)) return 1;
            prompt = 23;
        }
        break;
    default: return 0;
    }
    gtmenu_sound_effect(6);
    gtmenu_confirmation_open(prompt, 0, r_u16(0x800A77C4), 2);
    return 0;
}

uint32 gtmenu_entry_unavailable_prompt(void)
{
    FUNCTION_MARKER(0x80021484, "GTMENU.EXE");
    gtmenu_sound_effect(6);
    return gtmenu_confirmation_open(4, 0, r_u16(0x800A77C4), 2);
}

uint32 gtmenu_championship_entry_check(uint32 kind)
{
    sint32 index;
    uint32 record, prompt;
    FUNCTION_MARKER(0x800214BC, "GTMENU.EXE");
    index = gtmenu_current_record_index();
    if (index < 0) return 0;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    switch (kind) {
    case 3:
        if (gtmenu_license_grade(2)) return 1;
        prompt = 6;
        break;
    case 4:
        if (!gtmenu_license_grade(2)) prompt = 6;
        else {
            if (gtmenu_record_matches_filter(record, 0, 0) || gtmenu_record_matches_filter(record, 0, 1)) return 1;
            prompt = 17;
        }
        break;
    case 5:
        if (!gtmenu_license_grade(2)) prompt = 6;
        else {
            if (gtmenu_record_matches_filter(record, 0, 0) || gtmenu_record_matches_filter(record, 0, 2)) return 1;
            prompt = 18;
        }
        break;
    case 6:
        if (!gtmenu_license_grade(2)) prompt = 6;
        else {
            if (gtmenu_record_matches_filter(record, 0, 1) || gtmenu_record_matches_filter(record, 0, 2)) return 1;
            prompt = 19;
        }
        break;
    case 7:
    case 8:
        if (gtmenu_license_grade(3)) return 1;
        prompt = 7;
        break;
    case 9:
        if (!gtmenu_license_grade(3)) prompt = 7;
        else {
            if (!gtmenu_record_matches_filter(record, 2, 0) && !gtmenu_record_matches_filter(record, 4, 0)) return 1;
            prompt = 25;
        }
        break;
    default: return 1;
    }
    gtmenu_sound_effect(6);
    gtmenu_confirmation_open(prompt, 0, r_u16(0x800A77C4), 2);
    return 0;
}

sint32 gtmenu_race_action(uint32 action)
{
    uint32 group, index, failure, prompt, selection;
    sint32 current, category;
    FUNCTION_MARKER(0x80021778, "GTMENU.EXE");
    gtmenu_input_state_address(0);
    current = gtmenu_current_record_index();
    for (group = 0; group < 3; ++group) {
        for (index = 0; index < 8; ++index) {
            if (gtmenu_string_compare(action, r_u32(0x800589B0 + group * 32 + index * 4))) continue;
            if (!group) failure = index == 7 && !gtmenu_license_tests_complete(1);
            else if (index == 7) failure = 2 * !gtmenu_license_tests_complete(group + 1);
            else failure = !gtmenu_license_entry_unlocked(group + 1);
            if (failure) {
                gtmenu_sound_effect(6);
                prompt = !group || failure == 2 ? 8 : group + 4;
                gtmenu_confirmation_open(prompt, 0, group + 9, 2);
                return -1;
            }
            gtmenu_sound_effect(5);
            gtmenu_race_request(4, group, 0, 0, index);
            gtmenu_progress_selection_set(group + 8);
            w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
            return 1;
        }
    }
    for (index = 0; index < 4; ++index) {
        if (gtmenu_string_compare(action, r_u32(0x80058A10 + index * 4))) continue;
        if (current < 0) {
            gtmenu_entry_unavailable_prompt();
            return -1;
        }
        if (!gtmenu_event_license_check(index)) return -1;
        gtmenu_sound_effect(5);
        gtmenu_race_request(3, index, 0, 0, index);
        gtmenu_progress_selection_set(7);
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
        return 1;
    }
    for (index = 0; index < 6; ++index) {
        if (gtmenu_string_compare(action, r_u32(0x80058A20 + index * 8))) continue;
        if (current < 0) {
            gtmenu_entry_unavailable_prompt();
            return -1;
        }
        if (!gtmenu_special_event_check(index)) return -1;
        gtmenu_sound_effect(5);
        gtmenu_race_request(3, index + 4, 0, 0, index);
        gtmenu_progress_selection_set(13);
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
        return 1;
    }
    for (index = 0; index < 12; ++index) {
        if (gtmenu_string_compare(action, r_u32(0x80058A50 + index * 8))) continue;
        if (current < 0) {
            gtmenu_entry_unavailable_prompt();
            return -1;
        }
        if (!gtmenu_championship_entry_check(index)) return -1;
        gtmenu_sound_effect(5);
        gtmenu_race_request(5, 0, 0, 0, index);
        category = r_s32(0x80058A54 + index * 8);
        if (category == 2) selection = 18;
        else if (category == 1) selection = 17;
        else if (category == 3) selection = 16;
        else selection = 13;
        gtmenu_progress_selection_set(selection);
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
        return 1;
    }
    for (index = 0; index < 21; ++index) {
        if (gtmenu_string_compare(action, r_u32(0x8005895C + index * 4))) continue;
        if (current < 0) {
            gtmenu_entry_unavailable_prompt();
            return -1;
        }
        gtmenu_sound_effect(5);
        gtmenu_race_request(1, 0, 0, 0, index);
        gtmenu_progress_selection_set(12);
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
        return 1;
    }
    for (index = 0; index < 5; ++index) {
        if (gtmenu_string_compare(action, r_u32(0x80058AB0 + index * 4))) continue;
        if (current < 0) {
            gtmenu_entry_unavailable_prompt();
            return -1;
        }
        gtmenu_sound_effect(5);
        gtmenu_race_request(3, index + 10, 0, 0, 0);
        gtmenu_progress_selection_set(14);
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
        return 1;
    }
    for (index = 0; index < 3; ++index) {
        if (gtmenu_string_compare_folded(action, r_u32(0x80058AC4 + index * 4))) continue;
        if (current < 0) {
            gtmenu_entry_unavailable_prompt();
            return -1;
        }
        gtmenu_sound_effect(5);
        gtmenu_race_request(6, 0, 0, 0, index);
        gtmenu_progress_selection_set(13);
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000002);
        return 1;
    }
    return 0;
}

uint32 gtmenu_page_transition_prepare(uint32 page)
{
    sint32 category;
    uint32 progress;
    FUNCTION_MARKER(0x80021ED8, "GTMENU.EXE");
    category = gtmenu_page_category_set(page);
    gtmenu_hifi_filter_set(1);
    if ((uint32)category - 165 < 490) {
        gtmenu_available_parts_initialize();
        gtmenu_current_car_preview_initialize();
        return category;
    }
    switch (category) {
    case 1: gtmenu_music_request(1); break;
    case 2:
    case 3: gtmenu_music_request(0); break;
    case 8:
    case 12:
    case 40: gtmenu_music_request(3); break;
    case 19:
        progress = gtmenu_progress_byte(3);
        gtmenu_debug_integer(0x80054C0C, progress);
        if (gtmenu_progress_byte(3) == 1) gtmenu_hifi_filter_set(0);
        break;
    case 49:
        gtmenu_music_request(2);
        gtmenu_current_car_mileage_open();
        break;
    case 50: gtmenu_music_request(11); break;
    case 54: gtmenu_music_request(13); break;
    case 58: gtmenu_music_request(9); break;
    case 62: gtmenu_music_request(8); break;
    case 66: gtmenu_music_request(10); break;
    case 70: gtmenu_music_request(6); break;
    case 74: gtmenu_music_request(4); break;
    case 78: gtmenu_music_request(7); break;
    case 82: gtmenu_music_request(5); break;
    case 86: gtmenu_music_request(12); break;
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
    case 109: gtmenu_current_car_caption_open(r_u16(0x800A77C4)); break;
    case 656: gtmenu_music_stop(); break;
    }
    return category;
}

uint32 gtmenu_save_payload_valid(uint32 data)
{
    uint32 checksum;
    FUNCTION_MARKER(0x8001BC6C, "GTMENU.EXE");
    checksum = gtend_data_checksum(data, 27556);
    return r_u32(data + 27556) == checksum;
}

uint32 gtmenu_loaded_payload_apply(uint32 data)
{
    uint32 result;
    gtmenu_save_payload_import(data);
    gtmenu_sound_settings_apply();
    result = gtmenu_days_get();
    if (!result) {
        gtmenu_player_state_initialize();
        result = r_u32(0x800A2408) | 0x10C40037;
        w_u32(0x800A2408, result);
    }
    return result;
}

uint32 gtmenu_loaded_game_apply(void)
{
    FUNCTION_MARKER(0x80027958, "GTMENU.EXE");
    return gtmenu_loaded_payload_apply(r_u32(0x8009AC30) + 512);
}

uint32 gtmenu_ui_default_activate(void)
{
    uint32 action, category;
    sint32 race;
    FUNCTION_MARKER(0x8002219C, "GTMENU.EXE");
    action = r_u32(0x800A7A5C);
    category = r_u16(0x800A77C6);
    if (!action) {
        gtmenu_sound_effect(6);
        return 0;
    }
    race = gtmenu_race_action(action);
    if (race) return ~(uint32)race >> 31;
    if (!gtmenu_string_compare(action, 0x80054C28)) {
        if (gtmenu_ui_list_has_entries(0)) {
            gtmenu_sound_effect(5);
            gtmenu_ui_selection_mode_set(2);
        } else gtmenu_sound_effect(6);
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C34)) {
        if (gtmenu_record_count()) {
            gtmenu_sound_effect(5);
            gtmenu_ui_selection_mode_set(2);
        } else gtmenu_sound_effect(6);
        return 0;
    }
    if (gtmenu_model_code_action(action)) return 0;
    if (gtmenu_special_model_action(action)) return 0;
    if (!gtmenu_string_compare(action, 0x80054C40)) {
        gtmenu_parts_purchase_open();
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C44) && category == 25) {
        gtmenu_sound_effect(5);
        gtmenu_car_info_color_toggle();
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C58)) {
        gtmenu_sound_effect(5);
        gtmenu_versus_open();
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C5C)) {
        gtmenu_sound_effect(5);
        if (gtmenu_native_save_action(0) < 0) gtmenu_sound_effect(6);
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C64)) {
        gtmenu_sound_effect(5);
        if (gtmenu_native_save_action(1) < 0) gtmenu_sound_effect(6);
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C6C)) {
        gtmenu_sound_effect(5);
        gtmenu_trade_open();
        return 0;
    }
    if (!gtmenu_string_compare(action, 0x80054C78)) {
        gtmenu_sound_effect(5);
        gtmenu_race_info_open(r_u16(0x800A77C4));
        return 0;
    }
    if (!gtmenu_string_compare_folded(action, 0x80054C84)) {
        gtmenu_sound_effect(5);
        gtmenu_progress_selection_set(4);
        gtmenu_music_stop();
        w_u32(0x80056C58, r_u32(0x80056C58) | 0x80000001);
    } else {
        gtmenu_sound_effect(5);
        gtmenu_page_transition_prepare(action);
        gtmenu_page_open(action);
    }
    gtmenu_debug_message(0x80054C90);
    return 1;
}

uint32 gtmenu_page_timed_action(void)
{
    uint32 counter, previous;
    FUNCTION_MARKER(0x80026538, "GTMENU.EXE");
    if (r_u16(0x800A77C4) != 655) return 0;
    counter = r_u16(0x800A77B2) + 1;
    w_u16(0x800A77B2, counter);
    if ((sint16)counter < 900) return 0;
    previous = r_u16(0x800A77C4);
    w_u16(0x800A77C4, 656);
    w_u16(0x800A77C0, previous);
    gtmenu_page_open(0x800A768C);
    return 1;
}

uint32 gtmenu_rectangle_pulse_reset(void)
{
    FUNCTION_MARKER(0x8001B7E4, "GTMENU.EXE");
    return gtmenu_memory_fill(0x80081F80, 0, 4);
}

uint32 gtmenu_render_auxiliary_reset(void)
{
    FUNCTION_MARKER(0x8002247C, "GTMENU.EXE");
    return gtmenu_memory_fill(0x800A7B40, 0, 4);
}

void gtmenu_progress_badge_draw(uint32 ordering, sint16 x, sint16 y, sint32 progress)
{
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800224A8, "GTMENU.EXE");
    if (progress <= 0) return;
    if (progress <= 3) {
        gtmenu_logo_caption_draw(ordering, x, y, progress + 18);
        return;
    }
    frame = gtmenu_local_stack_push(8, 0x800224A8);
    gtmenu_format_integer(frame.address, 0x80054CA0, progress);
    gtmenu_text_color_set(0x185880);
    gtmenu_text_font_set(1);
    gtmenu_text_draw(ordering, x + 6, y + 5, frame.address);
    gtmenu_local_stack_pop(frame);
}

void gtmenu_progress_panel_draw(uint32 ordering)
{
    uint32 page, counter;
    sint32 age;
    FUNCTION_MARKER(0x800225C8, "GTMENU.EXE");
    age = r_s16(0x80058B00);
    page = r_u16(0x800A77C4);
    if (age >= 60) w_u16(0x80058B00, 0);
    counter = r_u16(0x80058B00) + 1;
    w_u16(0x80058B00, counter);
    if (page == 18) {
        if ((sint16)counter < 30) {
            if (gtmenu_license_grade(1)) {
                gtmenu_logo_caption_draw(ordering, 183, 142, 23);
                gtmenu_logo_caption_draw(ordering, 317, 142, 23);
                gtmenu_logo_caption_draw(ordering, 452, 142, 23);
                gtmenu_logo_caption_draw(ordering, 587, 142, 23);
            }
            if (gtmenu_license_grade(2)) {
                gtmenu_logo_caption_draw(ordering, 240, 300, 23);
                gtmenu_logo_caption_draw(ordering, 370, 300, 23);
                gtmenu_logo_caption_draw(ordering, 505, 300, 23);
            }
        }
        gtmenu_progress_badge_draw(ordering, 127, 104, gtmenu_progress_byte(7));
        gtmenu_progress_badge_draw(ordering, 260, 104, gtmenu_progress_byte(8));
        gtmenu_progress_badge_draw(ordering, 395, 104, gtmenu_progress_byte(9));
        gtmenu_progress_badge_draw(ordering, 529, 104, gtmenu_progress_byte(6));
        gtmenu_progress_badge_draw(ordering, 193, 262, gtmenu_progress_byte(16));
        gtmenu_progress_badge_draw(ordering, 328, 262, gtmenu_progress_byte(17));
        gtmenu_progress_badge_draw(ordering, 461, 262, gtmenu_progress_byte(18));
    } else if (page == 19) {
        if (r_s16(0x80058B00) < 30) {
            if (gtmenu_license_grade(2)) gtmenu_logo_caption_draw(ordering, 218, 138, 23);
            if (gtmenu_license_grade(3)) {
                gtmenu_logo_caption_draw(ordering, 374, 138, 23);
                gtmenu_logo_caption_draw(ordering, 530, 138, 23);
            }
            if (gtmenu_license_grade(2)) gtmenu_logo_caption_draw(ordering, 220, 293, 23);
            if (gtmenu_license_grade(3)) {
                gtmenu_logo_caption_draw(ordering, 376, 293, 23);
                gtmenu_logo_caption_draw(ordering, 532, 293, 23);
            }
        }
        gtmenu_progress_badge_draw(ordering, 162, 96, gtmenu_progress_byte(15));
        gtmenu_progress_badge_draw(ordering, 319, 96, gtmenu_progress_byte(19));
        gtmenu_progress_badge_draw(ordering, 473, 96, gtmenu_progress_byte(20));
        gtmenu_progress_badge_draw(ordering, 162, 252, gtmenu_progress_byte(4));
        gtmenu_progress_badge_draw(ordering, 318, 252, gtmenu_progress_byte(5));
        gtmenu_progress_badge_draw(ordering, 474, 252, gtmenu_progress_byte(21));
    } else if (page == 13) {
        if (r_s16(0x80058B00) < 30) {
            if (gtmenu_license_grade(1)) gtmenu_logo_caption_draw(ordering, 402, 111, 23);
            if (gtmenu_license_grade(2)) gtmenu_logo_caption_draw(ordering, 563, 111, 23);
            if (gtmenu_license_grade(2)) gtmenu_logo_caption_draw(ordering, 402, 281, 23);
            if (gtmenu_license_grade(3)) gtmenu_logo_caption_draw(ordering, 563, 281, 23);
        }
        gtmenu_progress_badge_draw(ordering, 350, 63, gtmenu_progress_byte(0));
        gtmenu_progress_badge_draw(ordering, 508, 63, gtmenu_progress_byte(1));
        gtmenu_progress_badge_draw(ordering, 349, 237, gtmenu_progress_byte(2));
        gtmenu_progress_badge_draw(ordering, 508, 237, gtmenu_progress_byte(3));
    }
}

void gtmenu_page_cursor_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 page, group = 0, count = 0, results = 0, complete = 0;
    uint32 index, entry, total = 0, category;
    sint32 grade, rank, y = 58;
    FUNCTION_MARKER(0x80022C40, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(32, 0x80022C40);
    page = r_u16(0x800A77C4);
    if (page == 9 || page == 10 || page == 11) {
        group = page - 9;
        complete = gtmenu_license_grade(group + 1);
        results = gtmenu_license_results(group + 1);
        count = 8;
    }
    for (index = 0; index < count; ++index) {
        grade = r_s8(results + index);
        rank = 3 - grade;
        if (grade) {
            gtmenu_logo_caption_draw(ordering, 138, y, rank + 19);
            entry = 0x8009E3B4 + 28 * rank + 224 * index + 1792 * group;
            gtmenu_time_format(frame.address, r_u32(entry + 36));
            total += r_u32(entry + 36);
        } else gtmenu_time_format(frame.address, 0);
        gtmenu_text_color_set(0x808080);
        gtmenu_text_font_set(0);
        gtmenu_text_draw(ordering, 500, y + 6, frame.address);
        if (r_s8(results + index)) gtmenu_logo_caption_draw(ordering, 138, y, rank + 19);
        y += 38;
    }
    if (count) {
        gtmenu_text_color_set(0x808080);
        gtmenu_text_font_set(0);
        gtmenu_time_format(frame.address + 16, complete ? total : 0);
        gtmenu_text_draw(ordering, 500, y + 6, frame.address + 16);
    }
    if (r_u16(0x800A77C4) != 663) {
        category = r_u16(0x800A77C6);
        if (category == 54) gtmenu_logo_caption_draw(ordering, 110, 412, 0);
        else if (category == 55) gtmenu_logo_caption_draw(ordering, 110, 412, 1);
        else if (category == 56) gtmenu_logo_caption_draw(ordering, 110, 412, 2);
    }
    gtmenu_progress_panel_draw(ordering);
    gtmenu_local_stack_pop(frame);
}

uint32 gtmenu_time_format(uint32 destination, uint32 milliseconds)
{
    uint32 seconds, minutes, hours, fraction;
    FUNCTION_MARKER(0x800347D0, "GTMENU.EXE");
    if (!milliseconds || milliseconds == 0xFFFFFFFF)
        return gtmenu_string_copy(destination, 0x80055C38);
    if (milliseconds > 35999999) milliseconds = 35999999;
    seconds = milliseconds / 1000;
    fraction = milliseconds % 1000;
    minutes = seconds / 60;
    seconds %= 60;
    hours = minutes / 60;
    minutes %= 60;
    if (hours) gtmenu_format_integers4(destination, 0x80055C54, hours, minutes, seconds, fraction / 100);
    else gtmenu_format_integers3(destination, 0x80055C44, minutes, seconds, fraction);
    return destination;
}

uint32 gtmenu_render_reset(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result, ordering_count, primitive_count;
    FUNCTION_MARKER(0x800231AC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x800231AC);
    w_u16(frame.address + 2, 0);
    w_u16(frame.address, 0);
    w_u16(frame.address + 4, 640);
    gtmenu_display_enable(0);
    gtmenu_render_mode_set(14);
    w_u16(frame.address + 6, 480);
    w_u8(0x800A8503, 0);
    w_u8(0x800A84A7, 0);
    gtmenu_renderer_reset();
    gtmenu_gpu_wait(0);
    gtmenu_gpu_clear_rectangle(frame.address, 0, 0, 0);
    gtmenu_gpu_wait(0);
    ordering_count = r_u16(0x800A7584);
    primitive_count = r_u32(0x800A7588);
    gtmenu_render_lists_initialize(ordering_count, primitive_count);
    gtmenu_text_surface_reset();
    gtmenu_fonts_initialize(14);
    gtmenu_rectangle_pulse_reset();
    result = gtmenu_render_auxiliary_reset();
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_text_surface_reset(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 clut_x, clut_y, x, y, result;
    FUNCTION_MARKER(0x8001BCA4, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8001BCA4);
    clut_x = r_u16(0x800A7600);
    w_u16(frame.address + 4, 64);
    clut_y = r_u16(0x800A7602);
    x = r_u16(0x800A75FC);
    y = r_u16(0x800A75FE);
    w_u16(frame.address + 6, 256);
    w_u16(frame.address, x);
    w_u16(frame.address + 2, y);
    result = gtmenu_text_surface_initialize(frame.address, clut_x | (clut_y << 16));
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_text_surface_initialize(uint32 rectangle, uint32 clut_position)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 clut, bounds, x, y, pixels, result;
    sint32 width, height;
    FUNCTION_MARKER(0x80023828, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x80023828);
    clut = frame.address;
    bounds = frame.address + 8;
    width = r_s16(rectangle + 4);
    height = r_s16(rectangle + 6);
    pixels = width * height;
    gtmenu_memory_fill(0x80091244, 0, pixels * 2);
    w_u16(clut, clut_position);
    w_u16(clut + 4, 16);
    w_u16(clut + 2, clut_position >> 16);
    w_u16(clut + 6, 1);
    x = r_u16(rectangle);
    y = r_u16(rectangle + 2);
    w_u16(bounds, x);
    w_u16(bounds + 2, y);
    w_u16(bounds + 4, 64);
    w_u16(bounds + 6, 256);
    gtmenu_text_surface_state_clear(0x8008F214);
    x = r_u16(clut);
    w_u32(0x8008F220, 0x8008F244);
    w_u32(0x8008F218, 0x8008F244);
    w_u32(0x8008F214, 0x80091244);
    y = r_u16(clut + 2);
    w_u16(0x8008F22C, ((sint16)x >> 4) | (y << 6));
    gtmenu_text_surface_bounds_set(0x8008F214, bounds);
    gtmenu_gpu_load_image(clut, 0x800739D4);
    result = gtmenu_gpu_wait(0);
    gtmenu_local_stack_pop(frame);
    return result;
}
