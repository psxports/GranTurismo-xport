#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8007EBA8 */
void gtmain_sound_tick_limit(sint16 limit)
{
    w_u16(0x800B59A0u, (uint16)limit);
}

/* GTMAIN.EXE 8007EDF8 */
uint32 gtmain_spu_master_volume(sint16 left, sint16 right)
{
    w_u16(0x1F801D84u, (uint16)left);
    w_u16(0x1F801D86u, (uint16)right);
    return 0x1F801C00u;
}

/* GTMAIN.EXE 8007EEE8 */
uint32 gtmain_sound_voice_pitch(uint32 owner, sint16 pitch)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    if (r_u32(voice + 20u) == owner) {
        uint8 flags;
        w_u8(voice + 4u, 2u);
        flags = r_u8(voice + 5u);
        w_u16(voice + 12u, (uint16)pitch);
        w_u8(voice + 5u, flags | 1u);
    }
    w_u8(voice + 4u, state);
    return voice;
}

/* GTMAIN.EXE 8007EF48 */
uint32 gtmain_sound_voice_volume(uint32 owner, uint32 volume_address)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice, bound_owner, volume;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    bound_owner = r_u32(voice + 20u);
    /* The branch delay slot reads volume even when the owner differs */
    volume = r_u32(volume_address);
    if (bound_owner == owner) {
        uint8 flags;
        w_u8(voice + 4u, 2u);
        flags = r_u8(voice + 5u);
        w_u32(voice + 8u, volume);
        w_u8(voice + 5u, flags | 2u);
    }
    w_u8(voice + 4u, state);
    return voice;
}

/* GTMAIN.EXE 8007F018 */
uint32 gtmain_sound_voice_release(uint32 owner)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    if (r_u32(voice + 20u) == owner) w_u16(voice + 6u, 256u);
    w_u8(voice + 4u, state);
    return voice;
}

/* GTMAIN.EXE 8007F068 */
uint32 gtmain_sound_voice_detach(uint32 owner)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    if (r_u32(voice + 20u) == owner) {
        w_u8(owner, 255u);
        w_u32(voice + 20u, 0u);
    }
    w_u8(voice + 4u, state);
    return voice;
}

/* GTMAIN.EXE 8007F0C8 */
uint32 gtmain_sound_voice_reverb(uint32 owner, uint8 reverb)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    if (r_u32(voice + 20u) == owner) {
        w_u8(voice + 36u, reverb);
        w_u8(0x800B55CEu, 1u);
    }
    w_u8(voice + 4u, state);
    return voice;
}

/* GTMAIN.EXE 8007F268 */
uint32 gtmain_sound_voice_priority(uint32 owner, uint8 priority)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    if (r_u32(voice + 20u) == owner) w_u8(voice + 3u, priority);
    w_u8(voice + 4u, state);
    return voice;
}

/* GTMAIN.EXE 8007EE18 */
uint32 gtmain_sound_pan(uint32 output, uint32 volume, uint32 pan)
{
    uint32 offset = pan * 2u;
    uint32 product = r_u16(0x8008EA86u - offset) * volume;
    w_u16(output, (uint16)((sint32)product >> 14));
    product = r_u16(0x8008E988u + offset) * volume;
    w_u16(output + 2u, (uint16)((sint32)product >> 14));
    return offset;
}

/* GTMAIN.EXE 8007EE68 */
uint32 gtmain_sound_pitch_ratio(uint32 pitch, uint32 reference)
{
    sint32 delta = (sint32)(pitch - reference);
    sint32 exponent = -2;
    uint32 value;
    while (delta < 0) { delta = (sint32)((uint32)delta + 3072u); --exponent; }
    while (delta >= 3072) { delta -= 3072; ++exponent; }
    value = r_u16(0x8008E588u + 2u * ((uint32)delta >> 3));
    if (exponent > 0) value <<= (uint32)exponent & 31u;
    else value = (uint32)((sint32)value >> ((0u - (uint32)exponent) & 31u));
    return value & 0xFFFFu;
}

/* GTMAIN.EXE 8007F178 */
uint32 gtmain_sound_instrument_relocate(uint32 image, uint32 spu_base)
{
    uint32 base, table, i, old_spu;
    if (r_u32(image) != 0x54534E49u) return 0;
    base = image - r_u32(image + 4u);
    table = r_u32(image + 20u) + base;
    w_u32(image + 4u, image);
    w_u32(image + 20u, table);
    w_u32(image + 28u, r_u32(image + 28u) + base);
    w_u32(image + 36u, r_u32(image + 36u) + base);
    for (i = 0; i < r_u32(image + 32u); ++i) {
        uint32 pointer = r_u32(image + 36u) + i * 4u;
        w_u32(pointer, r_u32(pointer) + base);
    }
    if (spu_base) {
        old_spu = r_u32(image + 12u);
        w_u32(image + 12u, spu_base);
        for (i = 0; i < r_u32(image + 24u); ++i) {
            uint32 sample = r_u32(image + 28u) + i * 20u;
            w_u16(sample, (uint16)(r_u16(sample) + ((spu_base - old_spu) >> 3)));
        }
    }
    return image;
}

/* GTMAIN.EXE 8007DD4C */
uint32 gtmain_sound_engine_relocate(uint32 image, uint32 spu_base)
{
    uint32 base, i, old_spu;
    if (r_u32(image) != 0x4E474E45u) return 0;
    base = image - r_u32(image + 4u);
    w_u32(image + 4u, image);
    w_u32(image + 20u, r_u32(image + 20u) + base);
    for (i = 0; i < 2u; ++i) {
        uint32 field = image + 28u + i * 8u;
        if (r_u32(field)) w_u32(field, r_u32(field) + base);
    }
    if (spu_base) {
        old_spu = r_u32(image + 12u);
        w_u32(image + 12u, spu_base);
        for (i = 0; i < 2u; ++i) {
            uint32 entry, table = r_u32(image + 28u + i * 8u);
            for (entry = 0; entry < r_u32(image + 24u + i * 8u); ++entry) {
                uint32 sample = table + entry * 16u + 12u;
                w_u32(sample, r_u32(sample) + spu_base - old_spu);
            }
        }
    }
    return image;
}

/* GTMAIN.EXE 8007DC0C */
uint32 gtmain_sound_engine_channel_init(uint32 channel, uint32 engine, uint32 entry, uint8 priority)
{
    uint32 record = engine + entry * 8u;
    uint32 data, i;
    w_u8(channel + 5u, priority);
    w_u8(channel + 4u, (uint8)r_u32(record + 24u));
    data = r_u32(record + 28u);
    w_u16(channel + 10u, 4096u);
    w_u16(channel + 12u, 0x3FFFu); w_u16(channel + 14u, 0x3FFFu);
    w_u16(channel + 6u, 0x4000u);
    w_u32(channel, data);
    for (i = 1u; i <= 2u; ++i) {
        w_u8(channel + i * 12u + 4u, 255u);
        w_u8(channel + i * 12u + 5u, 255u);
        w_u8(channel + i * 12u + 6u, 255u);
    }
    return 0xFFFFFFFFu;
}

/* GTMAIN.EXE 8007F628 */
sint32 gtmain_sound_voice_allocate(uint32 priority)
{
    sint32 index = (sint8)r_u8(0x800B55CCu), selected_index = -1;
    uint32 voice = 0x800B55D4u + (uint32)index * 40u, selected = 0, pass, n;
    for (pass = 0; pass < 2u; ++pass) {
        for (n = 0; n < 24u; ++n) {
            uint32 flags;
            voice += 40u;
            if (index == 23) { voice = 0x800B55D4u; index = -1; }
            ++index;
            if (r_u8(voice + 4u)) continue;
            if (!pass && r_u8(voice)) continue;
            w_u8(voice + 4u, 1u);
            flags = pass ? r_u32(voice) : r_u8(voice);
            if (!(flags & 255u)) {
                w_u8(voice + 3u, (uint8)priority);
                w_u8(0x800B55CCu, (uint8)index);
                return index;
            }
            if (pass && (flags & 255u) == 1u && (flags >> 24) >= priority) {
                uint32 age = r_u32(voice + 24u);
                uint32 envelope = (flags >> 16) & 255u;
                if (!envelope) envelope = r_u8(voice + 6u);
                (void)age; (void)envelope;
                /* Original tie branches all replace the previous candidate */
                if (selected) w_u8(selected + 4u, 0u);
                selected = voice; selected_index = index;
            } else w_u8(voice + 4u, 0u);
        }
    }
    if (!selected) return -1;
    if (r_u32(selected + 20u)) {
        uint32 owner = r_u32(selected + 20u);
        w_u32(selected + 20u, 0);
        w_u8(owner, 255u);
    }
    w_u32(selected + 4u, 0x01000001u);
    w_u8(0x800B55CCu, (uint8)selected_index);
    return selected_index;
}

/* GTMAIN.EXE 8007F788 */
void gtmain_sound_voice_initialize(sint32 index, uint32 instrument)
{
    uint32 voice = 0x800B55D4u + (uint32)index * 40u;
    uint32 duration, volume, pitch, sample;
    uint8 reverb;
    w_u8(voice + 4u, 2u);
    duration = (uint32)r_u16(instrument + 12u) * r_u16(0x800B55D2u);
    w_u8(voice + 1u, 0);
    w_u32(voice + 24u, 0); w_u32(voice + 32u, 0);
    volume = r_u32(instrument); pitch = r_u32(instrument + 4u); sample = r_u32(instrument + 8u);
    reverb = r_u8(instrument + 15u);
    w_u32(voice + 8u, volume); w_u32(voice + 12u, pitch); w_u32(voice + 16u, sample);
    w_u32(voice + 28u, duration); w_u8(voice + 36u, reverb & 1u);
    w_u8(0x800B55CEu, 1u);
    w_u32(voice + 4u, 0x00010700u);
}

/* GTMAIN.EXE 8007EFA8 */
uint32 gtmain_sound_voice_start(uint32 owner, uint32 instrument)
{
    sint32 index = gtmain_sound_voice_allocate(r_u8(instrument + 14u));
    uint32 result;
    if (owner) w_u8(owner, (uint8)index);
    if (index < 0) return (uint32)index;
    result = 0x800B55CCu + (uint32)index * 40u;
    w_u8(result + 8u, 1u);
    w_u32(result + 28u, owner);
    gtmain_sound_voice_initialize(index, instrument);
    return result;
}


/* GTMAIN.EXE 8007DC74 */
void gtmain_sound_engine_channel_stop(uint32 channel)
{
    uint32 offset;
    for (offset = 0; offset < 24u; offset += 12u) {
        uint32 owner = channel + offset + 16u;
        if ((sint8)r_u8(owner) >= 0) {
            gtmain_sound_voice_release(owner);
            gtmain_sound_voice_detach(owner);
        }
    }
}

/* GTMAIN.EXE 8007DCE0 */
void gtmain_sound_engine_channel_priority(uint32 channel, uint8 priority)
{
    uint32 offset;
    w_u8(channel + 5u, priority);
    for (offset = 0; offset < 24u; offset += 12u) {
        uint32 owner = channel + offset + 16u;
        if ((sint8)r_u8(owner) >= 0) gtmain_sound_voice_priority(owner, priority);
    }
}

/* GTMAIN.EXE 8007F594 */
uint32 gtmain_sound_voices_reset(void)
{
    uint32 enabled = r_u32(0x800B1BE4u), voice, result;
    w_u32(0x800B1BE4u, 0);
    for (voice = 0x800B55D4u; voice < 0x800B5994u; voice += 40u) {
        uint32 owner = r_u32(voice + 20u);
        w_u8(voice + 5u, 2u);
        w_u32(voice + 8u, 0);
        w_u8(voice + 7u, 1u); w_u8(voice + 6u, 0); w_u8(voice + 4u, 0); w_u8(voice, 0);
        if (owner) { w_u8(owner, 255u); w_u32(voice + 20u, 0); }
    }
    result = gtmain_sound_voice_tick();
    w_u32(0x800B1BE4u, enabled);
    return result;
}

/* GTMAIN.EXE 8007ED98 */
void gtmain_sound_shutdown(void)
{
    if (!r_u32(0x800B1BE4u)) return;
    w_u32(0x800B1BE4u, 0);
    gtmain_sound_voices_reset();
    if (r_u8(0x800B55CDu)) {
        gtmain_sound_reverb(0u, 0u, 0u);
        gtmain_sound_reverb_enable(0u);
    }
    if (r_u32(0x800B2414u) == 1u) {
        w_u32(0x800B2414u, 0);
        w_u32(0x800B237Cu, 0); w_u32(0x800B2380u, 0);
        DMACallback(4, NULL);
        /* Original SDK shutdown removes callbacks and retains sample RAM */
    }
}


/* GTMAIN.EXE 8007F128 */
uint32 gtmain_sound_sample_upload(uint32 spu_address, uint32 source, uint32 size, uint32 relocation_result)
{
    uint32 count;
    sint32 completed;
    if (!size) return relocation_result;
    (void)gtmain_spu_transfer_address(spu_address);
    (void)gtmain_spu_transfer_mode(0);
    count = size > 0x7EFF0u ? 0x7EFF0u : size;
    if (SpuWrite(psx_addr(source, count), count) != count)
        gt_draft_failure("Native SPU sample write", 0x800859A8u);
    if (!r_u32(0x800B237Cu)) w_u32(0x800B2378u, 0);
    completed = SpuIsTransferCompleted(1);
    if (!completed)
        gt_draft_failure("Native SPU sample completion", 0x800858F8u);
    return (uint32)completed;
}


static uint32 gtmain_sound_voice_start_native(uint32 owner, uint32 volume,
    uint16 pitch, uint16 sample, uint32 adsr, uint16 duration, uint8 priority, uint8 reverb)
{
    sint32 index = gtmain_sound_voice_allocate(priority);
    uint32 result, voice;
    if (owner) w_u8(owner, (uint8)index);
    if (index < 0) return (uint32)index;
    result = 0x800B55CCu + 40u * (uint32)index;
    w_u8(result + 8u, 1u); w_u32(result + 28u, owner);
    voice = result + 8u;
    w_u8(voice + 4u, 2u);
    w_u8(voice + 1u, 0);
    w_u32(voice + 24u, 0); w_u32(voice + 32u, 0);
    w_u32(voice + 8u, volume);
    w_u32(voice + 12u, (uint32)pitch | ((uint32)sample << 16));
    w_u32(voice + 16u, adsr);
    w_u32(voice + 28u, (uint32)duration * r_u16(0x800B55D2u));
    w_u8(voice + 36u, reverb & 1u);
    w_u8(0x800B55CEu, 1u); w_u32(voice + 4u, 0x00010700u);
    return result;
}

/* GTMAIN.EXE 8007D708 */
uint32 gtmain_sound_engine_pitch(uint32 channel, uint32 slot, uint32 speed, uint32 smooth)
{
    uint32 subvoice = channel + 12u * slot;
    uint32 record = r_u32(channel) + ((uint32)(sint32)(sint8)r_u8(subvoice + 17u) << 4);
    uint32 denominator = (uint32)(sint32)(sint16)r_u16(record);
    uint32 value, result = record;
    if (!denominator) { gt_draft_exception(7u); return result; }
    value = (r_u32(record + 8u) * speed) / denominator;
    value = (value * (uint32)(sint32)(sint16)r_u16(channel + 10u)) >> 12;
    if (!value) value = 1u;
    if (smooth) {
        result = r_u16(subvoice + 24u);
        if (2u * result < value) value = 2u * result;
        else if (value < (result >> 1)) value = result >> 1;
    }
    w_u16(subvoice + 24u, (uint16)value);
    return result;
}

static sint32 gtmain_sound_engine_divide(sint32 numerator, sint32 denominator)
{
    if (!denominator) { gt_draft_exception(7u); return 0; }
    if (numerator == (sint32)0x80000000u && denominator == -1) {
        gt_draft_exception(6u); return 0;
    }
    return numerator / denominator;
}

/* GTMAIN.EXE 8007D7BC */
uint32 gtmain_sound_engine_volume(uint32 channel, uint32 slot, sint32 speed, uint32 smooth)
{
    uint32 subvoice = channel + 12u * slot;
    uint32 record = r_u32(channel) + ((uint32)(sint32)(sint8)r_u8(subvoice + 17u) << 4);
    sint32 other = (sint8)r_u8(channel + 12u * ((slot + 1u) & 1u) + 17u);
    sint32 mix = 0x4000;
    uint32 phase, value, left, right;
    if (other >= 0) {
        uint32 other_record = r_u32(channel) + 16u * (uint32)other;
        sint32 begin = (sint16)r_u16(record + 2u);
        if (speed < begin) {
            sint32 end = (sint16)r_u16(other_record + 4u);
            mix = gtmain_sound_engine_divide((sint32)(((uint32)speed - (uint32)end) << 14), begin - end);
        } else {
            sint32 end = (sint16)r_u16(record + 4u);
            if (end < speed) {
                sint32 next = (sint16)r_u16(other_record + 2u);
                mix = gtmain_sound_engine_divide((sint32)(((uint32)next - (uint32)speed) << 14), next - end);
            }
        }
    }
    phase = (((uint32)mix << 10) >> 14) & 0xFFFu;
    value = (uint32)(sint32)(sint16)r_u16(0x8009423Cu + 2u * phase);
    value = (uint32)((sint32)((value << 14) - value) >> 12);
    value = (value * (uint32)(sint32)(sint16)r_u16(record + 6u)) >> 14;
    left = (value * (uint32)(sint32)(sint16)r_u16(channel + 12u)) >> 14;
    right = (value * (uint32)(sint32)(sint16)r_u16(channel + 14u)) >> 14;
    if (smooth) {
        sint32 old_left = (sint16)r_u16(subvoice + 20u);
        sint32 old_right = (sint16)r_u16(subvoice + 22u);
        if ((sint32)left > old_left + 4096) left = (uint32)(old_left + 4096);
        else if ((sint32)left < old_left - 4096) left = (uint32)(old_left - 4096);
        if ((sint32)right > old_right + 4096) right = (uint32)(old_right + 4096);
        else if ((sint32)right < old_right - 4096) right = (uint32)(old_right - 4096);
    }
    w_u16(subvoice + 20u, (uint16)left); w_u16(subvoice + 22u, (uint16)right);
    return subvoice;
}

/* GTMAIN.EXE 8007D998 */
uint32 gtmain_sound_engine_channel_update(uint32 channel)
{
    uint32 speed = r_u16(channel + 8u), previous = 0, result = 0;
    uint32 record = r_u32(channel), i;
    sint32 entry;
    if (speed < 200u) speed = 200u;
    for (i = 0; i < r_u8(channel + 4u); ++i, record += 16u) {
        if ((sint32)speed >= (sint16)r_u16(record + 2u) &&
            (sint16)r_u16(record + 4u) >= (sint32)speed) {
            w_u8(channel + 17u, (uint8)i); w_u8(channel + 29u, 255u); break;
        }
        if (previous && (sint16)r_u16(previous + 4u) < (sint32)speed &&
            (sint32)speed < (sint16)r_u16(record + 2u)) {
            w_u8(channel + 29u, (uint8)i); w_u8(channel + 17u, (uint8)(i - 1u)); break;
        }
        previous = record;
    }
    entry = (sint8)r_u8(channel + 17u);
    if (entry == (sint8)r_u8(channel + 30u) ||
        (sint8)r_u8(channel + 29u) == (sint8)r_u8(channel + 18u)) {
        uint8 next_entry = r_u8(channel + 29u);
        w_u8(channel + 29u, (uint8)entry); w_u8(channel + 17u, next_entry);
    }
    for (i = 0; i < 2u; ++i) {
        uint32 subvoice = channel + 12u * i, owner = subvoice + 16u;
        entry = (sint8)r_u8(subvoice + 17u); result = (uint32)entry;
        if (entry == (sint8)r_u8(subvoice + 18u) && (sint8)r_u8(owner) >= 0) {
            if (entry >= 0) {
                gtmain_sound_engine_pitch(channel, i, speed, 1u);
                gtmain_sound_engine_volume(channel, i, (sint32)speed, 1u);
                gtmain_sound_voice_pitch(owner, r_u16(subvoice + 24u));
                result = gtmain_sound_voice_volume(owner, subvoice + 20u);
            }
        } else {
            if ((sint8)r_u8(owner) >= 0) {
                gtmain_sound_voice_release(owner); gtmain_sound_voice_detach(owner);
                entry = (sint8)r_u8(subvoice + 17u); result = (uint32)entry;
            }
            if (entry >= 0) {
                uint32 sample;
                gtmain_sound_engine_pitch(channel, i, speed, 0);
                gtmain_sound_engine_volume(channel, i, (sint32)speed, 0);
                entry = (sint8)r_u8(subvoice + 17u);
                sample = r_u32(r_u32(channel) + 16u * (uint32)entry + 12u) >> 3;
                w_u16(subvoice + 26u, (uint16)sample);
                gtmain_sound_voice_start_native(owner, r_u32(subvoice + 20u),
                    r_u16(subvoice + 24u), r_u16(subvoice + 26u), 0x0003000Fu,
                    0, r_u8(channel + 5u), 1u);
                result = (uint32)(sint32)(sint8)r_u8(subvoice + 17u);
            }
            w_u8(subvoice + 18u, (uint8)result);
        }
    }
    return result;
}

uint32 gtmain_sound_voice_start_values(uint32 owner, uint32 volume, uint16 pitch, uint16 sample, uint32 adsr, uint16 duration, uint8 priority, uint8 reverb)
{
    return gtmain_sound_voice_start_native(owner, volume, pitch, sample, adsr, duration, priority, reverb);
}

uint32 gtmain_sound_voice_volume_value(uint32 owner, uint32 volume)
{
    sint32 index = (sint8)r_u8(owner);
    uint32 voice, bound_owner;
    uint8 state;
    if (index < 0) return (uint32)index;
    voice = 0x800B55D4u + 40u * (uint32)index;
    state = r_u8(voice + 4u);
    w_u8(voice + 4u, 1u);
    bound_owner = r_u32(voice + 20u);
    if (bound_owner == owner) {
        uint8 flags;
        w_u8(voice + 4u, 2u);
        flags = r_u8(voice + 5u);
        w_u32(voice + 8u, volume);
        w_u8(voice + 5u, flags | 2u);
    }
    w_u8(voice + 4u, state);
    return voice;
}
