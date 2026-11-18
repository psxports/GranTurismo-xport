#include "game_first2_registry.h"
#include <stdio.h>

static uint32 service_read8(uint32 address)
{
    /* TODO Bind actual indexed CD controller reads */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native indexed CD controller adapter", address);
    return r_u8(address);
}
static void service_write8(uint32 address, uint32 value)
{
    /* TODO Bind actual indexed CD controller writes */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native indexed CD controller adapter", address);
    w_u8(address, value);
}
static void service_publish(uint32 destination, const uint8 response[8])
{
    uint32 i;
    for (i = 0; i < 8; ++i) w_u8(destination + i, response[i]);
}
/* Original: GTMAIN.EXE:sub_80084138. */ uint32 gtmain_cd_controller_service(void)
{
    uint8 response[8];
    uint32 flags, status_address, count = 0, i, errors = 0, value;
    service_write8(r_u32(0x800B22F4u), 1);
    status_address = r_u32(0x800B2300u);
    flags = service_read8(status_address) & 7u;
    if (!flags) return 0;
    while (flags != (service_read8(status_address) & 7u))
        flags = service_read8(status_address) & 7u;
    do {
        if (!(service_read8(r_u32(0x800B22F4u)) & 0x20u)) break;
        response[count++] = (uint8)service_read8(r_u32(0x800B22F8u));
    } while (count < 8);
    /* The original explicitly clears the unread response tail */
    for (i = count; i < 8; ++i) response[i] = 0;
    service_write8(r_u32(0x800B22F4u), 1);
    service_write8(r_u32(0x800B2300u), 7);
    service_write8(r_u32(0x800B22FCu), 7);
    if (flags != 3 || r_u32(0x800B21F4u + 4u * r_u8(0x800B204Du))) {
        if (!(r_u32(0x800B203Cu) & 16u) && (response[0] & 16u))
            w_u32(0x800B2044u, r_u32(0x800B2044u) + 1u);
        errors = response[0] & 0x1Du;
        w_u32(0x800B203Cu, response[0]);
        w_u32(0x800B2040u, response[1]);
    }
    if (flags == 5 && (sint32)r_u32(0x800B2038u) > 0) {
        printf((const char *)psx_addr(0x8008ED90u, 1u));
        if ((sint32)r_u32(0x800B2038u) > 0) {
            uint32 command = r_u8(0x800B204Du);
            uint32 first = r_u32(0x800B203Cu), second = r_u32(0x800B2040u);
            uint32 name = r_u32(0x800B2054u + 4u * command);
            printf((const char *)psx_addr(0x8008ED9Cu, 1u), (const char *)psx_addr(name, 1u), first, second);
        }
    }
    switch (flags) {
    case 1:
        if (errors && count == 1) errors = 0;
        w_u8(0x800B230Du, errors ? 5 : 1);
        service_publish(0x800B5F54u, response);
        service_write8(r_u32(0x800B22F4u), 0);
        service_write8(r_u32(0x800B2300u), 0);
        return 4;
    case 2:
        w_u8(0x800B230Cu, errors ? 5 : 2);
        service_publish(0x800B5F4Cu, response);
        return 2;
    case 3:
        if (errors) {
            w_u8(0x800B230Cu, 5);
            service_publish(0x800B5F4Cu, response);
            return 2;
        }
        if (r_u32(0x800B20F4u + 4u * r_u8(0x800B204Du))) {
            w_u8(0x800B230Cu, 3);
            service_publish(0x800B5F4Cu, response);
            return 1;
        }
        w_u8(0x800B230Cu, 2);
        service_publish(0x800B5F4Cu, response);
        return 2;
    case 4:
        w_u8(0x800B230Eu, 4); value = r_u8(0x800B230Eu);
        w_u8(0x800B230Du, value);
        service_publish(0x800B5F5Cu, response);
        service_publish(0x800B5F54u, response);
        return 4;
    case 5:
        w_u8(0x800B230Du, 5); value = r_u8(0x800B230Du);
        w_u8(0x800B230Cu, value);
        service_publish(0x800B5F4Cu, response);
        service_publish(0x800B5F54u, response);
        return 6;
    default:
        puts((const char *)psx_addr(0x8008EDB8u, 1u));
        printf((const char *)psx_addr(0x8008EDCCu, 1u), flags);
        return 0;
    }
}
