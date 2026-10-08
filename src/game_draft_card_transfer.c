#include "game_draft_registry.h"

static void gtmain_card_notify(uint32 selector)
{
    uint32 channel, offset, spec;
    w_u32(0xA001B9D0u + (selector - 0xA7u) * 4u, 1u);
    channel = r_u32(0x7500u + r_u32(0x751Cu) * 4u);
    offset = (uint32)((sint32)channel / 16) * 4u;
    if (!r_u32(0xA0019F20u + offset)) return;
    if (selector == 0xA7u) {
        /* TODO Restore pending filesystem success operations */
        gt_draft_unresolved_call(0xBFC0B648u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    spec = selector == 0xA8u ? 0x8000u : selector == 0xA9u ? 0x100u : 0x2000u;
    w_u32(0xA0019F20u + offset, 0u);
    w_u32(0xA0019F28u + offset, 0u);
    w_u32(0xA0019F30u + offset, 0u);
    DeliverEvent(0xF4000001u, spec);
}

void gtmain_card_vblank(void)
{
    static const uint32 specs[] = {4u, 0x8000u, 0x100u, 0x200u, 0x2000u};
    uint32 index, interrupt, serial, port;
    for (index = 0u; index < 5u; ++index)
        gtmain_bios_clear_event(0xF0000011u, specs[index]);
    if (r_u8(0x755Au)) {
        w_u8(0x755Au, 0u);
        w_u32(0x74A4u, 0u);
        interrupt = r_u32(0x725Cu);
        serial = r_u32(0x7258u);
        w_u32(0x7514u, 0u);
        w_u32(interrupt, 0xFFFFFF7Fu);
        w_u32(interrupt + 4u, r_u32(interrupt + 4u) & 0xFFFFFF7Fu);
        w_u16(serial + 10u, 0u);
        w_u32(0x74C0u, 0u);
        port = r_u8(0x7264u);
        w_u8(0x7568u + port, 0x11u);
        w_u32(0x751Cu, r_u8(0x7264u));
        gtmain_card_notify(0xA9u);
        DeliverEvent(0xF0000011u, 0x100u);
        SysDeqIntRP(1u, 0x7540u);
        serial = r_u32(0x7258u);
        w_u16(serial + 10u, 0x40u);
        w_u16(serial + 14u, 0x88u);
        w_u16(serial + 8u, 13u);
        w_u16(serial + 10u, 0u);
        return;
    }
    w_u8(0x7264u, 1u - r_u8(0x7264u));
    port = r_u8(0x7264u);
    if (r_u8(0x7568u + port) & 1u) return;
    gtmain_card_sector_configure(r_u32(0x7550u + port * 4u),
        0x7568u + port, 0x7560u + port * 4u);
    SysDeqIntRP(1u, 0x7540u);
    SysEnqIntRP(1u, 0x7540u);
    w_u32(0x7514u, 0u);
    w_u32(0x7520u, 0u);
    gtmain_card_serial_irq();
}

void gtmain_card_serial_irq(void)
{
    uint32 port = r_u8(0x7264u);
    uint32 selected = port ? 0x2000u : 0u;
    uint32 serial = r_u32(0x7258u);
    uint32 interrupt, handler;
    sint32 result;
    w_u16(serial + 10u, r_u16(serial + 10u) | selected | 0x12u);
    port = r_u8(0x7264u);
    handler = r_u32(0x7528u + port * 4u);
    w_u32(0x7514u, r_u32(0x7514u) + 1u);
    w_u32(0x74A0u, selected);
    if (handler == 0x5688u) result = gtmain_card_read_step();
    else {
        /* TODO Dispatch the other BIOS card protocol handlers */
        result = (sint32)gt_draft_unresolved_call(handler, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    if (!result) {
        interrupt = r_u32(0x725Cu);
        w_u32(interrupt, 0xFFFFFF7Fu);
        w_u32(interrupt + 4u, r_u32(interrupt + 4u) | 0x80u);
        return;
    }
    if (result == 1) {
        w_u32(0x74A4u, 0u);
        serial = r_u32(0x7258u);
        w_u8(0x755Au, 0u);
        w_u16(serial + 10u, 0u);
        w_u32(0x7514u, 0u);
        SysDeqIntRP(1u, 0x7540u);
        interrupt = r_u32(0x725Cu);
        w_u32(interrupt, 0xFFFFFF7Fu);
        w_u32(interrupt + 4u, r_u32(interrupt + 4u) & 0xFFFFFF7Fu);
        if (!r_u32(0x7520u)) {
            w_u32(0x74C0u, 0u);
            w_u8(0x7568u + r_u8(0x7264u), 1u);
            w_u32(0x751Cu, r_u8(0x7264u));
            gtmain_card_notify(0xA7u);
            DeliverEvent(0xF0000011u, 4u);
        }
        return;
    }
    serial = r_u32(0x7258u);
    w_u8(0x755Au, 0u);
    w_u16(serial + 10u, 0u);
    if (!r_u32(0x7520u)) {
        w_u32(0x74C0u, 0u);
        w_u8(0x7568u + r_u8(0x7264u), 0x21u);
        w_u32(0x751Cu, r_u8(0x7264u));
        gtmain_card_notify(0xA8u);
        DeliverEvent(0xF0000011u, 0x8000u);
    }
    w_u32(0x7514u, 0u);
    w_u32(0x7520u, 0u);
    SysDeqIntRP(1u, 0x7540u);
    interrupt = r_u32(0x725Cu);
    w_u32(interrupt, 0xFFFFFF7Fu);
    w_u32(interrupt + 4u, r_u32(interrupt + 4u) & 0xFFFFFF7Fu);
}

static void gtmain_card_acknowledge(uint32 serial)
{
    w_u16(serial + 10u, r_u16(serial + 10u) | 0x10u);
    w_u32(r_u32(0x725Cu), 0xFFFFFF7Fu);
}

sint32 gtmain_card_sector_irq(void)
{
    uint32 mode, buffer, checksum_address, value, count;
    if (!r_u32(0x75C0u) || !(r_u32(0x1F801070u) & 0x80u) ||
        !(r_u32(0x1F801074u) & 0x80u)) return 0;
    mode = r_u8(r_u32(0x75C8u));
    if (mode != 4u && mode != 2u) return 0;
    buffer = r_u32(0x75C4u);
    if (mode == 4u) {
        (void)r_u8(0x1F801040u);
        value = r_u8(buffer);
        buffer += 1u;
        w_u8(0x1F801040u, value);
        w_u32(0x75C4u, buffer);
    } else {
        value = r_u8(0x1F801040u);
        w_u8(buffer, value);
        w_u8(0x1F801040u, 0u);
        w_u32(0x75C4u, buffer + 1u);
    }
    w_u16(0x1F80104Au, r_u16(0x1F80104Au) | 0x10u);
    w_u32(0x1F801070u, 0xFFFFFF7Fu);
    checksum_address = r_u32(0x75CCu);
    w_u32(checksum_address, r_u32(checksum_address) ^ value);
    count = (uint32)r_u16(0x72F0u) + 1u;
    w_u16(0x72F0u, count);
    if (count >= (mode == 4u ? 128u : 127u)) w_u16(0x75C0u, 0u);
    /* Native IRQ dispatch returns to the host instead of restoring guest registers */
    return 1;
}

void gtmain_card_sector_configure(uint32 buffer, uint32 mode_address, uint32 checksum_address)
{
    w_u32(0x72F0u, 0u);
    w_u32(0x75C0u, 0u);
    w_u32(0x75C4u, buffer);
    w_u32(0x75C8u, mode_address);
    w_u32(0x75CCu, checksum_address);
}

sint32 gtmain_card_read_step(void)
{
    uint32 port = r_u8(0x7264u);
    uint32 step = r_u32(0x7514u);
    uint32 sector = r_u16(0x7508u + port * 4u);
    uint32 buffer = r_u32(0x7550u + port * 4u);
    uint32 serial, incoming, selected;
    if (step < 1u || step > 13u) return -1;
    serial = r_u32(0x7258u);
    switch (step) {
    case 1u:
        selected = port ? 0x2000u : 0u;
        w_u16(serial + 10u, selected | 0x1003u);
        (void)r_u8(serial);
        w_u8(serial, (uint32)(sint32)(sint8)(r_u32(0x7500u + port * 4u) & 15u) + 0x81u);
        gtmain_card_acknowledge(serial);
        w_u8(0x755Au, 1u);
        w_u32(0x74A0u, selected);
        return 0;
    case 2u:
        (void)r_u8(serial);
        w_u8(serial, 0x52u);
        gtmain_card_acknowledge(serial);
        return 0;
    case 3u:
        incoming = r_u8(serial);
        w_u8(serial, 0u);
        gtmain_card_acknowledge(serial);
        if (r_u32(0x74C0u)) return 0;
        if (!(incoming & 8u)) return 0;
        w_u32(0x74C0u, 0u);
        w_u8(0x7568u + port, 1u);
        w_u32(0x751Cu, r_u8(0x7264u));
        gtmain_card_notify(0xAAu);
        DeliverEvent(0xF0000011u, 0x2000u);
        w_u32(0x7520u, 1u);
        return -1;
    case 4u:
    case 8u:
    case 9u:
        incoming = r_u8(serial);
        w_u8(serial, 0u);
        gtmain_card_acknowledge(serial);
        return incoming == (step == 4u ? 0x5Au : step == 8u ? 0x5Cu : 0x5Du) ? 0 : -1;
    case 5u:
        incoming = r_u8(serial);
        w_u8(serial, (sector >> 8) & 0xFFu);
        gtmain_card_acknowledge(serial);
        return incoming == 0x5Du ? 0 : -1;
    case 6u:
        (void)r_u8(serial);
        w_u8(serial, sector & 0xFFu);
        gtmain_card_acknowledge(serial);
        return 0;
    case 7u:
        (void)r_u8(serial);
        w_u8(serial, 0u);
        gtmain_card_acknowledge(serial);
        return 0;
    case 10u:
        incoming = r_u8(serial);
        w_u8(serial, 0u);
        gtmain_card_acknowledge(serial);
        if (incoming != ((sector >> 8) & 0xFFu)) return -1;
        w_u32(0x7560u + port * 4u, (sector & 0xFFu) ^ ((sector >> 8) & 0xFFu));
        w_u8(0x7558u + port, 0u);
        return 0;
    case 11u:
        incoming = r_u8(serial);
        w_u8(serial, r_u8(0x7558u + port));
        gtmain_card_acknowledge(serial);
        if (incoming != (sector & 0xFFu)) return -1;
        w_u32(0x75C0u, 1u);
        return 0;
    case 12u:
        incoming = r_u8(serial);
        w_u8(serial, 0u);
        gtmain_card_acknowledge(serial);
        w_u8(buffer + 127u, incoming);
        w_u32(0x7560u + port * 4u, r_u32(0x7560u + port * 4u) ^ r_u8(buffer + 127u));
        return 0;
    case 13u:
        incoming = r_u8(serial);
        w_u8(serial, 0u);
        gtmain_card_acknowledge(serial);
        if (incoming != r_u32(0x7560u + port * 4u)) return -1;
        while (!(r_u16(serial + 4u) & 2u)) {}
        return r_u8(serial) == 0x47u ? 1 : -1;
    }
    return -1;
}
