#include "game_first2_registry.h"

static sint32 effect_mul(sint32 first, sint32 second)
{
    return (sint32)((uint32)first * (uint32)second);
}

static uint32 effect_pan(uint32 volume, uint32 pan)
{
    uint32 offset = pan * 2u;
    uint16 left = (uint16)((sint32)(r_u16(0x8008EA86u - offset) * volume) >> 14);
    uint16 right = (uint16)((sint32)(r_u16(0x8008E988u + offset) * volume) >> 14);
    return (uint32)left | ((uint32)right << 16);
}

/* GTMAIN.EXE 80070B5C */
sint32 gtmain_race_sound_instrument(uint32 instrument, sint32 volume, sint32 pan, sint32 priority)
{
    sint32 global_volume = (sint32)r_u32(0x80099548u);
    uint32 packed, adsr;
    uint16 pitch, duration, sample;
    uint8 selected_priority, reverb;
    if (!global_volume) return global_volume;
    volume = effect_mul(volume, global_volume) / 0x4000;
    volume = effect_mul(volume, (sint16)r_u16(instrument + 2u)) / 0x4000;
    packed = effect_pan((uint32)volume, (uint32)pan);
    pitch = (uint16)gtmain_sound_pitch_ratio((uint32)r_u8(instrument + 12u) << 8,
        r_u16(instrument + 4u));
    duration = r_u16(instrument + 6u);
    adsr = r_u32(instrument + 16u);
    sample = r_u16(instrument);
    selected_priority = priority < 0 ? r_u8(instrument + 8u) : (uint8)priority;
    reverb = r_u8(instrument + 10u);
    return (sint32)gtmain_sound_voice_start_values(0, packed, pitch, sample,
        adsr, duration, selected_priority, reverb);
}

/* GTMAIN.EXE 80070C2C */
sint32 gtmain_race_sound_instrument_pair(uint32 instrument, uint32 stereo)
{
    sint32 pan = 64;
    if (stereo) {
        gtmain_race_sound_instrument(instrument, 0x4000, 0, 0);
        instrument += 20u;
        pan = 127;
    }
    return gtmain_race_sound_instrument(instrument, 0x4000, pan, 0);
}

/* GTMAIN.EXE 80031370 */
sint32 gtmain_race_sound_effect(sint32 instrument_index, sint32 volume, sint32 pan, sint32 priority)
{
    uint32 instrument = r_u32(0x800B4A08u) + 20u * (uint32)instrument_index;
    return gtmain_race_sound_instrument(instrument, volume, pan, priority);
}

/* GTMAIN.EXE 800313A4 */
sint32 gtmain_race_sound_effect_default(sint32 instrument_index)
{
    uint32 stereo = instrument_index == 8 || instrument_index == 10;
    uint32 instrument = r_u32(0x800B4A08u) + 20u * (uint32)instrument_index;
    return gtmain_race_sound_instrument_pair(instrument, stereo);
}

/* GTMAIN.EXE 800314EC */
uint32 gtmain_race_sound_channel_reverb(uint32 channel)
{
    gtmain_sound_voice_reverb(channel + 16u, 1);
    return gtmain_sound_voice_reverb(channel + 28u, 1);
}

/* GTMAIN.EXE 80031524 */
uint32 gtmain_race_sound_car_channels_prepare(uint32 car, uint32 engine_index)
{
    uint32 offset = engine_index * 4u;
    uint32 first = car + 2120u, second = car + 2160u;
    gtmain_sound_engine_channel_init(first, r_u32(0x800B49DCu + offset), 0, 0);
    gtmain_race_sound_channel_reverb(first);
    gtmain_sound_engine_channel_init(second, r_u32(0x800B49E0u + offset), 0, 0);
    return gtmain_race_sound_channel_reverb(second);
}

/* GTMAIN.EXE 800315E8 */
uint32 gtmain_race_sound_car_init(uint32 car)
{
    uint32 first = car + 2120u, second = car + 2160u;
    gtmain_sound_engine_channel_init(first, r_u32(0x800B49DCu), 0, 0);
    gtmain_race_sound_channel_reverb(first);
    gtmain_sound_engine_channel_stop(first);
    gtmain_sound_engine_channel_init(second, r_u32(0x800B49E4u), 0, 0);
    return gtmain_race_sound_channel_reverb(second);
}

/* GTMAIN.EXE 8003169C */
sint32 gtmain_race_sound_car_stop(uint32 car)
{
    gtmain_sound_engine_channel_stop(car + 2120u);
    gtmain_sound_engine_channel_stop(car + 2160u);
    gtmain_race_sound_voice(car + 2220u, 21, 0, 64, 0, 4096);
    return gtmain_race_sound_voice(car + 2221u, 24, 0, 64, 0, 4096);
}
