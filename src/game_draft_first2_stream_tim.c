#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_spu.h"
#include <stdint.h>

static void stream_playlist_shuffle(uint32 data, uint32 count)
{
    gtmain_stream_playlist_shuffle(data, (sint32)count);
}

static uint32 texture_source_extent(uint32 resource)
{
    uintptr_t source = (uintptr_t)psx_addr(resource + 16u, 1u);
    uintptr_t dram = (uintptr_t)psx_addr(0x80000000u, 0x200000u);
    uintptr_t scratch = (uintptr_t)psx_addr(0x1F800000u, 0x400u);
    if (source >= dram && source - dram < 0x200000u)
        return 0x200000u - (uint32)(source - dram);
    if (source >= scratch && source - scratch < 0x400u)
        return 0x400u - (uint32)(source - scratch);
    /* TODO Bind nonmapped native resource ownership and readable extent */
    gt_draft_failure("GTMAIN texture mapped source span unavailable", 0x80070598u);
    return 0u;
}

uint8 *gtmain_texture_decode_native(uint8 *destination, uint32 capacity, uint32 resource)
{
    uint32 bytes = r_u32(resource + 12u), source_capacity;
    if (!destination)
        gt_draft_failure("Native texture destination unavailable", 0x80070598u);
    if ((sint32)bytes <= 0)
        return gtmain_lz_decode(destination, capacity,
            (const uint8 *)(uintptr_t)(resource + 16u), 0u, (sint32)bytes);
    source_capacity = texture_source_extent(resource);
    return gtmain_lz_decode(destination, capacity, psx_addr(resource + 16u, source_capacity),
        source_capacity, (sint32)bytes);
}

uint32 gtmain_tim_block_upload_native(const uint8 *block, uint32 capacity, sint32 xy, sint32 wh)
{
    uint32 packed_xy = (uint32)xy, packed_wh = (uint32)wh, high, low, bytes;
    PSX_RECT rectangle;
    if (!block || capacity < 12u)
        gt_draft_failure("Native TIM block header extent", 0x80070500u);
    if (xy == -1) {
        high = xport_load_le16(block + 6u); low = xport_load_le16(block + 4u);
        packed_xy = low | (high << 16);
    }
    if (wh == -1) {
        high = xport_load_le16(block + 10u); low = xport_load_le16(block + 8u);
        packed_wh = low | (high << 16);
    }
    rectangle.x = (sint16)packed_xy; rectangle.y = (sint16)(packed_xy >> 16);
    rectangle.w = (sint16)packed_wh; rectangle.h = (sint16)(packed_wh >> 16);
    if (rectangle.w < 0 || rectangle.h < 0)
        gt_draft_failure("Native TIM negative transfer extent", 0x80070500u);
    bytes = (uint32)rectangle.w * (uint32)rectangle.h * 2u;
    if (bytes > capacity - 12u)
        gt_draft_failure("Native TIM pixel extent", 0x80070500u);
    DrawSync(0);
    LoadImagePSX(&rectangle, (uint32 *)(block + 12u));
    DrawSync(0);
    return packed_xy;
}

uint32 gtmain_tim_upload_native(const uint8 *image, uint32 capacity, sint32 first_xy,
    sint32 first_wh, sint32 second_xy, sint32 second_wh, uint32 reverse_order)
{
    uint32 offset;
    const uint8 *block;
    if (!image || capacity < 12u)
        gt_draft_failure("Native TIM image header extent", 0x800705F4u);
    block = image + 8u;
    if (!(xport_load_le32(image + 4u) & 8u))
        return gtmain_tim_block_upload_native(block, capacity - 8u, second_xy, -1);
    offset = 8u + xport_load_le32(block);
    if (offset < 8u || offset > capacity)
        gt_draft_failure("Native TIM second block extent", 0x800705F4u);
    if (reverse_order) {
        gtmain_tim_block_upload_native(image + offset, capacity - offset, second_xy, second_wh);
        return gtmain_tim_block_upload_native(block, capacity - 8u, first_xy, first_wh);
    }
    gtmain_tim_block_upload_native(block, capacity - 8u, first_xy, first_wh);
    return gtmain_tim_block_upload_native(image + offset, capacity - offset, second_xy, second_wh);
}

/* GTMAIN.EXE 80070024 */
/* Original: GTMAIN.EXE:sub_80070024. */ sint32 gtmain_stream_entry_find(uint32 index)
{
    return gtmain_stream_name_find(r_u32(0x800994CCu + 4u * index));
}

/* GTMAIN.EXE 80070190 */
/* Original: GTMAIN.EXE:sub_80070190. */ sint32 gtmain_stream_mode_set(uint8 mode)
{
    uint32 changed = (r_u8(0x800994C8u) ^ mode) & 128u;
    w_u8(0x800994C8u, mode);
    if (changed) {
        CdControl(14u, (uint8 *)psx_addr(0x800994C8u, 1u), NULL);
        VSync(0);
        return VSync(3);
    }
    return 0;
}

/* GTMAIN.EXE 80070054 */
/* Original: GTMAIN.EXE:sub_80070054. */ sint32 gtmain_stream_entry_prepare(uint32 entry, uint32 parameter)
{
    uint32 record, first, base, bytes, end;
    gtmain_stream_mode_set(0u);
    record = r_u32(0x800C0C28u) + 8u * entry;
    first = r_u32(record);
    base = r_u32(0x800994A4u);
    bytes = r_u32(record + 4u) << 11;
    first += base;
    w_u32(0x800994A8u, first);
    w_u32(0x800994ACu, bytes);
    end = gtmain_cd_sector_end(first, bytes);
    gtmain_cd_mode(0u);
    gtmain_cd_filter(1u, (uint8)(entry & 3u));
    gtmain_cd_parameter((uint16)parameter);
    gtmain_cd_buffers(first, end, 3u);
    w_u32(0x800C00A0u, first);
    return 1;
}

/* GTMAIN.EXE 80070118 */
/* Original: GTMAIN.EXE:sub_80070118. */ sint32 gtmain_stream_begin_if_loaded(void)
{
    return r_u32(0x800994A8u) ? gtmain_stream_begin() : 0;
}

/* GTMAIN.EXE 80070144 */
/* Original: GTMAIN.EXE:sub_80070144. */ uint32 gtmain_stream_playlist_next(uint32 playlist)
{
    uint32 index = r_u8(playlist);
    uint32 count = r_u8(playlist + 1u), entry;
    ++index;
    entry = playlist + index;
    if (index == count) {
        stream_playlist_shuffle(playlist + 2u, index);
        index = 0u;
        entry = playlist;
    }
    w_u8(playlist, index);
    return r_u8(entry + 2u);
}

/* GTMAIN.EXE 800701E4 */
/* Original: GTMAIN.EXE:sub_800701E4. */ sint32 gtmain_stream_stop_wait(uint32 wait)
{
    sint32 result = gtmain_cd_stop();
    if (wait) do { result = gtmain_cd_stop_status(0u); } while (result);
    return result;
}

/* GTMAIN.EXE 80070220 */
/* Original: GTMAIN.EXE:sub_80070220. */ sint32 gtmain_stream_stop_async(void)
{
    return gtmain_cd_stop_status(1u);
}

/* GTMAIN.EXE 8006FF54 */
/* Original: GTMAIN.EXE:sub_8006FF54. */ sint32 gtmain_stream_pause_state(void)
{
    uint32 state, request;
    sint32 pending = gtmain_cd_pending_command();
    if (pending) return pending;
    EnterCriticalSectionPSX();
    state = r_u8(0x800994C2u);
    if (gtmain_stream_state_active(state)) {
        if (!state) {
            gtmain_stream_callbacks_install();
            gtmain_stream_filter_apply();
            request = 0u;
        } else if (state >= 15u && state < 17u) request = 0u;
        else { gtmain_cd_stop_status(1u); request = 1u; }
        w_u8(0x800994BFu, request);
    }
    ExitCriticalSection();
    return 0;
}

/* GTMAIN.EXE 8006FFF8 */
/* Original: GTMAIN.EXE:sub_8006FFF8. */ sint32 gtmain_stream_pause_if_loaded(void)
{
    return r_u32(0x800994A8u) ? gtmain_stream_pause_state() : 0;
}

/* GTMAIN.EXE 80070500 */
/* Original: GTMAIN.EXE:sub_80070500. */ uint32 gtmain_tim_block_upload(uint32 block, sint32 xy, sint32 wh)
{
    uint32 packed_xy = (uint32)xy, packed_wh = (uint32)wh;
    PSX_RECT rectangle;
    if (xy == -1) {
        uint32 high = r_u16(block + 6u), low = r_u16(block + 4u);
        packed_xy = low | (high << 16);
    }
    if (wh == -1) {
        uint32 high = r_u16(block + 10u), low = r_u16(block + 8u);
        packed_wh = low | (high << 16);
    }
    rectangle.x = (sint16)packed_xy;
    rectangle.y = (sint16)(packed_xy >> 16);
    rectangle.w = (sint16)packed_wh;
    rectangle.h = (sint16)(packed_wh >> 16);
    DrawSync(0);
    {
        uint32 bytes = rectangle.w > 0 && rectangle.h > 0
            ? (uint32)rectangle.w * (uint32)rectangle.h * 2u : 1u;
        LoadImagePSX(&rectangle, (uint32 *)psx_addr(block + 12u, bytes));
    }
    DrawSync(0);
    return packed_xy;
}

/* GTMAIN.EXE 800705F4 */
/* Original: GTMAIN.EXE:sub_800705F4. */ uint32 gtmain_tim_upload(uint32 image, sint32 first_xy, sint32 first_wh,
    sint32 second_xy, sint32 second_wh, uint32 reverse_order)
{
    uint32 block = image + 8u, palette;
    if (!(r_u32(image + 4u) & 8u)) return gtmain_tim_block_upload(block, second_xy, -1);
    palette = block + r_u32(block);
    if (reverse_order) {
        gtmain_tim_block_upload(palette, second_xy, second_wh);
        return gtmain_tim_block_upload(block, first_xy, first_wh);
    }
    gtmain_tim_block_upload(block, first_xy, first_wh);
    return gtmain_tim_block_upload(palette, second_xy, second_wh);
}

/* GTMAIN.EXE 80070598 */
/* Original: GTMAIN.EXE:sub_80070598. */ uint32 gtmain_texture_decode(uint32 destination, uint32 resource)
{
    uint32 bytes = r_u32(resource + 12u), source_capacity;
    if (!destination) {
        destination = gtmain_heap_alloc(bytes);
        if (!destination) return 0u;
    }
    if ((sint32)bytes <= 0)
        return (uint32)(uintptr_t)gtmain_lz_decode((uint8 *)(uintptr_t)destination, 0u,
            (const uint8 *)(uintptr_t)(resource + 16u), 0u, (sint32)bytes);
    source_capacity = texture_source_extent(resource);
    gtmain_lz_decode((uint8 *)psx_addr(destination, bytes), bytes,
        (const uint8 *)psx_addr(resource + 16u, source_capacity), source_capacity, (sint32)bytes);
    return destination;
}
