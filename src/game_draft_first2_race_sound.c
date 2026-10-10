#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_spu.h"

static sint32 race_mul(sint32 a, sint32 b)
{
    return (sint32)((uint32)a * (uint32)b);
}

static uint32 race_pan_pair(uint32 volume, uint32 pan)
{
    uint32 offset = pan * 2u;
    uint16 left = (uint16)((sint32)(r_u16(0x8008EA86u - offset) * volume) >> 14);
    uint16 right = (uint16)((sint32)(r_u16(0x8008E988u + offset) * volume) >> 14);
    return (uint32)left | ((uint32)right << 16);
}

/* GTMAIN.EXE 800311E4 */
sint32 gtmain_race_sound_voice(uint32 owner, sint32 instrument_index, sint32 volume,
    sint32 pan, sint32 priority, sint32 pitch_scale)
{
    uint32 result = r_u8(0x800CB768u);
    uint32 instrument, packed;
    sint32 value, pitch;
    if (!result) return (sint32)result;
    if (!volume) {
        result = (uint32)(sint32)(sint8)r_u8(owner);
        if ((sint32)result >= 0) {
            gtmain_sound_voice_release(owner);
            result = gtmain_sound_voice_detach(owner);
        }
        return (sint32)result;
    }
    instrument = r_u32(0x800B4A08u) + 20u * (uint32)instrument_index;
    value = race_mul(volume, (sint32)result + 1) / 256;
    if (value >= 0x4000) value = 0x3FFF;
    value = race_mul(value, (sint16)r_u16(instrument + 2u)) / 0x4000;
    packed = race_pan_pair((uint32)value, (uint32)pan);
    value = (uint16)gtmain_sound_pitch_ratio((uint32)r_u8(instrument + 12u) << 8,
        r_u16(instrument + 4u));
    pitch = race_mul(value, pitch_scale) / 4096;
    if ((sint8)r_u8(owner) < 0) {
        uint16 duration = r_u16(instrument + 6u);
        uint32 adsr = r_u32(instrument + 16u);
        uint16 sample = r_u16(instrument);
        uint8 selected_priority = priority < 0 ? r_u8(instrument + 8u) : (uint8)priority;
        uint8 reverb = r_u8(instrument + 10u);
        return (sint32)gtmain_sound_voice_start_values(owner, packed, (uint16)pitch,
            sample, adsr, duration, selected_priority, reverb);
    }
    gtmain_sound_voice_pitch(owner, (sint16)(uint16)pitch);
    return (sint32)gtmain_sound_voice_volume_value(owner, packed);
}

/* GTMAIN.EXE 8003173C */
void gtmain_race_sound_car(uint32 scratch, uint32 car, uint32 car_index, uint32 priority_base)
{
    sint32 global_volume = (sint32)r_u32(0x80099548u);
    sint32 volume = race_mul((sint32)r_u32(car + 2204u), global_volume) / 0x4000;
    uint32 channel1 = car + 2120u, channel2 = car + 2160u;
    uint32 mode = r_u32(0x800B630Cu);
    uint32 special = (uint32)((sint32)(sint16)r_u16(car + 26u) - 1) < 2u;
    sint32 pitch_scale, pan = 64, first_volume, second_volume, effect_volume;
    sint16 sound_id;
    if (!volume) return;
    if (!special || mode == 1u) {
        uint32 listener = r_u32(0x800B6638u);
        sint32 shift, length, distance, dot, velocity, denominator, numerator, baseline;
        shift = gtmain_vector_narrow(psx_addr(scratch, 6u), psx_addr(car + 2208u, 12u));
        length = gtmain_vector_normalize_xyz(psx_addr(scratch, 6u), psx_addr(scratch, 6u));
        distance = (shift < 0 ? length >> ((uint32)shift & 31u) :
            (sint32)((uint32)length << ((uint32)shift & 31u))) >> 4;
        shift = gtmain_vector_narrow(psx_addr(scratch + 8u, 6u), psx_addr(car + 2320u, 12u));
        length = gtmain_vector_normalize_xyz(psx_addr(scratch + 8u, 6u), psx_addr(scratch + 8u, 6u));
        dot = gtmain_light_pair_transform(psx_addr(scratch, 8u), psx_addr(scratch + 8u, 8u));
        length = (shift < 0 ? length >> ((uint32)shift & 31u) :
            (sint32)((uint32)length << ((uint32)shift & 31u))) >> 4;
        velocity = race_mul(length, dot) >> 12;
        length = (sint32)r_u32(listener + 240u);
        dot = gtmain_light_pair_transform(psx_addr(scratch, 8u), psx_addr(listener + 232u, 8u));
        baseline = r_u32(0x800B6318u) == 2u ? 46420 : 23210;
        numerator = (sint32)(((uint32)baseline + (uint32)(race_mul(length, dot) >> 12)) << 12);
        denominator = (sint32)((uint32)baseline + (uint32)velocity);
        if (!denominator) { gt_draft_exception(7u); return; }
        if (denominator == -1 && numerator == (sint32)0x80000000u) {
            gt_draft_exception(6u); return;
        }
        pitch_scale = numerator / denominator;
        gtmain_matrix_rotate_short(psx_addr(scratch, 8u), psx_addr(listener + 188u, 20u), psx_addr(scratch, 8u));
        w_u16(scratch + 16u, r_u16(scratch));
        w_u16(scratch + 18u, r_u16(scratch + 4u));
        gtmain_vector_normalize_xy(psx_addr(scratch + 16u, 4u), psx_addr(scratch + 16u, 4u));
        pan = (sint16)r_u16(scratch + 16u);
        if (distance <= 0x7FFF) pan = (race_mul(pan, distance >> 3) >> 12) >> 5;
        else pan >>= 5;
        pan += 64;
        if (pan < 0) pan = 0;
        if (pan >= 128) pan = 127;
    } else {
        pitch_scale = 4096;
        volume = global_volume;
    }
    w_u16(channel1 + 10u, (uint16)pitch_scale);
    w_u16(channel2 + 10u, (uint16)pitch_scale);
    switch (r_u16(car + 2224u)) {
    case 0:
        first_volume = race_mul(volume, 1228) >> 12;
        second_volume = race_mul(volume, 2457) >> 12;
        effect_volume = race_mul(volume, 737) >> 12;
        break;
    case 1:
        first_volume = race_mul(global_volume, 1720) >> 12;
        second_volume = race_mul(global_volume, 2211) >> 12;
        effect_volume = first_volume;
        break;
    case 2:
        first_volume = race_mul(global_volume, 2457) >> 12;
        second_volume = race_mul(global_volume, 1228) >> 12;
        effect_volume = first_volume;
        break;
    default:
        first_volume = second_volume = effect_volume = volume;
        break;
    }
    sound_id = (sint16)r_u16(car + 78u);
    if (special) {
        sint32 scaled;
        sint64 product;
        w_u16(channel1 + 8u, first_volume ? (uint16)sound_id : 0);
        gtmain_sound_pan(channel1 + 12u, (uint32)first_volume, (uint32)pan);
        gtmain_sound_engine_channel_priority(channel1, (uint8)(priority_base + 1u));
        gtmain_sound_engine_channel_update(channel1);
        product = (sint64)((uint32)r_u16(car + 88u) << 12) * 715827883;
        scaled = (sint32)(product >> 32) >> 10;
        scaled = race_mul(scaled, pitch_scale) >> 12;
        if (!scaled) effect_volume = 0;
        else if (scaled < 4096) effect_volume = race_mul(effect_volume, scaled) >> 12;
        gtmain_race_sound_voice(car + 2221u, 24, effect_volume, pan,
            (sint32)(priority_base + 2u), scaled);
    }
    w_u16(channel2 + 8u, second_volume ? (uint16)sound_id : 0);
    gtmain_sound_pan(channel2 + 12u, (uint32)second_volume, (uint32)pan);
    gtmain_sound_engine_channel_priority(channel2, (uint8)priority_base);
    gtmain_sound_engine_channel_update(channel2);
    if (special || mode == 1u) {
        sint32 scaled = (sint32)(((uint32)volume * 3u) << 10) >> 12;
        scaled = race_mul((sint32)r_u16(car + 2222u), scaled) / 0x4000;
        gtmain_race_sound_voice(car + 2220u, 21, scaled, pan,
            (sint32)(priority_base + 4u), pitch_scale);
    }
    if (!mode && (sint32)(sint16)r_u16(car + 74u) == (sint32)r_u32(0x800BF518u)) {
        sint32 scaled = (sint32)(sint16)r_u16(car + 82u) * 8;
        if (scaled >= 4096) scaled = 4096;
        gt_draft_80031370(13u, (uint32)(race_mul(volume, scaled) >> 12),
            (uint32)pan, priority_base + 3u, 0, 0, 0, 0);
    }
}

/* GTMAIN.EXE 80031CD8 */
void gtmain_race_sound_update(void)
{
    uint32 car = r_u32(0x8008F370u), index = 0, priority = 8;
    while (car) {
        gtmain_race_sound_car(0x1F800000u, car, index, priority);
        car = r_u32(car);
        ++index;
        priority += 5u;
    }
}

/* GTMAIN.EXE 80031D48 */
uint32 gtmain_race_sound_volume_apply(void)
{
    uint32 value;
    w_u32(0x80099548u, ((uint32)r_u8(0x800CB768u) << 14) / 255u);
    w_u16(0x800994BCu, (uint16)(((uint32)r_u8(0x800CB767u) * 32767u) / 255u));
    if (!spu_mmio_read(NULL, 0x1F801DAAu, 2u, &value))
        gt_draft_failure("Native SPU control read", 0x80031D48u);
    value = (value & 0xFFFBu) | 1u;
    if (!spu_mmio_write(NULL, 0x1F801DAAu, 2u, value, 3u))
        gt_draft_failure("Native SPU control write", 0x80031D48u);
    return value;
}

/* GTMAIN.EXE 8006B530 */
sint32 gtmain_vector_normalize_xyz(uint8 *out, const uint8 *input)
{
    sint32 x = (sint16)xport_load_le16(input);
    sint32 y = (sint16)xport_load_le16(input + 2u);
    sint32 z = (sint16)xport_load_le16(input + 4u);
    sint32 length = 0;
    if (x | y | z) {
        uint32 squared = (uint32)x * (uint32)x + (uint32)y * (uint32)y + (uint32)z * (uint32)z;
        sint32 numerator;
        length = (sint32)gtmain_fixed_sqrt(squared, 0);
        numerator = (sint32)((uint32)x << 12);
        if (!length) gt_draft_exception(7u);
        if (length == -1 && numerator == (sint32)0x80000000u) {
            gt_draft_exception(6u);
        }
        x = numerator / length;
        numerator = (sint32)((uint32)y << 12);
        if (!length) gt_draft_exception(7u);
        if (length == -1 && numerator == (sint32)0x80000000u) {
            gt_draft_exception(6u);
        }
        y = numerator / length;
        numerator = (sint32)((uint32)z << 12);
        if (!length) gt_draft_exception(7u);
        if (length == -1 && numerator == (sint32)0x80000000u) {
            gt_draft_exception(6u);
        }
        z = numerator / length;
    }
    xport_store_le16(out, (uint16)x);
    xport_store_le16(out + 2u, (uint16)y);
    xport_store_le16(out + 4u, (uint16)z);
    return length;
}
