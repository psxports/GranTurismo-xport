#include "game_first2_registry.h"

static void pad_callback(uint32 token, uint32 port)
{
    if (token == 0x8007FB28u) gtmain_pad_reset_port(port);
    else if (token == 0x8007FC84u) gtmain_pad_command_clear(port);
    else if (token == 0x800811C0u) (void)gtmain_pad_response_apply(port);
    else {
        /* TODO Bind the actual native port callback token */
        gt_draft_failure("Native pad descriptor callback", token);
    }
}

static void pad_timer_snapshot(uint32 value)
{
    /* TODO Bind original timer1 counter read and snapshot service */
    gt_draft_failure("Native pad timer snapshot", 0x80081E98u);
}

static void pad_serial_wait(void)
{
    (void)gtmain_pad_serial_wait();
}

/* GTMAIN.EXE 8007FC84 */
uint32 gtmain_pad_command_clear(uint32 port)
{
    uint32 result = r_u8(port + 54u);
    w_u8(port + 54u, 0);
    w_u8(port + 55u, (uint8)result);
    return result;
}

/* GTMAIN.EXE 8007FC94 */
uint32 gtmain_pad_command_byte(uint32 port)
{
    sint32 index = (sint32)r_u8(port + 69u) - 3;
    uint8 command = r_u8(port + 54u);
    if (command) {
        uint8 count = r_u8(port + 53u);
        if (index >= count) return command == 77u ? 255u : 0u;
        return r_u8(r_u32(port + 44u) + (uint32)index);
    }
    if (index < 6 && !r_u8(port + 87u + (uint32)index)) return 0;
    if (index >= r_u8(port + 52u)) return 0;
    return r_u8(r_u32(port + 40u) + (uint32)index);
}

/* GTMAIN.EXE 8007FD50 */
void gtmain_pad_command_select(uint32 port)
{
    uint32 index;
    for (index = 0; index < 6u; ++index) w_u8(port + 87u + index, 0);
    if (r_u16(port + 230u) && r_u32(port + 40u)) {
        uint32 slots = r_u8(port + 52u), descriptor = 0;
        if (slots >= 7u) slots = 6;
        while (descriptor < r_u8(port + 233u)) {
            uint32 entries = r_u32(port + 4u), offset = descriptor * 5u;
            uint32 mask = r_u8(entries + offset + 2u) ? 255u : 1u;
            uint32 data = r_u32(port + 40u), slot;
            uint32 found = 0;
            for (slot = 0; slot < slots; ++slot) {
                if (r_u8(port + 93u + slot) == descriptor && (r_u8(data + slot) & mask)) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                sint32 sum = (sint32)(r_u32(0x800B1F04u) + r_u8(r_u32(port + 4u) + offset + 3u));
                if (sum < 61) {
                    w_u32(0x800B1F04u, (uint32)sum);
                    for (slot = 0; slot < slots; ++slot)
                        if (r_u8(port + 93u + slot) == descriptor) w_u8(port + 87u + slot, 1);
                }
            }
            ++descriptor;
        }
    } else {
        uint32 kind = r_u8(port + 232u);
        if ((kind == 4u || kind == 5u || kind == 7u) && !r_u16(port + 230u) && r_u8(port + 52u) >= 2u) {
            uint32 data = r_u32(port + 40u);
            if ((r_u8(data) & 0xC0u) == 64u && (r_u8(data + 1u) & 1u) &&
                (sint32)(r_u32(0x800B1F04u) + 10u) < 61) {
                w_u8(port + 88u, 1); w_u8(port + 87u, 1);
                w_u32(0x800B1F04u, r_u32(0x800B1F04u) + 10u);
            }
        } else if (r_u8(port + 232u) == 3u) {
            w_u8(port + 87u, 1);
        } else if (!r_u16(port + 230u)) {
            sint32 slot;
            for (slot = 5; slot >= 0; --slot) w_u8(port + 87u + (uint32)slot, 1);
        }
    }
}

/* GTMAIN.EXE 80081424 */
uint32 gtmain_pad_port_failure(uint32 port)
{
    uint32 counter = r_u32(port + 76u);
    uint32 state = r_u8(port + 70u), result;
    w_u32(port + 76u, counter + 1u);
    if (state) {
        uint32 attempt = r_u8(port + 74u);
        if (state == 1u) {
            result = attempt + 1u;
            if (attempt < 2u) { w_u8(port + 74u, (uint8)result); return result; }
            w_u8(port + 73u, 2); w_u8(port + 70u, 255);
            return 255;
        }
        result = attempt + 1u;
        if (attempt < 4u) { w_u8(port + 74u, (uint8)result); return result; }
        if (r_u8(port + 73u)) pad_callback(r_u32(0x800B1EC0u), port);
    }
    if (r_u8(r_u32(port + 60u)) == 243u) return 243;
    w_u8(r_u32(port + 48u), 255);
    result = r_u32(port + 48u);
    w_u8(result + 1u, 0); w_u8(port + 232u, 0);
    return result;
}

/* GTMAIN.EXE 80080914 */
/* Original: GTMAIN.EXE:sub_80080914. */ sint32 gtmain_pad_port_begin(uint32 port)
{
    uint32 control = r_u32(0x800B1F20u), count_address, index;
    sint32 count;
    w_u16(control + 10u, 64); w_u16(control + 10u, 0);
    w_u16(control + 8u, 13); w_u16(control + 14u, 136);
    pad_timer_snapshot(r_u8(port + 232u) == 8u ? 80u : 145u);
    index = r_u32(0x800B1EFCu);
    control = r_u32(0x800B1F20u);
    w_u16(control + 10u, index ? 12291u : 4099u);
    count_address = 0x800B1F14u + 4u * index;
    count = (sint32)r_u32(count_address);
    if (count >= 0) {
        if (count > 0) do {
            count_address = 0x800B1F14u + 4u * r_u32(0x800B1EFCu);
            count = (sint32)(r_u32(count_address) - 1u);
            w_u32(count_address, (uint32)count);
            {
                uint32 children = r_u32(port + 12u);
                uint32 callback = r_u32(0x800B1EDCu);
                pad_callback(callback, children + 240u * (uint32)count);
            }
            count = (sint32)r_u32(0x800B1F14u + 4u * r_u32(0x800B1EFCu));
        } while (count > 0);
        count_address = 0x800B1F14u + 4u * r_u32(0x800B1EFCu);
        if (!r_u32(count_address)) {
            uint32 token = r_u32(0x800B1EDCu);
            w_u32(count_address, 0xFFFFFFFFu);
            pad_callback(token, port);
            pad_callback(r_u32(0x800B1EE0u), port);
        }
    }
    control = r_u32(0x800B1F20u);
    if (r_u16(control + 4u) & 0x200u) {
        w_u16(control + 10u, r_u16(control + 10u) | 0x10u);
        if (r_u16(control + 4u) & 0x200u) {
            w_u8(control, 1);
            pad_serial_wait();
            (void)r_u8(r_u32(0x800B1F20u));
            return 0;
        }
        w_u32(r_u32(0x800B1F1Cu), 0xFFFFFF7Fu);
    }
    if (r_u8(port + 80u) && r_u8(port + 54u)) return 0;
    w_u8(r_u32(port + 60u), 0);
    return 1;
}

/* GTMAIN.EXE 8007FB90 */
sint32 gtmain_pad_next_port(sint32 reason)
{
    sint32 result;
    do {
        uint32 index = r_u32(0x800B1EFCu);
        uint32 port = 0x800B5D0Cu + 240u * index;
        uint32 next;
        if (reason != -9) {
            if (reason) {
                gtmain_pad_port_failure(port);
                gtmain_pad_command_clear(port);
            } else w_u32(0x800B1F14u + 4u * index, 0);
        }
        {
            uint32 control = r_u32(0x800B1EA4u);
            next = r_u32(0x800B1EFCu) + 1u;
            w_u32(0x800B1F00u, 0); w_u16(control + 10u, 0);
        }
        {
            sint32 last = (sint32)r_u32(0x800B1F10u);
            w_u32(0x800B1EFCu, next);
            result = last < (sint32)next ? 1 : gtmain_pad_port_begin(0x800B5D0Cu + 240u * next);
        }
        reason = 65535;
    } while (!result);
    return result;
}
