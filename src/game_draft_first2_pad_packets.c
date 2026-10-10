#include "game_first2_registry.h"

static void packet_hardware(uint32 address)
{
    /* TODO Bind actual SIO and IRQ access with the original width */
    if (address >= 0x1F801000u && address < 0x1F802000u)
        gt_draft_failure("Native pad SIO IRQ timer adapter", address);
}
static uint32 packet_callback(uint32 token, uint32 port)
{
    if (token == 0x800810ECu) return gtmain_pad_command_prepare(port);
    if (token == 0x800811C0u) return gtmain_pad_response_apply(port);
    if (token == 0x80081500u) return gtmain_pad_response_pending(port);
    if (token == 0x8007FC84u) return gtmain_pad_command_clear(port);
    if (token == 0x8007FC94u) return gtmain_pad_command_byte(port);
    if (token == 0x8007FB90u) return (uint32)gtmain_pad_next_port((sint32)port);
    gt_draft_failure("Native pad packet callback", token);
}
static void packet_notify(uint32 token, uint32 port)
{
    if (token == 0x8007FD50u) gtmain_pad_command_select(port);
    else if (token == 0x8007FB28u) gtmain_pad_reset_port(port);
    else (void)packet_callback(token, port);
}
static void packet_noarg(uint32 token)
{
    if (token == 0x80080684u) (void)gtmain_pad_poll();
    else if (token == 0x8008061Cu) (void)gtmain_pad_irq_test();
    else if (token == 0x800810B8u) (void)gtmain_pad_callback_bind();
    else gt_draft_failure("Native pad zero argument callback", token);
}
static void packet_ack_delay(void)
{
    /* TODO Bind unselected timer snapshot without fabricated timing */
    gt_draft_failure("Native pad timer snapshot", 0x80081E98u);
}

uint32 gtmain_pad_irq_bind(void)
{
    w_u32(0x800B5EF0u, 0x80080684u);
    w_u32(0x800B5EF4u, 0x8008061Cu);
    w_u32(0x800B5EECu, 0); w_u32(0x800B5EF8u, 0);
    return 0x800B5EF0u;
}
uint32 gtmain_pad_irq_test(void)
{
    uint32 irq = r_u32(0x800B1F1Cu);
    packet_hardware(irq + 4u);
    if (r_u32(irq + 4u) & 1u) {
        packet_hardware(irq);
        if (r_u32(irq) & 1u) {
            uint32 token = r_u32(0x800B1EE4u);
            if (token) packet_noarg(token);
            return 1;
        }
    }
    return 0;
}
uint32 gtmain_pad_port_address(uint8 selector)
{
    return selector & 0xF0u ? 0x800B5DFCu : 0x800B5D0Cu;
}
void gtmain_pad_command_data_bind(uint32 port, uint32 data, uint8 count)
{
    w_u32(port + 40u, data); w_u8(port + 52u, count);
}
sint32 gtmain_pad_packet_start(uint32 port)
{
    uint32 value = packet_callback(r_u32(0x800B1ED4u), port);
    w_u32(0x800B1F3Cu, value);
    return gtmain_pad_exchange(port, -2);
}
sint32 gtmain_pad_packet_command(uint32 port)
{
    uint32 index = r_u32(0x800B1EFCu), first = r_u32(0x800B1F0Cu);
    return gtmain_pad_packet_command_tail(port, index == first);
}
sint32 gtmain_pad_packet_command_tail(uint32 port, uint32 is_first_port)
{
    uint32 command;
    if (is_first_port && r_u32(0x800B1EF8u)) {
        packet_noarg(r_u32(0x800B1EECu)); packet_noarg(r_u32(0x800B1EE8u));
    }
    if (r_u32(0x800B1F3Cu)) {
        (void)packet_callback(r_u32(0x800B1ED4u), r_u32(port + 12u));
        (void)packet_callback(r_u32(0x800B1ED4u), r_u32(port + 12u) + 240u);
    }
    command = r_u8(port + 54u);
    command = command ? r_u8(port + 54u) : 66u;
    return gtmain_pad_exchange_timed(port, (uint8)command);
}
sint32 gtmain_pad_packet_length(uint32 port)
{
    return gtmain_pad_packet_length_tail(port, r_u32(0x800B1F3Cu));
}
sint32 gtmain_pad_packet_length_tail(uint32 port, uint32 prepare_children)
{
    uint32 outgoing = 0, length;
    sint32 result;
    if (prepare_children) {
        (void)packet_callback(r_u32(0x800B1ED4u), r_u32(port + 12u) + 480u);
        (void)packet_callback(r_u32(0x800B1ED4u), r_u32(port + 12u) + 720u);
    }
    if (!r_u8(port + 54u)) outgoing = r_u32(0x800B1F08u);
    result = gtmain_pad_exchange_timed(port, (uint8)outgoing);
    if (result < 0) return result;
    if (!(result & 0xF0)) return -9;
    length = 2u * ((uint32)result & 15u);
    w_u32(0x800B1F34u, length);
    if (!length) w_u32(0x800B1F34u, 32);
    return 0;
}
sint32 gtmain_pad_packet_ack(uint32 port)
{
    uint32 multitap = 0, outgoing;
    sint32 result;
    if ((r_u8(r_u32(port + 60u)) >> 4) == 8u) multitap = !r_u8(port + 54u);
    /* The known command callback ignores the meaningful second argument */
    (void)multitap;
    outgoing = packet_callback(r_u32(0x800B1EC4u), port);
    result = gtmain_pad_exchange_timed(port, (uint8)outgoing);
    if (result == 90 || !result || result < 0) return result;
    return -9;
}
sint32 gtmain_pad_packet_read(uint32 port)
{
    uint32 multitap = 0, related = 0, related_valid = 0, other, count_address, offset;
    sint32 result;
    packet_notify(r_u32(0x800B1EC8u), port);
    if (r_u32(0x800B1F08u) && (r_u8(r_u32(port + 60u)) >> 4) == 8u)
        multitap = !r_u8(port + 54u);
    if (multitap) {
        sint32 index = -1;
        uint32 child_offset = (uint32)-240;
        do {
            uint32 remaining = r_u32(0x800B1F34u) - 1u;
            w_u32(0x800B1F34u, remaining);
            if ((sint32)remaining <= 0) break;
            if (index >= 0) packet_notify(r_u32(0x800B1EC8u), r_u32(port + 12u) + child_offset);
            result = gtmain_pad_exchange_timed(port, (uint8)packet_callback(r_u32(0x800B1EC4u), port));
            if (result < 0) return result;
            packet_ack_delay(); ++index;
            if (!gtmain_pad_serial_ack()) return -3;
            child_offset += 240u;
        } while (index < 4);
    }
    other = r_u32(0x800B1EFCu) == 0;
    count_address = 0x800B1F14u + 4u * other; offset = 240u * other;
    while ((sint32)r_u32(0x800B1F34u) >= 2) {
        sint32 count = (sint32)r_u32(count_address);
        if (count < 0) break;
        if (count > 0) {
            related = r_u32(r_u32(0x800B1EF0u) + offset + 12u) + 240u * (uint32)count - 240u;
            related_valid = 1; packet_notify(r_u32(0x800B1EDCu), related);
        }
        count = (sint32)r_u32(count_address);
        if (count == 3) {
            if (!related_valid) gt_draft_failure("Native pad related descriptor provenance", 0x800821BCu);
            packet_notify(r_u32(0x800B1EDCu), related - 240u); w_u32(count_address, 1);
        } else if (count >= 4) {
            if (count == 4) w_u32(count_address, 3);
        } else if (count < 2 && count >= 0) {
            related = r_u32(0x800B1EF0u) + offset; related_valid = 1;
            packet_notify(r_u32(0x800B1EDCu), related);
            packet_notify(r_u32(0x800B1EE0u), related); w_u32(count_address, (uint32)-1);
        }
        result = gtmain_pad_exchange(port, (uint8)packet_callback(r_u32(0x800B1EC4u), port));
        if (result < 0) return result;
        packet_ack_delay(); if (!gtmain_pad_serial_ack()) return -3;
        w_u32(0x800B1F34u, r_u32(0x800B1F34u) - 1u);
    }
    for (;;) {
        uint32 remaining = r_u32(0x800B1F34u) - 1u;
        w_u32(0x800B1F34u, remaining);
        if ((sint32)remaining <= 0) break;
        result = gtmain_pad_exchange(port, (uint8)packet_callback(r_u32(0x800B1EC4u), port));
        if (result < 0) return result;
        packet_ack_delay(); if (!gtmain_pad_serial_ack()) return -3;
    }
    (void)gtmain_pad_serial_wait();
    {
        uint32 length = r_u8(port + 68u), serial, input;
        w_u8(port + 68u, (uint8)(length + 1u));
        serial = r_u32(0x800B1F38u); input = r_u32(port + 60u);
        packet_hardware(serial); w_u8(input + length, r_u8(serial));
    }
    packet_notify(r_u32(0x800B1EBCu), 0);
    return 0;
}
uint32 gtmain_pad_poll(void)
{
    uint32 first = r_u32(0x800B1F0Cu), index, control;
    w_u32(0x800B1F24u, 1);
    if (first && (sint32)r_u32(0x800B5EFCu) < 150)
        w_u32(0x800B5EFCu, r_u32(0x800B5EFCu) + 1u);
    if (!r_u32(0x800B1F10u) && (sint32)r_u32(0x800B5F00u) < 150)
        w_u32(0x800B5F00u, r_u32(0x800B5F00u) + 1u);
    if (r_u32(0x800B1EF4u) && (sint32)r_u32(0x800B1F10u) >= (sint32)r_u32(0x800B1F0Cu)) {
        first = r_u32(0x800B1F0Cu);
        w_u32(0x800B1F00u, 0); w_u32(0x800B1EFCu, first);
        if (!gtmain_pad_port_begin(r_u32(0x800B1EF0u) + 240u * first))
            packet_notify(r_u32(0x800B1EBCu), 65535);
        index = r_u32(0x800B1EFCu); w_u32(0x800B1F04u, 0);
        while ((sint32)r_u32(0x800B1F10u) >= (sint32)index) {
            (void)gtmain_pad_protocol_step(); index = r_u32(0x800B1EFCu);
        }
        control = r_u32(0x800B1F20u); packet_hardware(control + 14u); w_u16(control + 14u, 136);
    }
    return 0;
}
