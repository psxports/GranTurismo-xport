#include "game_first2_registry.h"

static void pad_mmio_boundary(uint32 address)
{
    /* TODO Bind actual native SIO IRQ and timer services */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native pad SIO IRQ timer adapter", address);
}
static uint32 pad_read8(uint32 address) { pad_mmio_boundary(address); return r_u8(address); }
static uint32 pad_read16(uint32 address) { pad_mmio_boundary(address); return r_u16(address); }
static uint32 pad_read32(uint32 address) { pad_mmio_boundary(address); return r_u32(address); }
static void pad_write8(uint32 address, uint32 value) { pad_mmio_boundary(address); w_u8(address, value); }
static void pad_write16(uint32 address, uint32 value) { pad_mmio_boundary(address); w_u16(address, value); }
static void pad_write32(uint32 address, uint32 value) { pad_mmio_boundary(address); w_u32(address, value); }
static void pad_timer(uint32 delay)
{
    /* TODO Bind unselected actual timer snapshot service */
    gt_draft_failure("Native pad timer snapshot", 0x80081E98u);
}
static uint32 pad_expired(void)
{
    /* TODO Bind unselected actual timer expiry service */
    gt_draft_failure("Native pad timer expiry", 0x80081EB8u);
}
static uint32 pad_port_callback(uint32 token, uint32 port)
{
    if (token == 0x8007FB28u)
        gt_draft_failure("Native pad callback meaningful return", token);
    if (token == 0x8007FC84u) return gtmain_pad_command_clear(port);
    if (token == 0x800811C0u) return gtmain_pad_response_apply(port);
    if (token == 0x800810ECu) return gtmain_pad_command_prepare(port);
    if (token == 0x80081500u) return gtmain_pad_response_pending(port);
    if (token == 0x8007FB90u) return (uint32)gtmain_pad_next_port((sint32)port);
    /* TODO Bind actual native callback result for the remaining token */
    gt_draft_failure("Native pad protocol callback", token);
}
static void pad_reset_callback(uint32 port)
{
    uint32 token = r_u32(0x800B1EC0u);
    if (token == 0x8007FB28u) gtmain_pad_reset_port(port);
    else (void)pad_port_callback(token, port);
}
static uint32 pad_noarg_callback(uint32 token)
{
    if (token == 0x80080B28u) return (uint32)gtmain_pad_protocol_step();
    if (token == 0x80080FFCu) return gtmain_pad_serial_ack();
    if (token == 0x8008108Cu) return gtmain_pad_serial_wait();
    /* TODO Bind actual native zero-argument callback */
    gt_draft_failure("Native pad protocol stage callback", token);
}

/* GTMAIN.EXE 80081500 */
uint32 gtmain_pad_response_pending(uint32 port)
{
    if (!r_u16(port + 230u)) return 1;
    return r_u8(port + 70u) != 255u;
}

/* GTMAIN.EXE 80081DE4 */
uint32 gtmain_pad_config_command(uint32 port, uint8 enabled)
{
    w_u8(port + 54u, 67);
    w_u32(port + 44u, port + 36u);
    w_u8(port + 36u, enabled);
    w_u8(port + 53u, 1);
    return 1;
}

/* GTMAIN.EXE 800810B8 */
uint32 gtmain_pad_callback_bind(void)
{
    w_u32(0x800B1ED4u, 0x800810ECu);
    w_u32(0x800B1ED8u, 0x80081500u);
    w_u32(0x800B1EDCu, 0x800811C0u);
    return 0x800811C0u;
}

/* GTMAIN.EXE 8008108C */
uint32 gtmain_pad_serial_wait(void)
{
    uint32 control = r_u32(0x800B1F20u), value;
    do value = pad_read16(control + 4u) & 2u; while (!value);
    return value;
}

/* GTMAIN.EXE 800810EC */
uint32 gtmain_pad_command_prepare(uint32 port)
{
    uint32 state;
    if (r_u8(r_u32(port + 60u)) == 243u && !r_u8(port + 232u)) {
        gtmain_pad_config_command(port, 0);
        return 0;
    }
    state = r_u8(port + 70u);
    if (state == 1u) gtmain_pad_config_command(port, 1);
    else if (state == 254u) gtmain_pad_config_command(port, 0);
    else if (state && state != 255u) {
        uint32 token = r_u32(port + 20u);
        if (token) (void)pad_noarg_callback(token);
        else {
            /* TODO Bind unselected actual default prepare state service */
            gt_draft_failure("Native pad default prepare state", 0x80081554u);
        }
    }
    return 0;
}

/* GTMAIN.EXE 800811C0 */
uint32 gtmain_pad_response_apply(uint32 port)
{
    uint32 input = r_u32(port + 60u);
    uint32 kind = r_u8(input) >> 4;
    uint32 old_kind = r_u8(port + 232u), state, result;
    w_u8(port + 232u, (uint8)kind);
    if (kind == 15u) w_u8(port + 232u, (uint8)old_kind);
    else {
        uint32 index;
        w_u8(r_u32(port + 48u), 0);
        input = r_u32(port + 60u);
        {
            uint32 output = r_u32(port + 48u);
            w_u8(output + 1u, r_u8(input));
        }
        for (index = 2; index < r_u8(port + 68u); ++index) {
            uint32 source = r_u32(port + 60u);
            uint32 destination = r_u32(port + 48u);
            w_u8(destination + index, r_u8(source + index));
        }
    }
    if ((!r_u8(r_u32(port + 60u) + 1u) &&
        (r_u8(port + 70u) != 1u || r_u32(port + 20u)) && !r_u8(port + 80u)) ||
        (!gtmain_pad_response_pending(port) && !r_u8(port + 55u) && !r_u8(port + 74u) &&
        r_u8(port + 232u) != old_kind)) pad_reset_callback(port);
    state = (r_u8(port + 70u) - 2u) & 255u;
    w_u8(port + 74u, 0);
    if (state < 252u && r_u8(r_u32(port + 60u)) != 243u) pad_reset_callback(port);
    state = r_u8(port + 70u);
    if (state && state != 255u) {
        result = r_u8(port + 54u);
        if (!result) return result;
    }
    if (state == 1u) {
        result = r_u8(port + 70u);
        w_u8(port + 71u, 0);
    } else if (state >= 2u) {
        if (state == 254u) { w_u8(port + 70u, 255); return 255; }
        if (state == 255u) return 255;
        {
            uint32 token = r_u32(port + 24u);
            if (token) {
                if (token == 0x8007FB28u)
                    gt_draft_failure("Native pad callback meaningful return", token);
                result = pad_port_callback(token, port);
            } else {
                /* TODO Bind unselected actual default response state service */
                gt_draft_failure("Native pad default response state", 0x800815D8u);
            }
            w_u8(port + 70u, (uint8)(r_u8(port + 70u) + result));
            return result;
        }
    } else {
        result = r_u8(port + 232u);
        if (!result) return result;
        result = r_u8(port + 70u);
        w_u8(port + 73u, 1);
    }
    ++result;
    w_u8(port + 70u, (uint8)result);
    return result;
}

/* GTMAIN.EXE 80080FFC */
uint32 gtmain_pad_serial_ack(void)
{
    uint32 irq = r_u32(0x800B1F1Cu), control = r_u32(0x800B1F20u);
    pad_write32(irq, 0xFFFFFF7Fu);
    if (pad_read16(control + 4u) & 0x80u) {
        do {
            if (pad_expired()) return 0;
            control = r_u32(0x800B1F20u);
        } while (pad_read16(control + 4u) & 0x80u);
    }
    control = r_u32(0x800B1F20u);
    pad_write16(control + 10u, pad_read16(control + 10u) | 0x10u);
    return 1;
}

/* GTMAIN.EXE 80080B28 */
sint32 gtmain_pad_protocol_step(void)
{
    uint32 index = r_u32(0x800B1F00u);
    uint32 token = r_u32(0x800B1F40u + 4u * index);
    sint32 result;
    w_u32(0x800B1F00u, index + 1u);
    {
        uint32 port = r_u32(0x800B1EF0u) + 240u * r_u32(0x800B1EFCu);
        if (token == 0x80081F58u) result = gtmain_pad_packet_start(port);
        else if (token == 0x80081F98u) result = gtmain_pad_packet_command(port);
        else if (token == 0x80081FA8u) result = gtmain_pad_packet_command_tail(port, r_u32(0x800B1EFCu) == r_u32(0x800B1F0Cu));
        else if (token == 0x80082070u) result = gtmain_pad_packet_length(port);
        else if (token == 0x80082078u) result = gtmain_pad_packet_length_tail(port, r_u32(0x800B1F3Cu));
        else if (token == 0x80082130u) result = gtmain_pad_packet_ack(port);
        else if (token == 0x800821BCu) result = gtmain_pad_packet_read(port);
        else result = (sint32)pad_noarg_callback(token);
    }
    if (result < 0) return (sint32)pad_port_callback(r_u32(0x800B1EBCu), (uint32)result);
    if (r_u32(0x800B1F00u)) {
        pad_timer(60);
        if (!gtmain_pad_serial_ack()) (void)pad_port_callback(r_u32(0x800B1EBCu), (uint32)-3);
    }
    index = r_u32(0x800B1F00u);
    result = (sint32)(index - 1u);
    if ((sint32)index >= 5) w_u32(0x800B1F00u, (uint32)result);
    return result;
}

/* GTMAIN.EXE 80080BF0 */
sint32 gtmain_pad_exchange(uint32 port, sint32 outgoing)
{
    uint32 control, value, result;
    if (outgoing < 0) {
        uint32 output;
        control = r_u32(0x800B1F20u);
        output = r_u32(port + 64u);
        result = pad_read8(control);
        w_u8(port + 68u, 255); w_u8(port + 69u, 1);
        w_u8(output, (uint8)~outgoing);
        control = r_u32(0x800B1F20u);
        while (!(pad_read16(control + 4u) & 1u)) {}
        while (!pad_expired()) {}
        pad_write8(r_u32(0x800B1F20u), (uint8)~outgoing);
        return (sint32)result;
    }
    value = 136;
    if ((r_u8(r_u32(port + 60u)) >> 4) == 8u && r_u8(port + 68u) >= 9u) value = 34;
    {
        uint32 counter = pad_read16(0x1F801120u);
        uint32 status;
        control = r_u32(0x800B1F20u);
        status = pad_read16(control + 4u);
        w_u32(0x800C4BF4u, 430); w_u32(0x800C4BF0u, counter);
        while (!(status & 2u)) status = pad_read16(control + 4u);
    }
    control = r_u32(0x800B1F20u);
    {
        uint32 irq = r_u32(0x800B1F1Cu);
        result = pad_read8(control);
        pad_write16(control + 14u, value);
        if (!(pad_read32(irq) & 0x80u)) {
            do {
                if (pad_expired()) return -20;
                irq = r_u32(0x800B1F1Cu);
            } while (!(pad_read32(irq) & 0x80u));
        }
    }
    pad_write8(r_u32(0x800B1F20u), (uint8)outgoing);
    {
        uint32 position = r_u8(port + 69u), length = r_u8(port + 68u);
        uint32 input = r_u32(port + 60u);
        w_u8(port + 69u, (uint8)(position + 1u));
        w_u8(input + length, (uint8)result);
        w_u8(port + 68u, (uint8)(r_u8(port + 68u) + 1u));
    }
    return (sint32)result;
}

/* GTMAIN.EXE 80080DC8 */
sint32 gtmain_pad_exchange_timed(uint32 port, uint8 outgoing)
{
    uint32 baud = 136, control, received, length, irq;
    if ((r_u8(r_u32(port + 60u)) >> 4) == 8u && r_u8(port + 68u) >= 9u) baud = 34;
    control = r_u32(0x800B1F20u);
    while (!(pad_read16(control + 4u) & 2u)) {}
    pad_timer(400);
    control = r_u32(0x800B1F20u);
    received = pad_read8(control); length = r_u8(port + 68u);
    pad_write16(control + 14u, !length && (received >> 4) == 8u ? 34u : baud);
    irq = r_u32(0x800B1F1Cu);
    if (!(pad_read32(irq) & 0x80u)) {
        uint32 start = r_u32(0x800C4BF0u), delay = r_u32(0x800C4BF4u);
        do {
            uint32 counter = pad_read16(0x1F801120u), elapsed;
            if (counter < start) {
                if (pad_read16(0x1F801128u)) counter += pad_read16(0x1F801128u);
                else counter += 0x10000u;
            }
            elapsed = counter - start;
            if ((pad_read16(0x1F801124u) & 0x200u) && elapsed >= delay) return -2;
            if ((elapsed >> 3) >= delay) return -2;
        } while (!(pad_read32(irq) & 0x80u));
    }
    if (r_u8(port + 232u) != 8u && r_u32(0x800B1F00u) == 2u) {
        pad_timer(60);
        while (!pad_expired()) {}
    }
    pad_write8(r_u32(0x800B1F20u), outgoing);
    {
        uint32 position = r_u8(port + 69u);
        length = r_u8(port + 68u);
        w_u8(port + 69u, (uint8)(position + 1u));
        if (length != 255u) {
            length = r_u8(port + 68u);
            w_u8(r_u32(port + 60u) + length, (uint8)received);
        }
        w_u8(port + 68u, (uint8)(r_u8(port + 68u) + 1u));
    }
    return (sint32)received;
}
