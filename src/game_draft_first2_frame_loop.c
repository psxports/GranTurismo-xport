#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

static void frame_tables_publish(uint32 *first, uint32 *second, uint32 count)
{
    uintptr_t base = (uintptr_t)DRAM, a = (uintptr_t)first, b = (uintptr_t)second;
    uint32 bytes;
    /* TODO Bind ordering arrays allocated outside actual DRAM */
    if (count > PSX_DRAM_SIZE / 4u || a < base || b < base)
        gt_draft_failure("GTMAIN native ordering buffer outside DRAM", 0x80078A90u);
    bytes = count * 4u;
    if (a - base > PSX_DRAM_SIZE - bytes || b - base > PSX_DRAM_SIZE - bytes)
        gt_draft_failure("GTMAIN native ordering buffer extent", 0x80078A90u);
    gtmain_audio_buffers_init(0x80000000u + (uint32)(a - base),
        0x80000000u + (uint32)(b - base), count);
}

static uint8 *native_frame_context;

uint8 *gtmain_frame_context_get(void)
{
    if (native_frame_context) return native_frame_context;
    return (uint8 *)psx_addr(r_u32(0x800C0098u), 40u);
}

static void frame_context_publish(uint8 *context)
{
    /* TODO Bind nested native frame contexts */
    if (!context || native_frame_context)
        gt_draft_failure("GTMAIN native frame context lifetime", 0x800C0098u);
    native_frame_context = context;
}

static void frame_event_install(uint32 *event)
{
    gtmain_native_frame_callback_insert(event);
}

static void frame_event_remove(uint32 *event)
{
    gtmain_native_frame_callback_remove(event);
}

static uint32 frame_context_word(const uint8 *context, uint32 offset)
{
    uint32 value;
    memcpy(&value, context + offset, sizeof(value));
    return value;
}

static sint32 frame_callback(uint32 token, uint32 argument)
{
    if (!token || token == 0x8006F11Cu) return 0;
    if (token == 0x80028968u) return gtmain_race_loading_tick();
    if (token == 0x80028984u) return gtmain_race_loading_done();
    if (token == 0x800288F4u) { gtmain_race_loading_draw(); return 0; }
    if (token == 0x8002B5A4u) { gtmain_race_ordering_reset(); return 0; }
    if (token == 0x8002B5D8u) return (sint32)gtmain_race_frame_finish();
    if (token == 0x8002B328u) { gtmain_race_frame_tick(); return 0; }
    if (token == 0x8002B44Cu) return gtmain_race_frame_status();
    if (token == 0x8002BA60u) { gtmain_race_frame_draw(); return 0; }
    if (token == 0x80035D90u) return gtmain_menu_overlay_tick();
    if (token == 0x80038304u) return (sint32)gtmain_menu_end_panel_tick();
    if (token == 0x80036350u) return gtmain_menu_race_selection_tick();
    if (token == 0x800336D8u) return gtmain_menu_name_commit_tick();
    if (token == 0x80034470u) return gtmain_menu_reward_tick();
    if (token == 0x800325C4u) return gtmain_menu_result_list_tick();
    /* TODO Resolve remaining native callback identities and their typed arguments */
    gt_draft_failure("GTMAIN native frame callback", token);
}

/* GTMAIN.EXE 8006A21C */
uint8 *gtmain_resource_read_retry(uint32 path, uint8 *buffer, uint32 capacity, uint32 decoded_bytes)
{
    for (;;) {
        uint8 *result = gtmain_cd_file_load_native(path, buffer, capacity, capacity, decoded_bytes);
        if (result || r_u32(0x800B1AB0u) == 12u) return result;
        gtmain_cd_file_retry_hook();
    }
}

/* GTMAIN.EXE 80078914 */
void gtmain_font_texture_upload(uint32 destination, sint32 page, const uint8 *data)
{
    uint32 pixels[8192], value;
    uint32 bits = (uint32)page, x = (bits & 15u) << 6, y = (bits >> 4) << 8;
    PSX_RECT rectangle = {(sint16)x, (sint16)y, 64, 256};
    uint8 *decoded;
    DrawSync(0);
    decoded = gtmain_lz_decode((uint8 *)pixels, sizeof(pixels), data, 32768u, 32768);
    LoadImagePSX(&rectangle, (uint32 *)decoded);
    memset(psx_addr(destination, 32u), 0, 32u);
    w_u16(destination + 8u, (uint16)((x >> 4) | (y << 6)));
    value = r_u32(destination + 8u);
    w_u8(destination + 15u, 100u);
    value = (value & 0xFFE0FFFFu) | ((bits & 31u) << 16);
    value = ((value & 0xFF9FFFFFu) | 0x200000u) & 0xFE7FFFFFu;
    w_u32(destination + 8u, value);
    w_u8(destination + 12u, 128u);
    w_u8(destination + 13u, 128u);
    w_u8(destination + 14u, 128u);
    DrawSync(0);
}

/* GTMAIN.EXE 8006F078 */
void gtmain_frame_tables_bind(uint32 *first, uint32 *second, uint32 count)
{
    frame_tables_publish(first, second, count);
    gtmain_frame_tables_clear(2);
}

/* GTMAIN.EXE 80070240 */
void gtmain_ordering_clear(uint32 index)
{
    uint32 table = r_u32(0x800C00E4u + 4u * index);
    uint32 count = r_u32(0x800C00F0u), node;
    ClearOTagR((uint32 *)psx_addr(table, count * 4u), (sint32)count);
    node = r_u32(0x800C00E0u);
    while (node) {
        w_u32(node + 24u, r_u32(node + 16u + 4u * index));
        node = r_u32(node + 4u);
    }
}

/* GTMAIN.EXE 8006C410 */
void gtmain_frame_tables_clear(sint32 selection)
{
    if (selection == 0) gtmain_ordering_clear(r_u32(0x800BFF68u));
    else if (selection == 1) gtmain_ordering_clear(r_u32(0x800BFF6Cu));
    else if (selection == 2) { gtmain_ordering_clear(0u); gtmain_ordering_clear(1u); }
}

/* GTMAIN.EXE 800702F8 */
void gtmain_ordering_submit(uint32 index)
{
    uint32 link = gtmain_audio_buffer_end(index);
    DrawOTag((uint32 *)psx_addr(link, 4u));
}

/* GTMAIN.EXE 80078B60 */
void gtmain_frame_submit(uint32 alternate)
{
    gtmain_ordering_submit(r_u32(alternate ? 0x800BFF6Cu : 0x800BFF68u));
}

/* GTMAIN.EXE 80078BF0 */
void gtmain_frame_links_reset(uint32 alternate)
{
    gtmain_packet_buffer_select(r_u32(alternate ? 0x800BFF6Cu : 0x800BFF68u));
}

/* GTMAIN.EXE 8006F900 */
void gtmain_frame_prepare(uint32 alternate)
{
    gtmain_frame_tables_clear((sint32)alternate);
    gtmain_frame_links_reset(alternate);
}

/* GTMAIN.EXE 80078AB0 */
void gtmain_frame_display_begin(void)
{
    ResetGraph(1);
    gtmain_frame_environments_apply();
}

/* GTMAIN.EXE 80078AD8 */
void gtmain_frame_swap(uint32 wait_mode)
{
    uint32 second = r_u32(0x800BFF6Cu), first = r_u32(0x800BFF68u);
    w_u32(0x800BFF68u, second);
    w_u32(0x800BFF6Cu, first);
    if (wait_mode == 1u || wait_mode == 2u) {
        if (wait_mode == 2u) DrawSync(0);
        /* TODO Bind the unselected frame wait service */
        gt_draft_unresolved_call(0x8007A1FCu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    gtmain_frame_environments_apply();
    if (!r_u32(0x800BFF70u)) gtmain_display_mask(1u);
}

/* GTMAIN.EXE 8006F174 */
sint32 gtmain_frame_run(uint8 *context)
{
    uint32 event[4], callback = frame_context_word(context, 0u);
    sint32 result;
    frame_context_publish(context);
    w_u32(0x800C0094u, 0u);
    w_u32(0x800C0090u, callback ? callback : 0x8006F11Cu);
    w_u32(0x800C009Cu, 0u);
    event[2] = 0x8006F124u;
    event[3] = 0u;
    frame_event_install(event);
    VSync(0);
    gtmain_frame_display_begin();
    for (;;) {
        frame_callback(frame_context_word(context, 4u), 0u);
        if (!context[38]) gtmain_frame_prepare(0u);
        frame_callback(frame_context_word(context, 8u), 0u);
        callback = frame_context_word(context, 12u);
        if (callback) { result = frame_callback(callback, 0u); if (result) break; }
        frame_callback(frame_context_word(context, 16u), 0u);
        if (!context[36]) DrawSync(0);
        frame_callback(frame_context_word(context, 20u), 0u);
        {
            sint32 required = (sint32)r_u32(0x800BFF74u);
            while ((sint32)r_u32(0x800C009Cu) < required) VSync(0);
        }
        if (context[37]) {
            w_u32(0x800C009Cu, 0u);
            while (!r_u32(0x800C009Cu)) {
                frame_callback(frame_context_word(context, 24u), 0u);
                VSync(0);
            }
        }
        w_u32(0x800C009Cu, 0u);
        if (context[36]) ResetGraph(1);
        frame_callback(frame_context_word(context, 28u), 0u);
        gtmain_frame_swap(0u);
        frame_callback(frame_context_word(context, 32u), 0u);
        if (!context[38]) gtmain_frame_submit(1u);
    }
    DrawSync(0);
    frame_event_remove(event);
    native_frame_context = NULL;
    return result;
}
