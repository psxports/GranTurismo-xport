#include "game_first2_registry.h"
#include "psx_spu.h"
#include <stdio.h>

static void controller_boundary(uint32 address)
{
    /* TODO Bind actual indexed CD controller access without synthetic responses */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native indexed CD controller adapter", address);
}
static uint32 controller_read8(uint32 address) { controller_boundary(address); return r_u8(address); }
static void controller_write8(uint32 address, uint32 value) { controller_boundary(address); w_u8(address, value); }
static void controller_write32(uint32 address, uint32 value) { controller_boundary(address); w_u32(address, value); }
static uint32 controller_spu_read(uint32 address)
{
    uint32 value;
    if (address >= 0x1F801C00u && address < 0x1F801E00u) {
        if (!spu_mmio_read(NULL, address, 2, &value)) gt_draft_failure("Native CD SPU halfword read", address);
        return value;
    }
    controller_boundary(address); return r_u16(address);
}
static void controller_spu_write(uint32 address, uint32 value)
{
    if (address >= 0x1F801C00u && address < 0x1F801E00u) {
        if (!spu_mmio_write(NULL, address, 2, value, 3)) gt_draft_failure("Native CD SPU halfword write", address);
        return;
    }
    controller_boundary(address); w_u16(address, value);
}
static uint32 controller_missing(uint32 owner, uint32 a0, uint32 a1, uint32 a2, uint32 a3)
{
    /* TODO Bind the original controller helper and its actual result */
    (void)a0; (void)a1; (void)a2; (void)a3;
    gt_draft_failure("Native CD controller helper adapter", owner);
}
static void controller_notify(uint32 token, uint32 status, uint32 guest_response)
{
    /* TODO Bind native callbacks with the actual published guest response buffer */
    (void)status; (void)guest_response;
    gt_draft_failure("Native CD controller callback token", token);
}
uint32 gtmain_cd_controller_volume_init(void)
{
    uint32 registers = r_u32(0x800B2308u);
    const uint8 mixer[4] = {128, 0, 128, 0};
    if (!controller_spu_read(registers + 440u) && !controller_spu_read(registers + 442u)) {
        controller_spu_write(registers + 384u, 0x3FFFu);
        controller_spu_write(registers + 386u, 0x3FFFu);
        registers = r_u32(0x800B2308u);
    }
    controller_spu_write(registers + 432u, 0x3FFFu);
    controller_spu_write(registers + 434u, 0x3FFFu);
    controller_spu_write(registers + 426u, 0xC001u);
    controller_write8(r_u32(0x800B22F4u), 2);
    controller_write8(r_u32(0x800B22FCu), mixer[0]);
    controller_write8(r_u32(0x800B2300u), mixer[1]);
    controller_write8(r_u32(0x800B22F4u), 3);
    controller_write8(r_u32(0x800B22F8u), mixer[2]);
    controller_write8(r_u32(0x800B22FCu), mixer[3]);
    controller_write8(r_u32(0x800B2300u), 32);
    return 0;
}
sint32 gtmain_cd_controller_init(void)
{
    uint32 value, address;
    puts((const char *)psx_addr(0x8008EE54u, 1u));
    printf((const char *)psx_addr(0x8008EE60u, 1u), 0x800B2310u);
    w_u8(0x800B204Du, 0); w_u8(0x800B204Cu, 0);
    w_u32(0x800B2030u, 0); w_u32(0x800B202Cu, 0);
    w_u32(0x800B2040u, 0); w_u32(0x800B203Cu, 0);
    (void)controller_missing(0x8008A248u, 0, 0, 0, 0);
    (void)controller_missing(0x8008A278u, 2, 0x800857C0u, 0, 0);
    controller_write8(r_u32(0x800B22F4u), 1);
    while (controller_read8(r_u32(0x800B2300u)) & 7u) {
        controller_write8(r_u32(0x800B22F4u), 1);
        controller_write8(r_u32(0x800B2300u), 7);
        controller_write8(r_u32(0x800B22FCu), 7);
    }
    w_u8(0x800B230Eu, 0); value = r_u8(0x800B230Eu);
    w_u8(0x800B230Du, (uint8)value);
    address = r_u32(0x800B22F4u); w_u8(0x800B230Cu, 2);
    controller_write8(address, 0); controller_write8(r_u32(0x800B2300u), 0);
    controller_write32(r_u32(0x800B2304u), 4901);
    (void)CD_cw(1, NULL, NULL, 0);
    if (r_u32(0x800B203Cu) & 16u) (void)CD_cw(1, NULL, NULL, 0);
    if (CD_cw(10, NULL, NULL, 0)) return -1;
    if (CD_cw(12, NULL, NULL, 0)) return -1;
    if (CD_sync(0, NULL) != 2u) return -1;
    return 0;
}
uint32 gtmain_cd_controller_interrupt(void)
{
    uint32 saved = controller_read8(r_u32(0x800B22F4u)) & 3u, flags, token, address;
    for (;;) {
        flags = gtmain_cd_controller_service();
        if (!flags) break;
        if (flags & 4u) {
            token = r_u32(0x800B2030u);
            if (token) controller_notify(token, r_u8(0x800B230Du), 0x800B5F54u);
        }
        if (flags & 2u) {
            token = r_u32(0x800B202Cu);
            if (token) controller_notify(token, r_u8(0x800B230Cu), 0x800B5F4Cu);
        }
    }
    address = r_u32(0x800B22F4u); controller_write8(address, saved);
    return address;
}
