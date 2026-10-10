#include "game_draft_registry.h"

static uint32 intro_frame_active;

void gtos_intro_frame_vblank(void)
{
    if (!intro_frame_active) return;
    w_u32(0x80079F0Cu, r_u32(0x80079F0Cu) + 1u);
    w_u32(0x80079F04u, r_u32(0x80079F04u) + 1u);
}

static void gtos_intro_environments(void)
{
    uint32 display = r_u32(0x8007A138u), draw = r_u32(0x8007A13Cu);
    PutDispEnv((DISPENV *)psx_addr(0x8007A058u + display * 20u, 20u));
    PutDrawEnv((DRAWENV *)psx_addr(0x8007A080u + draw * 92u, 92u));
}

static void gtos_intro_clear(uint32 index)
{
    uint32 node = r_u32(0x8007A150u), count = r_u32(0x8007A160u);
    uint32 ot = r_u32(0x8007A154u + index * 4u);
    ClearOTagR((uint32 *)psx_addr(ot, count * 4u), (sint32)count);
    w_u32(0x80074344u, (r_u32(0x80074330u) & 0xFFFFFFu) | 0x04000000u);
    w_u32(ot, 0x00074344u);
    while (node) {
        w_u32(node + 24u, r_u32(node + 16u + index * 4u));
        node = r_u32(node + 4u);
    }
    w_u32(0x8007A038u, index);
    ot = r_u32(0x8007A030u + index * 4u);
    w_u32(0x8007A024u, ot);
    w_u32(0x8007A028u, ot + r_u32(0x8007A020u));
}

static void gtos_intro_frame_loop(void)
{
    uint32 draw, display;
    /* Native callback state replaces the original temporary guest stack node */
    w_u32(0x80079F04u, 0u);
    w_u32(0x80079F0Cu, 0u);
    intro_frame_active = 1u;
    VSync(0);
    gtos_graph_reset(1u);
    gtos_intro_environments();
    for (;;) {
        draw = r_u32(0x8007A138u);
        gtos_intro_clear(draw);
        w_u32(0x8004D980u, r_u32(0x8004D980u) + 1u);
        if ((sint32)r_u32(0x8004D980u) >= 36) break;
        gtos_intro_draw();
        DrawSync(0);
        while (r_u32(0x80079F0Cu) < r_u32(0x8007A144u)) VSync(0);
        w_u32(0x80079F0Cu, 0u);
        VSync(0);
        display = r_u32(0x8007A13Cu);
        w_u32(0x8007A138u, display);
        w_u32(0x8007A13Cu, draw);
        gtos_intro_environments();
        if (!r_u32(0x8007A140u)) {
            w_u32(0x8007A140u, 1u);
            SetDispMask(1);
        }
        draw = r_u32(0x8007A13Cu);
        DrawOTag((uint32 *)psx_addr(r_u32(0x8007A154u + draw * 4u) + r_u32(0x8007A160u) * 4u - 4u, 4u));
    }
    DrawSync(0);
    intro_frame_active = 0u;
}

static uint32 gtos_link_packet(uint32 slot, uint32 packet)
{
    uint32 next = r_u32(slot) & 0xFFFFFFu;
    w_u32(slot, (r_u32(slot) & 0xFF000000u) | (packet & 0xFFFFFFu));
    w_u32(packet, (r_u32(packet) & 0xFF000000u) | next);
    return packet;
}

uint32 gtos_intro_draw(void)
{
    uint32 slot = r_u32(0x80075748u), packet = r_u32(0x8007A024u);
    sint32 x0 = (sint16)r_u16(0x8004D984u), y0 = (sint16)r_u16(0x8004D986u);
    sint32 x1 = (sint16)r_u16(0x8004D988u), y1 = (sint16)r_u16(0x8004D98Au);
    uint32 mode;
    /* Insert the drawing mode and flat rectangle in the original packet order */
    w_u32(packet, 0x01000000u);
    w_u32(packet + 4u, 0xE1000040u);
    mode = gtos_link_packet(slot, packet);
    packet += 8u;
    if (x1 < x0) ++x0; else ++x1;
    if (y1 < y0) ++y0; else ++y1;
    w_u32(packet, 0x05000000u);
    w_u32(packet + 4u, 34607120u ^ 0x29000000u);
    w_u16(packet + 8u, x0);
    w_u16(packet + 10u, y0);
    w_u16(packet + 12u, x1);
    w_u16(packet + 14u, y0);
    w_u16(packet + 16u, x0);
    w_u16(packet + 18u, y1);
    w_u16(packet + 20u, x1);
    w_u16(packet + 22u, y1);
    gtos_link_packet(mode, packet);
    w_u32(0x8007A024u, packet + 24u);
    return packet;
}

uint32 gtos_render_node_insert(uint32 node)
{
    uint32 used = r_u32(0x8007A15Cu) + r_u32(node + 12u);
    uint32 current, previous = 0u, first, second;
    if (r_u32(0x8007A160u) < used) return 0u;
    w_u32(0x8007A15Cu, used);
    current = r_u32(0x8007A150u);
    while (current) {
        if ((sint32)r_u32(node + 8u) < (sint32)r_u32(current + 8u)) break;
        previous = current;
        current = r_u32(current + 4u);
    }
    if (previous) w_u32(previous + 4u, node);
    else w_u32(0x8007A150u, node);
    if (current) w_u32(current, node);
    w_u32(node, previous);
    w_u32(node + 4u, current);
    first = r_u32(0x8007A154u);
    second = r_u32(0x8007A158u);
    current = r_u32(0x8007A150u);
    while (current) {
        uint32 bytes = r_u32(current + 12u) * 4u;
        w_u32(current + 16u, first);
        w_u32(current + 20u, second);
        w_u32(current + 24u, first);
        current = r_u32(current + 4u);
        first += bytes;
        second += bytes;
    }
    return 1u;
}

void gtos_intro_prepare(void)
{
    uint32 i, table = 0x800526C8u + 6u * 20u;
    const uint32 buffers[2] = { 0x8015EF7Cu, 0x80163F7Cu };
    w_u32(0x8007A150u, 0u);
    w_u32(0x8007A154u, buffers[0]);
    w_u32(0x8007A158u, buffers[1]);
    w_u32(0x8007A15Cu, 0u);
    w_u32(0x8007A160u, 0x1400u);
    for (i = 0u; i < 2u; ++i) {
        ClearOTagR((uint32 *)psx_addr(buffers[i], 0x5000u), 0x1400);
        w_u32(0x80074344u, (r_u32(0x80074330u) & 0xFFFFFFu) | 0x04000000u);
        w_u32(buffers[i], 0x00074344u);
    }
    w_u32(0x8007A030u, 0x80168F7Cu);
    w_u32(0x8007A034u, 0x80178F7Cu);
    w_u32(0x8007A020u, 0x10000u);
    w_u32(0x8007A040u, 6u);
    w_u32(0x8007A044u, r_u16(table));
    w_u16(0x8007A048u, 0u);
    w_u16(0x8007A04Au, 0u);
    for (i = 0u; i < 4u; ++i)
        w_u16(0x8007A04Cu + i * 2u, r_u16(table + 2u + i * 2u));
    w_u16(0x8007A054u, r_u16(table + 2u));
    w_u16(0x8007A056u, r_u16(table + 4u));
    for (i = 0u; i < 2u; ++i) {
        uint32 rect = 0x8007A048u + i * 8u;
        sint32 x = (sint16)r_u16(rect), y = (sint16)r_u16(rect + 2u);
        sint32 width = (sint16)r_u16(rect + 4u), height = (sint16)r_u16(rect + 6u);
        SetDefDispEnv((DISPENV *)psx_addr(0x8007A058u + i * 20u, 20u), x, y, width, height);
        SetDefDrawEnv((DRAWENV *)psx_addr(0x8007A080u + i * 92u, 92u), x, y, width, height);
    }
    w_u32(0x8007A138u, 0u);
    w_u32(0x8007A13Cu, 1u);
    w_u32(0x8007A144u, 1u);
    w_u32(0x8007A098u, 0u);
    w_u32(0x8007A0F4u, 0u);
    w_u32(0x80075738u, 0u);
    w_u32(0x8007573Cu, 32u);
    if (!gtos_render_node_insert(0x80075730u))
        gt_draft_unresolved_call(0x8002A3ACu, 0x80075730u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gtos_intro_frame_loop();
}
