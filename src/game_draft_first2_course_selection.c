#include "game_first2_registry.h"

/* GTMAIN.EXE 80022AA4 */
sint32 gtmain_course_selection_apply(sint32 mode)
{
    uint32 row = 0x8008F500u + 24u * r_u8(0x800CB7F8u);
    uint32 names = r_u32(row);
    uint32 name = r_u32(names + 4u * r_u8(0x800CB7F9u));
    uint32 single_course = r_u32(names + 4u) == 0u;
    sint32 course;
    w_u8(0x800B3914u, (uint8)single_course);
    if (!name) return 1;
    course = gtmain_course_name_find(name);
    if (course >= 0) gtmain_race_arcade_track((uint32)course);
    else course = 0;
    w_u8(0x800B6310u, 0u);
    w_u8(0x800B63BCu, (uint8)course);
    w_u8(0x800CB7F6u, (uint8)mode);
    if (mode == 1) {
        uint16 count;
        gtmain_race_cars_invalidate();
        count = r_u16(row + 20u);
        w_u8(0x800B6394u, 6u);
        w_u16(0x800B634Cu, count);
    } else if (mode == 2) {
        w_u8(0x800B6310u, 1u);
        w_u8(0x800B6394u, 1u);
        w_u16(0x800B634Cu, 0u);
    } else if (mode == 0) {
        w_u8(0x800B6394u, 1u);
        w_u16(0x800B634Cu, 1u);
    }
    return 0;
}

static sint32 gtmain_course_dot_delta(const uint8 *position, uint32 segment, uint32 reverse)
{
    uint8 vector[12];
    uint32 index;
    for (index = 0; index < 3u; ++index)
    {
        uint32 value = xport_load_le32(position + index * 4u);
        uint32 origin = r_u32(segment + 24u + index * 4u);
        xport_store_le32(vector + index * 4u, reverse ? origin - value : value - origin);
    }
    return gtmain_vector_dot_fixed(vector, (const uint8 *)psx_addr(segment + 36u, 6u));
}

sint32 gtmain_course_segment_find(uint32 index, const uint8 *position)
{
    uint32 segment = r_u32(r_u32(0x800BC1A8u) + index * 4u + 12u);
    for (;;)
    {
        uint32 previous;
        if (gtmain_course_dot_delta(position, segment, 0) >= 0) break;
        previous = r_u32(segment + 4u);
        if (!previous) return (sint32)index;
        index = r_u16(segment);
        segment = previous;
    }
    for (;;)
    {
        uint32 next = r_u32(segment + 8u);
        uint32 next_index;
        if (!next) return (sint32)index;
        next_index = r_u16(segment + 2u);
        segment = next;
        if (gtmain_course_dot_delta(position, segment, 0) < 0) return (sint32)index;
        index = next_index;
    }
}

sint32 gtmain_course_nearest_segment(const uint8 *position)
{
    uint32 table = r_u32(0x800BC1A8u);
    uint32 count = r_u16(table + 4u);
    sint32 selected = -1, minimum = 0x7FFFFFFF;
    uint32 index;
    if (count)
    {
        uint32 x, y, z;
        table = r_u32(0x800BC1A8u);
        x = xport_load_le32(position); y = xport_load_le32(position + 4u); z = xport_load_le32(position + 8u);
        for (index = 0; index < count; ++index)
        {
            uint32 segment = r_u32(table + index * 4u + 12u);
            uint32 dx = (uint32)((sint32)(r_u32(segment + 48u) - x) >> 16);
            uint32 dy = (uint32)((sint32)(r_u32(segment + 52u) - y) >> 16);
            uint32 dz = (uint32)((sint32)(r_u32(segment + 56u) - z) >> 16);
            sint32 distance = (sint32)(dx * dx + dy * dy + dz * dz);
            if (distance < minimum) { minimum = distance; selected = (sint32)index; }
        }
    }
    if (selected != -1) return gtmain_course_segment_find((uint32)selected, position);
    return selected;
}

uint32 gtmain_course_start_position(void)
{
    return r_u32(0x800BC1B0u) + 64u;
}

sint32 gtmain_course_start_angle(void)
{
    return (sint16)r_u16(r_u32(0x800BC1B0u) + 76u);
}

sint32 gtmain_course_start_segment(void)
{
    return gtmain_course_nearest_segment((const uint8 *)psx_addr(gtmain_course_start_position(), 12u));
}

uint32 gtmain_course_grid_position(uint32 index)
{
    return r_u32(0x800BC1B0u) + index * 12u + 80u;
}

sint32 gtmain_course_progress(uint32 index, const uint8 *position)
{
    uint32 owner = (uint32)gtmain_course_segment_find(index, position);
    uint32 segment = r_u32(r_u32(0x800BC1A8u) + owner * 4u + 12u);
    uint32 next = r_u32(segment + 8u);
    sint32 dot = gtmain_course_dot_delta(position, segment, 0);
    sint32 first = (sint32)(uint32)((uint64)((sint64)dot * (sint16)r_u16(segment + 22u)) >> 12);
    sint32 second;
    sint32 denominator;
    sint32 factor;
    uint32 start, course_length, difference, value;
    dot = gtmain_course_dot_delta(position, next, 1);
    second = (sint32)(uint32)((uint64)((sint64)dot * (sint16)r_u16(segment + 20u)) >> 12);
    denominator = (sint32)((uint32)first + (uint32)second);
    factor = denominator ? (sint32)(uint32)gtmain_signed_divide64((sint64)first * 4096,
        (sint64)denominator, 0x80024138u) : 0;
    start = r_u32(segment + 16u);
    course_length = r_u32(r_u32(0x800BC1A8u));
    difference = r_u32(next + 16u) - start;
    if ((sint32)course_length < (sint32)difference) difference -= course_length;
    else if ((sint32)difference < 0) difference += course_length;
    value = (uint32)((uint64)((sint64)(sint32)difference * factor) >> 12) + start;
    if ((sint32)course_length < (sint32)value) value -= course_length;
    else if ((sint32)value < 0) value += course_length;
    return (sint32)value;
}

static uint32 selection_stack_name(uint32 offset, uint32 bytes)
{
    uint32 launch = r_u32(0x8008F1D8u), alternate = r_u32(0x800B1AA4u);
    uint32 top = alternate ? alternate : launch;
    uint32 reserve = r_u32(0x800B1B4Cu);
    uint32 heap = r_u32(0x800B1AACu), heap_bytes = r_u32(0x800B1AA8u);
    uint32 extent = heap_bytes < 16u ? 16u : heap_bytes, address;
    if (top < 0x80010000u + offset || top > 0x80200000u || (top & 7u) || bytes > offset)
        gt_draft_failure("GTMAIN selection name stack range", 0x8006B798u);
    address = top - offset;
    if (address < 0x801FB724u || heap < 0x80010000u || heap > address || extent > address - heap ||
        (!alternate && (reserve < offset || address < launch - reserve)))
        gt_draft_failure("GTMAIN selection name stack overlap", 0x8006B798u);
    return address;
}
sint32 gtmain_car_record_find_variant(uint32 name, uint32 archive, uint32 section)
{
    uint32 local_name = selection_stack_name(0x4240u, 8u);
    sint32 result;
    gtmain_string_copy(local_name, name);
    result = gtmain_car_record_find(local_name, archive, section, 0u);
    if (result < 0)
    {
        if ((sint8)r_u8(local_name + 4u) == 114)
        {
            w_u8(local_name + 4u, 110u);
            result = gtmain_car_record_find(local_name, archive, section, 1u);
        }
    }
    return result;
}

uint32 gtmain_car_selected_record(uint32 index, uint32 archive, uint32 section)
{
    uint32 name = gtmain_car_name(index);
    sint32 record = gtmain_car_record_find_variant(name, archive, section);
    uint32 section_offset;
    if (record < 0) record = 0;
    section_offset = r_u32(archive + section * 12u + 16u);
    return archive + section_offset + 24u + (uint32)record * 108u;
}

/* TODO Resolve name extent in the independent selected-record lookup */
/* GTMAIN.EXE 8001E8DC */
sint32 gtmain_opponent_configurations(uint32 archive, uint32 section)
{
    uint32 index;
    for (index = 1; index < 6u; ++index)
    {
        uint32 record = gtmain_car_selected_record(r_u16(0x800B664Eu + index * 2u) & 0x1FFu, archive, section);
        gtmain_car_config_build(0x800BB850u + index * 386u, record, archive);
    }
    return 0;
}

uint32 gtmain_car_selection_bind(uint32 index, uint32 archive)
{
    uint32 name = selection_stack_name(0x4260u, 32u);
    uint32 offset = index * 96u;
    uint32 id = r_u16(0x800BB790u + offset) & 0xFFFu;
    uint32 source = archive + r_u32(archive + 172u) + id * 424u - 400u;
    sint32 selected, alternate;
    selected = gtmain_car_find(gtmain_string_copy(name, source));
    if (r_u16(0x800BB7B2u + offset))
    {
        w_u8(name + 4u, 114u);
        alternate = gtmain_car_find(name);
        if (alternate >= 0) selected = alternate;
    }
    w_u16(0x800B63C4u + index * 2u, (uint16)selected);
    return index * 2u;
}

/* TODO Resolve the independent selection bind name buffer contract */
/* GTMAIN.EXE 8001E7A0 */
uint32 gtmain_car_saved_selection_load(uint32 index, uint32 saved_index, uint32 archive)
{
    gtmain_memory_copy(0x800BB790u + index * 96u, 0x800C4DECu + saved_index * 96u, 96u);
    return gtmain_car_selection_bind(index, archive);
}

uint32 gtmain_player_configuration_build(void)
{
    return gtmain_car_config_build(0x800BB850u, 0x800BB790u, 0x800D6FDCu);
}


