#include "game_first2_registry.h"
#include <string.h>

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

static sint32 gtmain_native_pad_info_act(uint32 port, sint32 index, sint32 term)
{
    /* Native digital pads expose no actuator alignment */
    if ((port == 0u || port == 16u) &&
        PadGetState((sint32)port) == 6 && PadInfoMode((sint32)port, 1, 0) == 4)
        return 0;
    /* TODO Bind other native pad capability descriptors */
    gt_draft_failure("PadInfoAct native actuator descriptor", 0x8008028Cu);
}

/* GTMAIN.EXE 8007B7B4 */
/* Original: GTMAIN.EXE:sub_8007B7B4. */ void gtmain_input_buttons_reset(uint32 state)
{
    w_u16(state, 0u);
    w_u16(state + 2u, 0u);
    w_u16(state + 4u, 0u);
    w_u16(state + 6u, 0u);
}

/* GTMAIN.EXE 8007B7C8 */
/* Original: GTMAIN.EXE:sub_8007B7C8. */ uint32 gtmain_input_buttons_update(uint32 state, uint32 buttons)
{
    uint32 valid = r_u16(state);
    uint32 previous = r_u16(state + 2u);
    uint32 changed, released;
    w_u16(state, 0xFFFFu);
    w_u16(state + 2u, (uint16)buttons);
    changed = previous ^ buttons;
    released = changed & previous;
    w_u16(state + 4u, (uint16)(valid & changed & buttons));
    w_u16(state + 6u, (uint16)(valid & released));
    return released;
}

/* GTMAIN.EXE 8007B7FC */
/* Original: GTMAIN.EXE:sub_8007B7FC. */ sint32 gtmain_input_button_repeat(uint32 counter, uint32 held)
{
    uint32 count, delay, interval;
    sint32 repeat = 0;
    if (!held) {
        w_u16(counter + 2u, 0u);
        return 0;
    }
    interval = r_u8(counter + 1u);
    count = r_u16(counter + 2u) + 1u;
    delay = r_u8(counter);
    if (count == delay) repeat = -1;
    else if (count == delay + interval) {
        count = delay;
        repeat = -1;
    }
    w_u16(counter + 2u, (uint16)count);
    return repeat;
}

/* GTMAIN.EXE 8007A81C */
/* Original: GTMAIN.EXE:sub_8007A81C. */ uint32 gtmain_input_reset(uint32 state)
{
    uint32 button;
    gtmain_input_buttons_reset(state);
    w_u16(state + 8u, 0u);
    for (button = 0u; button < 16u; ++button)
        w_u32(state + 10u + 4u * button, r_u32(0x800B1A8Cu));
    return 64u;
}

/* GTMAIN.EXE 8007A87C */
/* Original: GTMAIN.EXE:sub_8007A87C. */ uint32 gtmain_input_packet(uint32 port)
{
    uint32 packet = 0x800C0B70u + ((port >> 2u) << 6u);
    if ((r_u8(packet + 1u) & 0xF0u) == 0x80u)
        packet += 2u + 8u * (port & 3u);
    return packet;
}

/* GTMAIN.EXE 8007A8BC */
/* Original: GTMAIN.EXE:sub_8007A8BC. */ sint32 gtmain_input_decode(uint32 state, uint32 packet)
{
    uint32 buttons = 0u, repeated = 0u, button;
    uint32 family = 0u, readable = packet && !r_u8(packet);
    if (readable) {
        buttons = (uint16)~r_u16(packet + 2u);
        family = r_u8(packet + 1u) & 0xF0u;
        readable = family != 0x10u && family != 0x30u && family != 0x60u;
    }
    if (readable) {
        if (family == 0x20u) {
            uint32 first = r_u8(packet + 5u);
            uint32 second = r_u8(packet + 6u);
            uint32 third = r_u8(packet + 7u);
            if (first >= 104u) buttons |= 0x4000u;
            if (second >= 104u) buttons |= 0x8000u;
            if (third >= 104u) buttons |= 0x0400u;
        }
        gtmain_input_buttons_update(state, buttons);
    } else {
        buttons = 0u;
        gtmain_input_buttons_reset(state);
    }
    for (button = 0u; button < 16u; ++button) {
        uint32 bit = 1u << button;
        repeated |= bit & (uint32)gtmain_input_button_repeat(state + 10u + 4u * button, buttons & bit);
    }
    w_u16(state + 8u, (uint16)repeated);
    return 0;
}

/* GTMAIN.EXE 8007A6F0 */
/* Original: GTMAIN.EXE:sub_8007A6F0. */ sint32 gtmain_input_connected(uint32 port)
{
    uint32 sdk_port = (port ^ (port & 4u)) | ((port & 4u) << 2u);
    sint32 state = PadGetState((sint32)sdk_port);
    return state == 0 || state == 2 || state == 6;
}

/* GTMAIN.EXE 8007A7F8 */
/* Original: GTMAIN.EXE:sub_8007A7F8. */ uint32 gtmain_input_rumble(uint32 slot, uint8 small, uint8 large)
{
    uint32 request = 0x800C0A90u + 8u * slot;
    w_u8(request + 19u, small);
    w_u8(request + 20u, large);
    w_u8(request + 18u, 1u);
    return 1u;
}

/* GTMAIN.EXE 8007B848 */
/* Original: GTMAIN.EXE:sub_8007B848. */ uint32 gtmain_input_port_update(uint32 state, uint32 port, uint32 slot)
{
    uint32 present = 1u << (slot & 31u);
    uint32 entry = state + 16u + 8u * slot;
    uint32 sdk_state = (uint8)PadGetState((sint32)port);
    uint32 actuators, request;
    w_u8(entry, (uint8)sdk_state);
    actuators = r_u8(entry + 1u);
    if (sdk_state == 2u) actuators = 1u;
    else if (!sdk_state) { present = 0u; actuators = 0u; }
    else if (sdk_state == 6u) actuators = (uint32)gtmain_native_pad_info_act(port, -1, 0);
    request = r_u8(entry + 2u);
    w_u8(entry + 1u, (uint8)actuators);
    if (!request) return present;
    {
        uint32 small = r_u8(entry + 3u), large = r_u8(entry + 4u);
        if (actuators == 1u) {
            if (!small) small = large >= 97u;
            large = small;
            small = 64u;
        }
        w_u8(entry + 5u, (uint8)small);
        w_u8(entry + 6u, (uint8)large);
        PadSetAct((sint32)port, psx_addr(entry + 5u, 2u), 2);
    }
    if (sdk_state == 0u || sdk_state == 2u)
        w_u8(entry + 2u, 0u);
    else if (sdk_state == 6u && PadSetActAlign((sint32)port, psx_addr(0x800B1A9Cu, 6u)))
        w_u8(entry + 2u, 0u);
    return present;
}

/* GTMAIN.EXE 8007B9A4 */
/* Original: GTMAIN.EXE:sub_8007B9A4. */ uint32 gtmain_input_scan(uint32 state)
{
    uint32 physical, present = 0u;
    for (physical = 0u; physical < 2u; ++physical) {
        uint32 packet = 0x800C0B70u + 64u * physical;
        uint32 port = 16u * physical, slot = 4u * physical;
        w_u8(state + 16u + 8u * physical, 0u);
        if (r_u8(packet)) continue;
        present |= 1u << (physical + 8u);
        if ((r_u8(packet + 1u) & 0xF0u) == 0x80u) {
            uint32 tap;
            present |= 1u << (physical + 10u);
            for (tap = 0u; tap < 4u; ++tap)
                present |= gtmain_input_port_update(state, port + tap, slot + tap);
        } else {
            present |= gtmain_input_port_update(state, port, slot);
        }
    }
    return present;
}

/* GTMAIN.EXE 8007A9EC */
/* Original: GTMAIN.EXE:sub_8007A9EC. */ uint32 gtmain_input_poll(void)
{
    uint32 present, previous, changed, slot;
    /* Native VBlank publishes the bound host controller packets */
    memcpy(psx_addr(0x800C0B70u, 128u), psx_addr(0x800C0AF0u, 128u), 128u);
    memset(psx_addr(0x800C0AE0u, 16u), 0, 16u);
    present = gtmain_input_scan(0x800C0A90u);
    previous = r_u32(0x800C0A90u);
    w_u32(0x800C0A90u, present);
    changed = previous ^ present;
    w_u32(0x800C0A94u, changed);
    for (slot = 0u; slot < 8u; ++slot) {
        uint32 type = 0u;
        if (present & (1u << slot)) {
            uint32 count_address;
            type = r_u8(gtmain_input_packet(slot) + 1u);
            count_address = 0x800C0AE0u + (type >> 4u);
            w_u8(count_address, (uint8)(r_u8(count_address) + 1u));
        }
        w_u8(0x800C0A98u + slot, (uint8)type);
    }
    return changed;
}
