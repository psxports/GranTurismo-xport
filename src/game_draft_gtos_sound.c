#include "game_draft_registry.h"
#include "psx_spu.h"
#include <string.h>

void gtos_audio_settings_init(void)
{
    uint32 volume = r_u8(0x80081707u);
    uint32 node = r_u32(0x8007672Cu);
    w_u32(0x8004F69Cu, r_u8(0x80081708u) * 16384u / 255u);
    w_u16(0x80076732u, volume * 16384u / 255u);
    while (node) {
        /* TODO Apply volume to any active GTOS sequence */
        gt_draft_unresolved_call(0x8003D21Cu, node, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        node = r_u32(node);
    }
    volume = volume * 32767u / 255u;
    w_u16(0x8004F6A0u, volume);
    w_u16(0x1F801DB0u, volume);
    w_u16(0x1F801DB2u, volume);
}

void gtos_sequence_init(void)
{
    uint32 voice;
    if (r_u32(0x80073744u)) return;
    memset(psx_addr(0x8007672Cu, 0x2D4u), 0, 0x2D4u);
    w_u32(0x8007672Cu, 0u);
    w_u16(0x80076732u, 0x3FFFu);
    w_u8(0x80076734u, 0u);
    w_u8(0x8007675Fu, 0xFFu);
    for (voice = 0u; voice < 24u; ++voice)
        w_u8(0x80076760u + voice * 28u + 27u, 0xFFu);
    w_u16(0x80076730u, 60u);
    w_u32(0x80073744u, 1u);
}

void gtos_reverb_init(void)
{
    uint32 i, table = 0x80073768u + 4u * 64u;
    w_u16(0x1F801D84u, 0u);
    w_u16(0x1F801D86u, 0u);
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) & 0xFF7Fu);
    w_u16(0x1F801DA2u, r_u16(0x80073754u + 8u));
    /* Preserve the SDK's halfword register writes for reverb mode four */
    for (i = 0u; i < 32u; ++i)
        w_u16(0x1F801DC0u + i * 2u, r_u16(table + i * 2u));
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 0x80u);
    w_u8(0x8007635Du, 1u);
}

static void gtos_spu_complete(void)
{
    uint32 registers = r_u32(0x80074114u);
    uint32 callback = r_u32(0x8007414Cu);
    w_u16(registers + 426u, r_u16(registers + 426u) & 0xFFCFu);
    if (callback)
        gt_draft_unresolved_call(callback, 0xF0000000u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    else w_u32(0x80074148u, 1u);
}

void gtos_sound_init(void)
{
    uint32 registers, i;
    SpuNativeTransferGuestBinding binding = {
        0x80074184u, 0x80074130u, 0x8007412Cu, 0x80074148u,
        0x8007414Cu, 0x80074168u, 0x8007416Cu, 3u
    };
    if (r_u32(0x80073734u)) return;
    ResetCallback();
    gt_native_spu_init();
    registers = r_u32(0x80074114u);
    w_u32(r_u32(0x80074124u), r_u32(r_u32(0x80074124u)) | 0xB0000u);
    w_u32(0x80074130u, 0u);
    w_u32(0x80074134u, 0u);
    w_u16(0x8007412Cu, 0x200u);
    w_u32(0x80074138u, 2u);
    w_u32(0x8007413Cu, 3u);
    w_u32(0x80074140u, 8u);
    w_u32(0x80074144u, 7u);
    w_u16(registers + 428u, 4u);
    w_u16(registers + 388u, 0u);
    w_u16(registers + 390u, 0u);
    w_u16(registers + 396u, 0xFFFFu);
    w_u16(registers + 398u, 0xFFFFu);
    w_u16(registers + 408u, 0u);
    w_u16(registers + 410u, 0u);
    memset(psx_addr(0x80078540u, 20u), 0, 20u);
    for (i = 400u; i <= 406u; i += 2u) w_u16(registers + i, 0u);
    for (i = 432u; i <= 438u; i += 2u) w_u16(registers + i, 0u);
    if (!spu_upload(0x1000u, psx_addr(0x80074154u, 16u), 16u))
        gt_draft_unresolved_call(0x80045720u, 0x80074154u, 16u, 0u, 0u, 0u, 0u, 0u, 0u);
    for (i = 0u; i < 24u; ++i) {
        uint32 voice = registers + i * 16u;
        w_u16(voice, 0u);
        w_u16(voice + 2u, 0u);
        w_u16(voice + 4u, 0x3FFFu);
        w_u16(voice + 6u, 0x200u);
        w_u16(voice + 8u, 0u);
        w_u16(voice + 10u, 0u);
        w_u16(0x800741B0u + i * 2u, 0xC000u);
    }
    w_u16(registers + 392u, 0xFFFFu);
    w_u16(registers + 394u, 0xFFu);
    w_u16(registers + 396u, 0xFFFFu);
    w_u16(registers + 398u, 0xFFu);
    w_u32(0x80074148u, 1u);
    w_u16(registers + 426u, 0xC000u);
    w_u32(0x8007414Cu, 0u);
    w_u32(0x80074150u, 0u);
    w_u32(0x800741E4u, 1u);
    DMACallback(4u, gtos_spu_complete);
    /* Native completion uses the game flag without a BIOS event */
    w_u32(0x8007417Cu, 0u);
    w_u32(0x80074188u, 0u);
    w_u32(0x8007418Cu, 0u);
    memset(psx_addr(0x80074198u, 16u), 0, 16u);
    w_u32(0x80074190u, r_u32(0x800741F4u));
    w_u16(registers + 418u, r_u32(0x800741F4u));
    memset(psx_addr(0x80074224u, 12u), 0, 12u);
    w_u32(0x80074184u, 0u);
    w_u32(0x80074130u, 0u);
    w_u32(0x80074180u, 0u);
    w_u32(0x800741ACu, 0u);
    w_u32(0x800741A8u, 0u);
    w_u32(0x800741E0u, 0u);
    if (!spu_bind_native_transfer_guest(&binding))
        gt_draft_unresolved_call(0x80045FE0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u16(registers + 384u, 0x3FFFu);
    w_u16(registers + 386u, 0x3FFFu);
    memset(psx_addr(0x8007635Cu, 0x3C8u), 0, 0x3C8u);
    w_u8(0x8007635Cu, 23u);
    w_u16(0x80076360u, 60u);
    w_u16(0x80076362u, 256u);
    w_u32(0x80073734u, 1u);
}
