#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8007BDFC */
/* Original: GTMAIN.EXE:sub_8007BDFC. */ uint32 gtmain_memory_copy(uint32 destination, uint32 source, uint32 size)
{
    uint32 output = destination;
    if (source != destination) while (size--) w_u8(output++, r_u8(source++));
    return destination;
}

/* GTMAIN.EXE 8007BE38 */
/* Original: GTMAIN.EXE:sub_8007BE38. */ uint32 gtmain_string_copy(uint32 destination, uint32 source)
{
    uint32 output = destination, offset = source & 3u;
    uint32 cursor = source - offset, word = r_u32(cursor);
    for (;;) {
        uint8 value = (uint8)(word >> (8u * offset));
        w_u8(output++, value);
        if (!value) return destination;
        if (++offset == 4u) {
            offset = 0; cursor += 4u; word = r_u32(cursor);
        }
    }
}

/* GTMAIN.EXE 8007C2B0 */
/* Original: GTMAIN.EXE:sub_8007C2B0. */ uint32 gtmain_string_length(uint32 source)
{
    uint32 cursor = source & ~3u, count = cursor - source;
    uint32 mask = ~(0xFFFFFFFFu << (8u * (source & 3u)));
    uint32 word = r_u32(cursor) | mask;
    for (;;) {
        uint32 i;
        for (i = 0; i < 4u; ++i) if (!(word & (255u << (8u * i)))) return count + i;
        cursor += 4u; count += 4u; word = r_u32(cursor);
    }
}

/* GTMAIN.EXE 8007BEC4 */
/* Original: GTMAIN.EXE:sub_8007BEC4. */ uint32 gtmain_string_append(uint32 destination, uint32 source)
{
    gtmain_string_copy(destination + gtmain_string_length(destination), source);
    return destination;
}

static uint32 gtmain_memory_read_word_unaligned(uint32 address)
{
    uint32 offset = address & 3u;
    uint32 upper = r_u32((address + 3u) & ~3u);
    uint32 lower = r_u32(address & ~3u);
    if (!offset) return lower;
    return (lower >> (8u * offset)) | (upper << (8u * (4u - offset)));
}

/* GTMAIN.EXE 8007BF04 */
/* Original: GTMAIN.EXE:sub_8007BF04. */ sint32 gtmain_string_compare(uint32 left, uint32 right)
{
    uint32 offset = left & 3u, a = left - offset, b = right - offset;
    uint32 mask = ~(0xFFFFFFFFu << (8u * offset));
    uint32 first = r_u32(a) | mask;
    uint32 second = ((left - right) & 3u ? gtmain_memory_read_word_unaligned(b) : r_u32(b)) | mask;
    for (;;) {
        uint32 i;
        for (i = 0; i < 4u; ++i) {
            uint32 x = first & (255u << (8u * i));
            uint32 y = second & (255u << (8u * i));
            if (x != y) return ((sint32)((x - y) << (8u * (3u - i))) >> 31) | 1;
            if (!x) return 0;
        }
        a += 4u; b += 4u;
        first = r_u32(a);
        second = (left - right) & 3u ? gtmain_memory_read_word_unaligned(b) : r_u32(b);
    }
}

/* GTMAIN.EXE 8007C044 */
/* Original: GTMAIN.EXE:sub_8007C044. */ uint32 gtmain_memory_move(uint32 destination, uint32 source, uint32 size)
{
    uint32 output = destination, step = 1u;
    if (source == destination) return destination;
    if (source < destination) {
        source += size - 1u; output += size - 1u; step = 0xFFFFFFFFu;
    }
    while (size--) {
        uint8 value = r_u8(source);
        source += step; w_u8(output, value); output += step;
    }
    return destination;
}

/* GTMAIN.EXE 8007C09C */
/* Original: GTMAIN.EXE:sub_8007C09C. */ sint32 gtmain_string_decimal(uint32 source)
{
    sint32 value = (sint8)r_u8(source), sign;
    uint32 result = 0;
    while (value == 9 || value == 32) value = (sint8)r_u8(++source);
    sign = value;
    if (sign == 43 || sign == 45) ++source;
    for (;;) {
        value = (sint8)r_u8(source++);
        if ((uint32)value - 48u >= 10u) break;
        result = 10u * result - 48u + (uint32)value;
    }
    return (sint32)(sign == 45 ? 0u - result : result);
}

/* GTMAIN.EXE 8007D61C */
/* Original: GTMAIN.EXE:sub_8007D61C. */ sint32 gtmain_memory_compare(uint32 left, uint32 right, uint32 size)
{
    while (size--) {
        uint8 a = r_u8(left++), b = r_u8(right++);
        if (a < b) return -1;
        if (b < a) return 1;
    }
    return 0;
}

/* GTMAIN.EXE 8007D344 */
/* Original: GTMAIN.EXE:sub_8007D344. */ uint32 gtmain_environment_find(uint32 name, uint32 length)
{
    uint32 entry = r_u32(0x800C0C34u);
    while (r_u8(entry)) {
        if ((sint8)r_u8(entry + length) == 61 && !gtmain_memory_compare(entry, name, length)) return entry;
        entry += gtmain_string_length(entry) + 1u;
    }
    return 0;
}

/* GTMAIN.EXE 8007D44C */
/* Original: GTMAIN.EXE:sub_8007D44C. */ uint32 gtmain_string_skip_field(uint32 source, sint8 delimiter)
{
    uint8 value;
    while ((value = r_u8(source)) != 0) {
        sint32 character = (sint8)value;
        ++source;
        if (character == delimiter || (!delimiter && (character == 9 || character == 32 || character == 44))) break;
    }
    return source;
}

/* GTMAIN.EXE 8007D4BC */
/* Original: GTMAIN.EXE:sub_8007D4BC. */ uint32 gtmain_string_copy_field(uint32 source, uint32 destination, sint8 delimiter)
{
    uint8 value = r_u8(source);
    while (value) {
        sint32 character = (sint8)value;
        ++source;
        if (character == delimiter || (!delimiter && (character == 9 || character == 32 || character == 44))) break;
        w_u8(destination, value);
        value = r_u8(source);
        ++destination;
    }
    w_u8(destination, 0);
    return source;
}

/* GTMAIN.EXE 8007D66C */
/* Original: GTMAIN.EXE:sub_8007D66C. */ uint32 gtmain_work_string_copy(uint32 destination, sint32 capacity)
{
    if (!capacity) { w_u32(0x800B1AB0u, 22u); return 0; }
    if ((sint32)gtmain_string_length(0x800B1B54u) >= capacity) {
        w_u32(0x800B1AB0u, 34u); return 0;
    }
    w_u32(0x800B1AB0u, 0);
    if (!destination) {
        destination = gtmain_heap_alloc((uint32)capacity);
        if (!destination) { w_u32(0x800B1AB0u, 12u); return 0; }
    }
    return gtmain_string_copy(destination, 0x800B1B54u);
}

/* GTMAIN.EXE 8007D5FC */
/* Original: GTMAIN.EXE:sub_8007D5FC. */ uint32 gtmain_work_string_copy_default(uint32 destination)
{
    return gtmain_work_string_copy(destination, 124);
}

/* GTMAIN.EXE 8007BCB4 */
/* Original: GTMAIN.EXE:sub_8007BCB4. */ uint32 gtmain_environment_value(uint32 name)
{
    uint32 length = gtmain_string_length(name);
    uint32 entry = gtmain_environment_find(name, length);
    return entry ? entry + length + 1u : 0;
}

/* GTMAIN.EXE 8007C13C */
/* Original: GTMAIN.EXE:sub_8007C13C. */ uint32 gtmain_environment_field(uint32 name, sint32 index, uint32 destination)
{
    uint32 source = gtmain_environment_value(name);
    sint32 field = 0;
    while (field < index) {
        if (!r_u8(source)) return 0;
        source = gtmain_string_skip_field(source, 44);
        ++field;
    }
    if (!r_u8(source)) return 0;
    gtmain_string_copy_field(source, destination, 44);
    return destination;
}

/* GTMAIN.EXE 8007D06C */
/* Original: GTMAIN.EXE:sub_8007D06C. */ sint32 gtmain_path_separator(sint8 character)
{
    return character == 47 || character == 92;
}

/* GTMAIN.EXE 8007D094 */
/* Original: GTMAIN.EXE:sub_8007D094. */ uint32 gtmain_path_resolve(uint32 destination, uint32 source)
{
    uint32 length, seen = 0;
    sint32 character;
    if (!gtmain_work_string_copy_default(destination)) {
        w_u32(0x800B1AB0u, 5u); return 0;
    }
    length = gtmain_string_length(destination);
    if (length != 1u) w_u8(destination + length++, 47u);
    character = (sint8)r_u8(source);
    while (character) {
        if (gtmain_path_separator((sint8)character)) {
            if (!seen) { w_u8(destination, 47u); length = 1u; }
            ++source;
        } else if (character == 46) {
            sint32 next = (sint8)r_u8(source + 1u);
            if (!next) ++source;
            else if (gtmain_path_separator((sint8)next)) source += 2u;
            else if (next == 46) {
                if (!gtmain_path_separator((sint8)r_u8(source + 2u))) break;
                while ((sint32)length >= 2) {
                    --length;
                    if (gtmain_path_separator((sint8)r_u8(destination + length - 1u))) break;
                }
                source += 3u;
            } else goto CopyComponent;
        } else {
CopyComponent:
            for (;;) {
                character = (sint8)r_u8(source);
                if (!character) break;
                if ((sint32)length >= 124) { w_u32(0x800B1AB0u, 78u); return 0; }
                ++source;
                if (character == 92) character = 47;
                w_u8(destination + length++, (uint8)character);
                if (character == 47) break;
            }
        }
        character = (sint8)r_u8(source);
        seen = 1u;
    }
    w_u8(destination + length, 0);
    w_u32(0x800B1AB0u, 0);
    return destination;
}

/* GTMAIN.EXE 8007D264 */
/* Original: GTMAIN.EXE:sub_8007D264. */ uint32 gtmain_string_last_character(uint32 source, uint8 character)
{
    uint32 result = 0, offset = source & 3u;
    uint32 cursor = source - offset, word = r_u32(cursor);
    for (;;) {
        uint8 value = (uint8)(word >> (8u * offset));
        if (value == character) result = source;
        ++source;
        if (!value) return result;
        if (++offset == 4u) { cursor += 4u; word = r_u32(cursor); offset = 0; }
    }
}

/* GTMAIN.EXE 8007D320 */
/* Original: GTMAIN.EXE:sub_8007D320. */ uint32 gtmain_environment_reset(void)
{
    w_u8(0x800C0BF0u, 0);
    w_u32(0x800B1B50u, 1u);
    return 1u;
}

/* GTMAIN.EXE 8007D33C */
/* Original: GTMAIN.EXE:sub_8007D33C. */ void gtmain_runtime_empty(void)
{
}

/* GTMAIN.EXE 8007CA94 */
/* Original: GTMAIN.EXE:sub_8007CA94. */ sint32 gtmain_format_decimal_cursor(uint32 cursor_address)
{
    uint32 cursor = r_u32(cursor_address), value = r_u8(cursor), result = 0;
    while (value - 48u < 10u) {
        sint32 character = (sint8)value;
        ++cursor;
        value = r_u8(cursor);
        result = 10u * result - 48u + (uint32)character;
    }
    w_u32(cursor_address, cursor);
    return (sint32)result;
}

/* GTMAIN.EXE 8007D3E4 */
/* Original: GTMAIN.EXE:sub_8007D3E4. */ sint32 gtmain_format_emit(sint8 character)
{
    uint32 destination = r_u32(0x800B55C8u);
    w_u32(0x800B55C8u, destination + 1u);
    w_u8(destination, (uint8)character);
    return 0;
}
