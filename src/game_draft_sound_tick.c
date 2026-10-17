#include "game_draft_registry.h"
#include "game_first2_registry.h"


static uint32 gtmain_sound_pan_pair_native(uint32 volume, uint32 pan)
{
    uint32 left = r_u16(0x8008EA86u - 2u * pan) * volume;
    uint32 right = r_u16(0x8008E988u + 2u * pan) * volume;
    return (uint32)(uint16)((sint32)left >> 14) |
        ((uint32)(uint16)((sint32)right >> 14) << 16);
}

static void gtmain_sound_voice_volume_native(uint32 owner, uint32 packed_volume)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice;
    uint8 state;
    if (index < 0) return;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    if (r_u32(voice + 20u) == owner) {
        uint8 flags;
        w_u8(voice + 4u, 2u);
        flags = r_u8(voice + 5u);
        w_u32(voice + 8u, packed_volume);
        w_u8(voice + 5u, flags | 2u);
    }
    w_u8(voice + 4u, state);
}

/* GTMAIN.EXE 8007DE44 */
uint32 gtmain_sound_voice_tick(void)
{
    uint32 update = 0u, off = 0u, on = 0u, i, voice, bit;
    uint32 result = 0x800B55D4u;
    w_u32(0x800B1BD4u, r_u32(0x800B1BD4u) + 1u);
    for (i = 0u, bit = 1u; i < 24u; ++i, bit <<= 1) {
        voice = 0x800B55D4u + i * 40u;
        if (r_u8(voice + 4u) != 2u) {
            uint32 finished = 0u;
            if (r_u8(voice + 1u) && !r_u8(voice + 6u)) {
                if (!r_u16(0x1F801C0Cu + i * 16u)) finished = 1u;
                else {
                    uint32 length = r_u32(voice + 28u);
                    uint32 elapsed = r_u32(voice + 32u);
                    finished = length < elapsed;
                }
            }
            if (finished) {
                uint32 kind = r_u8(voice);
                w_u8(voice + 1u, 0u);
                w_u8(voice + 7u, 1u);
                if (kind == 1u) {
                    uint32 handle = r_u32(voice + 20u);
                    if (handle) w_u8(handle, 0xFFu);
                    w_u32(voice + 20u, 0u);
                    w_u8(voice, 0u);
                }
            }
            if (r_u8(voice + 7u)) {
                if (r_u8(voice + 2u)) {
                    off |= bit;
                    w_u8(voice + 2u, 0u);
                }
                w_u8(voice + 7u, 0u);
            }
            {
                uint32 dirty = r_u8(voice + 5u);
                uint32 pending = r_u8(voice + 6u);
                if (dirty) update |= bit;
                if (pending) {
                    on |= bit;
                    if (r_u8(voice + 2u)) off |= bit;
                    w_u8(voice + 2u, 1u);
                    w_u8(voice + 1u, 1u);
                    w_u8(voice + 6u, 0u);
                }
            }
        }
        if (r_u8(voice + 1u)) {
            uint32 ticks = r_u32(voice + 24u);
            uint32 length = r_u32(voice + 28u);
            w_u32(voice + 24u, ticks + 1u);
            if (length) {
                uint32 pitch = r_u16(0x1F801C04u + i * 16u);
                uint32 elapsed = r_u32(voice + 32u);
                w_u32(voice + 32u, elapsed + pitch);
            }
        }
    }
    if (off) {
        w_u16(0x1F801D8Cu, off);
        w_u16(0x1F801D8Eu, off >> 16);
    }
    for (i = 0u; update; ++i, update >>= 1) {
        voice = 0x800B55D4u + i * 40u;
        if (update & 1u) {
            uint32 flags = r_u8(voice + 5u);
            uint32 registers = 0x1F801C00u + i * 16u;
            w_u8(voice + 5u, 0u);
            if (flags & 1u) w_u16(registers + 4u, r_u16(voice + 12u));
            if (flags & 2u) {
                uint32 left = r_u16(voice + 8u) & 0x7FFFu;
                uint32 right = r_u16(voice + 10u) & 0x7FFFu;
                w_u16(registers, left);
                w_u16(registers + 2u, right);
            }
            if (flags & 4u) {
                uint32 adsr1 = r_u16(voice + 16u), adsr2 = r_u16(voice + 18u), start = r_u16(voice + 14u);
                w_u16(registers + 8u, adsr1);
                w_u16(registers + 10u, adsr2);
                w_u16(registers + 6u, start);
            }
        }
    }
    if (on) {
        w_u16(0x1F801D88u, on);
        w_u16(0x1F801D8Au, on >> 16);
    }
    if (r_u8(0x800B55CEu)) {
        uint32 mask = 0u;
        for (i = 0u, bit = 1u; i < 24u; ++i, bit <<= 1)
            if (r_u8(0x800B55D4u + i * 40u + 36u)) mask |= bit;
        w_u16(0x1F801D98u, mask);
        w_u16(0x1F801D9Au, mask >> 16);
        w_u8(0x800B55CEu, 0u);
        result += 24u * 40u;
    }
    return result;
}

static void sound_sequence_events(uint32 node)
{
    /* TODO Bind unselected sequence event handler 8007E3F8 */
    (void)gt_draft_unresolved_call(0x8007E3F8u, node, 0, 0, 0, 0, 0, 0, 0);
}

static uint32 sound_sequence_delta(uint32 ramp)
{
    /* TODO Bind unselected control ramp 8007F39C */
    return gt_draft_unresolved_call(0x8007F39Cu, ramp, 0, 0, 0, 0, 0, 0, 0);
}

static void sound_sequence_volume(uint32 node)
{
    /* TODO Bind unselected channel volume update 8007F4A4 */
    (void)gt_draft_unresolved_call(0x8007F4A4u, node, 0, 0, 0, 0, 0, 0, 0);
}

/* GTMAIN.EXE 8007E6E8 */
void gtmain_sound_sequence_tick(void)
{
    uint32 node;
    if (!r_u32(0x800B1BE4u) || !r_u32(0x800B1BF4u)) return;
    node = r_u32(0x800B599Cu);
    while (node) {
        uint32 time = r_u32(node + 12u);
        w_u32(node + 20u, 0u);
        if ((sint32)time > 0) {
            uint32 step = r_u32(node + 8u);
            do {
                uint32 ticks = r_u32(node + 20u);
                time -= step;
                w_u32(node + 20u, ticks + 1u);
                w_u32(node + 12u, time);
            } while ((sint32)time > 0);
        }
        w_u32(node + 12u, time + r_u32(node + 16u));
        sound_sequence_events(node);
        if (r_u16(node + 28u)) {
            uint32 delta = sound_sequence_delta(node + 28u);
            w_u16(node + 4u, (uint16)(r_u16(node + 4u) + delta));
            sound_sequence_volume(node);
        }
        node = r_u32(node);
    }
    if (r_u16(0x800B59A8u)) {
        uint32 delta = sound_sequence_delta(0x800B59A8u);
        w_u16(0x800B59A2u, (uint16)(r_u16(0x800B59A2u) + delta));
        node = r_u32(0x800B599Cu);
        while (node) {
            sound_sequence_volume(node);
            node = r_u32(node);
        }
    }
    gtmain_sound_sequence_voices();
}

/* GTMAIN.EXE 8007E81C */
void gtmain_sound_sequence_voices(void)
{
    uint32 i;
    for (i = 0u; i < 25u; ++i) {
        uint32 voice = 0x800B59B4u + i * 28u;
        uint32 owner, channel, instrument, volume, changed;
        sint32 bend, cached_bend;
        uint32 remaining;
        if ((sint8)r_u8(voice + 27u) < 0) continue;
        owner = r_u32(voice);
        if (!owner) {
            gtmain_sound_voice_release(voice + 27u);
            gtmain_sound_voice_detach(voice + 27u);
            continue;
        }
        remaining = r_u32(voice + 16u);
        if ((sint32)remaining > 0) {
            remaining -= r_u32(owner + 20u);
            w_u32(voice + 16u, remaining);
            if ((sint32)remaining <= 0)
                gtmain_sound_voice_release(voice + 27u);
        }
        channel = r_u32(voice + 4u);
        cached_bend = (sint16)r_u16(voice + 22u);
        bend = (sint16)r_u16(channel + 12u);
        if (cached_bend != bend) {
            uint32 pitch, multiplier, argument;
            instrument = r_u32(voice + 12u);
            multiplier = r_u8(instrument + (bend > 0 ? 15u : 14u));
            argument = (r_u8(voice + 24u) << 8) + (uint32)((sint32)(multiplier * (uint32)bend) >> 5);
            pitch = gtmain_sound_pitch_ratio(argument, r_u16(instrument + 4u));
            gtmain_sound_voice_pitch(voice + 27u, (sint16)pitch);
            channel = r_u32(voice + 4u);
            w_u16(voice + 22u, r_u16(channel + 12u));
        }
        volume = r_u16(channel + 10u);
        changed = r_u16(voice + 20u) != volume;
        if (!changed) {
            uint32 cached_pan = r_u8(voice + 26u);
            uint32 current_pan = r_u8(channel + 16u);
            changed = cached_pan != current_pan;
        }
        if (changed) {
            uint32 patch = r_u32(voice + 8u);
            uint32 velocity = r_u8(voice + 25u);
            uint32 patch_level = (uint32)(sint32)(sint16)r_u16(patch + 2u);
            uint32 coefficient = r_u16(0x8008E888u + velocity * 2u);
            uint32 level, packed_volume, channel_pan, instrument_level, instrument_pan, patch_pan;
            sint32 pan;
            instrument = r_u32(voice + 12u);
            channel_pan = r_u8(channel + 16u);
            instrument_level = (uint32)(sint32)(sint16)r_u16(instrument + 2u);
            instrument_pan = r_u8(instrument + 9u);
            patch_pan = r_u8(patch + 1u);
            pan = (sint32)(instrument_pan + channel_pan + patch_pan) - 128;
            if (pan < 0) pan = 0;
            else if (pan >= 128) pan = 127;
            level = (uint32)((sint32)(volume * patch_level) >> 14);
            level = (uint32)((sint32)(coefficient * level) >> 14);
            level = (uint32)((sint32)(instrument_level * level) >> 14);
            packed_volume = gtmain_sound_pan_pair_native(level, (uint32)pan);
            gtmain_sound_voice_volume_native(voice + 27u, packed_volume);
            channel = r_u32(voice + 4u);
            w_u16(voice + 20u, r_u16(channel + 10u));
            w_u8(voice + 26u, r_u8(channel + 16u));
        }
    }
}
