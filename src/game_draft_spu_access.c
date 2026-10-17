#include "game_draft_registry.h"
#include "psx_spu.h"
#include "xport_trace.h"
#include <string.h>

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

/* SDK register carriers used by translated game sound code */
static uint16 gt_spu_registers[256];
static uint8 gt_spu_fifo[64];
static uint32 gt_spu_fifo_bytes;

static uint16 gt_spu_read_half(uint32 address)
{
    uint32 value;
    if (address == 0x1F801DAAu) return gt_spu_registers[0xAAu / 2u + 128u];
    if (address == 0x1F801DAEu) return gt_spu_registers[0xAAu / 2u + 128u] & 0x3Fu;
    if (spu_mmio_read(NULL, address, 2u, &value)) return (uint16)value;
    return gt_spu_registers[(address - 0x1F801C00u) / 2u];
}

static void gt_spu_write_half(uint32 address, uint16 value)
{
    uint32 offset = address - 0x1F801C00u;
    gt_spu_registers[offset / 2u] = value;
    if (offset < 384u) {
        SpuVoiceAttr attr = {0};
        attr.voice = 1u << (offset / 16u);
        switch (offset & 15u) {
        case 0: attr.mask = SPU_VOICE_VOLL; attr.volume.left = (sint16)value; break;
        case 2: attr.mask = SPU_VOICE_VOLR; attr.volume.right = (sint16)value; break;
        case 4: attr.mask = SPU_VOICE_PITCH; attr.pitch = value; break;
        case 6: attr.mask = SPU_VOICE_WDSA; attr.addr = (uint32)value << 3u; break;
        case 8: attr.mask = SPU_VOICE_ADSR_ADSR1; attr.adsr1 = value; break;
        case 10: attr.mask = SPU_VOICE_ADSR_ADSR2; attr.adsr2 = value; break;
        case 14: attr.mask = SPU_VOICE_LSAX; attr.loop_addr = (uint32)value << 3u; break;
        default: gt_draft_failure("TODO Native SPU envelope register write", address);
        }
        SpuSetVoiceAttr(&attr);
    } else if (offset == 0x188u || offset == 0x18Au || offset == 0x18Cu || offset == 0x18Eu) {
        uint32 mask = (uint32)value << ((offset & 2u) ? 16u : 0u);
        SpuSetKey(offset < 0x18Cu ? SPU_ON : SPU_OFF, mask);
    } else if (offset == 0x198u || offset == 0x19Au) {
        uint32 mask = (uint32)value << ((offset & 2u) ? 16u : 0u);
        uint32 half = (offset & 2u) ? 0xFF0000u : 0xFFFFu;
        SpuSetReverbVoice(SPU_OFF, half & ~mask);
        SpuSetReverbVoice(SPU_ON, mask);
    } else if (offset == 0x190u || offset == 0x192u || offset == 0x194u || offset == 0x196u) {
        /* TODO Add native pitch modulation and noise when requested */
        if (value) gt_draft_failure("TODO Native SPU modulation/noise", address);
    } else if (offset == 0x1A8u) {
        if (gt_spu_fifo_bytes > sizeof(gt_spu_fifo) - 2u)
            gt_draft_failure("Native SPU transfer FIFO extent", address);
        xport_store_le16(gt_spu_fifo + gt_spu_fifo_bytes, value); gt_spu_fifo_bytes += 2u;
    } else if (offset == 0x1AAu) {
        uint32 native_control;
        spu_native_set_mute(!(value & 0x4000u));
        if (!spu_mmio_read(NULL, address, 2u, &native_control))
            gt_draft_failure("Native SPU routing read", address);
        if (!spu_mmio_write(NULL, address, 2u, (native_control & ~0x8Fu) | (value & 0x8Fu), 3u))
            gt_draft_failure("Native SPU routing write", address);
        if ((value & 0x30u) == 0x10u && gt_spu_fifo_bytes) {
            uint32 target = (uint32)gt_spu_registers[0x1A6u / 2u] << 3u;
            if (!spu_upload(target, gt_spu_fifo, gt_spu_fifo_bytes))
                gt_draft_failure("Native SPU FIFO upload", target);
            gt_spu_registers[0x1A6u / 2u] += (uint16)(gt_spu_fifo_bytes >> 3u);
            gt_spu_fifo_bytes = 0u;
        }
    } else if (offset == 0x184u || offset == 0x186u || offset == 0x1A2u || offset >= 0x1C0u) {
        SpuReverbControl state;
        spu_reverb_export(&state);
        if (offset == 0x184u) state.depth_left = (sint16)value;
        else if (offset == 0x186u) state.depth_right = (sint16)value;
        else if (offset == 0x1A2u) state.work_address = value;
        else state.parameters[(offset - 0x1C0u) / 2u] = value;
        if (!spu_reverb_import(&state, NULL, NULL))
            gt_draft_failure("Native SPU reverb register binding", address);
    } else if (offset == 0x180u || offset == 0x182u || (offset >= 0x1B0u && offset <= 0x1B6u)) {
        if (!spu_mmio_write(NULL, address, 2u, value, 3u))
            gt_draft_failure("Native SPU volume register write", address);
    } else if (offset != 0x19Cu && offset != 0x19Eu && offset != 0x1A0u && offset != 0x1A4u && offset != 0x1A6u && offset != 0x1ACu) {
        gt_draft_failure("TODO Native SPU register operation", address);
    }
}

static sint32 gt_spu_mmio_read(void *user, uint32 physical, uint32 width, uint32 *output)
{
    uint32 value = 0u, i;
    for (i = 0u; i < width; ++i)
        value |= ((gt_spu_read_half((physical + i) & ~1u) >> (((physical + i) & 1u) * 8u)) & 255u) << (i * 8u);
    *output = value;
    return 1;
}

static sint32 gt_spu_mmio_write(void *user, uint32 physical, uint32 width, uint32 value, uint32 mask)
{
    uint32 i;
    for (i = 0u; i < width; ) {
        uint32 half = (physical + i) & ~1u, staged = gt_spu_read_half(half), touched = 0u;
        do {
            uint32 shift = ((physical + i) & 1u) * 8u;
            if (mask & (1u << i)) { staged = (staged & ~(255u << shift)) | (((value >> (8u * i)) & 255u) << shift); touched = 1u; }
            ++i;
        } while (i < width && ((physical + i) & ~1u) == half);
        if (touched) gt_spu_write_half(half, (uint16)staged);
    }
    return 1;
}

void gt_native_spu_bind_mmio(void)
{
    xport_memory_bind_mmio(0x1F801C00u, 0x200u, gt_spu_mmio_read, gt_spu_mmio_write, NULL);
}
