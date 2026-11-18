#include "game_first2_registry.h"
#include "game_sound_stack.h"
#include <string.h>

static uint32 native_local_stack_top;

uint32 gt_local_stack_top(void)
{
    uint32 alternate = r_u32(0x800B1AA4u);
    return native_local_stack_top ? native_local_stack_top :
        alternate ? alternate : r_u32(0x8008F1D8u);
}

GT_LOCAL_STACK_FRAME gt_local_stack_push(uint32 bytes, uint32 owner)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 alternate = r_u32(0x800B1AA4u);
    uint32 baseline = alternate ? alternate : r_u32(0x8008F1D8u);
    if (bytes > 0x200000u - 7u)
        gt_draft_failure("GTMAIN local stack allocation extent", owner);
    bytes = (bytes + 7u) & ~7u;
    frame.previous = native_local_stack_top;
    frame.address = gt_local_stack_workspace(bytes, bytes, owner);
    if (frame.address < baseline - 0x41D8u)
        gt_draft_failure("GTMAIN local stack resident context overlap", owner);
    native_local_stack_top = frame.address;
    return frame;
}

GT_LOCAL_STACK_FRAME gtmain_cd_decode_stack_push(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 launch = r_u32(0x8008F1D8u);
    uint32 reserve = r_u32(0x800B1B4Cu);
    uint32 top = gt_local_stack_top(), low, address;
    uint32 heap = r_u32(0x800B1AACu), heap_bytes = r_u32(0x800B1AA8u);
    uint32 extent = heap_bytes < 16u ? 16u : heap_bytes;
    uint32 context = r_u32(0x800B3990u);
    const uint32 bytes = 0x8080u;
    if (launch < 0x80010000u || launch > 0x80200000u ||
        reserve > launch - 0x80010000u || top > launch || (top & 7u) ||
        top < 0x80014500u || heap < 0x80010000u || heap > 0x80200000u ||
        extent > 0x80200000u - heap ||
        (context && (context < 0x80010000u || context > 0x801FFFD8u)))
        gt_draft_failure("GTMAIN CD decode stack bounds", 0x80071CA4u);
    low = launch - reserve;
    /* Keep existing caller workspaces through offset 0x4480 above this frame */
    top -= 0x4500u;
    for (;;) {
        if (top < low || bytes > top - low)
            gt_draft_failure("GTMAIN CD decode stack capacity", 0x80071CA4u);
        address = top - bytes;
        if (heap < top && heap + extent > address) {
            top = heap & ~7u;
            continue;
        }
        if (context && context < top && context + 40u > address) {
            top = context & ~7u;
            continue;
        }
        break;
    }
    /* The original reserved stack aliases BSS during this decoder */
    frame.previous = native_local_stack_top;
    frame.address = address;
    native_local_stack_top = address;
    return frame;
}

void gt_local_stack_pop(GT_LOCAL_STACK_FRAME frame)
{
    if (native_local_stack_top != frame.address)
        gt_draft_failure("GTMAIN local stack allocation order", 0x80030DCCu);
    native_local_stack_top = frame.previous;
}

/* GTMAIN.EXE 80030DFC */
/* Original: GTMAIN.EXE:sub_80030DFC. */ uint32 gtmain_texture_clut_native(uint32 id) { return r_u32(0x800B49D4u) + id * 24u + 16u; }

/* GTMAIN.EXE 80030E1C */
/* Original: GTMAIN.EXE:sub_80030E1C. */ uint32 gtmain_texture_page_native(uint32 id) { return r_u32(0x800B49D4u) + id * 24u + 18u; }

/* GTMAIN.EXE 80030E3C */
/* Original: GTMAIN.EXE:sub_80030E3C. */ sint32 gtmain_texture_x_native(uint32 id)
{
    return r_s16(r_u32(0x800B49D4u) + id * 24u + 20u);
}

/* GTMAIN.EXE 80030E60 */
/* Original: GTMAIN.EXE:sub_80030E60. */ sint32 gtmain_texture_y_native(uint32 id)
{
    return r_s16(r_u32(0x800B49D4u) + id * 24u + 22u);
}

/* GTMAIN.EXE 80030DCC */
/* Original: GTMAIN.EXE:sub_80030DCC. */ void gtmain_texture_workspace_publish(uint32 workspace)
{
    w_u32(0x800B49D4u, workspace);
    w_u32(0x800B49D8u, 0u);
}

/* GTMAIN.EXE 80030E84 */
/* Original: GTMAIN.EXE:sub_80030E84. */ sint32 gtmain_texture_name_find_native(const uint8 *name)
{
    uint32 index = 0u;
    if ((sint32)r_u32(0x800B49D8u) <= 0) return -1;
    do {
        uint32 record = r_u32(0x800B49D4u) + index * 24u;
        if (!strcmp((const char *)psx_addr(record, 24u), (const char *)name))
            return (sint32)index;
        ++index;
    } while ((sint32)index < (sint32)r_u32(0x800B49D8u));
    return -1;
}

static void resource_upload(uint32 block)
{
    PSX_RECT rectangle;
    uint32 bytes;
    rectangle.x = (sint16)r_u16(block + 4u);
    rectangle.y = (sint16)r_u16(block + 6u);
    rectangle.w = (sint16)r_u16(block + 8u);
    rectangle.h = (sint16)r_u16(block + 10u);
    bytes = rectangle.w > 0 && rectangle.h > 0
        ? (uint32)rectangle.w * (uint32)rectangle.h * 2u : 1u;
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(block + 12u, bytes));
    DrawSync(0);
}

/* GTMAIN.EXE 80030F00 */
/* Original: GTMAIN.EXE:sub_80030F00. */ uint32 gtmain_texture_palette_load(uint32 record, uint32 block, uint32 image)
{
    uint32 mode, next;
    uint16 value;
    if (!(r_u32(image + 4u) & 8u)) {
        w_u16(record + 16u, 0);
        return block;
    }
    mode = r_u32(image + 4u) & 7u;
    if (mode == 0u || mode == 1u) resource_upload(block);
    value = (uint16)((r_u16(record + 16u) & 0xFFC0u) | ((r_u16(block + 4u) >> 4) & 0x3Fu));
    next = block + r_u32(block);
    w_u16(record + 16u, value);
    value = (uint16)((value & 0x803Fu) | ((r_u16(block + 6u) & 0x1FFu) << 6));
    w_u16(record + 16u, value);
    return next;
}

/* GTMAIN.EXE 80030FFC */
/* Original: GTMAIN.EXE:sub_80030FFC. */ uint32 gtmain_texture_pixels_load(uint32 record, uint32 block, uint32 image)
{
    uint32 mode = r_u32(image + 4u) & 7u;
    uint32 depth = mode == 1u ? 1u : mode == 2u ? 2u : 0u;
    uint16 value;
    resource_upload(block);
    value = (uint16)((r_u16(record + 18u) & 0xFFF0u) | ((r_u16(block + 4u) >> 6) & 0xFu));
    w_u16(record + 18u, value);
    value = (uint16)((value & 0xFFEFu) | ((r_u16(block + 6u) >> 4) & 0x10u));
    value = (uint16)((value & 0xFE7Fu) | (depth << 7));
    w_u16(record + 18u, value);
    w_u16(record + 20u, (uint16)((r_u16(block + 4u) & 0x3Fu) << (2u - depth)));
    w_u16(record + 22u, r_u16(block + 6u));
    return block + r_u32(block);
}

/* GTMAIN.EXE 80031120 */
/* Original: GTMAIN.EXE:sub_80031120. */ uint32 gtmain_texture_records_load(uint32 image)
{
    uint32 index = 0, result = r_u32(image);
    if (result) do {
        uint32 record = r_u32(0x800B49D4u) + 24u * index;
        uint32 section, pixels;
        gtmain_string_copy(record, image + 20u * index + 4u);
        section = image + r_u32(image + 20u * index + 20u);
        pixels = gtmain_texture_palette_load(record, section + 8u, section);
        gtmain_texture_pixels_load(record, pixels, section);
        ++index;
        result = index < r_u32(image);
    } while (result);
    w_u32(0x800B49D8u, index);
    return result;
}

/* GTMAIN.EXE 80031E5C */
/* Original: GTMAIN.EXE:sub_80031E5C. */ uint32 gtmain_race_sound_bank_load(uint32 destination, uint32 spu_base,
    const uint8 *table, uint32 sector, uint32 section_index)
{
    uint32 path = gtmain_sound_path();
    uint32 image = gtmain_archive_section_load(path, table, sector, section_index, 0x801BB70Cu, 0);
    gtmain_memory_copy(destination, image, r_u32(image + 8u));
    gtmain_sound_engine_relocate(image, spu_base);
    {
        uint32 relocated = gtmain_sound_engine_relocate(destination, spu_base);
        uint32 address = r_u32(destination + 12u);
        uint32 source = r_u32(image + 20u);
        uint32 size = r_u32(image + 16u);
        (void)gtmain_sound_sample_upload(address, source, size, relocated);
    }
    return destination;
}

/* GTMAIN.EXE 80031F18 */
/* Original: GTMAIN.EXE:sub_80031F18. */ void gtmain_race_sound_banks_update(void)
{
    uint32 selectors = gt_sound_stack_workspace(0x4160u, 4128u, 0x80031F18u);
    uint32 count = 3, changed = 0, index;
    uint32 temporary = selectors + 16u, sector_address = temporary + 4096u;
    uint32 table;
    {
        uint32 second = r_u16(0x800BB9D0u), first = r_u16(0x800BB9C6u);
        w_u32(selectors + 4u, second);
        w_u32(selectors, first);
    }
    if (r_u8(0x800CB7F5u) == 2u) {
        count = 4;
        uint32 third = r_u16(0x800BBB48u), fourth = r_u16(0x800BBB52u);
        w_u32(selectors + 8u, third);
        w_u32(selectors + 12u, fourth);
    } else {
        w_u32(selectors + 8u, 54u);
        w_u32(selectors + 12u, 0xFFFFFFFFu);
    }
    for (index = 0; index < 4u; ++index) {
        if (index < count && (sint32)(sint16)r_u16(0x800B52ECu + 2u * index) != (sint32)r_u32(selectors + 4u * index))
            changed = 1;
        w_u16(0x800B52ECu + 2u * index, r_u16(selectors + 4u * index));
    }
    if (changed) {
        uint32 path = gtmain_sound_path();
        uint32 destination_offset = 0, spu_base = 0x2C800u;
        table = gtmain_archive_table_load_guest(path, sector_address, temporary, 4096u);
        for (index = 0; index < count; ++index) {
            uint32 destination = 0x800B4EECu + destination_offset;
            uint32 loaded, header_size, sample_size, section, sector;
            w_u32(0x800B49DCu + 4u * index, destination);
            section = r_u32(selectors + 4u * index) + 3u;
            sector = r_u32(sector_address);
            loaded = gtmain_race_sound_bank_load(destination, spu_base,
                table ? (uint8 *)psx_addr(table, 1u) : NULL, sector, section);
            header_size = r_u32(loaded + 8u);
            sample_size = r_u32(loaded + 16u);
            spu_base += (sample_size + 15u) & ~15u;
            destination_offset += (header_size + 3u) & ~3u;
        }
    }
}
