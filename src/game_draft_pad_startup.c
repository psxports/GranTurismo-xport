#include "game_draft_registry.h"
#include <string.h>

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

static void gtmain_pad_library_init(uint32 first_buffer, uint32 second_buffer)
{
    uint32 index, state, cursor;
    static const uint32 callbacks[] = {
        0x8007FB90u, 0x8007FB28u, 0x8007FC94u,
        0x8007FD50u, 0x8007FFCCu, 0x80080004u
    };
    w_u32(0x800B1EF4u, 0u);
    w_u32(0x800B1F08u, 0u);
    w_u32(0x800B1ED4u, 0x800810ECu);
    w_u32(0x800B1ED8u, 0x80081500u);
    w_u32(0x800B1EDCu, 0x800811C0u);
    for (index = 0u; index < 6u; ++index)
        w_u32(0x800B1EBCu + index * 4u, callbacks[index]);
    w_u32(0x800B1EF0u, 0x800B5D0Cu);
    w_u32(0x800B1EE0u, 0x8007FC84u);
    memset(psx_addr(0x800B5D0Cu, 480u), 0, 480u);
    w_u32(0x800B5D3Cu, first_buffer);
    w_u32(0x800B5E2Cu, second_buffer);
    for (index = 0u; index < 2u; ++index) {
        state = 0x800B5D0Cu + index * 240u;
        w_u32(state + 12u, 0u);
        w_u32(state + 16u, state);
        w_u8(r_u32(state + 48u), 0xFFu);
        w_u8(r_u32(state + 48u) + 1u, 0u);
        w_u32(state + 60u, 0x800B5C7Cu + index * 35u);
        w_u32(state + 64u, 0x800B5CC4u + index * 35u);
        for (cursor = 0u; cursor < 6u; ++cursor)
            w_u8(state + 93u + cursor, 0xFFu);
    }
    w_u32(0x800B5EF0u, 0x80080684u);
    w_u32(0x800B5EF4u, 0x8008061Cu);
    w_u32(0x800B5EECu, 0u);
    w_u32(0x800B5EF8u, 0u);
    w_u32(0x800B1EF4u, 1u);
}

static void gtmain_pad_reset_port(uint32 state)
{
    uint32 index;
    if (!r_u8(state + 73u)) return;
    w_u8(state + 73u, 0u);
    w_u8(state + 70u, 0u);
    w_u16(state + 230u, 0u);
    w_u32(state + 20u, 0u);
    w_u32(state + 24u, 0u);
    w_u8(state + 227u, 0u);
    w_u8(state + 228u, 0u);
    w_u16(state + 230u, 0u);
    w_u8(state + 233u, 0u);
    w_u8(state + 234u, 0u);
    w_u32(state, 0u);
    w_u32(state + 4u, 0u);
    w_u32(state + 8u, 0u);
    for (index = 0u; index < 6u; ++index) w_u8(state + 93u + index, 0xFFu);
}

static void gtmain_pad_start(void)
{
    uint32 interrupt, callback, state;
    w_u32(0x800B1EF4u, 0u);
    xport_bios_enter_critical();
    SysDeqIntRP(2u, 0x800B5EECu);
    SysEnqIntRP(2u, 0x800B5EECu);
    interrupt = r_u32(0x800B1F1Cu);
    w_u32(interrupt, 0xFFFFFFFEu);
    w_u32(interrupt + 4u, r_u32(interrupt + 4u) | 1u);
    ChangeClearRCnt(3u, 0u);
    xport_bios_exit_critical();
    callback = r_u32(0x800B1EC0u);
    state = r_u32(0x800B1EF0u);
    if (callback == 0x8007FB28u) gtmain_pad_reset_port(state);
    else gt_draft_unresolved_call(callback, state, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    callback = r_u32(0x800B1EC0u);
    state = r_u32(0x800B1EF0u) + 240u;
    if (callback == 0x8007FB28u) gtmain_pad_reset_port(state);
    else gt_draft_unresolved_call(callback, state, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x800B5F00u, 0u);
    w_u32(0x800B5EFCu, 0u);
    w_u32(0x800B1EF4u, 1u);
    /* TODO Dispatch the registered controller IRQ callbacks from native input */
}

void gtmain_pad_init(void)
{
    sub_8007BD58(0x800C0A90u, 0u, 96u);
    gtmain_pad_library_init(0x800C0AF0u, 0x800C0B30u);
    if (!InitPAD(0x800C0AF0u, 64u, 0x800C0B30u, 64u) || !StartPAD())
        gt_draft_unresolved_call(0x80080804u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    /* TODO Connect SDK port processing to the native controller packets */
}
