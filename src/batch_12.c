#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "psx_gte.h"

uint32 gtmenu_settings_from_record(uint32 destination, uint32 selectors, uint32 archive)
{
    uint32 table, cursor, type_word, id, section, record, stride, descriptors, result;
    uint32 index, value, other, delta, apply;
    sint32 type, section_index, next_type;
    FUNCTION_MARKER(0x80031854, "GTMENU.EXE");
    table = 0x80071044;
    cursor = selectors;
    type_word = r_u16(table);
    if (!type_word) return 0;
    for (;;) {
        id = r_u16(cursor);
        result = type_word << 16;
        if (id) {
            type = (sint16)type_word;
            if (type == 2) id &= 0xFFF;
            section_index = r_s16(table + 2);
            section = archive + r_u32(archive + (uint32)section_index * 12 + 16);
            stride = r_u32(section + 20);
            record = section + (id - 1) * stride + 24;
            result = (uint32)type << 2;
            apply = 0;
            if ((uint32)type < 13) {
                switch (type) {
                case 0:
                    return 0x80031AE0;
                case 1:
                    apply = 1;
                    break;
                case 2:
                    result = gtmenu_car_config_copy(destination, record);
                    break;
                case 3:
                case 7:
                    delta = r_u8(record + 18);
                    for (index = 0; index < 16; ++index) {
                        value = r_u8(destination + 52 + index);
                        w_u8(destination + 52 + index, value + delta);
                    }
                    value = r_u8(destination + 50);
                    other = r_u8(destination + 49);
                    w_u8(destination + 50, value + delta);
                    w_u8(destination + 49, other + delta);
                    apply = 1;
                    break;
                case 4:
                    value = r_u8(selectors + 64);
                    w_u8(destination + 46, value);
                    result = r_u8(selectors + 77);
                    w_u8(destination + 47, result);
                    break;
                case 5:
                    gtmenu_memory_copy(destination + 26, selectors + 46, 16);
                    apply = 1;
                    break;
                case 6:
                    value = r_u16(record + 20);
                    w_u16(destination + 384, value);
                    apply = 1;
                    break;
                case 8:
                    value = r_u8(selectors + 65);
                    w_u8(destination + 136, value);
                    value = r_u8(selectors + 78);
                    w_u8(destination + 137, value);
                    apply = 1;
                    break;
                case 9:
                    result = r_u8(selectors + 66);
                    w_u8(destination + 111, result);
                    break;
                case 10:
                    result = r_u8(selectors + 79);
                    w_u8(destination + 123, result);
                    break;
                case 11:
                    value = r_u8(selectors + 68); w_u8(destination + 278, value);
                    value = r_u8(selectors + 67); w_u8(destination + 108, value);
                    value = r_u8(selectors + 69); w_u8(destination + 110, value);
                    value = r_u8(selectors + 70); w_u8(destination + 115, value);
                    value = r_u8(selectors + 71); w_u8(destination + 119, value);
                    value = r_u8(selectors + 72); w_u8(destination + 117, value);
                    value = r_u8(selectors + 73); w_u8(destination + 121, value);
                    value = r_u8(selectors + 81); w_u8(destination + 279, value);
                    value = r_u8(selectors + 80); w_u8(destination + 109, value);
                    value = r_u8(selectors + 82); w_u8(destination + 122, value);
                    value = r_u8(selectors + 83); w_u8(destination + 127, value);
                    value = r_u8(selectors + 84); w_u8(destination + 131, value);
                    value = r_u8(selectors + 85); w_u8(destination + 129, value);
                    value = r_u8(selectors + 86); w_u8(destination + 133, value);
                    apply = 1;
                    break;
                case 12:
                    value = r_u8(selectors + 63);
                    w_u8(destination + 45, value);
                    gtmenu_car_config_fields(destination, record, 0x80070FE4, archive);
                    apply = 1;
                    break;
                }
                if (apply) {
                    descriptors = r_u32(table + 4);
                    result = gtmenu_car_config_fields(destination, record, descriptors, archive);
                }
            }
        }
        table += 8;
        next_type = r_s16(table);
        type_word = r_u16(table);
        cursor += 2;
        if (!next_type) return result;
    }
}

sint32 gtmenu_range_index(sint32 minimum, sint32 maximum, sint32 value, sint32 count)
{
    sint32 denominator, numerator, result;
    FUNCTION_MARKER(0x80031B04, "GTMENU.EXE");
    denominator = (uint32)maximum - (uint32)minimum;
    if (!denominator) return 0;
    numerator = ((uint32)value - (uint32)minimum) * ((uint32)count - 1);
    if (denominator == -1 && numerator == (sint32)0x80000000) gt_draft_exception(6);
    result = numerator / denominator;
    if (result < 0) result = 0;
    if (result >= count) result = (uint32)count - 1;
    return result;
}

void gtmenu_part_settings_apply(uint32 record, uint32 category, uint32 data)
{
    uint32 value, minimum, maximum, setting, count;
    sint32 step;
    FUNCTION_MARKER(0x80031B80, "GTMENU.EXE");
    switch (category) {
    case 17:
        value = r_u8(data); w_u8(record + 64, value);
        minimum = r_u8(data + 2);
        maximum = r_u8(data + 3);
        setting = r_u8(data);
        count = r_u8(data + 4);
        step = gtmenu_range_index(minimum, maximum, setting, count);
        w_u8(record + 74, step);
        value = r_u8(data + 1); w_u8(record + 77, value);
        minimum = r_u8(data + 5);
        maximum = r_u8(data + 6);
        setting = r_u8(data + 1);
        count = r_u8(data + 7);
        step = gtmenu_range_index(minimum, maximum, setting, count);
        w_u8(record + 87, step);
        break;
    case 23:
        gtmenu_memory_copy(record + 46, data + 2, 16);
        break;
    case 30:
        value = r_u8(data + 8); w_u8(record + 65, value);
        value = r_u8(data + 9); w_u8(record + 78, value);
        break;
    case 31:
    case 32:
        value = r_u8(data);
        w_u8(record + (category == 31 ? 66 : 79), value);
        minimum = r_u8(data + 2);
        maximum = r_u8(data + 3);
        setting = r_u8(data);
        count = r_u8(data + 1);
        step = gtmenu_range_index(minimum, maximum, setting, count);
        w_u8(record + (category == 31 ? 75 : 88), step);
        break;
    case 33:
        value = r_u8(data + 11); w_u8(record + 68, value);
        value = r_u8(data + 12); w_u8(record + 69, value);
        value = r_u8(data + 10); w_u8(record + 67, value);
        value = r_u8(data + 13); w_u8(record + 70, value);
        value = r_u8(data + 14); w_u8(record + 71, value);
        value = r_u8(data + 15); w_u8(record + 72, value);
        value = r_u8(data + 16); w_u8(record + 73, value);
        minimum = r_u8(data + 30);
        maximum = r_u8(data + 31);
        setting = r_u8(data + 13);
        count = r_u8(data + 38);
        step = gtmenu_range_index(minimum, maximum, setting, count);
        w_u8(record + 76, step);
        value = r_u8(data + 18); w_u8(record + 81, value);
        value = r_u8(data + 19); w_u8(record + 82, value);
        value = r_u8(data + 17); w_u8(record + 80, value);
        value = r_u8(data + 20); w_u8(record + 83, value);
        value = r_u8(data + 21); w_u8(record + 84, value);
        value = r_u8(data + 22); w_u8(record + 85, value);
        value = r_u8(data + 23); w_u8(record + 86, value);
        minimum = r_u8(data + 45);
        maximum = r_u8(data + 46);
        setting = r_u8(data + 20);
        count = r_u8(data + 53);
        step = gtmenu_range_index(minimum, maximum, setting, count);
        w_u8(record + 89, step);
        break;
    case 35:
        value = r_u8(data + 16); w_u8(record + 63, value);
        break;
    default:
        break;
    }
}

uint32 gtmenu_archive_record(uint32 section, uint32 id, uint32 archive)
{
    uint32 base, stride;
    FUNCTION_MARKER(0x80031DA0, "GTMENU.EXE");
    base = archive + r_u32(archive + section * 12 + 16);
    stride = r_u32(base + 20);
    return base + (id - 1) * stride + 24;
}

sint32 gtmenu_archive_record_matches(sint32 value, uint32 section, uint32 id, uint32 archive)
{
    uint32 record, offset;
    sint32 actual;
    FUNCTION_MARKER(0x80031DD8, "GTMENU.EXE");
    record = gtmenu_archive_record(section, id, archive);
    offset = r_u8(0x800710F5 + section);
    actual = r_s16(record + offset);
    return value == actual;
}

sint32 gtmenu_archive_record_flag(uint32 section, uint32 id, uint32 archive)
{
    uint32 record, offset;
    FUNCTION_MARKER(0x80031E38, "GTMENU.EXE");
    record = gtmenu_archive_record(section, id, archive);
    offset = r_u8(0x8007110D + section);
    return r_s8(record + offset);
}

uint32 gtmenu_archive_record_string(uint32 section, uint32 id, uint32 archive)
{
    uint32 record, offset;
    FUNCTION_MARKER(0x80031E74, "GTMENU.EXE");
    record = gtmenu_archive_record(section, id, archive);
    offset = r_u8(0x80071125 + section);
    return record + offset;
}

uint32 gtmenu_draw_area(uint32 ordering, uint32 offset, uint32 first, uint32 last)
{
    uint32 packet, link;
    FUNCTION_MARKER(0x80031F64, "GTMENU.EXE");
    packet = r_u32(0x800A8434);
    w_u32(packet, 0x03000000);
    w_u32(packet + 4, first | 0xE3000000);
    w_u32(packet + 8, last | 0xE4000000);
    w_u32(packet + 12, offset | 0xE5000000);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u32(0x800A8434, packet + 16);
    return packet;
}

uint32 gtmenu_packed_text_index_initialize(uint32 table)
{
    sint32 count, index;
    uint32 cursor, size;
    FUNCTION_MARKER(0x80031FF4, "GTMENU.EXE");
    count = r_s32(table);
    size = (uint32)count * 4 + 4;
    cursor = table + size;
    if (count <= 0) return size;
    for (index = 0; index < count; ++index) {
        w_u32(table + (uint32)index * 4 + 4, cursor);
        size = r_u16(cursor);
        cursor = gtmenu_packed_text_entry(cursor, size);
        if (cursor & 1) ++cursor;
    }
    return (uint32)count * 4;
}

uint32 gtmenu_model_color_name(uint32 id, sint8 code, uint32 archive)
{
    uint32 section, count, record, index, codes, entry, packed, table;
    sint32 text_index;
    sint8 wanted;
    FUNCTION_MARKER(0x80032180, "GTMENU.EXE");
    wanted = code | 0x20;
    section = archive + r_u32(archive + 472);
    count = r_u16(section + 14);
    record = section + 24;
    index = 0;
    if (count) {
        while (index < count) {
            if (id == r_u16(record)) break;
            ++index;
            record += 84;
        }
        count = r_u16(section + 14);
    }
    if (index == count) return 0;
    codes = record + 2;
    entry = record + 20;
    for (index = 0; index < 16; ++index) {
        if (r_s8(codes) == wanted) {
            packed = r_u32(entry);
            table = r_u32(section + 16);
            text_index = (sint32)packed >> 16;
            table = r_u32(table + (uint32)text_index * 4 + 4);
            return gtmenu_packed_text_entry(table, packed & 0xFFFF) + 1;
        }
        ++codes;
        entry += 4;
    }
    return 0;
}

uint32 gtmenu_line_packet(uint32 ordering, uint32 vertices, uint32 color)
{
    uint32 first, second, packet, link;
    FUNCTION_MARKER(0x800325B4, "GTMENU.EXE");
    first = r_u32(vertices);
    second = r_u32(vertices + 4);
    packet = r_u32(0x800A8434);
    w_u32(packet, 0x03000000);
    w_u32(packet + 4, color ^ 0x41000000);
    w_u32(packet + 8, first);
    w_u32(packet + 12, second);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u32(0x800A8434, packet + 16);
    return packet;
}

uint32 gtmenu_texture_state_upload(uint32 state)
{
    uint32 data, position;
    FUNCTION_MARKER(0x8003230C, "GTMENU.EXE");
    gtmenu_gpu_wait(0);
    data = r_u32(state);
    gtmenu_gpu_load_image(state + 16, data);
    position = r_u32(state + 4);
    w_u32(state + 8, 0);
    w_u32(state + 28, 0);
    w_u32(state + 12, position);
    return position;
}

uint32 gtmenu_font_texture_initialize(uint32 state, uint32 texture_page, uint32 packed_pixels)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 x, y, pixels, word, result;
    FUNCTION_MARKER(0x80032678, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(32776, 0x80032678);
    x = (texture_page & 15) << 6;
    y = (texture_page >> 4) << 8;
    w_u16(frame.address, x);
    w_u16(frame.address + 2, y);
    w_u16(frame.address + 4, 64);
    w_u16(frame.address + 6, 256);
    gtmenu_gpu_wait(0);
    pixels = gtmenu_lz_unpack(frame.address + 8, packed_pixels, 32768);
    gtmenu_gpu_load_image(frame.address, pixels);
    gtmenu_memory_fill(state, 0, 32);
    w_u16(state + 8, (x >> 4) | (y << 6));
    word = r_u32(state + 8);
    w_u8(state + 15, 100);
    word = (word & 0xFFE0FFFF) | ((texture_page & 31) << 16);
    word = (word & 0xFF9FFFFF) | 0x00200000;
    word &= 0xFE7FFFFF;
    w_u32(state + 8, word);
    w_u8(state + 12, 0x80);
    w_u8(state + 13, 0x80);
    w_u8(state + 14, 0x80);
    result = gtmenu_gpu_wait(0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_record_indices(uint32 records, sint32 count, uint32 ids, uint32 keys, uint32 id_count, uint32 key_count)
{
    sint32 unique = 0, index, slot;
    uint32 record, palette, id, value;
    FUNCTION_MARKER(0x80036ADC, "GTMENU.EXE");
    for (index = 0; index < count; ++index) {
        record = records + (uint32)index * 76;
        palette = r_u8(record + 2);
        id = r_u16(record);
        for (slot = 0; slot < unique; ++slot) {
            value = r_u32(ids + (uint32)slot * 4);
            if (value == id) break;
        }
        if (slot == unique) {
            w_u32(ids + (uint32)unique * 4, id);
            ++unique;
        }
        w_u16(record + 6, slot);
        w_u32(keys + (uint32)index * 4, (palette << 16) | id);
        w_u16(record + 4, index);
    }
    w_u32(id_count, unique);
    w_u32(key_count, index);
    if (r_s16(0x800A85A4) < unique) return 1;
    return 2 * (r_s16(0x800A85A6) < index);
}

uint32 gtmenu_model_texture_upload(uint32 texture, sint32 page)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80036C24, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80036C24);
    w_u16(frame.address, (page & 15) << 6);
    w_u16(frame.address + 4, 64);
    w_u16(frame.address + 2, (uint32)(page >> 4) << 8);
    w_u16(frame.address + 6, 256);
    gtmenu_gpu_wait(0);
    gtmenu_gpu_load_image(frame.address, texture + 96);
    result = gtmenu_gpu_wait(0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_palette_upload(uint32 texture, sint32 clut, uint32 palette)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80036C88, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80036C88);
    w_u16(frame.address, (clut & 63) << 4);
    w_u16(frame.address + 4, 64);
    w_u16(frame.address + 2, clut >> 6);
    w_u16(frame.address + 6, 4);
    gtmenu_gpu_wait(0);
    gtmenu_gpu_load_image(frame.address, texture + (palette << 9) + 0x8060);
    result = gtmenu_gpu_wait(0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_groups_load(uint32 records, sint32 count, uint32 geometry_buffers, uint32 texture_workspace)
{
    GT_LOCAL_STACK_FRAME lists, sorted;
    uint64 span;
    uint32 array_bytes, ids, keys, cluts, pages, status, record, texture, geometry;
    uint32 group_id, resource, offset, old_index, color, palette, output, input;
    uint32 chunk, source_offset, target_offset, a, b, c, d;
    sint32 groups, group, index, position, candidate, previous, page;
    FUNCTION_MARKER(0x80032A24, "GTMENU.EXE");
    span = (uint64)(count > 0 ? count : 0) * 8 + 8;
    if (span > 0xFFFFFFFF) gt_draft_failure("GTMENU model index stack span", 0x80032A24);
    lists = gtmenu_local_stack_push(span, 0x80032A24);
    array_bytes = count > 0 ? (uint32)count * 4 : 0;
    ids = lists.address + 8;
    keys = ids + array_bytes;
    cluts = r_u32(0x800A85B8);
    pages = r_u32(0x800A85A8);
    status = gtmenu_model_record_indices(records, count, ids, keys, lists.address, lists.address + 4);
    if (status) {
        gtmenu_local_stack_pop(lists);
        return status;
    }
    groups = r_s32(lists.address);
    sorted = gtmenu_local_stack_push(groups > 0 ? (uint32)groups * 4 : 0, 0x80032A24);
    for (group = 0; group < groups; ++group) {
        candidate = r_s32(ids + (uint32)group * 4);
        for (position = 0; position < group; ++position) {
            previous = r_s32(sorted.address + (uint32)position * 4);
            if (candidate < previous) {
                w_u32(sorted.address + (uint32)position * 4, candidate);
                candidate = previous;
            }
        }
        w_u32(sorted.address + (uint32)group * 4, candidate);
    }
    group = 0;
    while (group < r_s32(lists.address)) {
        page = -1;
        group_id = r_u32(sorted.address + (uint32)group * 4);
        offset = r_u32(0x800711BC);
        resource = group_id + offset;
        texture = gtmenu_model_resource_load(resource * 2, texture_workspace, 0);
        for (index = 0; index < count; ++index) {
            record = records + (uint32)index * 76;
            if (group_id != r_u16(record)) continue;
            if (page < 0) {
                old_index = r_u16(record + 6);
                page = r_u16(pages + old_index * 2);
                gtmenu_model_texture_upload(texture, page);
            }
            w_u16(record + 6, page);
            for (chunk = 0; chunk < 4; ++chunk) {
                source_offset = chunk < 2 ? 64 + chunk * 16 : 32 + (chunk - 2) * 16;
                target_offset = chunk < 2 ? 44 + chunk * 16 : 12 + (chunk - 2) * 16;
                input = texture + source_offset;
                output = record + target_offset;
                a = r_u32(input);
                b = r_u32(input + 4);
                c = r_u32(input + 8);
                d = r_u32(input + 12);
                w_u32(output, a);
                w_u32(output + 4, b);
                w_u32(output + 8, c);
                w_u32(output + 12, d);
            }
            old_index = r_u16(record + 4);
            color = r_u16(cluts + old_index * 2);
            palette = r_u8(record + 2);
            gtmenu_model_palette_upload(texture, color, palette);
            w_u16(record + 4, color);
        }
        if (!texture_workspace) gtmenu_heap_free(texture);
        output = geometry_buffers ? r_u32(geometry_buffers + (uint32)group * 4) : 0;
        geometry = gtmenu_model_resource_load(resource * 2 + 1, output, 0);
        for (index = 0; index < count; ++index) {
            record = records + (uint32)index * 76;
            if (group_id == r_u16(record)) w_u32(record + 8, geometry);
        }
        gtmenu_model_geometry_bind(geometry, page);
        ++group;
    }
    gtmenu_local_stack_pop(sorted);
    gtmenu_local_stack_pop(lists);
    return status;
}

uint32 gtmenu_ordering_slot_clear(uint32 slot)
{
    uint32 table, count, result, node, value;
    FUNCTION_MARKER(0x800369F0, "GTMENU.EXE");
    table = r_u32(0x800A8594 + slot * 4);
    count = r_u32(0x800A85A0);
    result = gtmenu_gpu_ordering_clear(table, count);
    node = r_u32(0x800A8590);
    if (node) {
        do {
            value = r_u32(node + slot * 4 + 16);
            w_u32(node + 24, value);
            node = r_u32(node + 4);
            result = slot * 4 + node;
        } while (node);
    }
    return result;
}

uint32 gtmenu_display_buffers_clear(sint32 mode)
{
    uint32 slot;
    FUNCTION_MARKER(0x800327DC, "GTMENU.EXE");
    switch (mode) {
    case 0:
        slot = r_u32(0x800A8548);
        return gtmenu_ordering_slot_clear(slot);
    case 1:
        slot = r_u32(0x800A854C);
        return gtmenu_ordering_slot_clear(slot);
    case 2:
        gtmenu_ordering_slot_clear(0);
        gtmenu_ordering_slot_clear(1);
        return 0;
    default:
        return mode < 2 ? 1 : 2;
    }
}

sint32 gtmenu_car_count(void)
{
    FUNCTION_MARKER(0x80036880, "GTMENU.EXE");
    return r_s16(0x800A885E);
}

uint32 gtmenu_car_name(uint32 index)
{
    uint32 names;
    FUNCTION_MARKER(0x80036890, "GTMENU.EXE");
    names = r_u32(0x800A8874);
    return names + index * 6 + 32;
}

void gtmenu_model_resource_base_set(uint32 mode)
{
    sint32 count;
    FUNCTION_MARKER(0x800329EC, "GTMENU.EXE");
    if (mode) {
        count = gtmenu_car_count();
        w_u32(0x800711BC, count);
    } else {
        w_u32(0x800711BC, 0);
    }
}

sint32 gtmenu_angle_asin(sint16 value)
{
    uint32 index, angle;
    FUNCTION_MARKER(0x8003EF48, "GTMENU.EXE");
    if (value < -4095) return -1024;
    if (value >= 4096) return 1024;
    if (value >= 0) return r_s16(0x800774D8 + (uint32)value * 2);
    index = -value;
    angle = r_u16(0x800774D8 + index * 2);
    return (sint16)(0 - angle);
}

sint32 gtmenu_angle_atan(sint16 x, sint16 y)
{
    uint32 magnitude_x, magnitude_y, ratio, quadrant = 0;
    sint32 angle, sign, offset;
    FUNCTION_MARKER(0x8003EFCC, "GTMENU.EXE");
    if (!x && !y) return 0;
    magnitude_y = y < 0 ? -y : y;
    magnitude_x = x < 0 ? -x : x;
    if (magnitude_y < magnitude_x) ratio = (magnitude_y << 12) / magnitude_x;
    else ratio = (magnitude_x << 12) / magnitude_y;
    if (x < 0) quadrant = 4;
    if (y < 0) quadrant |= 2;
    if (magnitude_y < magnitude_x) quadrant |= 1;
    offset = r_s16(0x80077464 + quadrant * 2);
    sign = r_s8(0x8007745C + quadrant);
    angle = r_s16(0x800794DC + ratio * 2);
    return (sint16)((angle ^ sign) + offset);
}

sint32 gtmenu_matrix_angles(uint32 output, uint32 matrix)
{
    sint32 first, second, third = 0, magnitude, branch, result;
    uint32 a, b;
    FUNCTION_MARKER(0x80037134, "GTMENU.EXE");
    result = gtmenu_angle_asin(r_s16(matrix + 10));
    w_u16(output, result);
    magnitude = r_s16(matrix + 14);
    if (magnitude < 0) magnitude = -magnitude;
    branch = magnitude < 4096 ? r_s16(0x80075458 + (uint32)magnitude * 2) : 0;
    if (!branch) {
        a = r_u16(matrix + 2);
        b = r_u16(matrix + 14);
        first = (sint16)(0 - a);
        second = (sint16)(0 - b);
    } else {
        first = r_s16(matrix + 6);
        second = r_s16(matrix + 8);
        third = gtmenu_angle_atan(first, second);
        first = r_s16(matrix + 4);
        second = r_s16(matrix + 16);
    }
    result = gtmenu_angle_atan(first, second);
    w_u16(output + 2, result);
    w_u16(output + 4, third);
    return result;
}

uint32 gtmenu_matrix_transpose(uint32 output, uint32 matrix)
{
    uint32 value, other, index, source_index;
    FUNCTION_MARKER(0x80037204, "GTMENU.EXE");
    if (output != matrix) {
        value = 0;
        for (index = 0; index < 9; ++index) {
            source_index = (index % 3) * 3 + index / 3;
            value = r_u16(matrix + source_index * 2);
            w_u16(output + index * 2, value);
        }
    } else {
        /* The original alias branch retains each element in place */
        other = r_u16(matrix + 2); value = r_u16(matrix + 6);
        w_u16(output + 2, other); w_u16(output + 6, value);
        other = r_u16(matrix + 4); value = r_u16(matrix + 12);
        w_u16(output + 4, other); w_u16(output + 12, value);
        other = r_u16(matrix + 10); value = r_u16(matrix + 14);
        w_u16(output + 10, other); w_u16(output + 14, value);
    }
    return value;
}

sint32 gtmenu_matrix_multiply(uint32 output, uint32 left, uint32 right)
{
    uint32 a, b, word0, word1, word2, word3, word4, xy;
    uint32 first_x, first_y, first_z, second_x, second_y, second_z, third_x, third_y, third_z;
    FUNCTION_MARKER(0x800372B0, "GTMENU.EXE");
    a = r_u32(left);
    b = r_u32(left + 4);
    gte_ctc2(a, 0);
    a = r_u32(left + 8);
    gte_ctc2(b, 1);
    b = r_u32(left + 12);
    gte_ctc2(a, 2);
    a = r_u32(left + 16);
    gte_ctc2(b, 3);
    gte_ctc2(a, 4);
    word0 = r_u32(right);
    word3 = r_u32(right + 12);
    word1 = r_u32(right + 4);
    xy = (word0 & 0xFFFF) | (word1 & 0xFFFF0000);
    gte_lwc2(0, xy);
    gte_lwc2(1, word3);
    xport_gte_execute(0x486012);
    word2 = r_u32(right + 8);
    xy = (word0 >> 16) | (word2 << 16);
    first_x = xport_gte_read_data(9);
    first_y = xport_gte_read_data(10);
    first_z = xport_gte_read_data(11);
    gte_lwc2(0, xy);
    gte_lwc2(1, word3 >> 16);
    xport_gte_execute(0x486012);
    xy = (word1 & 0xFFFF) | (word2 & 0xFFFF0000);
    word4 = r_u32(right + 16);
    second_x = xport_gte_read_data(9);
    second_y = xport_gte_read_data(10);
    second_z = xport_gte_read_data(11);
    gte_lwc2(0, xy);
    gte_lwc2(1, word4);
    xport_gte_execute(0x486012);
    w_u16(output, first_x);
    w_u16(output + 6, first_y);
    w_u16(output + 12, first_z);
    w_u16(output + 2, second_x);
    w_u16(output + 8, second_y);
    w_u16(output + 14, second_z);
    third_x = xport_gte_read_data(9);
    third_y = xport_gte_read_data(10);
    third_z = xport_gte_read_data(11);
    w_u16(output + 4, third_x);
    w_u16(output + 10, third_y);
    w_u16(output + 16, third_z);
    return third_x;
}

sint32 gtmenu_preview_view_finalize(uint32 view, uint32 basis)
{
    sint32 result;
    FUNCTION_MARKER(0x80033178, "GTMENU.EXE");
    gtmenu_matrix_angles(view + 132, view + 140);
    result = gtmenu_matrix_transpose(view + 160, view + 140);
    if (basis) result = gtmenu_matrix_multiply(view + 160, basis, view + 160);
    return result;
}

uint32 gtmenu_model_sections_relocate(uint32 model)
{
    uint32 cursor, first, second, index, count, stride;
    FUNCTION_MARKER(0x80036D00, "GTMENU.EXE");
    cursor = model + 80;
    first = r_u16(model + 40);
    second = r_u16(model + 42);
    w_u32(model, cursor);
    cursor += first * 8;
    w_u32(model + 4, cursor);
    cursor += second * 8;
    for (index = 0; index < 8; ++index) {
        w_u32(model + index * 4 + 8, cursor);
        count = r_u16(model + index * 2 + 44);
        stride = r_u16(0x80055CB0 + index * 2);
        cursor += count * stride;
    }
    return cursor;
}

uint32 gtmenu_model_texture_bits(uint32 model, uint32 page)
{
    uint32 index, bits, entry, count, stride, item, value;
    FUNCTION_MARKER(0x80036D78, "GTMENU.EXE");
    bits = (page & 31) << 16;
    for (index = 0; index < 8; ++index) {
        if (index & 4) {
            entry = r_u32(model + index * 4 + 8);
            count = r_u16(model + index * 2 + 44);
            stride = r_u16(0x80055CB0 + index * 2);
            for (item = 0; item < count; ++item) {
                value = r_u32(entry + 20);
                w_u32(entry + 20, (value & 0xFFE0FFFF) | bits);
                entry += stride;
            }
        }
    }
    return 0;
}

uint32 gtmenu_model_geometry_bind(uint32 geometry, sint32 page)
{
    uint32 count, cursor, index, model, result;
    FUNCTION_MARKER(0x80036E04, "GTMENU.EXE");
    count = r_u16(geometry + 60);
    cursor = geometry + count * 8 + 64;
    for (index = 0; index < count; ++index) {
        w_u32(geometry + index * 8 + 68, cursor);
        cursor = gtmenu_model_sections_relocate(cursor);
    }
    gtmain_model_distances_square(geometry + 60);
    w_u32(geometry + 56, cursor);
    result = gtmain_model_materials_relocate(cursor);
    for (index = 0; index < count; ++index) {
        model = r_u32(geometry + index * 8 + 68);
        gtmenu_model_texture_bits(model, page);
        result = (index + 1) * 8;
    }
    return result;
}

sint32 gtmenu_model_range_index(uint32 variants, uint32 vector, sint32 extent)
{
    sint32 denominator, quotient, threshold, distance;
    sint64 product;
    uint32 x, y, z, count, index, sum;
    FUNCTION_MARKER(0x80037438, "GTMENU.EXE");
    denominator = extent >> 4;
    quotient = denominator ? 0x01000000 / denominator : -1;
    x = r_u32(vector);
    y = r_u32(vector + 4);
    z = r_u32(vector + 8);
    xport_gte_write_data(9, (sint32)x >> 14);
    xport_gte_write_data(10, (sint32)y >> 14);
    xport_gte_write_data(11, (sint32)z >> 14);
    xport_gte_execute(0xA00428);
    count = r_u16(variants);
    x = xport_gte_read_data(25);
    y = xport_gte_read_data(26);
    z = xport_gte_read_data(27);
    sum = x + y + z;
    product = (sint64)(sint32)sum * quotient;
    distance = product >> 12;
    for (index = 0; index < count; ++index) {
        threshold = r_s32(variants + index * 8 + 4);
        if (distance < threshold) return index;
    }
    return -1;
}

uint32 gtmenu_matrix_bind_packed_negation(uint32 matrix)
{
    uint32 middle, first, zero, third, fourth, negated, result;
    FUNCTION_MARKER(0x800374F0, "GTMENU.EXE");
    middle = r_u32(matrix + 8);
    first = r_u32(matrix + 4);
    zero = r_u32(matrix);
    third = r_u32(matrix + 12);
    fourth = r_u32(matrix + 16);
    negated = 0 - middle;
    result = negated & 0xFFFF;
    first = (first ^ 0xFFFF0000) + 0x10000;
    if (result) negated += 0x10000;
    gte_ctc2(zero, 0);
    gte_ctc2(first, 1);
    gte_ctc2(negated, 2);
    gte_ctc2(third, 3);
    gte_ctc2(fourth, 4);
    return result;
}

void gtmenu_matrix_shift(uint32 output, uint32 matrix, sint32 shift)
{
    uint32 values[9], index, amount;
    FUNCTION_MARKER(0x80037600, "GTMENU.EXE");
    if (!shift && output == matrix) return;
    for (index = 0; index < 9; ++index) values[index] = r_s16(matrix + index * 2);
    amount = (shift < 0 ? 0 - (uint32)shift : (uint32)shift) & 31;
    for (index = 0; index < 9; ++index) {
        if (shift < 0) values[index] <<= amount;
        else values[index] = (sint32)values[index] >> amount;
        w_u16(output + index * 2, values[index]);
    }
}

uint32 gtmenu_matrix_negate_axes(uint32 output, uint32 matrix)
{
    uint32 first, third, fourth, middle, second, negated, result;
    FUNCTION_MARKER(0x80037908, "GTMENU.EXE");
    if (output != matrix) {
        first = r_u32(matrix);
        third = r_u32(matrix + 12);
        w_u32(output, first);
        fourth = r_u32(matrix + 16);
        w_u32(output + 12, third);
        w_u32(output + 16, fourth);
    }
    middle = r_u32(matrix + 8);
    second = r_u32(matrix + 4);
    negated = 0 - middle;
    if (negated & 0xFFFF) negated += 0x10000;
    result = (second ^ 0xFFFF0000) + 0x10000;
    w_u32(output + 8, negated);
    w_u32(output + 4, result);
    return result;
}

sint32 gtmenu_preview_geometry_transform(uint32 context, uint32 geometry, uint32 mode)
{
    uint32 x, y, z, cx, cy, cz, ax, ay, az, swap, distance, root, extent, model;
    uint32 selected, bits, a, b, low_z, high_z, index, point, rgb, cursor;
    sint32 exponent, order, difference, shift, denominator, numerator, ratio, factor, intensity;
    sint64 product;
    FUNCTION_MARKER(0x80033298, "GTMENU.EXE");
    x = r_u32(context + 864);
    cx = r_u32(context + 44);
    y = r_u32(context + 868);
    z = r_u32(context + 872);
    cy = r_u32(context + 48);
    cz = r_u32(context + 52);
    x -= cx; y -= cy; z -= cz;
    w_u32(context + 268, x);
    w_u32(context + 272, y);
    w_u32(context + 276, z);
    ax = (sint32)x < 0 ? 0 - x : x;
    ay = (sint32)y < 0 ? 0 - y : y;
    az = (sint32)z < 0 ? 0 - z : z;
    if ((sint32)ax < (sint32)ay) { swap = ax; ax = ay; ay = swap; }
    if ((sint32)ax < (sint32)az) { swap = ax; ax = az; az = swap; }
    if ((sint32)ay < (sint32)az) { swap = ay; ay = az; az = swap; }
    distance = ax + ((sint32)ay >> 1) + ((sint32)az >> 2);
    root = r_u32(context);
    extent = r_u32(root + 28) << 4;
    selected = gtmenu_model_range_index(geometry, context + 268, extent);
    if (selected == 0xFFFFFFFF) return -1;
    if (mode) selected = 2 * (mode != 1);
    model = r_u32(geometry + selected * 8 + 8);
    ax = (sint32)x < 0 ? 0 - x : x;
    ay = (sint32)y < 0 ? 0 - y : y;
    az = (sint32)z < 0 ? 0 - z : z;
    xport_gte_write_data(30, ax | ay | az);
    exponent = 31 - xport_gte_read_data(31);
    order = r_s16(model + 76) + 1;
    if (exponent < order) exponent = order;
    difference = exponent - order;
    shift = exponent - 13;
    if (shift < 0) shift = 0;
    x = (sint32)x >> (shift & 31);
    y = (sint32)y >> (shift & 31);
    z = (sint32)z >> (shift & 31);
    w_u16(context + 40, shift);
    w_u16(context + 42, difference);
    w_u16(context + 280, x);
    w_u16(context + 282, y);
    w_u16(context + 284, z);
    gtmenu_matrix_bind_packed_negation(context + 64);
    xport_gte_write_data(0, r_u32(context + 280));
    xport_gte_write_data(1, r_u32(context + 284));
    xport_gte_execute(0x486012);
    w_u32(context + 84, xport_gte_read_data(9));
    w_u32(context + 88, xport_gte_read_data(10));
    w_u32(context + 92, xport_gte_read_data(11));
    a = r_u32(context + 84); b = r_u32(context + 88);
    gte_ctc2(a, 5);
    a = r_u32(context + 92);
    gte_ctc2(b, 6); gte_ctc2(a, 7);
    gtmenu_matrix_multiply(context + 948, context + 64, context + 884);
    gtmenu_matrix_scale_gte(context + 96, context + 948, 4096);
    gtmenu_matrix_shift(context + 96, context + 96, difference);
    gtmenu_model_matrix_bind(context, context + 96);
    gtmenu_lighting_bind(context);
    a = r_u32(model + 60); b = r_u32(model + 68);
    low_z = r_s16(model + 64); high_z = r_s16(model + 72);
    for (index = 0; index < 8; ++index) {
        point = context + 184 + index * 8;
        w_u16(point, index & 1 ? b : a);
        w_u16(point + 2, index & 2 ? b >> 16 : a >> 16);
        w_u16(point + 4, index & 4 ? high_z : low_z);
    }
    bits = gtmenu_model_bounds_classify(context, context + 184, 8);
    if (bits & 31) return -1;
    gtmenu_matrix_transpose(context + 248, context + 884);
    gtmenu_matrix_scale_gte(context + 248, context + 248, (uint16)-4096);
    gtmenu_matrix_rotate_short(context + 56, context + 248, context + 280);
    if (r_u32(context + 912)) {
        product = (sint64)((sint32)extent >> 8) * 2560;
        denominator = product >> 8;
        numerator = (uint32)((sint32)distance >> 8) << 12;
        if (!denominator) gt_draft_exception(7);
        if (denominator == -1 && numerator == (sint32)0x80000000) gt_draft_exception(6);
        ratio = numerator / denominator;
        factor = 4096 - r_s16(context + 924);
        if (factor < 0) factor = 0;
        intensity = (sint32)((uint32)ratio * 80) >> 12;
        intensity = (uint32)(80 - intensity) * (uint32)factor;
        intensity >>= 12;
        if (intensity < 0) intensity = 0;
        else if (intensity >= 64) intensity = 64;
        rgb = (uint32)intensity | (uint32)intensity << 8 | (uint32)intensity << 16;
        w_u32(context + 968, rgb | 0x26000000);
        w_u32(context + 972, rgb | 0x2E000000);
        gtmenu_matrix_shift(context + 948, context + 948, r_s16(context + 910));
        if (!r_s16(context + 908)) gtmenu_matrix_negate_axes(context + 948, context + 948);
        a = r_s16(context + 920); b = r_s16(context + 922);
        gte_ctc2(a, 13); gte_ctc2(b, 14);
        a = r_u32(context + 948); b = r_u32(context + 952);
        gte_ctc2(a, 8);
        a = r_u32(context + 956); gte_ctc2(b, 9);
        b = r_u32(context + 960); gte_ctc2(a, 10);
        a = r_u32(context + 964); gte_ctc2(b, 11); gte_ctc2(a, 12);
    }
    a = r_u32(context + 96); b = r_u32(context + 100);
    gte_ctc2(a, 0);
    a = r_u32(context + 104); gte_ctc2(b, 1);
    b = r_u32(context + 108); gte_ctc2(a, 2);
    a = r_u32(context + 112); gte_ctc2(b, 3); gte_ctc2(a, 4);
    w_u32(context + 936, model);
    a = r_u32(model);
    cursor = r_u32(0x800A8434);
    w_u32(context + 940, a);
    a = r_u32(model + 4);
    w_u32(context + 944, a);
    w_u32(context + 932, cursor);
    if ((bits >> 16) & 63) gtmenu_model_primitives_clipped_draw(model, context);
    else gtmenu_model_primitives_draw(model, context);
    cursor = r_u32(context + 932);
    w_u32(0x800A8434, cursor);
    return selected;
}

sint32 gtmenu_matrix_scale_gte(uint32 output, uint32 matrix, uint16 scale)
{
    uint32 a, b, x, y, z, result;
    FUNCTION_MARKER(0x80037540, "GTMENU.EXE");
    a = r_u32(matrix); b = r_u32(matrix + 4);
    gte_ctc2(a, 0);
    a = r_u32(matrix + 8); gte_ctc2(b, 1);
    b = r_u32(matrix + 12); gte_ctc2(a, 2);
    a = r_u32(matrix + 16); gte_ctc2(b, 3); gte_ctc2(a, 4);
    gte_lwc2(0, scale); gte_lwc2(1, 0);
    xport_gte_execute(0x486012);
    gte_lwc2(4, 0); gte_lwc2(5, scale);
    gte_lwc2(2, (uint32)scale << 16); gte_lwc2(3, 0);
    x = xport_gte_read_data(9); y = xport_gte_read_data(10); z = xport_gte_read_data(11);
    xport_gte_execute(0x48E012);
    w_u16(output, x); w_u16(output + 6, y); w_u16(output + 12, z);
    x = xport_gte_read_data(9); y = xport_gte_read_data(10); z = xport_gte_read_data(11);
    xport_gte_execute(0x496012);
    w_u16(output + 2, x); w_u16(output + 8, y); w_u16(output + 14, z);
    result = xport_gte_read_data(9); y = xport_gte_read_data(10); z = xport_gte_read_data(11);
    w_u16(output + 4, result); w_u16(output + 10, y); w_u16(output + 16, z);
    return result;
}

uint32 gtmenu_model_matrix_bind(uint32 context, uint32 matrix)
{
    uint32 a, b;
    FUNCTION_MARKER(0x800376B8, "GTMENU.EXE");
    gtmenu_matrix_bind_packed_negation(matrix);
    a = xport_gte_read_control(0); b = xport_gte_read_control(1);
    w_u32(context + 96, a);
    a = xport_gte_read_control(2); w_u32(context + 100, b);
    b = xport_gte_read_control(3); w_u32(context + 104, a);
    a = xport_gte_read_control(4); w_u32(context + 108, b); w_u32(context + 112, a);
    return a;
}

uint32 gtmenu_lighting_bind(uint32 context)
{
    uint32 a, b;
    FUNCTION_MARKER(0x80037708, "GTMENU.EXE");
    a = r_u32(context + 116); b = r_u32(context + 120);
    gte_ctc2(a, 8);
    a = r_u32(context + 124); gte_ctc2(b, 9);
    b = r_u32(context + 128); gte_ctc2(a, 10);
    a = r_u32(context + 132); gte_ctc2(b, 11); gte_ctc2(a, 12);
    a = r_u32(context + 136); b = r_u32(context + 140);
    gte_ctc2(a, 16);
    a = r_u32(context + 144); gte_ctc2(b, 17);
    b = r_u32(context + 148); gte_ctc2(a, 18);
    a = r_u32(context + 152); gte_ctc2(b, 19); gte_ctc2(a, 20);
    return a;
}

sint32 gtmenu_matrix_rotate_short(uint32 output, uint32 matrix, uint32 vector)
{
    uint32 a, b, result;
    FUNCTION_MARKER(0x800378B8, "GTMENU.EXE");
    a = r_u32(matrix); b = r_u32(matrix + 4);
    gte_ctc2(a, 0);
    a = r_u32(matrix + 8); gte_ctc2(b, 1);
    b = r_u32(matrix + 12); gte_ctc2(a, 2);
    a = r_u32(matrix + 16); gte_ctc2(b, 3); gte_ctc2(a, 4);
    xport_gte_write_data(0, r_u32(vector));
    xport_gte_write_data(1, r_u32(vector + 4));
    xport_gte_execute(0x486012);
    w_u32(output, xport_gte_read_data(9));
    result = xport_gte_read_data(10);
    w_u32(output + 4, xport_gte_read_data(11));
    w_u16(output + 2, result);
    return result;
}

uint32 gtmenu_model_bounds_classify(uint32 context, uint32 vertices, uint32 count)
{
    uint32 either = 0, every = 0xFFFFFFFF, screen_every = 0xFFFFFFFF;
    uint32 threshold, next_xy, next_z, flags, x, y, z, screen, clip_x, clip_y, bits, screen_bits;
    sint32 width, height, screen_x, screen_y;
    FUNCTION_MARKER(0x80037760, "GTMENU.EXE");
    xport_gte_write_data(0, r_u32(vertices));
    xport_gte_write_data(1, r_u32(vertices + 4));
    xport_gte_execute(0x180001);
    threshold = r_u16(context + 8);
    width = r_s16(context + 4);
    height = r_s16(context + 6);
    while (count) {
        next_xy = r_u32(vertices + 8);
        next_z = r_u32(vertices + 12);
        flags = xport_gte_read_control(31);
        x = xport_gte_read_data(9); y = xport_gte_read_data(10); z = xport_gte_read_data(11);
        screen = xport_gte_read_data(14);
        --count;
        vertices += 8;
        xport_gte_execute(0x4BE012);
        xport_gte_write_data(2, (x & 0xFFFF) | (y << 16));
        xport_gte_write_data(3, z);
        xport_gte_write_data(0, next_xy); xport_gte_write_data(1, next_z);
        clip_x = xport_gte_read_data(9); clip_y = xport_gte_read_data(10);
        bits = (z - threshold) >> 31;
        xport_gte_execute(0x4CE012);
        if ((sint32)clip_x < 0) bits |= 2;
        if ((sint32)clip_y < 0) bits |= 4;
        if (flags & 0x80000000) bits |= 32;
        clip_x = xport_gte_read_data(9); clip_y = xport_gte_read_data(10);
        if ((sint32)clip_x < 0) bits |= 8;
        xport_gte_execute(0x180001);
        if ((sint32)clip_y < 0) bits |= 16;
        screen_bits = 0;
        screen_x = (sint16)screen;
        screen_y = (sint32)screen >> 16;
        if (screen_x < 0) screen_bits = 2;
        if (screen_x >= width) screen_bits |= 4;
        if (screen_y < 0) screen_bits |= 8;
        if (screen_y >= height) screen_bits |= 16;
        if (flags & 0x80000000) screen_bits = 0;
        either |= bits;
        every &= bits;
        screen_every &= screen_bits;
    }
    return every | screen_every | (either << 16);
}

sint32 gtmenu_preview_wheel_matrix_scale(uint32 context, sint32 scale)
{
    uint32 x, y, z;
    FUNCTION_MARKER(0x8003DBCC, "GTMENU.EXE");
    x = r_s16(context + 964);
    y = r_s16(context + 970);
    z = r_s16(context + 976);
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x198003D);
    x = xport_gte_read_data(9);
    y = xport_gte_read_data(10);
    z = xport_gte_read_data(11);
    w_u16(context + 970, y);
    w_u16(context + 964, x);
    w_u16(context + 976, z);
    return gtmenu_matrix_multiply(context + 960, context + 884, context + 960);
}

uint32 gtmenu_preview_wheel_position_set(uint32 context, uint32 x, uint32 y, uint32 z)
{
    uint32 cx, cy, cz, result;
    FUNCTION_MARKER(0x8003DC38, "GTMENU.EXE");
    w_u32(context + 944, y << 4);
    w_u32(context + 940, x << 4);
    w_u32(context + 948, z << 4);
    gtmenu_preview_vector_rotate(context + 940, context + 884, context + 940);
    x = r_u32(context + 940);
    y = r_u32(context + 944);
    cx = r_u32(context + 864);
    z = r_u32(context + 948);
    cz = r_u32(context + 872);
    w_u32(context + 940, cx + x);
    cy = r_u32(context + 868);
    w_u32(context + 948, cz + z);
    result = cy + y;
    w_u32(context + 944, result);
    return result;
}

uint32 gtmenu_preview_geometry_draw(uint32 context, uint32 model, sint16 roll, sint16 yaw, sint16 pitch)
{
    uint32 index, position, x, y, z;
    sint32 width, height, numerator, scale, angle, direction, offset;
    FUNCTION_MARKER(0x80033848, "GTMENU.EXE");
    position = model + 16;
    for (index = 0; index < 4; ++index) {
        width = r_s16(model + (index & 2 ? 52 : 48));
        numerator = (uint32)width << 12;
        height = r_s16(model + (index & 2 ? 54 : 50));
        if (!height) gt_draft_exception(7);
        if (height == -1 && numerator == (sint32)0x80000000) gt_draft_exception(6);
        scale = numerator / height;
        angle = index & 2 ? 0 : yaw;
        direction = roll;
        if (index & 1) { angle -= 1024; direction = -direction; }
        else angle += 1024;
        gtmenu_preview_rotation_set(context + 960, (sint16)angle, pitch, (sint16)direction);
        gtmenu_preview_wheel_matrix_scale(context, scale);
        x = r_s16(position);
        y = r_s16(position + 2);
        z = r_s16(position + 4);
        offset = index & 1 ? -width >> 1 : width >> 1;
        gtmenu_preview_wheel_position_set(context, x + offset, y, z);
        gtmenu_preview_wheel_draw(context, (uint32)height << 4, 0);
        position += 8;
    }
    return 0;
}

uint32 gtmenu_preview_wheel_draw(uint32 context, sint32 radius, uint32 detail)
{
    uint32 x, y, z, cx, cy, cz, ax, ay, az, a, b, scale;
    uint32 table, cursor, packet, ordering, words[6], next[6], count, result, index;
    uint32 fourth, flags, sum, first_area, second_area, palette, page, link, old;
    sint32 radius_exponent, exponent, minimum, difference, shift, scale_shift, depth, winding;
    FUNCTION_MARKER(0x8003DCB0, "GTMENU.EXE");
    x = r_u32(context + 940);
    cx = r_u32(context + 44);
    y = r_u32(context + 944);
    z = r_u32(context + 948);
    cy = r_u32(context + 48);
    cz = r_u32(context + 52);
    x -= cx; y -= cy; z -= cz;
    w_u32(context + 268, x);
    w_u32(context + 272, y);
    w_u32(context + 276, z);
    xport_gte_write_data(30, radius);
    radius_exponent = 31 - xport_gte_read_data(31);
    ax = (sint32)x < 0 ? 0 - x : x;
    ay = (sint32)y < 0 ? 0 - y : y;
    az = (sint32)z < 0 ? 0 - z : z;
    xport_gte_write_data(30, ax | ay | az);
    exponent = 31 - xport_gte_read_data(31);
    minimum = radius_exponent + 1;
    if (exponent < minimum) exponent = minimum;
    difference = exponent - minimum;
    shift = exponent - 13;
    if (shift < 0) shift = 0;
    x = (sint32)x >> (shift & 31);
    y = (sint32)y >> (shift & 31);
    z = (sint32)z >> (shift & 31);
    scale_shift = radius_exponent - 12;
    w_u16(context + 40, shift);
    w_u16(context + 42, difference);
    w_u16(context + 280, x);
    w_u16(context + 282, y);
    w_u16(context + 284, z);
    scale = radius;
    if (scale_shift > 0) scale = (sint32)scale >> (scale_shift & 31);
    else if (scale_shift < 0) scale <<= (0 - (uint32)scale_shift) & 31;
    gtmenu_matrix_transpose(context + 248, context + 960);
    gtmenu_matrix_scale_gte(context + 248, context + 248, (uint16)(0 - scale));
    gtmenu_matrix_rotate_short(context + 56, context + 248, context + 280);
    gtmenu_matrix_bind_packed_negation(context + 64);
    xport_gte_write_data(0, r_u32(context + 280));
    xport_gte_write_data(1, r_u32(context + 284));
    xport_gte_execute(0x486012);
    w_u32(context + 84, xport_gte_read_data(9));
    w_u32(context + 88, xport_gte_read_data(10));
    w_u32(context + 92, xport_gte_read_data(11));
    a = r_u32(context + 84); b = r_u32(context + 88);
    gte_ctc2(a, 5);
    a = r_u32(context + 92);
    gte_ctc2(b, 6); gte_ctc2(a, 7);
    gtmenu_matrix_multiply(context + 96, context + 64, context + 960);
    gtmenu_matrix_scale_gte(context + 96, context + 96, (uint16)scale);
    gtmenu_matrix_shift(context + 96, context + 96, difference);
    gtmenu_model_matrix_bind(context, context + 96);
    gtmenu_lighting_bind(context);
    table = r_u32(0x8007486C + detail * 4);
    result = table;
    count = r_s16(table);
    cursor = table + 4;
    if (!count) return result;
    ordering = r_u32(context + 928);
    shift = r_u16(context + 40);
    winding = r_s16(context + 908);
    packet = r_u32(0x800A8434);
    for (index = 0; index < 6; ++index) words[index] = r_u32(cursor + index * 4);
    do {
        xport_gte_write_data(0, words[2]); xport_gte_write_data(1, words[3]);
        xport_gte_write_data(2, words[4]); xport_gte_write_data(3, words[5]);
        xport_gte_write_data(4, words[0]); xport_gte_write_data(5, words[1]);
        w_u8(packet + 12, words[1] >> 16); w_u8(packet + 13, words[1] >> 24);
        w_u8(packet + 20, words[3] >> 16); w_u8(packet + 21, words[3] >> 24);
        xport_gte_execute(0x280030);
        for (index = 0; index < 4; ++index) next[index] = r_u32(cursor + 32 + index * 4);
        fourth = r_u32(cursor + 28);
        flags = xport_gte_read_control(31);
        w_u32(packet + 16, xport_gte_read_data(12));
        sum = xport_gte_read_data(17);
        sum += xport_gte_read_data(18);
        sum += xport_gte_read_data(19);
        w_u8(packet + 36, words[5] >> 16); w_u8(packet + 37, words[5] >> 24);
        w_u8(packet + 28, fourth >> 16); w_u8(packet + 29, fourth >> 24);
        xport_gte_execute(0x1400006);
        xport_gte_write_data(0, r_u32(cursor + 24));
        xport_gte_write_data(1, fourth);
        first_area = xport_gte_read_data(24);
        palette = r_u16(context + 936);
        page = r_u16(context + 938);
        xport_gte_execute(0x180001);
        next[4] = r_u32(cursor + 48); next[5] = r_u32(cursor + 52);
        w_u32(packet + 4, 0x2D000000);
        w_u32(packet, 0x09000000);
        w_u16(packet + 14, palette);
        w_u16(packet + 22, page);
        if (!winding) first_area = 0 - first_area;
        result = flags | xport_gte_read_control(31);
        a = xport_gte_read_data(19);
        --count;
        xport_gte_execute(0x1400006);
        sum += a;
        if ((sint32)result >= 0) {
            depth = (sint32)(sum << (shift & 31)) >> 15;
            link = ordering + (depth >= 4096 ? 16380 : (uint32)depth << 2);
            second_area = xport_gte_read_data(24);
            w_u32(packet + 32, xport_gte_read_data(12));
            if (winding) second_area = 0 - second_area;
            result = first_area & second_area;
            w_u32(packet + 8, xport_gte_read_data(13));
            if ((sint32)result >= 0) {
                w_u32(packet + 24, xport_gte_read_data(14));
                result = packet << 8;
                old = r_u8(link);
                old |= r_u8(link + 1) << 8;
                old |= r_u8(link + 2) << 16;
                w_u8(link, packet); w_u8(link + 1, packet >> 8); w_u8(link + 2, packet >> 16);
                w_u8(packet, old); w_u8(packet + 1, old >> 8); w_u8(packet + 2, old >> 16);
                packet += 40;
            }
        }
        cursor += 32;
        for (index = 0; index < 6; ++index) words[index] = next[index];
    } while (count);
    w_u32(0x800A8434, packet);
    return result;
}

uint32 gtmenu_font_quad16_draw(uint32 state, uint16 code, uint32 ordering, uint32 position, uint32 size)
{
    GT_LOCAL_STACK_FRAME frame;
    const uint8 *bitmap;
    uint32 pixels, colors, tex_x, tex_y, page, uv, packet, old, offset;
    uint32 x, y, width, height, right, bottom;
    sint32 pitch;
    FUNCTION_MARKER(0x80034D80, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80034D80);
    pixels = gtmenu_font_cache_acquire(state, code, frame.address, frame.address + 4);
    if (pixels) {
        bitmap = Krom2RawAdd2(code);
        colors = r_u32(state + 40);
        pitch = r_s16(state + 44);
        gtmenu_font_bitmap_unpack(pixels, bitmap, colors, pitch);
    }
    x = (sint16)position;
    width = (sint16)size - 1;
    height = (size >> 16) - 1;
    y = position >> 16;
    tex_y = r_u32(frame.address + 4);
    page = r_u32(state + 24);
    tex_x = r_u32(frame.address);
    colors = r_u32(state + 36);
    uv = ((tex_y & 255) << 8) | (tex_x & 255);
    page += (tex_x & 0xF00) << 8;
    packet = r_u32(0x800A8434);
    page += (tex_y & 0x100) << 12;
    w_u32(0x800A8434, packet + 40);
    old = r_u8(ordering);
    old |= r_u8(ordering + 1) << 8;
    old |= r_u8(ordering + 2) << 16;
    w_u8(ordering, packet); w_u8(ordering + 1, packet >> 8); w_u8(ordering + 2, packet >> 16);
    w_u8(packet, old); w_u8(packet + 1, old >> 8); w_u8(packet + 2, old >> 16);
    w_u16(packet + 14, page);
    w_u8(packet + 3, 9);
    w_u32(packet + 4, colors | 0x2C000000);
    w_u16(packet + 22, page >> 16);
    offset = r_u16(state + 46);
    right = x + width;
    w_u16(packet + 10, y);
    w_u16(packet + 18, y);
    bottom = y + height;
    w_u16(packet + 20, uv + 15);
    w_u16(packet + 12, uv);
    w_u16(packet + 16, right);
    w_u16(packet + 24, x);
    w_u16(packet + 26, bottom);
    w_u16(packet + 32, right);
    w_u16(packet + 34, bottom);
    w_u16(packet + 28, uv + 0xF00);
    w_u16(packet + 36, uv + 0xF0F);
    w_u16(packet + 8, x + offset);
    gtmenu_local_stack_pop(frame);
    return packet;
}

uint32 gtmenu_font_quad12_draw(uint32 state, uint16 code, uint32 ordering, uint32 position, uint32 size)
{
    GT_LOCAL_STACK_FRAME frame;
    const uint8 *bitmap;
    uint32 pixels, colors, tex_x, tex_y, page, uv, packet, old, offset;
    uint32 x, y, width, height, right, bottom;
    sint32 pitch;
    FUNCTION_MARKER(0x80034EF8, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80034EF8);
    pixels = gtmenu_font_cache_acquire(state, code, frame.address, frame.address + 4);
    if (pixels) {
        bitmap = Krom2RawAdd2(code);
        pitch = r_s16(state + 44);
        gtmenu_font_bitmap_resample12(pixels, bitmap, pitch);
    }
    x = (sint16)position;
    width = (sint16)size;
    y = position >> 16;
    height = size >> 16;
    tex_y = r_u32(frame.address + 4);
    page = r_u32(state + 24);
    tex_x = r_u32(frame.address);
    colors = r_u32(state + 36);
    uv = ((tex_y & 255) << 8) | (tex_x & 255);
    page += (tex_x & 0xF00) << 8;
    packet = r_u32(0x800A8434);
    page += (tex_y & 0x100) << 12;
    w_u32(0x800A8434, packet + 40);
    old = r_u8(ordering);
    old |= r_u8(ordering + 1) << 8;
    old |= r_u8(ordering + 2) << 16;
    w_u8(ordering, packet); w_u8(ordering + 1, packet >> 8); w_u8(ordering + 2, packet >> 16);
    w_u8(packet, old); w_u8(packet + 1, old >> 8); w_u8(packet + 2, old >> 16);
    w_u16(packet + 14, page);
    w_u8(packet + 3, 9);
    w_u32(packet + 4, colors | 0x2C000000);
    w_u16(packet + 22, page >> 16);
    offset = r_u16(state + 46);
    right = x + width;
    w_u16(packet + 10, y);
    w_u16(packet + 18, y);
    bottom = y + height;
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
    gtmenu_local_stack_pop(frame);
    return packet;
}

uint32 gtmenu_font_bitmap_resample12(uint32 pixels, const uint8 *bitmap, sint32 pitch)
{
    uint32 previous_left = 0, previous_right = 0, weight = 2, stride, row, result = 0;
    uint32 bits, first, second, third, fourth, left, right, mixed[2], index;
    uint32 shift, value, low, high;
    FUNCTION_MARKER(0x8003E658, "GTMENU.EXE");
    stride = pitch < 0 ? 0 - (uint32)pitch : (uint32)pitch;
    stride <<= 2;
    for (row = 0; row < 16; ++row) {
        /* The original discards the sixteenth ROM row */
        bits = row == 15 ? 0 : xport_load_le16(bitmap + row * 2);
        first = r_u16(0x80074920 + ((bits & 15) << 1));
        second = r_u16(0x80074920 + ((bits >> 3) & 30));
        third = r_u16(0x80074920 + ((bits >> 7) & 30));
        fourth = r_u16(0x80074920 + ((bits >> 12) << 1));
        left = (first << 15) | second;
        result = third << 15;
        right = result | fourth;
        if (weight) {
            mixed[0] = previous_left * weight + left * (4 - weight);
            mixed[1] = previous_right * weight + right * (4 - weight);
            for (index = 0; index < 6; ++index) {
                shift = (index % 3) * 10;
                value = mixed[index / 3];
                low = (value >> shift) & 31;
                high = (value >> (shift + 1)) & 0x1F0;
                if (low == 16) low = 15;
                if (high == 256) high = 240;
                result = low | high;
                w_u8(pixels + index, result);
            }
            pixels += stride;
        }
        weight = (weight - 1) & 3;
        previous_left = left;
        previous_right = right;
    }
    return result;
}

static void gtmenu_shadow_link(uint32 packet, uint32 link)
{
    uint32 old;
    old = r_u8(link);
    old |= r_u8(link + 1) << 8;
    old |= r_u8(link + 2) << 16;
    w_u8(link, packet); w_u8(link + 1, packet >> 8); w_u8(link + 2, packet >> 16);
    w_u8(packet, old); w_u8(packet + 1, old >> 8); w_u8(packet + 2, old >> 16);
}

static void gtmenu_shadow_vertices_save(uint32 context, uint32 quad)
{
    uint32 first = context + (quad ? 420 : 388);
    uint32 second = context + (quad ? 388 : 356);
    w_u32(first, xport_gte_read_data(0));
    w_u32(first + 4, xport_gte_read_data(1));
    w_u32(second, xport_gte_read_data(2));
    w_u32(second + 4, xport_gte_read_data(3));
    w_u32(context + 324, xport_gte_read_data(4));
    w_u32(context + 328, xport_gte_read_data(5));
}

static sint32 gtmenu_shadow_area_fallback(uint32 context)
{
    sint32 cx, cy, cz, vx, vy, vz, wx, wy, wz;
    uint32 x, y, z, a, b, c, restore0, restore2, restore4;
    cx = r_s16(context + 56); cy = r_s16(context + 58); cz = r_s16(context + 60);
    vx = r_s16(context + 356); vy = r_s16(context + 358);
    gte_ctc2(vx - cx, 0);
    vz = r_s16(context + 360);
    gte_ctc2(vy - cy, 2);
    wx = r_s16(context + 388);
    gte_ctc2(vz - cz, 4);
    wy = r_s16(context + 390);
    xport_gte_write_data(9, wx - cx);
    wz = r_s16(context + 392);
    xport_gte_write_data(10, wy - cy);
    xport_gte_write_data(11, wz - cz);
    vx = r_s16(context + 324); vy = r_s16(context + 326);
    xport_gte_execute(0x170000C);
    vz = r_s16(context + 328);
    x = xport_gte_read_data(25); y = xport_gte_read_data(26); z = xport_gte_read_data(27);
    xport_gte_write_data(9, vx - cx);
    xport_gte_write_data(10, vy - cy);
    xport_gte_write_data(11, vz - cz);
    gte_ctc2((x & 4095) | ((y & 4095) << 16), 16);
    a = (sint32)x >> 12; b = (sint32)y >> 12;
    gte_ctc2((z & 4095) | ((a & 4095) << 16), 17);
    c = (sint32)z >> 12;
    gte_ctc2((b & 4095) | ((c & 4095) << 16), 18);
    a = (sint32)a >> 12; b = (sint32)b >> 12; c = (sint32)c >> 12;
    gte_ctc2((a & 65535) | (b << 16), 19);
    gte_ctc2(c, 20);
    restore0 = r_u32(context + 96); restore2 = r_u32(context + 104);
    xport_gte_execute(0x45E012);
    restore4 = r_u32(context + 112);
    gte_ctc2(restore0, 0); gte_ctc2(restore2, 2); gte_ctc2(restore4, 4);
    x = xport_gte_read_data(25); y = xport_gte_read_data(26); z = xport_gte_read_data(27);
    return z + (uint32)((sint32)(y + ((sint32)x >> 12)) >> 12);
}

static sint32 gtmenu_shadow_screen_span(uint32 context, uint32 quad, uint32 *flags)
{
    sint32 first, second, third, fourth, minimum, maximum, extent;
    first = r_s16(context + 332); second = r_s16(context + 364);
    if (!quad) third = r_s16(context + 396);
    minimum = first < second ? first : second;
    maximum = first < second ? second : first;
    if (quad) third = r_s16(context + 396);
    if (third < minimum) minimum = third;
    if (maximum < third) maximum = third;
    if (quad) {
        fourth = r_s16(context + 428);
        if (fourth < minimum) minimum = fourth;
        if (maximum < fourth) maximum = fourth;
    }
    extent = r_s16(context + 4);
    if (maximum <= 0 || minimum >= extent) return 0;
    *flags |= 1023 - (maximum - minimum);
    first = r_s16(context + 334); second = r_s16(context + 366);
    if (!quad) third = r_s16(context + 398);
    minimum = first < second ? first : second;
    maximum = first < second ? second : first;
    if (quad) third = r_s16(context + 398);
    if (third < minimum) minimum = third;
    if (maximum < third) maximum = third;
    if (quad) {
        fourth = r_s16(context + 430);
        if (fourth < minimum) minimum = fourth;
        if (maximum < fourth) maximum = fourth;
    }
    extent = r_s16(context + 6);
    if (maximum <= 0 || minimum >= extent) return 0;
    *flags |= 511 - (maximum - minimum);
    return 1;
}

uint32 gtmenu_preview_mesh_draw(uint32 context, uint32 mesh)
{
    uint32 x, y, z, cx, cy, cz, ax, ay, az, a, b, low_z, high_z, index, point;
    uint32 bounds, records, packet, count, words[6], next[6], ordering, shift, flags;
    uint32 area, first_area, link, near_depth, kind, quad, clipped, stride, result;
    sint32 exponent, minimum, difference, delta_shift, depth, z1, z2;
    FUNCTION_MARKER(0x800339CC, "GTMENU.EXE");
    x = r_u32(context + 864); cx = r_u32(context + 44);
    y = r_u32(context + 868); z = r_u32(context + 872);
    cy = r_u32(context + 48); cz = r_u32(context + 52);
    x -= cx; y -= cy; z -= cz;
    w_u32(context + 268, x); w_u32(context + 272, y); w_u32(context + 276, z);
    ax = (sint32)x < 0 ? 0 - x : x;
    ay = (sint32)y < 0 ? 0 - y : y;
    az = (sint32)z < 0 ? 0 - z : z;
    xport_gte_write_data(30, ax | ay | az);
    exponent = 31 - xport_gte_read_data(31);
    minimum = r_u16(mesh + 4);
    if (exponent < minimum) exponent = minimum;
    difference = exponent - minimum;
    delta_shift = exponent - 12;
    if (delta_shift < 0) delta_shift = 0;
    x = (sint32)x >> (delta_shift & 31);
    y = (sint32)y >> (delta_shift & 31);
    z = (sint32)z >> (delta_shift & 31);
    w_u16(context + 40, delta_shift); w_u16(context + 42, difference);
    w_u16(context + 280, x); w_u16(context + 282, y); w_u16(context + 284, z);
    gtmenu_matrix_bind_packed_negation(context + 64);
    xport_gte_write_data(0, r_u32(context + 280)); xport_gte_write_data(1, r_u32(context + 284));
    xport_gte_execute(0x486012);
    w_u32(context + 84, xport_gte_read_data(9));
    w_u32(context + 88, xport_gte_read_data(10));
    w_u32(context + 92, xport_gte_read_data(11));
    a = r_u32(context + 84); b = r_u32(context + 88);
    gte_ctc2(a, 5); a = r_u32(context + 92); gte_ctc2(b, 6); gte_ctc2(a, 7);
    gtmenu_matrix_multiply(context + 96, context + 64, context + 884);
    gtmenu_matrix_scale_gte(context + 96, context + 96, 4096);
    gtmenu_matrix_shift(context + 96, context + 96, difference);
    gtmenu_model_matrix_bind(context, context + 96); gtmenu_lighting_bind(context);
    a = r_u32(mesh + 8); b = r_u32(mesh + 16);
    low_z = r_s16(mesh + 12); high_z = r_s16(mesh + 20);
    for (index = 0; index < 8; ++index) {
        point = context + 184 + index * 8;
        w_u16(point, index & 1 ? b : a);
        w_u16(point + 2, (index & 2 ? b : a) >> 16);
        w_u16(point + 4, index & 4 ? high_z : low_z);
    }
    bounds = gtmenu_model_bounds_classify(context, context + 184, 8);
    if (bounds & 31) return bounds & 31;
    gtmenu_matrix_transpose(context + 248, context + 884);
    gtmenu_matrix_scale_gte(context + 248, context + 248, (uint16)-4096);
    gtmenu_matrix_rotate_short(context + 56, context + 248, context + 280);
    a = r_u32(context + 96); b = r_u32(context + 100);
    gte_ctc2(a, 0); a = r_u32(context + 104); gte_ctc2(b, 1);
    b = r_u32(context + 108); gte_ctc2(a, 2);
    a = r_u32(context + 112); gte_ctc2(b, 3); gte_ctc2(a, 4);
    records = r_u32(mesh + 24);
    packet = r_u32(0x800A8434);
    w_u32(context + 932, packet);
    clipped = (bounds >> 16) & 63;
    for (kind = 0; kind < 2; ++kind) {
        quad = kind != 0;
        stride = quad ? 32 : 24;
        count = r_u16(mesh + kind * 2);
        if (quad || clipped) packet = r_u32(context + 932);
        if (!count) continue;
        for (index = 0; index < 6; ++index) words[index] = r_u32(records + index * 4);
        if (!quad) {
            if (!clipped) ordering = r_u32(context + 928);
            shift = r_u16(context + 40);
        }
        do {
            xport_gte_write_data(0, words[quad ? 2 : 4]);
            xport_gte_write_data(1, words[quad ? 3 : 5]);
            xport_gte_write_data(2, words[quad ? 4 : 2]);
            xport_gte_write_data(3, words[quad ? 5 : 3]);
            xport_gte_write_data(4, words[0]); xport_gte_write_data(5, words[1]);
            if (!clipped) --count;
            w_u32(packet, quad ? 0x05000000 : 0x04000000);
            xport_gte_execute(0x280030);
            if (clipped) --count;
            for (index = 0; index < 4; ++index) next[index] = r_u32(records + stride + index * 4);
            if (!quad && !clipped) {
                xport_gte_execute(0x1400006);
                next[4] = r_u32(records + 40); next[5] = r_u32(records + 44);
                area = xport_gte_read_data(24);
                depth = xport_gte_read_data(17);
                w_u32(packet + 4, 0x20000000);
                if ((sint32)area < 0) goto gtmenu_shadow_next;
                z1 = xport_gte_read_data(18); z2 = xport_gte_read_data(19);
                w_u32(packet + 16, xport_gte_read_data(12));
                if (depth < z1) depth = z1;
                w_u32(packet + 12, xport_gte_read_data(13));
                if (depth < z2) depth = z2;
                w_u32(packet + 8, xport_gte_read_data(14));
            } else if (!quad) {
                flags = xport_gte_read_control(31);
                depth = xport_gte_read_data(17);
                ordering = r_u32(context + 928); near_depth = r_u16(context + 8);
                xport_gte_execute(0x1400006);
                next[4] = r_u32(records + 40); next[5] = r_u32(records + 44);
                area = xport_gte_read_data(24);
                z1 = xport_gte_read_data(18); z2 = xport_gte_read_data(19);
                w_u32(packet + 16, xport_gte_read_data(12));
                if (depth < z1) depth = z1;
                w_u32(packet + 12, xport_gte_read_data(13));
                if (depth < z2) depth = z2;
                if (depth < (sint32)near_depth) goto gtmenu_shadow_next;
            } else {
                if (clipped) flags = xport_gte_read_control(31);
                if (clipped) {
                    w_u32(packet + 12, xport_gte_read_data(12));
                    w_u32(context + 364, xport_gte_read_data(12));
                    w_u32(context + 356, xport_gte_read_data(0));
                    w_u32(context + 360, xport_gte_read_data(1));
                    ordering = r_u32(context + 928); near_depth = r_u16(context + 8);
                }
                xport_gte_execute(0x1400006);
                xport_gte_write_data(0, r_u32(records + 24));
                xport_gte_write_data(1, r_u32(records + 28));
                first_area = xport_gte_read_data(24);
                if (!clipped) w_u32(packet + 12, xport_gte_read_data(12));
                depth = xport_gte_read_data(17); z1 = xport_gte_read_data(18); z2 = xport_gte_read_data(19);
                if (clipped) shift = r_u16(context + 40);
                else ordering = r_u32(context + 928);
                xport_gte_execute(0x180001);
                if (depth < z1) depth = z1;
                w_u32(packet + 4, 0x28000000);
                if (depth < z2) depth = z2;
                next[4] = r_u32(records + 48); next[5] = r_u32(records + 52);
                if (clipped) flags |= xport_gte_read_control(31);
                z1 = xport_gte_read_data(19);
                if (!clipped) shift = r_u16(context + 40);
                xport_gte_execute(0x1400006);
                if (depth < z1) depth = z1;
                if (clipped && depth < (sint32)near_depth) goto gtmenu_shadow_next;
                area = 0 - first_area;
            }
            depth = (sint32)((uint32)depth << (shift & 31)) >> 13;
            link = ordering + (depth >= 4096 ? 16380 : (uint32)depth << 2);
            if (quad) area &= xport_gte_read_data(24);
            if (!clipped) {
                if (quad) {
                    w_u32(packet + 20, xport_gte_read_data(12));
                    w_u32(packet + 8, xport_gte_read_data(13));
                    if ((sint32)area < 0) goto gtmenu_shadow_next;
                    w_u32(packet + 16, xport_gte_read_data(14));
                }
                gtmenu_shadow_link(packet, link);
                packet += quad ? 24 : 20;
                goto gtmenu_shadow_next;
            }
            if ((sint32)flags < 0) {
                gtmenu_shadow_vertices_save(context, quad);
                area = gtmenu_shadow_area_fallback(context);
            }
            if (quad) w_u32(context + 396, xport_gte_read_data(12));
            else w_u32(packet + 8, xport_gte_read_data(14));
            if ((sint32)area < 0) goto gtmenu_shadow_next;
            if (quad) {
                w_u32(context + 332, xport_gte_read_data(13));
                w_u32(context + 428, xport_gte_read_data(14));
            } else {
                w_u32(context + 396, xport_gte_read_data(12));
                w_u32(context + 364, xport_gte_read_data(13));
                w_u32(context + 332, xport_gte_read_data(14));
            }
            if (!gtmenu_shadow_screen_span(context, quad, &flags)) goto gtmenu_shadow_next;
            if (!quad) w_u32(packet + 4, 0x20000000);
            if ((sint32)flags >= 0) {
                gtmenu_shadow_link(packet, link);
                if (quad) {
                    w_u32(packet + 20, xport_gte_read_data(12));
                    w_u32(packet + 8, xport_gte_read_data(13));
                    w_u32(packet + 16, xport_gte_read_data(14));
                }
                packet += quad ? 24 : 20;
            } else {
                gtmenu_shadow_vertices_save(context, quad);
                if (quad) w_u16(context + 184, 4);
                w_u32(context + 168, link);
                w_u32(context + 172, packet);
                if (!quad) w_u16(context + 184, 3);
                w_u32(context + 156, quad ? 0x28000000 : 0x20000000);
                gtmain_polygon_clip_flat(context);
                packet = r_u32(context + 180);
                for (index = 0; index < 6; ++index) next[index] = r_u32(records + stride + index * 4);
            }
gtmenu_shadow_next:
            records += stride;
            for (index = 0; index < 6; ++index) words[index] = next[index];
        } while (count);
        w_u32(context + 932, packet);
    }
    result = r_u32(context + 932);
    w_u32(0x800A8434, result);
    return result;
}
