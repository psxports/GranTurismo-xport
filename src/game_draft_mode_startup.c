#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include <stdio.h>
#include <string.h>

static void gtmain_mode_audio_prepare(void)
{
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 1u);
    if ((sint16)r_u16(0x800C0C02u)) {
        uint32 entry = r_u32(0x800C0C34u), value = 0u;
        CdlFILE file;
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
        if (!value || !CdSearchFile(&file, (const char *)psx_addr(value, 1u))) {
            w_u32(0x800B1AB0u, 2u);
            gt_draft_unresolved_call(0x8007196Cu, value, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
        w_u32(0x800B1AB0u, 0u);
        {
            uint32 minutes = (file.pos.minute >> 4u) * 10u + (file.pos.minute & 15u);
            uint32 seconds = (file.pos.second >> 4u) * 10u + (file.pos.second & 15u);
            uint32 frames = (file.pos.sector >> 4u) * 10u + (file.pos.sector & 15u);
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

static uint32 gtmain_mode_menu_session(void)
{
    /* TODO Unselected mode2 session remains typed fail-fast */
    gt_draft_failure("GTMAIN mode2 session", 0x80027E18u);
}

/* GTMAIN.EXE:8002881C */
uint32 gtmain_mode_init(void)
{
    uint32 mode;
    /* TODO Native bootstrap and sound setup adapters remain partial */
    gtmain_heap_reset();
    gtmain_mode_prepare();
    gtmain_menu_status_reset();
    gtmain_sound_bank_load(0x800B49ECu, 0x1010u, 0x801BB70Cu, 0x500u, 0u);
    mode = r_u8(0x800CB7F4u);
    w_u32(0x800B6390u, 0x8008BE68u);
    w_u8(0x800B63DCu, 0u);
    if (mode == 2u) {
        w_u32(0x800B6390u, 0x8008BE74u);
        return gtmain_mode_menu_session();
    }
    if (mode == 3u || mode == 4u) {
        if (mode == 4u) w_u8(0x800B63DCu, 1u);
        return (uint32)gtmain_race_session_restore();
    }
    if (mode == 1u)
        return (uint32)gtmain_race_session_enter();
    return 1u;
}
