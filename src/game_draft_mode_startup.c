#include "game_draft_registry.h"
#include <stdio.h>
#include <string.h>

static void gtmain_mode_audio_prepare(void)
{
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 1u);
    if ((sint16)r_u16(0x800C0C02u)) {
        uint32 entry = r_u32(0x800C0C34u), value = 0u;
        uint8 file[24];
        while (r_u8(entry)) {
            uint32 length = 0u, i;
            while (r_u8(entry + length)) ++length;
            if (length > 10u && r_u8(entry + 10u) == '=') {
                for (i = 0u; i < 10u; ++i)
                    if (r_u8(entry + i) != (uint8)"music.file"[i]) break;
                if (i == 10u) { value = entry + 11u; break; }
            }
            entry += length + 1u;
        }
        if (!value || !CdSearchFilePSX(file, (const char *)psx_addr(value, 1u))) {
            w_u32(0x800B1AB0u, 2u);
            gt_draft_unresolved_call(0x8007196Cu, value, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
        w_u32(0x800B1AB0u, 0u);
        {
            uint32 minutes = (file[0] >> 4u) * 10u + (file[0] & 15u);
            uint32 seconds = (file[1] >> 4u) * 10u + (file[1] & 15u);
            uint32 frames = (file[2] >> 4u) * 10u + (file[2] & 15u);
            w_u32(0x800994A4u, (minutes * 60u + seconds) * 75u + frames - 150u);
        }
    }
}

static void gtmain_mode_audio_defaults(void)
{
    uint32 reverb = (uint32)r_u8(0x800CB768u) * 16384u / 255u;
    uint32 volume = (uint32)r_u8(0x800CB767u) * 32767u / 255u;
    w_u32(0x80099548u, reverb);
    w_u16(0x800994BCu, volume);
    w_u16(0x1F801DAAu, (r_u16(0x1F801DAAu) & 0xFFFBu) | 1u);
}

static uint32 gtmain_mode_prepare(void)
{
    uint32 end = r_u32(0x800B1AACu) + r_u32(0x800B1AA8u);
    printf("Heap: area = %08X, size = %08X, end = %08X\n", r_u32(0x800B1AACu), r_u32(0x800B1AA8u), end);
    printf("Menu: other = %08X\n", 0x28730u);
    if (end > 0x801EFF00u) printf("Warning: not enough memory\n");
    printf("GAMESTATUS: %d byte\n", 27560);
    gtmain_display_init(1u);
    w_u32(0x800BFF70u, 0u);
    SetDispMask(0);
    VSync(0);
    gtmain_font_init(5u);
    gtmain_initial_frame();
    gtmain_mode_audio_prepare();
    gtmain_mode_audio_defaults();
    memset(psx_addr(0x800B6650u, 12u), 0, 12u);
    memcpy(psx_addr(0x800BC160u, 69u), psx_addr(0x80093C08u, 69u), 69u);
    return 0x800BC160u;
}

static uint32 gtmain_mode_defaults(void)
{
    uint32 a = r_u32(0x80099490u), b = r_u32(0x80099494u);
    w_u32(0x800BFF80u, a);
    w_u32(0x800BFF84u, b);
    a = r_u32(0x80099498u); b = r_u32(0x8009949Cu);
    w_u32(0x800BFF88u, a);
    w_u32(0x800BFF8Cu, b);
    w_u32(0x800BFF90u, r_u32(0x800994A0u));
    if (!r_u16(0x800CB758u)) w_u16(0x800CB758u, 214u);
    w_u32(0x800B6318u, 2u);
    w_u8(0x800B6310u, 0u);
    w_u8(0x800B63BFu, 0u);
    w_u8(0x800B63BEu, 6u);
    return 6u;
}

uint32 gtmain_mode_init(void)
{
    uint32 mode;
    gtmain_heap_reset();
    gtmain_mode_prepare();
    gtmain_mode_defaults();
    gtmain_sound_bank_load(0x800B49ECu, 0x1010u, 0x801BB70Cu, 0x500u, 0u);
    mode = r_u8(0x800CB7F4u);
    w_u32(0x800B6390u, 0x8008BE68u);
    w_u8(0x800B63DCu, 0u);
    if (mode == 2u) {
        w_u32(0x800B6390u, 0x8008BE74u);
        return gt_draft_unresolved_call(0x80027E18u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    if (mode == 3u || mode == 4u) {
        if (mode == 4u) w_u8(0x800B63DCu, 1u);
        return gt_draft_unresolved_call(0x80028598u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    if (mode == 1u)
        return gt_draft_unresolved_call(0x80027640u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return 1u;
}
