#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdlib.h>
#include "psx_gte.h"

/* Unverified native draft from complete GTMAIN pseudocode and MIPS */
static void boundary(const char *name, uint32 address)
{
    gt_draft_failure(name, address);
    abort();
}

static uint32 call0(uint32 address)
{
    return gt_draft_unresolved_call(address, 0, 0, 0, 0, 0, 0, 0, 0);
}

static uint32 call2(uint32 address, uint32 first, uint32 second)
{
    return gt_draft_unresolved_call(address, first, second, 0, 0, 0, 0, 0, 0);
}

static uint64 sar64(uint64 value, uint32 shift)
{
    return (value >> shift) | ((value >> 63) ? (~(uint64)0 << (64u - shift)) : 0);
}

static uint32 random_native(uint32 *state)
{
    *state = *state * 17u + 17u;
    return *state;
}

static void bytes_zero(uint32 address, uint32 count)
{
    for (uint32 i = 0; i < count; ++i) w_u8(address + i, 0);
}

static uint32 texture_name_native(const uint8 *name)
{
    return (uint32)gtmain_texture_name_find_native(name);
}

static uint32 camera_texture_ids(const uint32 *ids, uint32 ids_count, uint32 packets,
    sint32 count, uint32 vertices, uint32 page_offset, uint32 third_uv, uint32 fourth_uv)
{
    uint32 index;
    for (index = 0u; (sint32)index < count; ++index) {
        uint32 slot = r_u16(packets + 22u), word, second_word, value, first, second;
        uint8 *clut, *page;
        sint32 x, y;
        if (!ids || slot >= ids_count)
            boundary("Native camera texture ID array extent", 0x8001C170u);
        clut = gtmain_texture_clut_native(ids[slot]);
        page = gtmain_texture_page_native(ids[slot]);
        x = gtmain_texture_x_native(ids[slot]);
        y = gtmain_texture_y_native(ids[slot]);
        word = r_u32(packets + page_offset);
        value = xport_load_le16(clut);
        word &= 0xFFE0FFFFu;
        w_u16(packets + 22u, (uint16)value);
        value = xport_load_le16(page);
        first = r_u8(packets + third_uv);
        word |= (value & 31u) << 16u;
        second = r_u8(packets + 20u);
        second_word = word & 0xFE7FFFFFu;
        w_u32(packets + page_offset, word);
        value = xport_load_le16(page);
        w_u8(packets + third_uv, (uint8)(first + (uint32)x));
        w_u8(packets + 20u, (uint8)(second + (uint32)x));
        second = r_u8(packets + 21u);
        w_u32(packets + page_offset, second_word | (((value >> 7u) & 3u) << 23u));
        first = r_u8(packets + page_offset);
        w_u8(packets + 21u, (uint8)(second + (uint32)y));
        second = r_u8(packets + third_uv + 1u);
        w_u8(packets + page_offset, (uint8)(first + (uint32)x));
        w_u8(packets + third_uv + 1u, (uint8)(second + (uint32)y));
        second = r_u8(packets + page_offset + 1u);
        w_u8(packets + page_offset + 1u, (uint8)(second + (uint32)y));
        if (vertices == 4u) {
            first = r_u8(packets + fourth_uv); second = r_u8(packets + fourth_uv + 1u);
            w_u8(packets + fourth_uv, (uint8)(first + (uint32)x));
            w_u8(packets + fourth_uv + 1u, (uint8)(second + (uint32)y));
        }
        packets += vertices == 3u ? 40u : 48u;
    }
    return packets;
}

/* GTMAIN.EXE 8001C170 */
uint32 gtmain_camera_flat_texture_ids(const uint32 *ids, uint32 ids_count,
    uint32 packets, sint32 count, uint32 vertices)
{
    return camera_texture_ids(ids, ids_count, packets, count, vertices, 28u, 36u, 44u);
}

/* GTMAIN.EXE 8001C304 */
uint32 gtmain_camera_gouraud_texture_ids(const uint32 *ids, uint32 ids_count,
    uint32 packets, sint32 count, uint32 vertices)
{
    return camera_texture_ids(ids, ids_count, packets, count, vertices, 32u, 44u, 56u);
}

uint32 gtmain_camera_resource_relocate(uint32 resource)
{
    uint32 vertices = r_u32(resource + 36u);
    uint32 first_count = r_u16(resource + 40u);
    uint32 third_count = r_u16(resource + 44u);
    uint32 start = resource + 56u;
    w_u32(0x8008F220u, resource);
    w_u32(resource + 24u, start);
    uint32 packets = start + vertices * 8u;
    uint32 second_count = r_u16(resource + 42u);
    w_u32(resource + 28u, packets);
    uint32 prefix = first_count * 28u + second_count * 32u + third_count * 36u
        + 44u * r_u16(resource + 46u);
    uint32 total = prefix + 40u * r_u16(resource + 48u) + 48u * r_u16(resource + 50u)
        + 48u * r_u16(resource + 52u) + 60u * r_u16(resource + 54u);
    uint32 next = packets + total;
    w_u32(resource + 32u, next);
    uint32 names = next + total;
    sint32 count = (sint32)r_u32(names);
    uint32 ids[256];
    uint8 name[32];
    names += 4u;
    if (count > 256) boundary("GTMAIN camera texture IDs extent", 0x8001C498u);
    for (sint32 i = 0; i < count; ++i) {
        name[16] = 0;
        for (uint32 j = 0; j < 16u; ++j) name[j] = r_u8(names + j);
        names += 16u;
        ids[i] = texture_name_native(name);
    }
    for (uint32 pass = 0; pass < 2u; ++pass) {
        uint32 packet = prefix + r_u32(resource + 28u + pass * 4u);
        packet = gtmain_camera_flat_texture_ids(ids, count > 0 ? (uint32)count : 0u, packet, r_u16(resource + 48u), 3u);
        packet = gtmain_camera_flat_texture_ids(ids, count > 0 ? (uint32)count : 0u, packet, r_u16(resource + 50u), 4u);
        packet = gtmain_camera_gouraud_texture_ids(ids, count > 0 ? (uint32)count : 0u, packet, r_u16(resource + 52u), 3u);
        gtmain_camera_gouraud_texture_ids(ids, count > 0 ? (uint32)count : 0u, packet, r_u16(resource + 54u), 4u);
    }
    return 0;
}

static void camera_workspace_native(uint8 *workspace, uint32 capacity)
{
    gtmain_texture_workspace_publish(workspace, capacity);
}

uint32 gtmain_camera_resource_load(uint32 car_index)
{
    uint32 previous = r_u32(0x8008F224u);
    if (previous == car_index) return previous;
    w_u32(0x8008F224u, car_index);
    uint32 path = gtmain_camera_resource_path(), sector;
    uint8 descriptor[40];
    GTMainArchiveTable table = gtmain_archive_table_load(path, &sector,
        (uint8 *)psx_addr(0x801BB70Cu, 4096), 0x801BB70Cu, 4096);
    uint32 a = xport_load_le32(table.bytes), b = xport_load_le32(table.bytes + 4u);
    uint32 c = xport_load_le32(table.bytes + 8u), d = xport_load_le32(table.bytes + 12u);
    xport_store_le32(descriptor, a); xport_store_le32(descriptor + 4u, b);
    xport_store_le32(descriptor + 8u, c); xport_store_le32(descriptor + 12u, d);
    const uint8 *row = table.bytes + car_index * 24u;
    a = xport_load_le32(row + 28u); b = xport_load_le32(row + 32u); c = xport_load_le32(row + 36u);
    xport_store_le32(descriptor + 16u, a); xport_store_le32(descriptor + 20u, b); xport_store_le32(descriptor + 24u, c);
    a = xport_load_le32(row + 16u); b = xport_load_le32(row + 20u); c = xport_load_le32(row + 24u);
    xport_store_le32(descriptor + 28u, a); xport_store_le32(descriptor + 32u, b); xport_store_le32(descriptor + 36u, c);
    uint32 image = gtmain_archive_section_load(path, descriptor, sector, 1, 0x801BB70Cu, 0);
    uint32 capacity = (gtmain_texture_workspace_size(image) + 14u) & ~7u;
    uint8 *workspace = (uint8 *)malloc(capacity);
    if (!workspace && capacity) boundary("GTMAIN camera texture workspace allocation", 0x8001C688u);
    camera_workspace_native(workspace, capacity);
    gtmain_texture_records_load(image);
    image = gtmain_archive_section_load(path, descriptor, sector, 0, 0x800CFFDCu, 0);
    uint32 result = gtmain_camera_resource_relocate(image);
    gtmain_texture_workspace_release(workspace);
    free(workspace);
    return result;
}

uint32 gtmain_camera_interval_update(uint32 camera)
{
    uint32 car = r_u32(camera + 180u);
    gtmain_camera_car_select(camera, r_u16(r_u32(camera + 184u) + 8u));
    uint32 record = gtmain_course_interval_find((sint16)r_u16(car + 96u), (sint32)r_u32(car + 1844u));
    if (!record) return gtmain_camera_mode_update(camera);
    uint32 progress = r_u32(car + 1844u), end = r_u32(record + 12u), start = r_u32(record + 8u);
    uint32 length = r_u32(r_u32(0x800BC1A8u));
    if ((sint32)end < (sint32)start) end += length;
    if ((sint32)progress < (sint32)start) progress += length;
    uint32 denominator = end - start, numerator = progress - start;
    w_u32(0x800B3910u, denominator ? (uint32)gtmain_signed_divide64((sint64)(sint32)numerator * 4096,
        (sint32)denominator, 0x8001CC20u) : 0);
    uint32 kind = r_u16(record);
    w_u32(0x800B6400u, record);
    if (kind == 0) return call2(0x8001D360u, camera, 0);
    if (kind == 1) return call2(0x8001D438u, camera, 0);
    if (kind == 2) return call2(0x8001D4ACu, camera, 0);
    if (kind == 3) return call2(0x8001D4DCu, camera, 0);
    return 3;
}

uint32 gtmain_camera_spline_position(uint32 spline, uint8 *position12)
{
    uint32 count = r_u16(spline + 2u), total = 0;
    for (uint32 i = 0; i < count; ++i) total += r_u32(spline + i * 56u + 4u);
    if (!total || r_u16(spline + 2u) < 2u) {
        uint32 x = r_u32(spline + 8u), y = r_u32(spline + 12u), z = r_u32(spline + 16u);
        xport_store_le32(position12, x); xport_store_le32(position12 + 4u, y); xport_store_le32(position12 + 8u, z);
        return x;
    }
    uint32 remaining = (uint32)sar64((uint64)((sint64)(sint32)total * (sint32)r_u32(0x800B3910u)), 12);
    uint32 index = 0, duration = total;
    do {
        duration = r_u32(spline + index * 56u + 4u);
        if ((sint32)remaining < (sint32)duration) break;
        ++index; remaining -= duration;
    } while (index < count);
    sint32 fraction = duration ? (sint32)gtmain_signed_divide64((sint64)(sint32)remaining * 4096,
        (sint32)duration, 0x8001CFA4u) : 0;
    if (fraction < 0) fraction = 0;
    if (fraction >= 4096) fraction = 4096;
    uint32 t = (uint32)fraction << 12;
    uint32 square = (uint32)sar64((uint64)((sint64)(sint32)t * (sint32)t), 24);
    uint32 cube = (uint32)sar64((uint64)((sint64)(sint32)square * (sint32)t), 24);
    uint32 segment = spline + index * 56u + 4u, increment = 0;
    for (uint32 axis = 0; axis < 3u; ++axis) {
        uint32 coefficients = segment + 20u + axis * 12u;
        uint64 sum = sar64((uint64)((sint64)(sint32)cube * (sint32)r_u32(coefficients)), 24);
        sum += sar64((uint64)((sint64)(sint32)square * (sint32)r_u32(coefficients + 4u)), 24);
        sum += sar64((uint64)((sint64)(sint32)t * (sint32)r_u32(coefficients + 8u)), 24);
        increment = (uint32)sar64(sum * r_u32(segment + 16u), 24);
        xport_store_le32(position12 + axis * 4u, r_u32(segment + 4u + axis * 4u) + increment);
    }
    return increment;
}


uint32 gtmain_roster_generate(uint32 guest_list, uint32 count, uint32 mode, uint32 spec)
{
    gtmain_race_config_reset();
    uint32 initial = r_u32(0x800B6640u);
    if (!r_u32(0x800B630Cu)) initial = gtmain_timer_random();
    uint32 seed = initial, selected[6], variants[6];
    if ((sint32)count > 6) boundary("GTMAIN native roster six-element extent TODO", 0x8001F5C8u);
    gtmain_roster_select(selected, guest_list, (sint32)count, &seed, mode, 0u);
    gtmain_roster_variants(variants, selected, (sint32)count, &seed, spec);
    return gtmain_roster_commit(selected, variants, (sint32)count, initial);
}

uint32 gtmain_car_start_delays(uint32 base_time)
{
    uint32 seed = r_u32(0x800B6640u), result = random_native(&seed);
    uint32 span = result % 500u, eligible = 0, index = 0;
    if ((sint16)r_u16(0x800B6666u) <= 0) return result;
    do {
        uint32 car = 0x800B6850u + index * 3380u;
        if (!(r_u32(car + 12u) & 1u)) {
            result = random_native(&seed);
            if (!span) boundary("GTMAIN car start delay BREAK7", 0x8001F774u);
            uint32 delay = base_time + result % span;
            uint32 odd = eligible++ & 1u;
            w_u32(car + 32u, delay);
            if (!odd) {
                result = random_native(&seed);
                base_time += span;
                span = result % 1000u + 250u;
            }
        }
        sint32 count = (sint16)r_u16(0x800B6666u);
        ++index; result = index << 1;
        if ((sint32)index >= count) break;
    } while (1);
    return result;
}

uint32 gtmain_race_settings_snapshot(void)
{
    gtmain_memory_copy(0x80093C08u, 0x800BC160u, 69);
    gtmain_memory_copy(0x80093C50u, 0x8008F374u, 32);
    sint32 preset = gtmain_race_preset_index();
    if (preset >= 0) gtmain_memory_copy(0x80093C50u, 0x8008FC18u + (uint32)preset * 32u, 32);
    if (r_u8(0x800CB7F5u) == 2) {
        w_u8(0x80093C0Eu, r_u8(0x800CBC06u));
        gtmain_race_assist_preset(r_u8(0x800CBC0Eu));
        call2(0x80051F04u, r_u8(0x800CBC07u), 0);
    }
    uint32 toggle = r_u8(0x800CB75Eu) + 1u;
    if (toggle >= 2u) toggle = 0;
    w_u8(0x80093C3Fu, (uint8)toggle); w_u8(0x80093C4Au, (uint8)toggle);
    return 0x80093C3Fu;
}

uint32 gtmain_roster_initialize(void)
{
    uint32 mode;
    sub_8007BD58(0x800B6670u, 0u, 456u);
    gtmain_roster_opponents();
    mode = r_u8(0x800CB7F5u);
    if (mode == 1u) {
        gtmain_roster_player(0); call0(0x8001FAE0u);
    } else if (mode == 2u) {
        gtmain_roster_player(0); gtmain_roster_player(1);
    } else {
        gtmain_roster_shift((sint8)r_u8(0x800B63D0u)); gtmain_roster_player(0);
    }
    uint32 path = gtmain_car_file_environment();
    return gtmain_car_config_bind(path, 0x8008B900u, 0x8008B90Cu, 6, 14);
}

sint32 gtmain_car_grid_place(uint32 car, uint32 grid_index, uint32 course_segment)
{
    uint32 point = gtmain_course_grid_position(grid_index);
    uint32 x = r_u32(point), y = r_u32(point + 4u), z = r_u32(point + 8u);
    w_u32(car + 2228u, x); w_u32(car + 2232u, y); w_u32(car + 2236u, z);
    uint32 angle = (uint32)gtmain_course_start_angle(), model = r_u32(car + 2336u);
    w_u16(car + 2242u, (uint16)angle); w_u16(car + 2240u, 0); w_u16(car + 2244u, 0);
    if (model) {
        sint32 radius = gtmain_model_extent(model);
        uint32 table_index = (r_u16(car + 2242u) & 4095u) * 2u;
        sint64 dx = (sint64)radius * (sint16)r_u16(0x8009423Cu + table_index);
        sint64 dz = (sint64)radius * (sint16)r_u16(0x80094A3Cu + table_index);
        w_u32(car + 2228u, r_u32(car + 2228u) - (uint32)sar64((uint64)dx, 12));
        w_u32(car + 2236u, r_u32(car + 2236u) - (uint32)sar64((uint64)dz, 12));
    }
    sint32 segment = gtmain_course_segment_find(course_segment, (const uint8 *)psx_addr(car + 2228u, 12));
    w_u32(car + 20u, (uint32)segment);
    return segment;
}

uint32 gtmain_race_cars_initialize(void)
{
    uint32 race_mode = r_u32(0x800B630Cu);
    gtmain_race_split_header_clear();
    gtmain_frame_tables_clear(2);
    sint32 frequency = (sint32)r_u32(0x800B6318u);
    if (frequency == 1) { w_u32(0x800BF518u, 60); w_u32(0x800BF514u, 1092); }
    else if (frequency == 0 || frequency == 2) { w_u32(0x800BF518u, 30); w_u32(0x800BF514u, 2184); }
    uint16 multiplier = r_u8(0x8008FBE8u) ? 9u : 0u;
    uint32 tick_rate = r_u32(0x800BF518u);
    w_u16(0x800B63DEu, multiplier);
    w_u16(0x800B6312u, (uint16)(multiplier * tick_rate));
    gtmain_race_settings_snapshot(); gtmain_race_simulation_configure();
    uint32 game_mode = r_u8(0x800CB7F5u);
    w_u16(0x800B63DAu, 65535); w_u16(0x800B63D6u, 0);
    if (game_mode == 4 && r_u8(0x800CB7F8u) == 1 && r_u8(0x800CB7F9u) == 6) {
        w_u16(0x800BC1A6u, 2); w_u32(0x800B66C4u, 0);
    }
    uint32 index = 0, course = (uint32)gtmain_course_start_segment();
    while ((sint32)index < (sint16)r_u16(0x800BC1A6u)) {
        uint32 car = 0x800B6850u + index * 3380u, descriptor = 0x800B6670u + index * 76u;
        bytes_zero(car, 3380);
        uint32 control = 1;
        w_u16(car + 16u, (uint16)index); w_u32(car + 32u, 0xffffffffu);
        w_u32(car + 2336u, r_u32(descriptor + 8u));
        uint32 model = r_u32(car + 2336u);
        w_u16(car + 2340u, r_u16(descriptor + 4u)); w_u16(car + 2342u, r_u16(descriptor + 6u));
        gtmain_model_marker(model, 0x800BB850u + index * 386u);
        gtmain_car_grid_place(car, r_u8(descriptor + 3u), course);
        sint32 body = -2;
        uint32 variant = 0, transmission = 0, rpm = 0, type = 0, subtype = 0, physics_mode = 0;
        if (index == 0) {
            w_u32(car + 28u, 0x800CBC1Cu); gtmain_race_controls_initialize(0x800CBC1Cu, race_mode);
            body = r_u8(0x800BB7ECu); variant = r_u8(0x800BB7EDu); transmission = r_u8(0x800BB7EEu);
            rpm = r_u16(0x800BB7EAu); type = race_mode == 1 ? 5u : 2u; subtype = 1;
        } else if (index == 1) {
            body = r_u8(0x800BB84Cu); variant = r_u8(0x800BB84Du); transmission = r_u8(0x800BB84Eu);
            rpm = r_u16(0x800BB84Au);
            uint32 mode = r_u8(0x800CB7F5u);
            if (mode == 2) {
                w_u32(car + 28u, 0x800CDF70u); gtmain_race_controls_initialize(0x800CDF70u, race_mode);
                type = race_mode == 1 ? 5u : 3u; subtype = 2;
            } else if (mode == 1) {
                w_u32(car + 28u, 0x800CDF70u); gtmain_race_controls_initialize(0x800CDF70u, 1);
                type = 6; subtype = 3; physics_mode = 1;
                if (r_u32(0x800B630Cu) == 1 || r_u32(0x800B6318u) == 1) control = 0;
            } else {
                if (mode == 4) { control = 0; body = -4; } else body = -2;
                variant = transmission = rpm = 0;
            }
        }
        w_u16(car + 24u, (uint16)type); w_u16(car + 26u, (uint16)subtype);
        w_u32(car + 12u, r_u32(car + 12u) | control);
        uint32 body_id = body < 0 ? 0u - (uint32)body : r_u16(0x8008F3A8u + (uint32)body * 2u);
        w_u32(car + 2332u, r_u16(descriptor));
        sint32 z = (sint32)r_u32(car + 2236u) >> 4;
        sint32 x = (sint32)r_u32(car + 2228u) >> 4;
        sint32 sine = (sint16)r_u16(0x8009423Cu + (r_u16(car + 2242u) & 4095u) * 2u);
        sint32 cosine = (sint16)r_u16(0x80094A3Cu + (r_u16(car + 2242u) & 4095u) * 2u);
        gtmain_race_vehicle_initialize(car + 40u, 0x800BB850u + index * 386u, r_u32(car + 20u), x, z,
            sine, cosine, body_id, (uint8)variant, (uint8)physics_mode, transmission);
        model = r_u32(car + 2336u);
        w_u32(car + 2008u, (uint32)sar64((uint64)((sint64)rpm * 600000), 12));
        if (model) {
            uint32 cursor = model + 16u;
            for (uint32 wheel = 0; wheel < 4u; ++wheel) {
                sint32 denominator = (sint16)r_u16(car + ((wheel & 2u) ? 1602u : 1600u));
                sint32 numerator = (sint16)r_u16(car + ((wheel & 2u) ? 1610u : 1608u));
                sint32 offset = (sint16)r_u16(car + ((wheel & 2u) ? 470u : 468u));
                if (!(wheel & 1u)) offset = -offset;
                sint32 scaled = (sint32)((uint32)numerator << 12);
                if (!denominator) boundary("GTMAIN car wheel ratio BREAK7", 0x80020304u);
                if (denominator == -1 && (uint32)scaled == 0x80000000u)
                    boundary("GTMAIN car wheel ratio BREAK6", 0x8002031Cu);
                sint32 quotient = scaled / denominator;
                uint32 base = car + wheel * 24u;
                uint16 height = r_u16(cursor + 4u);
                w_u16(base + 2036u, (uint16)offset); w_u16(base + 2038u, 0);
                w_u16(base + 2044u, (uint16)denominator); w_u16(base + 2040u, height);
                w_u16(base + 2046u, (uint16)quotient); cursor += 8u;
            }
        }
        gtmain_car_collision_positions(car);
        uint32 position_x = r_u32(car + 2228u), y = r_u32(car + 2232u), position_z = r_u32(car + 2236u);
        w_u32(car + 2308u, position_x); w_u32(car + 2312u, y); w_u32(car + 2316u, position_z);
        w_u32(car + 2320u, 0); w_u32(car + 2324u, 0); w_u32(car + 2328u, 0);
        w_u8(car + 2220u, 255); w_u8(car + 2221u, 255);
        w_u16(car + 2344u, (uint16)(100u * r_u8(0x800BB881u + index * 386u)));
        w_u16(car + 2346u, (uint16)(100u * r_u8(0x800BB882u + index * 386u)));
        gtmain_race_meters_prepare(car + 2344u); ++index;
    }
    gtmain_race_vehicle_initialization_finish(); gtmain_race_points_apply();
    sint32 remaining_count = (sint16)r_u16(0x800B6666u);
    while ((sint32)index < remaining_count) {
        uint32 car = 0x800B6850u + index * 3380u;
        bytes_zero(car, 3380); w_u16(car + 16u, (uint16)index);
        uint32 value = r_u16(0x800B6670u + index * 76u);
        remaining_count = (sint16)r_u16(0x800B6666u);
        ++index;
        w_u32(car + 2332u, value);
    }
    return gtmain_race_rank_list_sort();
}

uint32 gtmain_camera_projection_set(uint32 camera, sint32 distance)
{
    uint32 result;
    w_u16(camera + 12u, (uint32)distance);
    gtmain_view_projection_prepare(camera);
    gte_ctc2((uint32)r_s16(camera + 8u) << 16, 24u);
    gte_ctc2((uint32)r_s16(camera + 10u) << 16, 25u);
    result = r_u16(camera + 12u);
    gte_ctc2(result, 26u);
    return result;
}

uint32 gtmain_camera_mode_update(uint32 camera)
{
    sint32 mode = r_s8(camera + 253u);
    if (mode == 0) return gtmain_camera_follow_controls(camera);
    if (mode == 1) return gtmain_camera_countdown_mode(camera);
    if (mode == 2) return gtmain_camera_orbit_update(camera);
    return mode < 2 ? 1u : 2u;
}
