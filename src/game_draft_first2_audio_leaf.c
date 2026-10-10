#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_spu.h"

/* GTMAIN.EXE 8007B610 */
uint32 gtmain_spu_transfer_address(uint32 address)
{
    w_u32(0x800B235Cu, address >> 3);
    return address;
}

/* GTMAIN.EXE 8007B628 */
uint32 gtmain_spu_transfer_mode(uint32 mode)
{
    uint32 result = mode < 2u ? mode : 0;
    w_u32(0x800B23B4u, mode); w_u32(0x800B2360u, result);
    return result;
}

/* GTMAIN.EXE 8007B6C4 */
uint32 gtmain_spu_control_bit(uint32 enabled)
{
    uint32 previous, value;
    if (!spu_mmio_read(NULL, 0x1F801DAAu, 2u, &previous))
        gt_draft_failure("Native SPU halfword control read", 0x8007B6C4u);
    value = enabled ? previous | 1u : previous & 0xFFFEu;
    if (!spu_mmio_write(NULL, 0x1F801DAAu, 2u, value, 3u))
        gt_draft_failure("Native SPU halfword control write", 0x8007B6C4u);
    return previous;
}

/* GTMAIN.EXE 80078A34 */
sint32 gtmain_audio_status(void)
{
    return (sint16)r_u16(0x800C0C02u);
}

/* GTMAIN.EXE 80078A44 */
uint32 gtmain_music_file(void)
{
    return gtmain_environment_value(0x8008E2E4u);
}

/* GTMAIN.EXE 80078A68 */
uint32 gtmain_audio_buffer_end(uint32 index)
{
    uint32 words = r_u32(0x800C00F0u);
    uint32 buffer = r_u32(0x800C00E4u + 4u * index);
    return 4u * words + buffer - 4u;
}

/* GTMAIN.EXE 80078A90 */
uint32 gtmain_audio_buffers_init(uint32 first, uint32 second, uint32 words)
{
    w_u32(0x800C00E0u, 0); w_u32(0x800C00E4u, first);
    w_u32(0x800C00E8u, second); w_u32(0x800C00ECu, 0);
    w_u32(0x800C00F0u, words);
    return 0x800C00E0u;
}

/* GTMAIN.EXE 80078BA0 */
uint32 gtmain_audio_buffers_relocate(void)
{
    uint32 result = 0x800C00E4u;
    uint32 first = r_u32(0x800C00E4u), entry = r_u32(0x800C00E0u), second = r_u32(0x800C00E8u);
    while (entry) {
        uint32 first_words = r_u32(entry + 12u), second_words = r_u32(entry + 12u);
        w_u32(entry + 24u, first); w_u32(entry + 16u, first); w_u32(entry + 20u, second);
        entry = r_u32(entry + 4u);
        result = 4u * first_words; first += result; second += 4u * second_words;
    }
    return result;
}

/* GTMAIN.EXE 80078C74 */
sint32 gtmain_audio_output_scale(uint32 shift)
{
    sint32 value = (sint16)r_u16(0x800994BCu);
    value >>= shift & 31u;
    if (!spu_mmio_write(NULL, 0x1F801DB0u, 2u, (uint16)value, 3u))
        gt_draft_failure("Native SPU CD left volume write", 0x80078C74u);
    if (!spu_mmio_write(NULL, 0x1F801DB2u, 2u, (uint16)value, 3u))
        gt_draft_failure("Native SPU CD right volume write", 0x80078C74u);
    return value;
}
