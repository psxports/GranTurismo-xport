#include "game_first2_registry.h"
#include <stdio.h>

static void interrupt_bus(uint32 address)
{
    /* TODO Bind actual native IRQ DMA and timer register services */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native interrupt MMIO adapter", address);
}
static uint32 interrupt_read16(uint32 address) { interrupt_bus(address); return r_u16(address); }
static uint32 interrupt_read32(uint32 address) { interrupt_bus(address); return r_u32(address); }
static void interrupt_write16(uint32 address, uint32 value) { interrupt_bus(address); w_u16(address, value); }
static void interrupt_write32(uint32 address, uint32 value) { interrupt_bus(address); w_u32(address, value); }
static uint32 interrupt_bios(uint32 entry, uint32 argument0, uint32 argument1)
{
    /* TODO Bind actual BIOS context and interrupt services without saved register frames */
    (void)argument0; (void)argument1;
    gt_draft_failure("Native interrupt BIOS adapter", entry);
}
static void interrupt_notify(uint32 token)
{
    if (token == 0x8008ABA0u) (void)gtmain_vblank_interrupt_dispatch();
    else if (token == 0x8008ACB4u) (void)gtmain_dma_interrupt_dispatch();
    else if (token == 0x80080684u) (void)gtmain_pad_poll();
    else if (token == 0x8008061Cu) (void)gtmain_pad_irq_test();
    else gt_draft_failure("Native interrupt callback token", token);
}
static uint32 interrupt_bind_boundary(uint32 index, uint32 token)
{
    /* TODO Bind unselected original callback table dispatcher */
    (void)index; (void)token;
    gt_draft_failure("Native interrupt callback table adapter", 0x8008A278u);
}
uint32 gtmain_interrupt_words_clear(uint32 words, uint32 count)
{
    uint32 remaining = count - 1u;
    if (count) do {
        w_u32(words, 0); --remaining; words += 4u;
    } while (remaining != 0xFFFFFFFFu);
    return remaining;
}
uint32 gtmain_vblank_callbacks_clear(uint32 words, uint32 count)
{
    uint32 remaining = count - 1u;
    if (count) do {
        w_u32(words, 0); --remaining; words += 4u;
    } while (remaining != 0xFFFFFFFFu);
    return remaining;
}
uint32 gtmain_dma_callbacks_clear(uint32 words, uint32 count)
{
    uint32 remaining = count - 1u;
    if (count) do {
        w_u32(words, 0); --remaining; words += 4u;
    } while (remaining != 0xFFFFFFFFu);
    return remaining;
}
uint32 gtmain_vblank_callback(uint32 index, uint32 token)
{
    uint32 location = 0x800B3884u + 4u * index;
    uint32 previous = r_u32(location);
    if (previous != token) w_u32(location, token);
    return previous;
}
uint32 gtmain_vblank_interrupt_dispatch(void)
{
    uint32 index = 0, table = 0x800B3884u;
    w_u32(0x800B38A4u, r_u32(0x800B38A4u) + 1u);
    do {
        uint32 token = r_u32(table);
        if (token) interrupt_notify(token);
        ++index; table += 4u;
    } while (index < 8u);
    return 0;
}
uint32 gtmain_vblank_interrupt_start(void)
{
    interrupt_write32(r_u32(0x800B38A8u), 263);
    w_u32(0x800B38A4u, 0);
    (void)gtmain_vblank_callbacks_clear(0x800B3884u, 8);
    (void)interrupt_bind_boundary(0, 0x8008ABA0u);
    return 0x8008AC0Cu;
}
uint32 gtmain_dma_interrupt_start(void)
{
    (void)gtmain_dma_callbacks_clear(0x800B38B8u, 8);
    interrupt_write32(r_u32(0x800B38B4u), 0);
    (void)interrupt_bind_boundary(3, 0x8008ACB4u);
    return 0x8008AE34u;
}
uint32 gtmain_interrupt_start(void)
{
    uint32 status, mask, result;
    if (r_u16(0x800B27ACu)) return 0;
    status = r_u32(0x800B3838u); mask = r_u32(0x800B383Cu);
    interrupt_write16(mask, 0);
    interrupt_write16(status, interrupt_read16(mask));
    interrupt_write32(r_u32(0x800B3840u), 0x33333333u);
    (void)gtmain_interrupt_words_clear(0x800B27ACu, 1050);
    result = interrupt_bios(0x8008B838u, 0x800B27E4u, 0);
    if (result) (void)gtmain_interrupt_dispatch();
    w_u32(0x800B27E8u, 0x800B37C4u);
    gtmain_bios_exception_hook(0x800B27E4u);
    w_u16(0x800B27ACu, 1);
    result = gtmain_vblank_interrupt_start();
    w_u32(r_u32(0x800B3834u) + 20u, result);
    result = gtmain_dma_interrupt_start();
    w_u32(r_u32(0x800B3834u) + 4u, result);
    gtmain_bios_cd_remove();
    /* TODO Bind unselected original interrupt finalization */
    gt_draft_failure("Native interrupt finalization adapter", 0x8008AF28u);
    return 0x800B27ACu;
}
uint32 gtmain_interrupt_stop(void)
{
    uint32 mask, status, dma;
    if (!r_u16(0x800B27ACu)) return 0;
    /* TODO Bind unselected original interrupt suspension */
    gt_draft_failure("Native interrupt suspension adapter", 0x8008AF08u);
    mask = r_u32(0x800B383Cu); dma = r_u32(0x800B3840u);
    w_u16(0x800B27DEu, interrupt_read16(mask));
    status = r_u32(0x800B3838u);
    w_u32(0x800B27E0u, interrupt_read32(dma));
    interrupt_write16(mask, 0);
    interrupt_write16(status, interrupt_read16(mask));
    dma = r_u32(0x800B3840u);
    interrupt_write32(dma, interrupt_read32(dma) & 0x77777777u);
    gtmain_bios_exception_reset();
    w_u16(0x800B27ACu, 0);
    return 0x800B27ACu;
}
uint32 gtmain_interrupt_callback(uint32 index, uint32 callback_token)
{
    uint32 location = 0x800B27B0u + 4u * index, old = r_u32(location);
    uint32 mask, previous_mask, selected_mask, bit = 1u << (index & 31u);
    if (callback_token == old || !r_u16(0x800B27ACu)) return old;
    mask = r_u32(0x800B383Cu); previous_mask = interrupt_read16(mask);
    interrupt_write16(mask, 0);
    if (callback_token) {
        w_u32(location, callback_token); selected_mask = previous_mask | bit;
        w_u16(0x800B27DCu, r_u16(0x800B27DCu) | bit);
    } else {
        w_u32(location, 0); selected_mask = previous_mask & ~bit;
        w_u16(0x800B27DCu, r_u16(0x800B27DCu) & ~bit);
    }
    if (!index) {
        (void)interrupt_bios(0x8008B2C8u, !callback_token, 0);
        (void)interrupt_bios(0x8008B2B8u, 3, !callback_token);
    }
    if (index == 4u) (void)interrupt_bios(0x8008B2B8u, 0, !callback_token);
    if (index == 5u) (void)interrupt_bios(0x8008B2B8u, 1, !callback_token);
    if (index == 6u) (void)interrupt_bios(0x8008B2B8u, 2, !callback_token);
    interrupt_write16(r_u32(0x800B383Cu), selected_mask);
    return old;
}
uint32 gtmain_interrupt_dispatch(void)
{
    uint32 pending, status, mask, index, table;
    if (!r_u16(0x800B27ACu)) {
        printf("unexpected interrupt(%04x)\n", (unsigned)interrupt_read16(r_u32(0x800B3838u)));
        (void)gtmain_bios_exception_return();
    }
    status = r_u32(0x800B3838u);
    pending = r_u16(0x800B27DCu);
    w_u16(0x800B27AEu, 1); mask = r_u32(0x800B383Cu);
    pending &= interrupt_read16(status); pending &= interrupt_read16(mask);
    while (pending) {
        index = 0; table = 0x800B27B0u;
        while ((pending & 0xFFFFu) && index < 11u) {
            if (pending & 1u) {
                interrupt_write16(r_u32(0x800B3838u), ~(1u << index));
                { uint32 token = r_u32(table); if (token) interrupt_notify(token); }
            }
            table += 4u; pending >>= 1; ++index;
        }
        status = r_u32(0x800B3838u); pending = r_u16(0x800B27DCu);
        mask = r_u32(0x800B383Cu);
        pending &= interrupt_read16(status); pending &= interrupt_read16(mask);
    }
    status = r_u32(0x800B3838u); mask = r_u32(0x800B383Cu);
    pending = interrupt_read16(status); pending &= interrupt_read16(mask);
    if (pending) {
        uint32 count = r_u32(0x800B3844u);
        w_u32(0x800B3844u, count + 1u);
        if ((sint32)count >= 2049) {
            uint32 status_value = interrupt_read16(status), mask_value = interrupt_read16(mask);
            printf("intr timeout(%04x:%04x)\n", (unsigned)status_value, (unsigned)mask_value);
            status = r_u32(0x800B3838u);
            w_u32(0x800B3844u, 0); interrupt_write16(status, 0);
        }
    } else w_u32(0x800B3844u, 0);
    w_u16(0x800B27AEu, 0);
    return gtmain_bios_exception_return();
}
uint32 gtmain_dma_interrupt_dispatch(void)
{
    uint32 pending, index, table, dma = r_u32(0x800B38B4u), value, result;
    pending = (interrupt_read32(dma) >> 24) & 127u;
    while (pending) {
        index = 0; table = 0x800B38B8u;
        while (pending && index < 7u) {
            if (pending & 1u) {
                dma = r_u32(0x800B38B4u);
                value = interrupt_read32(dma);
                interrupt_write32(dma, value & ((1u << (index + 24u)) | 0xFFFFFFu));
                { uint32 token = r_u32(table); if (token) interrupt_notify(token); }
            }
            table += 4u; pending >>= 1; ++index;
        }
        pending = (interrupt_read32(r_u32(0x800B38B4u)) >> 24) & 127u;
    }
    dma = r_u32(0x800B38B4u); value = interrupt_read32(dma);
    if ((value & 0xFF000000u) == 0x80000000u) result = 1;
    else result = interrupt_read32(dma) & 0x8000u;
    if (result) {
        printf("DMA bus error: code=%08x\n", (unsigned)interrupt_read32(dma));
        for (index = 0; index < 7u; ++index) {
            uint32 address = r_u32(0x800B38D8u) + 16u * index;
            printf("MADR[%d]=%08x\n", (int)index, (unsigned)interrupt_read32(address));
        }
        result = 0;
    }
    return result;
}
