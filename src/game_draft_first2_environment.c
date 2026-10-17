#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

static void projection_normalize(uint32 *scratch)
{
    gtmain_vector_normalize_xy((uint8 *)scratch, (const uint8 *)scratch);
}

static void stream_alternate_sync(uint8 status, uint8 *response)
{
    gtmain_stream_alternate_sync(status, response);
}

static void stream_alternate_ready(uint8 status, uint8 *response)
{
    /* TODO Bind the translated alternate stream ready callback */
    gt_draft_failure("GTMAIN alternate stream ready callback", 0x8006FDA4u);
}

static void stream_default_sync(uint8 status, uint8 *response)
{
    gtmain_cd_default_sync(status, response);
}

/* GTMAIN.EXE 8007A368 */
void gtmain_frame_display_apply(uint32 alternate)
{
    uint32 index = r_u32(alternate ? 0x800BFF6Cu : 0x800BFF68u);
    uint32 address = 0x800BFE88u + 20u * index;
    PutDispEnv((DISPENV *)psx_addr(address, sizeof(DISPENV)));
}

/* GTMAIN.EXE 8006F7DC */
void gtmain_frame_draw_apply(uint32 alternate)
{
    uint32 index = r_u32(alternate ? 0x800BFF6Cu : 0x800BFF68u);
    uint32 address = 0x800BFEB0u + 92u * index;
    PutDrawEnv((DRAWENV *)psx_addr(address, sizeof(DRAWENV)));
}

/* GTMAIN.EXE 8007A1D4 */
void gtmain_frame_environments_apply(void)
{
    gtmain_frame_display_apply(0u);
    gtmain_frame_draw_apply(1u);
}

/* GTMAIN.EXE 8006F838 */
sint32 gtmain_ordering_node_insert(uint32 node)
{
    uint32 used = r_u32(0x800C00ECu), count = r_u32(node + 12u);
    uint32 capacity = r_u32(0x800C00F0u), previous = 0u, current;
    sint32 priority;
    if (capacity < used + count) return 0;
    used = r_u32(0x800C00ECu);
    count = r_u32(node + 12u);
    current = r_u32(0x800C00E0u);
    w_u32(0x800C00ECu, used + count);
    if (current) {
        priority = (sint32)r_u32(node + 8u);
        do {
            if (priority < (sint32)r_u32(current + 8u)) break;
            previous = current;
            current = r_u32(current + 4u);
        } while (current);
    }
    if (previous) w_u32(previous + 4u, node);
    else w_u32(0x800C00E0u, node);
    if (current) w_u32(current, node);
    w_u32(node, previous);
    w_u32(node + 4u, current);
    gtmain_audio_buffers_relocate();
    return 1;
}

/* GTMAIN.EXE 8007A04C */
sint32 gtmain_projection_horizontal(uint32 context)
{
    uint32 scratch[8];
    sint32 left = (sint16)r_u16(context), right = (sint16)r_u16(context + 4u);
    sint32 center = (sint16)r_u16(context + 8u), margin = (sint16)r_u16(context + 14u);
    sint32 depth = (sint16)r_u16(context + 12u), first, second;
    right = right - center + margin;
    scratch[0] = (uint16)(left - center - margin) | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    w_u16(context + 78u, 0u);
    w_u16(context + 76u, (uint16)second);
    w_u16(context + 80u, (uint16)(0u - (uint32)first));
    scratch[0] = (uint16)right | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    w_u16(context + 84u, 0u);
    w_u16(context + 88u, 0u);
    w_u16(context + 90u, 0u);
    w_u16(context + 92u, 4096u);
    w_u16(context + 82u, (uint16)(0u - (uint32)second));
    w_u16(context + 86u, (uint16)first);
    return 4096;
}

/* GTMAIN.EXE 8007A110 */
sint32 gtmain_projection_vertical(uint32 context)
{
    uint32 scratch[8];
    sint32 top = (sint16)r_u16(context + 2u), bottom = (sint16)r_u16(context + 6u);
    sint32 center = (sint16)r_u16(context + 10u), margin = (sint16)r_u16(context + 16u);
    sint32 depth = (sint16)r_u16(context + 12u), first, second;
    bottom = bottom - center + margin;
    scratch[0] = (uint16)(top - center - margin) | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    w_u16(context + 96u, 0u);
    w_u16(context + 98u, (uint16)second);
    w_u16(context + 100u, (uint16)(0u - (uint32)first));
    scratch[0] = (uint16)bottom | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    w_u16(context + 102u, 0u);
    w_u16(context + 108u, 0u);
    w_u16(context + 110u, 0u);
    w_u16(context + 112u, 0xF000u);
    w_u16(context + 104u, (uint16)(0u - (uint32)second));
    w_u16(context + 106u, (uint16)first);
    return -4096;
}

/* GTMAIN.EXE 8007A314 */
uint32 gtmain_cd_directory_name(uint32 entry)
{
    return gtmain_packed_bytes_entry(r_u32(0x800C0C24u), entry) + 1u;
}

/* GTMAIN.EXE 8007A3BC */
void gtmain_pad_system_init(void)
{
    memset(psx_addr(0x800C0A90u, 96u), 0, 96u);
    PadInitDirectPSX(0x800C0AF0u, 0x800C0B30u);
    PadStartCom();
}

/* GTMAIN.EXE 8006F92C */
void gtmain_stream_callbacks_install(void)
{
    w_u8(0x800994C0u, 0u);
    CdSyncCallback(stream_alternate_sync);
    CdReadyCallback(stream_alternate_ready);
}

/* GTMAIN.EXE 8006F96C */
sint32 gtmain_stream_callbacks_restore(void)
{
    CdSyncCallback(stream_default_sync);
    CdReadyCallback(NULL);
    w_u8(0x800994C2u, 0u);
    return 0;
}

/* GTMAIN.EXE 8006F9A4 */
sint32 gtmain_stream_pause(void)
{
    gtmain_audio_output_scale(7u);
    return gtmain_cd_command(9u, 0u);
}

/* GTMAIN.EXE 8006F9D0 */
sint32 gtmain_stream_filter_apply(void)
{
    uint8 parameter[8];
    parameter[0] = r_u8(0x800994B8u);
    parameter[1] = r_u8(0x800994B9u);
    gtmain_audio_output_scale(0u);
    gtmain_cd_command_native(13u, parameter);
    w_u8(0x800994C0u, 0u);
    w_u8(0x800994C2u, 11u);
    return 11;
}
