#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"
#include <stdlib.h>

static sint32 resource_render_divide(uint32 numerator, sint32 denominator)
{
    sint32 value = (sint32)numerator;
    if (!denominator) gt_draft_failure("Original wheel BREAK 7", 0x800714F8u);
    if (value == (sint32)0x80000000u && denominator == -1)
        gt_draft_failure("Original wheel BREAK 6", 0x80071510u);
    return value / denominator;
}

uint32 gtmain_car_wheels_draw(uint32 context, uint32 car, sint32 roll, sint16 steering, sint16 pitch)
{
    uint32 index, position = car + 16u;
    for (index = 0; index != 4u; ++index)
    {
        sint32 width = (sint16)r_u16(car + ((index & 2u) ? 52u : 48u));
        sint32 radius = (sint16)r_u16(car + ((index & 2u) ? 54u : 50u));
        sint32 scale = resource_render_divide((uint32)width << 12, radius);
        uint32 yaw = (index & 2u) ? 0u : (uint32)(sint32)steering;
        uint32 z = (uint32)roll, half;
        sint32 x, y, pz;
        if (index & 1u) { yaw -= 1024u; z = 0u - z; }
        else yaw += 1024u;
        gtmain_model_rotation((uint8 *)psx_addr(context + 960u, 20u), (sint16)yaw, pitch, (sint16)z);
        gtmain_model_wheel_matrix(context, scale);
        x = (sint16)r_u16(position); y = (sint16)r_u16(position + 2u); pz = (sint16)r_u16(position + 4u);
        half = (index & 1u) ? 0u - (uint32)width : (uint32)width;
        half = (uint32)((sint32)half >> 1);
        gtmain_model_position(context, (sint32)((uint32)x + half), y, pz);
        gtmain_model_wheel_draw(context, (sint32)((uint32)radius << 4), 0u);
        position += 8u;
    }
    return 0;
}

sint32 gtmain_car_record_index(uint32 records, sint32 count, uint32 *ids, uint32 *keys,
                               uint32 *id_count, uint32 *key_count)
{
    uint32 unique = 0, item = 0, key = 0;
    while ((sint32)item < count)
    {
        uint32 record = records + item * 76u, flag = r_u8(record + 2u), id = r_u16(record), found = 0;
        while (found < unique)
        {
            if (ids[found] == id) break;
            ++found;
        }
        if (found == unique) ids[unique++] = id;
        flag = (flag << 16) | id;
        record = records + item * 76u;
        w_u16(record + 6u, found); keys[key] = flag; w_u16(record + 4u, key);
        ++item; ++key;
    }
    *id_count = unique; *key_count = key;
    if ((sint16)r_u16(0x800C00C8u) < (sint32)unique) return 1;
    return 2 * ((sint16)r_u16(0x800C00CAu) < (sint32)key);
}

uint32 gtmain_car_section_load(uint32 section, uint32 destination, uint32 limit)
{
    uint32 path = r_u32(0x800C00D4u), table = r_u32(0x800C00D0u);
    uint32 sector = r_u32(0x800C00D8u);
    const uint8 *bytes = NULL;
    if (table)
    {
        uint64 extent = (uint64)section * 12u + 28u;
        if (extent > 0xFFFFFFFFu)
            gt_draft_failure("Native car archive table extent", 0x800788C0u);
        bytes = (const uint8 *)psx_addr(table, (uint32)extent);
    }
    return gtmain_archive_section_load(path, bytes, sector, section, destination, limit);
}

static uint32 resource_render_read32(uint32 address)
{
    uint32 a = r_u8(address), b = r_u8(address + 1u), c = r_u8(address + 2u), d = r_u8(address + 3u);
    return a | (b << 8) | (c << 16) | (d << 24);
}

static void resource_render_write32(uint32 address, uint32 value)
{
    w_u8(address, value); w_u8(address + 1u, value >> 8);
    w_u8(address + 2u, value >> 16); w_u8(address + 3u, value >> 24);
}

static void resource_render_copy_block(uint32 destination, uint32 source)
{
    uint32 a = resource_render_read32(source), b = resource_render_read32(source + 4u);
    uint32 c = resource_render_read32(source + 8u), d = resource_render_read32(source + 12u);
    resource_render_write32(destination, a); resource_render_write32(destination + 4u, b);
    resource_render_write32(destination + 8u, c); resource_render_write32(destination + 12u, d);
}

static uint32 *resource_render_words(uint32 count)
{
    uint32 *words;
    size_t actual = count ? (size_t)count : 1u;
    if (actual > (size_t)-1 / sizeof(uint32))
        gt_draft_failure("Native car resource workspace extent", 0x8006E420u);
    words = (uint32 *)malloc(actual * sizeof(uint32));
    if (!words) gt_draft_failure("Native car resource workspace allocation", 0x8006E420u);
    return words;
}

sint32 gtmain_car_records_decode(uint32 records, sint32 count, const uint32 *destinations,
    uint32 destinations_count, uint32 texture_destination)
{
    uint32 palette_indices = r_u32(0x800C00DCu), page_indices = r_u32(0x800C00CCu);
    uint32 capacity = count > 0 ? (uint32)count : 0u, unique, keys_count, group;
    uint32 *ids = resource_render_words(capacity), *keys = resource_render_words(capacity), *sorted;
    sint32 result = gtmain_car_record_index(records, count, ids, keys, &unique, &keys_count);
    if (result) { free(keys); free(ids); return result; }
    sorted = resource_render_words(unique);
    for (group = 0; group < unique; ++group)
    {
        uint32 value = ids[group], index;
        for (index = 0; index < group; ++index)
        {
            uint32 previous = sorted[index];
            if ((sint32)value < (sint32)previous) { sorted[index] = value; value = previous; }
        }
        sorted[group] = value;
    }
    for (group = 0; group < unique; ++group)
    {
        uint32 id = sorted[group], archive_index = id + r_u32(0x80097484u);
        uint32 image = gtmain_car_section_load(archive_index << 1, texture_destination, 0u), item = 0;
        sint32 page = -1;
        while ((sint32)item < count)
        {
            uint32 record = records + item * 76u;
            if (r_u16(record) == id)
            {
                uint32 palette;
                if (page < 0)
                {
                    uint32 index = r_u16(record + 6u);
                    page = (sint32)r_u16(page_indices + index * 2u);
                    gtmain_texture_page_upload(image, page);
                }
                record = records + item * 76u; w_u16(record + 6u, (uint32)page);
                resource_render_copy_block(record + 44u, image + 64u);
                resource_render_copy_block(record + 60u, image + 80u);
                resource_render_copy_block(record + 12u, image + 32u);
                resource_render_copy_block(record + 28u, image + 48u);
                palette = r_u16(palette_indices + r_u16(record + 4u) * 2u);
                gtmain_texture_palette_upload(image, (sint32)palette, r_u8(record + 2u));
                w_u16(record + 4u, palette);
            }
            ++item;
        }
        if (!texture_destination)
        {
            /* TODO Bind the unselected original guest heap free dependency */
            (void)image;
            gt_draft_failure("Unrecorded GTMAIN car image heap free", 0x8007CFE0u);
        }
        {
            uint32 destination = 0u;
            if (destinations)
            {
                if (group >= destinations_count)
                    gt_draft_failure("Native car destination array extent", 0x8006E420u);
                destination = destinations[group];
            }
            uint32 bundle = gtmain_car_section_load((archive_index << 1) + 1u, destination, 0u);
            item = 0;
            while ((sint32)item < count)
            {
                uint32 record = records + item * 76u;
                if (r_u16(record) == id) w_u32(record + 8u, bundle);
                ++item;
            }
            gtmain_model_bundle_relocate(bundle, (uint32)page);
        }
    }
    free(sorted); free(keys); free(ids);
    return result;
}

static void resource_render_blend_stage(uint32 accumulate)
{
    /* Both original blend operations use sf zero and lm zero */
    sint32 factor = (sint16)xport_gte_read_data(8u);
    uint32 component, flags = 0u;
    uint32 rgb = xport_gte_read_data(6u) & 0xFF000000u;
    for (component = 0u; component < 3u; ++component)
    {
        sint32 input = (sint16)xport_gte_read_data(9u + component);
        sint64 sum = (sint64)factor * input;
        sint32 mac, ir, channel;
        if (accumulate)
            sum += (sint32)xport_gte_read_data(25u + component);
        /* Signed16 products plus signed32 MAC cannot overflow signed44 */
        mac = (sint32)(uint32)sum;
        ir = mac;
        if (ir < -32768) { ir = -32768; flags |= 1u << (24u - component); }
        else if (ir > 32767) { ir = 32767; flags |= 1u << (24u - component); }
        xport_gte_write_data(25u + component, (uint32)mac);
        xport_gte_write_data(9u + component, (uint32)ir);
        channel = mac >> 4;
        if (channel < 0) { channel = 0; flags |= 1u << (21u - component); }
        else if (channel > 255) { channel = 255; flags |= 1u << (21u - component); }
        rgb |= (uint32)channel << (8u * component);
    }
    {
        uint32 previous1 = xport_gte_read_data(21u);
        uint32 previous2 = xport_gte_read_data(22u);
        xport_gte_write_data(20u, previous1);
        xport_gte_write_data(21u, previous2);
        xport_gte_write_data(22u, rgb);
    }
    xport_gte_write_control(31u, flags);
}

uint16 gtmain_color555_blend(uint16 first, uint16 second, sint16 weight)
{
    uint32 a, b, c, transparency = first >> 15;
    xport_gte_write_data(8u, 4096u - (uint32)(sint32)weight);
    xport_gte_write_data(9u, first & 31u); xport_gte_write_data(10u, (first >> 5) & 31u);
    xport_gte_write_data(11u, (first >> 10) & 31u); resource_render_blend_stage(0u);
    xport_gte_write_data(8u, (uint32)(sint32)weight);
    xport_gte_write_data(9u, second & 31u); xport_gte_write_data(10u, (second >> 5) & 31u);
    xport_gte_write_data(11u, (second >> 10) & 31u); resource_render_blend_stage(1u);
    if (weight >= 2048) transparency = second >> 15;
    a = xport_gte_read_data(25u); b = xport_gte_read_data(26u); c = xport_gte_read_data(27u);
    return (uint16)((a >> 12) | ((b >> 12) << 5) | ((c >> 12) << 10) | (transparency << 15));
}

static uint32 resource_render_timer_read(void)
{
    return (uint32)GetRCnt(1u) & 65535u;
}

uint32 gtmain_timer_random(void)
{
    uint32 a = resource_render_timer_read(), b = resource_render_timer_read(), c = resource_render_timer_read();
    uint32 result = (a & 65535u) ^ (b << 6), d = resource_render_timer_read();
    return result ^ (c << 12) ^ (d << 18);
}

static sint32 resource_render_text_emit(sint8 character)
{
    return gtmain_text_glyph((uint8)character);
}

sint32 gtmain_text_format(uint32 ordering, sint16 x, sint16 y, uint32 format, GTMainFormatArguments *arguments)
{
    gtmain_text_bind(ordering); gtmain_text_position(x, y);
    return gtmain_format_engine(resource_render_text_emit, format, arguments);
}

uint32 gtmain_car_file_environment(void)
{
    return gtmain_environment_value(0x8008E120u);
}
