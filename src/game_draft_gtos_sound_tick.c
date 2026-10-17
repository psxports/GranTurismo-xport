#include "game_draft_registry.h"

static void gtos_voice_mask(uint32 address, uint32 mask)
{
    w_u16(address, mask);
    w_u16(address + 2u, mask >> 16u);
}

void gtos_voice_tick(void)
{
    uint32 i, changes = 0u, off = 0u, on = 0u;
    w_u32(0x80073724u, r_u32(0x80073724u) + 1u);
    for (i = 0u; i < 24u; ++i) {
        uint32 row = 0x80076364u + i * 40u, bit = 1u << i;
        uint32 registers = 0x1F801C00u + i * 16u;
        if (r_u8(row + 4u) != 2u) {
            if (r_u8(row + 1u) && !r_u8(row + 6u) &&
                (!r_u16(registers + 12u) || r_u32(row + 28u) < r_u32(row + 32u))) {
                w_u8(row + 1u, 0u);
                w_u8(row + 7u, 1u);
                if (r_u8(row) == 1u) {
                    uint32 owner = r_u32(row + 20u);
                    if (owner) w_u8(owner, 0xFFu);
                    w_u32(row + 20u, 0u);
                    w_u8(row, 0u);
                }
            }
            if (r_u8(row + 7u)) {
                if (r_u8(row + 2u)) {
                    off |= bit;
                    w_u8(row + 2u, 0u);
                }
                w_u8(row + 7u, 0u);
            }
            if (r_u8(row + 5u)) changes |= bit;
            if (r_u8(row + 6u)) {
                on |= bit;
                if (r_u8(row + 2u)) off |= bit;
                w_u8(row + 2u, 1u);
                w_u8(row + 1u, 1u);
                w_u8(row + 6u, 0u);
            }
        }
        if (r_u8(row + 1u)) {
            w_u32(row + 24u, r_u32(row + 24u) + 1u);
            if (r_u32(row + 28u))
                w_u32(row + 32u, r_u32(row + 32u) + r_u16(registers + 4u));
        }
    }
    if (off) gtos_voice_mask(0x1F801D8Cu, off);
    for (i = 0u; changes; ++i, changes >>= 1u) {
        if (changes & 1u) {
            uint32 row = 0x80076364u + i * 40u, flags = r_u8(row + 5u);
            uint32 registers = 0x1F801C00u + i * 16u;
            w_u8(row + 5u, 0u);
            if (flags & 1u) w_u16(registers + 4u, r_u16(row + 12u));
            if (flags & 2u) {
                w_u16(registers, r_u16(row + 8u) & 0x7FFFu);
                w_u16(registers + 2u, r_u16(row + 10u) & 0x7FFFu);
            }
            if (flags & 4u) {
                w_u16(registers + 8u, r_u16(row + 16u));
                w_u16(registers + 10u, r_u16(row + 18u));
                w_u16(registers + 6u, r_u16(row + 14u));
            }
        }
    }
    if (on) gtos_voice_mask(0x1F801D88u, on);
    if (r_u8(0x8007635Eu)) {
        uint32 mask = 0u;
        for (i = 0u; i < 24u; ++i)
            if (r_u8(0x80076364u + i * 40u + 36u)) mask |= 1u << i;
        gtos_voice_mask(0x1F801D98u, mask);
        w_u8(0x8007635Eu, 0u);
    }
}

static sint32 gtos_sound_fade_step(uint32 fade)
{
    sint32 increment, correction, step;
    uint32 remaining = r_u16(fade);
    if (!remaining) return 0;
    correction = (sint16)r_u16(fade + 4u);
    increment = (sint16)r_u16(fade + 2u);
    w_u16(fade, remaining - 1u);
    if (correction) {
        step = (sint16)r_u16(fade + 6u);
        w_u16(fade + 10u, (uint16)((sint16)r_u16(fade + 10u) - step));
        if (step < 0) {
            w_u16(fade + 10u, (uint16)((sint16)r_u16(fade + 10u) + (sint16)r_u16(fade + 8u)));
            increment += correction;
        }
    }
    return increment;
}

static void gtos_sound_sequence_volume(uint32 sequence)
{
    uint32 i, product = (uint32)r_u16(0x80076732u) * r_u16(sequence + 4u);
    uint32 level = (uint32)((sint32)product >> 14u);
    w_u16(sequence + 6u, (uint16)level);
    for (i = 0u; i < 16u; ++i) {
        uint32 row = sequence + 40u + 40u * i;
        w_u16(row + 10u, (uint16)(((uint32)r_u16(row + 8u) * level) >> 14u));
    }
}

static sint32 gtos_voice_allocate(uint32 priority);

static uint16 gtos_sound_note_pitch(sint32 note, uint32 base)
{
    uint32 difference = (uint32)note - base, value;
    sint32 octave = -2;
    while ((sint32)difference < 0) { difference += 3072u; --octave; }
    while ((sint32)difference >= 3072) { difference -= 3072u; ++octave; }
    value = r_u16(0x8004CBB8u + 2u * (difference >> 3u));
    return (uint16)(octave > 0 ? value << ((uint32)octave & 31u) : value >> ((0u - (uint32)octave) & 31u));
}

static void gtos_sound_note(uint32 sequence, uint32 track, uint32 note, uint32 velocity, uint32 duration)
{
    uint32 bank = r_u32(sequence + 24u);
    uint32 instrument = r_u32(r_u32(bank + 36u) + 4u * r_u8(track + 17u));
    uint32 layer, product;
    sint32 level, pan_base;
    product = (uint32)r_u16(track + 10u) * (uint32)(sint32)(sint16)r_u16(instrument + 2u);
    level = (sint32)product >> 14u;
    product = (uint32)r_u16(0x8004CEB8u + 2u * velocity) * (uint32)level;
    level = (sint32)product >> 14u;
    pan_base = r_u8(track + 16u) + r_u8(instrument + 1u);
    for (layer = 0u; layer < r_u8(instrument); ++layer) {
        uint32 sample = r_u32(bank + 28u) + 20u * r_u8(instrument + 4u + layer);
        uint32 scan, slot = r_u8(0x80076734u), voice = 0u;
        sint32 pan, bend, pitch, volume;
        uint16 left, right;
        if ((sint32)note < r_u8(sample + 12u) || (sint32)note > r_u8(sample + 13u)) continue;
        for (scan = 0u; scan < 25u; ++scan) {
            slot = slot == 24u ? 0u : slot + 1u;
            voice = 0x80076744u + 28u * slot;
            if ((sint8)r_u8(voice + 27u) < 0) break;
        }
        if (scan == 25u) continue;
        product = (uint32)(sint32)(sint16)r_u16(sample + 2u) * (uint32)level;
        volume = (sint32)product >> 14u;
        pan = pan_base + r_u8(sample + 9u) - 128;
        if (pan < 0) pan = 0;
        else if (pan >= 128) pan = 127;
        bend = (sint16)r_u16(track + 12u);
        w_u8(0x80076734u, slot);
        w_u32(voice, sequence); w_u32(voice + 4u, track);
        w_u32(voice + 8u, instrument); w_u32(voice + 12u, sample);
        w_u32(voice + 16u, duration + r_u32(sequence + 20u));
        w_u16(voice + 22u, (uint16)bend);
        w_u8(voice + 24u, note); w_u8(voice + 25u, velocity);
        w_u16(voice + 20u, r_u16(track + 10u));
        w_u8(voice + 26u, r_u8(track + 16u));
        pitch = (sint32)(note << 8u);
        if (bend) pitch += (bend * r_u8(sample + (bend > 0 ? 15u : 14u))) >> 5u;
        product = r_u16(0x8004D0B6u - 2u * (uint32)pan) * (uint32)volume;
        left = (uint16)((sint32)product >> 14u);
        product = r_u16(0x8004CFB8u + 2u * (uint32)pan) * (uint32)volume;
        right = (uint16)((sint32)product >> 14u);
        {
            sint32 allocated = gtos_voice_allocate(r_u8(sample + 8u));
            w_u8(voice + 27u, (uint8)allocated);
            if (allocated >= 0) {
                uint32 hardware = 0x80076364u + 40u * (uint32)allocated;
                uint32 pitch_word = (uint32)gtos_sound_note_pitch(pitch, r_u16(sample + 4u)) |
                    ((uint32)r_u16(sample) << 16u);
                w_u8(hardware, 1u);
                w_u32(hardware + 20u, voice + 27u);
                w_u8(hardware + 4u, 2u);
                w_u8(hardware + 1u, 0u);
                w_u32(hardware + 24u, 0u); w_u32(hardware + 32u, 0u);
                w_u32(hardware + 8u, (uint32)left | ((uint32)right << 16u));
                w_u32(hardware + 12u, pitch_word);
                w_u32(hardware + 16u, r_u32(sample + 16u));
                w_u32(hardware + 28u, (uint32)r_u16(sample + 6u) * r_u16(0x80076362u));
                w_u8(hardware + 36u, r_u8(sample + 10u) & 1u);
                w_u8(0x8007635Eu, 1u);
                w_u32(hardware + 4u, 0x00010700u);
            }
        }
    }
}

static uint32 gtos_sound_delta(uint32 *cursor)
{
    uint32 byte = r_u8((*cursor)++), value = byte;
    if (byte & 128u) {
        value = byte & 127u;
        do {
            byte = r_u8((*cursor)++);
            value = (value << 7u) + (byte & 127u);
        } while (byte & 128u);
    }
    return value;
}

static void gtos_sound_sequence_events(uint32 sequence)
{
    uint32 i;
    for (i = 0u; i < 16u; ++i) {
        uint32 row = sequence + 40u + 40u * i, cursor = r_u32(row), remaining;
        if (!cursor) continue;
        remaining = r_u32(row + 4u) - r_u32(sequence + 20u);
        w_u32(row + 4u, remaining);
        while ((sint32)remaining <= 0) {
            uint32 command = r_u8(cursor++);
            if (command & 128u) {
                uint32 velocity = r_u8(cursor++), duration = gtos_sound_delta(&cursor);
                gtos_sound_note(sequence, row, command & 127u, velocity,
                    duration + r_u32(sequence + 20u) + remaining);
                remaining = r_u32(row + 4u);
            } else if (command & 64u) {
                w_u16(row + 12u, (uint16)((r_u8(cursor++) | ((command & 63u) << 8u)) - 0x2000u));
                remaining = r_u32(row + 4u);
            } else if (command & 32u) {
                sint32 depth = (sint8)r_u8(row + 18u) + 1;
                w_u8(row + 36u + (uint32)depth, (command & 31u) + 1u);
                w_u32(row + 20u + 4u * (uint32)depth, cursor);
                remaining = r_u32(row + 4u);
                w_u8(row + 18u, (uint8)depth);
            } else {
                switch (command) {
                case 0: cursor = 0u; break;
                case 1: {
                    sint32 depth = (sint8)r_u8(row + 18u) + 1;
                    uint32 repeat = r_u8(cursor++);
                    w_u8(row + 36u + (uint32)depth, repeat);
                    w_u32(row + 20u + 4u * (uint32)depth, cursor);
                    remaining = r_u32(row + 4u);
                    w_u8(row + 18u, (uint8)depth);
                    break;
                }
                case 2: {
                    sint32 depth = (sint8)r_u8(row + 18u);
                    uint32 repeat = r_u8(row + 36u + (uint32)depth);
                    if (!repeat) {
                        remaining = r_u32(row + 4u);
                        w_u8(row + 18u, (uint8)(depth - 1));
                    } else {
                        if (repeat != 255u) {
                            w_u8(row + 36u + (uint32)depth, repeat - 1u);
                            depth = (sint8)r_u8(row + 18u);
                            remaining = r_u32(row + 4u);
                        }
                        cursor = r_u32(row + 20u + 4u * (uint32)depth);
                    }
                    break;
                }
                case 3: w_u8(row + 17u, r_u8(cursor++)); remaining = r_u32(row + 4u); break;
                case 4: {
                    uint32 volume = r_u16(0x8004CEB8u + 2u * r_u8(cursor++));
                    remaining = r_u32(row + 4u);
                    w_u16(row + 8u, volume);
                    w_u16(row + 10u, (uint16)((sint32)((uint32)r_u16(sequence + 6u) * volume) >> 14u));
                    break;
                }
                case 5: w_u8(row + 16u, r_u8(cursor++)); remaining = r_u32(row + 4u); break;
                case 6:
                    w_u32(sequence + 8u, ((uint32)r_u8(cursor) << 16u) |
                        ((uint32)r_u8(cursor + 1u) << 8u) | r_u8(cursor + 2u));
                    remaining = r_u32(row + 4u); cursor += 3u; break;
                default: break;
                }
                if (!cursor) break;
            }
            remaining += gtos_sound_delta(&cursor);
            w_u32(row + 4u, remaining);
        }
        w_u32(row, cursor);
    }
}

static void gtos_sound_sequence_tick(void)
{
    uint32 sequence = r_u32(0x8007672Cu);
    while (sequence) {
        uint32 remaining = r_u32(sequence + 12u);
        w_u32(sequence + 20u, 0u);
        if ((sint32)remaining > 0) {
            uint32 period = r_u32(sequence + 8u);
            do {
                remaining -= period;
                w_u32(sequence + 20u, r_u32(sequence + 20u) + 1u);
                w_u32(sequence + 12u, remaining);
            } while ((sint32)remaining > 0);
        }
        w_u32(sequence + 12u, remaining + r_u32(sequence + 16u));
        gtos_sound_sequence_events(sequence);
        if (r_u16(sequence + 28u)) {
            sint32 change = gtos_sound_fade_step(sequence + 28u);
            w_u16(sequence + 4u, (uint16)(r_u16(sequence + 4u) + change));
            gtos_sound_sequence_volume(sequence);
        }
        sequence = r_u32(sequence);
    }
    if (r_u16(0x80076738u)) {
        sint32 change = gtos_sound_fade_step(0x80076738u);
        w_u16(0x80076732u, (uint16)(r_u16(0x80076732u) + change));
        for (sequence = r_u32(0x8007672Cu); sequence; sequence = r_u32(sequence))
            gtos_sound_sequence_volume(sequence);
    }
}

static void gtos_sound_voice_change(uint32 owner, uint32 operation, uint32 value)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 row, lock;
    if (index < 0) return;
    row = 0x80076364u + 40u * (uint32)index;
    lock = r_u8(row + 4u);
    w_u8(row + 4u, 1u);
    if (r_u32(row + 20u) == owner) {
        if (operation == 0u) w_u16(row + 6u, 256u);
        else if (operation == 1u) { w_u8(owner, 255u); w_u32(row + 20u, 0u); }
        else {
            w_u8(row + 4u, 2u);
            if (operation == 2u) w_u16(row + 12u, (uint16)value);
            else w_u32(row + 8u, value);
            w_u8(row + 5u, r_u8(row + 5u) | (operation == 2u ? 1u : 2u));
        }
    }
    w_u8(row + 4u, lock);
}

static void gtos_sound_active_voices(void)
{
    uint32 i;
    for (i = 0u; i < 25u; ++i) {
        uint32 voice = 0x80076744u + 28u * i, owner = voice + 27u;
        uint32 sequence, track, sample, instrument, remaining, product, level;
        sint32 bend, pitch, pan;
        uint16 left, right;
        if ((sint8)r_u8(owner) < 0) continue;
        sequence = r_u32(voice);
        if (!sequence) {
            gtos_sound_voice_change(owner, 0u, 0u);
            gtos_sound_voice_change(owner, 1u, 0u);
            continue;
        }
        remaining = r_u32(voice + 16u);
        if ((sint32)remaining > 0) {
            remaining -= r_u32(sequence + 20u);
            w_u32(voice + 16u, remaining);
            if ((sint32)remaining <= 0) gtos_sound_voice_change(owner, 0u, 0u);
        }
        track = r_u32(voice + 4u); sample = r_u32(voice + 12u);
        bend = (sint16)r_u16(track + 12u);
        if ((sint16)r_u16(voice + 22u) != bend) {
            pitch = ((sint32)r_u8(voice + 24u) << 8u) +
                ((bend * r_u8(sample + (bend > 0 ? 15u : 14u))) >> 5u);
            gtos_sound_voice_change(owner, 2u, gtos_sound_note_pitch(pitch, r_u16(sample + 4u)));
            track = r_u32(voice + 4u);
            w_u16(voice + 22u, r_u16(track + 12u));
        }
        level = r_u16(track + 10u);
        if (r_u16(voice + 20u) == level && r_u8(voice + 26u) == r_u8(track + 16u)) continue;
        instrument = r_u32(voice + 8u);
        product = level * (uint32)(sint32)(sint16)r_u16(instrument + 2u);
        level = (uint32)((sint32)product >> 14u);
        product = r_u16(0x8004CEB8u + 2u * r_u8(voice + 25u)) * level;
        level = (uint32)((sint32)product >> 14u);
        product = (uint32)(sint32)(sint16)r_u16(sample + 2u) * level;
        level = (uint32)((sint32)product >> 14u);
        pan = r_u8(sample + 9u) + r_u8(track + 16u) + r_u8(instrument + 1u) - 128;
        if (pan < 0) pan = 0; else if (pan >= 128) pan = 127;
        product = r_u16(0x8004D0B6u - 2u * (uint32)pan) * level;
        left = (uint16)((sint32)product >> 14u);
        product = r_u16(0x8004CFB8u + 2u * (uint32)pan) * level;
        right = (uint16)((sint32)product >> 14u);
        gtos_sound_voice_change(owner, 3u, (uint32)left | ((uint32)right << 16u));
        track = r_u32(voice + 4u);
        w_u16(voice + 20u, r_u16(track + 10u));
        w_u8(voice + 26u, r_u8(track + 16u));
    }
}

void gtos_sound_vblank(void)
{
    if (!r_u32(0x80073734u)) return;
    gtos_voice_tick();
    if (!r_u32(0x80073744u)) return;
    gtos_sound_sequence_tick();
    gtos_sound_active_voices();
}

static sint32 gtos_voice_allocate(uint32 priority)
{
    uint32 pass, scan, index = (uint32)(sint32)(sint8)r_u8(0x8007635Cu), chosen = 0u;
    sint32 chosen_index = -1;
    for (pass = 0u; pass < 2u; ++pass) {
        for (scan = 0u; scan < 24u; ++scan) {
            uint32 row;
            index = index == 23u ? 0u : index + 1u;
            row = 0x80076364u + 40u * index;
            if (r_u8(row + 4u) || (!pass && r_u8(row))) continue;
            w_u8(row + 4u, 1u);
            if (!r_u8(row)) {
                w_u8(row + 3u, priority);
                w_u8(0x8007635Cu, index);
                return (sint32)index;
            }
            if (r_u8(row) == 1u && r_u8(row + 3u) >= priority) {
                if (chosen) w_u8(chosen + 4u, 0u);
                chosen = row;
                chosen_index = (sint32)index;
            } else w_u8(row + 4u, 0u);
        }
    }
    if (chosen) {
        uint32 owner = r_u32(chosen + 20u);
        if (owner) { w_u32(chosen + 20u, 0u); w_u8(owner, 255u); }
        w_u32(chosen + 4u, 0x01000001u);
        w_u8(0x8007635Cu, chosen_index);
    }
    return chosen_index;
}

void gtos_sound_trigger(uint32 instrument, sint32 volume, sint32 pan, sint32 priority)
{
    sint32 product, pitch_shift = -2, difference, voice;
    uint32 left, right, pitch, row;
    if (!r_u32(0x8004F69Cu)) return;
    product = (sint32)((uint32)volume * r_u32(0x8004F69Cu));
    volume = product / 16384;
    product = (sint32)((uint32)volume * (uint32)(sint32)(sint16)r_u16(instrument + 2u));
    volume = product / 16384;
    left = (uint32)((sint32)((uint32)r_u16(0x8004D0B6u - 2u * pan) * (uint32)volume) >> 14);
    right = (uint32)((sint32)((uint32)r_u16(0x8004CFB8u + 2u * pan) * (uint32)volume) >> 14);
    difference = (sint32)(r_u8(instrument + 12u) << 8u) - (sint32)r_u16(instrument + 4u);
    while (difference < 0) { difference += 3072; --pitch_shift; }
    while (difference >= 3072) { difference -= 3072; ++pitch_shift; }
    pitch = r_u16(0x8004CBB8u + 2u * ((uint32)difference >> 3u));
    pitch = pitch_shift <= 0 ? pitch >> ((uint32)-pitch_shift & 31u) : pitch << ((uint32)pitch_shift & 31u);
    if (priority < 0) priority = r_u8(instrument + 8u);
    voice = gtos_voice_allocate((uint8)priority);
    if (voice < 0) return;
    row = 0x80076364u + 40u * (uint32)voice;
    w_u8(row, 1u);
    w_u32(row + 20u, 0u);
    w_u8(row + 4u, 2u);
    w_u8(row + 1u, 0u);
    w_u32(row + 24u, 0u);
    w_u32(row + 32u, 0u);
    w_u16(row + 8u, left);
    w_u16(row + 10u, right);
    w_u16(row + 12u, pitch);
    w_u16(row + 14u, r_u16(instrument));
    w_u32(row + 16u, r_u32(instrument + 16u));
    w_u32(row + 28u, r_u16(instrument + 6u) * r_u16(0x80076362u));
    w_u8(row + 36u, r_u8(instrument + 10u) & 1u);
    w_u8(0x8007635Eu, 1u);
    w_u32(row + 4u, 67328u);
}
