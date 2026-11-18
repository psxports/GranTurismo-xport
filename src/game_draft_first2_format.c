#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 format_sub(sint32 a, sint32 b) { return (sint32)((uint32)a - (uint32)b); }

/* GTMAIN.EXE 8007BCFC */
/* Original: GTMAIN.EXE:sub_8007BCFC. */ uint32 gtmain_string_copy_bounded(uint32 destination, uint32 source, uint32 capacity)
{
    uint32 size = gtmain_string_length(source) + 1u;
    if (capacity < size) size = capacity;
    return gtmain_memory_copy(destination, source, size);
}

/* GTMAIN.EXE 8007C3E0 */
/* Original: GTMAIN.EXE:sub_8007C3E0. */ sint32 gtmain_format_string(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision, uint32 source)
{
    sint32 length = (sint32)gtmain_string_length(source), padding, left = 0, right = 0, i;
    uint32 count = 0;
    if (precision >= 0 && precision < length) length = precision;
    padding = format_sub(width, length);
    if (padding < 0) padding = 0;
    if (flags & 1u) right = padding; else left = padding;
    for (i = 0; i < left; ++i) { if (emit(flags & 4u ? 48 : 32) < 0) return -1; ++count; }
    for (i = 0; i < length; ++i) { if (emit((sint8)r_u8(source + (uint32)i)) < 0) return -1; ++count; }
    for (i = 0; i < right; ++i) { if (emit(32) < 0) return -1; ++count; }
    return (sint32)count;
}

static sint32 format_number_native(sint32 (*emit)(sint8), uint8 flags,
    sint32 width, sint32 precision, uint8 sign, uint32 digits, const uint8 *native_digits, sint32 length, sint32 radix)
{
    uint8 prefix[3];
    sint32 prefix_length = 0, left = 0, right = 0, zeros, space, i;
    uint32 count = 0;
    if (width < 0) width = 0;
    if (precision < 0) precision = 1;
    if (radix < 11) {
        if ((sint8)sign != 45 && (flags & 2u)) sign = 43;
        if (sign) prefix[prefix_length++] = sign;
    }
    if (flags & 16u) {
        if (radix >= 8) prefix[prefix_length++] = 48;
        if (radix >= 16) prefix[prefix_length++] = radix & 1 ? 88 : 120;
    }
    zeros = format_sub(precision, length);
    if (zeros < 0) zeros = 0;
    if (!((uint32)length | (uint32)zeros)) { left = width; prefix_length = 0; }
    else {
        space = format_sub(format_sub(width, (sint32)((uint32)prefix_length + (uint32)length)), zeros);
        if (space < 0) {
            zeros = (sint32)((uint32)zeros + (uint32)space);
            if (zeros <= 0) zeros = length == 0;
        } else if (flags & 1u) right = space;
        else if (flags & 4u) zeros = (sint32)((uint32)zeros + (uint32)space);
        else left = space;
    }
    for (i = 0; i < left; ++i) { if (emit(32) < 0) return -1; ++count; }
    for (i = 0; i < prefix_length; ++i) { if (emit((sint8)prefix[i]) < 0) return -1; ++count; }
    for (i = 0; i < zeros; ++i) { if (emit(48) < 0) return -1; ++count; }
    for (i = 0; i < length; ++i) {
        uint8 character = native_digits ? native_digits[i] : r_u8(digits + (uint32)i);
        if (emit((sint8)character) < 0) return -1;
        ++count;
    }
    for (i = 0; i < right; ++i) { if (emit(32) < 0) return -1; ++count; }
    return (sint32)count;
}

/* GTMAIN.EXE 8007C534 */
/* Original: GTMAIN.EXE:sub_8007C534. */ sint32 gtmain_format_number(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision,
    uint8 sign, uint32 digits, sint32 length, sint32 radix)
{
    return format_number_native(emit, flags, width, precision, sign, digits, NULL, length, radix);
}

/* GTMAIN.EXE 8007C7A4 */
/* Original: GTMAIN.EXE:sub_8007C7A4. */ sint32 gtmain_format_decimal(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision,
    uint32 low, uint32 high, sint8 sign, uint32 decimal_point)
{
    uint8 digits[20];
    uint32 index, length = 0, started = 0;
    sint32 point = format_sub(18, precision);
    unsigned long long value = ((unsigned long long)high << 32) | low;
    if (!decimal_point || precision < 0 || point < 0) point = 19;
    for (index = 0; index < 19u; ++index) {
        uint32 address = 0x800B1AB4u + 8u * index;
        uint32 divisor_low = r_u32(address), divisor_high = r_u32(address + 4u);
        unsigned long long divisor = ((unsigned long long)divisor_high << 32) | divisor_low;
        uint32 digit = 0;
        while (value >= divisor) { value -= divisor; ++digit; }
        if (started || digit || (sint32)index == point) {
            if ((sint32)index == point) { digits[length++] = (uint8)(digit + 48u); digits[length++] = 46u; }
            else digits[length++] = (uint8)(digit + 48u);
            started = 1;
        }
    }
    return format_number_native(emit, flags, width, precision, (uint8)sign, 0, digits, (sint32)length, 10);
}

static uint32 format_argument_peek(GTMainFormatArguments *arguments, uint32 width)
{
    uint32 index = arguments->index;
    if (arguments->words || !arguments->guest_address) {
        uint32 value;
        if (index >= arguments->count) gt_draft_failure("Native formatter argument bound", 0x8007CAE4u);
        value = arguments->words[index];
        return width == 1u ? value & 255u : width == 2u ? value & 65535u : value;
    }
    if (width == 1u) return r_u8(arguments->guest_address + 4u * index);
    if (width == 2u) return r_u16(arguments->guest_address + 4u * index);
    return r_u32(arguments->guest_address + 4u * index);
}

static uint32 format_argument(GTMainFormatArguments *arguments)
{
    uint32 value = format_argument_peek(arguments, 4u);
    ++arguments->index;
    return value;
}

static sint32 format_cursor_decimal(uint32 *cursor)
{
    uint32 position = *cursor, value = r_u8(position), result = 0;
    while (value - 48u < 10u) {
        uint32 digit = value;
        value = r_u8(++position);
        result = 10u * result - 48u + digit;
    }
    *cursor = position;
    return (sint32)result;
}

static sint32 format_octal_missing(sint32 (*emit)(sint8), uint8 flags, sint32 width,
    sint32 precision, uint32 low, uint32 high, sint8 sign)
{
    return gtmain_format_octal(emit, flags, width, precision, low, high, sign);
}

static sint32 format_hex_missing(sint32 (*emit)(sint8), uint8 flags, sint32 width,
    sint32 precision, uint32 low, uint32 high, sint8 letter)
{
    return gtmain_format_hex(emit, flags, width, precision, low, high, (uint32)(sint32)letter);
}

/* GTMAIN.EXE 8007CAE4 */
/* Original: GTMAIN.EXE:sub_8007CAE4. */ sint32 gtmain_format_engine(sint32 (*emit)(sint8), uint32 format, GTMainFormatArguments *arguments)
{
    uint32 count = 0, cursor = format;
    for (;;) {
        uint8 character = r_u8(cursor++), lookahead;
        uint32 spec_start, modifier = 0;
        uint8 flags = 0;
        sint32 width = -1, precision = -1, added = 0;
        if (!character) return (sint32)count;
        lookahead = r_u8(cursor);
        if ((sint8)character != 37 || (sint8)lookahead == 37) {
            if ((sint8)character == 37) ++cursor;
            if (emit((sint8)character)) return -1;
            ++count; continue;
        }
        spec_start = cursor;
        for (;;) {
            character = r_u8(cursor);
            if (character == 32) flags |= 8u;
            else if (character == 35) flags |= 16u;
            else if (character == 43) flags |= 2u;
            else if (character == 45) flags |= 1u;
            else if (character == 48) flags |= 4u;
            else break;
            ++cursor;
        }
        if ((uint32)character - 48u < 10u) {
            width = format_cursor_decimal(&cursor);
            character = r_u8(cursor);
        } else if ((sint8)character == 42) {
            width = (sint32)format_argument(arguments);
            character = r_u8(++cursor);
        }
        if ((sint8)character == 46) {
            character = r_u8(++cursor);
            if ((uint32)character - 48u < 10u) precision = format_cursor_decimal(&cursor);
            else if ((sint8)character == 42) { width = (sint32)format_argument(arguments); ++cursor; }
        }
        character = r_u8(cursor++);
        if ((sint8)character == 104) modifier = 1;
        else if ((sint8)character == 108) modifier = 2;
        else if ((sint8)character == 76) modifier = 3;
        else --cursor;
        character = r_u8(cursor++);
        switch (character) {
        case 'd': case 'i': case 'u': case 't': case 'o': case 'x': case 'X': {
            uint32 low, high, magnitude_low, magnitude_high;
            sint8 sign = 0;
            if (modifier == 1) {
                low = (uint32)(sint32)(sint16)format_argument_peek(arguments, 2u);
                ++arguments->index;
                high = (uint32)((sint32)low >> 31); magnitude_low = low & 65535u; magnitude_high = 0;
            } else {
                low = format_argument(arguments);
                if (modifier == 3) { high = format_argument(arguments); magnitude_low = low; magnitude_high = high; }
                else { high = (uint32)((sint32)low >> 31); magnitude_low = low; magnitude_high = 0; }
            }
            if ((sint32)high < 0 && (character == 'd' || character == 'i' || character == 't')) {
                magnitude_low = 0u - low; magnitude_high = 0u - high - (magnitude_low != 0); sign = 45;
            }
            if (character == 'o' || character == 'i')
                added = format_octal_missing(emit, flags, width, precision, magnitude_low, magnitude_high, sign);
            else if (character == 'x' || character == 'X')
                added = format_hex_missing(emit, flags, width, precision, magnitude_low, magnitude_high, (sint8)character);
            else added = gtmain_format_decimal(emit, flags, width, precision, magnitude_low, magnitude_high, sign, character == 't');
            break;
        }
        case 'E': case 'G': case 'e': case 'f': case 'g':
            arguments->index += modifier < 2 ? 1u : 2u; break;
        case 'n': case 'p': ++arguments->index; break;
        case 'c':
            added = emit((sint8)format_argument_peek(arguments, 1u));
            ++arguments->index;
            if (added < 0) return -1;
            added = 1; break;
        case 's':
            added = gtmain_format_string(emit, flags, width, precision, format_argument_peek(arguments, 4u));
            ++arguments->index; break;
        default: cursor = spec_start; break;
        }
        if (added < 0) return -1;
        count += (uint32)added;
    }
}

/* GTMAIN.EXE 8007D408 */
/* Original: GTMAIN.EXE:sub_8007D408. */ sint32 gtmain_string_format_guest(uint32 destination, uint32 format, uint32 argument_address)
{
    GTMainFormatArguments arguments = { NULL, argument_address, 0, 0 };
    sint32 result;
    w_u32(0x800B55C8u, destination);
    result = gtmain_format_engine(gtmain_format_emit, format, &arguments);
    w_u8(destination + (uint32)result, 0);
    return result;
}

/* GTMAIN.EXE 8007C014 */
/* Original: GTMAIN.EXE:sub_8007C014. */ sint32 gtmain_string_format(uint32 destination, uint32 format, const uint32 *arguments, uint32 argument_count)
{
    GTMainFormatArguments input = { arguments, 0, argument_count, 0 };
    sint32 result;
    if (!arguments && argument_count) { gt_draft_failure("Native formatter argument array", 0x8007C014u); return -1; }
    w_u32(0x800B55C8u, destination);
    result = gtmain_format_engine(gtmain_format_emit, format, &input);
    w_u8(destination + (uint32)result, 0);
    return result;
}
