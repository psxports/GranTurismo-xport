#include "game_draft_registry.h"

void gtmain_spu_delay(void)
{
    volatile uint32 value = 13u;
    uint32 iteration;
    for (iteration = 0u; iteration < 60u; ++iteration) value *= 13u;
}

void gtmain_spu_dma_complete(void)
{
    uint32 registers;
    uint32 callback;
    uint32 polls;
    if (!r_u32(0x800B2394u)) gtmain_spu_delay();
    registers = r_u32(0x800B2344u);
    w_u16(registers + 426u, r_u16(registers + 426u) & 0xFFCFu);
    if (r_u16(registers + 426u) & 0x30u) {
        for (polls = 1u; polls < 0xF01u; ++polls)
            if (!(r_u16(registers + 426u) & 0x30u)) break;
    }
    callback = r_u32(0x800B237Cu);
    if (callback) {
        /* TODO Dispatch a configured guest transfer callback */
        gt_draft_unresolved_call(callback, 0xF0000000u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    } else {
        w_u32(0x800B2378u, 1u);
    }
}

uint32 gtmain_spu_transfer(uint32 source, uint32 size)
{
    uint32 registers = r_u32(0x800B2344u);
    uint32 initial_status = r_u16(registers + 430u) & 0x7FFu;
    uint32 result;
    uint32 polls;
    w_u16(registers + 422u, r_u16(0x800B235Cu));
    gtmain_spu_delay();
    while (size) {
        uint32 chunk = size < 65u ? size : 64u;
        uint32 offset;
        registers = r_u32(0x800B2344u);
        for (offset = 0u; offset < chunk; offset += 2u) {
            uint32 value = r_u16(source);
            source += 2u;
            w_u16(registers + 424u, value);
        }
        registers = r_u32(0x800B2344u);
        w_u16(registers + 426u, (r_u16(registers + 426u) & 0xFFCFu) | 0x10u);
        gtmain_spu_delay();
        if (r_u16(r_u32(0x800B2344u) + 430u) & 0x400u) {
            for (polls = 1u; polls < 0xF01u; ++polls)
                if (!(r_u16(r_u32(0x800B2344u) + 430u) & 0x400u)) break;
            if (polls == 0xF01u)
                gt_draft_unresolved_call(0x8008B7D8u, 0x8008EE78u, 0x8008EE98u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
        size -= chunk;
        gtmain_spu_delay();
        gtmain_spu_delay();
    }
    registers = r_u32(0x800B2344u);
    w_u16(registers + 426u, r_u16(registers + 426u) & 0xFFCFu);
    result = r_u16(registers + 430u) & 0x7FFu;
    if (result != initial_status) {
        for (polls = 1u; polls < 0xF01u; ++polls) {
            result = r_u16(r_u32(0x800B2344u) + 430u) & 0x7FFu;
            if (result == initial_status) return result;
        }
        return gt_draft_unresolved_call(0x8008B7D8u, 0x8008EE78u, 0x8008EEACu, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    return result;
}

uint32 gtmain_spu_chip_init(uint32 mode)
{
    uint32 address = r_u32(0x800B2354u);
    uint32 registers;
    uint32 index;
    uint32 polls;
    w_u32(address, r_u32(address) | 0xB0000u);
    registers = r_u32(0x800B2344u);
    w_u32(0x800B2360u, 0u);
    w_u32(0x800B2364u, 0u);
    w_u16(0x800B235Cu, 0u);
    w_u16(registers + 384u, 0u);
    w_u16(registers + 386u, 0u);
    w_u16(registers + 426u, 0u);
    gtmain_spu_delay();
    registers = r_u32(0x800B2344u);
    w_u16(registers + 384u, 0u);
    w_u16(registers + 386u, 0u);
    if (r_u16(registers + 430u) & 0x7FFu) {
        for (polls = 1u; polls < 0xF01u; ++polls)
            if (!(r_u16(r_u32(0x800B2344u) + 430u) & 0x7FFu)) break;
        if (polls == 0xF01u)
            gt_draft_unresolved_call(0x8008B7D8u, 0x8008EE78u, 0x8008EE88u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    w_u32(0x800B2368u, 2u);
    w_u32(0x800B236Cu, 3u);
    w_u32(0x800B2370u, 8u);
    w_u32(0x800B2374u, 7u);
    registers = r_u32(0x800B2344u);
    w_u16(registers + 428u, 4u);
    w_u16(registers + 388u, 0u);
    w_u16(registers + 390u, 0u);
    w_u16(registers + 396u, 0xFFFFu);
    w_u16(registers + 398u, 0xFFFFu);
    w_u16(registers + 408u, 0u);
    w_u16(registers + 410u, 0u);
    for (index = 0u; index < 10u; ++index) w_u16(0x800B62C0u + 2u * index, 0u);
    if (!mode) {
        registers = r_u32(0x800B2344u);
        w_u16(0x800B235Cu, 512u);
        for (index = 400u; index <= 406u; index += 2u) w_u16(registers + index, 0u);
        for (index = 432u; index <= 438u; index += 2u) w_u16(registers + index, 0u);
        /* The MIPS passes the address of the seed data, not its first word */
        gtmain_spu_transfer(0x800B2384u, 16u);
        registers = r_u32(0x800B2344u);
        for (index = 0u; index < 24u; ++index) {
            w_u16(registers, 0u);
            w_u16(registers + 2u, 0u);
            w_u16(registers + 4u, 0x3FFFu);
            w_u16(registers + 6u, 512u);
            w_u16(registers + 8u, 0u);
            w_u16(registers + 10u, 0u);
            registers += 16u;
        }
        registers = r_u32(0x800B2344u);
        w_u16(registers + 392u, 0xFFFFu);
        w_u16(registers + 394u, 255u);
        for (index = 0u; index < 4u; ++index) gtmain_spu_delay();
        registers = r_u32(0x800B2344u);
        w_u16(registers + 396u, 0xFFFFu);
        w_u16(registers + 398u, 255u);
        for (index = 0u; index < 4u; ++index) gtmain_spu_delay();
    }
    w_u32(0x800B2378u, 1u);
    w_u16(r_u32(0x800B2344u) + 426u, 0xC000u);
    w_u32(0x800B237Cu, 0u);
    w_u32(0x800B2380u, 0u);
    return 0u;
}
