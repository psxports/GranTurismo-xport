#include "game_draft_registry.h"

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);

static void gtmain_initial_ot_clear(uint32 index)
{
    uint32 address = r_u32(0x800C00E4u + index * 4u);
    uint32 count = r_u32(0x800C00F0u);
    uint32 node;
    ClearOTagR((uint32 *)psx_addr(address, count * 4u), (sint32)count);
    w_u32(0x800B2574u, 0x040B2560u);
    w_u32(address, 0x000B2574u);
    node = r_u32(0x800C00E0u);
    while (node) {
        w_u32(node + 24u, r_u32(node + index * 4u + 16u));
        node = r_u32(node + 4u);
    }
}

uint32 gtmain_initial_frame(void)
{
    uint32 bound = r_u32(0x8008F1D8u);
    uint32 storage = (bound - 0x4128u) & ~15u;
    uint32 i;
    if (bound > 0x80200000u || storage < 0x801FB740u || storage >= bound)
        return gt_draft_unresolved_call(0x80028994u, storage, bound, 0u, 0u, 0u, 0u, 0u, 0u);
    /* GPU buffers occupy unused memory above game globals without a guest stack */
    w_u32(0x800C00E0u, 0u);
    w_u32(0x800C00E4u, storage + 40u);
    w_u32(0x800C00E8u, storage + 168u);
    w_u32(0x800C00ECu, 0u);
    w_u32(0x800C00F0u, 32u);
    gtmain_initial_ot_clear(0u);
    gtmain_initial_ot_clear(1u);
    w_u32(0x800BFE58u, storage + 296u);
    w_u32(0x800BFE5Cu, storage + 8488u);
    w_u32(0x800BFE48u, 8192u);
    gtmain_display_init(1u);
    sub_8007BD58(storage, 0u, 40u);
    w_u32(storage + 16u, 0x800288F4u);
    w_u32(storage + 8u, 0x80028968u);
    w_u32(storage + 12u, 0x80028984u);
    w_u32(0x800BFF74u, 1u);
    for (i = 0u; i < 2u; ++i) {
        uint32 draw = 0x800BFEC8u + i * 92u;
        w_u8(draw, 1u);
        w_u8(draw + 1u, 0u);
        w_u8(draw + 2u, 0u);
        w_u8(draw + 3u, 0u);
    }
    VSync(0);
    w_u32(0x800BFF70u, 0u);
    SetDispMask(0);
    VSync(0);
    gtmain_font_init(5u);
    w_u32(0x8008F880u, 4u);
    return gtmain_frame_engine(storage, storage - 16u);
}

static uint32 gtmain_initial_text_width(uint32 text)
{
    uint32 width = 0u, character = r_u8(text);
    while (character) {
        uint32 next = (uint32)(sint32)(sint8)r_u8(++text);
        uint32 kern = r_u32(0x800AD498u + character * 4u);
        uint32 adjustment = (r_u32(kern + ((next >> 1u) & 0x7Cu)) >> ((next & 7u) * 4u)) & 15u;
        width += 1u + r_u8(0x800A7618u + character * 8u + 6u) - adjustment;
        character = next & 255u;
    }
    return width;
}

static void gtmain_packet_link(uint32 table, uint32 packet)
{
    uint32 previous = r_u32(table) & 0xFFFFFFu;
    w_u32(table, (r_u32(table) & 0xFF000000u) | (packet & 0xFFFFFFu));
    w_u32(packet, (r_u32(packet) & 0xFF000000u) | previous);
}

static void gtmain_initial_text_draw(void)
{
    uint32 width = gtmain_initial_text_width(0x800BD944u);
    uint32 text = 0x800BD944u, character = r_u8(text);
    uint32 index = r_u32(0x800BFF68u);
    uint32 table = r_u32(0x800C00E4u + index * 4u) + r_u32(0x800C00F0u) * 4u - 4u;
    uint32 mode_packet = r_u32(0x800BFE4Cu), packet = mode_packet + 8u;
    uint32 x = (uint32)((sint32)(320u - width) >> 1u);
    w_u16(0x800C008Au, 3u);
    w_u32(0x800C0070u, 0x800A7618u);
    w_u32(0x800C0074u, 0x800AD498u);
    w_u16(0x800C008Eu, 24u);
    w_u16(0x800C0088u, r_u16(0x800C0078u) + 192u);
    w_u8(0x800C007Cu, 0x80u);
    w_u8(0x800C007Du, 0x80u);
    w_u8(0x800C007Eu, 0x80u);
    w_u32(mode_packet, 0x01000000u);
    w_u32(mode_packet + 4u, 0xE1000000u | r_u16(0x800C007Au) | (r_u32(0x1F801814u) & 0x600u));
    gtmain_packet_link(table, mode_packet);
    while (character) {
        uint32 glyph = r_u32(0x800A7618u + character * 8u);
        uint32 metrics = r_u32(0x800A761Cu + character * 8u);
        uint32 height = glyph >> 24u;
        uint32 next, kern, adjustment;
        if (height) {
            if (packet > r_u32(0x800BFE50u) - 20u)
                gt_draft_unresolved_call(0x80072418u, packet, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
            w_u32(packet, 0x04000000u);
            w_u32(packet + 4u, r_u32(0x800C007Cu));
            w_u16(packet + 8u, x);
            w_u16(packet + 10u, 108u + ((metrics >> 8u) & 255u) - 4u);
            w_u16(packet + 12u, glyph);
            w_u16(packet + 14u, r_u16(0x800C0088u));
            w_u16(packet + 16u, (glyph >> 16u) & 255u);
            w_u16(packet + 18u, height);
            gtmain_packet_link(mode_packet, packet);
            packet += 20u;
        }
        next = (uint32)(sint32)(sint8)r_u8(++text);
        kern = r_u32(0x800AD498u + character * 4u);
        adjustment = (r_u32(kern + ((next >> 1u) & 0x7Cu)) >> ((next & 7u) * 4u)) & 15u;
        x += 1u + ((metrics >> 16u) & 255u) - adjustment;
        character = next & 255u;
    }
    w_u32(0x800BFE4Cu, packet);
}

static uint32 gtmain_frame_callback(uint32 address)
{
    if (!address || address == 0x8006F11Cu) return 0u;
    if (address == 0x80028968u) {
        uint32 remaining = r_u32(0x8008F880u) - 1u;
        w_u32(0x8008F880u, remaining);
        return remaining;
    }
    if (address == 0x80028984u) return r_u32(0x8008F880u) == 0u;
    if (address == 0x800288F4u) {
        gtmain_initial_text_draw();
        return 0u;
    }
    /* TODO Translate drawing callback and further mode callbacks */
    return gt_draft_unresolved_call(address, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

void gtmain_frame_vblank(void)
{
    w_u32(0x800C009Cu, r_u32(0x800C009Cu) + 1u);
    w_u32(0x800C0094u, r_u32(0x800C0094u) + 1u);
    gtmain_frame_callback(r_u32(0x800C0090u));
}

static void gtmain_frame_environments(void)
{
    uint32 display = r_u32(0x800BFF68u);
    uint32 draw = r_u32(0x800BFF6Cu);
    PutDispEnv((DISPENV *)psx_addr(0x800BFE88u + display * 20u, sizeof(DISPENV)));
    PutDrawEnv((DRAWENV *)psx_addr(0x800BFEB0u + draw * 92u, sizeof(DRAWENV)));
}

static void gtmain_frame_buffers(void)
{
    uint32 index = r_u32(0x800BFF68u);
    uint32 start;
    gtmain_initial_ot_clear(index);
    start = r_u32(0x800BFE58u + index * 4u);
    w_u32(0x800BFE60u, index);
    w_u32(0x800BFE4Cu, start);
    w_u32(0x800BFE50u, start + r_u32(0x800BFE48u));
}

uint32 gtmain_frame_engine(uint32 descriptor, uint32 callback_node)
{
    uint32 callback = r_u32(descriptor), result;
    w_u32(0x800C0098u, descriptor);
    w_u32(0x800C0094u, 0u);
    w_u32(0x800C0090u, callback ? callback : 0x8006F11Cu);
    w_u32(0x800C009Cu, 0u);
    w_u32(callback_node + 8u, 0x8006F124u);
    w_u32(callback_node + 12u, 0u);
    gtmain_frame_callback_insert(callback_node);
    VSync(0);
    DrawSync(1);
    gtmain_frame_environments();
    for (;;) {
        gtmain_frame_callback(r_u32(descriptor + 4u));
        if (!r_u8(descriptor + 38u)) gtmain_frame_buffers();
        gtmain_frame_callback(r_u32(descriptor + 8u));
        callback = r_u32(descriptor + 12u);
        if (callback && (result = gtmain_frame_callback(callback)) != 0u) break;
        gtmain_frame_callback(r_u32(descriptor + 16u));
        if (!r_u8(descriptor + 36u)) DrawSync(0);
        gtmain_frame_callback(r_u32(descriptor + 20u));
        /* Native VSync delivers the frame callback while waiting */
        while ((sint32)r_u32(0x800C009Cu) < (sint32)r_u32(0x800BFF74u)) VSync(0);
        if (r_u8(descriptor + 37u)) {
            w_u32(0x800C009Cu, 0u);
            while (!r_u32(0x800C009Cu)) {
                gtmain_frame_callback(r_u32(descriptor + 24u));
                VSync(0);
            }
        }
        w_u32(0x800C009Cu, 0u);
        if (r_u8(descriptor + 36u)) DrawSync(1);
        gtmain_frame_callback(r_u32(descriptor + 28u));
        {
            uint32 index = r_u32(0x800BFF68u);
            w_u32(0x800BFF68u, r_u32(0x800BFF6Cu));
            w_u32(0x800BFF6Cu, index);
        }
        gtmain_frame_environments();
        if (!r_u32(0x800BFF70u)) {
            w_u32(0x800BFF70u, 1u);
            SetDispMask(1);
        }
        gtmain_frame_callback(r_u32(descriptor + 32u));
        if (!r_u8(descriptor + 38u)) {
            uint32 index = r_u32(0x800BFF6Cu);
            uint32 table = r_u32(0x800C00E4u + index * 4u);
            table += r_u32(0x800C00F0u) * 4u - 4u;
            DrawOTag((uint32 *)psx_addr(table, 4u));
        }
    }
    DrawSync(0);
    {
        uint32 previous = r_u32(callback_node), next = r_u32(callback_node + 4u);
        w_u32(previous ? previous + 4u : 0x800B1A20u, next);
        if (next) w_u32(next, previous);
    }
    return result;
}
