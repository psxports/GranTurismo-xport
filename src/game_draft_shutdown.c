#include "game_draft_registry.h"
#include "game_first2_registry.h"

uint32 gtmain_shutdown(void)
{
    uint32 i;
    sint32 frames = (sint32)r_u32(0x800C0A88u);
    for (i = 0u; (sint32)i < (sint16)r_u16(0x800BC1A6u); ++i) {
        (void)gtmain_race_sound_car_stop(0x800B6850u + i * 0xD34u);
    }
    w_u32(0x800C4C00u, 0x47546F73u);
    w_u32(0x800C4C40u, r_u32(0x800C4C40u) + (uint32)(frames / 60));
    w_u32(0x800CFF98u, gtmain_data_checksum(0x800C4C00u, 0xB398u));
    for (i = 0u; i < 24u; ++i) {
        w_u16(0x1F801C02u + i * 16u, 0u);
        w_u16(0x1F801C00u + i * 16u, 0u);
    }
    w_u32(0x800B235Cu, 0x2000u >> 3u);
    w_u32(0x800B23B4u, 0u);
    w_u32(0x800B2360u, 0u);
    if (SpuWrite(psx_addr(0x800C4C00u, 0xB3C0u), 0xB3C0u) != 0xB3C0u)
        return gt_draft_unresolved_call(0x8006A150u, 0x800C4C00u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    for (i = 0u; i < 8u; ++i) {
        uint32 handle = r_u32(0x800C0A60u + i * 4u);
        DisableEvent(handle);
        CloseEvent(handle);
    }
    /* Guest CD and card kernel teardown is unnecessary for native host services */
    if (r_u32(0x800B1BE4u)) {
        w_u32(0x800B1BE4u, 0u);
        for (i = 0u; i < 24u; ++i) {
            uint32 voice = 0x800B55D4u + i * 40u;
            uint32 owner = r_u32(voice + 20u);
            w_u8(voice + 5u, 2u);
            w_u32(voice + 8u, 0u);
            w_u8(voice + 7u, 1u);
            w_u8(voice + 6u, 0u);
            w_u8(voice + 4u, 0u);
            w_u8(voice, 0u);
            if (owner) {
                w_u8(owner, 0xFFu);
                w_u32(voice + 20u, 0u);
            }
        }
        gtmain_sound_voice_tick();
        if (r_u8(0x800B55CDu)) {
            gtmain_sound_reverb(0u, 0u, 0u);
            gtmain_sound_reverb_enable(0u);
        }
        w_u32(0x800B2414u, 0u);
        w_u32(0x800B237Cu, 0u);
        w_u32(0x800B2380u, 0u);
        DMACallback(4u, NULL);
    }
    if (r_u32(0x800B1A24u)) {
        w_u32(0x800B1A24u, 0u);
        VSyncCallback(NULL);
    }
    StopCallback();
    return 0u;
}
