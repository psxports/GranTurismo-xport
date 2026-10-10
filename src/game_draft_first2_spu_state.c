#include "game_first2_registry.h"

static uint32 spu_callback_slot(uint32 index, uint32 token)
{
    /* TODO Bind the original callback slot service with its real previous token */
    (void)index; (void)token;
    gt_draft_failure("Native SPU callback slot adapter", 0x8008A2A8u);
}
uint32 gtmain_spu_init_cold(void)
{
    return gtmain_spu_init_state(0);
}
uint32 gtmain_spu_init_state(uint32 mode)
{
    uint32 value, result;
    (void)ResetCallback();
    (void)gtmain_spu_chip_init(mode);
    if (!mode) {
        uint32 address = 0x800B240Eu, count;
        for (count = 0; count < 24; ++count) { w_u16(address, 0xC000u); address -= 2u; }
    }
    (void)gtmain_spu_driver_start();
    value = r_u32(0x800B2424u);
    w_u32(0x800B23B8u, 0); w_u32(0x800B23BCu, 0);
    w_u32(0x800B23C8u, 0); w_u16(0x800B23CCu, 0); w_u16(0x800B23CEu, 0);
    w_u32(0x800B23D0u, 0); w_u32(0x800B23D4u, 0); w_u32(0x800B23C0u, value);
    result = gtmain_spu_register_write(209, value, 0);
    w_u32(0x800B2454u, 0); w_u32(0x800B2458u, 0); w_u32(0x800B245Cu, 0);
    w_u32(0x800B23B4u, 0); w_u32(0x800B2360u, 0); w_u32(0x800B23B0u, 0);
    w_u32(0x800B23DCu, 0); w_u32(0x800B23D8u, 0); w_u32(0x800B2410u, 0);
    return result;
}
uint32 gtmain_spu_callback_bind(uint32 token)
{
    return spu_callback_slot(4, token);
}
uint32 gtmain_spu_read(uint32 guest_destination, uint32 bytes)
{
    if (bytes > 0x7EFF0u) bytes = 0x7EFF0u;
    (void)gtmain_spu_dma_read(guest_destination, bytes);
    if (!r_u32(0x800B237Cu)) w_u32(0x800B2378u, 0);
    return bytes;
}
