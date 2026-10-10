#include "game_draft_registry.h"
#include "psx_spu.h"

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);
void xport_bind_native_spu_transfer(void);

static void gtmain_spu_dma_callback(void)
{
    gtmain_spu_dma_complete();
}

static void gtmain_spu_start(void)
{
    if (r_u32(0x800B2414u)) return;
    w_u32(0x800B2414u, 1u);
    /* TODO Connect the native transfer scheduler to the registered DMA callback */
    DMACallback(4u, gtmain_spu_dma_callback);
    /* Native completion does not allocate a BIOS event descriptor */
    w_u32(0x800B23ACu, 0u);
}

static uint32 gtmain_spu_initialize(uint32 mode)
{
    uint32 voice;
    uint32 result;
    uint32 value;
    ResetCallback();
    gt_native_spu_init();
    gtmain_spu_chip_init(mode);
    xport_bind_native_spu_transfer();
    if (!mode) {
        for (voice = 0u; voice < 24u; ++voice)
            w_u16(0x800B240Eu - 2u * voice, 0xC000u);
    }
    gtmain_spu_start();
    value = r_u32(0x800B2424u);
    w_u32(0x800B23B8u, 0u);
    w_u32(0x800B23BCu, 0u);
    w_u32(0x800B23C8u, 0u);
    w_u16(0x800B23CCu, 0u);
    w_u16(0x800B23CEu, 0u);
    w_u32(0x800B23D0u, 0u);
    w_u32(0x800B23D4u, 0u);
    w_u32(0x800B23C0u, value);
    result = r_u32(0x800B2344u) + 2u * 209u;
    w_u16(result, value);
    w_u32(0x800B2454u, 0u);
    w_u32(0x800B2458u, 0u);
    w_u32(0x800B245Cu, 0u);
    w_u32(0x800B23B4u, 0u);
    w_u32(0x800B2360u, 0u);
    w_u32(0x800B23B0u, 0u);
    w_u32(0x800B23DCu, 0u);
    w_u32(0x800B23D8u, 0u);
    w_u32(0x800B2410u, 0u);
    return result;
}

uint32 gtmain_sound_tempo(uint32 tempo)
{
    uint32 scaled = tempo << 8;
    w_u16(0x800B55D0u, tempo);
    w_u16(0x800B55D2u, (uint32)((sint32)scaled / 60));
    return 0x800B55CCu;
}

uint32 gtmain_sound_mode(uint32 mode)
{
    uint32 selected = mode < 2u ? mode : 0u;
    w_u32(0x800B23B4u, mode);
    w_u32(0x800B2360u, selected);
    return selected;
}

uint32 gtmain_sound_init(uint32 incoming_result)
{
    uint32 result = incoming_result;
    if (!r_u32(0x800B1BE4u)) {
        /* Original wrapper 800866A8 invokes the classified _SpuInit with mode zero */
        gtmain_spu_initialize(0u);
        w_u16(0x1F801D80u, 0x3FFFu);
        w_u16(0x1F801D82u, 0x3FFFu);
        gtmain_sound_mode(0u);
        sub_8007BD58(0x800B55CCu, 0u, 0x3C8u);
        w_u8(0x800B55CCu, 23u);
        result = gtmain_sound_tempo(60u);
        w_u32(0x800B1BE4u, 1u);
    }
    return result;
}


uint32 gtmain_data_checksum(uint32 address, uint32 size)
{
    uint32 sum = 0xAAAAu, crc = 0x3770u, i, bit;
    for (i = 0u; i < size; ++i) {
        uint32 value = r_u8(address + i);
        sum = (sum + value) ^ (value << 8);
        for (bit = 0u; bit < 8u; ++bit) {
            crc <<= 1;
            if (crc & 0x10000u) crc ^= 0x11021u;
            crc |= (value >> 7) & 1u;
            value <<= 1;
        }
    }
    return (sum << 16) | (crc & 0xFFFFu);
}

/* GTMAIN.EXE:80069F54 */
uint32 gtmain_sound_saved_data_init(uint32 buffer)
{
    uint32 checksum, header, stored;
    w_u32(0x800B235Cu, 0x2000u >> 3);
    w_u32(0x800B23B4u, 0u);
    w_u32(0x800B2360u, 0u);
    /* TODO Preserve original read and wait state through the native SPU adapter */
    w_u16(r_u32(0x800B2344u) + 422u, r_u16(0x800B235Cu));
    w_u32(0x800B2394u, 1u);
    w_u32(0x800B2398u, buffer);
    w_u32(0x800B239Cu, 0xB3C0u >> 6);
    if (!spu_download((uint32)r_u16(0x800B235Cu) << (r_u32(0x800B236Cu) & 31u), psx_addr(buffer, 0xB3C0u), 0xB3C0u))
        gt_draft_unresolved_call(0x800862B0u, buffer, 0xB3C0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gtmain_spu_dma_complete();
    checksum = gtmain_data_checksum(buffer, 0xB398u);
    header = r_u32(buffer);
    if (header == 0x47546F73u) {
        stored = r_u32(buffer + 0xB398u);
        if (stored == checksum) return stored;
    }
    sub_8007BD58(buffer, 0u, 0xB3C0u);
    w_u32(buffer, 0x47546F73u);
    return 0x47546F73u;
}

void gtmain_sound_sequence_init(void)
{
    uint32 i, row;
    if (r_u32(0x800B1BF4u)) return;
    sub_8007BD58(0x800B599Cu, 0u, 0x2D4u);
    w_u32(0x800B599Cu, 0u);
    w_u16(0x800B59A2u, 0x3FFFu);
    w_u8(0x800B59A4u, 0u);
    w_u8(0x800B59CFu, 0xFFu);
    for (i = 0u; i < 6u; ++i) {
        row = 0x800B59D0u + i * 112u;
        w_u8(row + 55u, 0xFFu);
        w_u8(row + 83u, 0xFFu);
        w_u8(row + 111u, 0xFFu);
        w_u8(row + 27u, 0xFFu);
    }
    w_u16(0x800B59A0u, 60u);
    w_u32(0x800B1BF4u, 1u);
}

uint32 gtmain_sound_reverb(uint32 preset, uint32 delay, uint32 feedback)
{
    uint32 selected = preset & 0xFFu;
    uint32 row = 0x800B1C18u + selected * 64u;
    uint32 table = 0x800B1C04u + selected * 2u;
    uint32 i;
    if (selected == 7u || selected == 8u) {
        uint32 value = (delay << 13) / 127u;
        uint32 half = (uint32)((sint32)value >> 1);
        uint32 a = r_u16(row), b = r_u16(row + 2u);
        uint32 c = r_u16(row + 26u), d = r_u16(row + 34u);
        uint32 e = r_u16(row + 56u), f = r_u16(row + 58u);
        w_u16(row + 20u, value - a);
        w_u16(row + 22u, half - b);
        w_u16(row + 24u, c + half);
        w_u16(row + 32u, d + half);
        w_u16(row + 52u, e + half);
        w_u16(row + 54u, f + half);
    }
    if (selected == 7u) {
        uint32 product = ((feedback << 7) + feedback) << 8;
        w_u16(row + 14u, (uint32)((sint32)product / 127));
    }
    w_u16(0x1F801D84u, 0u);
    w_u16(0x1F801D86u, 0u);
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) & 0xFF7Fu);
    w_u16(0x1F801DA2u, r_u16(table));
    for (i = 0u; i < 4u; ++i) {
        uint32 a = r_u32(row + i * 16u);
        uint32 b = r_u32(row + i * 16u + 4u);
        uint32 c = r_u32(row + i * 16u + 8u);
        uint32 d = r_u32(row + i * 16u + 12u);
        uint32 registers = 0x1F801DC0u + i * 16u;
        w_u16(registers, a);
        w_u16(registers + 2u, a >> 16);
        w_u16(registers + 6u, b >> 16);
        w_u16(registers + 10u, c >> 16);
        w_u16(registers + 14u, d >> 16);
        w_u16(registers + 4u, b);
        w_u16(registers + 8u, c);
        w_u16(registers + 12u, d);
    }
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 0x80u);
    w_u8(0x800B55CDu, 1u);
    return (0x10000u - r_u16(table)) << 3;
}

uint32 gtmain_sound_reverb_enable(uint32 mode)
{
    uint32 result = (r_u16(0x1F801DAAu) & 0xFF7Fu) | ((mode & 1u) << 7);
    w_u16(0x1F801DAAu, result);
    return result;
}
