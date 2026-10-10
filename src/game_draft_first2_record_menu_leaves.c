#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8002E208 */
void gtmain_race_timing_flag_clear(void)
{
    w_u16(0x8009071Au, 0u);
}

/* GTMAIN.EXE 8003B91C */
uint32 gtmain_menu_state_capture(uint32 state)
{
    uint32 value = r_u8(state + 1u);
    w_u8(state + 12u, value);
    return value;
}

/* GTMAIN.EXE 8003C314 */
void gtmain_menu_resource_request_initialize(uint32 request, sint16 height, sint16 width, uint32 output)
{
    w_u8(output, 0u);
    w_u8(request, 0u);
    w_u8(request + 1u, 0u);
    w_u32(request + 4u, output);
    w_u8(request + 12u, 0u);
    w_u8(request + 13u, 0u);
    w_u16(request + 10u, (uint16)height);
    w_u16(request + 8u, (uint16)width);
}

/* GTMAIN.EXE 8003C37C */
sint32 gtmain_menu_glyph_index(sint8 column, sint8 row)
{
    if (row >= 6) return -1;
    return (sint32)row * 16 + (sint32)column + 32;
}

/* GTMAIN.EXE 8002CE68 */
sint32 gtmain_race_pause_menu_measure(void)
{
    uint32 selectors = r_u32(0x8008FBF4u);
    uint32 maximum = 0u, count = 0u, selector, width, height;
    sint32 x, y;
    w_u8(0x800B63D8u, 0u);
    w_u8(0x8008FBEFu, 0u);
    w_u8(0x8008FBF0u, 0u);
    w_u8(0x8008FBF1u, 0u);
    selector = r_u8(selectors);
    while (selector)
    {
        uint32 length = gtmain_string_length(r_u32(0x8008FBF8u + selector * 4u));
        if ((sint32)maximum < (sint32)length) maximum = length - 1u;
        selectors = r_u32(0x8008FBF4u);
        ++count;
        selector = r_u8(selectors + count);
    }
    width = maximum * 9u + 4u;
    height = count * 10u + 4u;
    w_u16(0x800B3998u, (uint16)width);
    x = (320 - (sint16)width) >> 1;
    w_u16(0x800B399Au, (uint16)height);
    w_u16(0x800B3994u, (uint16)x);
    y = (240 - (sint16)height) >> 1;
    w_u8(0x8008FBF2u, count);
    w_u16(0x800B3996u, (uint16)y);
    return y;
}

/* GTMAIN.EXE 8002D81C */
void gtmain_race_recorder_stop(uint32 object)
{
    uint32 previous;
    sint32 state;
    if (!object) return;
    previous = r_u16(object + 12u);
    w_u16(object + 12u, 1);
    state = r_s32(object + 8u);
    if (!previous && state >= 0) gtmain_race_input_record_flush(object);
}

/* GTMAIN.EXE 80020534 */
uint32 gtmain_race_recorders_stop(void)
{
    sint32 count = r_s16(0x800BC1A6u), index = 0;
    uint32 result;
    if (count <= 0) return (uint32)count;
    do {
        uint32 car = 0x800B6850u + 3380u * (uint32)index;
        sint32 kind = r_s16(car + 24u);
        if (kind < 4 && kind >= 2) gtmain_race_recorder_stop(r_u32(car + 28u));
        count = r_s16(0x800BC1A6u);
        ++index;
        result = 2u * (uint32)index;
    } while (index < count);
    return result;
}

/* GTMAIN.EXE 800207D0 */
uint32 gtmain_race_points_save(void)
{
    uint32 result = r_u32(0x800CB7F4u) & 0x00FFFF00u;
    sint32 index, count;
    if (result != 0x00010300u) return result;
    count = r_s16(0x800B6666u);
    for (index = 0; index < count; ++index)
        w_u16(0x800B6840u + 2u * (uint32)index, r_u16(0x800B6850u + 3380u * (uint32)index + 38u));
    return (uint32)gtmain_race_points_print(0x8008B9A4u);
}

/* GTMAIN.EXE 800205D0 */
uint32 gtmain_race_session_save(void)
{
    if (r_u32(0x800B630Cu) == 1u) return 1u;
    gtmain_race_recorders_stop();
    gtmain_car_configuration_export();
    return gtmain_race_points_save();
}

/* GTMAIN.EXE 8002CB28 */
uint32 gtmain_race_record_size(uint32 record)
{
    return (r_u16(record + 844u) + 854u) & 0xFFFFFFFCu;
}

/* GTMAIN.EXE 8002CB98 */
uint32 gtmain_race_record_copy(uint32 destination, uint32 source, uint32 size)
{
    sub_8007BD58(destination, 0u, 9044u);
    return gtmain_memory_copy(destination, source, size);
}

/* GTMAIN.EXE 8002CBEC */
void gtmain_race_record_publish(uint32 source)
{
    uint32 size = gtmain_race_record_size(source);
    gtmain_race_record_copy(0x800CDC34u, source, size);
    gtmain_race_cars_invalidate();
}

/* GTMAIN.EXE 800484F8 */
sint32 gtmain_menu_resource_status(uint32 mode)
{
    sint32 result;
    result = gtmain_menu_name_edit(0x80092EACu, mode);
    if (result == -2) return -2;
    return result == 0 ? 0 : -1;
}

/* GTMAIN.EXE 8004A2A4 */
uint32 gtmain_menu_entry_set(uint32 owner, uint32 index, sint16 x, sint16 y, uint32 text, sint16 height,
    uint8 first, uint8 second, uint32 resource, uint8 kind, uint8 state, uint32 extra)
{
    uint32 entry = owner + 512u + 52u * index;
    w_u16(entry + 40u, (uint16)x); w_u16(entry + 42u, (uint16)y); w_u16(entry + 44u, (uint16)height);
    w_u8(entry + 48u, first); w_u8(entry + 49u, second); w_u32(entry + 52u, resource);
    w_u8(entry + 46u, kind); w_u8(entry + 47u, state); w_u32(entry + 56u, extra);
    return gtmain_string_copy(entry + 8u, text);
}
