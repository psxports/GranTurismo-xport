#include "game_first2_registry.h"

sint32 gtmain_driver_mode_set(uint32 unused, uint32 driver, sint32 mode)
{
    (void)unused;
    if (mode == 1) { w_u16(driver + 246u, 1); w_u8(driver + 253u, 0); return 1; }
    if (!mode) { w_u16(driver + 246u, 0); return 2; }
    if (mode == 2)
    {
        sint32 result = (sint16)r_u16(0x800B63DEu) < 2;
        w_u16(driver + 246u, 2);
        w_u8(driver + 253u, result ? 0 : 1);
        return 1;
    }
    if (mode == 3) { w_u16(driver + 246u, 3); w_u8(driver + 253u, 2); }
    return 2;
}

uint32 gtmain_driver_leader(void)
{
    uint32 first = 0x800B6410u, second = first + 276u;
    if (r_u8(0x800CB7F5u) == 2)
    {
        uint32 first_car = r_u32(first + 180u);
        uint32 second_car = r_u32(second + 180u);
        sint32 first_position = (sint8)r_u8(first_car + 1737u);
        sint32 second_position = (sint8)r_u8(second_car + 1737u);
        if (first_position >= second_position) return second;
    }
    return first;
}

sint32 gtmain_driver_assist(uint32 car)
{
    sint32 mode = (sint16)r_u16(car + 26u);
    uint32 driver = r_u32(0x800B6638u);
    if (mode >= 3 || mode <= 0) return 0;
    if (car != r_u32(driver + 180u) && !r_u8(0x800B633Cu)) return 0;
    if (r_u8(driver + 254u)) return 2;
    return (sint16)r_u16(driver + 246u) == 2;
}

sint32 gtmain_driver_demo_active(void)
{
    uint32 driver = r_u32(0x800B6638u);
    if (!r_u8(driver + 254u)) return 0;
    return r_u8(driver + 255u) == 0;
}

sint32 gtmain_driver_is_primary(uint32 car)
{
    uint32 driver = r_u32(0x800B6638u);
    sint32 mode = (sint8)r_u8(driver + 253u);
    if (mode < 0 || mode >= 2) return 0;
    return r_u32(driver + 180u) == car;
}

uint32 gtmain_car_configuration_export(void)
{
    if (r_u8(0x800CB7F4u) != 2) return 2;
    if (r_u8(0x800CB7F5u) == 2 || r_u8(0x800CB7F5u) == 4) return 4;
    return gtmain_memory_copy(0x800C4DECu + (uint32)(sint32)(sint16)r_u16(0x800C4C58u) * 96u,
        0x800BB790u, 96u);
}

sint32 gtmain_name_group_find(uint32 groups, uint32 name)
{
    uint32 index = 0, group = r_u32(groups);
    while (group)
    {
        uint32 entry = r_u32(group);
        while (entry)
        {
            if (!gtmain_string_compare(name, entry)) return (sint32)index;
            group += 4u;
            entry = r_u32(group);
        }
        ++index;
        group = r_u32(groups + index * 4u);
    }
    return -1;
}

sint32 gtmain_race_preset_index(void)
{
    uint32 kind = r_u8(0x800CB7F4u);
    if (kind == 1)
    {
        if (r_u8(0x800CB7F5u)) return -1;
        return (sint32)(r_u8(0x800B63BDu) + 8u * r_u8(0x800CB7F9u) + 64u);
    }
    if (kind == 2 && r_u8(0x800CB7F5u) == 3)
    {
        uint32 index = r_u8(0x800CB7F8u);
        if (index < 10u) return (sint32)(r_u8(0x8008F394u + index) + r_u8(0x800CB7F9u));
        if (index < 15u) return (sint32)(index + 28u);
        if (index < 22u) return (sint32)(r_u8(0x8008F391u + index) + r_u8(0x800CB7F9u));
    }
    return -1;
}

sint32 gtmain_roster_shift(sint32 position)
{
    uint32 index;
    for (index = 1; index < 6u; ++index)
    {
        uint32 record = 0x800B6670u + index * 76u;
        if ((sint32)r_u8(record + 3u) >= position)
            w_u8(record + 3u, r_u8(record + 3u) + 1u);
    }
    return 0;
}

uint32 gtmain_roster_player(uint32 index)
{
    uint32 record = 0x800B6670u + index * 76u;
    uint32 equipment = r_u16(0x800BB790u + index * 96u);
    uint32 car = r_u16(0x800B63C4u + index * 2u);
    uint32 position = r_u8(0x800B63D0u + index * 3u);
    w_u16(record, car);
    w_u8(record + 2u, equipment >> 12);
    w_u8(record + 3u, position);
    return record;
}

sint32 gtmain_roster_opponents(void)
{
    uint32 index;
    for (index = 1; index < 6u; ++index)
    {
        uint32 selector = 0x800B664Eu + index * 2u;
        uint32 record = 0x800B6670u + index * 76u;
        w_u16(record, r_u16(selector) & 0x1FFu);
        {
            uint32 equipment = r_u16(selector);
            w_u8(record + 3u, index - 1u);
            w_u8(record + 2u, equipment >> 9);
        }
    }
    return 0;
}

sint32 gtmain_model_extent(uint32 model)
{
    uint32 geometry = r_u32(model + 68u);
    sint32 shift = (sint16)r_u16(geometry + 76u) - 12;
    sint32 value = (sint16)r_u16(geometry + 72u);
    if (shift < 0) return value >> ((0u - (uint32)shift) & 31u);
    return (sint32)((uint32)value << ((uint32)shift & 31u));
}

void gtmain_model_marker(uint32 model, uint32 destination)
{
    if (model)
    {
        uint32 geometry = r_u32(model + 68u);
        sint32 shift = (sint16)r_u16(geometry + 76u) - 16;
        uint32 x = (uint32)(sint32)(sint16)r_u16(geometry + 64u);
        uint32 z = (uint32)(sint32)(sint16)r_u16(geometry + 72u);
        if (shift < 0)
        {
            uint32 amount = (0u - (uint32)shift) & 31u;
            x = (uint32)((sint32)x >> amount); z = (uint32)((sint32)z >> amount);
        }
        else { x <<= (uint32)shift & 31u; z <<= (uint32)shift & 31u; }
        x = ((0u - x) * 125u) >> 9;
        z = (z * 125u) >> 9;
        w_u16(destination + 8u, z);
        w_u16(destination + 10u, x);
    }
}

uint32 gtmain_race_course_car_filter(void)
{
    uint32 entry = gtmain_course_entry(r_u8(0x800B63BCu));
    uint32 enabled = r_u8(entry) >> 7;
    gtmain_course_car_filter(enabled);
    return enabled ? r_u32(0x80097484u) : entry;
}

uint32 gtmain_race_saved_flag(void)
{
    uint32 result = 1;
    if (r_u8(0x800CB7F5u) == 1)
    {
        result = r_u32(0x800B630Cu);
        if (result == 1)
        {
            result = r_u8(0x800B6394u);
            w_u16(0x800BC1A6u, result);
        }
    }
    return result;
}

sint32 gtmain_collision_layout(uint8 *out, const uint8 *source)
{
    uint32 first = xport_load_le32(source);
    uint32 second = xport_load_le32(source + 8u);
    uint32 third = xport_load_le32(source + 4u);
    sint32 result;
    xport_store_le32(out, first << 4);
    xport_store_le32(out + 4u, second << 4);
    xport_store_le32(out + 8u, third << 4);
    first = xport_load_le16(source + 20u); second = xport_load_le16(source + 24u); third = xport_load_le16(source + 22u);
    xport_store_le16(out + 20u, (uint16)first); xport_store_le16(out + 26u, (uint16)second); xport_store_le16(out + 32u, (uint16)third);
    first = xport_load_le16(source + 28u); second = xport_load_le16(source + 32u); third = xport_load_le16(source + 30u);
    xport_store_le16(out + 22u, (uint16)first); xport_store_le16(out + 28u, (uint16)second); xport_store_le16(out + 34u, (uint16)third);
    result = (sint16)xport_load_le16(source + 12u); second = xport_load_le16(source + 16u); third = xport_load_le16(source + 14u);
    xport_store_le16(out + 24u, (uint16)result); xport_store_le16(out + 30u, (uint16)second); xport_store_le16(out + 36u, (uint16)third);
    return result;
}

static uint32 gtmain_roster_lwl(uint32 address, uint32 previous)
{
    uint32 shift = (3u - (address & 3u)) * 8u;
    uint32 preserved = shift ? previous & ((1u << shift) - 1u) : 0;
    return preserved | (r_u32(address & ~3u) << shift);
}

static void gtmain_roster_swl(uint32 address, uint32 value)
{
    uint32 count = (address & 3u) + 1u;
    uint32 base = address & ~3u;
    uint32 index;
    value >>= (4u - count) * 8u;
    for (index = 0; index < count; ++index) w_u8(base + index, value >> (index * 8u));
}

uint32 gtmain_packet_three_tiles(uint32 link, uint32 tile, uint32 xy)
{
    uint32 coordinate = tile + 194u;
    uint32 low = (coordinate & 63u) << 4;
    uint32 high = (coordinate >> 6) << 16;
    uint32 left = (uint16)low | high;
    uint32 right = (low + 16u) | high;
    uint32 packet = r_u32(0x800BFE4Cu);
    uint32 tag;
    w_u32(packet + 24u, right); w_u32(packet + 44u, right);
    w_u32(packet + 4u, 0x80000000u); w_u32(packet + 20u, 0x80000000u); w_u32(packet + 36u, 0x80000000u);
    w_u8(packet + 3u, 12);
    w_u32(packet + 8u, left); w_u32(packet + 12u, xy); w_u32(packet + 28u, left); w_u32(packet + 40u, xy);
    w_u32(packet + 16u, 0x10010u); w_u32(packet + 32u, 0x10010u); w_u32(packet + 48u, 0x10010u);
    tag = gtmain_roster_lwl(link + 2u, 0x10010u);
    gtmain_roster_swl(link + 2u, packet << 8);
    gtmain_roster_swl(packet + 2u, tag);
    packet += 52u;
    w_u32(0x800BFE4Cu, packet);
    return packet;
}

sint32 gtmain_vector_dot_fixed(const uint8 *vector, const uint8 *coefficients)
{
    sint64 first = (sint64)(sint16)xport_load_le16(coefficients) * (sint32)xport_load_le32(vector);
    sint64 second = (sint64)(sint16)xport_load_le16(coefficients + 2u) * (sint32)xport_load_le32(vector + 4u);
    sint64 third = (sint64)(sint16)xport_load_le16(coefficients + 4u) * (sint32)xport_load_le32(vector + 8u);
    uint64 sum = (uint64)first + (uint64)second + (uint64)third;
    return (sint32)(uint32)(sum >> 12);
}

uint32 gtmain_course_resource_name(uint32 index)
{
    return gtmain_packed_bytes_entry(r_u32(0x800C0C10u), index) + 1u;
}

uint32 gtmain_car_archive_load(uint32 buffer, uint32 capacity)
{
    /* TODO Archive load retains its named CD filesystem and extent adapter dependencies */
    uint32 result = gtmain_archive_table_load_guest(r_u32(0x800C00D4u), 0x800C00D8u, buffer, capacity);
    w_u32(0x800C00D0u, result);
    return result;
}
