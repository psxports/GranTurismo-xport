#include "game_first2_registry.h"

static uint32 car_archive_read_guest(uint32 path, uint32 destination, uint32 requested, uint32 decoded)
{
    for (;;) {
        uint32 result = gtmain_cd_file_load(path, destination, requested, decoded);
        if (result || r_u32(0x800B1AB0u) == 12u) return result;
        gtmain_cd_file_retry_hook();
    }
}

static uint32 car_archive_size_field(uint8 output[16])
{
    uint32 source = gtmain_environment_value(0x800AD9A8u), index = 0;
    uint8 value;
    if (!r_u8(source)) return 0;
    source = gtmain_string_skip_field(source, 44);
    if (!r_u8(source)) return 0;
    value = r_u8(source);
    while (value) {
        sint32 character = (sint8)value;
        ++source;
        if (character == 44) break;
        if (index >= 15u)
            gt_draft_failure("Native car archive field extent", 0x80072250u);
        output[index] = value;
        value = r_u8(source);
        ++index;
    }
    output[index] = 0;
    return 1;
}

/* GTMAIN.EXE 80072220 */
uint32 gtmain_car_archive_path(void)
{
    return gtmain_environment_field(0x800AD9A8u, 0, 0x800B55B4u);
}

/* GTMAIN.EXE 80072250 */
sint32 gtmain_car_archive_decoded_size(void)
{
    uint8 field[16];
    uint32 index = 0, result = 0;
    sint32 character, sign;
    if (!car_archive_size_field(field)) return gtmain_string_decimal(0);
    character = (sint8)field[index];
    while (character == 9 || character == 32) character = (sint8)field[++index];
    sign = character;
    if (character == 43 || character == 45) ++index;
    for (;;) {
        character = (sint8)field[index++];
        if ((uint32)(character - 48) >= 10u) break;
        result = 10u * result - 48u + (uint32)character;
    }
    return (sint32)(sign == 45 ? 0u - result : result);
}

/* GTMAIN.EXE 80031E04 */
void gtmain_race_instrument_bank_load(uint32 destination, uint32 buffer, uint32 capacity)
{
    gtmain_sound_bank_load_original(destination, 0x2C800u, buffer, capacity, 2);
}

/* GTMAIN.EXE 8006C144 */
uint32 gtmain_car_archive_records_load(uint32 destination)
{
    uint32 path = gtmain_car_archive_path();
    uint32 decoded = (uint32)gtmain_car_archive_decoded_size();
    uint32 loaded = car_archive_read_guest(path, destination, 0, decoded);
    uint32 index;
    if (loaded) {
        for (index = 0; index < 39u; ++index) {
            uint32 offset = r_u32(loaded + 12u * index + 16u);
            gtmain_resource_section_relocate(loaded + offset);
        }
    }
    return loaded;
}
