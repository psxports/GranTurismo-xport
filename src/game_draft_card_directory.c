#include "game_draft_registry.h"
#include <string.h>

static uint32 gtmain_card_read(uint32 channel, uint32 sector, uint32 buffer)
{
    sint32 port = (sint32)channel / 16;
    uint32 offset = (uint32)port * 4u;
    if (!(r_u8(0x7568u + (uint32)port) & 1u) || (sint32)sector < 0 || sector >= 0x401u)
        return 0u;
    w_u32(0x7514u, 0u);
    w_u32(0x74A4u, 0u);
    w_u32(0x7500u + offset, channel);
    w_u32(0x7550u + offset, buffer);
    w_u32(0x7528u + offset, 0x5688u);
    w_u32(0x7508u + offset, sector);
    w_u8(0x7568u + (uint32)port, 2u);
    return 1u;
}

static uint32 gtmain_card_wait(void)
{
    uint32 cursor, result;
    for (;;) {
        result = 1u;
        if (r_u32(0xA001B9D0u) != 1u) {
            result = 0u;
            for (cursor = 0xA001B9D4u; cursor != 0xA001B9E4u; cursor += 4u)
                if (r_u32(cursor) == 1u) break;
            if (cursor == 0xA001B9E4u) {
                if (gtmain_card_sector_irq()) continue;
                /* Advance the native field source while BIOS waits for card I/O */
                if (VSync(0) < 0)
                    gt_draft_unresolved_call(0xBFC09144u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
                if (r_u32(0x74BCu)) gtmain_card_vblank();
                continue;
            }
        }
        for (cursor = 0xA001B9D0u; cursor != 0xA001B9E4u; cursor += 4u)
            w_u32(cursor, 0u);
        gtmain_card_clear_events();
        return result;
    }
}

static uint32 gtmain_card_sector_valid(uint32 buffer)
{
    uint32 index, checksum = 0u;
    for (index = 0u; index < 127u; ++index) checksum ^= r_u8(buffer + index);
    return checksum == r_u8(buffer + 127u);
}

static uint32 gtmain_card_chain_cleanup(uint32 port, uint32 index)
{
    uint32 base = 0xA001BA88u + port * 480u;
    uint32 entry = base + index * 32u;
    uint32 kind = r_u8(entry), expected, next, original_next, linked;
    sint32 blocks, remaining;
    uint32 invalid = 0u;
    if (kind == 0x51u) expected = 0xA0u;
    else if (kind == 0xA1u) expected = 0x50u;
    else return 0u;
    blocks = (sint32)r_u32(entry + 4u) / 8192 - 1;
    original_next = r_u16(entry + 8u);
    next = original_next;
    remaining = blocks;
    while (remaining > 0 && next != 0xFFFFu) {
        linked = base + next * 32u;
        if ((r_u32(linked) & 0xF0u) == expected) {
            invalid = 1u;
            break;
        }
        next = r_u16(linked + 8u);
        --remaining;
    }
    if (!invalid) return 0u;
    w_u8(entry + 10u, 0u);
    w_u32(entry, 0xA0u);
    w_u32(entry + 4u, 0u);
    w_u16(entry + 8u, 0xFFFFu);
    next = original_next;
    remaining = blocks;
    while (remaining > 0 && next != 0xFFFFu) {
        linked = base + next * 32u;
        next = r_u16(linked + 8u);
        w_u32(linked, 0xA0u);
        w_u32(linked + 4u, 0u);
        w_u16(linked + 8u, 0xFFFFu);
        --remaining;
    }
    return 1u;
}

uint32 gtmain_card_directory_load(uint32 channel)
{
    sint32 port = (sint32)channel / 16;
    uint32 buffer = 0xA001BE48u + (uint32)port * 128u;
    uint32 directory = 0xA001BA88u + (uint32)port * 480u;
    uint32 sectors = 0xA001B9E8u + (uint32)port * 80u;
    uint32 index, entry, next, result;
    uint32 references[15];
    sint32 blocks;
    _new_card();
    if (gtmain_card_read(channel, 0u, buffer) != 1u || !gtmain_card_wait())
        goto read_failure;
    if (r_u8(buffer) != 0x4Du || r_u8(buffer + 1u) != 0x43u) {
        if (r_u32(0xA0019F90u) == 1u) {
            /* TODO Restore BIOS formatting for an invalid card header */
            return gt_draft_unresolved_call(0xBFC0B170u, channel, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
        goto read_failure;
    }
    _new_card();
    /* The original writes sector 63 and ignores both results */
    gt_draft_unresolved_call(0xBFC0DA20u, channel, 63u, buffer, 0u, 0u, 0u, 0u, 0u);
    gtmain_card_wait();
    for (index = 0u; index < 15u; ++index) {
        entry = directory + index * 32u;
        memset(psx_addr(entry, 32u), 0, 32u);
        w_u32(entry, 0xA0u);
        w_u32(entry + 4u, 0u);
    }
    for (index = 0u; index < 15u; ++index) {
        if (gtmain_card_read(channel, index + 1u, buffer) != 1u || !gtmain_card_wait())
            goto read_failure;
        result = gtmain_card_sector_valid(buffer);
        if (!result) goto read_failure;
        memcpy(psx_addr(directory + index * 32u, 32u), psx_addr(buffer, 32u), 32u);
    }
    for (index = 0u; index < 15u; ++index) {
        result = gtmain_card_chain_cleanup((uint32)port, index);
        references[index] = result == 1u ? 0x52u : 0u;
    }
    /* The original clears the temporary ownership array before counting links */
    memset(references, 0, sizeof(references));
    for (index = 0u; index < 15u; ++index) {
        entry = directory + index * 32u;
        if (r_u32(entry) != 0x51u && r_u32(entry) != 0xA1u) continue;
        references[index] = 1u;
        blocks = (sint32)r_u32(entry + 4u) / 8192 - 1;
        next = r_u16(entry + 8u);
        while (blocks > 0 && next != 0xFFFFu) {
            if (next >= 15u) {
                /* TODO Model malformed chains that access beyond the temporary array */
                gt_draft_unresolved_call(0xBFC08E74u, next, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
            }
            ++references[next];
            next = r_u16(directory + next * 32u + 8u);
            --blocks;
        }
    }
    for (index = 0u; index < 15u; ++index) {
        if (references[index]) continue;
        entry = directory + index * 32u;
        w_u32(entry + 4u, 0u);
        w_u32(entry, 0xA0u);
        w_u16(entry + 8u, 0xFFFFu);
    }
    for (index = 0u; index < 20u; ++index) {
        if (gtmain_card_read(channel, index + 16u, buffer) != 1u || !gtmain_card_wait())
            goto read_failure;
        result = gtmain_card_sector_valid(buffer);
        if (!result) goto read_failure;
        w_u32(sectors + index * 4u, r_u32(buffer));
    }
    return 1u;

read_failure:
    memset(psx_addr(directory, 480u), 0, 480u);
    for (index = 0u; index < 20u; ++index) w_u32(sectors + index * 4u, 0xFFFFFFFFu);
    return 0u;
}
