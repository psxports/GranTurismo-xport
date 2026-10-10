#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include <stdio.h>
#include <string.h>

uint32 gtmain_graph_reset(uint32 mode)
{
    uint32 selected = mode & 7u;
    uint32 result, callback;
    if (selected == 0u || selected == 3u || selected == 5u) {
        if (selected != 5u)
            printf("ResetGraph:jtb=%08x,env=%08x\n", 0x800B246Cu, 0x800B24B4u);
        memset(psx_addr(0x800B24B4u, 128u), 0, 128u);
        ResetCallback();
        /* Initialize the native GPU and window without BIOS command thunks */
        if (ResetGraph(0) < 0)
            gt_draft_unresolved_call(0x80088DB4u, mode, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        result = (uint32)GetGraphType();
        w_u8(0x800B24B4u, result);
        w_u8(0x800B24B5u, 1u);
        result = r_u8(0x800B24B4u);
        w_u16(0x800B24B8u, r_u16(0x800B2534u + result * 4u));
        w_u16(0x800B24BAu, r_u16(0x800B2540u + result * 4u));
        memset(psx_addr(0x800B24C4u, 92u), 0xFF, 92u);
        memset(psx_addr(0x800B2520u, 20u), 0xFF, 20u);
        return r_u8(0x800B24B4u);
    }
    if (r_u8(0x800B24B6u) >= 2u) {
        callback = r_u32(0x800B24B0u);
        if (callback == 0x8008B7D8u) printf("ResetGraph(%d)...\n", (sint32)mode);
        else gt_draft_unresolved_call(callback, 0x8008EF1Cu, mode, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    /* Soft reset retains native VRAM and display state */
    w_u32(0x800B25B0u, 0u);
    w_u32(0x800B25ACu, 0u);
    w_u32(0x1F801814u, 0x02000000u);
    w_u32(0x1F801814u, 0x01000000u);
    return 0u;
}

static uint32 *native_frame_node;
static uint32 vblank_dispatching;

void gtmain_native_frame_callback_insert(uint32 *node)
{
    uint32 previous = 0u, current;
    /* TODO Bind nested nodes and mutations during mixed VBlank dispatch */
    if (!node || native_frame_node || vblank_dispatching)
        gt_draft_failure("GTMAIN native VBlank node lifetime", 0x8007A608u);
    EnterCriticalSectionPSX();
    current = r_u32(0x800B1A20u);
    while (current) {
        if (node[3] < r_u32(current + 12u)) break;
        previous = current;
        current = r_u32(current + 4u);
    }
    node[0] = previous;
    node[1] = current;
    native_frame_node = node;
    ExitCriticalSection();
}

void gtmain_native_frame_callback_remove(uint32 *node)
{
    if (!node || native_frame_node != node || vblank_dispatching)
        gt_draft_failure("GTMAIN native VBlank node removal", 0x8007B6E4u);
    EnterCriticalSectionPSX();
    native_frame_node = NULL;
    ExitCriticalSection();
}

void gtmain_native_frame_guest_removed(uint32 node, uint32 previous, uint32 next)
{
    if (!native_frame_node) return;
    /* TODO Bind mutations during mixed VBlank dispatch */
    if (vblank_dispatching)
        gt_draft_failure("GTMAIN mixed VBlank list mutation", 0x8007B6E4u);
    if (native_frame_node[0] == node) native_frame_node[0] = previous;
    if (native_frame_node[1] == node) native_frame_node[1] = next;
}

static void native_frame_node_dispatch(uint32 previous, uint32 next)
{
    if (native_frame_node && native_frame_node[0] == previous &&
        native_frame_node[1] == next) {
        uint32 callback = native_frame_node[2];
        if (callback == 0x8006F124u) gtmain_frame_vblank();
        else if (callback)
            gt_draft_failure("GTMAIN native VBlank callback identity", callback);
    }
}

static void gtmain_vblank(void)
{
    uint32 node = r_u32(0x800B1A20u), previous = 0u;
    uint32 first_ticks, second_ticks;
    vblank_dispatching = 1u;
    while (node) {
        native_frame_node_dispatch(previous, node);
        uint32 callback = r_u32(node + 8u);
        uint32 next = r_u32(node + 4u);
        /* TODO Dispatch translated callbacks as they become reachable */
        if (callback == 0x80019EE8u)
            gt_draft_80019EE8(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        else if (callback == 0x8006F124u)
            gtmain_frame_vblank();
        else if (callback)
            gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        previous = node;
        node = next;
    }
    native_frame_node_dispatch(previous, 0u);
    first_ticks = r_u32(0x800C0A88u);
    second_ticks = r_u32(0x800C0A8Cu);
    w_u32(0x800C0A88u, first_ticks + 1u);
    w_u32(0x800C0A8Cu, second_ticks + 1u);
    vblank_dispatching = 0u;
}

uint32 gtmain_frame_init(uint32 mode)
{
    if (r_u32(0x800B1A24u)) return 0xFFFFFFFFu;
    w_u32(0x800B1A24u, 1u);
    w_u32(0x800C0A88u, 0u);
    w_u32(0x800C0A8Cu, 0u);
    gtmain_graph_reset(mode);
    w_u32(0x800B1A20u, 0u);
    VSyncCallback(gtmain_vblank);
    return 0u;
}

uint32 gtmain_frame_events_init(void)
{
    uint32 i;
    for (i = 0u; i < 8u; ++i) {
        uint32 row = 0x800B1A2Cu + i * 12u;
        uint32 event = (uint32)OpenEventGuest(r_u32(row), r_u32(row + 4u), 0x1000u, r_u32(row + 8u));
        if (event != 0xFFFFFFFFu && !EnableEvent(event))
            gt_draft_unresolved_call(0x8007B734u, event, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u32(0x800C0A60u + i * 4u, event);
    }
    return 16u;
}

void gtmain_frame_callback_insert(uint32 node)
{
    if (native_frame_node && vblank_dispatching)
        gt_draft_failure("GTMAIN mixed VBlank list mutation", 0x8007A608u);
    EnterCriticalSectionPSX();
    uint32 previous = 0u;
    uint32 current = r_u32(0x800B1A20u);
    uint32 link = 0x800B1A20u;
    uint32 priority = r_u32(node + 12u);
    while (current) {
        if (priority < r_u32(current + 12u)) break;
        previous = current;
        current = r_u32(current + 4u);
    }
    if (native_frame_node && native_frame_node[0] == previous &&
        native_frame_node[1] == current) {
        if (priority < native_frame_node[3]) native_frame_node[0] = node;
        else native_frame_node[1] = node;
    }
    w_u32(node, previous);
    w_u32(node + 4u, current);
    if (previous) {
        link = previous + 4u;
        w_u32(previous + 4u, node);
    }
    if (current) w_u32(current, node);
    w_u32(link, node);
    ExitCriticalSection();
}


uint32 gtmain_display_init(uint32 flags)
{
    uint32 selected = flags & 7u;
    uint32 source = 0x800AD898u + selected * 20u;
    uint32 i;
    w_u32(0x800BFE70u, selected);
    w_u16(0x800BFE78u, 0u);
    w_u16(0x800BFE7Au, 0u);
    w_u32(0x800BFE74u, r_u16(source));
    for (i = 0u; i < 4u; ++i)
        w_u16(0x800BFE7Cu + i * 2u, r_u16(source + 2u + i * 2u));
    w_u16(0x800BFE84u, r_u16(source + 2u));
    w_u16(0x800BFE86u, r_u16(source + 4u));
    if (flags & 0x20u) w_u32(0x800B3874u, 1u);
    for (i = 0u; i < 2u; ++i) {
        uint32 rectangle = 0x800BFE78u + i * 8u;
        uint32 display = 0x800BFE88u + i * 20u;
        uint32 draw = 0x800BFEB0u + i * 92u;
        sint32 x = (sint16)r_u16(rectangle);
        sint32 y = (sint16)r_u16(rectangle + 2u);
        sint32 width = (sint16)r_u16(rectangle + 4u);
        sint32 height = (sint16)r_u16(rectangle + 6u);
        SetDefDispEnv((DISPENV *)psx_addr(display, sizeof(DISPENV)), x, y, width, height);
        SetDefDrawEnv((DRAWENV *)psx_addr(draw, sizeof(DRAWENV)), x, y, width, height);
        if (flags & 0x10u) w_u8(display + 17u, 1u);
        if (flags & 8u) w_u8(display + 16u, 1u);
        if (flags & 0x20u) w_u16(display + 10u, (selected == 7u || selected == 37u) ? 16u : 24u);
    }
    if (!(flags & 0x40u)) {
        w_u32(0x800BFF68u, 0u);
        w_u32(0x800BFF6Cu, 1u);
    }
    w_u32(0x800BFF74u, 1u);
    return 1u;
}
