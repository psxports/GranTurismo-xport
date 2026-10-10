#include "game_first2_registry.h"

static void irq_leaf_boundary(uint32 address)
{
    /* TODO Bind actual IRQ and DMA register access */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native IRQ DMA register adapter", address);
}
uint32 gtmain_interrupt_callback_enabled(void)
{
    return r_u16(0x800B27AEu);
}
uint32 gtmain_interrupt_mask_exchange(uint16 mask)
{
    uint32 address = r_u32(0x800B383Cu), previous;
    irq_leaf_boundary(address);
    previous = r_u16(address);
    w_u16(address, mask);
    return previous;
}
uint32 gtmain_dma_callback_exchange(uint32 index, uint32 token)
{
    uint32 slot = 0x800B38B8u + index * 4u;
    uint32 previous = r_u32(slot), address, value, bit;
    if (token == previous) return previous;
    address = r_u32(0x800B38B4u);
    w_u32(slot, token);
    irq_leaf_boundary(address);
    value = r_u32(address);
    bit = 1u << ((index + 16u) & 31u);
    if (token) value = (value & 0x00FFFFFFu) | bit | 0x00800000u;
    else value = ((value & 0x00FFFFFFu) | 0x00800000u) & ~bit;
    w_u32(address, value);
    return previous;
}
