#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_spu.h"

static uint32 dma_read16(uint32 address)
{
    uint32 value;
    if (address >= 0x1F801C00u && address < 0x1F801E00u) {
        if (!spu_mmio_read(NULL, address, 2, &value))
            gt_draft_failure("Native SPU halfword read", address);
        return value;
    }
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native SPU DMA halfword adapter", address);
    return r_u16(address);
}
static void dma_write16(uint32 address, uint32 value)
{
    if (address >= 0x1F801C00u && address < 0x1F801E00u) {
        if (!spu_mmio_write(NULL, address, 2, value, 3))
            gt_draft_failure("Native SPU halfword write", address);
        return;
    }
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native SPU DMA halfword adapter", address);
    w_u16(address, value);
}
static uint32 dma_read32(uint32 address)
{
    /* TODO Bind actual DMA controller word access */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native SPU DMA word read", address);
    return r_u32(address);
}
static void dma_write32(uint32 address, uint32 value)
{
    /* TODO Bind actual DMA controller word access */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native SPU DMA word write", address);
    w_u32(address, value);
}
uint32 gtmain_spu_dma_write_mode(void)
{
    uint32 address = r_u32(0x800B2358u);
    uint32 value = (dma_read32(address) & 0xF0FFFFFFu) | 0x20000000u;
    dma_write32(address, value); return value;
}
uint32 gtmain_spu_dma_read_mode(void)
{
    uint32 address = r_u32(0x800B2358u);
    uint32 value = (dma_read32(address) & 0xF0FFFFFFu) | 0x22000000u;
    dma_write32(address, value); return value;
}
uint32 gtmain_spu_register_write(uint32 index, uint32 value, uint32 scaled)
{
    uint32 offset = index << 1, address;
    address = offset + r_u32(0x800B2344u);
    if (scaled) value >>= r_u32(0x800B236Cu) & 31u;
    dma_write16(address, value);
    return address;
}
sint32 gtmain_spu_dma_control(sint32 mode, uint32 address, uint32 bytes)
{
    uint32 base, desired, count, value, captured;
    if (mode == 2) {
        value = address >> (r_u32(0x800B236Cu) & 31u);
        base = r_u32(0x800B2344u);
        w_u16(0x800B235Cu, value); dma_write16(base + 422u, value);
        return 0;
    }
    if (mode == 0 || mode == 1) {
        base = r_u32(0x800B2344u); captured = r_u16(0x800B235Cu);
        value = dma_read16(base + 422u);
        w_u32(0x800B2394u, mode == 0);
        count = 0;
        if (value != captured) {
            ++count;
            for (;;) {
                if (count >= 0xF01u) return -2;
                value = dma_read16(base + 422u);
                if (value == captured) break;
                ++count;
            }
        }
        base = r_u32(0x800B2344u); value = dma_read16(base + 426u);
        value = mode == 0 ? value | 0x30u : (value & 0xFFCFu) | 0x20u;
        dma_write16(base + 426u, value); return 0;
    }
    if (mode != 3) return 0;
    desired = r_u32(0x800B2394u) == 1u ? 48u : 32u;
    base = r_u32(0x800B2344u); count = 0;
    value = dma_read16(base + 426u) & 48u;
    ++count;
    while (value != desired) {
        if (count >= 0xF01u) return -2;
        value = dma_read16(base + 426u) & 48u;
        if (value == desired) break;
        ++count;
    }
    if (r_u32(0x800B2394u) == 1u) (void)gtmain_spu_dma_read_mode();
    else (void)gtmain_spu_dma_write_mode();
    w_u32(0x800B2398u, address);
    {
        uint32 target = r_u32(0x800B2348u);
        uint32 blocks = (bytes >> 6) + ((bytes & 63u) != 0);
        uint32 published_address = r_u32(0x800B2398u);
        w_u32(0x800B239Cu, blocks); dma_write32(target, published_address);
    }
    value = r_u32(0x800B239Cu);
    dma_write32(r_u32(0x800B234Cu), (value << 16) | 16u);
    value = r_u32(0x800B2394u) == 1u ? 0x01000200u : 0x01000201u;
    dma_write32(r_u32(0x800B2350u), value);
    return 0;
}
uint32 gtmain_spu_dma_read(uint32 destination, uint32 bytes)
{
    uint32 transfer = r_u16(0x800B235Cu);
    uint32 shift = r_u32(0x800B236Cu) & 31u;
    (void)gtmain_spu_dma_control(2, transfer << shift, 0);
    /* Mode zero does not consume the original residual argument registers */
    (void)gtmain_spu_dma_control(0, 0, 0);
    (void)gtmain_spu_dma_control(3, destination, bytes);
    return bytes;
}
static uint32 dma_missing(uint32 entry, uint32 a0, uint32 a1, uint32 a2, uint32 a3)
{
    /* TODO Bind actual SDK or BIOS result without fabricated completion */
    (void)a0; (void)a1; (void)a2; (void)a3;
    gt_draft_failure("Native SPU event callback adapter", entry);
}
uint32 gtmain_spu_driver_start(void)
{
    uint32 result = r_u32(0x800B2414u);
    if (result) return result;
    w_u32(0x800B2414u, 1);
    (void)dma_missing(0x8008AF08u, 0, 0, 0, 0);
    (void)dma_missing(0x80086748u, 0x80085E48u, 0, 0, 0);
    result = dma_missing(0x8008B278u, 0xF0000009u, 32, 0x2000u, 0);
    w_u32(0x800B23ACu, result);
    (void)dma_missing(0x8008B288u, result, 0, 0, 0);
    return dma_missing(0x8008AF28u, 0, 0, 0, 0);
}
