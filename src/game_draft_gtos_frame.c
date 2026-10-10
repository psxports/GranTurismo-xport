#include "game_draft_registry.h"
#include "psx_spu.h"
#include <string.h>

static void gtos_vblank(void)
{
    uint32 node = r_u32(0x800736A0u);
    gtos_intro_frame_vblank();
    gtos_movie_vblank();
    while (node) {
        uint32 callback = r_u32(node + 8u), next = r_u32(node + 4u);
        /* TODO Dispatch translated GTOS frame callbacks */
        if (callback == 0x80010154u) {
            gtos_sound_vblank();
        } else if (callback) gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        node = next;
    }
    w_u32(0x8007AA24u, r_u32(0x8007AA24u) + 1u);
    w_u32(0x8007AA28u, r_u32(0x8007AA28u) + 1u);
}

uint32 gtos_frame_init(uint32 mode)
{
    if (r_u32(0x800736A4u)) return 0xFFFFFFFFu;
    w_u32(0x800736A4u, 1u);
    w_u32(0x8007AA24u, 0u);
    w_u32(0x8007AA28u, 0u);
    gtos_graph_reset(mode);
    w_u32(0x800736A0u, 0u);
    VSyncCallback(gtos_vblank);
    return 0u;
}

static sint32 gtos_event_flag(uint32 bit)
{
    uint32 result = r_u16(0x800736A8u) | bit;
    w_u16(0x800736A8u, result);
    return (sint32)result;
}

#define GTOS_EVENT(index, bit) static sint32 gtos_event_##index(void) { return gtos_event_flag(bit); }
GTOS_EVENT(0, 1u)
GTOS_EVENT(1, 2u)
GTOS_EVENT(2, 4u)
GTOS_EVENT(3, 8u)
GTOS_EVENT(4, 16u)
GTOS_EVENT(5, 32u)
GTOS_EVENT(6, 64u)
GTOS_EVENT(7, 128u)
#undef GTOS_EVENT

void gtos_frame_events_init(void)
{
    static const PsxEventCallback callbacks[8] = {
        gtos_event_0, gtos_event_1, gtos_event_2, gtos_event_3,
        gtos_event_4, gtos_event_5, gtos_event_6, gtos_event_7
    };
    uint32 i;
    for (i = 0u; i < 8u; ++i) {
        uint32 row = 0x800736ACu + i * 12u;
        uint32 event = (uint32)OpenEvent(r_u32(row), (sint32)r_u32(row + 4u), 0x1000, callbacks[i]);
        if (event != 0xFFFFFFFFu && !EnableEvent(event))
            gt_draft_unresolved_call(0x8003BA00u, event, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u32(0x8007AA00u + i * 4u, event);
    }
}

void gtos_frame_callback_insert(uint32 node)
{
    uint32 current = r_u32(0x800736A0u), previous = 0u;
    uint32 link = 0x800736A0u, priority = r_u32(node + 12u);
    while (current) {
        if (priority < r_u32(current + 12u)) break;
        previous = current;
        current = r_u32(current + 4u);
    }
    w_u32(node, previous);
    w_u32(node + 4u, current);
    if (previous) link = previous + 4u;
    if (current) w_u32(current, node);
    w_u32(link, node);
}

void gtos_saved_data_init(uint32 buffer)
{
    uint32 checksum;
    w_u32(0x8007412Cu, 0x400u);
    w_u32(0x80074184u, 0u);
    w_u32(0x80074130u, 0u);
    w_u16(r_u32(0x80074114u) + 422u, 0x400u);
    w_u32(0x80074164u, 1u);
    w_u32(0x80074168u, buffer);
    w_u32(0x8007416Cu, 0xB3C0u >> 6u);
    if (!spu_download(0x2000u, psx_addr(buffer, 0xB3C0u), 0xB3C0u))
        gt_draft_unresolved_call(0x80045330u, buffer, 0xB3C0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x80074148u, 1u);
    checksum = gtmain_data_checksum(buffer, 0xB398u);
    if (r_u32(buffer) != 0x47546F73u || r_u32(buffer + 0xB398u) != checksum) {
        memset(psx_addr(buffer, 0xB3C0u), 0, 0xB3C0u);
        w_u32(buffer, 0x47546F73u);
    }
}
